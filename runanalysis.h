#ifndef RUNANALYSIS_H
#define RUNANALYSIS_H

#include <QDialog>
#include "ui_runanalysis.h"
#include "model.h"

class RunAnalysis : public QDialog, /*public Design,*/ private Ui::RunAnalysis
{
    Q_OBJECT
public:
    explicit RunAnalysis(/*std::map <std::string, Model> *allDesign = {},*/ QWidget *parent = nullptr);
    void setDesign(std::map <std::string, Model> &alldesign);

private:
    static std::map <std::string, Model> allD;

private slots:
    void createActions();
};

#endif // RUNANALYSIS_H
