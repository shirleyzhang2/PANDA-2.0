#include <QtWidgets>
#include "warningmessage.h"

WarningMessage::WarningMessage(QString message, QWidget *parent)
    : QMessageBox(parent)
{
    setIcon(QMessageBox::Critical);
    setText("Error");
    setInformativeText(message);
    setWindowTitle("Error");
    //exec();
}
