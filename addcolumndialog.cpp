#include "addcolumndialog.h"
#include "ui_addcolumndialog.h"

AddColumnDialog::AddColumnDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddColumnDialog)
{
    ui->setupUi(this);
}

AddColumnDialog::~AddColumnDialog()
{
    delete ui;
}
