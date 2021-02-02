#include "loadcase.h"


LoadCase::LoadCase()/*(QObject *parent) : QObject(parent)*/
{

}
//Functions to rewrite

/*

int OutputMesh(ofstream& meshout);

int OutputAll(ofstream& canoeout);

int ReadC4Table(ifstream& c4); - read from UI  -- done

int ReadWeightsTable(ifstream& targettable); - to read from UI -- done

int CreateInputs(ifstream& inputsetup, ofstream& writeinput); -- replace this with a sql table and a output to csv at the end

int OneInput(double* maxes, double* mins, int* nums, int* ranges, int numinputs, ofstream& writeinput); -- CreateInputs calls from Oneinput, lol .. so convulated ... still need to replace with sql
*/


double LoadCase::SplineArea(double P0y, double P0z, double P1y, double P1z,
    double P2y, double P2z, double P3y, double P3z,
    double ti, double tf) {

    double A1 = P0y - 3 * P1y + 3 * P2y - P3y;
    double B1 = 3 * P1y - 6 * P2y + 3 * P3y;
    double C1 = 3 * P2y - 3 * P3y;
    double D1 = P3y;

    double A2 = 0;
    double B2 = 3 * P0z - 9 * P1z + 9 * P2z - 3 * P3z;
    double C2 = 6 * P1z - 12 * P2z + 6 * P3z;
    double D2 = 3 * P2z - 3 * P3z;

    double E[7];
    E[6] = A1 * A2;
    E[5] = A1 * B2 + B1 * A2;
    E[4] = A1 * C2 + B1 * B2 + C1 * A2;
    E[3] = A1 * D2 + B1 * C2 + C1 * B2 + D1 * A2;
    E[2] = B1 * D2 + C1 * C2 + D1 * B2;
    E[1] = C1 * D2 + D1 * C2;
    E[0] = D1 * D2;

    double area = 0;
    int i;
    for (i = 0; i < 7; i++) {
        area += E[i] / (i + 1) * (pow(tf, (i + 1)) - pow(ti, (i + 1)));
    }
    return area;
}

int LoadCase::SplineMoment(double P0y, double P0z, double P1y, double P1z,
    double P2y, double P2z, double P3y, double P3z,
    double ti, double tf, double& momentareay, double& momentareaz) {

    double A1 = P0y - 3 * P1y + 3 * P2y - P3y;
    double B1 = 3 * P1y - 6 * P2y + 3 * P3y;
    double C1 = 3 * P2y - 3 * P3y;
    double D1 = P3y;

    double A2 = 0;
    double B2 = 3 * P0z - 9 * P1z + 9 * P2z - 3 * P3z;
    double C2 = 6 * P1z - 12 * P2z + 6 * P3z;
    double D2 = 3 * P2z - 3 * P3z;

    double A3 = P0z - 3 * P1z + 3 * P2z - P3z;
    double B3 = 3 * P1z - 6 * P2z + 3 * P3z;
    double C3 = 3 * P2z - 3 * P3z;
    double D3 = P3z;

    double A4 = 0;
    double B4 = 3 * P0y - 9 * P1y + 9 * P2y - 3 * P3y;
    double C4 = 6 * P1y - 12 * P2y + 6 * P3y;
    double D4 = 3 * P2y - 3 * P3y;

    double E[7];
    E[6] = A1 * A2;
    E[5] = A1 * B2 + B1 * A2;
    E[4] = A1 * C2 + B1 * B2 + C1 * A2;
    E[3] = A1 * D2 + B1 * C2 + C1 * B2 + D1 * A2;
    E[2] = B1 * D2 + C1 * C2 + D1 * B2;
    E[1] = C1 * D2 + D1 * C2;
    E[0] = D1 * D2;

    double L[10];
    L[9] = A3 * E[6];
    L[8] = B3 * E[6] + A3 * E[5];
    L[7] = C3 * E[6] + B3 * E[5] + A3 * E[4];
    L[6] = D3 * E[6] + C3 * E[5] + B3 * E[4] + A3 * E[3];
    L[5] = D3 * E[5] + C3 * E[4] + B3 * E[3] + A3 * E[2];
    L[4] = D3 * E[4] + C3 * E[3] + B3 * E[2] + A3 * E[1];
    L[3] = D3 * E[3] + C3 * E[2] + B3 * E[1] + A3 * E[0];
    L[2] = D3 * E[2] + C3 * E[1] + B3 * E[0];
    L[1] = D3 * E[1] + C3 * E[0];
    L[0] = D3 * E[0];

    double Ey[7];
    Ey[6] = A3 * A4;
    Ey[5] = A3 * B4 + B3 * A4;
    Ey[4] = A3 * C4 + B3 * B4 + C3 * A4;
    Ey[3] = A3 * D4 + B3 * C4 + C3 * B4 + D3 * A4;
    Ey[2] = B3 * D4 + C3 * C4 + D3 * B4;
    Ey[1] = C3 * D4 + D3 * C4;
    Ey[0] = D3 * D4;

    double Ly[10];
    Ly[9] = A1 * Ey[6];
    Ly[8] = B1 * Ey[6] + A1 * Ey[5];
    Ly[7] = C1 * Ey[6] + B1 * Ey[5] + A1 * Ey[4];
    Ly[6] = D1 * Ey[6] + C1 * Ey[5] + B1 * Ey[4] + A1 * Ey[3];
    Ly[5] = D1 * Ey[5] + C1 * Ey[4] + B1 * Ey[3] + A1 * Ey[2];
    Ly[4] = D1 * Ey[4] + C1 * Ey[3] + B1 * Ey[2] + A1 * Ey[1];
    Ly[3] = D1 * Ey[3] + C1 * Ey[2] + B1 * Ey[1] + A1 * Ey[0];
    Ly[2] = D1 * Ey[2] + C1 * Ey[1] + B1 * Ey[0];
    Ly[1] = D1 * Ey[1] + C1 * Ey[0];
    Ly[0] = D1 * Ey[0];

    momentareaz = 0;
    momentareay = 0;
    int i;
    for (i = 0; i < 10; i++) {
        momentareaz += L[i] / (i + 1) * (pow(tf, (i + 1)) - pow(ti, (i + 1)));
        momentareay += Ly[i] / (i + 1) * (pow(tf, (i + 1)) - pow(ti, (i + 1)));
    }

    double Piy = Spline(A1, B1, C1, D1, ti);
    double Piz = Spline(A3, B3, C3, D3, ti);
    double Pfy = Spline(A1, B1, C1, D1, tf);
    double Pfz = Spline(A3, B3, C3, D3, tf);

    momentareay += Piy * Piy * Piz / 2 - Pfy * Pfy * Pfz / 2;	// extra rectangular regions

    //momentareaz = momentareaz;		// I think the negatives should be here...
    momentareay = -momentareay;			// But not here?

    return 0;
}

