#include "editdesign.h"
#include <QWidget>
#include <QtWidgets>
#include <iostream>

//! [0]
EditDesign::EditDesign(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    createActions();
}

void EditDesign::createActions()
{
    const QIcon addIcon = QIcon(":/images/add.png");
    addDesignButton -> setIcon(addIcon);
    connect(addDesignButton, &QToolButton::clicked, this, &EditDesign::addDesign);

    const QIcon deleteIcon = QIcon(":/images/delete.png");
    deleteDesignButton -> setIcon(deleteIcon);
    connect(deleteDesignButton, &QToolButton::clicked, this, &EditDesign::deleteDesign);

    // Update inputSetup and weightedTable each time a different cell is clicked
    connect(inputSetupTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateInputSetup()));
    connect(weightedTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateWeightedTable()));

    // Update line edit after name is modified in design table
    connect(editDesignTable, SIGNAL(cellChanged()), this, SLOT(nameChange()));
}

void EditDesign::addDesign()
{
    int row = editDesignTable->rowCount();
    std::string name = "New Design"+std::to_string(row);
    editDesignTable->insertRow(row);
    QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(name));
    editDesignTable->setItem(row, 0, newItem);
}

void EditDesign::deleteDesign()
{
    QModelIndexList indexes =  editDesignTable->selectionModel()->selectedRows();
    int countRow = indexes.count();
    for( int i = countRow; i > 0; i--)
           editDesignTable->removeRow( indexes.at(i-1).row() );
    //editDesignTable->setCurrentCell(0, 0);
}

void EditDesign::updateInputSetup()
{
    // if table is not empty
    if (inputSetupTable->item(0,0)->text() != "")
    {
        int row = inputSetupTable->rowCount();
        int col = inputSetupTable->columnCount();
        std::vector<std::vector<double>> inputSetup(row, std::vector<double>(col));

        for( int i = row; i > 0; i--)
        {
            for ( int j = row; j > 0; j--)
            {
                double val = inputSetupTable->item(i,j)->text().toDouble();
                inputSetup[row][col] = val;
                qDebug() << val;
            }
        }
    }
}

void EditDesign::updateWeightedTable()
{
    // if table is not empty
    if (weightedTable->item(0,0)->text() != "")
    {
        int row = weightedTable->rowCount();
        int col = weightedTable->columnCount();
        std::vector<std::vector<double>> weighted(row, std::vector<double>(col));

        for( int i = row; i > 0; i--)
        {
            for ( int j = row; j > 0; j--)
            {
                double val = weightedTable->item(i,j)->text().toDouble();
                weighted[row][col] = val;
                qDebug() << val;
            }
        }
    }
}

void EditDesign::updateDesign()
{

}

void EditDesign::nameChange()
{

}
