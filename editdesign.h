#ifndef EDITDESIGN_H
#define EDITDESIGN_H

//! [0]
#include <QDialog>
#include "ui_editdesign.h"
//! [0]
//!
class Design
{

public:
    double inputSetupInfo[13][3];
    double weightedTableInfo[9][3];
    std::string optimal[10];
    //Design(double inputSetupInfo[13][3], double weightedTableinfo[9][3], std::string optimal[10]);
};

//! [1]
class EditDesign : public QDialog, private Ui::EditDesign
{
    Q_OBJECT

public:
    explicit EditDesign(QWidget *parent = nullptr);
    std::map <std::string, Design> allDesign;
    std::string currDesign;
    //EditDesign( QWidget * parent = 0);


private slots:
    void createActions();
    void addDesign();
    void deleteDesign();
    void updateInputSetup();
    void updateWeightedTable();
    void updateDesign();
    void populate();
    void nameChange();
    int getInputRow();
    int getInputCol();
    int getWeightedRow();
    int getWeightedCol();
    void clearTables();

};



//! [1]

#endif // EDITDESIGN_H
