#include <QtWidgets>
#include "mainwindow.h"
//#include "editdesign.h"
//#include "runanalysis.h"
#include "printview.h"
#include "spreadsheet.h"
//#include "canoe.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport>
#endif

MainWindow::MainWindow(/*int rows, int cols, */QWidget *parent)
    : QMainWindow(parent),
      central_widget(new QTabWidget)
      //runanalysis(new RunAnalysis(this))

{
    //central_widget = new QTabWidget;
    // Set up main table
    inputSpreadsheet = new Spreadsheet(numInputHeaderV,numInputHeaderH);
    central_widget->addTab(inputSpreadsheet, "Input Setup");
    inputSpreadsheet->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    inputSpreadsheet->setHorizontalHeaderLabels( inputHeaderH );
    inputSpreadsheet->setVerticalHeaderLabels( inputHeaderV );
    //inputSpreadsheet->setSizeAdjustPolicy(QTableWidget::AdjustToContents);

    weightedSpreadsheet = new Spreadsheet(numWeightedHeaderV,numWeightedHeaderH);
    central_widget->addTab(weightedSpreadsheet, "Weighted Table");
    weightedSpreadsheet->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    weightedSpreadsheet->setHorizontalHeaderLabels( weightedHeaderH );
    weightedSpreadsheet->setVerticalHeaderLabels( weightedHeaderV );
    //weightedSpreadsheet->setSizeAdjustPolicy(QTableWidget::AdjustToContents);

    setCentralWidget(central_widget);

    //QStringList horizHeader = {"Design", "Score", "Length", "Lp", "Ld", "Lf", "w", "t1", "t2", "d", "b", "s", "f", "n", "density", "mesh"};

    createActions();
    createContextMenu();
    createStatusBar();
    readSettings();

    setWindowTitle("UT Concrete Canoe - PANDA + POSSUM");
    setWindowIcon(QIcon(":/images/canoe_logo.png"));
    //setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
      event->accept();
}

void MainWindow::tutorial()
{
   QMessageBox::about(this, tr("Tutorial"),
            tr("<u>Hull Design using PANDA + POSSUM</u><br>"
               "<br>"
               "<b>1.</b> Define input setup and weighted table in Design -> Edit Designs (or use menu bar shortcut)<br>"
               "<b>2.</b> Iterate to find the most optimal design for the given inputs in Design -> Run Analysis<br>"
               "<b>3.</b> View the best design and associated score in main window<br>"
               "<b>4.</b> [Optional] Create additional versions of input setup and weighted table if needed<br>"
               "<b>5.</b> [Optional] Find the best canoe across all versions using Design -> Rank Designs<br>"
               "<b>6.</b> Output mesh for selected design(s) by checking the box(s) and clicking Design -> Mesh<br>"
               "<b>7.</b> Save and Exit<br>"));
}

void MainWindow::parameters()
{
   QMessageBox::about(this, tr("Parameters Definition"),
            tr("<u>Definition of Input Table Parameters</u><br>"
               "<br>"
               "<b>L</b> = Total length of canoe<br>"
               "<b>Lp</b> = Length of paddlers’ “box”<br>"
               "<b>Ld</b> = Distance from bow to deepest point of canoe<br>"
               "<b>Lf</b> = Distance from bow to first paddler<br>"
               "<b>W</b> = Nominal beam of canoe<br>"
               "<b>t1</b> = Waterline smoothing parameter (bow side)<br>"
               "<b>t2</b> = Waterline smoothing parameter (stern side)<br>"
               "<b>d</b> = Depth of canoe below the flange<br>"
               "<b>h</b> = Height of the flange<br>"
               "<b>b</b> = Bow rocker<br>"
               "<b>s</b> = Stern rocker<br>"
               "<b>f(x)</b> = Flare angle of the flange<br>"
               "<b>n</b> = Cross-section shape parameter<br>"));
}

void MainWindow::analysis()
{
//    Check if every cell has been filled
//    for(int i=0; i < inputSpreadsheet->rowCount(); i++)
//    {
//        for(int j=0; j < inputSpreadsheet->columnCount(); j++)
//        {
//            bool flag = inputSpreadsheet->item(i,j)->text().isNull();

//            if (!flag) /* the cell is not empty */
//            {
//                // do stuff
//            }
//            else /* the cell is empty */
//            {
//                // do stuff
//            }
//        }
//    }
    QMessageBox::StandardButton analysisDialog;
    analysisDialog = QMessageBox::question(this, "Analysis", "Run analysis?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (analysisDialog == QMessageBox::Yes) {
      qDebug() << "Yes was clicked";

      //QApplication::quit();
      saveInputToText();
      c = new Canoe;
      c->UIBulk();


    } else {
      qDebug() << "Yes was *not* clicked";
    }
    //RunAnalysis runanalysis(this);
}

