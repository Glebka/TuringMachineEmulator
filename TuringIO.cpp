#include <TuringIO.h>

QString TuringIO::m_last_error;
QString TuringIO::m_project_file;
QString TuringIO::m_fs_file;
QString TuringIO::m_tape_file;

bool TuringIO::loadFunctionalSchemeFromFile(FunctionalSchemeModel *model, QString fileName)
{
    QFile fs_file(fileName);
    QTextStream stream;
    QString header;
    if (!fs_file.open(QFile::ReadOnly | QFile::Text))
        goto error_open;
    TuringIO::m_fs_file=fileName;
    stream.setDevice(&fs_file);
    header=stream.readLine(100);
    stream.seek(0);
    if(header.contains("<?xml"))
        return functionalSchemeFromXML(model,fs_file);
    else
        if(header=="MTFs")
            return functionalSchemeFromText(model,fs_file);
        else
            goto error_format;
    error_open:
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл функциональной схемы %1.\n%2")
                .arg(fileName)
                .arg(fs_file.errorString());
        return false;
    error_format:
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл функциональной схемы. Формат файла %1 не поддерживается.")
                .arg(fileName);
        return false;
}

bool TuringIO::loadTapeFromFile(TapeModel *model, QString fileName)
{
    QFile tape_file(fileName);
    QTextStream stream;
    QString header;
    if(!tape_file.open(QFile::ReadOnly|QFile::Text))
        goto error_open;
    TuringIO::m_tape_file=fileName;
    stream.setDevice(&tape_file);
    header=stream.readLine(100);
    stream.seek(0);
    if(header.contains("<?xml"))
        return tapeFromXML(model,tape_file);
    else
        if(header=="MTLine")
            return tapeFromText(model,tape_file);
        else
            goto error_format;
    error_open:
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл ленты %1.\n%2").arg(fileName).arg(tape_file.errorString());
        return false;
    error_format:
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл ленты. Формат файла %1 не поддерживается.").arg(fileName);
        return false;
}

bool TuringIO::loadMachineFromFile(FunctionalSchemeModel *fs_model, TapeModel *tape_model, QString fileName)
{
    QFile machine_file(fileName);
    QTextStream stream;
    QString header;
    if(!machine_file.open(QFile::ReadOnly|QFile::Text))
        goto error_open;
    TuringIO::m_project_file=fileName;
    stream.setDevice(&machine_file);
    header=stream.readLine(100);
    stream.seek(0);
    if(header.contains("<?xml"))
        return machineFromXML(fs_model,tape_model,machine_file);
    else
        if(header=="MTProj")
            return machineFromText(fs_model,tape_model,machine_file);
        else
            goto error_format;
    error_open:
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл проекта %1.\n%2")
                .arg(fileName)
                .arg(machine_file.errorString());
        return false;
    error_format:
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл проекта. Формат файла %1 не поддерживается.")
                .arg(fileName);
        return false;
}

bool TuringIO::saveFunctionalSchemeToFile(FunctionalSchemeModel *model, QString fileName, TuringIO::FileFormat format)
{
    if(fileName.isEmpty())
        fileName=TuringIO::m_fs_file;
    if(!fileName.endsWith(".mts"))
        fileName.append(".mts");
    QTemporaryFile fs_file;
    bool result=false;
    if(!fs_file.open())
        goto error;
    TuringIO::m_fs_file=fileName;
    switch(format)
    {
    case TuringIO::Compatibility_v1:
    case TuringIO::Compatibility_v2:
        result=functionalSchemeToXml(model,fs_file);
        break;
    case TuringIO::Default:
    default:
        result=functionalSchemeToText(model,fs_file);
        break;
    }
    if(!result)
        goto error;
    if(QFile::exists(fileName))
    {
        if(!QFile::rename(fileName,fileName+".bkp"))
            goto error;
    }
    if(!fs_file.copy(fileName))
        goto error;
    QFile::remove(fileName+".bkp");
    return result;
    error:
        QFile::rename(fileName+".bkp",fileName);
    return false;
}

bool TuringIO::saveTapeToFile(TapeModel *model, QString fileName, TuringIO::FileFormat format)
{
    if(fileName.isEmpty())
        fileName=TuringIO::m_tape_file;
    if(!fileName.endsWith(".mtl"))
        fileName.append(".mtl");
    QTemporaryFile tape_file;
    bool result=false;
    if(!tape_file.open())
        goto error;
    TuringIO::m_tape_file=fileName;
    switch(format)
    {
    case TuringIO::Compatibility_v2:
        result=tapeToXml(model,tape_file);
        break;
    case TuringIO::Compatibility_v1:
    case TuringIO::Default:
    default:
        result=tapeToText(model,tape_file);
        break;
    }
    if(!result)
        goto error;
    if(QFile::exists(fileName))
    {
        if(!QFile::rename(fileName,fileName+".bkp"))
            goto error;
    }
    if(!tape_file.copy(fileName))
        goto error;
    QFile::remove(fileName+".bkp");
    return result;
    error:
        QFile::rename(fileName+".bkp",fileName);
    return false;
}

