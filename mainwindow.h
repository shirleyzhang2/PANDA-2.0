#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"
#include "editdesign.h"
#include "runanalysis.h"

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
class QTableWidgetItem;
class QTableWidget;
class QAction;
class QMenu;
class QSessionManager;

class Spreadsheet;

QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(/*int rows, int cols, */QWidget *parent = nullptr);

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
    void createContextMenu();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void copy();
    void paste();

    // should keep all actions here
    //QAction *copyAct;

    std::map <std::string, Model> allDesign;
    enum { MagicNumber = 0x7F51C883, RowCount = 999, ColumnCount = 16 };

    QString strippedName(const QString &fullFileName);

    QPlainTextEdit *textEdit;
    QTableWidget *table;
    QString curFile;

    EditDesign *editdesign;
    RunAnalysis *runanalysis;

    Spreadsheet *spreadsheet;
};
//! [0]

#endif
