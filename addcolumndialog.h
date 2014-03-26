#ifndef ADDCOLUMNDIALOG_H
#define ADDCOLUMNDIALOG_H

#include <QDialog>
#include <QSet>
#include <QAbstractListModel>
#include <alphabetmodel.h>

namespace Ui {
class AddColumnDialog;
}

class AvailCharsModel : public QAbstractListModel
{
    // QAbstractItemModel interface
public:
    AvailCharsModel(AlphabetModel * model);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    AlphabetModel * m_alphabet_model;
    QSet<QChar> m_all_chars;
    QList<QChar> m_available_chars;
};

class AddColumnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddColumnDialog(AlphabetModel * model,FunctionalSchemeModel * fs_model,QWidget *parent = 0);
    QChar selectedChar();
    ~AddColumnDialog();
private:
    Ui::AddColumnDialog *ui;
    AvailCharsModel * m_chars_model;
    FunctionalSchemeModel * m_fs_model;
    // QDialog interface
public slots:
    void accept();
};

#endif // ADDCOLUMNDIALOG_H
