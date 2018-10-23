/*********************************************************/
/* solar_radiation                                       */
/* Author: N.Manago Nov,04,2009                          */
/* $Revision: 264 $                                      */
/* $Date: 2014-09-01 01:37:01 +0900 (Mon, 01 Sep 2014) $ */
/*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <math.h>
//#include <bits/nan.h>
#include <complex.h>
#include <time.h>
#include <sys/time.h>
#include "aeros_profile.h"
#include "strutil.h"

// Mathematical constants
#define	PI				3.141592653589793	// PI
#define	PI2				6.283185307179586	// 2*PI
#define	R_TO_D				57.29577951308232	// 180/PI rad -> deg
#define	D_TO_R				1.745329251994329e-02	// PI/180 deg -> rad
// Physical constants
#define	SEC_DAY				86400
// Common constants
#define	NONAME				"NONE"			// No name
#define	NODATA				0			// No data
#define	INVALID				-1			// Invalid value
#define	MAXENTR				25			// Max #entries in a line
#define	MAXLINE				256			// Max #chars in a line
#define	EPSILON				1.0e-14			// A small number
#define	DELTA				1.0e-12			// A small number
// Constants for Direct Solar radiation
#define	DSR_MAXWLEN			256			// Max #wavelengths for DSR
// Constants for Scattered Solar Radiation
#define	SSR_MAXWLEN			256			// Max #wavelengths for SSR
#define	SSR_SBMP			15			// Band model parameter
// Constants for Simulation
#define	SIM_N_ANGL			50			// #Angles
#define	SIM_MAXDATA			50000			// #Data
#define	SIM_MAXCONV			1000			// #Data
#define	SIM_NCOL			8			// #Columns
#define	SIM_NINT			2			// #Ints
#define	SIM_NDBL			6			// #Doubles
#define	SIM_NSGL			6			// #Columns
#define	SIM_FLAG			2			// Simulation flag
#define	SIM_IEMSCT			2			// MODTRAN execution mode
#define	SIM_PRNT			1			// NOPRNT flag
#define	SIM_IATM			2			// Atmosphere modle number
#define	SIM_ISEA			1			// Season modle number
#define	SIM_IVUL			0			// Volcanic modle number
#define	SIM_MULT			1			// Multiple scattering flag
#define	SIM_SALB			53			// Spectral albedo#
#define	SIM_IDAY			-1			// Day of year
#define	SIM_IHAZ			1			// Aerosol model#
#define	SIM_ICLD			0			// Cloud model#
#define	SIM_DBMP			1			// DSR Band model number
#define	SIM_SBMP			1			// SSR Band model number
#define	SIM_CFLG			0			// Correction flag
#define	SIM_DFLG			0			// DISORT flag
#define	SIM_WFLG			0			// H2OAER flag
#define	SIM_NSAM			4			// #Sampling
#define	SIM_IM				0			// CARD2C read flag
#define	SIM_2C				0			// CARD2C flag
#define	SIM_CMIX			365.0			// CO2 mixing ratio in ppmv
#define	SIM_NPAR			37			// #Parameters
#define	SIM_VTAU			0.36777			// Aerosol optical depth
#define	SIM_VMIE			20.0			// Visibility in km
#define	SIM_WSCL			1.0			// Water scale
#define	SIM_OSCL			1.0			// Ozone scale
#define	SIM_GRHO			0.3			// Ground albedo
#define	SIM_H1				0.0			// H1 altitude
#define	SIM_H2				0.0			// H2 altitude
#define	SIM_XMGN			16.0			// X margin
#define	SIM_XSGM			4.0			// X sigma
#define	SIM_WSGM			20.0			// X width
#define	SIM_YUNI			10.0			// Y unit
#define	SIM_WLEN_MIN			550.0			// Min wavelength in nm
#define	SIM_WLEN_MAX			550.0			// Max wavelength in nm
#define	SIM_TH_SUN			20.0			// Solar zenith
#define	SIM_PH_SUN			0.0			// Solar azimuth (N=0,CW)
#define	SIM_TH_LOS			0.0			// LOS zenith
#define	SIM_PH_LOS			0.0			// LOS azimuth (N=0,CW)
#define	SIM_DMIN			1.0e-12			// Min parameter difference
#define	SIM_DMAX			10.0			// Max wavelength difference in nm
#define	SIM_MODE_DSR_MODTRAN_BO		1			// Simulation mode
#define	SIM_MODE_DSR_CUSTOM_BO		2			// Simulation mode
#define	SIM_MODE_SSR_MODTRAN_SO		3			// Simulation mode
#define	SIM_MODE_SSR_MODTRAN_BO		4			// Simulation mode
#define	SIM_MODE_SSR_CUSTOM_SO		5			// Simulation mode
#define	SIM_MODE_SSR_CUSTOM_BO		6			// Simulation mode
#define	SIM_MODTRAN_VTAU		1			// Simulation mode
#define	SIM_MODTRAN_VMIE		2			// Simulation mode
#define	SIM_PATH			"/usr/local/Mod4v2r1"	// Path
#define	SIM_FALB			"DATA/spec_alb.dat"	// Spectral albedo file
// Constants for Profile
#define	PRF_HAZ_NALT			34
#define	PRF_NALT			36
#define	PRF_MAXNALT			1000
#define	PRF_NMOL			12
#define	PRF_N_AMOL			7
#define	PRF_N_TRAC			5
#define	PRF_MOL_H2O			0
#define	PRF_MOL_CO2			1
#define	PRF_MOL_O3			2
#define	PRF_MOL_N2O			3
#define	PRF_MOL_CO			4
#define	PRF_MOL_CH4			5
#define	PRF_MOL_O2			6
#define	PRF_MOL_NO			7
#define	PRF_MOL_SO2			8
#define	PRF_MOL_NO2			9
#define	PRF_MOL_NH3			10
#define	PRF_MOL_HNO3			11
#define	PRF_NHAZ			4
#define	PRF_NHAZ_LOW			2
#define	PRF_NPAR			14
// Constants for Mie calculation
#define	MIE_N_WLEN			11			// #Wavelengths
#define	MIE_MAXDATA			1000			// Max #data
#define	MIE_WLEN_REF			550.0			// Reference wavelength in nm
#define	MIE_WLEN_MIN			250.0			// Min wavelength in nm
#define	MIE_WLEN_MAX			1500.0			// Max wavelength in nm
#define	MIE_TRUE			1			// True  value
#define	MIE_FALSE			0			// False value
#define	MIE_XMAX			2.0e4			// Max size parameter
#define	MIE_RMIN			NAN			// R min in um
#define	MIE_RMAX			NAN			// R max in um
#define	MIE_LMIN			-3.0			// Min Log10(R)
#define	MIE_LMAX			+2.0			// Max Log10(R)
#define	MIE_LSTP			1.0e-3			// Log10(R) step
#define	MIE_WSGM			5.0			// Log10(R) width in sigma
#define	MIE_NSTP			-1			// #Steps
#define	MIE_N_ANGL			121			// #Angles
#define	MIE_MAXCOMP			10			// Max #components
// Constants for Cloud calculation
#define	CLD_THIK			0.0			// Cloud thickness
#define	CLD_ALT				0.0			// Cloud altitude
#define	CLD_EXT				0.0			// Cloud extinction
// Constants for control
#define	CNT_MODE			1
#define	CNT_CONF			NONAME
#define	CNT_MAXCMNT			30
#define	CNT_MAXFORM			30
#define	CNT_MAXNOPT			100			// Max #options
// Functions
#define	MAX(a,b)			((b)>(a)?(b):(a))
#define	INTERP(x,x1,x2,y1,y2)		(y1+(y2-y1)/(x2-x1)*(x-x1))

extern float aerprf_(int*,float*,int*,int*,int*);
extern void miev0_();

int Init(void);
void Finish(void);
int GetOpt(int argn,char **args);
int Usage(void);
int SolarCalc(void);
int MieCalc(void);
int MixComp(void);
int PrfInitMolecule(int imod);
int PrfInitAerosol(int ihaz,int isea,int ivul,double vis);
int PrfGround(void);
int PrfScaleMolecule(void);
int PrfScaleAerosol(double t);
int PrfCalcAOD(void);
int WriteTape5(double *par,int mode);
int WriteCard1(FILE *fp,int mode);
int WriteCard2(FILE *fp,int mode);
int WriteCard2C(FILE *fp);
int WriteCard2D(FILE *fp);
int WriteCard3(FILE *fp,int mode,double *par);
int WriteCard3C(FILE *fp);
int WriteCard4(FILE *fp,int mode,double *par);
int WriteCard5(FILE *fp);
int RunModtran(void);
int ReadPlt_S(const double *par,double *y);
int ReadPlt_B(const double *par,int n,const double *x,double *y);
int Convolute(double xmin,double xmax,double xstp,double xsgm,double wsgm,
              int ninp,const double *xinp,const double *yinp,
              int nout,double *xout,double *yout);
double Gauss(double g,double f0,double f);
double range(double a);
double range2(double a);
int PolToXyz(double th,double ph,double *x,double *y,double *z);
int XyzToPol(double x,double y,double z,double *th,double *ph);
void IjkToXyz(double th,double ph,
              double xi,double yi,double zi,
              double *x,double *y,double *z);
void XyzToIjk(double th,double ph,
              double xi,double yi,double zi,
              double *x,double *y,double *z);
int Interp1D(const double *x1,const double *z1,int nx1,
             const double *x2,      double *z2,int nx2,int f);
int Interp2D(const double *x1,const double *y1,const double *z1,int nx1,int ny1,
             const double *x2,const double *y2,      double *z2,int nx2,int ny2,int f);
int Sampling(int ninp,const double *xinp,const double *yinp,
             int nout,const double *xout,double *yout,double yuni);
int SamplingE(int ninp,const double *xinp,const double *yinp,
              int nout,const double *xout,double *yout,double yuni);
int Init(void);
int PreConfig(void);
int ReadConfig(void);
int PostConfig(void);
int ReadXA(char *s,int size,int cx,double ux,int (*func)(double*),double *x);
int ReadXYA(char *s,int size,int cx,int cy,double ux,double uy,int (*func)(double*),double *x,double *y);
int ReadXYZA(char *s,int size,int cx,int cy,int cz,double ux,double uy,double uz,int (*func)(double*),
             double *x,double *y,double *z);
int ReadXP(char *s,int size,int cx,double ux,int (*func)(double*),double **x);
int ReadXYP(char *s,int size,int cx,int cy,double ux,double uy,int (*func)(double*),double **x,double **y);
int ReadXYZP(char *s,int size,int cx,int cy,int cz,double ux,double uy,double uz,int (*func)(double*),
             double **x,double **y,double **z);
int ReadDC(char *s,int np,int size,const int *c,const double *u,int (*func)(double*),double **d);
int ReadDA(char *s,int np,int size,const int *c,const double *u,double **d);
int ReadNA(char *s,int size,int c,int *d);

// Parameters for Direct Solar radiation
double	dsr_dsim[DSR_MAXWLEN];					// Simulation data
// Parameters for Scattered Solar Radiation
int	ssr_n_wlen			= NODATA;		// #Wavelengths
int	ssr_n_sbmp			= NODATA;		// #Models
int	ssr_sbmp[SSR_MAXWLEN];					// SSR Band model number
double	ssr_wlen[SSR_MAXWLEN];					// Wavelength
double	ssr_dsim[SSR_MAXWLEN];					// Simulation data
int	ssr_n_tdiv			= 10;			// #Divisions in theta
int	ssr_n_pdiv			= 20;			// #Divisions in phi
double	ssr_th_min			= 0.0;			// Theta min in deg
double	ssr_th_max			= 180.0;		// Theta max in deg
// Parameters for Simulation
int	sim_mode			= SIM_MODE_DSR_MODTRAN_BO; // Simulation mode
int	sim_n_angl			= SIM_N_ANGL;		// #Angles
int	sim_flag			= SIM_FLAG;		// Simulation flag
int	sim_iemsct			= SIM_IEMSCT;		// MODTRAN execution mode
int	sim_prnt			= SIM_PRNT;		// NOPRNT flag
int	sim_iatm			= SIM_IATM;		// Atmosphere model number
int	sim_isea			= SIM_ISEA;		// Season model number
int	sim_ivul			= SIM_IVUL;		// Volcanic model number
int	sim_mult			= SIM_MULT;		// Multiple scattering flag
int	sim_salb			= SIM_SALB;		// Spectral albedo
int	sim_iday			= SIM_IDAY;		// Day of year
int	sim_ihaz			= SIM_IHAZ;		// Aerosol model#
int	sim_icld			= SIM_ICLD;		// Cloud model#
int	sim_dbmp			= SIM_DBMP;		// DSR Band model number
int	sim_sbmp			= SIM_SBMP;		// SSR Band model number
int	sim_sbmp_init			= SIM_SBMP;		// SSR Band model number
int	sim_prf_mode			= SIM_MODTRAN_VMIE;	// Simulation mode
int	sim_cflg			= SIM_CFLG;		// Correction flag
int	sim_dflg			= SIM_DFLG;		// DISORT flag
int	sim_wflg			= SIM_WFLG;		// H2OAER flag
int	sim_nsam			= SIM_NSAM;		// #Sampling
int	sim_im				= SIM_IM;		// CARD2C read flag
int	sim_2c				= SIM_2C;		// CARD2C flag
double	sim_cmix			= SIM_CMIX;		// CO2 mixing ratio in ppmv
double	sim_vtau			= SIM_VTAU;		// Aerosol optical depth
double	sim_vmie			= SIM_VMIE;		// Visibility in km
double	sim_wscl			= SIM_WSCL;		// Water scale
double	sim_oscl			= SIM_OSCL;		// Ozone scale
double	sim_grho			= SIM_GRHO;		// Ground albedo
double	sim_h1				= SIM_H1;		// H1 altitude
double	sim_h2				= SIM_H2;		// H2 altitude
double	sim_xmgn			= SIM_XMGN;		// X margin
double	sim_xsgm			= SIM_XSGM;		// X sigma
double	sim_wsgm			= SIM_WSGM;		// X width in sigma
double	sim_yuni			= SIM_YUNI;		// Y unit
double	sim_angl[SIM_N_ANGL] =
{
    0.0,   0.5,   1.0,   1.5,   2.0,   2.5,   3.0,   3.5,
    4.0,   4.5,   5.0,   6.0,   7.0,   8.0,   9.0,  10.0,
   12.0,  14.0,  16.0,  18.0,  20.0,  24.0,  28.0,  32.0,
   36.0,  40.0,  50.0,  60.0,  70.0,  80.0,  90.0,  95.0,
  100.0, 105.0, 110.0, 115.0, 120.0, 125.0, 130.0, 135.0,
  140.0, 145.0, 150.0, 155.0, 160.0, 164.0, 168.0, 172.0,
  176.0, 180.0,
};
double	*sim_phas			= NULL;
double	*sim_tropo_phas			= NULL;
double	*sim_strat_phas			= NULL;
double	*sim_meteo_phas			= NULL;
double	sim_th_sun			= SIM_TH_SUN;		// Solar zenith
double	sim_ph_sun			= SIM_PH_SUN;		// Solar azimuth (N=0,CW)
double	sim_th_los			= SIM_TH_LOS;		// LOS zenith
double	sim_ph_los			= SIM_PH_LOS;		// LOS azimuth (N=0,CW)
int	sim_prf_pscl[PRF_NMOL];
double	sim_prf_pres[PRF_MAXNALT];
double	sim_prf_temp[PRF_MAXNALT];
double	sim_prf_wmol[PRF_NMOL][PRF_MAXNALT];
double	sim_prf_haze[PRF_NHAZ][PRF_MAXNALT];
double	sim_prf_vscl[PRF_NMOL];
char	sim_prf_jchar[PRF_NPAR];
char	sim_prf_jcharx;
char	sim_path[MAXLINE]		= SIM_PATH;
char	sim_falb[MAXLINE]		= SIM_FALB;		// Spectral albedo file
// Parameters for Profile
int	prf_n_alt			= PRF_NALT;
double	prf_alt[PRF_MAXNALT]		=
{
   0.0,  1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,
   8.0,  9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0,
  16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0,
  24.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0,
  60.0, 70.0, 80.0, 100.0,
};
double	prf_haz_alt[PRF_HAZ_NALT]	=
{
    0.0,  1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,
    8.0,  9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0,
   16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0,
   24.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 70.0,
  100.0, 99999.0,
};
double	prf_pres[PRF_MAXNALT];
double	prf_temp[PRF_MAXNALT];
double	prf_wmol[PRF_NMOL][PRF_MAXNALT];
double	prf_haze[PRF_NHAZ][PRF_MAXNALT];
double	prf_pres_gnd			= NAN;
double	prf_temp_gnd			= NAN;
double	prf_aod				= 0.0;
double	prf_aod_lo			= 0.0;
double	prf_aod_hi			= 0.0;
char	prf_jchar[PRF_NPAR];
char	prf_wmol_nam[PRF_NMOL][MAXLINE]	=
{
  "H2O", "CO2", "O3", "N2O", "CO", "CH4",
  "O2", "NO", "SO2", "NO2", "NH3", "HNO3",
};
// Parameters for Mie calculation
int	mie_iref			= -1;			// Ref wave line#
int	mie_n_wlen			= MIE_N_WLEN;		// #Wavelengths (output)
int	mie_nstp			= MIE_NSTP;		// #Steps
double	mie_wlen[MIE_MAXDATA]		=
{
  300.0, 337.1, 400.0,  488.0,  514.5, 550.0,
  632.8, 694.3, 860.0, 1060.0, 1300.0,
};
int	mie_n_comp			= 0;			// #Components
double	mie_rmin			= MIE_RMIN;		// R min in um
double	mie_rmax			= MIE_RMAX;		// R max in um
double	mie_lstp			= MIE_LSTP;		// Log10(R) step
double	mie_wsgm			= MIE_WSGM;		// Log10(R) width in sigma
double	mie_lmin;
double	mie_lmax;
double	mie_lmod_com[MIE_MAXCOMP];
double	*mie_wlen_um			= NULL;
double	*mie_aext			= NULL;
double	*mie_asca			= NULL;
double	*mie_asym			= NULL;
int	mie_n_angl			= MIE_N_ANGL;		// #Angles
double	mie_angl[MIE_MAXDATA]		=
{
    0.0,   0.5,   1.0,   1.5,   2.0,   2.5,   3.0,   3.5,   4.0,   4.5,   5.0,
    5.5,   6.0,   6.5,   7.0,   7.5,   8.0,   8.5,   9.0,   9.5,  10.0,  12.0,
   14.0,  16.0,  18.0,  20.0,  22.0,  24.0,  26.0,  28.0,  30.0,  32.0,  34.0,
   36.0,  38.0,  40.0,  42.0,  44.0,  46.0,  48.0,  50.0,  52.0,  54.0,  56.0,
   58.0,  60.0,  62.0,  64.0,  66.0,  68.0,  70.0,  72.0,  74.0,  76.0,  78.0,
   80.0,  82.0,  84.0,  86.0,  88.0,  90.0,  92.0,  94.0,  96.0,  98.0, 100.0,
  102.0, 104.0, 106.0, 108.0, 110.0, 112.0, 114.0, 116.0, 118.0, 120.0, 122.0,
  124.0, 126.0, 128.0, 130.0, 132.0, 134.0, 136.0, 138.0, 140.0, 142.0, 144.0,
  146.0, 148.0, 150.0, 152.0, 154.0, 156.0, 158.0, 160.0, 162.0, 164.0, 166.0,
  168.0, 170.0, 170.5, 171.0, 171.5, 172.0, 172.5, 173.0, 173.5, 174.0, 174.5,
  175.0, 175.5, 176.0, 176.5, 177.0, 177.5, 178.0, 178.5, 179.0, 179.5, 180.0,
};
double	*mie_angl_rad			= NULL;
double	*mie_angl_sin			= NULL;
double	*mie_angl_cos			= NULL;
double	*mie_angl_dif			= NULL;
double	*mie_phs1			= NULL;
double	*mie_phs2			= NULL;
double	*mie_phas			= NULL;
double	*mie_refr_com[MIE_MAXCOMP];				// Refractive index (real)
double	*mie_refi_com[MIE_MAXCOMP];				// Refractive index (imaginary)
double	*mie_aext_com[MIE_MAXCOMP];				// Extinction coefficient
double	*mie_asca_com[MIE_MAXCOMP];				// Scattering coefficient
double	*mie_asym_com[MIE_MAXCOMP];				// Asymmetry parameter
double	*mie_phs1_com[MIE_MAXCOMP];				// Phase function 1
double	*mie_phs2_com[MIE_MAXCOMP];				// Phase function 2
double	*mie_phas_com[MIE_MAXCOMP];				// Phase function 3
double	mie_wcom_com[MIE_MAXCOMP];				// Weight
double	mie_rmod_com[MIE_MAXCOMP];				// Mode radius in um
double	mie_lsgm_com[MIE_MAXCOMP];				// Log10(sigma in um)
// Parameters for Cloud calculation
double	cld_thik			= CLD_THIK;		// Cloud thickness
double	cld_alt				= CLD_ALT;		// Cloud altitude
double	cld_ext				= CLD_EXT;		// Cloud extinction
// Parameters for control
int	cnt_mode			= CNT_MODE;		// Control mode
int	cnt_xmod			= 0;			// Mixing  mode
int	cnt_cflg			= 0;			// Clear   flag
int	cnt_db				= 0;			// Debug   mode
int	cnt_vb				= 0;			// Verbose mode
int	cnt_hp				= 0;			// Help    mode
int	cnt_n_cmnt			= NODATA;
int	cnt_n_own_format		= NODATA;
int	cnt_optn			= 1;
char	cnt_conf[MAXLINE]		= CNT_CONF;		// Configuration file
char	cnt_cmnt[CNT_MAXCMNT][MAXLINE];				// Comments
char	cnt_own_format[CNT_MAXFORM][MAXLINE];			// Formats
char	*cnt_opts[CNT_MAXNOPT];

int main(int argc,char **argv)
{
  if(GetOpt(argc,argv) < 0) return -1;
  if(Init() < 0) return -1;
  if(cnt_hp) {Usage(); return 0;}
  if(SolarCalc() < 0) return -1;

  return 0;
}

int SolarCalc(void)
{
  int j;
  int ith,iph;
  int err;
  double x,y,z;
  double th,ph;
  double xi,yi,zi;
  double thi,phi;
  double th0,th1,th2;
  double dth,dph,ds;
  double p[6];
  char fnam[MAXLINE] = "SolarCalc";

  // Calculate solar irradiance
  p[0] = ssr_wlen[0]-sim_xmgn;
  p[1] = ssr_wlen[ssr_n_wlen-1]+sim_xmgn;
  err = 0;
  do
  {
    switch(cnt_mode)
    {
      case 1:
      case 2:
        if(WriteTape5(p,SIM_MODE_DSR_MODTRAN_BO) < 0) err = 1;
        break;
      case 3:
      case 4:
        if(WriteTape5(p,SIM_MODE_DSR_CUSTOM_BO) < 0) err = 1;
        break;
      default:
        fprintf(stderr,"%s: invalid mode >>> %d\n",fnam,sim_mode);
        err = 1;
        break;
    }
    if(err) break;
    if(RunModtran() < 0)
    {
      err = 1;
      break;
    }
    if(ReadPlt_B(p,ssr_n_wlen,ssr_wlen,dsr_dsim) < 0)
    {
      err = 1;
      break;
    }
  }
  while(0);
  if(err)
  {
    return -1;
  }

  // Calculate sky radiance
  sim_sbmp = SSR_SBMP;
  th0 = ssr_th_min*D_TO_R;
  dth = (ssr_th_max-ssr_th_min)*D_TO_R/ssr_n_tdiv;
  dph = PI2/ssr_n_pdiv;
  p[2] = sim_th_sun;
  p[3] = sim_ph_sun;
  for(j=0; j<ssr_n_wlen; j++)
  {
    if(cnt_mode==1 || cnt_mode==3)
    {
      p[0] = ssr_wlen[j]-sim_xmgn;
      p[1] = ssr_wlen[j]+sim_xmgn;
    }
    else
    {
      p[0] = ssr_wlen[j];
      p[1] = (double)sim_nsam;
    }
    ssr_dsim[j] = 0.0;
    for(ith=0; ith<ssr_n_tdiv; ith++)
    {
      th1 = th0+dth*ith;
      th2 = th0+dth*(ith+1);
      thi = 0.5*(th1+th2);
      ds = (cos(th1)-cos(th2))*dph;
      for(iph=0; iph<ssr_n_pdiv; iph++)
      {
        phi = dph*((double)iph+0.5);
        PolToXyz(thi,phi,&xi,&yi,&zi);
        IjkToXyz(sim_th_sun*D_TO_R,sim_ph_sun*D_TO_R,xi,yi,zi,&x,&y,&z);
        XyzToPol(x,y,z,&th,&ph);
        p[4] = th*R_TO_D;
        p[5] = ph*R_TO_D;
        err = 0;
        do
        {
          switch(cnt_mode)
          {
            case 1:
              if(WriteTape5(p,SIM_MODE_SSR_MODTRAN_BO) < 0) err = 1;
              break;
            case 2:
              if(WriteTape5(p,SIM_MODE_SSR_MODTRAN_SO) < 0) err = 1;
              break;
            case 3:
              if(WriteTape5(p,SIM_MODE_SSR_CUSTOM_BO) < 0) err = 1;
              break;
            case 4:
              if(WriteTape5(p,SIM_MODE_SSR_CUSTOM_SO) < 0) err = 1;
              break;
            default:
              fprintf(stderr,"%s: invalid mode >>> %d\n",fnam,cnt_mode);
              err = 1;
              break;
          }
          if(err) break;
          if(RunModtran() < 0)
          {
            err = 1;
            break;
          }
          if(cnt_mode==1 || cnt_mode==3)
          {
            if(ReadPlt_B(p,1,&ssr_wlen[j],&y) < 0)
            {
              err = 1;
              break;
            }
          }
          else
          {
            if(ReadPlt_S(p,&y) < 0)
            {
              err = 1;
              break;
            }
          }
        }
        while(0);
        if(err)
        {
          return -1;
        }
        if(cnt_vb > 1)
        {
          fprintf(stderr,"%9.4f %9.4f %9.4f %9.4f %9.4f %9.4f %9.4f %22.14e\n",
                          p[2],p[3],ssr_wlen[j],
                          thi*R_TO_D,phi*R_TO_D,p[4],p[5],y);
        }
        ssr_dsim[j] += y*ds;
      }
    }
  }

  if(cnt_vb)
  {
    for(j=0; j<ssr_n_wlen; j++)
    {
      fprintf(stderr,"%9.4f %22.14e %22.14e\n",ssr_wlen[j],dsr_dsim[j],ssr_dsim[j]);
    }
  }

  return 0;
}

int Init(void)
{
  if(ReadConfig() < 0) return -1;
  if(sim_mode==2 || sim_mode==3)
  {
    if(MieCalc() < 0)
    {
      return -1;
    }
    if(MixComp() < 0)
    {
      return -1;
    }
  }

  return 0;
}

void Finish(void)
{
  int err = 0;

  if(cnt_cflg > 0)
  {
    err += system("rm -f modtran.7sc");
    err += system("rm -f modtran.7sr");
    err += system("rm -f modtran.mc");
    err += system("rm -f modtran.tp6");
    err += system("rm -f modtran.tp7");
    err += system("rm -f modtran.tp8");
  }
  if(cnt_cflg > 1)
  {
    err += system("rm -f DATA");
    err += system("rm -f modroot.in");
    err += system("rm -f modtran.tp5");
  }
  if(cnt_cflg > 2)
  {
    err += system("rm -f modtran.plt");
  }
  if(err != 0)
  {
    fprintf(stderr,"Warning, failed to remove files.\n");
  }

  return;
}

int MieCalc(void)
{
  int i,j,k,m,n;
  int n_step = 0;
  int err = 0;
  int ANYANG,PERFCT,PRNT[2];
  int IPOLZN,MOMDIM,NUMANG,NMOM;
  double x,y,w,r,l;
  double sy,p1,p2,p3;
  double s1,s2,s3;
  double norm,slop;
  double epsilon = 1.0e-1;
  double GQSC,MIMCUT,PMOM[2][2],QEXT,QSCA,SPIKE;
  double _Complex CREFIN,SFORW,SBACK,*S1,*S2,TFORW[2],TBACK[2];

  if(!isnan(mie_rmin))
  {
    mie_lmin = log10(mie_rmin);
    if(mie_lmin < MIE_LMIN)
    {
      mie_lmin = MIE_LMIN;
    }
  }
  if(!isnan(mie_rmax))
  {
    mie_lmax = log10(mie_rmax);
    if(mie_lmax > MIE_LMAX)
    {
      mie_lmax = MIE_LMAX;
    }
  }
  if(mie_nstp > 0)
  {
    n_step = mie_nstp;
  }
  if(!isnan(mie_rmin) && !isnan(mie_rmax))
  {
    if(mie_nstp > 0)
    {
      mie_lstp = (mie_lmax-mie_lmin)/mie_nstp;
    }
    else
    {
      n_step = (int)((mie_lmax-mie_lmin)/mie_lstp+0.5);
      if(n_step < 1)
      {
        fprintf(stderr,"MieCalc: error, invalid #steps (lmin=%13.4e lmax=%13.4e lstp=%13.4e n_step=%d\n",
                        mie_lmin,mie_lmax,mie_lstp,n_step);
        return -1;
      }
    }
  }

  for(n=0; n<mie_n_comp; n++)
  {
    mie_lmod_com[n] = log10(mie_rmod_com[n]);
  }

  // initialization
  S1 = (double _Complex *)malloc(mie_n_angl*sizeof(double _Complex));
  S2 = (double _Complex *)malloc(mie_n_angl*sizeof(double _Complex));
  if(S1==NULL || S2==NULL)
  {
    fprintf(stderr,"MieCalc: failed in allocating memory\n");
    return -1;
  }
  PERFCT = MIE_FALSE;
  MIMCUT = DELTA;
  ANYANG = MIE_FALSE;
  NUMANG = mie_n_angl;
  PRNT[0] = (cnt_db>1?MIE_TRUE:MIE_FALSE);
  PRNT[1] = (cnt_db>0?MIE_TRUE:MIE_FALSE);
  NMOM = 0;
  IPOLZN = 0;
  MOMDIM = 1;
  for(n=0; n<mie_n_comp; n++)
  {
    for(i=0; i<mie_n_wlen; i++)
    {
      mie_aext_com[n][i] = 0.0;
      mie_asca_com[n][i] = 0.0;
      mie_asym_com[n][i] = 0.0;
      for(j=0; j<mie_n_angl; j++)
      {
        k = mie_n_angl*i+j;
        mie_phs1_com[n][k] = 0.0;
        mie_phs2_com[n][k] = 0.0;
        mie_phas_com[n][k] = 0.0;
      }
    }
  }

  // mie calculation
  for(n=0; n<mie_n_comp; n++)
  {
    if(isnan(mie_rmin) || isnan(mie_rmax))
    {
      if(isnan(mie_rmin))
      {
        mie_lmin = mie_lmod_com[n]-mie_lsgm_com[n]*mie_wsgm;
        if(mie_lmin < MIE_LMIN)
        {
          mie_lmin = MIE_LMIN;
        }
      }
      if(isnan(mie_rmax))
      {
        mie_lmax = mie_lmod_com[n]+mie_lsgm_com[n]*mie_wsgm;
        if(mie_lmax > MIE_LMAX)
        {
          mie_lmax = MIE_LMAX;
        }
      }
      if(mie_nstp > 0)
      {
        mie_lstp = (mie_lmax-mie_lmin)/mie_nstp;
      }
      else
      {
        n_step = (int)((mie_lmax-mie_lmin)/mie_lstp+0.5);
        if(n_step < 1)
        {
          fprintf(stderr,"Error, invalid #steps (lmin=%13.4e lmax=%13.4e lstp=%13.4e n_step=%d\n",
                          mie_lmin,mie_lmax,mie_lstp,n_step);
          err = 1;
          break;
        }
      }
    }
    norm = mie_lstp/(sqrt(PI2)*mie_lsgm_com[n]);
    slop = -0.5/(mie_lsgm_com[n]*mie_lsgm_com[n]);
    if(cnt_vb > 1)
    {
      fprintf(stderr,"Component#   : %2d\n",n);
      fprintf(stderr,"Min Log10(R) : %13.4e\n",mie_lmin);
      fprintf(stderr,"Max Log10(R) : %13.4e\n",mie_lmax);
      fprintf(stderr,"#Steps       : %13d\n",n_step);
      // calculate averages
      s1 = s2 = s3 = 0.0;
      for(m=0,sy=0.0; m<=n_step; m++)
      {
        l = mie_lmin+mie_lstp*m;
        r = pow(10.0,l);
        y = norm*exp(slop*(l-mie_lmod_com[n])*(l-mie_lmod_com[n]));
        w = y*r;
        s1 += w; w *= r;
        s2 += w; w *= r;
        s3 += w;
        sy += y;
      }
      if(fabs(sy-1.0) > epsilon)
      {
        fprintf(stderr,"Warning, sy=%13.4e\n",sy);
      }
      s1 /= sy;
      s2 /= sy;
      s3 /= sy;
      fprintf(stderr,"Average R    : %13.6e\n",s1);
      fprintf(stderr,"Average R^2  : %13.6e\n",s2);
      fprintf(stderr,"Average R^3  : %13.6e\n",s3);
    }
    for(i=0; i<mie_n_wlen; i++)
    {
      CREFIN = mie_refr_com[n][i]-fabs(mie_refi_com[n][i])*I;
      for(m=0,sy=0.0; m<=n_step; m++)
      {
        l = mie_lmin+mie_lstp*m;
        r = pow(10.0,l);
        x = PI2*r/(mie_wlen_um[i]);
        y = norm*exp(slop*(l-mie_lmod_com[n])*(l-mie_lmod_com[n]));
        w = y*PI*r*r*1.0e-3;
        if(x >= MIE_XMAX) break;
        miev0_(&x,&CREFIN,&PERFCT,&MIMCUT,&ANYANG,&NUMANG,mie_angl_cos,
               &NMOM,&IPOLZN,&MOMDIM,PRNT,&QEXT,&QSCA,&GQSC,
               PMOM,&SFORW,&SBACK,S1,S2,TFORW,TBACK,&SPIKE);
        mie_aext_com[n][i] += QEXT*w;
        mie_asca_com[n][i] += QSCA*w;
        for(j=0; j<mie_n_angl; j++)
        {
          k = mie_n_angl*i+j;
          s1 = cabs(S1[j]);
          s2 = cabs(S2[j]);
          mie_phs1_com[n][k] += s1*s1*y;
          mie_phs2_com[n][k] += s2*s2*y;
        }
        sy += y;
      }
      if(fabs(sy-1.0) > epsilon)
      {
        fprintf(stderr,"Warning, sy=%13.4e\n",sy);
      }
      mie_aext_com[n][i] /= sy;
      mie_asca_com[n][i] /= sy;
      for(j=0; j<mie_n_angl; j++)
      {
        k = mie_n_angl*i+j;
        mie_phs1_com[n][k] /= sy;
        mie_phs2_com[n][k] /= sy;
        mie_phas_com[n][k] = 0.5*(mie_phs1_com[n][k]+mie_phs2_com[n][k]);
      }
      p1 = p2 = p3 = 0.0;
      for(j=1; j<mie_n_angl; j++)
      {
        k = mie_n_angl*i+j;
        p1 += (mie_phs1_com[n][k-1]*mie_angl_sin[j-1]+mie_phs1_com[n][k]*mie_angl_sin[j])*mie_angl_dif[j];
        p2 += (mie_phs2_com[n][k-1]*mie_angl_sin[j-1]+mie_phs2_com[n][k]*mie_angl_sin[j])*mie_angl_dif[j];
        p3 += (mie_phas_com[n][k-1]*mie_angl_sin[j-1]+mie_phas_com[n][k]*mie_angl_sin[j])*mie_angl_dif[j];
      }
      for(j=0; j<mie_n_angl; j++)
      {
        k = mie_n_angl*i+j;
        mie_phs1_com[n][k] /= p1;
        mie_phs2_com[n][k] /= p2;
        mie_phas_com[n][k] /= p3;
      }
      for(j=1; j<mie_n_angl; j++)
      {
        k = mie_n_angl*i+j;
        mie_asym_com[n][i] += (mie_phas_com[n][k-1]*mie_angl_sin[j-1]*mie_angl_cos[j-1]+
                               mie_phas_com[n][k]*mie_angl_sin[j]*mie_angl_cos[j])*mie_angl_dif[j];
      }
    }
    if(err) break;
  }

  free(S1);
  free(S2);
  if(err)
  {
    return -1;
  }

  return 0;
}

int MixComp(void)
{
  int i,j,k,n;
  double r,ws;
  double sw,se,ss,sa;
  double s1,s2,s3;
  double wcom[MIE_MAXCOMP];

  // calculate number mixing ratio
  if(cnt_xmod == 1)
  {
    for(r=0.0,n=0; n<mie_n_comp; n++)
    {
      wcom[n] = mie_wcom_com[n]/mie_aext_com[n][mie_iref];
      r += wcom[n];
    }
    for(n=0; n<mie_n_comp; n++)
    {
      wcom[n] /= r;
      if(cnt_vb > 1)
      {
        message(stderr,"Component#   : %2d\n",n);
        message(stderr,"Mixing ratio : %13.4e\n",wcom[n]);
      }
    }
  }
  else
  {
    for(n=0; n<mie_n_comp; n++)
    {
      wcom[n] = mie_wcom_com[n];
    }
  }

  for(i=0; i<mie_n_wlen; i++)
  {
    sw = se = ss = sa = 0.0;
    for(n=0; n<mie_n_comp; n++)
    {
      sw += wcom[n];
      se += wcom[n]*mie_aext_com[n][i];
      ws  = wcom[n]*mie_asca_com[n][i];
      ss += ws;
      sa += ws*mie_asym_com[n][i];
    }
    mie_aext[i] = se/sw;
    mie_asca[i] = ss/sw;
    mie_asym[i] = sa/ss;
    for(j=0; j<mie_n_angl; j++)
    {
      k = mie_n_angl*i+j;
      s1 = s2 = s3 = 0.0;
      for(n=0; n<mie_n_comp; n++)
      {
        ws  = wcom[n]*mie_asca_com[n][i];
        s1 += ws*mie_phs1_com[n][k];
        s2 += ws*mie_phs2_com[n][k];
        s3 += ws*mie_phas_com[n][k];
      }
      mie_phs1[k] = s1/ss;
      mie_phs2[k] = s2/ss;
      mie_phas[k] = s3/ss;
    }
  }

  for(i=0; i<mie_n_wlen; i++)
  {
    s3 = 0.0;
    for(j=1; j<mie_n_angl; j++)
    {
      k = mie_n_angl*i+j;
      s3 += (mie_phas[k-1]*mie_angl_sin[j-1]+mie_phas[k]*mie_angl_sin[j])*mie_angl_dif[j];
    }
    for(j=0; j<mie_n_angl; j++)
    {
      k = mie_n_angl*i+j;
      mie_phas[k] /= s3;
    }
  }

  if(Interp2D(mie_wlen,mie_angl,mie_phas,mie_n_wlen,mie_n_angl,
              mie_wlen,sim_angl,sim_phas,mie_n_wlen,sim_n_angl,0) < 0)
  {
    return -1;
  }

  return 0;
}

int PrfInitMolecule(int imod)
{
  int i,j;
  int nmod;

  if(imod<=0 || imod>PRF_DEF_NMOD)
  {
    fprintf(stderr,"PrfInitMolecule: invalid model number >>> %d\n",imod);
    return -1;
  }
  else
  {
    nmod = imod-1;
  }

  if(Sampling(PRF_DEF_NALT,prf_def_alt,prf_def_pres[nmod],prf_n_alt,prf_alt,prf_pres,1.0) < 0) return -1;
  if(Sampling(PRF_DEF_NALT,prf_def_alt,prf_def_temp[nmod],prf_n_alt,prf_alt,prf_temp,1.0) < 0) return -1;
  for(i=0; i<PRF_N_AMOL; i++)
  {
    if(Sampling(PRF_DEF_NALT,prf_def_alt,prf_def_amol[i][nmod],prf_n_alt,prf_alt,prf_wmol[i],1.0) < 0) return -1;
  }
  for(j=0; j<PRF_N_TRAC; i++,j++)
  {
    if(Sampling(PRF_DEF_NALT,prf_def_alt,prf_def_trac[j],prf_n_alt,prf_alt,prf_wmol[i],1.0) < 0) return -1;
  }

  for(j=0; j<prf_n_alt; j++)
  {
    sim_prf_pres[j] = 0.0;
    sim_prf_temp[j] = 0.0;
  }
  for(i=0; i<PRF_NMOL; i++)
  {
    sim_prf_pscl[i] = 0;
    sim_prf_vscl[i] = NAN;
    for(j=0; j<prf_n_alt; j++)
    {
      sim_prf_wmol[i][j] = 0.0;
    }
  }

  for(i=0; i<PRF_NPAR; i++)
  {
    prf_jchar[i] = 'A';
    sim_prf_jchar[i] = (char)('0'+imod);
  }
  sim_prf_jcharx = (char)('0'+imod);

  sim_2c = 0;

  return 0;
}

int PrfInitAerosol(int ihaz,int isea,int ivul,double vis)
{
  int i,j;
  float v;
  double p[PRF_HAZ_NALT];
  double q[PRF_MAXNALT];

  v = (float)vis;
  for(j=0; j<PRF_HAZ_NALT; j++)
  {
    i = j+1;
    p[j] = (double)aerprf_(&i,&v,&ihaz,&isea,&ivul);
  }
  if(SamplingE(PRF_HAZ_NALT,prf_haz_alt,p,prf_n_alt,prf_alt,q,1.0) < 0) return -1;
  for(i=0; i<PRF_NHAZ; i++)
  {
    for(j=0; j<prf_n_alt; j++)
    {
      prf_haze[i][j] = 0.0;
    }
  }
  if(ihaz != 0)
  {
    for(j=0; j<prf_n_alt; j++)
    {
      if(prf_alt[j] < 2.0+EPSILON)
      {
        prf_haze[0][j] = q[j];
      } else
      if(prf_alt[j] < 10.0+EPSILON)
      {
        prf_haze[1][j] = q[j];
      } else
      if(prf_alt[j] < 30.0+EPSILON)
      {
        prf_haze[2][j] = q[j];
      }
      else
      {
        prf_haze[3][j] = q[j];
      }
    }
  }

  for(i=0; i<PRF_NHAZ; i++)
  {
    for(j=0; j<prf_n_alt; j++)
    {
      sim_prf_haze[i][j] = prf_haze[i][j];
    }
  }

  return 0;
}

int PrfGround(void)
{
  int i,j;
  double f;

  if(!isnan(prf_pres_gnd))
  {
    f = prf_pres_gnd/prf_pres[0];
    for(j=0; j<prf_n_alt; j++)
    {
      sim_prf_pres[j] = prf_pres[j]*f;
    }
    sim_prf_jchar[0] = prf_jchar[0];
    for(i=0; i<PRF_NMOL; i++)
    {
      if(sim_prf_pscl[i] != 0)
      {
        for(j=0; j<prf_n_alt; j++)
        {
          sim_prf_wmol[i][j] = prf_wmol[i][j]*f;
        }
        sim_prf_jchar[i+2] = prf_jchar[i+2];
        if(i > 2) sim_2c |= 0x0010;
      }
    }
  }

  if(!isnan(prf_temp_gnd))
  {
    f = prf_temp_gnd/prf_temp[0];
    for(j=0; j<prf_n_alt; j++)
    {
      sim_prf_temp[j] = prf_temp[j]*f;
    }
    sim_prf_jchar[1] = prf_jchar[1];
  }

  sim_2c |= 0x0001;

  return 0;
}

int PrfScaleMolecule(void)
{
  int i,j;

  for(i=0; i<PRF_NMOL; i++)
  {
    if(!isnan(sim_prf_vscl[i]))
    {
      for(j=0; j<prf_n_alt; j++)
      {
        sim_prf_wmol[i][j] = prf_wmol[i][j]*sim_prf_vscl[i];
      }
      sim_prf_jchar[i+2] = prf_jchar[i+2];
      if(i > 2) sim_2c |= 0x0010;
    }
  }

  sim_2c |= 0x0001;

  return 0;
}

int PrfScaleAerosol(double t)
{
  int i,j;
  double f;

  f = (t-prf_aod_hi)/prf_aod_lo;
  for(i=0; i<PRF_NHAZ_LOW; i++)
  {
    for(j=0; j<prf_n_alt; j++)
    {
      sim_prf_haze[i][j] = prf_haze[i][j]*f;
    }
  }

  sim_2c |= 0x1000;

  return 0;
}

int PrfCalcAOD(void)
{
  int i,j;
  double a1,a2;
  double dz,dt;
  double r;
  double t[PRF_NHAZ];

  for(i=0; i<PRF_NHAZ; i++)
  {
    t[i] = 0.0;
    for(j=0; j<prf_n_alt-1; j++)
    {
      a1 = prf_haze[i][j];
      a2 = prf_haze[i][j+1];
      dz = prf_alt[j+1]-prf_alt[j];
      if(a1>0.0 && a2>0.0 && (fabs(a1-a2)>a2*EPSILON))
      {
        r = dz/log(a1/a2);
        if(dz/r < EPSILON)
        {
          r = 0.0;
        }
      }
      else
      {
        r = 0.0;
      }
      if(r > 0.0)
      {
        dt = a1*r*(1.0-exp(-dz/r));
      }
      else
      {
        dt = 0.5*(a1+a2)*dz;
      }
      t[i] += dt;
    }
  }

  prf_aod_lo = 0.0;
  for(i=0; i<PRF_NHAZ_LOW; i++)
  {
    prf_aod_lo += t[i];
  }
  prf_aod_hi = 0.0;
  for(i=PRF_NHAZ_LOW; i<PRF_NHAZ; i++)
  {
    prf_aod_hi += t[i];
  }
  prf_aod = prf_aod_lo+prf_aod_hi;

  return 0;
}

int WriteTape5(double *par,int mode)
{
  int err;
  FILE *fp;
  char fnam[] = "WriteTape5";

  if((fp=fopen("modtran.tp5","w")) == NULL)
  {
    fprintf(stderr,"Error, cannot open %s\n","modtran.tp5");
    return -1;
  }

  sim_im = (sim_2c>0?1:0);
  err = 0;
  switch(mode)
  {
    case SIM_MODE_DSR_MODTRAN_BO: // DSR simulation with MODTRAN aerosol model
    case SIM_MODE_DSR_CUSTOM_BO:  // DSR simulation with custom  aerosol model
    case SIM_MODE_SSR_MODTRAN_SO: // SSR simulation with MODTRAN aerosol model
    case SIM_MODE_SSR_MODTRAN_BO: // SSR simulation with MODTRAN aerosol model
    case SIM_MODE_SSR_CUSTOM_SO:  // SSR simulation with custom  aerosol model
    case SIM_MODE_SSR_CUSTOM_BO:  // SSR simulation with custom  aerosol model
      // CARD 1
      if(WriteCard1(fp,mode) < 0)
      {
        err = 1;
        break;
      }
      // CARD 2
      if(WriteCard2(fp,mode) < 0)
      {
        err = 1;
        break;
      }
      // CARD 3
      if(WriteCard3(fp,mode,par) < 0)
      {
        err = 1;
        break;
      }
      // CARD 4
      if(WriteCard4(fp,mode,par) < 0)
      {
        err = 1;
        break;
      }
      // CARD 5
      if(WriteCard5(fp) < 0)
      {
        err = 1;
        break;
      }
      break;
    default:
      fprintf(stderr,"%s: invalid mode >>> %d\n",fnam,mode);
      err = 1;
      break;
  }
  fclose(fp);
  if(err)
  {
    return -1;
  }

  return 0;
}

int WriteCard1(FILE *fp,int mode)
{
  char fnam[] = "WriteCard1";

  if(mode==SIM_MODE_DSR_MODTRAN_BO || mode==SIM_MODE_DSR_CUSTOM_BO)
  {
    sim_iemsct = 3;
  } else
  if(mode==SIM_MODE_SSR_MODTRAN_SO || mode==SIM_MODE_SSR_MODTRAN_BO ||
     mode==SIM_MODE_SSR_CUSTOM_SO  || mode==SIM_MODE_SSR_CUSTOM_BO)
  {
    if(sim_th_sun > 100.0)
    {
      sim_iemsct = 1;
    }
    else
    {
      sim_iemsct = 2;
    }
  }
  else
  {
    fprintf(stderr,"%s: invalid mode >>> %d\n",fnam,mode);
    return -1;
  }

  // CARD 1
  if(sim_salb > 0)
  {
    fprintf(fp,"MM%3d    3   %2d%5d    0    0    0    0    0    0    0%5d%5d    .000 LAMBER\n",
                (sim_2c>0?7:sim_iatm),sim_iemsct,sim_mult,sim_im,sim_prnt);
  }
  else
  {
    fprintf(fp,"MM%3d    3   %2d%5d    0    0    0    0    0    0    0%5d%5d    .000%7.4f\n",
                (sim_2c>0?7:sim_iatm),sim_iemsct,sim_mult,sim_im,sim_prnt,sim_grho);
  }
  if(sim_iemsct == 3)
  {
    // CARD 1A
    cprintf(fp,"tt  8f   0%10.3f%10.0f%10.0f f t f %s\n",
                sim_cmix,sim_wscl,sim_oscl,sim_wflg==1?"t":"f");
    // CARD 1A2
    fprintf(fp,"DATA/B2001_%02d.BIN\n",sim_dbmp);
  }
  else
  {
    // CARD 1A
    cprintf(fp,"%st 16f   0%10.3f%10.0f%10.0f f t f %s\n",sim_dflg==0?"f":"t",
                sim_cmix,sim_wscl,sim_oscl,sim_wflg==1?"t":"f");
    // CARD 1A2
    fprintf(fp,"DATA/B2001_%02d.BIN\n",sim_sbmp);
  }

  return 0;
}

int WriteCard2(FILE *fp,int mode)
{
  // CARD 2
  cprintf(fp,"  %3d    0   %2d    0   %2d    0%10.5f      .000      .000      .000      .000\n",sim_ihaz,sim_ivul,sim_icld,sim_vmie);
  if(sim_icld > 0)
  {
    // CARD 2A
    if(sim_icld==18 || sim_icld==19)
    {
      cprintf(fp,"%8.3f%8.3f%8.3f\n",cld_thik,cld_alt,cld_ext);
    } else
    if(sim_icld>0 && sim_icld<11)
    {
      cprintf(fp,"%8.3f%8.3f%8.3f%4d%4d%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f\n",
                  -9.0,-9.0,-9.0,-9,-9,-9.0,-9.0,-9.0,-9.0,-9.0,-9.0);
    }
  }
  if(sim_2c>0 && sim_im>0)
  {
    // CARD 2C
    WriteCard2C(fp);
  }
  if(mode==SIM_MODE_DSR_CUSTOM_BO || mode==SIM_MODE_SSR_CUSTOM_SO || mode==SIM_MODE_SSR_CUSTOM_BO)
  {
    // CARD 2D
    WriteCard2D(fp);
  }

  return 0;
}

int WriteCard2C(FILE *fp)
{
  int i;
  int f2,f3;
  char jchar[15] = "";
  char jcharx[2] = " ";

  if(sim_2c <= 0)
  {
    return 0;
  }
  else
  {
    f2 = (sim_2c&0x0010);
    f3 = (sim_2c&0x1000);
  }

  for(i=0; i<PRF_NPAR; i++)
  {
    jchar[i] = sim_prf_jchar[i];
  }
  jchar[i] = '\0';
  jcharx[0] = sim_prf_jcharx;

  // CARD 2C
  fprintf(fp,"%5d%5d%5d%20s%10.0f\n",prf_n_alt,(f2>0?1:0),(f3>0?2:0),"Custom",0.0);
  for(i=0; i<prf_n_alt; i++)
  {
    // CARD 2C1
    fprintf(fp,"%10.3f%10.3e%10.3e%10.3e%10.3e%10.3e%14s %1s\n",
                prf_alt[i],sim_prf_pres[i],sim_prf_temp[i],
                sim_prf_wmol[0][i],sim_prf_wmol[1][i],sim_prf_wmol[2][i],jchar,jcharx);
    if(f2 > 0)
    {
      // CARD 2C2
      fprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_wmol[3][i],sim_prf_wmol[4][i],sim_prf_wmol[5][i],sim_prf_wmol[6][i],
                  sim_prf_wmol[7][i],sim_prf_wmol[8][i],sim_prf_wmol[9][i],sim_prf_wmol[10][i]);
      fprintf(fp,"%10.3e\n",sim_prf_wmol[11][i]);
    }
    if(f3 > 0)
    {
      // CARD 2C3
      cprintf(fp,"%10s%10.0f%10s%10.0f%10.0f%10.0f%10.0f\n","",sim_prf_haze[0][i],"",
                  0.0,sim_prf_haze[1][i],sim_prf_haze[2][i],sim_prf_haze[3][i]);
    }
  }

  return 0;
}

int WriteCard2D(FILE *fp)
{
  int i;

  // CARD 2D
  fprintf(fp,"%5d%5d%5d%5d\n",mie_n_wlen,0,0,0);
  // CARD 2D1
  fprintf(fp,"%10.3e%70s\n",0.0,"title");
  // CARD 2D2
  for(i=0; i<mie_n_wlen; i++)
  {
    cprintf(fp,"%6.2f%7.5f%7.5f%6.4f%s",mie_wlen_um[i],mie_aext[i]/mie_aext[mie_iref],
                                        (mie_aext[i]-mie_asca[i])/mie_aext[mie_iref],mie_asym[i],
                                        i==mie_n_wlen-1?"\n":(i%3)==2?"\n":"");
  }

  return 0;
}

int WriteCard3(FILE *fp,int mode,double *par)
{
  // DSR_MODTRAN_BO || DSR_CUSTOM_BO || SSR_MODTRAN_BO || SSR_CUSTOM_BO || SSR_MODTRAN_S0 || SSR_CUSTOM_S0
  // par[2] = solar zenith
  // SSR_MODTRAN_BO || SSR_CUSTOM_BO || SSR_MODTRAN_S0 || SSR_CUSTOM_S0
  // par[3] = solar azimuth
  // par[4] = los   zenith
  // par[5] = los   azimuth
  double ph;
  char fnam[] = "WriteCard3";

  if(mode==SIM_MODE_DSR_MODTRAN_BO || mode==SIM_MODE_DSR_CUSTOM_BO)
  {
    // CARD 3
    fprintf(fp,"%10.4f%10.4f%10.6f%5d           .000    0      .000\n",sim_h1,sim_h2,par[2],sim_iday);
  } else
  if(mode==SIM_MODE_SSR_MODTRAN_SO || mode==SIM_MODE_SSR_MODTRAN_BO ||
     mode==SIM_MODE_SSR_CUSTOM_SO  || mode==SIM_MODE_SSR_CUSTOM_BO)
  {
    // CARD 3
    fprintf(fp,"%10.4f%10.4f%10.6f      .000      .000      .000    0        0.00000\n",sim_h1,sim_h2,par[4]);
    if(sim_iemsct == 2)
    {
      // CARD 3A1
      if(mode==SIM_MODE_SSR_CUSTOM_SO  || mode==SIM_MODE_SSR_CUSTOM_BO)
      {
        fprintf(fp,"    2    1%5d    0\n",sim_iday);
      }
      else
      {
        fprintf(fp,"    2    2%5d    0\n",sim_iday);
      }
      ph = par[3]-par[5];
      if(ph < -180.0) ph += 360.0;
      // CARD 3A2
      fprintf(fp,"%10.5f%10.6f      .000      .000      .000      .000      .000      .000\n",ph,par[2]);
      if(mode==SIM_MODE_SSR_CUSTOM_SO  || mode==SIM_MODE_SSR_CUSTOM_BO)
      {
        // CARD 3B1
        fprintf(fp,"%5d%5d\n",sim_n_angl,mie_n_wlen);
        // CARD 3C
        WriteCard3C(fp);
      }
    }
  }
  else
  {
    fprintf(stderr,"%s: invalid mode >>> %d\n",fnam,mode);
    return -1;
  }

  return 0;
}

int WriteCard3C(FILE *fp)
{
  int i,j,k;

  // CARD 3C1
  for(i=0; i<sim_n_angl; i++)
  {
    fprintf(fp,"%10.5f%s",sim_angl[i],i==sim_n_angl-1?"\n":i%8==7?"\n":"");
  }
  // CARD 3C2
  for(i=0; i<mie_n_wlen; i++)
  {
    fprintf(fp,"%10.6f%s",mie_wlen_um[i],i==mie_n_wlen-1?"\n":i%8==7?"\n":"");
  }
  // CARD 3C3
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<mie_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      cprintf(fp," %9.3e%s",sim_phas[k],i==mie_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C4
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<mie_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      fprintf(fp,"%10.3e%s",sim_tropo_phas[k],i==mie_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C5
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<mie_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      fprintf(fp,"%10.3e%s",sim_strat_phas[k],i==mie_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C6
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<mie_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      fprintf(fp,"%10.3e%s",sim_meteo_phas[k],i==mie_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }

  return 0;
}

int WriteCard4(FILE *fp,int mode,double *par)
{
  // DSR_MODTRAN_BO || DSR_CUSTOM_BO || SSR_MODTRAN_BO || SSR_CUSTOM_BO
  // par[0] = min wavelength
  // par[1] = max wavelength
  // SSR_MODTRAN_S0 || SSR_CUSTOM_S0
  // par[0] = wavelength
  // par[1] = #data in modtran.plt
  int n1,n2;
  char fnam[] = "WriteCard4";

  // CARD 4
  if(mode==SIM_MODE_SSR_MODTRAN_SO || mode==SIM_MODE_SSR_CUSTOM_SO)
  {
    n1 = sim_sbmp*((int)((1.0e7/(par[0]*sim_sbmp)-0.5*(par[1]-1.0))+0.5));
    n2 = sim_sbmp*((int)(par[1]+0.5))+n1-sim_sbmp;
    fprintf(fp,"%10d%10d%10.1f%10.1frn\n",n1,n2,1.0*sim_sbmp,2.0*sim_sbmp);
  } else
  if(mode==SIM_MODE_DSR_MODTRAN_BO || mode==SIM_MODE_DSR_CUSTOM_BO ||
     mode==SIM_MODE_SSR_MODTRAN_BO || mode==SIM_MODE_SSR_CUSTOM_BO)
  {
    if(mode==SIM_MODE_DSR_MODTRAN_BO || mode==SIM_MODE_DSR_CUSTOM_BO)
    {
      fprintf(fp,"%10.0f%10.0f%10.1f%10.1frn\n",1.0e7/par[1],1.0e7/par[0],1.0*sim_dbmp,2.0*sim_dbmp);
    }
    else
    {
      fprintf(fp,"%10.0f%10.0f%10.1f%10.1frn\n",1.0e7/par[1],1.0e7/par[0],1.0*sim_sbmp,2.0*sim_sbmp);
    }
  }
  else
  {
    fprintf(stderr,"%s: invalid mode >>> %d\n",fnam,mode);
    return -1;
  }
  if(sim_salb > 0)
  {
    // CARD 4A
    fprintf(fp,"1      0.0\n");
    // CARD 4L1
    fprintf(fp,"%s\n",sim_falb);
    // CARD 4L2
    fprintf(fp,"%d\n",sim_salb);
  }

  return 0;
}

int WriteCard5(FILE *fp)
{
  // CARD 5
  fprintf(fp,"    0\n");
  return 0;
}

int RunModtran(void)
{
  char line[MAXLINE];
  FILE *fp;

  if((fp=fopen("modroot.in","w")) == NULL)
  {
    fprintf(stderr,"Error, cannot open %s\n","modroot.in");
    return -1;
  }
  fprintf(fp,"modtran\n");
  fclose(fp);
  if(access("DATA",R_OK) < 0)
  {
    snprintf(line,MAXLINE,"%s/DATA",sim_path);
    if(symlink(line,"DATA") < 0)
    {
      fprintf(stderr,"Error, cannot create symlink to %s\n",line);
      return -1;
    }
  }
  snprintf(line,MAXLINE,"%s/Mod4v2r1_F90.exe",sim_path);
  if(system(line) != 0)
  {
    fprintf(stderr,"Error in MODTRAN.\n");
    return -1;
  }

  return 0;
}

int ReadPlt_S(const double *par,double *y)
{
  // par[0] = wavelength
  // par[1] = #data in modtran.plt
  int i;
  int n1,n2;
  int err;
  int flag;
  double s;
  double v[2];
  char line[MAXLINE];
  char str1[MAXLINE];
  char str2[MAXLINE];
  char fnam[] = "ReadPlt_S";
  char *p;
  FILE *fp;

  if((fp=fopen("modtran.plt","r")) == NULL)
  {
    fprintf(stderr,"%s: error, cannot open %s\n",fnam,"modtran.plt");
    return -1;
  }
  n1 = (int)(par[1]+0.5);
  i = 0;
  s = 0.0;
  err = 0;
  flag = 0;
  while(fgets(line,MAXLINE,fp) != NULL)
  {
    if(sscanf(line,"%s%s",str1,str2) != 2)
    {
      flag = 1;
      break;
    }
    errno = 0;
    v[0] = strtod(str1,&p);
    if(errno==ERANGE || *p!='\0')
    {
      fprintf(stderr,"%s: convert error >>> %s\n",fnam,line);
      err = 1;
      break;
    }
    errno = 0;
    v[1] = strtod(str2,&p);
    if(errno==ERANGE || *p!='\0')
    {
      fprintf(stderr,"%s: convert error >>> %s\n",fnam,line);
      err = 1;
      break;
    }
    if(fabs(v[0]-par[0]) < SIM_DMAX)
    {
      s += v[1];
      i += 1;
    }
  }
  n2 = i;
  if(flag == 0)
  {
    fprintf(stderr,"%s: error, no blank line\n",fnam);
    err = 1;
  }
  if(n2 != n1)
  {
    fprintf(stderr,"%s: error, n1=%d, n2=%d\n",fnam,n1,n2);
    err = 1;
  }
  *y = s*sim_yuni/n1;
  fclose(fp);
  if(err)
  {
    return -1;
  }

  return 0;
}

int ReadPlt_B(const double *par,int n,const double *x,double *y)
{
  // par[0] = min wavelength
  // par[1] = max wavelength
  int i;
  int n1,n2;
  int err;
  int flag;
  double v[2];
  double wlen[SIM_MAXDATA];
  double data[SIM_MAXDATA];
  double wcnv[SIM_MAXCONV];
  double dcnv[SIM_MAXCONV];
  char line[MAXLINE];
  char str1[MAXLINE];
  char str2[MAXLINE];
  char fnam[] = "ReadPlt_B";
  char *p;
  FILE *fp;

  if((fp=fopen("modtran.plt","r")) == NULL)
  {
    fprintf(stderr,"%s: error, cannot open %s\n",fnam,"modtran.plt");
    return -1;
  }
  i = 0;
  err = 0;
  flag = 0;
  while(fgets(line,MAXLINE,fp) != NULL)
  {
    if(sscanf(line,"%s%s",str1,str2) != 2)
    {
      flag = 1;
      break;
    }
    errno = 0;
    v[0] = strtod(str1,&p);
    if(errno==ERANGE || *p!='\0')
    {
      fprintf(stderr,"%s: convert error >>> %s\n",fnam,line);
      err = 1;
      break;
    }
    errno = 0;
    v[1] = strtod(str2,&p);
    if(errno==ERANGE || *p!='\0')
    {
      fprintf(stderr,"%s: convert error >>> %s\n",fnam,line);
      err = 1;
      break;
    }
    if(i >= SIM_MAXDATA)
    {
      fprintf(stderr,"%s: error, #data exceed the limit >>> %d\n",fnam,i);
      err = 1;
      break;
    }
    wlen[i] = v[0];
    data[i] = v[1];
    i++;
  }
  n1 = i;
  if(flag == 0)
  {
    fprintf(stderr,"%s: error, no blank line\n",fnam);
    err = 1;
  }
  if(n1 < 1)
  {
    fprintf(stderr,"%s: no data\n",fnam);
    err = 1;
  }
  if((n2=Convolute(par[0],par[1],1.0,sim_xsgm,sim_wsgm,
                   n1,wlen,data,SIM_MAXCONV,wcnv,dcnv)) < 0)
  {
    err = 1;
  }
  if(Sampling(n2,wcnv,dcnv,n,x,y,sim_yuni) < 0)
  {
    err = 1;
  }
  fclose(fp);
  if(err)
  {
    return -1;
  }

  return 0;
}

int Convolute(double xmin,double xmax,double xstp,double xsgm,double wsgm,
              int ninp,const double *xinp,const double *yinp,
              int nout,double *xout,double *yout)
{
  int i,j,k;
  int nstp;
  int nwid;
  double norm;
  double offset = 0.5000000000001;
  char fnam[] = "Convolute";

  if(isnan(xmin)) xmin = xinp[0];
  if(isnan(xmax)) xmax = xinp[ninp-1];
  nstp = (int)((xmax-xmin)/xstp+0.5);
  if(nstp < 1)
  {
    fprintf(stderr,"%s: error, invalid #steps (xmin=%13.6e xmax=%13.6e xstp=%13.6e nstp=%d\n",
                    fnam,xmin,xmax,xstp,nstp);
    return -1;
  } else
  if(nstp >= nout)
  {
    fprintf(stderr,"%s: warning, nstp=%d -> changed to %d\n",fnam,nstp,nout-1);
    nstp = nout-1;
  }
  nwid = (int)(xsgm*wsgm/xstp+0.5);
  if(nwid < 1)
  {
    fprintf(stderr,"%s: error, nwid=%d, xsgm=%13.6e wsgm=%13.6e xstp=%13.6e\n",fnam,nwid,xsgm,wsgm,xstp);
    return -1;
  }

  for(i=0; i<=nstp; i++)
  {
    xout[i] = xmin+xstp*i;
    yout[i] = 0.0;
  }
  for(i=0; i<ninp; i++)
  {
    norm = yinp[i]*(i==ninp-1?fabs(xinp[i-1]-xinp[i]):fabs(xinp[i+1]-xinp[i]));
    j = (int)((xinp[i]-xmin)/xstp+offset);
    for(k=j-nwid; k<=j+nwid; k++)
    {
      if(k < 0)
      {
        continue;
      } else
      if(k > nstp)
      {
        break;
      }
      yout[k] += norm*Gauss(xsgm,xinp[i],xout[k]);
    }
  }

  return nstp+1;
}

double Gauss(double s,double x0,double x)
{
  return exp(-0.5*(x-x0)*(x-x0)/(s*s))/(s*sqrt(2.0*M_PI));
}

double range(double a)
{
  double b;

  b = a/PI2;
  return a-PI2*floor(b);
}

double range2(double a)
{
  double b;

  b = (a+PI)/PI2;
  return a-PI2*floor(b);
}

int PolToXyz(double th,double ph,double *x,double *y,double *z)
{
  *x = sin(th)*cos(ph);
  *y = sin(th)*sin(ph);
  *z = cos(th);

  return 0;
}

int XyzToPol(double x,double y,double z,double *th,double *ph)
{
  double r;

  r   = sqrt(x*x+y*y);
  *th = atan2(r,z);
  *ph = range(atan2(y,x));

  return 0;
}

void IjkToXyz(double th,double ph,
              double xi,double yi,double zi,
              double *x,double *y,double *z)
{
  *x =  xi*cos(th)*cos(ph)-yi*sin(ph)+zi*sin(th)*cos(ph);
  *y =  xi*cos(th)*sin(ph)+yi*cos(ph)+zi*sin(th)*sin(ph);
  *z = -xi*sin(th)+zi*cos(th);
}

void XyzToIjk(double th,double ph,
              double xi,double yi,double zi,
              double *x,double *y,double *z)
{
  *x =  xi*cos(th)*cos(ph)+yi*cos(th)*sin(ph)-zi*sin(th);
  *y = -xi*sin(ph)+yi*cos(ph);
  *z =  xi*sin(th)*cos(ph)+yi*sin(th)*sin(ph)+zi*cos(th);
}

int Interp1D(const double *x1,const double *z1,int nx1,
             const double *x2,      double *z2,int nx2,int f)
{
  // x1[nx1],z1[nx1]
  // x2[nx2],z2[nx2]
  // x1 must be arranged in ascending order
  int i,j;
  static int flag = 1;
  static int *i_1 = NULL;
  static int *i_2 = NULL;

  if(flag==1 || f==1)
  {
    if(i_1 != NULL) free(i_1);
    if(i_2 != NULL) free(i_2);
    i_1 = (int*)malloc(nx2*sizeof(int));
    i_2 = (int*)malloc(nx2*sizeof(int));
    if(i_1==NULL || i_2==NULL)
    {
      fprintf(stderr,"Interp1D: error in allocating memory\n");
      return -1;
    }
    for(i=0; i<nx2; i++)
    {
      i_1[i] = i_2[i] = -1;
      for(j=nx1-1; j>=0; j--)
      {
        if(x1[j] <= x2[i])
        {
          i_1[i] = j;
          break;
        }
      }
      for(j=0; j<nx1; j++)
      {
        if(x1[j] >= x2[i])
        {
          i_2[i] = j;
          break;
        }
      }
      if(i_1[i] < 0)
      {
        if(nx1 < 2)
        {
          i_1[i] = 0;
          i_2[i] = 0;
        }
        else
        {
          i_1[i] = 0;
          i_2[i] = 1;
        }
      }
      if(i_2[i] < 0)
      {
        if(nx1 < 2)
        {
          i_1[i] = 0;
          i_2[i] = 0;
        }
        else
        {
          i_1[i] = nx1-2;
          i_2[i] = nx1-1;
        }
      }
      if(cnt_db)
      {
        fprintf(stderr,"Interp1D: i_1[%d]=%d, i_2[%d]=%d\n",i,i_1[i],i,i_2[i]);
      }
    }
    flag = 0;
  }

  for(i=0; i<nx2; i++)
  {
    if(fabs(x1[i_2[i]]-x1[i_1[i]]) < EPSILON)
    {
      // NO INTERPOLATION IS NECESSARY
      z2[i] = z1[i_1[i]];
    }
    else
    {
      z2[i] = INTERP(x2[i],x1[i_1[i]],x1[i_2[i]],z1[i_1[i]],z1[i_2[i]]);
    }
  }

  return 0;
}

int Interp2D(const double *x1,const double *y1,const double *z1,int nx1,int ny1,
             const double *x2,const double *y2,      double *z2,int nx2,int ny2,int f)
{
  // x1[nx1],y1[ny1],z1[nx1*ny1]
  // x2[nx2],y2[ny2],z2[nx2*ny2]
  // x1,y1 must be arranged in ascending order
  int i,j,k;
  static int flag = 1;
  static int *i_1 = NULL;
  static int *i_2 = NULL;
  static int *j_1 = NULL;
  static int *j_2 = NULL;
  int k_1,k_2;
  double p1,p2;

  if(flag==1 || f==1)
  {
    if(i_1 != NULL) free(i_1);
    if(i_2 != NULL) free(i_2);
    if(j_1 != NULL) free(j_1);
    if(j_2 != NULL) free(j_2);
    i_1 = (int*)malloc(nx2*sizeof(int));
    i_2 = (int*)malloc(nx2*sizeof(int));
    j_1 = (int*)malloc(ny2*sizeof(int));
    j_2 = (int*)malloc(ny2*sizeof(int));
    if(i_1==NULL || i_2==NULL || j_1==NULL || j_2==NULL)
    {
      fprintf(stderr,"Interp2D: error in allocating memory\n");
      return -1;
    }
    for(i=0; i<nx2; i++)
    {
      i_1[i] = i_2[i] = -1;
      for(j=nx1-1; j>=0; j--)
      {
        if(x1[j] <= x2[i])
        {
          i_1[i] = j;
          break;
        }
      }
      for(j=0; j<nx1; j++)
      {
        if(x1[j] >= x2[i])
        {
          i_2[i] = j;
          break;
        }
      }
      if(i_1[i] < 0)
      {
        if(nx1 < 2)
        {
          i_1[i] = 0;
          i_2[i] = 0;
        }
        else
        {
          i_1[i] = 0;
          i_2[i] = 1;
        }
      }
      if(i_2[i] < 0)
      {
        if(nx1 < 2)
        {
          i_1[i] = 0;
          i_2[i] = 0;
        }
        else
        {
          i_1[i] = nx1-2;
          i_2[i] = nx1-1;
        }
      }
      if(cnt_db)
      {
        fprintf(stderr,"Interp2D: i_1[%d]=%d, i_2[%d]=%d\n",i,i_1[i],i,i_2[i]);
      }
    }
    for(i=0; i<ny2; i++)
    {
      j_1[i] = j_2[i] = -1;
      for(j=ny1-1; j>=0; j--)
      {
        if(y1[j] <= y2[i])
        {
          j_1[i] = j;
          break;
        }
      }
      for(j=0; j<ny1; j++)
      {
        if(y1[j] >= y2[i])
        {
          j_2[i] = j;
          break;
        }
      }
      if(j_1[i] < 0)
      {
        if(ny1 < 2)
        {
          j_1[i] = 0;
          j_2[i] = 0;
        }
        else
        {
          j_1[i] = 0;
          j_2[i] = 1;
        }
      }
      if(j_2[i] < 0)
      {
        if(ny1 < 2)
        {
          j_1[i] = 0;
          j_2[i] = 0;
        }
        else
        {
          j_1[i] = ny1-2;
          j_2[i] = ny1-1;
        }
      }
      if(cnt_db)
      {
        fprintf(stderr,"Interp2D: j_1[%d]=%d, j_2[%d]=%d\n",i,j_1[i],i,j_2[i]);
      }
    }
    flag = 0;
  }

  for(i=0; i<nx2; i++)
  {
    if(fabs(x1[i_2[i]]-x1[i_1[i]]) < EPSILON)
    {
      for(j=0; j<ny2; j++)
      {
        k = ny2*i+j;
        if(fabs(y1[j_2[j]]-y1[j_1[j]]) < EPSILON)
        {
          // NO INTERPOLATION IS NECESSARY
          k_1 = ny1*i_1[i]+j_1[j];
          z2[k] = z1[k_1];
        }
        else
        {
          // ONLY Y INTERPOLATION IS NECESSARY
          k_1 = ny1*i_1[i]+j_1[j];
          k_2 = ny1*i_1[i]+j_2[j];
          z2[k] = INTERP(y2[j],y1[j_1[j]],y1[j_2[j]],z1[k_1],z1[k_2]);
        }
      }
    }
    else
    {
      for(j=0; j<ny2; j++)
      {
        k = ny2*i+j;
        if(fabs(y1[j_2[j]]-y1[j_1[j]]) < EPSILON)
        {
          // ONLY X INTERPOLATION IS NECESSARY
          k_1 = ny1*i_1[i]+j_1[j];
          k_2 = ny1*i_2[i]+j_1[j];
          z2[k] = INTERP(x2[i],x1[i_1[i]],x1[i_2[i]],z1[k_1],z1[k_2]);
        }
        else
        {
          // INTERPOLATE IN X THEN Y
          k_1 = ny1*i_1[i]+j_1[j];
          k_2 = ny1*i_2[i]+j_1[j];
          p1 = INTERP(x2[i],x1[i_1[i]],x1[i_2[i]],z1[k_1],z1[k_2]);
          k_1 = ny1*i_1[i]+j_2[j];
          k_2 = ny1*i_2[i]+j_2[j];
          p2 = INTERP(x2[i],x1[i_1[i]],x1[i_2[i]],z1[k_1],z1[k_2]);
          z2[k] = INTERP(y2[j],y1[j_1[j]],y1[j_2[j]],p1,p2);
        }
      }
    }
  }

  return 0;
}

int Sampling(int ninp,const double *xinp,const double *yinp,
             int nout,const double *xout,double *yout,double yuni)
{
  int i,j;
  int i1,i2;
  int err;
  double x1,x2;
  double xstp;
  double epsilon = 1.0e-12;
  char fnam[] = "Sampling";

  // check input
  if(ninp < 2)
  {
    fprintf(stderr,"%s: error, ninp=%d\n",fnam,ninp);
    return -1;
  }
  // check whether data interval is constant or not
  xstp = xinp[1]-xinp[0];
  err = 0;
  for(i=1; i<ninp; i++)
  {
    if(fabs(xinp[i]-(xinp[0]+xstp*i)) > epsilon)
    {
      err = 1;
      break;
    }
  }
  // interpolate
  if(err)
  {
    if(cnt_vb > 2) fprintf(stderr,"%s: nonconstant data interval.\n",fnam);
    x1 = xinp[0];
    x2 = xinp[ninp-1];
    for(i=0; i<nout; i++)
    {
      if(xout[i] < x1)
      {
        i1 = 0;
        i2 = 1;
      } else
      if(xout[i] > x2)
      {
        i2 = ninp-1;
        i1 = i2-1;
      }
      else
      {
        i1 = -1;
        i2 = -1;
        for(j=1; j<ninp; j++)
        {
          if(xout[i]>=xinp[j-1] && xout[i]<=xinp[j])
          {
            i1 = j-1;
            i2 = j;
            break;
          }
        }
        if(i1<0 || i2<0)
        {
          fprintf(stderr,"%s: error, faild in finding region for %13.6e\n",fnam,xout[i]);
          return -1;
        }
      }
      yout[i] = (yinp[i1]+(yinp[i2]-yinp[i1])*(xout[i]-xinp[i1])/(xinp[i2]-xinp[i1]))*yuni;
    }
  }
  else
  {
    if(cnt_vb > 2) fprintf(stderr,"%s: constant data interval.\n",fnam);
    for(i=0; i<nout; i++)
    {
      i1 = (int)((xout[i]-xinp[0])/xstp+epsilon);
      if(i1 < 0) i1 = 0;
      i2 = i1+1;
      if(i2 >= ninp)
      {
        i2 = ninp-1;
        i1 = i2-1;
      }
      yout[i] = (yinp[i1]+(yinp[i2]-yinp[i1])*(xout[i]-xinp[i1])/(xinp[i2]-xinp[i1]))*yuni;
    }
  }

  return 0;
}

int SamplingE(int ninp,const double *xinp,const double *yinp,
              int nout,const double *xout,double *yout,double yuni)
{
  int i,j;
  int i1,i2;
  int err;
  double x1,x2;
  double xstp;
  double epsilon = 1.0e-12;
  double delta = 1.0e-100;
  char fnam[] = "SamplingE";

  // check input
  if(ninp < 2)
  {
    fprintf(stderr,"%s: error, ninp=%d\n",fnam,ninp);
    return -1;
  }
  // check whether data interval is constant or not
  xstp = xinp[1]-xinp[0];
  err = 0;
  for(i=1; i<ninp; i++)
  {
    if(fabs(xinp[i]-(xinp[0]+xstp*i)) > epsilon)
    {
      err = 1;
      break;
    }
  }
  // interpolate
  if(err)
  {
    if(cnt_vb > 2) fprintf(stderr,"%s: nonconstant data interval.\n",fnam);
    x1 = xinp[0];
    x2 = xinp[ninp-1];
    for(i=0; i<nout; i++)
    {
      if(xout[i] < x1)
      {
        i1 = 0;
        i2 = 1;
      } else
      if(xout[i] > x2)
      {
        i2 = ninp-1;
        i1 = i2-1;
      }
      else
      {
        i1 = -1;
        i2 = -1;
        for(j=1; j<ninp; j++)
        {
          if(xout[i]>=xinp[j-1] && xout[i]<=xinp[j])
          {
            i1 = j-1;
            i2 = j;
            break;
          }
        }
        if(i1<0 || i2<0)
        {
          fprintf(stderr,"%s: error, faild in finding region for %13.6e\n",fnam,xout[i]);
          return -1;
        }
      }
      if(yinp[i1]<delta || yinp[i2]<delta)
      {
        yout[i] = (yinp[i1]+(yinp[i2]-yinp[i1])*(xout[i]-xinp[i1])/(xinp[i2]-xinp[i1]))*yuni;
      }
      else
      {
        yout[i] = (yinp[i1]*pow(yinp[i2]/yinp[i1],(xout[i]-xinp[i1])/(xinp[i2]-xinp[i1])))*yuni;
      }
    }
  }
  else
  {
    if(cnt_vb > 2) fprintf(stderr,"%s: constant data interval.\n",fnam);
    for(i=0; i<nout; i++)
    {
      i1 = (int)((xout[i]-xinp[0])/xstp+epsilon);
      if(i1 < 0) i1 = 0;
      i2 = i1+1;
      if(i2 >= ninp)
      {
        i2 = ninp-1;
        i1 = i2-1;
      }
      if(yinp[i1]<delta || yinp[i2]<delta)
      {
        yout[i] = (yinp[i1]+(yinp[i2]-yinp[i1])*(xout[i]-xinp[i1])/(xinp[i2]-xinp[i1]))*yuni;
      }
      else
      {
        yout[i] = (yinp[i1]*pow(yinp[i2]/yinp[i1],(xout[i]-xinp[i1])/(xinp[i2]-xinp[i1])))*yuni;
      }
    }
  }

  return 0;
}

int PreConfig(void)
{
  switch(sim_iatm)
  {
    case 0:
    case 1:
    case 2:
    case 4:
    case 6:
    case 7:
      sim_isea = 1;
      break;
    case 3:
    case 5:
      sim_isea = 2;
      break;
    default:
      sim_isea = 1;
      break;
  }
  if(sim_prf_mode == SIM_MODTRAN_VTAU)
  {
    if(PrfInitMolecule(sim_iatm) < 0) return -1;
    if(PrfInitAerosol(sim_ihaz,sim_isea,sim_ivul,sim_vmie) < 0) return -1;
  }

  return 0;
}

int ReadConfig(void)
{
  int n,nc;
  int num;
  int idx;
  int err;
  int ntmp;
  double xtmp;
  double uni;
  double *x,*y;
  char line[MAXLINE];
  char temp[MAXLINE];
  char str[MAXENTR][MAXLINE];
  char fnam[] = "ReadConfig";
  char *p;
  FILE *fp;

  if(strcmp(cnt_conf,"")==0 || strcmp(cnt_conf,NONAME)==0)
  {
    return 0;
  }
  // Read parameters which may have affects on others
  if((fp=fopen(cnt_conf,"r")) == NULL)
  {
    fprintf(stderr,"%s: error, cannot open %s\n",fnam,cnt_conf);
    return -1;
  }
  err = 0;
  while(fgets(line,MAXLINE,fp) != NULL)
  {
    strncpy(temp,line,MAXLINE);
    if((p=strchr(temp,'#')) != NULL) *p = '\0';
    // read line
    for(n=nc=0,p=temp; n<MAXENTR; n++,p+=nc)
    {
      if(sscanf(p,"%s%n",str[n],&nc) == EOF) break;
    }
    if(n < 1) continue;
    if(strcasecmp(str[0],"prf_falt") == 0)
    {
      num = 0;
      uni = 1.0;
      if(n > 2)
      {
        errno = 0;
        ntmp = strtol(str[2],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=0) num = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 3)
      {
        errno = 0;
        xtmp = strtod(str[3],&p);
        if(errno!=ERANGE && *p=='\0') uni = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 1)
      {
        if(strcmp(str[1],NONAME) != 0)
        {
          if((prf_n_alt=ReadXA(str[1],PRF_MAXNALT,num,uni,NULL,prf_alt)) < 1)
          {
            err = 1;
            break;
          }
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s %4d %13.6e\n",str[0],str[1],num,uni);
        cnt_n_cmnt++;
      }
    }
  }
  fclose(fp);
  if(err)
  {
    return -1;
  }
  if(PreConfig() < 0) return -1;
  // Read others
  if((fp=fopen(cnt_conf,"r")) == NULL)
  {
    fprintf(stderr,"%s: error, cannot open %s\n",fnam,cnt_conf);
    return -1;
  }
  err = 0;
  while(fgets(line,MAXLINE,fp) != NULL)
  {
    strncpy(temp,line,MAXLINE);
    if((p=strchr(temp,'#')) != NULL) *p = '\0';
    // read line
    for(n=nc=0,p=temp; n<MAXENTR; n++,p+=nc)
    {
      if(sscanf(p,"%s%n",str[n],&nc) == EOF) break;
    }
    if(n < 1) continue;
    if(strcasecmp(str[0],"ssr_th_min") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') ssr_th_min = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30.14e\n",str[0],ssr_th_min);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"ssr_th_max") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') ssr_th_max = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30.14e\n",str[0],ssr_th_max);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"ssr_n_tdiv") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0') ssr_n_tdiv = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30d\n",str[0],ssr_n_tdiv);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"ssr_n_pdiv") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0') ssr_n_pdiv = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30d\n",str[0],ssr_n_pdiv);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"ssr_wlen") == 0)
    {
      num = 0;
      uni = 1.0;
      if(n > 2)
      {
        errno = 0;
        ntmp = strtol(str[2],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=0) num = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 3)
      {
        errno = 0;
        xtmp = strtod(str[3],&p);
        if(errno!=ERANGE && *p=='\0') uni = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 1)
      {
        if(strcmp(str[1],NONAME) != 0)
        {
          if((ssr_n_wlen=ReadXA(str[1],SSR_MAXWLEN,num,uni,NULL,ssr_wlen)) < 1)
          {
            err = 1;
            break;
          }
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s %4d %13.6e\n",str[0],str[1],num,uni);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"ssr_sbmp") == 0)
    {
      num = 0;
      if(n > 2)
      {
        errno = 0;
        ntmp = strtol(str[2],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=0) num = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 1)
      {
        if(strcmp(str[1],NONAME) != 0)
        {
          if((ssr_n_sbmp=ReadNA(str[1],SSR_MAXWLEN,num,ssr_sbmp)) < 1)
          {
            err = 1;
            break;
          }
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s %4d\n",str[0],str[1],num);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_th_sun") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') sim_th_sun = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30.14e\n",str[0],sim_th_sun);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_ph_sun") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') sim_ph_sun = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30.14e\n",str[0],sim_ph_sun);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_th_los") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') sim_th_los = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30.14e\n",str[0],sim_th_los);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_ph_los") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') sim_ph_los = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30.14e\n",str[0],sim_ph_los);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_path") == 0)
    {
      if(n > 1)
      {
        strncpy(sim_path,str[1],MAXLINE);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],sim_path);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_falb") == 0)
    {
      if(n > 1)
      {
        strncpy(sim_falb,str[1],MAXLINE);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],sim_falb);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_salb") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0') sim_salb = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30d\n",str[0],sim_salb);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_prf_vscl") == 0)
    {
      idx = 0;
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=0) idx = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(idx<0 || idx>=PRF_NMOL)
      {
        fprintf(stderr,"%s: index out of range >>> %s\n",fnam,line);
        err = 1;
        break;
      }
      if(n > 2)
      {
        errno = 0;
        xtmp = strtod(str[2],&p);
        if(errno!=ERANGE && *p=='\0') sim_prf_vscl[idx] = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>2 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %2d %27.14e\n",str[0],idx,sim_prf_vscl[idx]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_prf_pscl") == 0)
    {
      idx = 0;
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=0) idx = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(idx<0 || idx>=PRF_NMOL)
      {
        fprintf(stderr,"%s: index out of range >>> %s\n",fnam,line);
        err = 1;
        break;
      }
      if(n > 2)
      {
        errno = 0;
        ntmp = strtol(str[2],&p,10);
        if(errno!=ERANGE && *p=='\0') sim_prf_pscl[idx] = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>2 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %2d %27d\n",str[0],idx,sim_prf_pscl[idx]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_prf_jcharx") == 0)
    {
      if(n > 1)
      {
        sim_prf_jcharx = str[1][0];
      }
      else
      {
        sim_prf_jcharx = ' ';
      }
      if(cnt_hp && cnt_n_cmnt<CNT_MAXCMNT)
      {
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30c\n",str[0],sim_prf_jcharx);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"prf_fpre") == 0)
    {
      num = 1;
      if(n > 2)
      {
        errno = 0;
        ntmp = strtol(str[2],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=0) num = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 3)
      {
        prf_jchar[0] = str[3][0];
      }
      if(n > 1)
      {
        if(strcmp(str[1],NONAME) != 0)
        {
          x = NULL;
          y = NULL;
          if((nc=ReadXYP(str[1],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
          {
            err = 1;
            break;
          }
          if(Sampling(nc,x,y,prf_n_alt,prf_alt,prf_pres,1.0) < 0)
          {
            err = 1;
          }
          free(x);
          free(y);
          if(err)
          {
            break;
          }
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s %4d %c\n",str[0],str[1],num,prf_jchar[0]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"prf_ftmp") == 0)
    {
      num = 1;
      if(n > 2)
      {
        errno = 0;
        ntmp = strtol(str[2],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=0) num = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 3)
      {
        prf_jchar[1] = str[3][0];
      }
      if(n > 1)
      {
        if(strcmp(str[1],NONAME) != 0)
        {
          x = NULL;
          y = NULL;
          if((nc=ReadXYP(str[1],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
          {
            err = 1;
            break;
          }
          if(Sampling(nc,x,y,prf_n_alt,prf_alt,prf_temp,1.0) < 0)
          {
            err = 1;
          }
          free(x);
          free(y);
          if(err)
          {
            break;
          }
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s %4d %c\n",str[0],str[1],num,prf_jchar[1]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"prf_fmol") == 0)
    {
      idx = 0;
      num = 1;
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=0) idx = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(idx<0 || idx>=PRF_NMOL)
      {
        fprintf(stderr,"%s: index out of range >>> %s\n",fnam,line);
        err = 1;
        break;
      }
      if(n > 3)
      {
        errno = 0;
        ntmp = strtol(str[3],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=0) num = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 4)
      {
        prf_jchar[idx+2] = str[4][0];
      }
      if(n > 2)
      {
        if(strcmp(str[2],NONAME) != 0)
        {
          x = NULL;
          y = NULL;
          if((nc=ReadXYP(str[2],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
          {
            err = 1;
            break;
          }
          if(Sampling(nc,x,y,prf_n_alt,prf_alt,prf_wmol[idx],1.0) < 0)
          {
            err = 1;
          }
          free(x);
          free(y);
          if(err)
          {
            break;
          }
        }
      }
      if(cnt_hp && n>2 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %4d %25s %4d %c\n",str[0],idx,str[2],num,prf_jchar[idx+2]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"prf_fhaz") == 0)
    {
      idx = 0;
      num = 1;
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=0) idx = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(idx<0 || idx>=PRF_NHAZ)
      {
        fprintf(stderr,"%s: index out of range >>> %s\n",fnam,line);
        err = 1;
        break;
      }
      if(n > 3)
      {
        errno = 0;
        ntmp = strtol(str[3],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=0) num = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 2)
      {
        if(strcmp(str[2],NONAME) != 0)
        {
          x = NULL;
          y = NULL;
          if((nc=ReadXYP(str[2],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
          {
            err = 1;
            break;
          }
          if(SamplingE(nc,x,y,prf_n_alt,prf_alt,prf_haze[idx],1.0) < 0)
          {
            err = 1;
          }
          free(x);
          free(y);
          if(err)
          {
            break;
          }
        }
      }
      if(cnt_hp && n>2 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %4d %25s %4d\n",str[0],idx,str[2],num);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"prf_pres_gnd") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') prf_pres_gnd = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.4e\n",str[0],prf_pres_gnd);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"prf_temp_gnd") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') prf_temp_gnd = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.4e\n",str[0],prf_temp_gnd);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"cld_thik") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') cld_thik = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.4e\n",str[0],cld_thik);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"cld_alt") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') cld_alt = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.4e\n",str[0],cld_alt);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"cld_ext") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') cld_ext = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.4e\n",str[0],cld_ext);
        cnt_n_cmnt++;
      }
    }
    else
    {
      fprintf(stderr,"%s: unrecognized option >>> %s\n",fnam,line);
      break;
    }
  }
  fclose(fp);
  if(err)
  {
    return -1;
  }
  if(PostConfig() < 0) return -1;

  return 0;
}

int PostConfig(void)
{
  int i,j;
  int flag;

  // post configuration
  if(sim_prf_mode == SIM_MODTRAN_VTAU)
  {
    if(PrfGround() < 0) return -1;
    if(PrfScaleMolecule() < 0) return -1;
    if(PrfCalcAOD() < 0) return -1;
    if(cnt_vb > 1)
    {
      flag = 0;
      fprintf(stderr,"Molecular profile:\n");
      fprintf(stderr,"%6s %9s %9s","Alt","Pres","Temp");
      if(!isnan(prf_pres_gnd)) flag = 1;
      if(!isnan(prf_temp_gnd)) flag = 1;
      for(j=0; j<PRF_NMOL; j++)
      {
        fprintf(stderr," %9s",prf_wmol_nam[j]);
        if(sim_prf_pscl[j]!=0 || (!isnan(sim_prf_vscl[j]))) flag = 1;
      }
      fprintf(stderr,"\n");
      for(i=0; i<prf_n_alt; i++)
      {
        fprintf(stderr,"%6.2f %9.2e %9.2e",prf_alt[i],prf_pres[i],prf_temp[i]);
        for(j=0; j<PRF_NMOL; j++)
        {
          fprintf(stderr," %9.2e",prf_wmol[j][i]);
        }
        fprintf(stderr,"\n");
      }
      if(flag)
      {
        fprintf(stderr,"After scaling:\n");
        fprintf(stderr,"%6s","Alt");
        if(!isnan(prf_pres_gnd))
        {
          fprintf(stderr," %9s","Pres");
        }
        if(!isnan(prf_temp_gnd))
        {
          fprintf(stderr," %9s","Temp");
        }
        for(j=0; j<PRF_NMOL; j++)
        {
          if(sim_prf_pscl[j]!=0 || (!isnan(sim_prf_vscl[j])))
          {
            fprintf(stderr," %9s",prf_wmol_nam[j]);
          }
        }
        fprintf(stderr,"\n");
        for(i=0; i<prf_n_alt; i++)
        {
          fprintf(stderr,"%6.2f",prf_alt[i]);
          if(!isnan(prf_pres_gnd))
          {
            fprintf(stderr," %9.2e",sim_prf_pres[i]);
          }
          if(!isnan(prf_temp_gnd))
          {
            fprintf(stderr," %9.2e",sim_prf_temp[i]);
          }
          for(j=0; j<PRF_NMOL; j++)
          {
            if(sim_prf_pscl[j]!=0 || (!isnan(sim_prf_vscl[j])))
            {
              fprintf(stderr," %9.2e",sim_prf_wmol[j][i]);
            }
          }
          fprintf(stderr,"\n");
        }
      }
      fprintf(stderr,"Aerosol profile:\n");
      for(i=0; i<prf_n_alt; i++)
      {
        fprintf(stderr,"%6.2f %13.10f %13.10f %13.10f %13.10f\n",prf_alt[i],prf_haze[0][i],prf_haze[1][i],prf_haze[2][i],prf_haze[3][i]);
      }
    }
  }

  return 0;
}

int ReadXA(char *s,int size,int cx,double ux,int (*func)(double*),double *x)
{
  int nd;
  int np = 1;
  int c[1];
  double u[1];
  double *d[1];

  // fill array
  c[0] = cx; u[0] = ux; d[0] = x;
  // read input
  if(func == NULL)
  {
    nd = ReadDA(s,size,np,c,u,d);
  }
  else
  {
    nd = ReadDC(s,size,np,c,u,func,d);
  }

  return nd;
}

int ReadXYA(char *s,int size,int cx,int cy,double ux,double uy,int (*func)(double*),double *x,double *y)
{
  int nd;
  int np = 2;
  int c[2];
  double u[2];
  double *d[2];

  // fill array
  c[0] = cx; u[0] = ux; d[0] = x;
  c[1] = cy; u[1] = uy; d[1] = y;
  // read input
  if(func == NULL)
  {
    nd = ReadDA(s,size,np,c,u,d);
  }
  else
  {
    nd = ReadDC(s,size,np,c,u,func,d);
  }

  return nd;
}

int ReadXYZA(char *s,int size,int cx,int cy,int cz,double ux,double uy,double uz,int (*func)(double*),
             double *x,double *y,double *z)
{
  int nd;
  int np = 3;
  int c[3];
  double u[3];
  double *d[3];

  // fill array
  c[0] = cx; u[0] = ux; d[0] = x;
  c[1] = cy; u[1] = uy; d[1] = y;
  c[2] = cz; u[2] = uz; d[2] = z;
  // read input
  if(func == NULL)
  {
    nd = ReadDA(s,size,np,c,u,d);
  }
  else
  {
    nd = ReadDC(s,size,np,c,u,func,d);
  }

  return nd;
}

int ReadXP(char *s,int size,int cx,double ux,int (*func)(double*),double **x)
{
  int n;
  int nd;
  int err;
  int np = 1;
  int c[1];
  double u[1];
  double *d[1];
  double **p[1];
  char fnam[] = "ReadXP";

  // fill array
  c[0] = cx; u[0] = ux; d[0] = *x; p[0] = x;
  // allocate memory
  for(n=0; n<np; n++)
  {
    if(d[n] == NULL)
    {
      if((d[n]=(double*)malloc(size*sizeof(double))) == NULL)
      {
        fprintf(stderr,"%s: failed in allocating memory (%s)\n",fnam,s);
        return -1;
      }
    }
  }
  // read input
  if(func == NULL)
  {
    nd = ReadDA(s,size,np,c,u,d);
  }
  else
  {
    nd = ReadDC(s,size,np,c,u,func,d);
  }
  if(nd < 0)
  {
    for(n=0; n<np; n++)
    {
      if(*p[n] == NULL) free(d[n]);
    }
    return -1;
  }
  // resize memory
  err = 0;
  for(n=0; n<np; n++)
  {
    if(*p[n] == NULL)
    {
      if((*p[n]=(double*)realloc(d[n],nd*sizeof(double))) == NULL)
      {
        fprintf(stderr,"%s: failed in allocating memory (%s)\n",fnam,s);
        free(d[n]); // this is needed only when realloc failed
        err = 1;
      }
    }
  }
  if(err)
  {
    return -1;
  }

  return nd;
}

int ReadXYP(char *s,int size,int cx,int cy,double ux,double uy,int (*func)(double*),double **x,double **y)
{
  int n;
  int nd;
  int err;
  int np = 2;
  int c[2];
  double u[2];
  double *d[2];
  double **p[2];
  char fnam[] = "ReadXYP";

  // fill array
  c[0] = cx; u[0] = ux; d[0] = *x; p[0] = x;
  c[1] = cy; u[1] = uy; d[1] = *y; p[1] = y;
  // allocate memory
  for(n=0; n<np; n++)
  {
    if(d[n] == NULL)
    {
      if((d[n]=(double*)malloc(size*sizeof(double))) == NULL)
      {
        fprintf(stderr,"%s: failed in allocating memory (%s)\n",fnam,s);
        return -1;
      }
    }
  }
  // read input
  if(func == NULL)
  {
    nd = ReadDA(s,size,np,c,u,d);
  }
  else
  {
    nd = ReadDC(s,size,np,c,u,func,d);
  }
  if(nd < 0)
  {
    for(n=0; n<np; n++)
    {
      if(*p[n] == NULL) free(d[n]);
    }
    return -1;
  }
  // resize memory
  err = 0;
  for(n=0; n<np; n++)
  {
    if(*p[n] == NULL)
    {
      if((*p[n]=(double*)realloc(d[n],nd*sizeof(double))) == NULL)
      {
        fprintf(stderr,"%s: failed in allocating memory (%s)\n",fnam,s);
        free(d[n]); // this is needed only when realloc failed
        err = 1;
      }
    }
  }
  if(err)
  {
    return -1;
  }

  return nd;
}

int ReadXYZP(char *s,int size,int cx,int cy,int cz,double ux,double uy,double uz,int (*func)(double*),
             double **x,double **y,double **z)
{
  int n;
  int nd;
  int err;
  int np = 3;
  int c[3];
  double u[3];
  double *d[3];
  double **p[3];
  char fnam[] = "ReadXYZP";

  // fill array
  c[0] = cx; u[0] = ux; d[0] = *x; p[0] = x;
  c[1] = cy; u[1] = uy; d[1] = *y; p[1] = y;
  c[2] = cz; u[2] = uz; d[2] = *z; p[2] = z;
  // allocate memory
  for(n=0; n<np; n++)
  {
    if(d[n] == NULL)
    {
      if((d[n]=(double*)malloc(size*sizeof(double))) == NULL)
      {
        fprintf(stderr,"%s: failed in allocating memory (%s)\n",fnam,s);
        return -1;
      }
    }
  }
  // read input
  if(func == NULL)
  {
    nd = ReadDA(s,size,np,c,u,d);
  }
  else
  {
    nd = ReadDC(s,size,np,c,u,func,d);
  }
  if(nd < 0)
  {
    for(n=0; n<np; n++)
    {
      if(*p[n] == NULL) free(d[n]);
    }
    return -1;
  }
  // resize memory
  err = 0;
  for(n=0; n<np; n++)
  {
    if(*p[n] == NULL)
    {
      if((*p[n]=(double*)realloc(d[n],nd*sizeof(double))) == NULL)
      {
        fprintf(stderr,"%s: failed in allocating memory (%s)\n",fnam,s);
        free(d[n]); // this is needed only when realloc failed
        err = 1;
      }
    }
  }
  if(err)
  {
    return -1;
  }

  return nd;
}

int ReadDC(char *s,int size,int np,const int *c,const double *u,int (*func)(double*),double **d)
{
  int i,j,n;
  int nc,nd;
  int num;
  int err;
  int flag;
  double v[3];
  char line[MAXLINE];
  char str[MAXENTR][MAXLINE];
  char fnam[] = "ReadDC";
  char *p;
  FILE *fp;

  // check input
  if(np<0 || np>3)
  {
    fprintf(stderr,"%s: error, np=%d\n",fnam,np);
    return -1;
  }
  for(n=0; n<np; n++)
  {
    if(c[n]<0 || c[n]>=MAXENTR)
    {
      fprintf(stderr,"%s: invalid column number >>> c[%d]=%d (%s)\n",fnam,n,c[n],s);
      return -1;
    }
  }
  // open input
  flag = 0;
  if(strcmp(s,"") == 0)
  {
    fp = stdin;
  }
  else
  {
    if((fp=fopen(s,"r")) == NULL)
    {
      fprintf(stderr,"%s: cannot open %s\n",fnam,s);
      return -1;
    }
    flag = 1;
  }
  // read input
  num = c[0];
  for(n=1; n<np; n++)
  {
    num = MAX(num,c[n]);
  }
  i = 0;
  err = 0;
  while(fgets(line,MAXLINE,fp) != NULL)
  {
    for(j=0,p=line; j<=num; j++,p+=nc)
    {
      if(sscanf(p,"%s%n",str[j],&nc) == EOF)
      {
        fprintf(stderr,"%s: read error >>> %s (%s)\n",fnam,line,s);
        err = 1;
        break;
      }
    }
    if(err) break;
    for(n=0; n<np; n++)
    {
      errno = 0;
      v[n] = strtod(str[c[n]],&p)*u[n];
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"%s: convert error >>> %s (%s)\n",fnam,line,s);
        err = 1;
        break;
      }
    }
    if(func(v))
    {
      continue;
    }
    if(i >= size)
    {
      fprintf(stderr,"%s: warning, #data exceed the limit %d (%s)\n",fnam,i,s);
      break;
    }
    for(n=0; n<np; n++)
    {
      d[n][i] = v[n];
    }
    i++;
  }
  if(flag)
  {
    fclose(fp);
  }
  nd = i;
  if(nd < 1)
  {
    fprintf(stderr,"%s: error, nd=%d (%s)\n",fnam,nd,s);
    err = 1;
  }
  if(err)
  {
    return -1;
  }

  return nd;
}

int ReadDA(char *s,int size,int np,const int *c,const double *u,double **d)
{
  int i,j,n;
  int nc,nd;
  int num;
  int err;
  int flag;
  double v[3];
  char line[MAXLINE];
  char str[MAXENTR][MAXLINE];
  char fnam[] = "ReadDA";
  char *p;
  FILE *fp;

  // check input
  if(np<0 || np>3)
  {
    fprintf(stderr,"%s: error, np=%d\n",fnam,np);
    return -1;
  }
  for(n=0; n<np; n++)
  {
    if(c[n]<0 || c[n]>=MAXENTR)
    {
      fprintf(stderr,"%s: invalid column number >>> c[%d]=%d (%s)\n",fnam,n,c[n],s);
      return -1;
    }
  }
  // open input
  flag = 0;
  if(strcmp(s,"") == 0)
  {
    fp = stdin;
  }
  else
  {
    if((fp=fopen(s,"r")) == NULL)
    {
      fprintf(stderr,"%s: cannot open %s\n",fnam,s);
      return -1;
    }
    flag = 1;
  }
  // read input
  num = c[0];
  for(n=1; n<np; n++)
  {
    num = MAX(num,c[n]);
  }
  i = 0;
  err = 0;
  while(fgets(line,MAXLINE,fp) != NULL)
  {
    for(j=0,p=line; j<=num; j++,p+=nc)
    {
      if(sscanf(p,"%s%n",str[j],&nc) == EOF)
      {
        fprintf(stderr,"%s: read error >>> %s (%s)\n",fnam,line,s);
        err = 1;
        break;
      }
    }
    if(err) break;
    for(n=0; n<np; n++)
    {
      errno = 0;
      v[n] = strtod(str[c[n]],&p)*u[n];
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"%s: convert error >>> %s (%s)\n",fnam,line,s);
        err = 1;
        break;
      }
    }
    if(i >= size)
    {
      fprintf(stderr,"%s: warning, #data exceed the limit %d (%s)\n",fnam,i,s);
      break;
    }
    for(n=0; n<np; n++)
    {
      d[n][i] = v[n];
    }
    i++;
  }
  if(flag)
  {
    fclose(fp);
  }
  nd = i;
  if(nd < 1)
  {
    fprintf(stderr,"%s: error, nd=%d (%s)\n",fnam,nd,s);
    err = 1;
  }
  if(err)
  {
    return -1;
  }

  return nd;
}

int ReadNA(char *s,int size,int c,int *d)
{
  int i,j;
  int nc,nd;
  int err;
  int flag;
  int ntmp;
  char line[MAXLINE];
  char str[MAXENTR][MAXLINE];
  char fnam[] = "ReadNA";
  char *p;
  FILE *fp;

  // check input
  if(c<0 || c>=MAXENTR)
  {
    fprintf(stderr,"%s: invalid column number >>> %d (%s)\n",fnam,c,s);
    return -1;
  }
  // open input
  flag = 0;
  if(strcmp(s,"") == 0)
  {
    fp = stdin;
  }
  else
  {
    if((fp=fopen(s,"r")) == NULL)
    {
      fprintf(stderr,"%s: cannot open %s\n",fnam,s);
      return -1;
    }
    flag = 1;
  }
  // read input
  i = 0;
  err = 0;
  while(fgets(line,MAXLINE,fp) != NULL)
  {
    for(j=0,p=line; j<=c; j++,p+=nc)
    {
      if(sscanf(p,"%s%n",str[j],&nc) == EOF)
      {
        fprintf(stderr,"%s: read error >>> %s (%s)\n",fnam,line,s);
        err = 1;
        break;
      }
    }
    if(err) break;
    errno = 0;
    ntmp = strtol(str[c],&p,10);
    if(errno==ERANGE || *p!='\0')
    {
      fprintf(stderr,"%s: convert error >>> %s (%s)\n",fnam,line,s);
      err = 1;
      break;
    }
    if(i >= size)
    {
      fprintf(stderr,"%s: warning, #data exceed the limit %d (%s)\n",fnam,i,s);
      break;
    }
    d[i] = ntmp;
    i++;
  }
  if(flag)
  {
    fclose(fp);
  }
  nd = i;
  if(nd < 1)
  {
    fprintf(stderr,"%s: error, nd=%d (%s)\n",fnam,nd,s);
    err = 1;
  }
  if(err)
  {
    return -1;
  }

  return nd;
}

int GetOpt(int argn,char **args)
{
  int c,rt;
  int ntmp;
  int option_index = 0;
  int this_option_optind;
  double xtmp;
  char *endp;
  struct option long_options[] =
  {
    {"sim_vtau",1,0,'U'},
    {"sim_vmie",1,0,'V'},
    {"sim_wscl",1,0,'W'},
    {"sim_oscl",1,0,'O'},
    {"sim_cmix",1,0,'P'},
    {"sim_iatm",1,0,'A'},
    {"sim_ihaz",1,0,'a'},
    {"sim_icld",1,0,'k'},
    {"sim_grho",1,0,'G'},
    {"sim_h1",1,0,'g'},
    {"sim_h2",1,0,'T'},
    {"sim_xsgm",1,0,'w'},
    {"sim_dbmp",1,0,'m'},
    {"sim_sbmp_init",1,0,'M'},
    {"sim_prf_mode",1,0,'s'},
    {"sim_mode",1,0,'S'},
    {"cnt_conf",0,0,'C'},
    {"cnt_cflg",0,0,'c'},
    {"cnt_db",0,0,'d'},
    {"cnt_vb",0,0,'v'},
    {"cnt_hp",0,0,'h'}
  };

  rt = 0;
  while(1)
  {
    this_option_optind = optind?optind:1;
    c = getopt_long(argn,args,":U:V:W:O:P:A:a:k:G:g:T:w:m:M:s:S:C:cdvh",long_options,&option_index);
    if(c == -1) break;

    switch(c)
    {
      case 'U':
        errno = 0;
        xtmp = strtod(optarg,&endp);
        if(errno!=ERANGE && *endp=='\0' && xtmp>0.0) sim_vtau = xtmp;
        else
        {
          fprintf(stderr,"Aerosol optical depth -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'V':
        errno = 0;
        xtmp = strtod(optarg,&endp);
        if(errno!=ERANGE && *endp=='\0' && xtmp>0.0) sim_vmie = xtmp;
        else
        {
          fprintf(stderr,"Visibility -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'W':
        errno = 0;
        xtmp = strtod(optarg,&endp);
        if(errno!=ERANGE && *endp=='\0' && xtmp>=0.0) sim_wscl = xtmp;
        else
        {
          fprintf(stderr,"Water scale -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'O':
        errno = 0;
        xtmp = strtod(optarg,&endp);
        if(errno!=ERANGE && *endp=='\0' && xtmp>=0.0) sim_oscl = xtmp;
        else
        {
          fprintf(stderr,"Ozone scale -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'P':
        errno = 0;
        xtmp = strtod(optarg,&endp);
        if(errno!=ERANGE && *endp=='\0' && xtmp>=0.0) sim_cmix = xtmp;
        else
        {
          fprintf(stderr,"CO2 amount -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'A':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && ntmp>=0 && ntmp<=8) sim_iatm = ntmp;
        else
        {
          fprintf(stderr,"Atmosphere model# -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'a':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && ntmp>=0 && ntmp<=10) sim_ihaz = ntmp;
        else
        {
          fprintf(stderr,"Aerosol model# -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'k':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && ((ntmp>=0&&ntmp<=11) || (ntmp>=18&&ntmp<=19))) sim_icld = ntmp;
        else
        {
          fprintf(stderr,"Cloud model# -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'G':
        errno = 0;
        xtmp = strtod(optarg,&endp);
        if(errno!=ERANGE && *endp=='\0' && xtmp>=0.0)
        {
          sim_grho = xtmp;
          sim_salb = -1;
        }
        else
        {
          fprintf(stderr,"Ground albedo -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'g':
        errno = 0;
        xtmp = strtod(optarg,&endp);
        if(errno!=ERANGE && *endp=='\0') sim_h1 = xtmp;
        else
        {
          fprintf(stderr,"H1 altitude -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'T':
        errno = 0;
        xtmp = strtod(optarg,&endp);
        if(errno!=ERANGE && *endp=='\0') sim_h2 = xtmp;
        else
        {
          fprintf(stderr,"H2 altitude -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'w':
        errno = 0;
        xtmp = strtod(optarg,&endp);
        if(errno!=ERANGE && *endp=='\0' && xtmp>0.0) sim_xsgm = xtmp;
        else
        {
          fprintf(stderr,"X sigma -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'm':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && (ntmp==1||ntmp==5||ntmp==15)) sim_dbmp = ntmp;
        else
        {
          fprintf(stderr,"DSR Band model number -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'M':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && (ntmp==1||ntmp==5||ntmp==15)) sim_sbmp_init = ntmp;
        else
        {
          fprintf(stderr,"SSR Band model number -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 's':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && ntmp>=1 && ntmp<=2) sim_prf_mode = ntmp;
        else
        {
          fprintf(stderr,"Profile mode -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'S':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && ntmp>=1 && ntmp<=6) sim_mode = ntmp;
        else
        {
          fprintf(stderr,"Simulation mode -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'C':
        strncpy(cnt_conf,optarg,MAXLINE);
        break;
      case 'c':
        cnt_cflg++;
        break;
      case 'd':
        cnt_db++;
        break;
      case 'v':
        cnt_vb++;
        break;
      case 'h':
        cnt_hp = 1;
        break;
      case '?':
        if(optopt == '\0')
        {
          fprintf(stderr,"Invalid option : %s\n",args[this_option_optind]);
        }
        else
        {
          fprintf(stderr,"Invalid option : %c\n",optopt);
        }
        rt = -1;
        break;
      case ':':
        if(optopt == '\0')
        {
          fprintf(stderr,"Option requires an argument : %s\n",args[this_option_optind]);
        }
        else
        {
          fprintf(stderr,"Option requires an argument : %c\n",optopt);
        }
        rt = -1;
        break;
      default:
        fprintf(stderr,"?? getopt returned character code 0%o ??\n",c);
        rt = -1;
        break;
    }
  }

  if(optind < argn)
  {
    fprintf(stderr,"non-option ARGV-elements:\n");
    while(optind < argn)
    {
      fprintf(stderr,"%s\n",args[optind++]);
    }
    rt = -1;
  }

  if(cnt_hp) rt = 1;
  return rt;
}

int Usage(void)
{
  int n = 15;
  char e[MAXLINE];
  char a[MAXLINE];
  char d[MAXLINE];

  fprintf(stderr,"Usage:\n");
  fprintf(stderr,"solar_radiation -[option] (argument) -[option] (argument) ...\n");
  fprintf(stderr,"------------------------------------------------------------------------------------\n");
  fprintf(stderr,"   option         |%s|%s|%s| current\n",As(e,"",n),          As(a,"argument",n),   As(d,"default",n));
  fprintf(stderr," U -sim_vtau      |%s|%s|%s| %e\n",As(e,"Optical depth",n),  As(a,"value",n),      Af(d,SIM_VTAU,n),sim_vtau);
  fprintf(stderr," V -sim_vmie      |%s|%s|%s| %e\n",As(e,"Visibility",n),     As(a,"km",n),         Af(d,SIM_VMIE,n),sim_vmie);
  fprintf(stderr," W -sim_wscl      |%s|%s|%s| %e\n",As(e,"Water scale",n),    As(a,"value",n),      Af(d,SIM_WSCL,n),sim_wscl);
  fprintf(stderr," O -sim_oscl      |%s|%s|%s| %e\n",As(e,"Ozone scale",n),    As(a,"value",n),      Af(d,SIM_OSCL,n),sim_oscl);
  fprintf(stderr," P -sim_cmix      |%s|%s|%s| %e\n",As(e,"CO2 amount",n),     As(a,"ppmv",n),       Af(d,SIM_CMIX,n),sim_cmix);
  fprintf(stderr," A -sim_iatm      |%s|%s|%s| %d\n",As(e,"Atmosphere",n),     As(a,"#",n),          Ad(d,SIM_IATM,n),sim_iatm);
  fprintf(stderr," a -sim_ihaz      |%s|%s|%s| %d\n",As(e,"Aerosol",n),        As(a,"#",n),          Ad(d,SIM_IHAZ,n),sim_ihaz);
  fprintf(stderr," k -sim_icld      |%s|%s|%s| %d\n",As(e,"Cloud",n),          As(a,"#",n),          Ad(d,SIM_ICLD,n),sim_icld);
  fprintf(stderr," G -sim_grho      |%s|%s|%s| %f\n",As(e,"Ground albedo  ",n),As(a,"value",n),      Af(d,SIM_GRHO,n),sim_grho);
  fprintf(stderr," g -sim_h1        |%s|%s|%s| %f\n",As(e,"H1 altitude",n),    As(a,"km",n),         Af(d,SIM_H1,n),sim_h1);
  fprintf(stderr," T -sim_h2        |%s|%s|%s| %f\n",As(e,"H2 altitude",n),    As(a,"km",n),         Af(d,SIM_H2,n),sim_h2);
  fprintf(stderr," w -sim_xsgm      |%s|%s|%s| %e\n",As(e,"X sigma",n),        As(a,"value",n),      Af(d,SIM_XSGM,n),sim_xsgm);
  fprintf(stderr," m -sim_dbmp      |%s|%s|%s| %d\n",As(e,"DSR Band model",n), As(a,"1|5|15",n),     Ad(d,SIM_DBMP,n),sim_dbmp);
  fprintf(stderr," M -sim_sbmp_init |%s|%s|%s| %d\n",As(e,"SSR Band model",n), As(a,"1|5|15",n),     Ad(d,SIM_SBMP,n),sim_sbmp_init);
  fprintf(stderr," s -sim_prf_mode  |%s|%s|%s| %d\n",As(e,"Profile mode",n),   As(a,"1|2",n),        Ad(d,SIM_MODTRAN_VMIE,n),sim_prf_mode);
  fprintf(stderr," S -cnt_mode      |%s|%s|%s| %d\n",As(e,"Execution mode",n), As(a,"1~4",n),        Ad(d,CNT_MODE,n),cnt_mode);
  fprintf(stderr," C -cnt_conf      |%s|%s|%s| %s\n",As(e,"Config  file",n),   As(a,"name",n),       As(d,CNT_CONF,n),cnt_conf);
  fprintf(stderr," c -cnt_cflg      |%s|%s|%s| %d\n",As(e,"Clear   flag",n),   As(a,"nothing",n),    Ad(d,0,n),cnt_cflg);
  fprintf(stderr," d -cnt_db        |%s|%s|%s| %d\n",As(e,"Debug   mode",n),   As(a,"nothing",n),    Ad(d,0,n),cnt_db);
  fprintf(stderr," v -cnt_vb        |%s|%s|%s| %d\n",As(e,"Verbose mode",n),   As(a,"nothing",n),    Ad(d,0,n),cnt_vb);
  fprintf(stderr," h -cnt_hp        |%s|%s|%s| %d\n",As(e,"Help    mode",n),   As(a,"nothing",n),    Ad(d,0,n),1);
  fprintf(stderr,"------------------------------------------------------------------------------------\n");
  fprintf(stderr,"Execution mode: 1:MODTRAN_B 2:MODTRAN_S\n");
  fprintf(stderr,"                3:CUSTOM_B  4:CUSTOM_S\n");
  fprintf(stderr,"Config file (cnt_conf) format:\n");
  fprintf(stderr,"ssr_th_min    value        | min zenith  in deg(%.1f)\n",0.0);
  fprintf(stderr,"ssr_th_max    value        | max zenith  in deg(%.1f)\n",180.0);
  fprintf(stderr,"ssr_n_tdiv    value        | #divisions in theta(%3d)\n",10);
  fprintf(stderr,"ssr_n_pdiv    value        | #divisions in phi(%3d)\n",20);
  fprintf(stderr,"ssr_wlen      name # unit  | file name(%s),wlen column#(%d),unit in nm(%.1f)\n",NONAME,0,1.0);
  fprintf(stderr,"ssr_sbmp      name #       | file name(%s),sbmp column#(%d)\n",NONAME,0);
  fprintf(stderr,"sim_th_sun    value        | solar zenith  in deg(%.1f)\n",SIM_TH_SUN);
  fprintf(stderr,"sim_ph_sun    value        | solar azimuth in deg(%.1f)\n",SIM_PH_SUN);
  fprintf(stderr,"sim_th_los    value        | los   zenith  in deg(%.1f)\n",SIM_TH_LOS);
  fprintf(stderr,"sim_ph_los    value        | los   azimuth in deg(%.1f)\n",SIM_PH_LOS);
  fprintf(stderr,"sim_path      name         | path name(%s)\n",SIM_PATH);
  fprintf(stderr,"sim_falb      name         | file name(%s)\n",SIM_FALB);
  fprintf(stderr,"sim_salb      #            | albedo#(%d)\n",SIM_SALB);
  fprintf(stderr,"sim_prf_vscl  # value      | mol#(%d),scale factor (%f)\n",0,NAN);
  fprintf(stderr,"sim_prf_pscl  # flag       | mol#(%d),scale flag(%d)\n",0,0);
  fprintf(stderr,"sim_prf_jcharx value       | jcharx(%d)\n",SIM_IATM);
  fprintf(stderr,"prf_falt      name # unit  | file name(%s),column#(%d),unit in km(%.1f)\n",NONAME,0,1.0);
  fprintf(stderr,"prf_fpre      name # unit  | file name(%s),column#(%d),unit(%c)\n",NONAME,1,'A');
  fprintf(stderr,"prf_ftmp      name # unit  | file name(%s),column#(%d),unit(%c)\n",NONAME,1,'A');
  fprintf(stderr,"prf_fmol    # name # unit  | mol#(%d),file name(%s),column#(%d),unit(%c)\n",0,NONAME,1,'A');
  fprintf(stderr,"prf_fhaz    # name #       | haz#(%d),file name(%s),column#(%d)\n",0,NONAME,1);
  fprintf(stderr,"prf_pres_gnd  value        | Ground pressure(%.1f)\n",NAN);
  fprintf(stderr,"prf_temp_gnd  value        | Ground temperature(%.1f)\n",NAN);
  fprintf(stderr,"cld_thik      value        | Cloud thickness(%.1f)\n",CLD_THIK);
  fprintf(stderr,"cld_alt       value        | Cloud altitude(%.1f)\n",CLD_ALT);
  fprintf(stderr,"cld_ext       value        | Cloud extinction(%.1f)\n",CLD_EXT);
  for(n=0; n<cnt_n_own_format; n++)
  {
    fprintf(stderr,"%s",cnt_own_format[n]);
  }
  if(cnt_n_cmnt > 0)
  {
    fprintf(stderr,"<Read from %s>\n",cnt_conf);
    for(n=0; n<cnt_n_cmnt; n++)
    {
      fprintf(stderr,"%s",cnt_cmnt[n]);
    }
  }
  fprintf(stderr,"------------------------------------------------------------------------------------\n");
  fprintf(stderr,"Profile file (prf_fpre,prf_ftmp,prf_fmol,prf_fhaz) format: z v\n");
  fprintf(stderr,"    z  ... Altitude [km]\n");
  fprintf(stderr,"    v  ... Value\n");

  return 0;
}
