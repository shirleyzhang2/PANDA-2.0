#include "editdesign.h"
#include <QWidget>
#include <QtWidgets>
#include <iostream>

//static std::string currDesign;

//! [0]
EditDesign::EditDesign(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    createActions();
    //QLineEdit nameLineEdit;
}

void EditDesign::createActions()
{
    currDesign = editDesignTable->item(0,0)->text().toStdString();
    allDesign[currDesign] = Design();
    editDesignTable->setSortingEnabled(false);

    const QIcon addIcon = QIcon(":/images/add.png");
    addDesignButton -> setIcon(addIcon);
    connect(addDesignButton, &QToolButton::clicked, this, &EditDesign::addDesign);

    const QIcon deleteIcon = QIcon(":/images/delete.png");
    deleteDesignButton -> setIcon(deleteIcon);
    connect(deleteDesignButton, &QToolButton::clicked, this, &EditDesign::deleteDesign);

    connect(editDesignTable, SIGNAL(itemClicked()), this, SLOT(updateDesign()));

    // Update inputSetup and weightedTable each time a different cell is clicked
    connect(inputSetupTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateInputSetup()));

    connect(weightedTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateWeightedTable()));

    connect(editDesignTable, SIGNAL(cellChanged(int,int)), this, SLOT(nameChange()));

}

void EditDesign::addDesign()
{
    int id = 1;
    static std::string name;
    auto iter = allDesign.begin();
    while (iter != allDesign.end())
    {
        id += 1;
        ++iter;
        name = "New Design "+std::to_string(id);
    }
    allDesign[name] = Design();

    populate();
    clearTables();
}

void EditDesign::deleteDesign()
{
    //Add warning to prevent user from deleting the last design
    /*
    QModelIndexList indexes =  editDesignTable->selectionModel()->selectedRows();
    int countRow = indexes.count();
    for( int i = countRow; i > 0; i--)
    {
        std::map <std::string, Design>::iterator it;
        std::string name = editDesignTable->item(countRow,0)->text().toStdString();
        it = allDesign.find (name);
            allDesign.erase (it);
        editDesignTable->removeRow( indexes.at(i-1).row() );
    }
    */
    QList<QTableWidgetSelectionRange> sRangeList = editDesignTable->selectedRanges();
    for(const auto &p : qAsConst(sRangeList))
    {
        for (auto i = p.topRow() + p.rowCount() - 1; i > p.topRow() - 1; i--)
        {
            std::string key = editDesignTable->item(i,0)->text().toStdString();
            allDesign.erase(key);
            editDesignTable->removeRow(i);

        }
    }
    clearTables();
}

void EditDesign::updateInputSetup()
{
    currDesign = editDesignTable->currentItem()->text().toStdString();

    for( int i = getInputRow(); i > 0; i--)
    {
        for ( int j = getInputCol(); j > 0; j--)
        {
            double val = inputSetupTable->item(i,j)->text().toDouble();
            allDesign[currDesign].inputSetupInfo[getInputRow()][getInputCol()] = val;
            qDebug() << val;
        }
    }
}

void EditDesign::updateWeightedTable()
{
    currDesign = editDesignTable->currentItem()->text().toStdString();

    for( int i = getWeightedRow(); i > 0; i--)
    {
        for ( int j = getWeightedCol(); j > 0; j--)
        {
            double val = weightedTable->item(i,j)->text().toDouble();
            allDesign[currDesign].weightedTableInfo[i][j] = val;
            qDebug() << val;
        }
    }
}

void EditDesign::updateDesign()
{
    clearTables();
    currDesign = editDesignTable->currentItem()->text().toStdString();

    for (int i = 0; i < getInputRow(); ++i)
    {
            for (int j = 0; j < getInputCol(); ++j)
            {
                inputSetupTable->setItem(i,j, new QTableWidgetItem(allDesign[currDesign].inputSetupInfo[i][j]));
            }
    }

    for (int i = 0; i < getWeightedRow(); ++i)
    {
            for (int j = 0; j < getWeightedCol(); ++j)
            {
                inputSetupTable->setItem(i,j, new QTableWidgetItem(allDesign[currDesign].weightedTableInfo[i][j]));
            }
    }
}

void EditDesign::populate()
{
//    editDesignTable->setRowCount(0);
    editDesignTable->model()->removeRows(0,editDesignTable->rowCount());

    for (const auto& [key, value] : allDesign)
    {
        int row = editDesignTable->rowCount();
        editDesignTable->insertRow(row);
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(key));

        editDesignTable->setItem(row, 0, newItem);
    }
}

void EditDesign::nameChange()
{
    //qDebug() << QString::fromStdString(currDesign);
    int row = editDesignTable->currentRow();
    // This if statements prevents program from crashing when adding new rows
    if (row != -1)
    {
        const std::string newDesign = editDesignTable->currentItem()->text().toStdString();

        auto nodeHandler = allDesign.extract(currDesign);
        nodeHandler.key() = newDesign;
        allDesign.insert(std::move(nodeHandler));

        updateDesign();
    }

}

int EditDesign::getInputRow()
{
    return inputSetupTable->rowCount();
}

int EditDesign::getInputCol()
{
    return inputSetupTable->columnCount();
}

int EditDesign::getWeightedRow()
{
    return weightedTable->rowCount();
}

int EditDesign::getWeightedCol()
{
    return weightedTable->rowCount();
}

void EditDesign::clearTables()
{
    //inputSetupTable->setRowCount(0);
    //weightedTable->setRowCount(0);
    QStringList inputHeader = {"Min", "Max", "Value"};
    QStringList weightedHeader = {"Target Value", "Standard Deviation", "Weight"};
    for (int i=0; i<3; i++)
    {
        inputSetupTable->removeColumn(i);
        weightedTable->removeColumn(i);

        inputSetupTable->insertColumn(i);
        weightedTable->insertColumn(i);
    }
    inputSetupTable->setHorizontalHeaderLabels(inputHeader);
    weightedTable->setHorizontalHeaderLabels(weightedHeader);

}

