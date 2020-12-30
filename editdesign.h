#ifndef EDITDESIGN_H
#define EDITDESIGN_H

//! [0]
#include <QDialog>
#include "ui_editdesign.h"
#include "model.h"

//! [0]
//!
class Spreadsheet;
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

    Spreadsheet *inputSetupTable;
    Spreadsheet *weightedTable;

    QStringList inputHeaderH = {"Min", "Max", "Value"};
    QStringList inputHeaderV = {"length", "Lp", "Ld", "Lf", "w", "t1", "t2", "d", "b", "s", "f", "n", "density"};
    QStringList weightedHeaderH = {"Target Value", "Standard Dev.", "Weight"};
    QStringList weightedHeaderV = {"Canoe Weight", "Cp", "Freeboard", "Drag", "Stability", "Leak angle", "Second moment", "Waterplane centroid", "Paddler center"};

    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;

private slots:
    void createActions();
    void createContextMenu();
    void addDesign();
    void deleteDesign();
    void updateInput();
    //void updateInputSetup();
    //void updateWeightedTable();
    void updateDesign();
    void populate();
    void nameChange();
    void clearTables();

};

//! [1]

#endif // EDITDESIGN_H
