#ifndef ERRORTABLEMODEL_H
#define ERRORTABLEMODEL_H

#include <QAbstractTableModel>

class ErrorTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ErrorTableModel(QObject *parent = 0);

signals:

public slots:

};

#endif // ERRORTABLEMODEL_H