bool TuringIO::saveProjectFile(QString fileName,QString fsFile,QString tapeFile, FileFormat format)
{
    if(!fileName.endsWith(".mtp"))
        fileName.append(".mtp");
    QTemporaryFile project_file;
    QTextStream stream;
    QXmlStreamWriter writer;
    QFileInfo info(fileName);
    if(fsFile.isEmpty())
        fsFile=info.absoluteDir().absoluteFilePath(info.baseName().append(".mts"));
    if(tapeFile.isEmpty())
        tapeFile=info.absoluteDir().absoluteFilePath(info.baseName().append(".mtl"));
    if(!project_file.open())
        goto error;
    TuringIO::m_project_file=fileName;
    TuringIO::m_fs_file=info.absoluteDir().absoluteFilePath(fsFile);
    TuringIO::m_tape_file=info.absoluteDir().absoluteFilePath(tapeFile);
    switch(format)
    {
    case TuringIO::Compatibility_v2:
        writer.setDevice(&project_file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("TuringMachine");
        writer.writeAttribute("name",info.baseName());
        writer.writeStartElement("FunctionalScheme");
        writer.writeAttribute("path",info.absoluteDir().relativeFilePath(fsFile));
        writer.writeEndElement();
        writer.writeStartElement("Tape");
        writer.writeAttribute("path",info.absoluteDir().relativeFilePath(tapeFile));
        writer.writeEndElement();
        writer.writeEndElement();
        writer.writeEndDocument();
        if(writer.hasError())
            goto error;
        break;
    case TuringIO::Default:
    case TuringIO::Compatibility_v1:
    default:
        stream.setDevice(&project_file);
        stream<<"MTProj"<<endl;
        stream<<info.absoluteDir().relativeFilePath(fsFile)<<endl;
        stream<<info.absoluteDir().relativeFilePath(tapeFile);
        if(stream.status()==QTextStream::WriteFailed)
            goto error;
        break;
    }
    if(info.exists())
    {
        if(!QFile::rename(fileName,fileName+".bkp"))
            goto error_bkp;
    }
    if(!project_file.copy(fileName))
        goto error_bkp;
    QFile::remove(fileName+".bkp");
    return true;
    error:
        TuringIO::m_last_error=QObject::tr("Ошибка при сохранении файла проекта %1.\n%2")
                .arg(fileName)
                .arg(project_file.errorString());
        return false;
    error_bkp:
        TuringIO::m_last_error=QObject::tr("Ошибка при сохранении файла проекта %1.").arg(fileName);
        QFile::rename(fileName+".bkp",fileName);
        return false;
}

bool TuringIO::tapeFromXML(TapeModel *model, QFile &file)
{
    QDomDocument doc;
    doc.setContent(&file);
    QDomElement tape=doc.elementsByTagName("Tape").at(0).toElement();
    QString emptyChar;
    if(tape.isNull())
        goto error_format;
    model->setString(tape.attribute("string"));
    model->setStartPos(atoi(tape.attribute("start","0").toStdString().c_str()));
    emptyChar=tape.attribute("EmptyChar");
    if(!emptyChar.isEmpty())
        model->setEmptyChar(emptyChar.at(0));
    return true;
    error_format:
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл ленты. Формат файла %1 не поддерживается.")
                .arg(file.fileName());
        return false;
}

bool TuringIO::tapeFromText(TapeModel *model, QFile &file)
{
    QTextStream stream(&file);
    stream.readLine();
    if(stream.atEnd())
        goto error_format;
    model->setString(stream.readLine(1024*1204));
    if(stream.atEnd())
        goto error_format;
    model->setStartPos(atoi(stream.readLine(10).toStdString().c_str()));
    return true;
    error_format:
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл ленты. Формат файла %1 не поддерживается.")
                .arg(file.fileName());
        return false;
}

bool TuringIO::functionalSchemeFromText(FunctionalSchemeModel *model, QFile &file)
{
    int c=0,r=0,col=0;
    QTextStream stream(&file);
    QString header;
    QString temp="";
    QString command="";
    QTextStream header_stream(&header,QIODevice::ReadOnly);
    stream.readLine();
    if(stream.atEnd())
        goto error_format;
    header=stream.readLine(1024);
    while(!header_stream.atEnd())
    {
        header_stream>>temp;
        if(temp.isEmpty())
            return false;
        model->appendCharColumn(temp.at(0));
        col++;
    }

    while(!stream.atEnd())
    {
        stream>>temp;
        command.append(temp);
        command.append(' ');
        stream>>temp;
        command.append(temp);
        command.append(' ');
        stream>>temp;
        command.append(temp);
        if(c==0 && !stream.atEnd())
            model->insertRow(r,QModelIndex());
        model->setData(model->index(r,c),command,Qt::EditRole);
        c++;
        command.clear();
        if(c==col)
        {
            c=0;
            r++;
        }
    }
    return true;
    error_format:
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл функциональной схемы. Формат файла %1 не поддерживается.")
                .arg(file.fileName());
        return false;
}

bool TuringIO::functionalSchemeFromXML(FunctionalSchemeModel *model, QFile &file)
{
    FunctionalSchemeReader handler(model);
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);
    QXmlInputSource xmlInputSource(&file);
    if (!reader.parse(xmlInputSource))
    {
        TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(file.fileName());
        return false;
    }
    return true;
}

