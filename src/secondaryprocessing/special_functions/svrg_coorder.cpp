#include "svrg_coorder.h"
#include "math.h"



namespace {
//    void dd_to_dms (double dd, double dms[3]);
//    void dms_to_dd (double dms[3], double *dd);
    void geo_cc(int key, double *fi, double *lam, double *h);
}









namespace geo_conv {

double geo_cc2fi (int key, double fi, double lam, double h)
{
    geo_cc(key, &fi, &lam, &h); // geo coordinate converter
    return fi;
}



double geo_cc2lam(int key, double fi, double lam, double h)
{
    geo_cc(key, &fi, &lam, &h); // geo coordinate converter
    return lam;
}

double geo_cc2h  (int key, double fi, double lam, double h)
{
    geo_cc(key, &fi, &lam, &h); // geo coordinate converter
    return h;
}

}












namespace {
//void dd_to_dms (double dd, double dms[3]) // decimal degrees to degrees, minutes, seconds
//{
//    dms[0] = int (dd);
//    dms[1] = int (60.0*(dd - dms[0]));
//    dms[2] = 60.0*(60.0*(dd - dms[0]) - dms[1]);
//}

//void dms_to_dd (double dms[3], double *dd) // degrees, minutes, seconds to decimal degrees
//{
//    *dd = dms[0] + (dms[1] + dms[2]/60.0)/60.0;
//}

void geo_cc(int key, double *fi, double *lam, double *h) // geo coordinate converter
/*
<key> set conversation type
0 = reserverd
1 = from SK-42 to PZ-90.02
2 = from PZ-90.02 to SK-42
3 = from SK-95 to PZ-90.02
4 = from PZ-90.02 to SK-95
5 = from SK-42 to PZ-90
6 = from PZ-90 to SK-42
7 = from SK-95 to PZ-90
8 = from PZ-90 to SK-95
9 = from PZ-90.02 to WGS 84
10 = from WGS 84 to PZ-90.02
11 = from PZ-90 to WGS 84
12 = from WGS 84 to PZ-90
13 = from PZ-90 to PZ-90.02
14 = from PZ-90.02 to PZ-90
*/
{

    double delta_x = 0, delta_y = 0, delta_z = 0, omega_x = 0, omega_y = 0, omega_z = 0, m = 0, aa = 0, ab = 0, alfa = 0, alfb = 0, ro = 206264.806, B_in = 0, L_in = 0, H_in = 0, ea = 0, eb = 0, a = 0, e2 = 0, delta_a = 0, delta_e2 = 0, M = 0, N = 0, B_delta = 0, L_delta = 0, H_delta = 0;
	switch (key)
	{
		case 0: // reserved
			break;
		case 1: // from SK-42 to PZ-90.02
			delta_x = 23.93;
			delta_y = -141.03;
			delta_z = -79.98;
			omega_x = 0.0;
			omega_y = -0.35;
			omega_z = -0.79;
			m = -0.22*pow(10.0,-6.0);
			aa = 6378245.0;
			ab = 6378136.0;
			alfa = 1/298.3;
			alfb = 1/298.25784;
			break;
		case 2: // from PZ-90.02 to SK-42
			delta_x = -23.93;
			delta_y = 141.03;
			delta_z = 79.98;
			omega_x = 0;
			omega_y = 0.35;
			omega_z = 0.79;
			m = 0.22*pow(10.0,-6.0);
			aa = 6378136.0;
			ab = 6378245.0;
			alfa = 1/298.25784;
			alfb = 1/298.3;
			break;
		case 3: // from SK-95 to PZ-90.02
			delta_x = 24.83;
			delta_y = -130.97;
			delta_z = -81.74;
			omega_x = 0.0;
			omega_y = 0.0;
			omega_z = -0.13;
			m = -0.22*pow(10.0,-6.0);
			aa = 6378245.0;
			ab = 6378136.0;
			alfa = 1/298.3;
			alfb = 1/298.25784;
			break;
		case 4: // from PZ-90.02 to SK-95
			delta_x = -24.83;
			delta_y = 130.97;
			delta_z = 81.74;
			omega_x = 0.0;
			omega_y = 0.0;
			omega_z = 0.13;
			m = 0.22*pow(10.0,-6.0);
			aa = 6378136.0;
			ab = 6378245.0;
			alfa = 1/298.25784;
			alfb = 1/298.3;
			break;
		case 5: // from SK-42 to PZ-90
			delta_x = 25.0;
			delta_y = -141.0;
			delta_z = -80.0;
			omega_x = 0.0;
			omega_y = -0.35;
			omega_z = -0.66;
			m = 0.0;
			aa = 6378245.0;
			ab = 6378136.0;
			alfa = 1.0/298.3;
			alfb = 1.0/298.25784;
			break;
		case 6: // from PZ-90 to SK-42
			delta_x = -25.0;
			delta_y = 141.0;
			delta_z = 80.0;
			omega_x = 0.0;
			omega_y = 0.35;
			omega_z = 0.66;
			m = 0.0;
			aa = 6378136.0;
			ab = 6378245.0;
			alfa = 1.0/298.25784;
			alfb = 1.0/298.3;
			break;
		case 7: // from SK-95 to PZ-90
			delta_x = 25.9;
			delta_y = -130.94;
			delta_z = -81.76;
			omega_x = 0.0;
			omega_y = 0.0;
			omega_z = 0.0;
			m = 0.0;
			aa = 6378245.0;
			ab = 6378136.0;
			alfa = 1/298.3;
			alfb = 1/298.25784;
			break;
		case 8: // from PZ-90 to SK-95
			delta_x = -25.9;
			delta_y = 130.94;
			delta_z = 81.76;
			omega_x = 0.0;
			omega_y = 0.0;
			omega_z = 0.0;
			m = 0.0;
			aa = 6378136.0;
			ab = 6378245.0;
			alfa = 1/298.25784;
			alfb = 1/298.3;
			break;
		case 9: // from PZ-90.02 to WGS 84
			delta_x = -0.36;
			delta_y = 0.08;
			delta_z = 0.18;
			omega_x = 0.0;
			omega_y = 0.0;
			omega_z = 0.0;
			m = 0.0;
			aa = 6378136.0;
			ab = 6378137.0;
			alfa = 1/298.25784;
			alfb = 1/298.257223563;
			break;
		case 10: // from WGS 84 to PZ-90.02
			delta_x = 0.36;
			delta_y = -0.08;
			delta_z = -0.18;
			omega_x = 0.0;
			omega_y = 0.0;
			omega_z = 0.0;
			m = 0.0;
			aa = 6378137.0;
			ab = 6378136.0;
			alfa = 1/298.257223563;
			alfb = 1/298.25784;
			break;
		case 11: // from PZ-90 to WGS 84
			delta_x = -1.1;
			delta_y = -0.3;
			delta_z = -0.9;
			omega_x = 0.0;
			omega_y = 0.0;
			omega_z = -0.2;
            m = -0.12*pow(10.0,-6.0);
			aa = 6378136.0;
			ab = 6378137.0;
			alfa = 1/298.25784;
			alfb = 1/298.257223563;
			break;
		case 12: // from WGS 84 to PZ-90
			delta_x = 1.1;
			delta_y = 0.3;
			delta_z = 0.9;
			omega_x = 0.0;
			omega_y = 0.0;
			omega_z = 0.2;
			m = 0.12*pow(10.0,-6.0);
			aa = 6378137.0;
			ab = 6378136.0;
			alfa = 1/298.257223563;
			alfb = 1/298.25784;
			break;
		case 13: // from PZ-90 to PZ-90.02
			delta_x = 1.07;
			delta_y = 0.03;
			delta_z = -0.02;
			omega_x = 0.0;
			omega_y = 0.0;
			omega_z = 0.13;
			m = 0.22*pow(10.0,-6.0);
			aa = 6378136.0;
			ab = 6378136.0;
			alfa = 1/298.25784;
			alfb = 1/298.25784;
			break;
		case 14: // from PZ-90.02 to PZ-90
			delta_x = -1.07;
			delta_y = -0.03;
			delta_z = 0.02;
			omega_x = 0.0;
			omega_y = 0.0;
			omega_z = -0.13;
			m = -0.22*pow(10.0,-6.0);
			aa = 6378136.0;
			ab = 6378136.0;
			alfa = 1/298.25784;
			alfb = 1/298.25784;
			break;
		default:
			break;
	}
    B_in = *fi;
    L_in = *lam;
    H_in = *h;
    ea = 2.0*alfa-alfa*alfa;
    eb = 2.0*alfb-alfb*alfb;
    a = (ab + aa)/2.0;
    e2 = (eb*eb-ea*eb)/2.0;
    delta_a = ab - aa;
    delta_e2 = eb*eb - ea*ea;
    M = a*(1.0-e2)*pow((1-(e2*(sin(B_in)*sin(B_in)))),(-3.0/2.0));
    N = a*(pow((1.0-e2*(sin(B_in)*sin(B_in))),(-1.0/2.0)));
    B_delta = (ro/(M+H_in))*((N/a)*e2*sin(B_in)*cos(B_in)*delta_a+((N*N)/(a*a)+1.0)*N*sin(B_in)*cos(B_in)*(delta_e2/2.0)-(delta_x*cos(L_in)+delta_y*sin(L_in))*sin(B_in)+delta_z*cos(B_in))-omega_x*sin(L_in)*(1.0+e2*cos(2.0*B_in))+omega_y*cos(L_in)*(1.0+e2*cos(2.0*B_in))-ro*m*e2*sin(B_in)*cos(B_in);
    L_delta = (ro/((N+H_in)*cos(B_in)))*(-delta_x*sin(L_in)+delta_y*cos(L_in))+tan(B_in)*(1.0-e2)*(omega_x*cos(L_in)+omega_y*sin(L_in))-omega_z;
    H_delta = (-a/N)*delta_a+N*(sin(B_in)*sin(B_in))*(delta_e2/2.0)+(delta_x*cos(L_in)+delta_y*sin(L_in))*cos(B_in)+delta_z*sin(B_in)-N*e2*sin(B_in)*cos(B_in)*(omega_x*sin(L_in)/ro-omega_y*cos(L_in)/ro)+((a*a)/N+H_in)*m;
    *fi += B_delta/ro;
    *lam += L_delta/ro;
    *h += H_delta;
}

}