double LoadCase::SolveCubic(double a, double b, double c, double d) {


    double ap, bp, cp, p, q, f;
    double rootfr, rootfi, gr, gi, magg, argg, magu1, argu1, u1r, u1i, u2r, u2i, u3r, u3i, x1r, x1i, x2r, x2i, x3r, x3i;

    ap = b / a;
    bp = c / a;
    cp = d / a;

    p = bp - ap * ap / 3.;
    q = cp + (2 * ap * ap * ap - 9 * ap * bp) / 27.;

    if (p == 0 && q == 0)
        return -ap / 3.;
    if (p == 0) {
        magg = q;
        argg = 0;
    }
    else {
        f = q * q / 4. + p * p * p / 27.;
        if (f < 0) {
            rootfr = 0;
            rootfi = sqrt(-f);
        }
        else {
            rootfr = sqrt(f);
            rootfi = 0;
        }

        gr = rootfr + q / 2.;
        gi = rootfi;

        magg = sqrt(gr * gr + gi * gi);
        argg = atan2(gi, gr) + (rootfi < 0 ? 2 * PI : 0);
    }

    magu1 = pow(magg, 1. / 3.);
    argu1 = argg / 3.;

    u1r = magu1 * cos(argu1);
    u1i = magu1 * sin(argu1);

    u2r = -0.5 * u1r - sqrt(3) / 2. * u1i;
    u2i = -0.5 * u1i + sqrt(3) / 2. * u1r;

    u3r = -0.5 * u1r + sqrt(3) / 2. * u1i;
    u3i = -0.5 * u1i - sqrt(3) / 2. * u1r;

    x1r = p / 3. * u1r / (u1r * u1r + u1i * u1i) - u1r - ap / 3.;
    x1i = -p / 3. * u1i / (u1r * u1r + u1i * u1i) - u1i;

    x2r = p / 3. * u2r / (u2r * u2r + u2i * u2i) - u2r - ap / 3.;
    x2i = -p / 3. * u2i / (u2r * u2r + u2i * u2i) - u2i;

    x3r = p / 3. * u3r / (u3r * u3r + u3i * u3i) - u3r - ap / 3.;
    x3i = -p / 3. * u3i / (u3r * u3r + u3i * u3i) - u3i;

    //	cout << x1r << " + " << x1i << "i\n";
    //	cout << x2r << " + " << x2i << "i\n";
    //	cout << x3r << " + " << x3i << "i\n";

        // determine which real root falls between 0 and 1 - if none do, return -1
    double threshold = 1e-10;		// zero threshold
    double ROUNDOFF = threshold * 1000;	// this one is killing me
    if (x1r <= 1 + ROUNDOFF && x1r >= 0 - ROUNDOFF && fabs(x1i) < threshold)
        return x1r;
    else if (x2r <= 1 + ROUNDOFF && x2r >= 0 - ROUNDOFF && fabs(x2i) < threshold)
        return x2r;
    else if (x3r <= 1 + ROUNDOFF && x3r >= 0 - ROUNDOFF && fabs(x3i) < threshold)
        return x3r;

    return -1;

}

