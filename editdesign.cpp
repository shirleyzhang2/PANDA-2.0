#include "editdesign.h"
#include <QWidget>

//! [0]
EditDesign::EditDesign(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
}
