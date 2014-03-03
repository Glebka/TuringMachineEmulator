#include <TuringIO.h>

QString TuringIO::m_last_error;
QString TuringIO::m_project_file;
QString TuringIO::m_fs_file;
QString TuringIO::m_tape_file;

bool TuringIO::loadFunctionalSchemeFromFile(FunctionalSchemeModel *model, QString fileName)
{
    QFile fs_file(fileName);
    if (!fs_file.open(QFile::ReadOnly | QFile::Text))
    {
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл функциональной схемы %1.\n%2").arg(fileName).arg(fs_file.errorString());
        return false;
    }
    m_fs_file=fileName;
    QTextStream stream(&fs_file);
    QString header=stream.readLine(100);
    stream.seek(0);
    if(header.contains("<?xml"))
        return functionalSchemeFromXML(model,fs_file);
    else
        if(header=="MTFs")
            return functionalSchemeFromText(model,fs_file);
        else
        {
            TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(fileName);
            return false;
        }
}

bool TuringIO::loadTapeFromFile(TapeModel *model, QString fileName)
{
    QFile tape_file(fileName);
    if(!tape_file.open(QFile::ReadOnly|QFile::Text))
    {
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл ленты %1.\n%2").arg(fileName).arg(tape_file.errorString());
        return false;
    }
    m_tape_file=fileName;
    QTextStream stream(&tape_file);
    QString header=stream.readLine(100);
    stream.seek(0);
    if(header.contains("<?xml"))
        return tapeFromXML(model,tape_file);
    else
        if(header=="MTLine")
            return tapeFromText(model,tape_file);
        else
        {
            TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(fileName);
            return false;
        }
}

bool TuringIO::loadMachineFromFile(FunctionalSchemeModel *fs_model, TapeModel *tape_model, QString fileName)
{
    QFile machine_file(fileName);
    if(!machine_file.open(QFile::ReadOnly|QFile::Text))
    {
        TuringIO::m_last_error=QObject::tr("Не удалось открыть файл проекта %1.\n%2").arg(fileName).arg(machine_file.errorString());
        return false;
    }
    m_project_file=fileName;
    QTextStream stream(&machine_file);
    QString header=stream.readLine(100);
    stream.seek(0);
    if(header.contains("<?xml"))
        return machineFromXML(fs_model,tape_model,machine_file);
    else
        if(header=="MTProj")
            return machineFromText(fs_model,tape_model,machine_file);
        else
        {
            TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(fileName);
            return false;
        }
}

TuringIO::SaveState TuringIO::saveFunctionalSchemeToFile(FunctionalSchemeModel *model, QString fileName, TuringIO::FileFormat format, bool overwrite)
{
    return TuringIO::Error;
}

TuringIO::SaveState TuringIO::saveTapeToFile(TapeModel *model, QString fileName, TuringIO::FileFormat format, bool overwrite)
{
    return TuringIO::Error;
}

