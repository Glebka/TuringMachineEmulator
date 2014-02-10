#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QUndoStack>

#include "functionalschememodel.h"
#include "alphabetmodel.h"
#include "commands.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
    void onCellAboutToBeUpdated(const QModelIndex &index,QString oldtext,QString newtext);
private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    FunctionalSchemeModel * m_fs_model;
    AlphabetModel * m_alphabet;
    char m_code;
    QUndoStack m_undo_stack;
};

#endif // MAINWINDOW_H
