#include "TString.h"
// Directories to read and save the data and plots ,normaly saves as NuQ2Yo-?Phi-?Pt-?Z/ with ? the number of bins
const TString dataDirectory  = "/work/mbarrial/Data/";
const TString inputDirectory  = "/work/mbarrial/Data/";
TString outputDirectory = "/work/mbarrial/Data/60/";
const int N_Pt2 = 60;
float Pt2_BINS[N_Pt2+1];

const int UseCutOff = 1; // Select 1 Use the Cutoff and the interpolation
			// Select 2 Use the Cutoff 
			// Select 3 Dont use any processing 
// Number of  bins
#define N_Q2 3
#define N_Nu 3

// #define N_Zh 8
// const int N_Zh = 6;
#define N_Zh 8

// Bin of Zh to take in count in the integration
const int ZH_SUM = 1;
// const int ZH_SUM = 1;


#define N_Phi 6
// const int N_Phi = 6;

const float Delta_Pt2 = 3.0/N_Pt2;
const float Delta_Phi = 360.0/N_Phi;

// Number of solids targets an qd max number of pion
//const int N_PION = 3;
const int N_PION = 2;
const int N_STARGETS = 3;


// Limits
const float Q2_MIN = 1.0;
const float Q2_MAX = 4.0;
const float Nu_MIN = 2.2;
const float Nu_MAX = 4.26;
const float Zh_MIN = 0.0;
const float Zh_MAX = 1.0;
const float Pt2_MIN = 0.0;
const float Pt2_MAX = 3.0;
const float Phi_MIN = -180;
const float Phi_MAX = 180;


// B I N N I N G

// Stlip one pion events equaly
//const float Q2_BINS[N_Q2+1] = {1, 1.3, 1.8, 4.0};	
//const float Nu_BINS[N_Nu+1] = {2.2, 3.2, 3.7, 4.26};

// Stlip Two pion events equaly
const float Q2_BINS[N_Q2+1] = {1, 1.32, 1.74, 4.0};	
const float Nu_BINS[N_Nu+1] = {2.2, 3.36, 3.82, 4.26};


// Stlip Three pion events equaly
//const float Q2_BINS[N_Q2+1] = {1, 1.29, 1.68, 4.0};
//const float Nu_BINS[N_Nu+1] = {2.2, 3.59, 3.96, 4.26};

// const float Zh_BINS[N_Zh+1] = {0, 0.2, 0.4, 0.6 ,1.0};
// const float Zh_BINS[N_Zh+1] = {0, 0.1, 0.3, 0.4, 0.6, 0.7 , 1.0};
const float Zh_BINS[N_Zh+1] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6 , 0.8 , 1.0};

//const float Phi_BINS[N_Phi+1] = {-180, -90, 0, 90, 180};
const float Phi_BINS[N_Phi+1] = {-180, -120, -60, 0, 60, 120, 180};
// const float Phi_BINS[N_Phi+1] = {-180, -150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150, 180};

char DC[3] = {'D', 'C', '\0' }; char DFe[4] = {'D', 'F', 'e', '\0'}; char DPb[4] = {'D', 'P', 'b', '\0'};
char C[2]  = {'C', '\0'}; char Fe[3]  = {'F', 'e', '\0'}; char Pb[3]  = {'P', 'b', '\0'};

char cQ2[3] = {'Q', '2', '\0'}; char cNu[3] = {'N', 'u', '\0'}; char cZh[3] = {'Z', 'h', '\0'};


const float Q2_BINS_E[4] = {1, 1.3, 1.8, 4.0};
const float Nu_BINS_E[4] = {2.2, 3.2, 3.7, 4.26};
const float Zh_BINS_E[9] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6 , 0.8 , 1.0};
