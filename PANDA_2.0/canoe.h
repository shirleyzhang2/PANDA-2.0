#ifndef CANOE_H
#define CANOE_H
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string.h>
using namespace std;
#define PI 3.14159265358979323846264338327950
#include <loadcase.h>
#include <fstream>
#include <QObject>
// Global variables
// KAPER chart

//double C4TABLE[3][17][14];		// [Cv][Cp][vtol]
//double targets[3][17];

inline int counter;
inline int bowpower;
inline int sternpower;

class Canoe/* : public QObject*/
{
    //Q_OBJECT
public:
    //int bowpower;
    //int sternpower;
    explicit Canoe(/*QObject *parent = nullptr*/);
    int numstations;
    double increment;
    double* wlvalues;
    double* klvalues;
    double wltol;		// waterline iteration tolerance
    double tippingtol;	// tipping angle tolerance
    double bowtol;		// tolerance for bow angle calculation
    int wlmaxit;		// max iterations
    int tippingmaxit;

    // input quantities
    double length; // L = Total length of canoe
    double lpaddler; // Lp = Length of paddlers' "box"
    double ldeepest; // Ld = Distance from bow to deepest point of canoe
    double lfirst; // Lf = Distance from bow to first paddler
    double width; // W = Nominal beam of canoe (the actual beam is in general smaller than this value due to the smoothing of the ramp function
    double smooth1; // t1 = Waterline smoothing parameter (bow side)
    double smooth2; // t2 = Waterline smoothing parameter (stern side)
    double depth; // d = Depth of canoe below the flange
    //double hflange; // h = Height of the flange
    double brocker; // b = Bow rocker
    double srocker; // s = Stern rocker
    double flare; // f(x) = Flare angle of the flange (in general a function of distance along the canoe)
    double shapeparam; // n = Cross-section
    double density;	// area density

    // derived quantities
    LoadCase loadcase;	// light and heavy loading ~ reedited to only allow for 4 paddler case
    //LoadCase loadcase = LoadCase();
    std::ofstream output;	// output file
    double surfacearea;	// surface area
    double weight;			// self weight
    double CMx;				// centre of mass x
    double CMz;				// centre of mass z
    double actualbeam;
    // modified from 17 to 9 (only 1 load case)
    double outputs[9] {};

    int ControlPoints(int station, double& P0y, double& P0z, double& P1y, double& P1z, double& P2y, double& P2z,
        double& P3y, double& P3z);

    int ControlPoints(double x, double& P0y, double& P0z, double& P1y, double& P1z, double& P2y, double& P2z,
        double& P3y, double& P3z);

    double Keelline(double x);

    double Waterline(double x);

    int InitializeCanoe(double L, double Lp, double Ld, double Lf, double W, double t1, double t2, double d, /*double h, */double b,
        double s, double f, double n, double density);		// density in kg/m^2

    int Destruct();

    Canoe(double L, double Lp, double Ld, double Lf, double W, double t1, double t2, double d, /*double h, */double b,
        double s, double f, double n);

    //Canoe();

    int Analyze(double d, double theta, double trim, double& volume, double& cenx, double& ceny, double& cenz);

    double FindWLine(double v, double theta, double trim);

    int CrossCurve(double weight, double cmheight, double& tangent, double& tipping);

    int DisplacementCurve();

    double SurfaceArea(double d, double& cx, double& cz);

    double Kaper(double BWL, double EWL, double WS, double Cp, double Cv, double LCB, double disp, double le, double v);

    double GetEWL(double d, double theta, double trim);

    double GetLWL(double d);

    double GetBWL(double d);

    double Getle(double d);

    double GetCp(double d, double LWL);

    double GetCv(double disp, double LWL);

    double GetFriction(double d, double v);

    int OutputMesh(std::ofstream& meshout);

    int Waterplane(double d, double& wpcen, double& wparea, double& wpmom2);

    int AnalyzeAll();

    int OutputAll(std::ofstream& canoeout);

    int UIBulk();

signals:

};

#endif // CANOE_H
