#include "canoe.h"

using namespace  std;


Canoe::Canoe() /*(QObject *parent) : QObject(parent)*/
{

}

int Canoe::ControlPoints(int station, double& P0y, double& P0z, double& P1y, double& P1z,
    double& P2y, double& P2z, double& P3y, double& P3z) {
    //	double x = station*increment;
    double w = wlvalues[station];
    double d = klvalues[station];
    double flareangle;
    double fmax = asin(w / (2. * d));
    if (fmax < flare)
        flareangle = fmax;
    else
        flareangle = flare;

    double u1max = d / cos(flareangle);
    double u2max = w - d * tan(flareangle);
    double u1 = u1max * (0.25 + 0.75 * shapeparam);
    double u2 = u2max * (0.5 + 0.5 * shapeparam);

    P0y = w;
    P0z = 0;
    P1y = P0y - u1 * sin(flareangle);
    P1z = P0z - u1 * cos(flareangle);
    P3y = 0;
    P3z = -d;
    P2y = P3y + u2;
    P2z = P3z;

    return 0;
}

// OVERLOADED FUNCTION
// This version works for any x value, not just at predefined stations. This one is more computationally intensive,
// so it should be used only sparingly
int Canoe::ControlPoints(double x, double& P0y, double& P0z, double& P1y, double& P1z,
    double& P2y, double& P2z, double& P3y, double& P3z) {
    double w = Waterline(x);
    double d = Keelline(x);
    double flareangle;
    double fmax = acos(w / (2. * d));
    if (fmax < flare)
        flareangle = fmax;
    else
        flareangle = flare;

    double u1max = d / cos(flareangle);
    double u2max = w - d * tan(flareangle);
    double u1 = u1max * (0.25 + 0.75 * shapeparam);
    double u2 = u2max * (0.5 + 0.5 * shapeparam);

    P0y = w;
    P0z = 0;
    P1y = P0y - u1 * sin(flareangle);
    P1z = P0z - u1 * cos(flareangle);
    P3y = 0;
    P3z = -d;
    P2y = P3y + u2;
    P2z = P3z;

    return 0;

}

int Canoe::Destruct() {
    delete[] wlvalues;
    delete[] klvalues;
    return 0;
}

Canoe::Canoe(double L, double Lp, double Ld, double Lf, double W, double t1, double t2, double d, double h, double b,
    double s, double f, double n) {
    length = L;
    lpaddler = Lp;
    ldeepest = Ld;
    lfirst = Lf;
    width = W;
    smooth1 = t1;
    smooth2 = t2;
    depth = d;
    hflange = h;
    brocker = b;
    srocker = s;
    flare = f;
    shapeparam = n;
}

int Canoe::Analyze(double d, double theta, double trim, double& volume, double& cenx, double& ceny, double& cenz) {

    double* areas = new double[numstations];
    double* centroidx = new double[numstations];
    double* centroidy = new double[numstations];
    double* centroidz = new double[numstations];

    double x = 0;
    int i;

    double P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z;
    int flag = 0;

    for (i = 0; i < numstations; i++) {
        ControlPoints(i, P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z);
        // freeboard at distance x is given by d - trim/length*x
        if (loadcase[0].SectionResultant(P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z, (trim == 0 ? d : d - trim / length * x), theta, areas[i], centroidy[i], centroidz[i]) == -1) {
            //cout << "LEAK\n";			// let it pass for now
            flag = 1;
            //return 1;
        }
        centroidx[i] = x;

        x += increment;
    }

    volume = 0;
    for (i = 1; i < numstations; i++) {							// numerically integrate using parallelograms
        volume += (areas[i] + areas[i - 1]) / 2;
    }
    volume *= increment;

    cenx = 0;
    ceny = 0;
    cenz = 0;

    for (i = 0; i < numstations; i++) {
        cenx += centroidx[i] * areas[i] * increment;			// this is not 100% correct... but it's not too far wrong
        ceny += centroidy[i] * areas[i] * increment;
        cenz += centroidz[i] * areas[i] * increment;
    }
    cenx /= volume;
    ceny /= volume;
    cenz /= volume;

    delete[] areas;
    delete[] centroidx;
    delete[] centroidy;
    delete[] centroidz;

    if (flag == 1)
        return 1;

    return 0;
}

