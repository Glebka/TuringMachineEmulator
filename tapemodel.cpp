#include "tapemodel.h"

TapeModel::TapeModel(QObject *parent) :
    QStandardItemModel(parent)
{
    m_regexp.setCaseSensitivity(Qt::CaseSensitive);
    m_regexp.setPattern("^[\\x21-\\x7e]$");
}

bool TapeModel::appendCells(QChar c, int count)
{
    if(c.toLatin1()<33||c.toLatin1()>126)
        return false;
    while(count>0)
    {
        m_tape.append(c);
        insertColumn(columnCount(QModelIndex()));
    }
    return true;
}

QString TapeModel::toString()
{
    return m_tape;
}

void TapeModel::setString(QString s)
{
    beginResetModel();
    m_tape=s;
    endResetModel();
}

QVariant TapeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QStandardItemModel::data(index,role);
    switch(role)
    {
    case Qt::DisplayRole:
        return QStandardItemModel::data(index,role);
    case Qt::BackgroundRole:
    {
        QString s=QStandardItemModel::data(index,Qt::DisplayRole).toString();
        if(!m_regexp.exactMatch(s) && !s.isEmpty())
            return QVariant(QColor(Qt::red));
        break;
    }
    default:
        break;
    }
    return QStandardItemModel::data(index,role);
}

int TapeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

bool TapeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)
        emit cellAboutToBeUpdated(index,data(index,Qt::DisplayRole).toString(),value.toString());
    return QStandardItemModel::setData(index,value,role);
}

bool TapeModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    m_tape.remove(column,count);
    return QStandardItemModel::removeColumns(column,count,parent);
}
