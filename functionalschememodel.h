#ifndef FUNCTIONALSCHEMEMODEL_H
#define FUNCTIONALSCHEMEMODEL_H

#include <QStandardItemModel>
#include <QString>
#include <QRegExp>

class FunctionalSchemeModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit FunctionalSchemeModel(QObject *parent = 0);
    bool appendCharColumn(QChar c);
    QString getAlphabetAsString();
signals:
    void cellAboutToBeUpdated(const QModelIndex &index,QString oldtext,QString newtext);
private:
    QString m_header;
    QRegExp m_regexp;
    QRegExp m_regexp_cap;
    QString tooltip(QString command) const;

    // QAbstractItemModel interface
public:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool removeColumns(int column, int count, const QModelIndex &parent);
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
};

#endif // FUNCTIONALSCHEMEMODEL_H