double Canoe::Waterline(double x) {
    double m1 = width / (2. * lfirst);
    double m2 = width / (2. * (length - lpaddler - lfirst));
    double rbow = width / 2. - m1 * loadcase[0].Ramp(width / (2. * m1), smooth1) - m2 * loadcase[0].Ramp(-length + width / (2. * m2), smooth2);
    double rstern = width / 2. - m1 * loadcase[0].Ramp(width / (2. * m1) - length, smooth1) - m2 * loadcase[0].Ramp(width / (2. * m2), smooth2);

    return (width / 2. - m1 * loadcase[0].Ramp(width / (2. * m1) - x, smooth1) - m2 * loadcase[0].Ramp(x - length + width / (2. * m2), smooth2) - (rstern - rbow) * x / length - rbow);
}

double Canoe::Keelline(double x) {
    if (x < ldeepest) {
        return (depth - brocker * fabs(pow((1. - x / ldeepest), bowpower)));
    }
    else
        return (depth - srocker * fabs((pow((ldeepest - x) / (length - ldeepest), sternpower))));
    return 0;
}

double Canoe::FindWLine(double vtarget, double theta, double trim) {
    double v, cx, cy, cz;
    int MAXITERATIONS = wlmaxit;
    double tolerance = wltol;

    double testd = depth / 2;
    double width = depth / 2;

    int flag;
    // Check if it's possible to attain equilibrium (only valid for the zero trim case)
    if (trim == 0) {
        double width = GetBWL(0) / 2.;
        double dcheck = width * tan(theta);
        Analyze(dcheck, theta, trim, v, cx, cy, cz);
        if (v < vtarget)
            return -1;
    }

    // binary-type algorithm
    int i;
    for (i = 0; i < MAXITERATIONS; i++) {
        flag = Analyze(testd, theta, trim, v, cx, cy, cz);
        if (fabs(v - vtarget) < tolerance)
            return testd;
        //		if (v < vtarget && flag == 1)
        //			return -1;								// equilibrium not possible -- leak
        if (v > vtarget || flag == 1)
            testd += width / 2.;
        else
            testd -= width / 2.;
        width /= 2.;
    }

    //	cout << "WARNING: LOSS OF PRECISION\n";
    return -1;

}

int Canoe::InitializeCanoe(double L, double Lp, double Ld, double Lf, double W, double t1, double t2, double d, double h, double b,
    double s, double f, double n, double density) {
    numstations = 101;
    increment = L / (numstations - 1);

    // Default tolerance values
    wltol = 1e-4;
    tippingtol = 1e-2;
    bowtol = 1e-4;
    wlmaxit = 50;
    tippingmaxit = 50;

    wlvalues = new double[numstations];
    klvalues = new double[numstations];

    length = L;
    lpaddler = Lp;
    ldeepest = Ld;
    lfirst = Lf;
    width = W;
    smooth1 = t1;
    smooth2 = t2;
    depth = d;
    hflange = h;
    brocker = b;
    srocker = s;
    flare = f;
    shapeparam = n;
    this->density = density;

    int i;
    for (i = 0; i < numstations; i++) {
        wlvalues[i] = Waterline(i * increment);
        klvalues[i] = Keelline(i * increment);
    }

//    // 3 men load case - turning is important
//    loadcase[0].numpaddlers = 3;
//    loadcase[0].paddlerweight = 85;			// about 185 pounds
//    loadcase[0].paddlercm = .40;		// 40 cm (rough estimate)

    // 2+2 load case - speed is important
    loadcase[0].numpaddlers = 4;
    loadcase[0].paddlerweight = 72.5;			// about 135 pounds for women
    loadcase[0].paddlercm = .35;			// about 30 cm for women

    return 0;
}

// Run this only after AnalyzeAll has been run - rewrite to SQL table
//int Canoe::OutputAll(ofstream& canoeout) {
//    canoeout << "Canoe Data\n\n";

//    canoeout << "Input Values\n\n";
//    canoeout << "Length\tNominal Beam\tDepth\tNominal Flare Angle\tLength to Deepest Point\tLength to First Paddler\tLength of Paddlers' Box\tShape Parameter\tBow Rocker\tStern Rocker\tArea Density\n"
//        << length << '\t' << width << '\t' << depth << '\t' << flare << '\t' << ldeepest << '\t' << lfirst << '\t' << lpaddler << '\t' << shapeparam << '\t' << brocker << '\t' << srocker << '\t' << density << "\n\n";

