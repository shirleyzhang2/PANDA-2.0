#ifndef RUNANALYSIS_H
#define RUNANALYSIS_H

#include <QDialog>
#include "model.h"

QT_BEGIN_NAMESPACE
class Spreadsheet;
QT_END_NAMESPACE

class ShowScore : public QDialog
{
    Q_OBJECT
public:
    explicit ShowScore(QWidget *parent = nullptr);

private:
    Spreadsheet *resultsSpreadsheet;
    QStringList resultsHeaderH = {"Design", "Weight", "Cp", "Freeboard", "Drag", "Stability", "LeakAngle", "SecondMoment", "WaterplaneCentroid", "PaddlerCentre", "Score"};
    int numresultsHeaderH{ static_cast<int>(std::size(resultsHeaderH)) };

private slots:
};

#endif // RUNANALYSIS_H
