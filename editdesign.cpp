#include "editdesign.h"
#include "warningmessage.h"
#include "spreadsheet.h"
#include <QWidget>
#include <QtWidgets>
#include <iostream>

std::map <std::string, Model> EditDesign::allDesignForEdit {};
std::map <std::string, Model> EditDesign::allDesignCopy {};

//! [0]
EditDesign::EditDesign(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    // Input Setup
    inputSetupTable = new Spreadsheet(/*numInputHeaderV*/0,numInputHeaderH);
    tabWidget->addTab(inputSetupTable, "Input Setup");
    //inputSetupTable->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
    inputSetupTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //inputSetupTable->horizontalHeader()->setStretchLastSection(true);
    inputSetupTable->setHorizontalHeaderLabels( inputHeaderH );
    //inputSetupTable->setVerticalHeaderLabels( inputHeaderV );
    //inputSetupTable->setCurrentCell(0,0);

    // Weighted Table
    weightedTable = new Spreadsheet(/*numWeightedHeaderV*/0,numWeightedHeaderH);
    tabWidget->addTab(weightedTable, "Weighted Table");
    //weightedTable->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
    weightedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //weightedTable->horizontalHeader()->setStretchLastSection(true);
    weightedTable->setHorizontalHeaderLabels( weightedHeaderH );
    //weightedTable->setVerticalHeaderLabels( weightedHeaderV );
    //weightedTable->setCurrentCell(0, 0);

    //allDesignForEdit = {};

    createActions();
    createContextMenu();
}

void EditDesign::setDesignForEdit(std::map <std::string, Model> &alldesign)
{
    allDesignForEdit = alldesign;
    allDesignCopy = allDesignForEdit;
}

void EditDesign::createActions()
{
    editDesignTable->setSortingEnabled(false);

    const QIcon addIcon = QIcon(":/images/add.png");
    addDesignButton -> setIcon(addIcon);
    connect(addDesignButton, &QAbstractButton::clicked, this, &EditDesign::addDesign);

    const QIcon deleteIcon = QIcon(":/images/delete.png");
    deleteDesignButton -> setIcon(deleteIcon);
    //connect(deleteDesignButton, &QAbstractButton::clicked, this, &EditDesign::deleteDesign);

    connect(editDesignTable, SIGNAL(itemClicked(QTableWidgetItem* item)), this, SLOT(updateDesign()));

    connect(templateButton, &QAbstractButton::clicked, this, &EditDesign::addTemplate);

    //connect(inputSetupTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateInput()));
    connect(inputSetupTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateInput()));
    connect(weightedTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateWeighted()));

    //connect(editDesignTable, SIGNAL(cellChanged(int,int)), this, SLOT(nameChange()));

    // Set ok and cancel buttons
    //connect(designButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(designButtonBox, SIGNAL(accepted()), this, SLOT(save()));
    connect(designButtonBox, SIGNAL(accepted()), this, SLOT(close()));
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

void EditDesign::save()
{
    allDesignForEdit = allDesignCopy;
}

void EditDesign::addDesign()
{
    // TO DO: check if design has been confirmed (table filled out, button pressed)
    int id = 1;
    static std::string name;
    name = "New Design "+std::to_string(id);

    auto iter = allDesignCopy.begin();
    while (iter != allDesignCopy.end())
    {
        id += 1;
        ++iter;
        name = "New Design "+std::to_string(id);
    }
    allDesignCopy[name] = Model();
    currDesignForEdit = name;

    //clearTables();
    populate();
    inputSetupTable->blockSignals(true);
    inputSetupTable->model()->removeRows(0,inputSetupTable->rowCount());
    inputSetupTable->blockSignals(false);
    weightedTable->blockSignals(true);
    weightedTable->model()->removeRows(0,weightedTable->rowCount());
    weightedTable->blockSignals(false);
//    inputSetupTable->clearContents();
//    weightedTable->clearContents();
    //updateDesign();
    //updateInput();
    //updateWeighted();

}

void EditDesign::deleteDesign()
{
    QList<QTableWidgetSelectionRange> sRangeList = editDesignTable->selectedRanges();
    for(const auto &p : qAsConst(sRangeList))
    {
        for (auto i = p.topRow() + p.rowCount() - 1; i > p.topRow() - 1; i--)
        {
            std::string key = editDesignTable->item(i,0)->text().toStdString();
            allDesignCopy.erase(key);
            editDesignTable->removeRow(i);
        }
    }
    //clearTables();
    inputSetupTable->model()->removeRows(0,inputSetupTable->rowCount());
    weightedTable->model()->removeRows(0,weightedTable->rowCount());
}

