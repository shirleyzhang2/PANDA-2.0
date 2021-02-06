#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canoe.h"
//#include "model.h"
//#include "editdesign.h"
//#include "runanalysis.h"

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
class QTabWidget;
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
    void createActions();
    void createContextMenu();
    void tabSelected();
    void createStatusBar();
    void readSettings();
    void writeSettings();

    void tutorial();
    void parameters();
    void analysis();
    void final();
    void mesh();

    void createInputTable();
    void saveInputToText();

private:

    // should keep all actions here
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;

//    std::map <std::string, Model> allDesign;
    enum { MagicNumber = 0x7F51C883, RowCount = 999, ColumnCount = 16 };

    QString strippedName(const QString &fullFileName);

//    QPlainTextEdit *textEdit;
    QTableWidget *table;
    QString curFile;

//    EditDesign *editdesign;
    QTabWidget *central_widget;
//    RunAnalysis *runanalysis;

    Spreadsheet *inputSpreadsheet;
    Spreadsheet *weightedSpreadsheet;

    Canoe *c;


    // spreadsheet dimensions
    QStringList inputHeaderH = {"Min", "Max", "Value"};
    int numInputHeaderH{ static_cast<int>(std::size(inputHeaderH)) };
    QStringList inputHeaderV = {"length", "Lp", "Ld", "Lf", "w", "t1", "t2", "d", "b", "s", "f", "n", "density"};
    int numInputHeaderV{ static_cast<int>(std::size(inputHeaderV)) };
    QStringList weightedHeaderH = {"Target Value", "Standard Dev.", "Weight"};
    int numWeightedHeaderH{ static_cast<int>(std::size(weightedHeaderH)) };
    QStringList weightedHeaderV = {"Weight", "Cp", "Freeboard", "Drag", "Stability", "LeakAngle", "SecondMoment", "WaterplaneCentroid", "PaddlerCentre"};
    int numWeightedHeaderV{ static_cast<int>(std::size(weightedHeaderV)) };

};
//! [0]

#endif