//    canoeout << "Output Values\n\n";
//    canoeout << "Surface Area\tWeight\tLCM\tActual Beam\n"
//        << surfacearea << '\t' << weight << '\t' << CMx << '\t' << actualbeam << "\n\n";

//    canoeout << "3 Men Load Case\n";
//    int i = 0;
////    canoeout << "Cp\tFreeboard\tDrag at 5 knots\tInitial Stability\tLeak Angle\tSecond Moment of Waterplane Area\tWaterplane Centroid\tPaddler Centre\n"
////        << loadcase[i].Cp << '\t' << loadcase[i].freeboard << '\t' << loadcase[i].friction5 << '\t' << loadcase[i].initstability << '\t' << loadcase[i].tippingangle << '\t' << loadcase[i].waterplane2 << '\t' << loadcase[i].waterplanecentroid << '\t' << loadcase[i].paddlercentre << "\n\n";

////    i = 1;
//    canoeout << "4 Mixed Load Case\n";
//    canoeout << "Cp\tFreeboard\tDrag at 5 knots\tInitial Stability\tLeak Angle\tSecond Moment of Waterplane Area\tWaterplane Centroid\n"
//        << loadcase[i].Cp << '\t' << loadcase[i].freeboard << '\t' << loadcase[i].friction5 << '\t' << loadcase[i].initstability << '\t' << loadcase[i].tippingangle << '\t' << loadcase[i].waterplane2 << '\t' << loadcase[i].waterplanecentroid << '\t' << loadcase[i].paddlercentre << "\n\n";

//    return 0;
//}

// Get Cv
double Canoe::GetCv(double disp, double LWL) {
    return (disp / (LWL * LWL * LWL));
}


// Get Cp
double Canoe::GetCp(double d, double LWL) {
    double* areas = new double[numstations];

    //	double x=0;
    int i;
    double dummy1, dummy2;
    double areamax = 0;

    double P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z;

    for (i = 0; i < numstations; i++) {
        ControlPoints(i, P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z);
        // freeboard at distance x is given by d - trim/length*x
        if (loadcase[0].SectionResultant(P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z, d, 0, areas[i], dummy1, dummy2) == -1) {
            cout << "LEAK\n";
            delete[] areas;
            return 1;
        }
        if (areas[i] > areamax) {
            areamax = areas[i];
        }
        //		x += increment;
    }

    double v, cx, cy, cz;
    Analyze(d, 0, 0, v, cx, cy, cz);

    delete[] areas;

    return (v / LWL / areamax);
}

// NOTE - cmheight is the centre of mass height taken ABOVE THE TOP OF THE GUNWALE
// The tipping angle calculation assumes the leak happens at the widest point (which is always true for zero trim)
int Canoe::CrossCurve(double weight, double cmheight, double& tangent, double& tipping) {
    double theta = 0;
    double tinc = 0.01;
    double v, cx, cy, cz;

    // roughly get tangent
    double theta1 = theta;
    double theta2 = theta + tinc;
    double d1 = FindWLine(weight, theta1, 0);
    Analyze(d1, theta1, 0, v, cx, cy, cz);
    double moment1 = loadcase[0].Moment(cy, cz, 0, cmheight, weight, theta);
    double d2 = FindWLine(weight, theta2, 0);
    Analyze(d2, theta2, 0, v, cx, cy, cz);
    double moment2 = loadcase[0].Moment(cy, cz, 0, cmheight, weight, theta);
    tangent = (moment2 - moment1) / (theta2 - theta1);

    // find critical tipping angle
    double tolerance = tippingtol;
    double width = GetBWL(0) / 2.;
    double dtest = d1;					// start off with d for 0 heel
    double dmax = depth;					// initial maximum value of d is depth of canoe
    double dmin = 0.;						// initial minimum value of d is 0


    theta = atan2(dtest, width);
    Analyze(dtest, theta, 0, v, cx, cy, cz);
    int MAXITERATIONS = tippingmaxit;
    int count = 0;
    int flag = 0;
    while (fabs(weight - v) > tolerance) {
        if (count >= MAXITERATIONS) {
            flag = 1;
            break;
        }
        count++;
        if (weight < v) {
            dmin = dtest;
            dtest = (dtest + dmax) / 2.;
        }
        else {
            dmax = dtest;
            dtest = (dtest + dmin) / 2.;
        }
        theta = atan2(dtest, width);
        Analyze(dtest, theta, 0, v, cx, cy, cz);
    }

    tipping = theta;

    return flag;
}

