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
    connect(loadDesignButton, &QPushButton::clicked, this, &RunAnalysis::loadDesign);


   connect(analysisButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(analysisButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
}


void RunAnalysis::loadDesign()
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

       //add checkbox
       QCheckBox * checkB = new QCheckBox(this); //Create checkbox
       checkB->setCheckState(Qt::Unchecked); //Set the status
       QWidget *w = new QWidget(this); //Create a widget
       QHBoxLayout *hLayout = new QHBoxLayout(); //Create layout
       hLayout->addWidget(checkB); //Add checkbox
       hLayout->setMargin(0); //Set the edge distance otherwise it will be difficult to see
       hLayout->setAlignment(checkB, Qt::AlignCenter); //Center
       w->setLayout(hLayout); //Set the layout of the widget
       constexpr int checkbox_loc{ 0 };
       analysisTable->setCellWidget(row, checkbox_loc, w); //Place the widget in the table
   }
}
