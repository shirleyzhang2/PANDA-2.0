#include <QtWidgets>
#include "mainwindow.h"
#include "editdesign.h"
#include "runanalysis.h"
#include "printview.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport>
#endif

MainWindow::MainWindow(int rows, int cols, QWidget *parent)
    : QMainWindow(parent),
      textEdit(new QPlainTextEdit),
      table(new QTableWidget(rows, cols, this))
      //runanalysis(new RunAnalysis(&allDesign, this))

{
    setCentralWidget(table);
    table->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    const char *params[16] = {"Design", "Score", "Length", "Lp", "Ld", "Lf", "w", "t1", "t2", "d", "b", "s", "f", "n", "density", "mesh"};

    for (int i = 0; i < 16; i++)
    {
        table->setHorizontalHeaderItem(i, new QTableWidgetItem(QString(params[i])));
    }

    // Create checkboxes

    for (int j = 0; j < table->rowCount(); j++)
    {
        QCheckBox * checkB = new QCheckBox(this); //Create checkbox
        checkB->setCheckState(Qt::Unchecked); //Set the status
        QWidget *w = new QWidget(this); //Create a widget
        QHBoxLayout *hLayout = new QHBoxLayout(); //Create layout
        hLayout->addWidget(checkB); //Add checkbox
        hLayout->setMargin(0); //Set the edge distance otherwise it will be difficult to see
        hLayout->setAlignment(checkB, Qt::AlignCenter); //Center
        w->setLayout(hLayout); //Set the layout of the widget
        table->setCellWidget(j, 15, w); //Place the widget in the table

    }

    createActions();
    createStatusBar();
    readSettings();

    setWindowTitle("UT Concrete Canoe - PANDA + POSSUM");
    setWindowIcon(QIcon(":/images/canoe_logo.png"));
    setCurrentFile(QString());
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

void MainWindow::design()
{
    EditDesign editDesign(this);
    editDesign.setDesignForEdit(allDesign);
    editDesign.exec();
    allDesign = editDesign.getDesign();

    for (const auto& [key, value] : allDesign)
    {
        qDebug() << QString::fromStdString(key);
    }
}

void MainWindow::analysis()
{
    qDebug() << "When is this called??";
    //RunAnalysis analysis(this);
    //qDebug() << "Finish running analysis";
    //qDebug() << "Analysis";
    for (const auto& [key, value] : allDesign)
    {
        qDebug() << QString::fromStdString(key);
    }
    RunAnalysis runanalysis(this);
    runanalysis.setDesign(allDesign);
    runanalysis.exec();
}

void MainWindow::rank()
{
    QMessageBox::about(this, tr("Rank Designs"),
              tr(""));
}

void MainWindow::mesh()
{
    QMessageBox::about(this, tr("Mesh"),
              tr(""));
}

void MainWindow::documentWasModified()
{
    setWindowModified(textEdit->document()->isModified());
}

void MainWindow::createActions()
{

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

    // Edit: Copy, Paste

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));

    const QIcon copyIcon = QIcon(":/images/copy.png");
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setStatusTip(tr("Copy selected cells"));
    connect(copyAct, &QAction::triggered, this, &MainWindow::copy);
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);

    const QIcon pasteIcon = QIcon(":/images/paste.png");
    QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setStatusTip(tr("Paste to spreadsheet"));
    connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);

    // Design: Edit Design, Run Analysis, Rank Designs, Generate Mesh

    QMenu *designMenu = menuBar()->addMenu(tr("&Design"));
    QToolBar *designToolBar = addToolBar(tr("Design"));

    const QIcon designIcon = QIcon(":/images/design.png");
    QAction *designAct = new QAction(designIcon, tr("&Edit Design"), this);
    designAct->setStatusTip(tr("Edit Input Setup and Weighted Table"));
    connect(designAct, &QAction::triggered, this, &MainWindow::design);
    designMenu->addAction(designAct);
    designToolBar->addAction(designAct);

    const QIcon analysisIcon = QIcon(":/images/analyze.png");
    QAction *analysisAct = new QAction(analysisIcon, tr("&Run Analysis"), this);
    analysisAct->setStatusTip(tr("Edit Input Setup and Weighted Table"));
    connect(analysisAct, &QAction::triggered, this, &MainWindow::analysis);
    designToolBar->addAction(analysisAct);
    designToolBar->addAction(analysisAct);

    const QIcon rankIcon = QIcon(":/images/rank.png");
    QAction *rankAct = new QAction(rankIcon, tr("&Rank Designs"), this);
    rankAct->setStatusTip(tr("Edit Input Setup and Weighted Table"));
    connect(rankAct, &QAction::triggered, this, &MainWindow::rank);
    designToolBar->addAction(rankAct);
    designToolBar->addAction(rankAct);

    const QIcon meshIcon = QIcon(":/images/mesh.png");
    QAction *meshAct = new QAction(meshIcon, tr("&Generate Mesh"), this);
    meshAct->setStatusTip(tr("Edit Input Setup and Weighted Table"));
    connect(meshAct, &QAction::triggered, this, &MainWindow::mesh);
    designToolBar->addAction(meshAct);
    designToolBar->addAction(meshAct);

    // Help: Tutorial, Parameters

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *tutAct = helpMenu->addAction(tr("&Tutorial"), this, &MainWindow::tutorial);
    tutAct->setStatusTip(tr("Show a step-by-step tutorial on how to use PANDA + POSSUM"));

    QAction *paramAct = helpMenu->addAction(tr("&Parameters"), this, &MainWindow::parameters);
    paramAct->setStatusTip(tr("Show the definitions for parameters used in the input table"));

}

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

void MainWindow::setCurrentFile(const QString &fileName)

{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName)

{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printpreviewdialog)
    QPrinter printer(QPrinter::ScreenResolution);
    QPrintPreviewDialog dlg(&printer);
    PrintView view;
    view.setModel(table->model());
    connect(&dlg, &QPrintPreviewDialog::paintRequested, &view, &PrintView::print);
    dlg.exec();
#endif
}

void MainWindow::copy()
{

}

void MainWindow::paste()
{

}