int Canoe::DisplacementCurve() {
    double d = depth;
    double dinc = 0.01;
    double v, cx, cy, cz;
    while (d >= 0) {
        Analyze(d, 0, 0, v, cx, cy, cz);

        output << d << '\t' << v << endl;

        d -= dinc;
    }

    return 0;
}


double Canoe::SurfaceArea(double d, double& cx, double& cz) {
    double area = 0;
    double areamoment = 0;
    double areazmoment = 0;
    int i;
    double* lengths = new double[numstations];
    double* cenzs = new double[numstations];
    double P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z;
    //	double x=0;
    double cmz;
    for (i = 0; i < numstations; i++) {
        ControlPoints(i, P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z);
        lengths[i] = loadcase[0].SplineLength(P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z, d, cmz);
        //x += increment;
        cenzs[i] = cmz;			//
    }
    for (i = 1; i < numstations; i++) {							// numerically integrate using parallelograms
        area += (lengths[i] + lengths[i - 1]) / 2;
        areamoment += (lengths[i] * i * increment);
        areazmoment += cenzs[i] * lengths[i];
    }
    cx = areamoment / area;
    cz = areazmoment / area;

    area *= increment;
    area *= 2.;

    delete[] lengths;
    delete[] cenzs;

    return area;
}


// John Winters' KAPER formula - disp is in long tonnes, lengths in feet, speed in knots, LCB is a fraction of length
double Canoe::Kaper(double BWL, double EWL, double WS, double Cp, double Cv, double LCB, double disp, double le, double v) {
    double vtol = v / sqrt(EWL);

    double c1 = 0.002 * sqrt(BWL / EWL) * pow((4 * vtol), 4);
    double c2 = 0.005 * (sin(le * 360 / 2. / PI)) * (4 * vtol) * (4 * vtol);
    double c3 = (vtol < 1.5 ? 1 : 0.7) * 0.8 * cos(3.65 * vtol + 0.07) + (vtol < 1.5 ? 1 : 0.96) * 1.2;
    double c4 = loadcase[0].GetC4(Cv, Cp, vtol);
    double c5 = pow((0.5 / LCB), 0.35);

    double Rr = (4 * c5 * disp * pow(vtol, 4) + c1 + c2) * (c3 * c4 * c5);
    if (v >= 1.6 && v < 3)
        Rr *= 0.75;
    if (v >= 1.4 && v < 1.6)
        Rr *= 0.85;

    double reynold = v * 1.6889 * EWL / 1.2791 * 100000;
    double Cf = 0.075 / pow((log10(reynold) - 2), 2);
    double Rf = 0.99525 * Cf * WS * (v * 1.6889) * (v * 1.6889);		// speed in feet/sec

    return (Rf + Rr);
}

// Get "Effective waterline length" (metres)
double Canoe::GetEWL(double d, double theta, double trim) {
    double* areas = new double[numstations];

    double x = 0;
    int i;
    double dummy1, dummy2;
    double areamax = 0;
    int xmax = 0;

    double P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z;

    for (i = 0; i < numstations; i++) {
        ControlPoints(i, P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z);
        // freeboard at distance x is given by d - trim/length*x
        if (loadcase[0].SectionResultant(P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z, d - trim / length * x, theta, areas[i], dummy1, dummy2) == -1) {
            cout << "LEAK\n";
            delete[] areas;
            return 1;
        }
        if (areas[i] > areamax) {
            areamax = areas[i];
            xmax = i;
        }
        x += increment;
    }


    int xstart, xend;
    bool flag;

    for (xstart = 0; xstart < xmax; xstart++) {
        flag = false;
        for (i = xstart; i < xmax; i++) {
            if ((i - xstart) / (xmax - xstart) * areamax > areas[i]) {			// Find (approx) tangent to the curve
                flag = true;
                break;
            }
        }
        if (!flag)
            break;	// We now have xstart for EWL
    }

    for (xend = numstations - 1; xend > xmax; xend--) {
        flag = false;
        for (i = xend; i > xmax; i--) {
            if ((xend - i) / (xend - xmax) * areamax > areas[i]) {			// Find (approx) tangent to the curve
                flag = true;
                break;
            }
        }
        if (!flag)
            break;	// We now have end for EWL
    }

    double EWL = (xend - xstart) * increment;
    //	cout << "EWL = " << EWL << endl;
    delete[] areas;

    return EWL;
}

