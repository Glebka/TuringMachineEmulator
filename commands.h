#ifndef COMMANDS_H
#define COMMANDS_H

#include <QAbstractItemModel>
#include <QUndoCommand>
#include <QString>

class UpdateCell : public QUndoCommand
{
public:
    UpdateCell(const QModelIndex & index,QString oldtext, QString text);

    // QUndoCommand interface
public:
    virtual void undo();
    virtual void redo();

private:
    QAbstractItemModel * m_model;
    QString m_oldtext,m_text;
    int m_row,m_col;
};

#endif // COMMANDS_H
