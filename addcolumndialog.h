#ifndef ADDCOLUMNDIALOG_H
#define ADDCOLUMNDIALOG_H

#include <QDialog>

namespace Ui {
class AddColumnDialog;
}

class AddColumnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddColumnDialog(QWidget *parent = 0);
    ~AddColumnDialog();

private:
    Ui::AddColumnDialog *ui;
};

#endif // ADDCOLUMNDIALOG_H
