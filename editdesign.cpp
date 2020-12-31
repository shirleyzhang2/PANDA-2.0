#include "editdesign.h"
#include "warningmessage.h"
#include "spreadsheet.h"
#include <QWidget>
#include <QtWidgets>
#include <iostream>

std::map <std::string, Model> EditDesign::allDesignForEdit;

//! [0]
EditDesign::EditDesign(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    // Input Setup
    inputSetupTable = new Spreadsheet(13,3);
    tabWidget->addTab(inputSetupTable, "Input Setup");
    //inputSetupTable->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
    inputSetupTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //inputSetupTable->horizontalHeader()->setStretchLastSection(true);
    inputSetupTable->setHorizontalHeaderLabels( inputHeaderH );
    inputSetupTable->setVerticalHeaderLabels( inputHeaderV );
    inputSetupTable->setCurrentCell(0, 0);

    // Weighted Table
    weightedTable = new Spreadsheet(9,3);
    tabWidget->addTab(weightedTable, "Weighted Table");
    //weightedTable->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
    weightedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //weightedTable->horizontalHeader()->setStretchLastSection(true);
    weightedTable->setHorizontalHeaderLabels( weightedHeaderH );
    weightedTable->setVerticalHeaderLabels( weightedHeaderV );
    weightedTable->setCurrentCell(0, 0);

    EditDesign::allDesignForEdit = {};

    createActions();
    createContextMenu();
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

    //connect(editDesignTable, SIGNAL(itemClicked()), this, SLOT(updateDesign()));

    connect(confirmButton, &QAbstractButton::clicked, this, &EditDesign::updateInput);

    connect(editDesignTable, SIGNAL(cellChanged(int,int)), this, SLOT(nameChange()));

    // Set ok and cancel buttons
    connect(designButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(designButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

    // Check current tab
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));

    // Cut, copy, paste, delete
    cutAction = new QAction(tr("Cu&t"), this);
    //cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut the current selection's contents "
                               "to the clipboard"));

    copyAction = new QAction(tr("&Copy"), this);
    //copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy the current selection's contents "
                                "to the clipboard"));

    pasteAction = new QAction(tr("&Paste"), this);
    //pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste the clipboard's contents into "
                                 "the current selection"));

    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Delete the current selection's "
                                  "contents"));

    // connect signals and slots for input setup table upon initialization
    connect(cutAction, SIGNAL(triggered()), inputSetupTable, SLOT(cut()));
    connect(copyAction, SIGNAL(triggered()), inputSetupTable, SLOT(copy()));
    connect(pasteAction, SIGNAL(triggered()), inputSetupTable, SLOT(paste()));
    connect(deleteAction, SIGNAL(triggered()), inputSetupTable, SLOT(del()));

    populate();

}

void EditDesign::createContextMenu()
{
    inputSetupTable->addAction(cutAction);
    inputSetupTable->addAction(copyAction);
    inputSetupTable->addAction(pasteAction);
    inputSetupTable->setContextMenuPolicy(Qt::ActionsContextMenu);

    weightedTable->addAction(cutAction);
    weightedTable->addAction(copyAction);
    weightedTable->addAction(pasteAction);
    weightedTable->setContextMenuPolicy(Qt::ActionsContextMenu);
}

// Clear selection in table not located in current tab
void EditDesign::tabSelected(){

    // connect and disconnect signals depending on which tab is active
    if(tabWidget->currentIndex() == 0){

        connect(cutAction, SIGNAL(triggered()), inputSetupTable, SLOT(cut()));
        connect(copyAction, SIGNAL(triggered()), inputSetupTable, SLOT(copy()));
        connect(pasteAction, SIGNAL(triggered()), inputSetupTable, SLOT(paste()));
        connect(deleteAction, SIGNAL(triggered()), inputSetupTable, SLOT(del()));

        disconnect(cutAction, SIGNAL(triggered()), weightedTable, SLOT(cut()));
        disconnect(copyAction, SIGNAL(triggered()), weightedTable, SLOT(copy()));
        disconnect(pasteAction, SIGNAL(triggered()), weightedTable, SLOT(paste()));
        disconnect(deleteAction, SIGNAL(triggered()), weightedTable, SLOT(del()));

    }
    if(tabWidget->currentIndex()==1){

        connect(cutAction, SIGNAL(triggered()), weightedTable, SLOT(cut()));
        connect(copyAction, SIGNAL(triggered()), weightedTable, SLOT(copy()));
        connect(pasteAction, SIGNAL(triggered()), weightedTable, SLOT(paste()));
        connect(deleteAction, SIGNAL(triggered()), weightedTable, SLOT(del()));

        disconnect(cutAction, SIGNAL(triggered()), inputSetupTable, SLOT(cut()));
        disconnect(copyAction, SIGNAL(triggered()), inputSetupTable, SLOT(copy()));
        disconnect(pasteAction, SIGNAL(triggered()), inputSetupTable, SLOT(paste()));
        disconnect(deleteAction, SIGNAL(triggered()), inputSetupTable, SLOT(del()));

//        QModelIndex newIndex  = weightedTable->model()->index(0,0);
//        //inputSetupTable->selectionModel()->select(newIndex, QItemSelectionModel::Select);
//        weightedTable->setCurrentIndex(newIndex);
//        inputSetupTable->clearSelection();

    }
}

