#ifndef TAPEMODEL_H
#define TAPEMODEL_H

#include <QStandardItemModel>
#include <QString>
#include <QRegExp>

class TapeModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TapeModel(QObject *parent = 0);
    bool appendCells(QChar c,int count);
    QString toString();
    void setString(QString s);
signals:
    void cellAboutToBeUpdated(const QModelIndex &index,QString oldtext,QString newtext);
private:
    QString m_tape;
    QRegExp m_regexp;
    // QAbstractItemModel interface
public:
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual bool removeColumns(int column, int count, const QModelIndex &parent);
};

#endif // TAPEMODEL_H
