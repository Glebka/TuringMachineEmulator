#include "tapemodel.h"

TapeModel::TapeModel(QObject *parent)
    : QStandardItemModel(parent)
    , m_start_pos(0)
{
    m_regexp.setCaseSensitivity(Qt::CaseSensitive);
    m_regexp.setPattern("^[\\x21-\\x7e]$");
    insertRow(0);
}

bool TapeModel::appendCells(QChar c, int count)
{
    if(c.toLatin1()<33||c.toLatin1()>126)
        return false;
    int col=0;
    while(count>0)
    {
        insertColumn(columnCount(QModelIndex()));
        setData(index(0,col),c,Qt::EditRole);
        col++;
        count--;
    }
    return true;
}

QString TapeModel::toString()
{
    return QString();
}

void TapeModel::setString(QString s)
{
    beginResetModel();
    removeColumns(0,columnCount(),QModelIndex());
    insertColumns(0,s.length());
    int col=0;
    foreach(QChar c, s)
    {
        setData(index(0,col),c,Qt::EditRole);
        col++;
    }
    endResetModel();
}

bool TapeModel::setStartPos(int pos)
{
    if(pos>=columnCount(QModelIndex()))
        return false;
    m_start_pos=pos;
    return true;
}

bool TapeModel::setEmptyChar(QChar c)
{
    if(c.toLatin1()<33||c.toLatin1()>126)
        return false;
    m_empty_char=c;
    return true;
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

bool TapeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QString v=value.toString();
    if(v.length()>1)
        v.chop(v.length()-1);
    if(role==Qt::EditRole)
        emit cellAboutToBeUpdated(index,data(index,Qt::DisplayRole).toString(),v);
    return QStandardItemModel::setData(index,v,role);
}

bool TapeModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    return QStandardItemModel::removeColumns(column,count,parent);
}

QVariant TapeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