bool TuringIO::machineFromText(FunctionalSchemeModel *fs_model, TapeModel *tape_model, QFile &file)
{
    QTextStream stream(&file);
    QFileInfo file_info(file);
    QString fs_filename,tape_filename;
    stream.readLine();
    if(stream.atEnd())
        goto error_format;
    fs_filename=stream.readLine();
    tape_filename=stream.readLine();
    if(fs_filename.isEmpty()||tape_filename.isEmpty())
        goto error_format;
    if(fs_filename.startsWith('\\'))
        fs_filename=fs_filename.mid(1);
    if(tape_filename.startsWith('\\'))
        tape_filename=tape_filename.mid(1);

    return (
                loadFunctionalSchemeFromFile(
                    fs_model,
                    file_info.absoluteDir().absoluteFilePath(fs_filename)
                )
            &&
                loadTapeFromFile(
                    tape_model,
                    file_info.absoluteDir().absoluteFilePath(tape_filename)
                )
           );
    error_format:
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл проекта. Формат файла %1 не поддерживается.")
                .arg(file.fileName());
        return false;
}

bool TuringIO::machineFromXML(FunctionalSchemeModel *fs_model, TapeModel *tape_model, QFile &file)
{
    QDomDocument doc;
    doc.setContent(&file);
    QDomElement machine=doc.elementsByTagName("TuringMachine").at(0).toElement();
    if(machine.isNull())
    {
        TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(file.fileName());
        return false;
    }
    if(machine.elementsByTagName("FunctionalScheme").isEmpty())
    {
        TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(file.fileName());
        return false;
    }
    QDomElement fs=machine.elementsByTagName("FunctionalScheme").at(0).toElement();
    QDomElement tape=machine.elementsByTagName("Tape").at(0).toElement();
    if(fs.isNull()||tape.isNull())
    {
        TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(file.fileName());
        return false;
    }
    QString fs_filename=fs.attribute("path");
    QString tape_filename=tape.attribute("path");
    if(fs_filename.isEmpty()||tape_filename.isEmpty())
    {
        TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(file.fileName());
        return false;
    }
    if(fs_filename.startsWith('\\'))
        fs_filename=fs_filename.mid(1);
    if(tape_filename.startsWith('\\'))
        tape_filename=tape_filename.mid(1);
    QFileInfo file_info(file);
    return (
                loadFunctionalSchemeFromFile(
                    fs_model,
                    file_info.absoluteDir().absoluteFilePath(fs_filename)
                )
            &&
                loadTapeFromFile(
                    tape_model,
                    file_info.absoluteDir().absoluteFilePath(tape_filename)
                )
                );
}

