#include "functionalschememodel.h"

FunctionalSchemeModel::FunctionalSchemeModel(QObject *parent) :
    QStandardItemModel(parent)
{
    m_regexp.setCaseSensitivity(Qt::CaseSensitive);
    m_regexp.setPattern("^q[0-9]+\\s[\\x21-\\x7e]\\s[SRL]$");
    m_regexp_cap.setCaseSensitivity(Qt::CaseSensitive);
    m_regexp_cap.setPattern("^(q[0-9]+)\\s([\\x21-\\x7e])\\s([SRL])$");
    m_header="";
}

bool FunctionalSchemeModel::appendCharColumn(QChar c)
{
    if(!m_header.isEmpty())
        if(m_header.contains(c)||c.toLatin1()<33||c.toLatin1()>126)
            return false;
    m_header.append(c);
    insertColumn(columnCount(QModelIndex()));
    return true;
}

QString FunctionalSchemeModel::getAlphabetAsString()
{
    return m_header;
}

QString FunctionalSchemeModel::tooltip(QString command) const
{
    if(command.isEmpty()) return QString();

    if (m_regexp_cap.indexIn(command) == -1)
        return tr("Неправильный формат ввода. Пожалуйста, введите команду по слудующему образцу:\n<Новое состояние> <Новый символ> <Перемещение>");
    else
    {
        QString shift;
        switch(m_regexp_cap.cap(3).at(0).toLatin1())
        {
        case 'S':
            shift=tr("оставить головку на месте");
            break;
        case 'R':
            shift=tr("сместить головку вправо");
            break;
        case 'L':
            shift=tr("сместить головку влево");
            break;
        }

        return tr("Перейти в состояние '%1', записать символ '%2', %3")
                .arg(m_regexp_cap.cap(1))
                .arg(m_regexp_cap.cap(2))
                .arg(shift);
    }
}

QVariant FunctionalSchemeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation==Qt::Horizontal)
        return QVariant(m_header.at(section));
    else
        return QString("q%1").arg(QString::number(section+1));
}

QVariant FunctionalSchemeModel::data(const QModelIndex &index, int role) const
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
    case Qt::StatusTipRole:
    case Qt::ToolTipRole:
        return QVariant(tooltip(QStandardItemModel::data(index,Qt::DisplayRole).toString()));
    }
    return QStandardItemModel::data(index,role);
}

bool FunctionalSchemeModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    m_header.remove(column,count);
    return QStandardItemModel::removeColumns(column,count,parent);
}

bool FunctionalSchemeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)
        emit cellAboutToBeUpdated(index,data(index,Qt::DisplayRole).toString(),value.toString());
    return QStandardItemModel::setData(index,value,role);
}
