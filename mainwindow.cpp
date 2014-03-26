#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_fs_model(0),
    m_tape_model(0)
{
    ui->setupUi(this);
    prepareUI();
    m_clipboard=QApplication::clipboard();
    connect(&m_undo_group,&QUndoGroup::cleanChanged,this,&MainWindow::onCleanStateChanged);
    connect(m_clipboard,&QClipboard::changed,this,&MainWindow::onClipboardChanged);
    m_fs_model=new FunctionalSchemeModel();
    m_tape_model=new TapeModel();

    ui->functionalSchemeView->setModel(m_fs_model);
    ui->functionalSchemeView->horizontalHeader()->setSectionsMovable(true);

    ui->tapeView->setModel(m_tape_model);
    ui->tapeView->horizontalHeader()->setDefaultSectionSize(25);

    m_alphabet=new AlphabetModel(m_fs_model);
    ui->alphabetView->setModel(m_alphabet);
    //m_fs_model->appendCharColumn('0');
    //if(!TuringIO::loadMachineFromFile(m_fs_model,m_tape_model,"+1/PlusOne.mtp"))
    //    QMessageBox::warning(this,tr("Эмулятор машины Тьюринга"),TuringIO::getLastError());
    //ui->tapeView->sec
    //TuringIO::loadFunctionalSchemeFromFile(m_fs_model,"test2.mts");
    //TuringIO::loadTapeFromFile(m_tape_model,"test2.mtl");
    //connect(m_fs_model,&FunctionalSchemeModel::cellAboutToBeUpdated,this,&MainWindow::onCellAboutToBeUpdated);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFocusChanged(QWidget *old, QWidget *now)
{
    Q_UNUSED(old);
    if(!now)
        return;
    if(now==ui->functionalSchemeView)
        m_undo_group.setActiveStack(&m_undo_fs);
    if(now==ui->tapeView)
        m_undo_group.setActiveStack(&m_undo_tape);
}

void MainWindow::onCleanStateChanged(bool clean)
{
    Q_UNUSED(clean);
    bool all_clean=true;
    foreach(QUndoStack * stack,m_undo_group.stacks())
    {
        if(!stack->isClean())
        {
            all_clean=false;
            break;
        }
    }
    if(all_clean)
        qDebug()<<"Saved state";
    else
        qDebug()<<"Unsaved state";
}

void MainWindow::onClipboardChanged(QClipboard::Mode mode)
{

}

void MainWindow::prepareUI()
{
    QVBoxLayout * mainLayout=new QVBoxLayout();
    mainLayout->setMargin(2);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(ui->splitter_2);
    ui->centralWidget->setLayout(mainLayout);
    ui->splitter_2->setStretchFactor(0,8);
    ui->splitter_2->setStretchFactor(1,3);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(0,1);
    QVBoxLayout * tapeLayout=new QVBoxLayout();
    tapeLayout->setMargin(2);
    tapeLayout->setSpacing(0);
    tapeLayout->addWidget(ui->tapeView);
    ui->tabTape->setLayout(tapeLayout);
    QVBoxLayout * errorsLayout=new QVBoxLayout();
    errorsLayout->setMargin(2);
    errorsLayout->setSpacing(0);
    errorsLayout->addWidget(ui->errorsView);
    ui->tabErrors->setLayout(errorsLayout);
    ui->tabWidget->setTabText(0,tr("Лента"));
    ui->tabWidget->setTabText(1,tr("Ошибки"));
    QAction * m_undo_action=m_undo_group.createUndoAction(this,tr("Отменить действие"));
    m_undo_action->setIcon(QIcon(":/images/undo.png"));
    QAction * m_redo_action=m_undo_group.createRedoAction(this,tr("Повторить действие"));
    m_redo_action->setIcon(QIcon(":/images/redo.png"));
    QList<QAction *> list;
    list<<m_undo_action<<m_redo_action;
    ui->mnuEdit->insertActions(ui->mnuEdit->actions().first(),list);
    m_undo_group.addStack(&m_undo_fs);
    m_undo_group.addStack(&m_undo_tape);
}
/*
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
}*/


void MainWindow::showEvent(QShowEvent *e)
{
    e->accept();
}


void MainWindow::resizeEvent(QResizeEvent *e)
{

}

void MainWindow::on_openProject_triggered()
{
    QString filename=QFileDialog::getOpenFileName(this,tr("Открыть проект..."),"","Файлы проекта МТ (*.mtp)");
    if(filename.isEmpty())
        return;
    FunctionalSchemeModel * fs_model=new FunctionalSchemeModel();
    TapeModel * tape_model=new TapeModel();
    AlphabetModel * alphabet_model=new AlphabetModel(fs_model);
    if(!TuringIO::loadMachineFromFile(fs_model,tape_model,filename))
    {
        delete fs_model;
        delete tape_model;
        return;
        QMessageBox::warning(this,tr("Эмулятор машины Тьюринга"),TuringIO::getLastError());
    }
    ui->functionalSchemeView->setModel(fs_model);
    ui->tapeView->setModel(tape_model);
    ui->alphabetView->setModel(alphabet_model);
    AddColumnDialog dialog(alphabet_model,fs_model,this);
    dialog.setModal(true);
    dialog.exec();
    qDebug()<<dialog.selectedChar();
    delete m_fs_model;
    delete m_tape_model;
    delete m_alphabet;
    m_fs_model=fs_model;
    m_tape_model=tape_model;
    m_alphabet=alphabet_model;
}

void MainWindow::on_saveProjectAs_triggered()
{
    QString filename=QFileDialog::getSaveFileName(this,tr("Сохранить проект как..."),"","Файлы проекта МТ (*.mtp)");
    if(filename.isEmpty())
        return;
    if(!TuringIO::saveProjectFile(filename,QString(),QString(),TuringIO::Compatibility_v2))
        QMessageBox::warning(this,tr("Эмулятор машины Тьюринга"),TuringIO::getLastError());
    if(!TuringIO::saveFunctionalSchemeToFile(m_fs_model,QString(),TuringIO::Compatibility_v2))
        QMessageBox::warning(this,tr("Эмулятор машины Тьюринга"),TuringIO::getLastError());
    if(!TuringIO::saveTapeToFile(m_tape_model,QString(),TuringIO::Compatibility_v2))
        QMessageBox::warning(this,tr("Эмулятор машины Тьюринга"),TuringIO::getLastError());
}