int LoadCase::SectionResultant(double P0y, double P0z, double P1y, double P1z,
    double P2y, double P2z, double P3y, double P3z,
    double d, double theta,												// d is freeboard
    double& magnitude, double& yloc, double& zloc) {

    if (d > -P3z) {
        magnitude = 0;
        yloc = 0;
        zloc = 0;
        return 0;
    }

    int flag = 0;

    double A1 = P0y - 3 * P1y + 3 * P2y - P3y;
    double B1 = 3 * P1y - 6 * P2y + 3 * P3y;
    double C1 = 3 * P2y - 3 * P3y;
    double D1 = P3y;

    double A3 = P0z - 3 * P1z + 3 * P2z - P3z;
    double B3 = 3 * P1z - 6 * P2z + 3 * P3z;
    double C3 = 3 * P2z - 3 * P3z;
    double D3 = P3z;

    double ti, tfr, tfl;
    if (d == 0 && theta == 0) {
        ti = 0;
        tfr = 1;
        tfl = 1;
    }
    else {
        ti = 0;								// lower bound
        tfr = SolveCubic(A3 - tan(theta) * A1, B3 - tan(theta) * B1, C3 - tan(theta) * C1, D3 - tan(theta) * D1 + d - P0z);	// upper bound right side
        if (tfr < 0)
            flag = -1;				// leak point
        tfl = SolveCubic(A3 + tan(theta) * A1, B3 + tan(theta) * B1, C3 + tan(theta) * C1, D3 + tan(theta) * D1 + d - P0z);	// upper bound left side
        if (tfl < 0)
            flag = -1;
    }

    double arearspline = SplineArea(P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z, ti, tfr);
    double arealspline = SplineArea(P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z, ti, tfl);

    double momyrspline, momzrspline, momylspline, momzlspline;
    SplineMoment(P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z, ti, tfr, momyrspline, momzrspline);
    SplineMoment(P0y, P0z, P1y, P1z, P2y, P2z, P3y, P3z, ti, tfl, momylspline, momzlspline);

    momylspline = -momylspline;

    double ytfr = Spline(A1, B1, C1, D1, tfr);
    double ztfr = Spline(A3, B3, C3, D3, tfr);
    double ytfl = -Spline(A1, B1, C1, D1, tfl);
    double ztfl = Spline(A3, B3, C3, D3, tfl);

    double areartri = -ytfr * ((P0z - d) - ztfr) / 2;
    double arealtri = -ytfl * ((P0z - d) - ztfl) / 2;

    double arear = arearspline - areartri;		// subtract triangular portion - there's an explicit negative here
    double areal = arealspline + arealtri;		// add triangular portion

    magnitude = arear + areal;

    double ymoment = momyrspline - 1. / 3. * ytfr * areartri + momylspline + 1. / 3. * ytfl * arealtri;
    double zmoment = momzrspline - (ztfr - 1. / 3. * (ztfr - (P0z - d))) * areartri
        + momzlspline + (P0z - d - 2. / 3. * ((P0z - d) - ztfl)) * arealtri;

    yloc = (magnitude == 0 ? 0 : ymoment / magnitude);
    zloc = (magnitude == 0 ? 0 : zmoment / magnitude);

    if (flag == -1)
        return -1;
    return 0;
}

double LoadCase::Spline(double A, double B, double C, double D, double t) {
    return (A * t * t * t + B * t * t + C * t + D);
}



double LoadCase::Ramp(double x, double t) {
    return (t / 2. * log(0.5 * exp(x / t) + 0.5 * exp(-x / t)) + x / 2. + t * log(2.) / 2.);
}

