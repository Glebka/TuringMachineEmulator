#ifndef TURINGIO_H
#define TURINGIO_H

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QXmlDefaultHandler>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDomDocument>
#include <QMap>
#include <QStack>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

#include <functionalschememodel.h>
#include <tapemodel.h>

class FunctionalSchemeReader : public QXmlDefaultHandler
{
public:
    enum Tags{Script,Table,Header,HeaderCell,Row,Cell};
    FunctionalSchemeReader(FunctionalSchemeModel * model);
    // QXmlContentHandler interface
public:
    virtual bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
    virtual bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    virtual bool characters(const QString &ch);

    // QXmlErrorHandler interface
public:
    virtual bool warning(const QXmlParseException &exception);
    virtual bool error(const QXmlParseException &exception);
    virtual bool fatalError(const QXmlParseException &exception);

    // QXmlDeclHandler interface
public:
    virtual QString errorString() const;

private:
    FunctionalSchemeModel * m_model;
    QMap<QString,QString> m_rules;
    QMap<QString,Tags> m_tags;
    QStack<QString> m_parse_context;
    QString m_error_string;
    QString m_cell_buffer;
    int m_row,m_col;
};

class TuringIO
{
public:
    static bool loadFunctionalSchemeFromFile(FunctionalSchemeModel * model, QString fileName);
    static bool loadTapeFromFile(TapeModel * model, QString fileName);
    static bool loadMachineFromFile(FunctionalSchemeModel * fs_model, TapeModel * tape_model,QString file);
    static bool saveFunctionalSchemeToFile(FunctionalSchemeModel * model,QString file);
    static bool saveTapeToFile(TapeModel * model,QString file);
    static bool saveMachineToFile(FunctionalSchemeModel * fs_model, TapeModel * tape_model,QString file);
private:
    static bool tapeFromXML(TapeModel * model,QFile & file);
    static bool tapeFromText(TapeModel * model,QFile & file);
};

#endif // TURINGIO_H
