#include "runanalysis.h"
#include "editdesign.h"
#include "model.h"
#include <QWidget>
#include <QtWidgets>
#include <iostream>

std::map <std::string, Model> RunAnalysis::allD;

RunAnalysis::RunAnalysis(/*std::map <std::string, Model> *allDesign, */QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    // Change labels in button box
    analysisButtonBox->button(QDialogButtonBox::Ok)->setText("Run");
//    allD = *allDesign;
    createActions();
    //QLineEdit nameLineEdit;
}

void RunAnalysis::setDesign(std::map <std::string, Model> &alldesign)
{
    allD = alldesign;
}

void RunAnalysis::createActions()
{
    analysisTable->model()->removeRows(0,analysisTable->rowCount());
    qDebug() << "INSIDE analysis";
   for (const auto& [key, value] : allD)
   {
       int row = analysisTable->rowCount();
       analysisTable->insertRow(row);
       QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(key));
       qDebug() << QString::fromStdString(key);


       analysisTable->setItem(row, 1, newItem);
   }

   connect(analysisButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(analysisButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
