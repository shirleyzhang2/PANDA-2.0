/****************************************************************************
**
** Created for the University of Toronto Concrete Canoe Team (2020-2021)
** Contributors: David Ruggiero, Rick Liu, Shirley Zhang, Ernie Lee, Reynold Chan
**
****************************************************************************/

//! [0]
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "mainwindow.h"
#include "editdesign.h"
#include "runanalysis.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(PANDA_POSSUM);
#ifdef Q_OS_ANDROID
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    EditDesign editDesign;
    RunAnalysis runanalysis;
    QCoreApplication::setOrganizationName("University of Toronto Concrete Canoe Team");
    QCoreApplication::setApplicationName("PANDA_POSSUM");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    MainWindow mainWin(4,16);
    mainWin.show();
    return app.exec();
}
//! [0]