TuringIO::SaveState TuringIO::saveProjectFile(QString fileName,QString fsFile,QString tapeFile, FileFormat format,bool overwrite)
{
    if(!fileName.endsWith(".mtp"))
        fileName.append(".mtp");
    if(QFileInfo::exists(fileName) && !overwrite)
        return TuringIO::Overwrite;
    QFile project_file(fileName);
    QTextStream stream;
    QXmlStreamWriter writer;
    QFileInfo info(project_file);
    if(fsFile.isEmpty())
        fsFile=info.absoluteDir().absoluteFilePath(info.baseName().append(".mts"));
    if(tapeFile.isEmpty())
        tapeFile=info.absoluteDir().absoluteFilePath(info.baseName().append(".mtl"));
    if(fsFile.isEmpty() || tapeFile.isEmpty())
        goto error;
    if(project_file.exists())
    {
        if(QFile::exists(fileName+".bkp"))
            if(!QFile::remove(fileName+".bkp"))
                goto error;
        if(!QFile::copy(fileName,fileName+".bkp"))
            goto error;
    }
    if(!project_file.open(QIODevice::WriteOnly))
        goto error;
    switch(format)
    {
    case TuringIO::Compatibility_v2:

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
    if(QFileInfo::exists(info.absoluteFilePath().append(".bkp")))
        QFile::remove(info.absoluteFilePath().append(".bkp"));
    return TuringIO::Ok;
    error:
        TuringIO::m_last_error=QObject::tr("Ошибка при сохранении файла %1.").arg(fileName);
        if(!QFileInfo::exists(fileName) && QFileInfo::exists(fileName+".bkp"))
            QFile::rename(fileName+".bkp",fileName);
        return TuringIO::Error;
}

bool TuringIO::tapeFromXML(TapeModel *model, QFile &file)
{
    QDomDocument doc;
    doc.setContent(&file);
    QDomElement tape=doc.elementsByTagName("Tape").at(0).toElement();
    if(tape.isNull())
    {
        TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(file.fileName());
        return false;
    }
    model->setString(tape.attribute("string"));
    model->setStartPos(atoi(tape.attribute("start","0").toStdString().c_str()));
    QString emptyChar=tape.attribute("EmptyChar");
    if(!emptyChar.isEmpty())
        model->setEmptyChar(emptyChar.at(0));
    return true;
}

bool TuringIO::tapeFromText(TapeModel *model, QFile &file)
{
    QTextStream stream(&file);
    stream.readLine();
    if(stream.atEnd())
    {
        TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(file.fileName());
        return false;
    }
    model->setString(stream.readLine(1024*1204));
    if(stream.atEnd())
    {
        TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(file.fileName());
        return false;
    }
    model->setStartPos(atoi(stream.readLine(10).toStdString().c_str()));
    return true;
}

bool TuringIO::functionalSchemeFromText(FunctionalSchemeModel *model, QFile &file)
{
    QTextStream stream(&file);
    stream.readLine();
    if(stream.atEnd())
    {
        TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(file.fileName());
        return false;
    }
    QString header=stream.readLine(1024);
    QTextStream header_stream(&header,QIODevice::ReadOnly);
    QString temp="";
    int col=0;
    while(!header_stream.atEnd())
    {
        header_stream>>temp;
        if(temp.isEmpty())
            return false;
        model->appendCharColumn(temp.at(0));
        col++;
    }
    int c,r=0;
    QString command="";
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
        model->setData(model->index(r,c),command,Qt::EditRole);
        c++;
        if(c>col)
        {
            c=0;
            r++;
        }
    }
    return true;
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
    stream.readLine();
    if(stream.atEnd())
    {
        TuringIO::m_last_error=QObject::tr("Формат файла %1 не поддерживается.").arg(file.fileName());
        return false;
    }
    QString fs_filename=stream.readLine();
    QString tape_filename=stream.readLine();
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

bool TuringIO::functionalSchemeToXml(FunctionalSchemeModel *model, QString fileName)
{
    QFile fs_file(fileName);
    QXmlStreamWriter writer;
    if(!fs_file.open(QIODevice::WriteOnly|QFile::Text))
        goto error;
    writer.setDevice(&fs_file);
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
        TuringIO::m_last_error=QObject::tr("Не удалось сохранить файл %1.\n%2").arg(fileName).arg(fs_file.errorString());
        return false;
}

bool TuringIO::functionalSchemeToText(FunctionalSchemeModel *model, QString fileName)
{
    QFile fs_file(fileName);
    QTextStream stream;
    QStringList line;
    if(!fs_file.open(QIODevice::WriteOnly|QFile::Text))
        goto error;
    stream.setDevice(&fs_file);
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
        TuringIO::m_last_error=QObject::tr("Не удалось сохранить файл %1.\n%2").arg(fileName).arg(fs_file.errorString());
        return false;
}

bool TuringIO::tapeToXml(TapeModel *model, QString fileName)
{
    QFile tape_file(fileName);
    QXmlStreamWriter writer;
    if(!tape_file.open(QIODevice::WriteOnly|QFile::Text))
        goto error;
    writer.setDevice(&tape_file);
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
    return false;
}

bool TuringIO::tapeToText(TapeModel *model, QString fileName)
{
    QFile tape_file(fileName);
    QTextStream stream;
    if(!tape_file.open(QIODevice::WriteOnly|QFile::Text))
        goto error;
    stream.setDevice(&tape_file);
    stream<<"MTLine"<<endl;
    stream<<model->toString()<<endl;
    stream<<QString::number(model->startPos());
    if(!model->emptyChar().isNull())
        stream<<endl<<model->emptyChar();
    if(stream.status()==QTextStream::WriteFailed)
        goto error;
    return true;
    error:
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
    return "Some parse error";
}