// Get waterline length (metres)
double Canoe::GetLWL(double d) {
    int i;
    bool flag = false;
    int starti, endi;

    for (i = 0; i < numstations; i++) {
        if (klvalues[i] > d) {
            flag = true;
            starti = i;
            break;
        }
    }

    if (!flag)
        return length;

    flag = false;
    for (i = numstations - 1; i >= 0; i--) {
        if (klvalues[i] > d) {
            flag = true;
            endi = i;
            break;
        }
    }

    return ((endi - starti) * increment);
}

// Get waterline beam (metres)
double Canoe::GetBWL(double d) {
    double BWL = 0;
    double P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z;

    double width;

    int i;
    for (i = 0; i < numstations; i++) {
        ControlPoints(i, P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z);
        double A3 = P0z - 3 * P1z + 3 * P2z - P3z;
        double B3 = 3 * P1z - 6 * P2z + 3 * P3z;
        double C3 = 3 * P2z - 3 * P3z;
        double D3 = P3z;
        double tf = loadcase[0].SolveCubic(A3, B3, C3, D3 + d - P0z);

        double A1 = P0y - 3 * P1y + 3 * P2y - P3y;
        double B1 = 3 * P1y - 6 * P2y + 3 * P3y;
        double C1 = 3 * P2y - 3 * P3y;
        double D1 = P3y;

        width = loadcase[0].Spline(A1, B1, C1, D1, tf);
        if (width > BWL) {
            BWL = width;
        }
    }
    return BWL * 2;
}

// Get waterline bow half angle (degrees)
double Canoe::Getle(double d) {
    double xentrance;
    double TOLERANCE = bowtol;
    double xinterval = ldeepest;
    double xpos = ldeepest / 2.;
    if (depth - brocker > d) {
        xentrance = 0;
    }
    else {
        bool flag = false;
        while (!flag) {
            if (fabs(Keelline(xpos) - d) < TOLERANCE) {
                flag = true;
            }
            else {
                if (Keelline(xpos) > d) {
                    xpos = xpos - xinterval / 2.;
                }
                else {
                    xpos = xpos + xinterval / 2.;
                }
                xinterval /= 2.;
            }
        }
        xentrance = xpos;
    }

    double P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z;

    // have to use the expensive ControlPoints function
    ControlPoints(xentrance, P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z);
    double A3 = P0z - 3 * P1z + 3 * P2z - P3z;
    double B3 = 3 * P1z - 6 * P2z + 3 * P3z;
    double C3 = 3 * P2z - 3 * P3z;
    double D3 = P3z;
    double tf = loadcase[0].SolveCubic(A3, B3, C3, D3 + d - P0z);
    double A1 = P0y - 3 * P1y + 3 * P2y - P3y;
    double B1 = 3 * P1y - 6 * P2y + 3 * P3y;
    double C1 = 3 * P2y - 3 * P3y;
    double D1 = P3y;
    double w1 = loadcase[0].Spline(A1, B1, C1, D1, tf);

    double dx = length / 1000.;													// a sufficiently small dx
    ControlPoints(xentrance + dx, P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z);
    A3 = P0z - 3 * P1z + 3 * P2z - P3z;
    B3 = 3 * P1z - 6 * P2z + 3 * P3z;
    C3 = 3 * P2z - 3 * P3z;
    D3 = P3z;
    tf = loadcase[0].SolveCubic(A3, B3, C3, D3 + d - P0z);
    A1 = P0y - 3 * P1y + 3 * P2y - P3y;
    B1 = 3 * P1y - 6 * P2y + 3 * P3y;
    C1 = 3 * P2y - 3 * P3y;
    D1 = P3y;
    double w2 = loadcase[0].Spline(A1, B1, C1, D1, tf);

    double angle = atan2(w2 - w1, dx);

    angle = angle / PI * 180;

    return angle;
}


