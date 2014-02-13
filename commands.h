#ifndef COMMANDS_H
#define COMMANDS_H

#include <QAbstractItemModel>
#include <QUndoCommand>
#include <QString>

#include "functionalschememodel.h"

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

class RemoveFSColumn : public QUndoCommand
{
public:
    RemoveFSColumn(QChar c,FunctionalSchemeModel * model, int id=-1);

    // QUndoCommand interface
public:
    virtual void undo();
    virtual void redo();
    virtual int id() const;
    virtual bool mergeWith(const QUndoCommand *other);

private:
    int m_id;
    FunctionalSchemeModel * m_model;
    QChar m_char;

    // QUndoCommand interface
};

#endif // COMMANDS_H
