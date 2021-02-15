#ifndef RUNANALYSIS_H
#define RUNANALYSIS_H

#include <QMessageBox>
#include "model.h"

class RunAnalysis : public QMessageBox
{
    Q_OBJECT
public:
    explicit RunAnalysis(QWidget *parent = nullptr);
    //void setDesign(std::map <std::string, Model> &alldesign);

private:
    //static std::map <std::string, Model> allD;

private slots:
    //void createInputTable();
};

#endif // RUNANALYSIS_H
