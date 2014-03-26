#ifndef ALPHABETMODEL_H
#define ALPHABETMODEL_H

#include <QAbstractTableModel>
#include <functionalschememodel.h>
#include <QSet>

class AlphabetModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit AlphabetModel(FunctionalSchemeModel * fs,QObject *parent = 0);
    QSet<QChar> toSet();
signals:

private slots:
    void on_fs_column_inserted(const QModelIndex & parent, int start, int end);
    void on_fs_column_removed(const QModelIndex & parent, int start, int end);
    void on_fs_reset();
private:
    FunctionalSchemeModel * m_fs_model;
    QString m_alphabet;

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // ALPHABETMODEL_H