void MainWindow::mesh()
{
    QMessageBox::StandardButton meshDialog;
    meshDialog = QMessageBox::question(this, "Mesh", "Generate mesh?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (meshDialog == QMessageBox::Yes) {
      qDebug() << "Yes was clicked";
      //QApplication::quit();
    } else {
      qDebug() << "Yes was *not* clicked";
    }
}

void MainWindow::final()
{

}

void MainWindow::createActions()
{
    // Identify current tab
    connect(central_widget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));

    // File: Exit, Print
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    fileMenu->addSeparator();

    const QIcon printIcon = QIcon(":/images/print.png");
    QAction *printAct = new QAction(printIcon, tr("&Print"), this);
    printAct->setStatusTip(tr("Print the results spreadsheet"));
    connect(printAct, &QAction::triggered, this, &MainWindow::print);
    fileMenu->addAction(printAct);
    fileToolBar->addAction(printAct);

    const QIcon exitIcon = QIcon(":/images/exit.png");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("&Exit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    // Edit: Cut, copy, paste, delete

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));

    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut the current selection's contents "
                               "to the clipboard"));
    connect(cutAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(cut()));
    editMenu->addAction(cutAction);
    editToolBar->addAction(cutAction);

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy the current selection's contents "
                                "to the clipboard"));
    connect(copyAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(copy()));
    editMenu->addAction(copyAction);
    editToolBar->addAction(copyAction);

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste the clipboard's contents into "
                                 "the current selection"));
    connect(pasteAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(paste()));
    editMenu->addAction(pasteAction);
    editToolBar->addAction(pasteAction);

    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setIcon(QIcon(":/images/delete.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Delete the current selection's "
                                  "contents"));
    connect(deleteAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(del()));
    editMenu->addAction(deleteAction);
    editToolBar->addAction(deleteAction);

    // Design: Run Analysis, Show Results, Generate Mesh

    QMenu *designMenu = menuBar()->addMenu(tr("&Design"));
    QToolBar *designToolBar = addToolBar(tr("Design"));

    const QIcon analysisIcon = QIcon(":/images/analyze.png");
    QAction *analysisAct = new QAction(analysisIcon, tr("&Run Analysis"), this);
    analysisAct->setStatusTip(tr("Iterate design options based on input setup and weighted table"));
    connect(analysisAct, &QAction::triggered, this, &MainWindow::analysis);
    designMenu->addAction(analysisAct);
    designToolBar->addAction(analysisAct);

    const QIcon finalIcon = QIcon(":/images/results.png");
    QAction *finalAct = new QAction(finalIcon, tr("&Show Results"), this);
    finalAct->setStatusTip(tr("Chose final design with best score"));
    connect(finalAct, &QAction::triggered, this, &MainWindow::final);
    designMenu->addAction(finalAct);
    designToolBar->addAction(finalAct);

    const QIcon meshIcon = QIcon(":/images/mesh.png");
    QAction *meshAct = new QAction(meshIcon, tr("&Generate Mesh"), this);
    meshAct->setStatusTip(tr("Mesh final design and create output files"));
    connect(meshAct, &QAction::triggered, this, &MainWindow::mesh);
    designMenu->addAction(meshAct);
    designToolBar->addAction(meshAct);

    // Help: Tutorial, Parameters

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *tutAct = helpMenu->addAction(tr("&Tutorial"), this, &MainWindow::tutorial);
    tutAct->setStatusTip(tr("Show a step-by-step tutorial on how to use PANDA + POSSUM"));

    QAction *paramAct = helpMenu->addAction(tr("&Parameters"), this, &MainWindow::parameters);
    paramAct->setStatusTip(tr("Show the definitions for parameters used in the input table"));

}

