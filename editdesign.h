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
    static std::map <std::string, Model> allDesignCopy;
    std::string currDesignForEdit {""};
    //bool confirmed {false};

    Spreadsheet *inputSetupTable;
    Spreadsheet *weightedTable;

    QStringList inputHeaderH = {"Min", "Max", "Value"};
    int numInputHeaderH{ static_cast<int>(std::size(inputHeaderH)) };
    QStringList inputHeaderV = {"length", "Lp", "Ld", "Lf", "w", "t1", "t2", "d", "b", "s", "f", "n", "density"};
    int numInputHeaderV{ static_cast<int>(std::size(inputHeaderV)) };
    QStringList weightedHeaderH = {"Target Value", "Standard Dev.", "Weight"};
    int numWeightedHeaderH{ static_cast<int>(std::size(weightedHeaderH)) };
    QStringList weightedHeaderV = {"Canoe Weight", "Cp", "Freeboard", "Drag", "Stability", "Leak angle", "Second moment", "Waterplane centroid", "Paddler center"};
    int numWeightedHeaderV{ static_cast<int>(std::size(weightedHeaderV)) };

    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;

private slots:
    void createActions();
    void createContextMenu();
    void tabSelected();
    void save();
    void addDesign();
    void deleteDesign();
    void updateInput();
    void updateWeighted();
    void updateDesign();
    void addTemplate();
    void populate();
    void nameChange();
    //void clearTables();

};

//! [1]

#endif // EDITDESIGN_H
