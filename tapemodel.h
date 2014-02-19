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
    bool setStartPos(int pos);
    int startPos() {return m_start_pos;}
    QChar emptyChar(){return m_empty_char;}
    bool setEmptyChar(QChar c);
signals:
    void cellAboutToBeUpdated(const QModelIndex &index,QString oldtext,QString newtext);
private:
    QRegExp m_regexp;
    QChar m_empty_char;
    int m_start_pos;
    // QAbstractItemModel interface
public:
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual bool removeColumns(int column, int count, const QModelIndex &parent);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // TAPEMODEL_H
