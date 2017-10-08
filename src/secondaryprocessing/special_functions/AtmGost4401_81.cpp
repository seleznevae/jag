#include "secondaryprocessing/special_functions/AtmGost4401_81.h"

#include <cmath>

//   THIS SHIT wasn't completed
// Functions were not tested !!!!!!!!!  !!!!!

namespace yAtmosphere{

const double TABLE4[9][4]={//TRANSITION POINTS
    00000 , -0.0065 , 288.150 , 1.01325000000000E+5 ,
    11000 ,  0.0000 , 216.650 , 2.26320639734629E+4 ,
    20000 ,  0.0010 , 216.650 , 5.47487E+3 ,
    32000 ,  0.0028 , 228.650 , 8.68014E+2 ,
    47000 ,  0.0000 , 270.650 , 1.10906E+2 ,
    51000 , -0.0028 , 270.650 , 6.69384E+1 ,
    71000 , -0.0020 , 214.650 , 3.95639E+0 ,   //!!!!!!!!!!!!!
    85000 ,  0.0000 , 186.650 , 4.45710E-1 ,       //!!!!!!!!!!! in table data are in geometrical altitude!!
    94000 ,  0.0000 , 186.650 , 8.E-2,              ///////
/*   ^         ^        ^             ^
     ^         ^        ^             ^----------Pressure vector
     ^         ^        ^------------------------Temperature vector
     ^         ^---------------------------------?
     ^------------------------------------------- GEOPOTENTIAL ALTITUDE
      */
};


//TEMPERATURE (K)
inline double Temperature(double z)
{// z - ALTITUDE (m) (T IS VALID FOR -5,000 m < z < 86,000 m)



    double H = z * 6356766 / (z + 6356766);
    int b;
    for (b = 0; b < 7; ++b)
        if (H < TABLE4[b + 1][0])
            break;
    return TABLE4[b][2] + TABLE4[b][1] * (H - TABLE4[b][0]);
}


//PRESSURE (Pa)
inline double Pressure(double z)
{// z - ALTITUDE (m) (P IS VALID FOR -5,000 m < z < 86,000 m)
    double H = z * 6356766 / (z + 6356766);
    int b;
    for (b = 0; b < 7; ++b)
        if (H < TABLE4[b + 1][0])
            break;
    double C = -.0341631947363104;//C = -G0*M0/RSTAR
    double Hb = TABLE4[b][0];
    double Lb = TABLE4[b][1];
    double Tb = TABLE4[b][2];
    double Pb = TABLE4[b][3];
    return Pb * (fabs(Lb) > 1E-12 ? pow(1 + Lb/Tb*(H-Hb), C/Lb) : exp(C*(H-Hb)/Tb));
}

//DENSITY (kg/m^3)
inline double Density(double T, double P)
{ // T -TEMPERATURE (K) (CALCULATE T USING Temperature()) and  P -PRESSURE (Pa) (CALCULATE P USING Pressure())
    return P * .00348367635597379 / T;
}

// SPEED OF SOUND (m/s)
inline double SpeedofSound(double T)
{
//  T  - TEMPERATURE (K) (CALCULATE T USING Temperature())
    return sqrt(401.87430086589 * T);
}


//ACCELERATION DUE TO GRAVITY (m/s^2)
inline double Gravity(double z)
{
    //z ----ALTITUDE (m) (g IS VALID FOR -5,000 m < z < 86,000 m)
    return 9.80665 * pow(1 + z/6356766, -2);
}

}







namespace atmGost4401_81 {

double atmGost_HToRho(double h)
{
    return yAtmosphere::Density(yAtmosphere::Temperature(h), yAtmosphere::Pressure(h));
}

double atmGost_HToA(double h)
{
    return yAtmosphere::SpeedofSound(yAtmosphere::Temperature(h));
}

double atmGost_HToG(double h)
{
    return yAtmosphere::Gravity(h);
}

double atmGost_HToT(double h)
{
    return yAtmosphere::Temperature(h);
}

double atmGost_HToP(double h)
{
    return yAtmosphere::Pressure(h);
}

}



