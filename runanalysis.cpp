#include "runanalysis.h"
#include "editdesign.h"
#include <QWidget>
#include <QtWidgets>
#include <iostream>

RunAnalysis::RunAnalysis(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    // Change labels in button box
    analysisButtonBox->button(QDialogButtonBox::Ok)->setText("Run");

    createActions();
    //QLineEdit nameLineEdit;
}

void RunAnalysis::createActions()
{
    EditDesign design;
   for (const auto& [key, value] : design.allDesign)
   {
       int row = analysisTable->rowCount();
       analysisTable->insertRow(row);
       QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(key));

       analysisTable->setItem(row, 1, newItem);
   }

   connect(analysisButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(analysisButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
