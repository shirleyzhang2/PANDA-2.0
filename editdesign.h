#ifndef EDITDESIGN_H
#define EDITDESIGN_H

//! [0]
#include <QDialog>
#include "ui_editdesign.h"
//! [0]

//! [1]
class EditDesign : public QDialog, private Ui::EditDesign
{
    Q_OBJECT

public:
    explicit EditDesign(QWidget *parent = nullptr);
    //EditDesign( QWidget * parent = 0);

private slots:
    void createActions();
    void addDesign();
    void deleteDesign();

};
//! [1]

#endif // EDITDESIGN_H