int Canoe::Waterplane(double d, double& wpcen, double& wparea, double& wpmom2) {
    double* depths = new double[numstations];

    double keel;
    double submerged;

    int i;
    for (i = 0; i < numstations; i++) {
        keel = klvalues[i];
        submerged = keel - d;
        if (submerged < 0) submerged = 0;
        depths[i] = submerged;
    }

    double area = 0;
    double firstmoment = 0;
    double segcentroid;
    double segarea;
    double centroid;
    for (i = 1; i < numstations; i++) {
        segarea = (depths[i] + depths[i - 1]) / 2.;
        area += segarea;				// parallelogram
        segcentroid = increment * (i - (2 * depths[i - 1] + depths[i]) / 3. / (depths[i - 1] + depths[i]));		// centroid of parallelogram
        firstmoment += segarea * segcentroid;
    }
    area *= increment;
    centroid = firstmoment / area * increment;

    double secondmoment = 0;
    for (i = 1; i < numstations; i++) {			// find second moments
        segarea = (depths[i] + depths[i - 1]) / 2.;
        segcentroid = increment * (i - (2 * depths[i - 1] + depths[i]) / 3. / (depths[i - 1] + depths[i]));
        secondmoment += (segcentroid - centroid) * (segcentroid - centroid) * segarea;			// parallel axis theorem
    }
    secondmoment *= increment;

    wpcen = centroid;
    wparea = area;
    wpmom2 = secondmoment;

    delete[] depths;

    return 0;
}


int Canoe::AnalyzeAll() {

    double cx, cz;
    surfacearea = SurfaceArea(0, cx, cz);
    CMx = cx;
    CMz = cz;
    weight = surfacearea * density;
    actualbeam = GetBWL(0);

    int i;
    bool error = false;
    for (i = 0; i < 1; i++) {
        loadcase[i].totweight = (loadcase[i].numpaddlers * loadcase[i].paddlerweight + weight) / 1000.;
        loadcase[i].totcm = (loadcase[i].numpaddlers * loadcase[i].paddlerweight * (loadcase[i].paddlercm - depth) + weight * CMz) / loadcase[i].totweight;

        double d = FindWLine(loadcase[i].totweight * 1.0, 0, 0);		// density of water = 1.0
        loadcase[i].freeboard = d;
        if (d < 0) {
            error = true;
            break;
        }

        double LCB, ceny, cenz, volume;
        Analyze(d, 0, 0, volume, LCB, ceny, cenz);
        // report paddler centre as percentage of paddler box length away from the "ideal" paddler centre
        double paddlercentreabs = (LCB * volume - CMx * weight / 1000.) / (loadcase[i].numpaddlers * loadcase[i].paddlerweight / 1000.);
        loadcase[i].paddlercentre = (paddlercentreabs - (lfirst + 0.5 * lpaddler)) / lpaddler;				// I THINK THIS EQUATION IS FLAWED

        double LWL = GetLWL(d);
        loadcase[i].Cp = GetCp(d, LWL);
        double friction = GetFriction(d, 2.57222222);	// 5 knots = about 10 km/h
        if (friction < 0) {
            error = true;
            break;
        }
        loadcase[i].friction5 = friction;

        double tangent, tipping;
        if (CrossCurve(loadcase[i].totweight * 1.0, loadcase[i].totcm, tangent, tipping) == 1) {
            error = true;
            break;
        }
        loadcase[i].initstability = tangent;
        loadcase[i].tippingangle = tipping;

        double wpcen, wparea, wpmom2;
        Waterplane(d, wpcen, wparea, wpmom2);
        loadcase[i].waterplane2 = wpmom2;
        loadcase[i].waterplanecentroid = wpcen / length;		// give it as a percentage of length
    }

    outputs[0] = weight;
    outputs[1] = loadcase[0].Cp;
    outputs[2] = loadcase[0].freeboard;
    outputs[3] = loadcase[0].friction5;
    outputs[4] = loadcase[0].initstability;
    outputs[5] = loadcase[0].tippingangle;
    outputs[6] = loadcase[0].waterplane2;
    outputs[7] = loadcase[0].waterplanecentroid;
    outputs[8] = loadcase[0].paddlercentre;
//    outputs[9] = loadcase[1].Cp;
//    outputs[10] = loadcase[1].freeboard;
//    outputs[11] = loadcase[1].friction5;
//    outputs[12] = loadcase[1].initstability;
//    outputs[13] = loadcase[1].tippingangle;
//    outputs[14] = loadcase[1].waterplane2;
//    outputs[15] = loadcase[1].waterplanecentroid;
//    outputs[16] = loadcase[1].paddlercentre;

    return (error ? 1 : 0);
}

