/****************************************************************************
**
** Created for the University of Toronto Concrete Canoe Team (2020)
** Contributors: David Ruggiero, Rick Liu, Shirley Zhang, Ernie Lee, Reynold Chan
**
****************************************************************************/

//! [0]
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(PANDA_POSSUM);
#ifdef Q_OS_ANDROID
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("University of Toronto Concrete Canoe Team");
    QCoreApplication::setApplicationName("PANDA_POSSUM");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    //parser.setApplicationDescription(QCoreApplication::applicationName());
    //parser.addHelpOption();
    //parser.addVersionOption();
    //parser.addPositionalArgument("file", "The file to open.");
    //parser.process(app);

    MainWindow mainWin(4,16);
    //if (!parser.positionalArguments().isEmpty())
    //    mainWin.loadFile(parser.positionalArguments().first());
    mainWin.show();
    return app.exec();
}
//! [0]
