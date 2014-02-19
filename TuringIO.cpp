#include <TuringIO.h>


bool TuringIO::loadFunctionalSchemeFromFile(FunctionalSchemeModel *model, QString fileName)
{
    FunctionalSchemeReader handler(model);
    QFile fs_file(fileName);
    if (!fs_file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Эмулятор машины Тьюринга"),
                             QObject::tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(fs_file.errorString()));
        return false;
    }
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);
    QXmlInputSource xmlInputSource(&fs_file);
    if (reader.parse(xmlInputSource))
        return true;
    return false;
}

bool TuringIO::loadTapeFromFile(TapeModel *model, QString fileName)
{
    QFile tape_file(fileName);
    if(!tape_file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(0, QObject::tr("Эмулятор машины Тьюринга"),
                             QObject::tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(tape_file.errorString()));
        return false;
    }
    QTextStream stream(&tape_file);
    QString header=stream.readLine(100);
    stream.seek(0);
    if(header.contains("<?xml"))
        return tapeFromXML(model,tape_file);
    else
        if(header=="MTLine")
            return tapeFromText(model,tape_file);
        else
            return false;
}

bool TuringIO::tapeFromXML(TapeModel *model, QFile &file)
{
    QDomDocument doc;
    doc.setContent(&file);
    QDomElement tape=doc.firstChild().toElement();
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
        return false;
    model->setString(stream.readLine(1024*1204));
    if(stream.atEnd())
        return false;
    model->setStartPos(atoi(stream.readLine(10).toStdString().c_str()));
    return true;
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
    }
    return true;
}

bool FunctionalSchemeReader::characters(const QString &ch)
{
    m_cell_buffer.append(ch);
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