void EditDesign::updateDesign()
{
    qDebug() << "Update Design clicked";
    if (currDesignForEdit != "")
    {
        //inputSetupTable->model()->removeRows(0,inputSetupTable->rowCount());
        //weightedTable->model()->removeRows(0,weightedTable->rowCount());
        //inputSetupTable->clearContents();
        //weightedTable->clearContents();

        currDesignForEdit = editDesignTable->currentItem()->text().toStdString();
        qDebug() << editDesignTable->currentItem()->text();

        inputSetupTable->blockSignals(true);
        for (int i = 0; i < numInputHeaderH; ++i)
        {
            for (int j = 0; j < numInputHeaderV; ++j)
            {
                //inputSetupTable->insertRow(j);
                double numInput = allDesignCopy[currDesignForEdit].inputSetupInfo[i][j];
                qDebug() << numInput;
                //newInputUpdate = QTableWidgetItem(QString::number(numInput));
                //inputSetupTable->setItem(i,j, newInputUpdate);
                inputSetupTable->setItem(i,j, new QTableWidgetItem(QString::number(numInput)));
            }
        }
        inputSetupTable->blockSignals(false);

        weightedTable->blockSignals(true);
        for (int i = 0; i < numWeightedHeaderH; ++i)
        {
            for (int j = 0; j < numWeightedHeaderV; ++j)
            {
                //weightedTable->insertRow(j);
                double numWeighted = allDesignCopy[currDesignForEdit].weightedTableInfo[i][j];
                qDebug() << numWeighted;
                //QTableWidgetItem *newWeightedUpdate = new QTableWidgetItem(QString::number(numWeighted));
                //weightedTable->setItem(i,j, newWeightedUpdate);
                weightedTable->setItem(i,j, new QTableWidgetItem(QString::number(numWeighted)));
            }
        }
        weightedTable->blockSignals(false);
    }

}

void EditDesign::addTemplate()
{
    inputSetupTable->blockSignals(true);

    inputSetupTable->setRowCount(numInputHeaderV);
    inputSetupTable->setVerticalHeaderLabels( inputHeaderV );
    for (int i = 0; i < numInputHeaderV; ++i)
    {
        for (int j = 0; j < numInputHeaderH; ++j)
        {
            //QTableWidgetItem *zeroInput = new QTableWidgetItem(QString::number(0)); //convert from integer
            //inputSetupTable->setItem(i, j, zeroInput); //set object
            inputSetupTable->setItem(i, j, new QTableWidgetItem(QString::number(0)));
        }
    }
    updateInput();

    inputSetupTable->blockSignals(false);

    weightedTable->blockSignals(true);

    weightedTable->setRowCount(numWeightedHeaderV);
    weightedTable->setVerticalHeaderLabels( weightedHeaderV );
    for (int i = 0; i < numWeightedHeaderV; ++i)
    {
        for (int j = 0; j < numWeightedHeaderH; ++j)
        {
            //QTableWidgetItem *zeroWeighted = new QTableWidgetItem(QString::number(0)); //convert from integer
            //weightedTable->setItem(i, j, zeroWeighted); //set object
            weightedTable->setItem(i, j, new QTableWidgetItem(QString::number(0)));
        }
    }
    updateWeighted();

    weightedTable->blockSignals(false);
}

void EditDesign::updateInput()
{
    qDebug() << "update input";
    if (currDesignForEdit != "")
    {
        currDesignForEdit = editDesignTable->currentItem()->text().toStdString();
        qDebug() << editDesignTable->currentItem()->text();

        for( int i = 0; i < numInputHeaderV; ++i)
        {
            for ( int j = 0; j < numInputHeaderH; ++j)
            {
                //QTableWidgetItem *input =inputSetupTable->item(i,j);
                //allDesignCopy[currDesignForEdit].inputSetupInfo[i][j] = input->text().toDouble();
                allDesignCopy[currDesignForEdit].inputSetupInfo[i][j] = inputSetupTable->item(i,j)->text().toDouble();
            }
        }
    }
}

void EditDesign::updateWeighted()
{
    if (currDesignForEdit != "")
    {
        currDesignForEdit = editDesignTable->currentItem()->text().toStdString();
        qDebug() << editDesignTable->currentItem()->text();

        for( int i = 0; i < numWeightedHeaderV; ++i)
        {
            for ( int j = 0; j < numWeightedHeaderH; ++j)
            {
                //QTableWidgetItem *weighted =weightedTable->item(i,j);
                //allDesignCopy[currDesignForEdit].weightedTableInfo[i][j] = weighted->text().toDouble();
                allDesignCopy[currDesignForEdit].weightedTableInfo[i][j] = weightedTable->item(i,j)->text().toDouble();

            }
        }
    }
}


