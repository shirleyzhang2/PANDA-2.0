#ifndef MODEL_H
#define MODEL_H
#include <QObject>
#include <iostream>
#include <map>

class Model
{
public:
    double inputSetupInfo[13][3];
    double weightedTableInfo[9][3];
    std::string optimal[10];
    Model();
};

class Design
{
public:
    static std::map <std::string, Model> allDesign;
    static std::string currDesign;
};

#endif // MODEL_H
