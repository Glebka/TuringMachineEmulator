#include "commands.h"

UpdateCell::UpdateCell(const QModelIndex &index, QString oldtext, QString text)
    : m_oldtext(oldtext)
    , m_text(text)
    , m_model(const_cast<QAbstractItemModel *>(index.model()))
    , m_row(index.row())
    , m_col(index.column())
{
}

void UpdateCell::undo()
{
    QModelIndex index=m_model->index(m_row,m_col,QModelIndex());
    if(!index.isValid())
        return;
    m_model->setData(index,m_oldtext,Qt::DisplayRole);
}

void UpdateCell::redo()
{
    QModelIndex index=m_model->index(m_row,m_col,QModelIndex());
    if(!index.isValid())
        return;
    m_model->setData(index,m_text,Qt::DisplayRole);
}