double LoadCase::Moment(double y1, double z1, double y2, double z2, double weight, double theta) {
    return -(cos(theta) * (y2 - y1) - sin(theta) * (z2 - z1)) * weight;
}


double LoadCase::SplineLength(double P0y, double P0z, double P1y, double P1z,
    double P2y, double P2z, double P3y, double P3z,
    double d, double &cmz) {
    if (d > -P3z) {
        return 0;
    }

    double A1 = P0y - 3 * P1y + 3 * P2y - P3y;
    double B1 = 3 * P1y - 6 * P2y + 3 * P3y;
    double C1 = 3 * P2y - 3 * P3y;
    double D1 = P3y;

    double A3 = P0z - 3 * P1z + 3 * P2z - P3z;
    double B3 = 3 * P1z - 6 * P2z + 3 * P3z;
    double C3 = 3 * P2z - 3 * P3z;
    double D3 = P3z;

    double ti = 0;								// lower bound
    double tf = SolveCubic(A3, B3, C3, D3 + d - P0z);	// upper bound
    if (tf < 0)
        tf = 0;			// if there's a leak we ignore the section - presumably there should not be any leaks in this routine
    if (d == 0)
        tf = 1;

    double t;
    double tinc = 0.01;	// UNDEFINED CONSTANT
    double y1, z1, y2, z2;
    double l = 0;

    y1 = Spline(A1, B1, C1, D1, ti);
    z1 = Spline(A3, B3, C3, D3, ti);

    double zmoment = 0;

    for (t = ti + tinc; t <= tf; t += tinc) {
        y2 = Spline(A1, B1, C1, D1, t);
        z2 = Spline(A3, B3, C3, D3, t);
        double segment = sqrt((y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
        l += segment;
        zmoment += segment * (z1 + z2) / 2.;		// moment = length * average height

        y1 = y2;
        z1 = z2;
    }

    cmz = zmoment / l;		// z centre of mass

    return l;
}



//int ReadC4Table(ifstream& c4) {
//    int i, j, k;
//    for (i = 0; i < 3; i++) {
//        for (j = 0; j < 17; j++) {
//            for (k = 0; k < 14; k++) {
//                c4 >> C4TABLE[i][j][k];
//            }
//        }
//    }
//    return 0;
//} old ReadC4Table


// will probably need to link this to the event cpp
int LoadCase::ReadC4Table(double c4[3][17][14]){
    int i,j,k;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 17; j++) {
            for (k = 0; k < 14; k++) {
                C4TABLE[i][j][k] = &c4[i][j][k];
            }
        }
    }
    return 0;
}

// Look up c4 in the table
double LoadCase::GetC4(double Cv, double Cp, double vtol) {
    int Cvind1 = (int)((Cv - .001) / .0005);
    int Cvind2 = (int)((Cv - .001) / .0005) + 1;
    if (Cvind1 < 0) Cvind1 = 0;
    if (Cvind1 > 2) Cvind1 = 2;
    if (Cvind2 < 0) Cvind2 = 0;
    if (Cvind2 > 2) Cvind2 = 2;

    int Cpind1 = (int)((Cp - 0.48) / 0.01);
    int Cpind2 = (int)((Cp - 0.48) / 0.01) + 1;
    if (Cpind1 < 0) Cpind1 = 0;
    if (Cpind1 > 16) Cpind1 = 16;
    if (Cpind2 < 0) Cpind2 = 0;
    if (Cpind2 > 16) Cpind2 = 16;

    int vtolind1 = (int)((vtol - 0.4) / 0.1);
    int vtolind2 = (int)((vtol - 0.4) / 0.1) + 1;
    if (vtolind1 < 0) vtolind1 = 0;
    if (vtolind1 > 13) vtolind1 = 13;
    if (vtolind2 < 0) vtolind2 = 0;
    if (vtolind2 > 13) vtolind2 = 13;

    double Cvratio, Cpratio, vtolratio;
    if (Cvind1 == Cvind2)
        Cvratio = 0;
    else
        Cvratio = (Cv - (Cvind1 * .0005 + .001)) / (Cvind2 * .0005 - Cvind1 * .0005);
    if (Cpind1 == Cpind2)
        Cpratio = 0;
    else
        Cpratio = (Cp - (Cpind1 * .01 + .48)) / (Cpind2 * .01 - Cpind1 * .01);
    if (vtolind1 == vtolind2)
        vtolratio = 0;
    else
        vtolratio = (vtol - (vtolind1 * .1 + .4)) / (vtolind2 * .1 - vtolind1 * .1);

    // 8 corners of the prism to be interpolated
    double corners[2][2][2];
    int i, j, k;

    for (i = Cvind1; i <= Cvind2; i++) {
        for (j = Cpind1; j <= Cpind2; j++) {
            for (k = vtolind1; k <= vtolind2; k++) {
                corners[i - Cvind1][j - Cpind1][k - vtolind1] = *C4TABLE[i][j][k];
            }
        }
    }

    double actualCv[2][2];
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 1; j++) {
            actualCv[i][j] = Cvratio * corners[1][i][j] + (1 - Cvratio) * corners[0][i][j];
        }
    }

    double actualCp[2];
    for (i = 0; i <= 1; i++) {
        actualCp[i] = Cpratio * actualCv[1][i] + (1 - Cpratio) * actualCv[0][i];
    }

    double c4 = vtolratio * actualCp[1] + (1 - vtolratio) * actualCp[0];
    return c4;
}

