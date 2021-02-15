#ifndef LOADCASE_H
#define LOADCASE_H

#include <QObject>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string.h>
#define PI 3.14159265358979323846264338327950

using namespace std;

inline double C4TABLE[3][17][14];		// [Cv][Cp][vtol]
inline double targets[3][9];

class LoadCase/* : public QObject*/
{
   // Q_OBJECT
public:
    explicit LoadCase(/*QObject *parent = nullptr*/);
    int numpaddlers;
    double paddlerweight;
    double totweight;			// canoe + paddlers
    double paddlercm;		// height from the base of knees to the centre of mass of kneeling person
    double totcm;			// canoe + paddlers
    double freeboard;		// d value
    double initstability;
    double tippingangle;
    double friction5;		// friction at 5 knots
    double Cp;
    double waterplane2;	// second moment of waterplane area
    double waterplanecentroid;
    double paddlercentre;		// where the paddlers should be centred

    //double * C4TABLE[3][17][14];
    //double * targets[3][17];


    //Canoe* canoe;		// canoe it belongs to

    double SplineArea(double P0y, double P0z, double P1y, double P1z,
        double P2y, double P2z, double P3y, double P3z,
        double ti, double tf);

    int SplineMoment(double P0y, double P0z, double P1y, double P1z,
        double P2y, double P2z, double P3y, double P3z,
        double ti, double tf, double& centroidy, double& centroidz);

    double SolveCubic(double a, double b, double c, double d);

    int SectionResultant(double P0y, double P0z, double P1y, double P1z,
        double P2y, double P2z, double P3y, double P3z,
        double d, double theta,
        double& magnitude, double& yloc, double& zloc);

    double Spline(double A, double B, double C, double D, double t);

    double Ramp(double x, double t);

    double Moment(double y1, double z1, double y2, double z2, double weight, double theta);

    double SplineLength(double P0y, double P0z, double P1y, double P1z,
        double P2y, double P2z, double P3y, double P3z,
        double d, double &cmz);

    double GetC4(double Cv, double Cp, double vtol);

    int ReadC4Table(ifstream& c4);
    //int ReadC4Table(double c4[3][17][14]);

    int ReadWeightsTable(ifstream& targettable);
    //int ReadWeightsTable(double Weights[3][9]);

    double Score(double* values, double* targetmeans, double* targetstds, double* weights, int n);

    double Gaussian(double mean, double std, double x);

    int CreateInputs(ifstream& inputsetup, ofstream& writeinput);

    int OneInput(double* maxes, double* mins, int* nums, int* ranges, int numinputs, ofstream& writeinput);

signals:

};

//double * C4TABLE[3][17][14];		// [Cv][Cp][vtol]
// Targets and weights chart

//CHANGE THIS TO [3][9]
//double * targets[3][17];			// mean,std,weights
/* Weight
Cp
Freeboard
Drag
Stability
Leak Angle
Second Moment
Waterplane Centroid
Paddler Centre
Cp
Freeboard
Drag
Stability
Leak Angle
Second Moment
Waterplane Centroid
Paddler Centre
*/


// Class definitions

// Canoe class
//double SplineArea(double P0y, double P0z, double P1y, double P1z,
//    double P2y, double P2z, double P3y, double P3z,
//    double ti, double tf);

//int SplineMoment(double P0y, double P0z, double P1y, double P1z,
//    double P2y, double P2z, double P3y, double P3z,
//    double ti, double tf, double& centroidy, double& centroidz);

//double SolveCubic(double a, double b, double c, double d);

//int SectionResultant(double P0y, double P0z, double P1y, double P1z,
//    double P2y, double P2z, double P3y, double P3z,
//    double d, double theta,
//    double& magnitude, double& yloc, double& zloc);

//double Spline(double A, double B, double C, double D, double t);

//double Ramp(double x, double t);

//double Moment(double y1, double z1, double y2, double z2, double weight, double theta);

//double SplineLength(double P0y, double P0z, double P1y, double P1z,
//    double P2y, double P2z, double P3y, double P3z,
//    double d, double &cmz);

//double GetC4(double Cv, double Cp, double vtol);

////int ReadC4Table(ifstream& c4);
//int ReadC4Table(double c4[3][17][14]);

////int ReadWeightsTable(ifstream& targettable);
//int ReadWeightsTable(double Input[3][17]);

//double Score(double* values, double* targetmeans, double* targetstds, double* weights, int n);

//double Gaussian(double mean, double std, double x);

//int CreateInputs(ifstream& inputsetup, ofstream& writeinput);

//int OneInput(double* maxes, double* mins, int* nums, int* ranges, int numinputs, ofstream& writeinput);

#endif // LOADCASE_H
