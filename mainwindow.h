#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QShowEvent>
#include <QSettings>
#include <QResizeEvent>
#include <QUndoGroup>
#include <QClipboard>

#include "functionalschememodel.h"
#include "alphabetmodel.h"
#include "commands.h"
#include "TuringIO.h"

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
    void onFocusChanged(QWidget * old, QWidget * now);
    void onCleanStateChanged(bool clean);
    void onClipboardChanged(QClipboard::Mode mode);
    /*
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
    void onCellAboutToBeUpdated(const QModelIndex &index,QString oldtext,QString newtext);
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();*/

private:

    void prepareUI();

    Ui::MainWindow *ui;
    QUndoGroup m_undo_group;
    QUndoStack m_undo_fs,m_undo_tape;
    QClipboard * m_clipboard;

    FunctionalSchemeModel * m_fs_model;
    AlphabetModel * m_alphabet;

    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *e);
    virtual void resizeEvent(QResizeEvent *e);
};

#endif // MAINWINDOW_H