//    QTableWidgetItem *design =editDesignTable->item(0,0);
//    if (!design || design->text().isEmpty())
//    {
//        WarningMessage warning("Please add a new design first!");
//        warning.exec();
//        return;
//    }
//    else
//    {
//        currDesignForEdit = editDesignTable->currentItem()->text().toStdString();

//        // Input setup
//        for( int i = 0; i < constants::input_row; ++i)
//        {
//            for ( int j = 0; j < constants::input_col; ++j)
//            {
//                QTableWidgetItem *input =inputSetupTable->item(i,j);
//                if (!input || input->text().isEmpty())
//                {
//                    WarningMessage warning("Please complete all required fields in input setup and weighted table before updating design");
//                    warning.exec();
//                    return;
//                }
//                else
//                {
//                    allDesignCopy[currDesignForEdit].inputSetupInfo[i][j] = input->text().toDouble();
//                    confirmed = true;
//                    //qDebug() << allDesignForEdit[currDesignForEdit].inputSetupInfo[i][j];
//                }
//            }
//        }

//        // Weighted table
//        for( int i = 0; i < constants::weighted_row; ++i)
//        {
//            for ( int j = 0; j < constants::weighted_col; ++j)
//            {
//                QTableWidgetItem *weighted =weightedTable->item(i,j);
//                if (!weighted || weighted->text().isEmpty())
//                {
//                    WarningMessage warning("Please complete all required fields in input setup and weighted table before updating design");
//                    warning.exec();
//                    return;
//                }
//                else
//                {
//                    allDesignCopy[currDesignForEdit].weightedTableInfo[i][j] = weighted->text().toDouble();
//                    qDebug() << allDesignCopy[currDesignForEdit].weightedTableInfo[i][j];
//                }
//            }
//        }

//    }
//}


//void EditDesign::updateWeighted()
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


void EditDesign::populate()
{
    editDesignTable->model()->removeRows(0,editDesignTable->rowCount());
    for (const auto& [key, value] : allDesignCopy)
    {
        int row = editDesignTable->rowCount();
        editDesignTable->insertRow(row);
        //QTableWidgetItem *newDesign = new QTableWidgetItem(QString::fromStdString(key));
        //editDesignTable->setItem(row, 0, newDesign);
        //editDesignTable->setCurrentItem(newDesign);
        editDesignTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(key)));
        QModelIndex newIndex  = editDesignTable->model()->index(row,0);
        editDesignTable->selectionModel()->select(newIndex, QItemSelectionModel::Select);
        editDesignTable->setCurrentIndex(newIndex);
        //editDesignTable->setCurrentItem(new QTableWidgetItem(QString::fromStdString(key)));
        currDesignForEdit = editDesignTable->currentItem()->text().toStdString();

    }
}

void EditDesign::nameChange()
{
    //qDebug() << QString::fromStdString(currDesign);
    int row = editDesignTable->currentRow(); //returns -1 when populating empty table

    if (row != -1/* && currDesignForEdit != ""*/)
    {
        std::string newName = editDesignTable->currentItem()->text().toStdString();
        if (newName == "")
        {
            WarningMessage warning("Name cannot be empty!");
            warning.exec();
            return;
        }

        // assign original parameters to new name
        auto nodeHandler = allDesignCopy.extract(currDesignForEdit);
        nodeHandler.key() = newName;
        allDesignCopy.insert(std::move(nodeHandler));

        //problem??
        updateDesign();
    }

}

//void EditDesign::clearTables()
//{
//    //inputSetupTable->clearContents();
//    qDebug() << "clear Tables";
//    qDebug() << QString::fromStdString(currDesignForEdit);
//    for (int i = 0; i < numInputHeaderV; ++i)
//    {
//            for (int j = 0; j < numInputHeaderH; ++j)
//            {
//                QTableWidgetItem *zeroInput = new QTableWidgetItem(QString::number(0)); //convert from integer
//                inputSetupTable->setItem(i, j, zeroInput); //set object
//                allDesignCopy[currDesignForEdit].inputSetupInfo[i][j] = 0;
//                //qDebug() << allDesignCopy[currDesignForEdit].inputSetupInfo[i][j];
//            }
//    }

//    //weightedTable->clearContents();
//    for (int i = 0; i < numWeightedHeaderV; ++i)
//    {
//            for (int j = 0; j < numWeightedHeaderH; ++j)
//            {
//                QTableWidgetItem *zeroWeighted = new QTableWidgetItem(QString::number(0)); //convert from integer
//                weightedTable->setItem(i, j, zeroWeighted); //set object
//                allDesignCopy[currDesignForEdit].weightedTableInfo[i][j] = 0;
//                //qDebug() << allDesignCopy[currDesignForEdit].weightedTableInfo[i][j];
//            }
//    }
//}
