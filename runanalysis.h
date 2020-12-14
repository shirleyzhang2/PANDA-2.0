#ifndef RUNANALYSIS_H
#define RUNANALYSIS_H

#include <QDialog>
#include "ui_runanalysis.h"

class RunAnalysis : public QDialog, private Ui::RunAnalysis
{
    Q_OBJECT
public:
    explicit RunAnalysis(QWidget *parent = nullptr);

private slots:
    void createActions();
};

#endif // RUNANALYSIS_H
