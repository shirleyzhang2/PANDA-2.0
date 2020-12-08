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
}

void EditDesign::addDesign()
{
    editDesignTable->insertRow( editDesignTable->rowCount() );
}

void EditDesign::deleteDesign()
{
    QModelIndexList indexes =  editDesignTable->selectionModel()->selectedRows();
    int countRow = indexes.count();
    for( int i = countRow; i > 0; i--)
           editDesignTable->removeRow( indexes.at(i-1).row() );
    //editDesignTable->setCurrentCell(0, 0);
}
