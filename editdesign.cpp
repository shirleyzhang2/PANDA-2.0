#include "editdesign.h"
#include <QWidget>
#include <QtWidgets>
#include <iostream>

std::map <std::string, Model> EditDesign::allDesignForEdit;

//! [0]
EditDesign::EditDesign(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    //EditDesign::allDesignForEdit = {};
    createActions();
}

void EditDesign::setDesignForEdit(std::map <std::string, Model> &alldesign)
{
    allDesignForEdit = alldesign;
}

void EditDesign::createActions()
{
    editDesignTable->setSortingEnabled(false);

    const QIcon addIcon = QIcon(":/images/add.png");
    addDesignButton -> setIcon(addIcon);
    connect(addDesignButton, &QAbstractButton::clicked, this, &EditDesign::addDesign);

    const QIcon deleteIcon = QIcon(":/images/delete.png");
    deleteDesignButton -> setIcon(deleteIcon);
    connect(deleteDesignButton, &QAbstractButton::clicked, this, &EditDesign::deleteDesign);

    connect(editDesignTable, SIGNAL(itemClicked()), this, SLOT(updateDesign()));

    // Update inputSetup and weightedTable each time a different cell is clicked
    connect(inputSetupTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateInputSetup()));

    connect(weightedTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateWeightedTable()));

    connect(editDesignTable, SIGNAL(cellChanged(int,int)), this, SLOT(nameChange()));

    // Set ok and cancel buttons
    connect(designButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(designButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

    populate();

}

void EditDesign::addDesign()
{
    int id = 1;
    static std::string name;
    name = "New Design "+std::to_string(id);

    auto iter = allDesignForEdit.begin();
    while (iter != allDesignForEdit.end())
    {
        id += 1;
        ++iter;
        name = "New Design "+std::to_string(id);
    }
    allDesignForEdit[name] = Model();
    currDesignForEdit = name;

    populate();
    clearTables();
}

void EditDesign::deleteDesign()
{
    //Add warning to prevent user from deleting the last design
    QList<QTableWidgetSelectionRange> sRangeList = editDesignTable->selectedRanges();
    for(const auto &p : qAsConst(sRangeList))
    {
        for (auto i = p.topRow() + p.rowCount() - 1; i > p.topRow() - 1; i--)
        {
            std::string key = editDesignTable->item(i,0)->text().toStdString();
            allDesignForEdit.erase(key);
            editDesignTable->removeRow(i);
        }
    }
    clearTables();
}

void EditDesign::updateInputSetup()
{
    currDesignForEdit = editDesignTable->currentItem()->text().toStdString();

    for( int i = getInputRow(); i > 0; i--)
    {
        for ( int j = getInputCol(); j > 0; j--)
        {
            double val = inputSetupTable->item(i,j)->text().toDouble();
            allDesignForEdit[currDesignForEdit].inputSetupInfo[getInputRow()][getInputCol()] = val;
            qDebug() << val;
        }
    }
}

void EditDesign::updateWeightedTable()
{
    currDesignForEdit = editDesignTable->currentItem()->text().toStdString();

    for( int i = getWeightedRow(); i > 0; i--)
    {
        for ( int j = getWeightedCol(); j > 0; j--)
        {
            double val = weightedTable->item(i,j)->text().toDouble();
            allDesignForEdit[currDesignForEdit].weightedTableInfo[i][j] = val;
            qDebug() << val;
        }
    }
}

void EditDesign::updateDesign()
{
    clearTables();
    currDesignForEdit = editDesignTable->currentItem()->text().toStdString();

    for (int i = 0; i < getInputRow(); ++i)
    {
            for (int j = 0; j < getInputCol(); ++j)
            {
                inputSetupTable->setItem(i,j, new QTableWidgetItem(allDesignForEdit[currDesignForEdit].inputSetupInfo[i][j]));
            }
    }

    for (int i = 0; i < getWeightedRow(); ++i)
    {
            for (int j = 0; j < getWeightedCol(); ++j)
            {
                inputSetupTable->setItem(i,j, new QTableWidgetItem(allDesignForEdit[currDesignForEdit].weightedTableInfo[i][j]));
            }
    }
}

void EditDesign::populate()
{
    editDesignTable->model()->removeRows(0,editDesignTable->rowCount());
    for (const auto& [key, value] : allDesignForEdit)
    {
        int row = editDesignTable->rowCount();
        editDesignTable->insertRow(row);
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(key));

        editDesignTable->setItem(row, 0, newItem);
        qDebug() << QString::fromStdString(key);
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

        auto nodeHandler = allDesignForEdit.extract(currDesignForEdit);
        nodeHandler.key() = newDesign;
        allDesignForEdit.insert(std::move(nodeHandler));

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
