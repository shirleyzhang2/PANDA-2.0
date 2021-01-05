#ifndef MODEL_H
#define MODEL_H
#include <QObject>
#include <iostream>
#include <map>
#include "constants.h"

class Model
{
public:
    double inputSetupInfo[constants::input_row][constants::input_col] = {0};
    double weightedTableInfo[constants::weighted_row][constants::weighted_col] = {0};
    std::string optimal[constants::optimal_col] = {};
    Model();
};

#endif // MODEL_H