int Canoe::OutputMesh(ofstream& meshout) {
    int i, j;
    double n = 20;
    double x, y, z, t;
    double P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z;
    meshout << "0\n\n";
    double A1;
    double B1;
    double C1;
    double D1;

    double A3;
    double B3;
    double C3;
    double D3;
    for (i = 0;i < numstations;i++) {
        x = i * increment;
        ControlPoints(i, P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z);																																																																	//Chen was here... and so was Kilroy
        A1 = P0y - 3 * P1y + 3 * P2y - P3y;
        B1 = 3 * P1y - 6 * P2y + 3 * P3y;
        C1 = 3 * P2y - 3 * P3y;
        D1 = P3y;

        A3 = P0z - 3 * P1z + 3 * P2z - P3z;
        B3 = 3 * P1z - 6 * P2z + 3 * P3z;
        C3 = 3 * P2z - 3 * P3z;
        D3 = P3z;
        for (j = 0;j <= n;j++) {
            t = j / (double)n;
            y = loadcase[0].Spline(A1, B1, C1, D1, t);
            z = loadcase[0].Spline(A3, B3, C3, D3, t);
            meshout << length - x << "\t" << y << "\t" << z << "\n"; // x values need to be reversed for freeship
        }
        meshout << endl;
    }
    meshout << "EOF\n";
    return 0;
}


// Get Friction Force (Kaper wrapper function)
// Input v in m/s
double Canoe::GetFriction(double d, double v) {
    double volume, cx, cy, cz, ccenx, ccenz;
    if (Analyze(d, 0, 0, volume, cx, cy, cz) == 1) {
        cout << "LEAK - Terminating\n";
        return -1;
    }
    double LWL = GetLWL(d);
    double BWL = GetBWL(d);
    double EWL = GetEWL(d, 0, 0);
    double WS = SurfaceArea(d, ccenx, ccenz);
    double Cp = GetCp(d, LWL);
    double Cv = GetCv(volume, LWL);
    double LCB = cx / length;
    double disp = volume;
    double le = Getle(d);

    // Convert units just to be sure...
    LWL *= 3.2808399;			// metres to feet
    BWL *= 3.2808399;			// metres to feet
    EWL *= 3.2808399;			// metres to feet
    WS *= 10.7639104;			// square metres to square feet
    disp *= 0.984206528;		// cubic metres of water to long tons
    v *= 1.94384449;			// m/s to knots

    double force = Kaper(BWL, EWL, WS, Cp, Cv, LCB, disp, le, v);

    return force;
}

//read weights table
//create input table
//analyze
//output

//int Canoe::UIBulk(double ) {

//    loadcase[0].CreateInputs(Input);

//    int numcanoes;
//    // need dynamic input table
//    inputtable >> numcanoes;			// first line of file
//    int i;
//    for (i = 0; i < 15; i++) {
//        inputtable >> buffer;
//    }

//    Canoe c;
//    int flag;
//    for (i = 0; i < numcanoes; i++) {
//        flag = 0;
//        double canoenum, L, Lp, Ld, Lf, W, t1, t2, d, h, b, s, f, n, dens;
//        inputtable >> canoenum >> L >> Lp >> Ld >> Lf >> W >> t1 >> t2 >> d >> h >> b >> s >> f >> n >> dens;
//        c.InitializeCanoe(L, Lp, Ld, Lf, W, t1, t2, d, h, b, s, f, n, dens);
//        flag = c.AnalyzeAll();
//        resultsout << canoenum;

//        if (flag == 1) {
//            resultsout << '\t' << "Analysis Failed\n";
//            cout << "Canoe " << canoenum << " analysis failed.\n";
//        }
//        else {
//            for (int j = 0; j < 17; j++) {
//                resultsout << '\t' << c.outputs[j];
//            }
//            resultsout << '\t' << loadcase[0].Score(c.outputs, loadcase[0].targets[0], loadcase[0].targets[1], loadcase[0].targets[2], 17) << endl;
//            cout << "Canoe " << canoenum << " successfully analyzed.\n";
//            //		cout << canoenum << '\t' << Score(c.outputs,targets[0],targets[1],targets[2],17) << endl;
//        }

//        c.Destruct();
//    }

//    cout << "\n\n";


//    return 0;
//}