double LoadCase::Score(double* values, double* targetmeans[], double* targetstds[], double* weights[], int n) {
    int i;
    double score = 0;
    for (i = 0; i < n; i++) {
        score += *weights[i] * Gaussian(*targetmeans[i], *targetstds[i], values[i]);
    }
    return score;
}

double LoadCase::Gaussian(double mean, double std, double x) {
    return 1. * exp(-(x - mean) * (x - mean) / 2 / std / std);
}

/*
int ReadWeightsTable(ifstream& targettable) {
    int i, j;
    char buffer[128];
    for (i = 0; i < 17; i++) {
        targettable >> buffer; // this skips the initial input
        for (j = 0; j < 3; j++) {
            targettable >> targets[j][i];
        }
    }
    return 0;
}*/ // old ReadWeightsTable

int LoadCase::ReadWeightsTable(double Weights[3][17]) {
    //Create Weight Targets, assuming that UI passes already an input of [3][17] matrix of only the data , will adjust accordingly
    int i, j;
    for (i = 0; i < 17; i++) {
        for (j = 0; j < 3; j++) {
            targets[j][i] = &Weights[j][i];
        }
    }
    return 0;}

//int CreateInputs(double Input[3][14])
//{
//    int i = 0;
//    int numinputs = 14;

//    vector<double> dataVec;
//    //int dataArray[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
//    //unsigned dataArraySize = sizeof(dataArray) / sizeof(int);
//    unsigned dataArraySize = 14;

//    //dataVec.insert(dataVec.end(), &dataArray[0], &dataArray[dataArraySize]);
//    for (i = 0; i < numinputs; i++) {
//    //create new list and append to
//    //&Input[i][j]
//}

int LoadCase::CreateInputs(ifstream& inputsetup, ofstream& writeinput) {

    int numinputs;
    inputsetup >> numinputs;

    char buffer[128];
    double* mins = new double[numinputs];
    double* maxes = new double[numinputs];
    int* nums = new int[numinputs];
    int* ranges = new int[numinputs];

    int i = 0;
    int product = 1;
    for (i = 0; i < numinputs; i++) {
        inputsetup >> buffer;
        inputsetup >> mins[i];
        inputsetup >> maxes[i];
        inputsetup >> nums[i];
        ranges[i] = 1;
        product *= nums[i];
    }

    writeinput << product << endl;
    writeinput << "Canoe\tLength\tLp\tLd\tLf\tW\tt1\tt2\td\th\tb\ts\tf\tn\tdensity\n";

    i = 0;
    int count = 1;
    writeinput << count++ << '\t';
    OneInput(maxes, mins, nums, ranges, numinputs, writeinput);
    while (i < numinputs) {
        if (ranges[i] < nums[i]) {
            ranges[i]++;
            writeinput << count++ << '\t';
            OneInput(maxes, mins, nums, ranges, numinputs, writeinput);
            i = 0;
        }
        else if (ranges[i] == nums[i]) {
            ranges[i] = 1;
            i++;
        }
    }

    delete[] mins;
    delete[] maxes;
    delete[] nums;
    delete[] ranges;

    return 0;
}

int LoadCase::OneInput(double* maxes, double* mins, int* nums, int* ranges, int numinputs, ofstream& writeinput) {
    int i;

    for (i = 0; i < numinputs; i++) {
        writeinput << (nums[i] == 1 ? mins[i] : (maxes[i] - mins[i]) * (1.0 * ranges[i] - 1.0) / (1.0 * nums[i] - 1.0) + mins[i]) << '\t';
    }
    writeinput << endl;

    return 0;
}
