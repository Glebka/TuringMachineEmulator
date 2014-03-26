#ifndef ERRORTABLEMODEL_H
#define ERRORTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMap>

class ErrorTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ErrorTableModel(QObject *parent = 0);
    enum ErrorType {Notice,Warning,Error};
    struct ErrorRecord
    {
        ErrorType type;
        QString sender_name;
        QString text;
    };

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:

};

#endif // ERRORTABLEMODEL_H