int Canoe::UIBulk() {
    cout << "(1) Analyze from an existing input setup file\n(2) Create a new input setup file and analyze from it\n(3) Back to main menu\n\n";

    int numeric;
    cout << "> ";
    cin >> numeric;
    cout << "\n\n";
    ifstream input;
    ofstream writeinput;
    double n1, n2, n3;
    ofstream inputfile;
    char buffer[128];


    switch (numeric) {
    case 1:
        cout << "Input filename: ";
        cin >> buffer;
        cout << "\n";
        input.open(buffer);
        if (input.fail()) {
            cout << "File not found\n\n\n";
            return 1;
        }
        break;
    case 2:
        cout << "Input input setup filename: ";
        cin >> buffer;
        cout << "\n\n";
        writeinput.open(buffer);

        cout << "Input all fields in the format [Minimum Value] [Maximum Value] [Number of Values]\n";

        writeinput << 14 << endl;
        cout << "Length: ";	cin >> n1 >> n2 >> n3;
        writeinput << "Length\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        cout << "Lp: ";	cin >> n1 >> n2 >> n3;
        writeinput << "Lp\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        cout << "Ld: ";	cin >> n1 >> n2 >> n3;
        writeinput << "Ld\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        cout << "Lf: ";	cin >> n1 >> n2 >> n3;
        writeinput << "Lf\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        cout << "W: ";	cin >> n1 >> n2 >> n3;
        writeinput << "W\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        cout << "t1: ";	cin >> n1 >> n2 >> n3;
        writeinput << "t1\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        cout << "t2: ";	cin >> n1 >> n2 >> n3;
        writeinput << "t2\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        cout << "d: ";	cin >> n1 >> n2 >> n3;
        writeinput << "d\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        writeinput << "h\t" << 0 << '\t' << 0 << '\t' << 1 << endl;
        cout << "b: ";	cin >> n1 >> n2 >> n3;
        writeinput << "b\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        cout << "s: ";	cin >> n1 >> n2 >> n3;
        writeinput << "s\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        cout << "f: ";	cin >> n1 >> n2 >> n3;
        writeinput << "f\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        cout << "n: ";	cin >> n1 >> n2 >> n3;
        writeinput << "n\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        cout << "Density: ";	cin >> n1 >> n2 >> n3;
        writeinput << "Density\t" << n1 << '\t' << n2 << '\t' << n3 << endl;
        writeinput.close();
        input.open(buffer);
        break;
    default:
        return 1;
    }

    // Do the bulk analysis

    cout << "\nInput input table filename: ";
    cin >> buffer;
    cout << "\n";
    inputfile.open(buffer);

    loadcase[0].CreateInputs(input, inputfile);
    inputfile.close();

    ifstream inputtable;
    inputtable.open(buffer);

    cout << "Input output table filename: ";
    cin >> buffer;
    cout << "\n";
    ofstream resultsout;
    resultsout.open(buffer);

    int numcanoes;
    inputtable >> numcanoes;			// first line of file
    int i;
    for (i = 0; i < 15; i++) {
        inputtable >> buffer;
    }

    Canoe c;
    int flag;
    for (i = 0; i < numcanoes; i++) {
        flag = 0;
        double canoenum, L, Lp, Ld, Lf, W, t1, t2, d, h, b, s, f, n, dens;
        inputtable >> canoenum >> L >> Lp >> Ld >> Lf >> W >> t1 >> t2 >> d >> h >> b >> s >> f >> n >> dens;
        c.InitializeCanoe(L, Lp, Ld, Lf, W, t1, t2, d, h, b, s, f, n, dens);
        flag = c.AnalyzeAll();
        resultsout << canoenum;

        if (flag == 1) {
            resultsout << '\t' << "Analysis Failed\n";
            cout << "Canoe " << canoenum << " analysis failed.\n";
        }
        else {
            for (int j = 0; j < 17; j++) {
                resultsout << '\t' << c.outputs[j];
            }
            resultsout << '\t' << loadcase[0].Score(c.outputs, loadcase[0].targets[0], loadcase[0].targets[1], loadcase[0].targets[2], 17) << endl;
            cout << "Canoe " << canoenum << " successfully analyzed.\n";
            //		cout << canoenum << '\t' << Score(c.outputs,targets[0],targets[1],targets[2],17) << endl;
        }

        c.Destruct();
    }

    cout << "\n\n";


    return 0;
}