bool TuringIO::functionalSchemeToXml(FunctionalSchemeModel *model, QFile & file)
{
    QXmlStreamWriter writer;
    writer.setDevice(&file);
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(1);
    writer.writeStartDocument();
    writer.writeStartElement("script");
    writer.writeStartElement("table");
    if(model->columnCount()>0)
    {
        writer.writeStartElement("trh");
        for(int i=0;i<model->columnCount();i++)
        {
            writer.writeStartElement("th");
            writer.writeCharacters(model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
    if(model->rowCount()>0)
    {
        for(int i=0;i<model->rowCount();i++)
        {
            writer.writeStartElement("tr");
            for(int j=0;j<model->columnCount();j++)
            {
                writer.writeStartElement("td");
                writer.writeCharacters(model->data(model->index(i,j),Qt::DisplayRole).toString());
                writer.writeEndElement();
            }
            writer.writeEndElement();
            if(writer.hasError())
                goto error;
        }
    }
    writer.writeEndDocument();
    if(writer.hasError())
        goto error;
    return true;
    error:
        TuringIO::m_last_error=QObject::tr("Не удалось сохранить файл функциональной схемы %1.\n%2")
                .arg(file.fileName())
                .arg(file.errorString());
        return false;
}

bool TuringIO::functionalSchemeToText(FunctionalSchemeModel *model, QFile &file)
{
    QTextStream stream;
    QStringList line;
    stream.setDevice(&file);
    stream<<"MTFs"<<endl;
    for(int i=0;i<model->columnCount();i++)
        line<<model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
    stream<<line.join('\t')<<endl;
    line.clear();
    if(stream.status()==QTextStream::WriteFailed)
        goto error;
    for(int i=0;i<model->rowCount();i++)
    {
        for(int j=0;j<model->columnCount();j++)
            line<<model->data(model->index(i,j),Qt::DisplayRole).toString();
        stream<<line.join('\t')<<endl;
        line.clear();
        if(stream.status()==QTextStream::WriteFailed)
            goto error;
    }
    return true;
    error:
        TuringIO::m_last_error=QObject::tr("Не удалось сохранить файл функциональной схемы %1.\n%2")
                .arg(file.fileName())
                .arg(file.errorString());
        return false;
}

bool TuringIO::tapeToXml(TapeModel *model, QFile &file)
{
    QXmlStreamWriter writer;
    writer.setDevice(&file);
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(1);
    writer.writeStartDocument();
    writer.writeStartElement("Tape");
    writer.writeAttribute("string",model->toString());
    writer.writeAttribute("start",QString::number(model->startPos()));
    if(!model->emptyChar().isNull())
        writer.writeAttribute("EmptyChar",model->emptyChar());
    writer.writeEndElement();
    writer.writeEndDocument();
    if(writer.hasError())
        goto error;
    return true;
    error:
        TuringIO::m_last_error=QObject::tr("Не удалось сохранить файл ленты %1.\n%2")
                .arg(file.fileName())
                .arg(file.errorString());
        return false;
}

bool TuringIO::tapeToText(TapeModel *model, QFile &file)
{
    QTextStream stream;
    stream.setDevice(&file);
    stream<<"MTLine"<<endl;
    stream<<model->toString()<<endl;
    stream<<QString::number(model->startPos());
    if(!model->emptyChar().isNull())
        stream<<endl<<model->emptyChar();
    if(stream.status()==QTextStream::WriteFailed)
        goto error;
    return true;
    error:
    TuringIO::m_last_error=QObject::tr("Не удалось сохранить файл ленты %1.\n%2")
            .arg(file.fileName())
            .arg(file.errorString());
    return false;
}


FunctionalSchemeReader::FunctionalSchemeReader(FunctionalSchemeModel *model)
    : m_model(model)
    , m_row(0)
    , m_col(0)

{
    m_rules["script"]="";
    m_rules["table"]="script";
    m_rules["trh"]="table";
    m_rules["th"]="trh";
    m_rules["tr"]="table";
    m_rules["td"]="tr";
    m_tags["script"]=Script;
    m_tags["table"]=Table;
    m_tags["trh"]=Header;
    m_tags["th"]=HeaderCell;
    m_tags["tr"]=Row;
    m_tags["td"]=Cell;
    m_parse_context.push(QString());
}

bool FunctionalSchemeReader::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
    if(m_rules[localName]!=m_parse_context.top())
        return false;
    m_parse_context.push(localName);
    m_cell_buffer.clear();
    if(m_tags[localName]==Row)
        m_model->insertRow(m_row,QModelIndex());
    return true;
}

bool FunctionalSchemeReader::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    m_parse_context.pop();
    switch(m_tags[localName])
    {
    case HeaderCell:
        return m_model->appendCharColumn(m_cell_buffer.at(0));
    case Cell:
    {
        bool result=m_model->setData(m_model->index(m_row,m_col,QModelIndex()),m_cell_buffer,Qt::EditRole);
        m_col++;
        return result;
    }
    case Row:
        m_col=0;
        m_row++;
        break;
    default:
        break;
    }
    return true;
}

bool FunctionalSchemeReader::characters(const QString &ch)
{
    m_cell_buffer.append(ch);
    return true;
}

bool FunctionalSchemeReader::warning(const QXmlParseException &exception)
{
    return true;
}

bool FunctionalSchemeReader::error(const QXmlParseException &exception)
{
    return false;
}

bool FunctionalSchemeReader::fatalError(const QXmlParseException &exception)
{
    return false;
}

QString FunctionalSchemeReader::errorString() const
{
    return "Ошибка разбора функциональной схемы.";
}
