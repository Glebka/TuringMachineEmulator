#include "alphabetmodel.h"

AlphabetModel::AlphabetModel(FunctionalSchemeModel *fs, QObject *parent) :
    QAbstractTableModel(parent),
    m_fs_model(fs)
{
    m_alphabet=m_fs_model->getAlphabetAsString();
    qSort(m_alphabet.begin(),m_alphabet.end());
    connect(m_fs_model,&FunctionalSchemeModel::columnsInserted,this,&AlphabetModel::on_fs_column_inserted);
    connect(m_fs_model,&FunctionalSchemeModel::columnsRemoved,this,&AlphabetModel::on_fs_column_removed);
    connect(m_fs_model,&FunctionalSchemeModel::modelReset,this,&AlphabetModel::on_fs_reset);
}

void AlphabetModel::on_fs_column_inserted(const QModelIndex &parent, int start, int end)
{
    int count=end-start+1;
    beginInsertRows(QModelIndex(),rowCount(QModelIndex()),rowCount(QModelIndex())+count-1);
    m_alphabet=m_fs_model->getAlphabetAsString();
    qSort(m_alphabet.begin(),m_alphabet.end());
    endInsertRows();
}

void AlphabetModel::on_fs_column_removed(const QModelIndex &parent, int start, int end)
{
    beginRemoveRows(QModelIndex(),start,end);
    m_alphabet=m_fs_model->getAlphabetAsString();
    qSort(m_alphabet.begin(),m_alphabet.end());
    endRemoveRows();
}

void AlphabetModel::on_fs_reset()
{
    beginResetModel();
    m_alphabet.clear();
    endResetModel();
}

int AlphabetModel::rowCount(const QModelIndex &parent) const
{
    return m_alphabet.length();
}

int AlphabetModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant AlphabetModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role==Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            return m_alphabet.at(index.row());
        case 1:
            return (int)m_alphabet.at(index.row()).toLatin1();
        }
    }
    return QVariant();
}

QVariant AlphabetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation==Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return QVariant(tr("Символ"));
        case 1:
            return QVariant(tr("Код ASCII"));
        default:
            break;
        }
    }
    return QVariant();
}

Qt::ItemFlags AlphabetModel::flags(const QModelIndex &index) const
{
    if(index.column()==1)
        return Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}
