#ifndef TURINGIO_H
#define TURINGIO_H

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QXmlDefaultHandler>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QMap>
#include <QStack>
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
    enum FileFormat {Default,Compatibility_v1,Compatibility_v2};
    enum SaveState {Ok,Error,Overwrite};

    static bool loadFunctionalSchemeFromFile(FunctionalSchemeModel * model, QString fileName);
    static bool loadTapeFromFile(TapeModel * model, QString fileName);
    static bool loadMachineFromFile(FunctionalSchemeModel * fs_model, TapeModel * tape_model,QString fileName);

    static SaveState saveFunctionalSchemeToFile(FunctionalSchemeModel * model,QString fileName, FileFormat format=Default,bool overwrite=false);
    static SaveState saveTapeToFile(TapeModel * model,QString fileName, FileFormat format=Default,bool overwrite=false);
    static SaveState saveProjectFile(QString fileName,QString fsFile=QString(),QString tapeFile=QString(), FileFormat format=Default,bool overwrite=false);

    static inline QString getLastError(){return TuringIO::m_last_error;}
    static inline QString getCurrentProjectFile(){return TuringIO::m_project_file;}
    static inline QString getCurrentFunctionalSchemeFile(){return TuringIO::m_fs_file;}
    static inline QString getCurrentTapeFile(){return TuringIO::m_tape_file;}

private:
    static bool tapeFromXML(TapeModel * model,QFile & file);
    static bool tapeFromText(TapeModel * model,QFile & file);

    static bool functionalSchemeFromText(FunctionalSchemeModel * model,QFile &file);
    static bool functionalSchemeFromXML(FunctionalSchemeModel * model,QFile &file);

    static bool machineFromText(FunctionalSchemeModel * fs_model, TapeModel * tape_model,QFile & file);
    static bool machineFromXML(FunctionalSchemeModel * fs_model, TapeModel * tape_model,QFile & file);

    static bool functionalSchemeToXml(FunctionalSchemeModel * model, QString fileName);
    static bool functionalSchemeToText(FunctionalSchemeModel * model, QString fileName);

    static bool tapeToXml(TapeModel * model, QString fileName);
    static bool tapeToText(TapeModel * model, QString fileName);
    static QString m_last_error;
    static QString m_project_file;
    static QString m_fs_file;
    static QString m_tape_file;
};

#endif // TURINGIO_H
