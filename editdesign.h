#ifndef EDITDESIGN_H
#define EDITDESIGN_H

//! [0]
#include <QDialog>
#include "ui_editdesign.h"
#include "model.h"

//! [0]
//!

//! [1]
class EditDesign : public QDialog, /*public Design,*/ private Ui::EditDesign
{
    Q_OBJECT

public:

    explicit EditDesign(QWidget *parent = nullptr);
    void setDesignForEdit(std::map <std::string, Model> &alldesign);
    std::map <std::string, Model> getDesign() { return allDesignForEdit; }

private:
    static std::map <std::string, Model> allDesignForEdit;
    std::string currDesignForEdit;

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
