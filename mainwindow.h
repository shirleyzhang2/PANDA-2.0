#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"
#include "runanalysis.h"

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
class QTableWidgetItem;
class QTableWidget;
class QAction;
class QMenu;
class QSessionManager;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int rows, int cols, QWidget *parent = nullptr);

public slots:
    void print();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void tutorial();
    void parameters();
    void design();
    void analysis();
    void rank();
    void mesh();
    void documentWasModified();

private:
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void copy();
    void paste();

    QAction *printAction;

    //Design d;
    std::map <std::string, Model> allDesign;


    QString strippedName(const QString &fullFileName);

    QPlainTextEdit *textEdit;
    QTableWidget *table;
    QString curFile;

//    RunAnalysis *runanalysis;
};
//! [0]

#endif
