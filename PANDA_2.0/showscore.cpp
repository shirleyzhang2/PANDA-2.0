#include "showscore.h"
//#include "editdesign.h"
//#include "model.h"
#include "spreadsheet.h"
//#include "canoe.h"
#include <QWidget>
#include <QtWidgets>
#include <iostream>

ShowScore::ShowScore(QWidget *parent)
    : QDialog(parent)
{
    // TO DO: check if results exist

    //load results file
    QFile file("TEST_output.txt");
    QTextStream in(&file);
    QString allData;
    QStringList rowData;

    if(file.open(QIODevice::ReadOnly)){
        allData=in.readAll();
        rowData=allData.split("\n");
        file.close();
    }

    setWindowTitle("Results");
    resize(1000,600);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

    //QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    //mainLayout->addWidget(mainWidget);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    resultsSpreadsheet = new Spreadsheet(rowData.size()-1,numresultsHeaderH);
    resultsSpreadsheet->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultsSpreadsheet -> setHorizontalHeaderLabels( resultsHeaderH );
    mainLayout->addWidget(resultsSpreadsheet);
    mainLayout->addWidget(buttonBox);

    for(int row=0;row<rowData.count();row++)
    {
        QStringList cellData = rowData.at(row).split("\t");

        for(int col=0;col < cellData.count(); col++){
            QTableWidgetItem *item = new QTableWidgetItem(cellData[col]);
            resultsSpreadsheet->setItem(row,col,item);
        }
    }
    //exec();
    //setAttribute(Qt::WA_DeleteOnClose);
}