void EditDesign::addDesign()
{
    // TO DO: check if design has been confirmed (table filled out, button pressed)
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

void EditDesign::updateInput()
{
    QTableWidgetItem *design =editDesignTable->item(0,0);
    if (!design || design->text().isEmpty())
    {
        WarningMessage warning("Please add a new design first!");
        warning.exec();
        return;
    }
    else
    {
        currDesignForEdit = editDesignTable->currentItem()->text().toStdString();

        // Input setup
        for( int i = 0; i < constants::input_row; ++i)
        {
            for ( int j = 0; j < constants::input_col; ++j)
            {
                QTableWidgetItem *input =inputSetupTable->item(i,j);
                if (!input || input->text().isEmpty())
                {
                    WarningMessage warning("Please complete all required fields in input setup and weighted table before updating design");
                    warning.exec();
                    return;
                }
                else
                {
                    allDesignForEdit[currDesignForEdit].inputSetupInfo[i][j] = input->text().toDouble();
                    //qDebug() << allDesignForEdit[currDesignForEdit].inputSetupInfo[i][j];
                }
            }
        }

        // Weighted table
        for( int i = 0; i < constants::weighted_row; ++i)
        {
            for ( int j = 0; j < constants::weighted_col; ++j)
            {
                QTableWidgetItem *weighted =weightedTable->item(i,j);
                if (!weighted || weighted->text().isEmpty())
                {
                    WarningMessage warning("Please complete all required fields in input setup and weighted table before updating design");
                    warning.exec();
                    return;
                }
                else
                {
                    allDesignForEdit[currDesignForEdit].weightedTableInfo[i][j] = weighted->text().toDouble();
                    qDebug() << allDesignForEdit[currDesignForEdit].weightedTableInfo[i][j];
                }
            }
        }

    }


}

//void EditDesign::updateWeightedTable()
//{
//    currDesignForEdit = editDesignTable->currentItem()->text().toStdString();
//    WarningMessage warning("Please complete all required fields in input setup and weighted table before updating design");
//    warning.exec();

//    for( int i = constants::weighted_row; i > 0; i--)
//    {
//        for ( int j = constants::weighted_col; j > 0; j--)
//        {
//            double val = weightedTable->item(i,j)->text().toDouble();
//            allDesignForEdit[currDesignForEdit].weightedTableInfo[i][j] = val;
//            qDebug() << val;
//        }
//    }
//}

void EditDesign::updateDesign()
{
    clearTables();
    currDesignForEdit = editDesignTable->currentItem()->text().toStdString();

    for (int i = 0; i < constants::input_row; ++i)
    {
            for (int j = 0; j < constants::input_col; ++j)
            {
                inputSetupTable->setItem(i,j, new QTableWidgetItem(allDesignForEdit[currDesignForEdit].inputSetupInfo[i][j]));
            }
    }

    for (int i = 0; i < constants::weighted_row; ++i)
    {
            for (int j = 0; j < constants::weighted_col; ++j)
            {
                weightedTable->setItem(i,j, new QTableWidgetItem(allDesignForEdit[currDesignForEdit].weightedTableInfo[i][j]));
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
        editDesignTable->setCurrentItem(newItem);
        currDesignForEdit = editDesignTable->currentItem()->text().toStdString();

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

        //problem??
        //updateDesign();
    }

}

void EditDesign::clearTables()
{
    inputSetupTable->clearContents();
    weightedTable->clearContents();
}