// Clear selection in table not located in current tab
void MainWindow::tabSelected(){

    // connect and disconnect signals depending on which tab is active
    if(central_widget->currentIndex() == 0){

        connect(cutAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(cut()));
        connect(copyAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(copy()));
        connect(pasteAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(paste()));
        connect(deleteAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(del()));

        disconnect(cutAction, SIGNAL(triggered()), weightedSpreadsheet, SLOT(cut()));
        disconnect(copyAction, SIGNAL(triggered()), weightedSpreadsheet, SLOT(copy()));
        disconnect(pasteAction, SIGNAL(triggered()), weightedSpreadsheet, SLOT(paste()));
        disconnect(deleteAction, SIGNAL(triggered()), weightedSpreadsheet, SLOT(del()));

    }
    if(central_widget->currentIndex() == 1){

        connect(cutAction, SIGNAL(triggered()), weightedSpreadsheet, SLOT(cut()));
        connect(copyAction, SIGNAL(triggered()), weightedSpreadsheet, SLOT(copy()));
        connect(pasteAction, SIGNAL(triggered()), weightedSpreadsheet, SLOT(paste()));
        connect(deleteAction, SIGNAL(triggered()), weightedSpreadsheet, SLOT(del()));

        disconnect(cutAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(cut()));
        disconnect(copyAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(copy()));
        disconnect(pasteAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(paste()));
        disconnect(deleteAction, SIGNAL(triggered()), inputSpreadsheet, SLOT(del()));

    }
}

void MainWindow::createContextMenu()
{
    inputSpreadsheet->addAction(cutAction);
    inputSpreadsheet->addAction(copyAction);
    inputSpreadsheet->addAction(pasteAction);
    inputSpreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);

    weightedSpreadsheet->addAction(cutAction);
    weightedSpreadsheet->addAction(copyAction);
    weightedSpreadsheet->addAction(pasteAction);
    weightedSpreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

//TO DO: separate create actions function into create tool bar, create menu etc.

void MainWindow::createStatusBar()

{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings()

{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

void MainWindow::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printpreviewdialog)
    QPrinter printer(QPrinter::ScreenResolution);
    QPrintPreviewDialog dlg(&printer);
    PrintView view;
    view.setModel(inputSpreadsheet->model());
    connect(&dlg, &QPrintPreviewDialog::paintRequested, &view, &PrintView::print);
    dlg.exec();
#endif
}

void MainWindow::createInputTable()
{

}

//void MainWindow::saveInputToText()
//{
//    QString textData;
//    int rows = inputSpreadsheet->rowCount();
//    int columns = inputSpreadsheet->columnCount();

//    for (int i = 0; i < rows; i++) {
//        for (int j = 0; j < columns; j++) {
//                textData += inputSpreadsheet->item(i,j)).toString();
//                textData += "\t";      // for .csv file format
//        }
//        textData += "\n";             // (optional: for new line segmentation)
//    }

//    // [Save to file] (header file <QFile> needed)
//    // .csv
//    QFile csvFile("test.csv");
//    if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

//        QTextStream out(&csvFile);
//        out << textData;

//        csvFile.close();
//    }

//    // .txt
//    QFile txtFile("test.txt");
//    if(txtFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

//        QTextStream out(&txtFile);
//        out << textData;

//        txtFile.close();
//    }
//}

void MainWindow::saveInputToText()
{ 
    // Save input table to txt in directory containing .exe
    //QString inputPath = QCoreApplication::applicationDirPath().append("/TEST_INPUT.txt");
    QString inputPath = "TEST_inputsetup.txt";
    QFile inputData(inputPath);

    if (inputData.open(QFile::WriteOnly))
    {
        QTextStream input(&inputData);
        //output << "success";
        qDebug() << "file created";

        int rows = inputSpreadsheet->rowCount();
        int columns = inputSpreadsheet->columnCount();

        input << rows << "\n";

        for (int i = 0; i < rows; i++) {
            input << inputHeaderV[i] << '\t';
            for (int j = 0; j < columns; j++) {
                    input << inputSpreadsheet->item(i,j)->text() << '\t';    // for .csv file format
            }
            input << "\n";             // (optional: for new line segmentation)
        }

        inputData.close();
     }

    // Save weighted table
    //QString weightedPath = QCoreApplication::applicationDirPath().append("/TEST_WEIGHTED.txt");
    QString weightedPath = "TEST_weighttable.txt";
    QFile weightedData(weightedPath);

    if (weightedData.open(QFile::WriteOnly))
    {
        QTextStream weighted(&weightedData);
        //output << "success";
        qDebug() << "file created";

        int rows = weightedSpreadsheet->rowCount();
        int columns = weightedSpreadsheet->columnCount();

        // << rows << "\n";

        for (int i = 0; i < rows; i++) {
            weighted << weightedHeaderV[i] << '\t';
            for (int j = 0; j < columns; j++) {
                    weighted << weightedSpreadsheet->item(i,j)->text() << '\t';    // for .csv file format
            }
            weighted << "\n";             // (optional: for new line segmentation)
        }

        weightedData.close();
     }
}
