#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_code(126),
    m_fs_model(0)
{
    ui->setupUi(this);
    m_fs_model=new FunctionalSchemeModel();
    ui->tableView->setModel(m_fs_model);
    ui->tableView->horizontalHeader()->setSectionsMovable(true);
    m_alphabet=new AlphabetModel(m_fs_model);
    ui->tableView_2->setModel(m_alphabet);
    connect(m_fs_model,&FunctionalSchemeModel::cellAboutToBeUpdated,this,&MainWindow::onCellAboutToBeUpdated);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    m_fs_model->insertCharColumn(m_code);
    m_code--;
}

void MainWindow::on_pushButton_2_clicked()
{
    m_fs_model->insertRow(m_fs_model->rowCount(QModelIndex()));
}

void MainWindow::on_pushButton_3_clicked()
{
    QModelIndexList indexes;
    while((indexes = ui->tableView->selectionModel()->selectedRows()).size()) {
        m_fs_model->removeRow(indexes.first().row(),QModelIndex());
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QModelIndexList indexes;
    while((indexes = ui->tableView->selectionModel()->selectedColumns()).size()) {
        m_fs_model->removeColumn(indexes.first().column(),QModelIndex());
    }
}

void MainWindow::onCellAboutToBeUpdated(const QModelIndex &index, QString oldtext, QString newtext)
{
    QUndoCommand * cmd=new UpdateCell(index,oldtext,newtext);
    m_undo_stack.push(cmd);
}

void MainWindow::on_pushButton_5_clicked()
{
    m_undo_stack.undo();
}

void MainWindow::on_pushButton_6_clicked()
{
    m_undo_stack.redo();
}
