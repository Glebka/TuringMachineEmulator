#include "addcolumndialog.h"
#include "ui_addcolumndialog.h"

AddColumnDialog::AddColumnDialog(AlphabetModel *model, FunctionalSchemeModel *fs_model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddColumnDialog),
    m_chars_model(0),
    m_fs_model(fs_model)
{
    ui->setupUi(this);
    m_chars_model=new AvailCharsModel(model);
    ui->comboBox->setModel(m_chars_model);
}

QChar AddColumnDialog::selectedChar()
{
    if(ui->comboBox->currentText().isEmpty())
        return QChar();
    return ui->comboBox->currentText().at(0);
}

AddColumnDialog::~AddColumnDialog()
{
    delete ui;
    delete m_chars_model;
}

AvailCharsModel::AvailCharsModel(AlphabetModel *model)
    : m_alphabet_model(model)
{
    for(int i=33;i<127;i++)
        m_all_chars.insert(QChar(i));
    QSet<QChar> temp=m_all_chars;
    m_available_chars=temp.subtract(m_alphabet_model->toSet()).toList();
    qSort(m_available_chars);
}

int AvailCharsModel::rowCount(const QModelIndex &parent) const
{
    return m_available_chars.size();
}

QVariant AvailCharsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role!=Qt::DisplayRole)
        return QVariant();
    return m_available_chars.at(index.row());
}


void AddColumnDialog::accept()
{
    m_fs_model->appendCharColumn(selectedChar());
    QDialog::accept();
}
