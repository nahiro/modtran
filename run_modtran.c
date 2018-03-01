/*********************************************************/
/* run_modtran                                           */
/* Author: N.Manago Apr,30,2008                          */
/* $Revision: 334 $                                      */
/* $Date: 2016-09-26 18:23:32 +0900 (Mon, 26 Sep 2016) $ */
/*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <math.h>
#include <bits/nan.h>
#include <complex.h>
#include <time.h>
#include <sys/time.h>
#include "aeros_profile.h"
#include "strutil.h"

// Mathematical constants
#define	PI				3.141592653589793	// PI
#define	PI2				6.283185307179586	// 2*PI
#define	PI4_3				4.1887902047863905	// 4*PI/3
#define	M_LNA				2.3025850929940459	// Ln(10)
#define	M_1_LNA				0.43429448190325176	// 1/Ln(10)
#define	R_TO_D				57.29577951308232	// 180/PI rad -> deg
#define	D_TO_R				1.745329251994329e-02	// PI/180 deg -> rad
#define	ZERO				0.0			// 0
// Physical constants
#define	SEC_DAY				86400
// Common constants
#define	NONAME				"NONE"			// No name
#define	NODATA				0			// No data
#define	INVALID				-1			// Invalid value
#define	MAXITEM				25			// Max #entries in a line
#define	MAXLINE				256			// Max #chars in a line
#define	EPSILON				1.0e-14			// A small number
#define	DELTA				1.0e-12			// A small number
// Constants for Direct Solar radiation
#define	DSR_MAXWLEN			100000			// Max #wavelengths for DSR
// Constants for Scattered Solar Radiation
#define	SSR_MAXWLEN			100000			// Max #wavelengths for SSR
// Constants for Simulation
#define SIM_MODTRAN                     'M'                     // MODTRAN band model
#define SIM_SPEED                       'M'                     // Simulation speed
#define	SIM_N_PHAS_WLEN			11			// #Wavelengths for phase function
#define	SIM_MAX_PHASE_WLEN		15			// Max #wavelengths for phase function
#define	SIM_N_PHAS_ANGL			50			// #Angles
#define	SIM_MAX_PHASE_ANGL		51			// Max #angles for phase function
#define	SIM_MAXNDIR			1000			// Max #directions
#define	SIM_MAXDATA			50000			// #Data
#define	SIM_MAXCONV			10000			// #Data
#define	SIM_NCOL			8			// #Columns
#define	SIM_NINT			2			// #Ints
#define	SIM_NDBL			6			// #Doubles
#define	SIM_NSGL			6			// #Columns
#define	SIM_FLAG			2			// Simulation flag
#define	SIM_ITYPE			3			// LOS geometric type
#define	SIM_IEMSCT			2			// MODTRAN execution mode
#define	SIM_PRNT			1			// NOPRNT flag
#define	SIM_IATM			2			// Atmosphere modle number
#define	SIM_ISEA			1			// Season modle number
#define	SIM_IVUL			0			// Volcanic modle number
#define	SIM_MULT			1			// Multiple scattering flag
#define	SIM_SALB			-1			// Spectral albedo#
#define	SIM_IDAY			93			// Day of year
#define	SIM_IHAZ			1			// Aerosol model#
#define	SIM_ICLD			0			// Cloud model#
#define	SIM_DBMP			1			// DSR Band model number
#define	SIM_SBMP			1			// SSR Band model number
#define	SIM_NSAM			4			// #Sampling
#define	SIM_MLFLX			-1			// #Levels for spectral fluxes
#define	SIM_CFLG			0			// Correction flag
#define	SIM_DFLG			1			// DISORT flag
#define	SIM_NSTR			16			// Number of streams for DISORT
#define	SIM_WFLG			0			// H2OAER flag
#define	SIM_MDEF			0			// CARD2C2X read flag
#define	SIM_IM				0			// CARD2C read flag
#define	SIM_2C				0			// CARD2C flag
#define	SIM_CMIX			365.0			// CO2 mixing ratio in ppmv
#define	SIM_SOLCON			NAN			// Solar constant
#define	SIM_NPAR			37			// #Parameters
#define	SIM_VTAU			NAN			// Aerosol optical depth
#define	SIM_VMIE			20.0			// Visibility in km
#define	SIM_WSCL			1.0			// Water scale
#define	SIM_OSCL			1.0			// Ozone scale
#define	SIM_GRHO			0.3			// Ground albedo
#define	SIM_H1				0.0			// H1 altitude
#define	SIM_H2				0.0			// H2 altitude
#define	SIM_RANGE			0.0			// Range
#define	SIM_RO				0.0			// Radius of the earth in km
#define	SIM_XMGN			16.0			// X margin
#define	SIM_XSGM			4.0			// X sigma
#define	SIM_WSGM			20.0			// X width
#define	SIM_YUNI			10.0			// Y unit
#define	SIM_DMIN			1.0e-12			// Min parameter difference
#define	SIM_DMAX			1.0e4			// Max wavelength difference in nm
#define	SIM_WLEN_MIN			350.0			// Min wavelength in nm
#define	SIM_WLEN_MAX			1050.0			// Max wavelength in nm
#define	SIM_TH_SUN			20.0			// Solar zenith
#define	SIM_PH_SUN			0.0			// Solar azimuth (N=0,CW)
#define	SIM_TH_LOS			0.0			// LOS zenith
#define	SIM_PH_LOS			0.0			// LOS azimuth (N=0,CW)
#define	SIM_MODE_DSR_MODTRAN_BO		1			// Simulation mode
#define	SIM_MODE_DSR_CUSTOM_BO		2			// Simulation mode
#define	SIM_MODE_SSR_MODTRAN_SO		3			// Simulation mode
#define	SIM_MODE_SSR_MODTRAN_BO		4			// Simulation mode
#define	SIM_MODE_SSR_CUSTOM_SO		5			// Simulation mode
#define	SIM_MODE_SSR_CUSTOM_BO		6			// Simulation mode
#define	SIM_MODE_TRN_MODTRAN_BO		7			// Simulation mode
#define	SIM_MODE_TRN_CUSTOM_BO		8			// Simulation mode
#define	SIM_PRF_CUSTOM			1			// Simulation mode
#define	SIM_PRF_MODTRAN			2			// Simulation mode
#define	SIM_FALB			"DATA/spec_alb.dat"	// Spectral albedo file
// Constants for MODTRAN
#define	MOD_V4				4000			// MODTRAN version 4
#define	MOD_V5				5000			// MODTRAN version 5
#define	MOD_PATH_V4			"/usr/local/Mod4v2r1"	// MODTRAN4 path
#define	MOD_PATH_V5			"/usr/local/Mod5.2.0.0"	// MODTRAN5 path
// Constants for Profile
#define	PRF_HAZ_NALT			34
#define	PRF_NALT			36
#define	PRF_MAXNALT			1000
#define	PRF_NMOL			12
#define	PRF_NMOLX			13
#define	PRF_NMOLY			16
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
#define	MIE_FLAG_SIZE			4
#define	MIE_FUNC_FILE			'F'
#define	MIE_FUNC_LOGNORMAL		'L'
#define	MIE_FUNC_EFF_LOGNORMAL		'E'
#define	MIE_FUNC_GAMMA			'G'
#define	MIE_XTYPE_R			'R'
#define	MIE_XTYPE_LOGR			'L'
#define	MIE_XTYPE_LNR			'N'
#define	MIE_YTYPE_NUMBER		'N'
#define	MIE_YTYPE_VOLUME		'V'
#define	MIE_SHAPE_SPHERE		'S'
#define	MIE_SHAPE_SPHEROID		'T'
#define	MIE_N_WLEN			11			// #Wavelengths
#define	MIE_N_ANGL			121			// #Angles
#define	MIE_MAXDATA			2000			// Max #data
#define	MIE_DMAX			1000000			// Max #data for size dist
#define	MIE_IMIN			0			// Min line#
#define	MIE_IMAX			1000000000		// Min line#
#define	MIE_REAL_NUM			1			// Ref. index (real) column#
#define	MIE_IMAG_NUM			2			// Ref. index (imag) column#
#define	MIE_WLEN_REF			550.0			// Reference wavelength in nm
#define	MIE_WLEN_MIN			250.0			// Min wavelength in nm
#define	MIE_WLEN_MAX			1500.0			// Max wavelength in nm
#define	MIE_ANGL_MIN			0.0			// Min angle in degree
#define	MIE_ANGL_MAX			180.0			// Max angle in degree
#define	MIE_MAXCOMP			10			// Max #components
#define	MIE_TRUE			1			// True  value
#define	MIE_FALSE			0			// False value
#define	MIE_XMAX			2.0e4			// Max size parameter
#define	MIE_NSTP			10000			// Log10(R) #steps
#define	MIE_RMIN			NAN			// R min in um
#define	MIE_RMAX			NAN			// R max in um
#define	MIE_LRAD_MIN			-4.0			// Min Log10(R)
#define	MIE_LRAD_MAX			+3.0			// Max Log10(R)
#define	MIE_WSGM			5.0			// Log10(R) width in sigma
#define	MIE_OUT0			"mie_out0.dat"		// Out. file 0
#define	MIE_OUT1			"mie_out1.dat"		// Out. file 1
#define	MIE_OUT2			"mie_out2.dat"		// Out. file 2
// Constants for T-matrix calculation
#define	TMX_NPN1			200
#define	TMX_NPNG1			(3*TMX_NPN1)
#define	TMX_NPNG2			(2*TMX_NPNG1)
#define	TMX_NPN2			(2*TMX_NPN1)
#define	TMX_NPL				(TMX_NPN2+1)
#define	TMX_NPN3			(TMX_NPN1+1)
#define	TMX_NPN4			(TMX_NPN1-25)
#define	TMX_NPN5			(2*TMX_NPN4)
#define	TMX_NPN6			(TMX_NPN4+1)
#define	TMX_NPL1			(TMX_NPN5+1)
#define	TMX_NAXMAX			10
#define	TMX_NAMAX			500
#define	TMX_NAFMAX			(TMX_NAMAX*6)
#define	TMX_NALMAX			(TMX_NAXMAX*TMX_NPL)
#define	TMX_NARMAX			(TMX_NAXMAX*TMX_NAFMAX)
#define	TMX_NDIS			0			// Distribution#
#define	TMX_NANG			361			// #Angles
#define	TMX_NDGS			2			// Control #divisions
#define	TMX_KMAX			100			// #Quad points
#define	TMX_SHAP			-1			// Shape#
#define	TMX_REPS			1.000001		// Aspect ratio or deformation parameter
#define	TMX_DELT			0.001			// Accuracy of the computations
#define	TMX_RMAX			8.0			// Max radius in um
// Constants for Cloud calculation
#define	CLD_THIK			0.0			// Cloud thickness
#define	CLD_ALT				0.0			// Cloud altitude
#define	CLD_EXT				0.0			// Cloud extinction
// Constants for upper atmosphere
#define	UPP_N_WLEN			6
#define	UPP_N_ANGL			34
#define	UPP_N_PHAS			204
#define	UPP_N_RHUM			4
#define	UPP_RH				50.0			// RH of upper atmosphere in %
// Constants for control
#define	CNT_CONF			NONAME
#define	CNT_MAXCMNT			30
#define	CNT_MAXFORM			30
#define	CNT_MAXNOPT			100			// Max #options
// Functions
#define	MAX(a,b)			((b)>(a)?(b):(a))
#define	INTERP(x,x1,x2,y1,y2)		(y1+(y2-y1)/(x2-x1)*(x-x1))

extern void miev0_();
extern void tmatrix_();
extern float aerprf_(int*,float*,int*,int*,int*);
int Init(void);
void Finish(void);
int GetOpt(int argn,char **args);
int Usage(void);
int MieInit(void);
int MieTable(int n1,int n2);
int MieCalc(void);
int MixComp(void);
int ReadMie(int iaer);
int SetMie1(int iaer);
int SetMie2(int iaer);
int MiePrintout(void);
int upper_pfunc(int iaer);
int PrfInitMolecule(int imod);
int PrfInitAerosol(int ihaz,int isea,int ivul,double vis);
int PrfGround(void);
int PrfScaleMolecule(void);
int PrfScaleAerosol(double t);
int PrfCalcAOD(void);
int (*WriteTape5)(double *par,int mode);
int (*RunModtran)(void);
int ReadPlt_S(const double *par,double *y);
int ReadPlt_B(const double *par,int n,const double *x,double *y);
int Convolute(double xmin,double xmax,double xstp,double xsgm,double wsgm,
              int ninp,const double *xinp,const double *yinp,
              int nout,double *xout,double *yout);
double Gauss(double g,double f0,double f);
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
int WaveSelect(double *w);
int AnglSelect(double *a);
int ReadComp(int iaer,int size,double *wcom,double *rmod,double *lsgm,double **refr,double **refi);
int Read1A(char *s,int size,int cx,double ux,int (*func)(double*),double *x);
int Read2A(char *s,int size,int cx,int cy,double ux,double uy,int (*func)(double*),double *x,double *y);
int Read3A(char *s,int size,int cx,int cy,int cz,double ux,double uy,double uz,
           int (*func)(double*),double *x,double *y,double *z);
int Read4A(char *s,int size,int cx,int cy,int cz,int cw,double ux,double uy,double uz,double uw,
           int (*func)(double*),double *x,double *y,double *z,double *w);
int Read1P(char *s,int size,int cx,double ux,int (*func)(double*),double **x);
int Read2P(char *s,int size,int cx,int cy,double ux,double uy,int (*func)(double*),double **x,double **y);
int Read3P(char *s,int size,int cx,int cy,int cz,double ux,double uy,double uz,
           int (*func)(double*),double **x,double **y,double **z);
int Read4P(char *s,int size,int cx,int cy,int cz,int cw,double ux,double uy,double uz,double uw,
           int (*func)(double*),double **x,double **y,double **z,double **w);
int ReadDC(char *s,int np,int size,const int *c,const double *u,int (*func)(double*),double **d);
int ReadDA(char *s,int np,int size,const int *c,const double *u,double **d);
int ReadNA(char *s,int size,int cx,int *x);
int ReadNP(char *s,int size,int cx,int **x);
int ReadIA(char *s,int size,int np,const int *c,int **d);

// Parameters for Direct Solar radiation
int	dsr_n_wlen			= NODATA;		// #Wavelengths
double	*dsr_wlen			= NULL;			// Wavelength
double	*dsr_dsim			= NULL;			// Simulation data
// Parameters for Scattered Solar Radiation
int	ssr_n_wlen			= NODATA;		// #Wavelengths
int	ssr_n_sbmp			= NODATA;		// #Models
int	ssr_n_nsam			= NODATA;		// #Numbers
int	*ssr_sbmp			= NULL;			// SSR Band model number
int	*ssr_nsam			= NULL;			// #Sampling
double	*ssr_wlen			= NULL;			// Wavelength
double	*ssr_dsim			= NULL;			// Simulation data
// Parameters for Simulation
char    sim_modtran                     = SIM_MODTRAN;          // MODTRAN band model
char    sim_speed                       = SIM_SPEED;            // Simulation speed
int	sim_mode			= SIM_MODE_DSR_MODTRAN_BO; // Simulation mode
int	sim_n_aers_wlen[4]		= {NODATA,NODATA,NODATA,NODATA}; // #Wavelengths for cext, etc.
int	sim_n_phas_wlen			= SIM_N_PHAS_WLEN;	// #Wavelengths for phase function
int	sim_n_phas_angl			= SIM_N_PHAS_ANGL;	// #Angles
int	sim_n_dir			= NODATA;		// #Directions
int	sim_flag			= SIM_FLAG;		// Simulation flag
int	sim_itype			= SIM_ITYPE;		// LOS geometric type
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
int	sim_nsam_init			= SIM_NSAM;		// #Sampling
int	sim_mlflx			= SIM_MLFLX;		// #Levels for spectral fluxes
int	sim_prf_mode			= SIM_PRF_MODTRAN;	// Simulation mode
int	sim_cflg			= SIM_CFLG;		// Correction flag
int	sim_dflg			= SIM_DFLG;		// DISORT flag
int	sim_nstr			= SIM_NSTR;		// Number of streams for DISORT
int	sim_wflg			= SIM_WFLG;		// H2OAER flag
int	sim_mdef			= SIM_MDEF;		// CARD2C2X read flag
int	sim_im				= SIM_IM;		// CARD2C read flag
int	sim_2c				= SIM_2C;		// CARD2C flag
double	sim_cmix			= SIM_CMIX;		// CO2 mixing ratio in ppmv
double	sim_solcon			= SIM_SOLCON;		// Solar constant
double	sim_vtau			= SIM_VTAU;		// Aerosol optical depth
double	sim_vmie			= SIM_VMIE;		// Visibility in km
double	sim_wscl			= SIM_WSCL;		// Water scale
double	sim_oscl			= SIM_OSCL;		// Ozone scale
double	sim_grho			= SIM_GRHO;		// Ground albedo
double	sim_h1				= SIM_H1;		// H1 altitude
double	sim_h2				= SIM_H2;		// H2 altitude
double	sim_range			= SIM_RANGE;		// Range
double	sim_ro				= SIM_RO;		// Radius of the earth in km
double	sim_xmgn			= SIM_XMGN;		// X margin
double	sim_xsgm			= SIM_XSGM;		// X sigma
double	sim_wsgm			= SIM_WSGM;		// X width in sigma
double	sim_yuni			= SIM_YUNI;		// Y unit
double	sim_dmax			= SIM_DMAX;		// Max wavelength difference in nm
double	sim_phas_wlen[SIM_MAX_PHAS_WLEN]=
{
  300.0, 337.1, 400.0,  488.0,  514.5, 550.0,
  632.8, 694.3, 860.0, 1060.0, 1300.0,
};
double	sim_phas_angl[SIM_MAX_PHAS_ANGL]=
{
    0.0,   0.5,   1.0,   1.5,   2.0,   2.5,   3.0,   3.5,
    4.0,   4.5,   5.0,   6.0,   7.0,   8.0,   9.0,  10.0,
   12.0,  14.0,  16.0,  18.0,  20.0,  24.0,  28.0,  32.0,
   36.0,  40.0,  50.0,  60.0,  70.0,  80.0,  90.0,  95.0,
  100.0, 105.0, 110.0, 115.0, 120.0, 125.0, 130.0, 135.0,
  140.0, 145.0, 150.0, 155.0, 160.0, 164.0, 168.0, 172.0,
  176.0, 180.0,
};
double	*sim_dir[4]			= {NULL,NULL,NULL,NULL};
double	*sim_aers_wlen_um[4]		= {NULL,NULL,NULL,NULL};
double	*sim_aers_cext[4]		= {NULL,NULL,NULL,NULL};
double	*sim_aers_cabs[4]		= {NULL,NULL,NULL,NULL};
double	*sim_aers_asym[4]		= {NULL,NULL,NULL,NULL};
double	*sim_phas_wlen_um		= NULL;
double	*sim_aers_phas[4]		= {NULL,NULL,NULL,NULL};
double	sim_wlen_min			= SIM_WLEN_MIN;		// Min wavelength in nm
double	sim_wlen_max			= SIM_WLEN_MAX;		// Max wavelength in nm
double	sim_th_sun			= SIM_TH_SUN;		// Solar zenith
double	sim_ph_sun			= SIM_PH_SUN;		// Solar azimuth (N=0,CW)
double	sim_th_los			= SIM_TH_LOS;		// LOS zenith
double	sim_ph_los			= SIM_PH_LOS;		// LOS azimuth (N=0,CW)
char	sim_fdir[MAXLINE]		= NONAME;		// Solar/LOS direction file
int	sim_prf_pscl[PRF_NMOL];
int	sim_prf_psclx[PRF_NMOLX];
int	sim_prf_pscly[PRF_NMOLY];
double	sim_prf_pres[PRF_MAXNALT];
double	sim_prf_temp[PRF_MAXNALT];
double	sim_prf_wmol[PRF_NMOL][PRF_MAXNALT];
double	sim_prf_xmol[PRF_NMOLX][PRF_MAXNALT];
double	sim_prf_ymol[PRF_NMOLY][PRF_MAXNALT];
double	sim_prf_haze[PRF_NHAZ][PRF_MAXNALT];
double	sim_prf_vscl[PRF_NMOL];
double	sim_prf_vsclx[PRF_NMOLX];
double	sim_prf_vscly[PRF_NMOLY];
char	sim_prf_jchar[PRF_NPAR];
char	sim_prf_jcharx;
char	sim_prf_jchary;
char	sim_fsun[MAXLINE]		= NONAME;		// TOA solar irradiance file
char	sim_falb[MAXLINE]		= SIM_FALB;		// Spectral albedo file
// Parameters for MODTRAN
char	mod_path_v4[MAXLINE]		= MOD_PATH_V4;		// MODTRAN4 path
char	mod_path_v5[MAXLINE]		= MOD_PATH_V5;		// MODTRAN5 path
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
double	prf_xmol[PRF_NMOLX][PRF_MAXNALT];
double	prf_ymol[PRF_NMOLY][PRF_MAXNALT];
double	prf_haze[PRF_NHAZ][PRF_MAXNALT];
double	prf_pres_gnd			= NAN;
double	prf_temp_gnd			= NAN;
double	prf_aod				= 0.0;
double	prf_aod_lo			= 0.0;
double	prf_aod_hi			= 0.0;
char	prf_jchar[PRF_NPAR];
char	prf_jcharx;
char	prf_jchary;
char	prf_wmol_nam[PRF_NMOL][MAXLINE]	=
{
  "H2O", "CO2", "O3", "N2O", "CO", "CH4",
  "O2", "NO", "SO2", "NO2", "NH3", "HNO3",
};
char	prf_xmol_nam[PRF_NMOLX][MAXLINE]=
{
  "CFC11","CFC12","CFC13","CFC14","CFC22",
  "CFC113","CFC114","CFC115",
  "ClONO2","HNO4","CHCl2F","CCl4","N2O5",
};
char	prf_ymon_nam[PRF_NMOLY][MAXLINE]=
{
  "OH","HF","HCl","HBr","HI","ClO","OCS","H2CO",
  "HOCl","N2","HCN","CH3Cl","H2O2","C2H2","C2H6","PH3",
};
// Parameters for Mie calculation
int	mie_iref			= -1;			// Ref wave line#
int	mie_n_wlen			= MIE_N_WLEN;		// #Wavelengths (output)
int	mie_n_angl			= MIE_N_ANGL;		// #Angles
int	mie_n_comp			= NODATA;		// #Components
int	mie_n_step			= MIE_NSTP;		// Log10(R) #steps
int	mie_n_size[MIE_MAXCOMP];				// #Size parameters
char	mie_size_func[MIE_MAXCOMP];				// Size distribution function
char	mie_size_xtype[MIE_MAXCOMP];				// Size distribution X type
char	mie_size_ytype[MIE_MAXCOMP];				// Size distribution Y type
char	mie_size_shape[MIE_MAXCOMP];				// Particle shape
double	mie_wlen_ref			= MIE_WLEN_REF;		// Reference wavelength
double	mie_wlen_min			= MIE_WLEN_MIN;		// Min wavelength in nm
double	mie_wlen_max			= MIE_WLEN_MAX;		// Max wavelength in nm
double	mie_wlen[MIE_MAXDATA]		=
{
  300.0, 337.1, 400.0,  488.0,  514.5, 550.0,
  632.8, 694.3, 860.0, 1060.0, 1300.0,
};
double	*mie_wlen_um			= NULL;
double	*mie_aext			= NULL;
double	*mie_asca			= NULL;
double	*mie_asym			= NULL;
double	mie_angl_min			= MIE_ANGL_MIN;		// Min angle in degree
double	mie_angl_max			= MIE_ANGL_MAX;		// Max angle in degree
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
double	*mie_phas_com[MIE_MAXCOMP];				// Averaged phase function
double	*mie_aext_tab[MIE_MAXCOMP];				// Extinction coefficient
double	*mie_asca_tab[MIE_MAXCOMP];				// Scattering coefficient
double	*mie_phs1_tab[MIE_MAXCOMP];				// Phase function 1
double	*mie_phs2_tab[MIE_MAXCOMP];				// Phase function 2
double	mie_mixr_com[MIE_MAXCOMP];				// Number mixing ratio
double	mie_wcom_com[MIE_MAXCOMP];				// Weight
double	mie_vcom_com[MIE_MAXCOMP];				// Volume
double	mie_rmod_com[MIE_MAXCOMP];				// Mode radius in um
double	mie_lmod_com[MIE_MAXCOMP];				// Log10(R in um)
double	mie_lsgm_com[MIE_MAXCOMP];				// Sigma Log10(R in um)
double	mie_reps_com[MIE_MAXCOMP];				// Aspect ratio
double *mie_xval_com[MIE_MAXCOMP];				// R, Log10(R), or Ln(R)
double *mie_yval_com[MIE_MAXCOMP];				// dN(R) or dV(R)
double	*mie_lrad_min[MIE_MAXCOMP];				// Log10(R) min
double	*mie_lrad_max[MIE_MAXCOMP];				// Log10(R) max
double	*mie_lrad_stp[MIE_MAXCOMP];				// Log10(R) step
double	mie_rmin			= MIE_RMIN;		// R min in um
double	mie_rmax			= MIE_RMAX;		// R max in um
double	mie_wsgm			= MIE_WSGM;		// Log10(R) width in sigma
char	mie_aers_cmp[4][MAXLINE]	= {"\0","\0","\0","\0"};// Parameters for ReadComp
char	mie_out0[MAXLINE]		= MIE_OUT0;		// Output file 0
char	mie_out1[MAXLINE]		= MIE_OUT1;		// Output file 1
char	mie_out2[MAXLINE]		= MIE_OUT2;		// Output file 2
char	mie_aers_inp1[4][MAXLINE]	= {"\0","\0","\0","\0"};// Input file 1
char	mie_aers_inp2[4][MAXLINE]	= {"\0","\0","\0","\0"};// Input file 2
// Parameters for T-matrix calculation
int	tmx_ndis			= TMX_NDIS;		// Distribution#
int	tmx_nang			= TMX_NANG;		// #Angles
int	tmx_ndgs			= TMX_NDGS;		// Control #divisions
int	tmx_kmax			= TMX_KMAX;		// #Quad points
int	tmx_shap[MIE_MAXCOMP];					// Shape#
double	tmx_reps[MIE_MAXCOMP];					// Aspect ratio or deformation parameter
double	tmx_delt			= TMX_DELT;		// Accuracy of the computations
// Parameters for Cloud calculation
double	cld_thik			= CLD_THIK;		// Cloud thickness
double	cld_alt				= CLD_ALT;		// Cloud altitude
double	cld_ext				= CLD_EXT;		// Cloud extinction
// Parameters for Upper Atmosphere
double	upp_wlen[UPP_N_WLEN] = {200.0, 300.0, 550.0, 694.3, 1060.0, 1536.0};
double	upp_angl[UPP_N_ANGL] =
{
    0.0,   2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  16.0,
   20.0,  24.0,  28.0,  32.0,  36.0,  40.0,  50.0,  60.0,
   70.0,  80.0,  90.0, 100.0, 110.0, 120.0, 125.0, 130.0,
  135.0, 140.0, 145.0, 150.0, 155.0, 160.0, 165.0, 170.0,
  175.0, 180.0,
};
double	upp_rhum[UPP_N_RHUM] = {0.0, 70.0, 80.0, 99.0};
double	upp_rh				= UPP_RH;
double	upp_tropo_phas[UPP_N_RHUM][UPP_N_PHAS] =
{
  {// RH=0%
    // PHSFNC_29
    2.72707, 2.52017, 2.15725, 1.81463, 1.52555, 1.28665, 1.08932, 0.78963,
    0.58023, 0.43161, 0.32473, 0.24695, 0.18977, 0.14727, 0.08150, 0.04783,
    0.02980, 0.01981, 0.01414, 0.01091, 0.00911, 0.00819, 0.00796, 0.00784,
    0.00781, 0.00787, 0.00798, 0.00812, 0.00825, 0.00831, 0.00830, 0.00829,
    0.00847, 0.00868,
    // PHSFNC_16
    1.52473, 1.44645, 1.30800, 1.16736, 1.03716, 0.92088, 0.81802, 0.64725,
    0.51405, 0.40964, 0.32742, 0.26255, 0.21129, 0.17075, 0.10236, 0.06361,
    0.04130, 0.02819, 0.02041, 0.01583, 0.01330, 0.01222, 0.01213, 0.01235,
    0.01288, 0.01373, 0.01491, 0.01635, 0.01784, 0.01906, 0.02003, 0.02225,
    0.02715, 0.03082,
    // PHSFNC_32
    0.79503, 0.78714, 0.76605, 0.73601, 0.70017, 0.66092, 0.61994, 0.53736,
    0.45896, 0.38796, 0.32563, 0.27208, 0.22676, 0.18879, 0.11979, 0.07727,
    0.05125, 0.03541, 0.02586, 0.02025, 0.01717, 0.01576, 0.01552, 0.01551,
    0.01571, 0.01607, 0.01656, 0.01710, 0.01764, 0.01814, 0.01872, 0.01965,
    0.02093, 0.02164,
    // PHSFNC_32
    0.79503, 0.78714, 0.76605, 0.73601, 0.70017, 0.66092, 0.61994, 0.53736,
    0.45896, 0.38796, 0.32563, 0.27208, 0.22676, 0.18879, 0.11979, 0.07727,
    0.05125, 0.03541, 0.02586, 0.02025, 0.01717, 0.01576, 0.01552, 0.01551,
    0.01571, 0.01607, 0.01656, 0.01710, 0.01764, 0.01814, 0.01872, 0.01965,
    0.02093, 0.02164,
    // PHSFNC_36
    0.72999, 0.72086, 0.69632, 0.66227, 0.62369, 0.58366, 0.54390, 0.46850,
    0.40091, 0.34178, 0.29081, 0.24724, 0.21025, 0.17893, 0.12047, 0.08253,
    0.05810, 0.04253, 0.03278, 0.02688, 0.02352, 0.02183, 0.02142, 0.02122,
    0.02118, 0.02127, 0.02146, 0.02171, 0.02202, 0.02235, 0.02273, 0.02314,
    0.02351, 0.02367,
    // PHSFNC_36
    0.72999, 0.72086, 0.69632, 0.66227, 0.62369, 0.58366, 0.54390, 0.46850,
    0.40091, 0.34178, 0.29081, 0.24724, 0.21025, 0.17893, 0.12047, 0.08253,
    0.05810, 0.04253, 0.03278, 0.02688, 0.02352, 0.02183, 0.02142, 0.02122,
    0.02118, 0.02127, 0.02146, 0.02171, 0.02202, 0.02235, 0.02273, 0.02314,
    0.02351, 0.02367,
  },
  {// RH=70%
    // PHSFNC_29
    2.72707, 2.52017, 2.15725, 1.81463, 1.52555, 1.28665, 1.08932, 0.78963,
    0.58023, 0.43161, 0.32473, 0.24695, 0.18977, 0.14727, 0.08150, 0.04783,
    0.02980, 0.01981, 0.01414, 0.01091, 0.00911, 0.00819, 0.00796, 0.00784,
    0.00781, 0.00787, 0.00798, 0.00812, 0.00825, 0.00831, 0.00830, 0.00829,
    0.00847, 0.00868,
    // PHSFNC_16
    1.52473, 1.44645, 1.30800, 1.16736, 1.03716, 0.92088, 0.81802, 0.64725,
    0.51405, 0.40964, 0.32742, 0.26255, 0.21129, 0.17075, 0.10236, 0.06361,
    0.04130, 0.02819, 0.02041, 0.01583, 0.01330, 0.01222, 0.01213, 0.01235,
    0.01288, 0.01373, 0.01491, 0.01635, 0.01784, 0.01906, 0.02003, 0.02225,
    0.02715, 0.03082,
    // PHSFNC_32
    0.79503, 0.78714, 0.76605, 0.73601, 0.70017, 0.66092, 0.61994, 0.53736,
    0.45896, 0.38796, 0.32563, 0.27208, 0.22676, 0.18879, 0.11979, 0.07727,
    0.05125, 0.03541, 0.02586, 0.02025, 0.01717, 0.01576, 0.01552, 0.01551,
    0.01571, 0.01607, 0.01656, 0.01710, 0.01764, 0.01814, 0.01872, 0.01965,
    0.02093, 0.02164,
    // PHSFNC_32
    0.79503, 0.78714, 0.76605, 0.73601, 0.70017, 0.66092, 0.61994, 0.53736,
    0.45896, 0.38796, 0.32563, 0.27208, 0.22676, 0.18879, 0.11979, 0.07727,
    0.05125, 0.03541, 0.02586, 0.02025, 0.01717, 0.01576, 0.01552, 0.01551,
    0.01571, 0.01607, 0.01656, 0.01710, 0.01764, 0.01814, 0.01872, 0.01965,
    0.02093, 0.02164,
    // PHSFNC_32
    0.79503, 0.78714, 0.76605, 0.73601, 0.70017, 0.66092, 0.61994, 0.53736,
    0.45896, 0.38796, 0.32563, 0.27208, 0.22676, 0.18879, 0.11979, 0.07727,
    0.05125, 0.03541, 0.02586, 0.02025, 0.01717, 0.01576, 0.01552, 0.01551,
    0.01571, 0.01607, 0.01656, 0.01710, 0.01764, 0.01814, 0.01872, 0.01965,
    0.02093, 0.02164,
    // PHSFNC_36
    0.72999, 0.72086, 0.69632, 0.66227, 0.62369, 0.58366, 0.54390, 0.46850,
    0.40091, 0.34178, 0.29081, 0.24724, 0.21025, 0.17893, 0.12047, 0.08253,
    0.05810, 0.04253, 0.03278, 0.02688, 0.02352, 0.02183, 0.02142, 0.02122,
    0.02118, 0.02127, 0.02146, 0.02171, 0.02202, 0.02235, 0.02273, 0.02314,
    0.02351, 0.02367,
  },
  {// RH=80%
    // PHSFNC_29
    2.72707, 2.52017, 2.15725, 1.81463, 1.52555, 1.28665, 1.08932, 0.78963,
    0.58023, 0.43161, 0.32473, 0.24695, 0.18977, 0.14727, 0.08150, 0.04783,
    0.02980, 0.01981, 0.01414, 0.01091, 0.00911, 0.00819, 0.00796, 0.00784,
    0.00781, 0.00787, 0.00798, 0.00812, 0.00825, 0.00831, 0.00830, 0.00829,
    0.00847, 0.00868,
    // PHSFNC_28
    1.99262, 1.84586, 1.62814, 1.42586, 1.24733, 1.09157, 0.95572, 0.73435,
    0.56567, 0.43699, 0.33874, 0.26371, 0.20635, 0.16235, 0.09181, 0.05440,
    0.03397, 0.02253, 0.01601, 0.01230, 0.01031, 0.00946, 0.00938, 0.00951,
    0.00985, 0.01043, 0.01123, 0.01222, 0.01328, 0.01405, 0.01405, 0.01340,
    0.01391, 0.01521,
    // PHSFNC_37
    1.15457, 1.13481, 1.08637, 1.02208, 0.95035, 0.87581, 0.80158, 0.66139,
    0.53812, 0.43402, 0.34834, 0.27905, 0.22360, 0.17950, 0.10538, 0.06398,
    0.04056, 0.02712, 0.01935, 0.01490, 0.01250, 0.01143, 0.01126, 0.01130,
    0.01153, 0.01191, 0.01242, 0.01301, 0.01357, 0.01399, 0.01422, 0.01450,
    0.01526, 0.01585,
    // PHSFNC_37
    1.15457, 1.13481, 1.08637, 1.02208, 0.95035, 0.87581, 0.80158, 0.66139,
    0.53812, 0.43402, 0.34834, 0.27905, 0.22360, 0.17950, 0.10538, 0.06398,
    0.04056, 0.02712, 0.01935, 0.01490, 0.01250, 0.01143, 0.01126, 0.01130,
    0.01153, 0.01191, 0.01242, 0.01301, 0.01357, 0.01399, 0.01422, 0.01450,
    0.01526, 0.01585,
    // PHSFNC_32
    0.79503, 0.78714, 0.76605, 0.73601, 0.70017, 0.66092, 0.61994, 0.53736,
    0.45896, 0.38796, 0.32563, 0.27208, 0.22676, 0.18879, 0.11979, 0.07727,
    0.05125, 0.03541, 0.02586, 0.02025, 0.01717, 0.01576, 0.01552, 0.01551,
    0.01571, 0.01607, 0.01656, 0.01710, 0.01764, 0.01814, 0.01872, 0.01965,
    0.02093, 0.02164,
    // PHSFNC_36
    0.72999, 0.72086, 0.69632, 0.66227, 0.62369, 0.58366, 0.54390, 0.46850,
    0.40091, 0.34178, 0.29081, 0.24724, 0.21025, 0.17893, 0.12047, 0.08253,
    0.05810, 0.04253, 0.03278, 0.02688, 0.02352, 0.02183, 0.02142, 0.02122,
    0.02118, 0.02127, 0.02146, 0.02171, 0.02202, 0.02235, 0.02273, 0.02314,
    0.02351, 0.02367,
  },
  {// RH=99%
    // PHSFNC_15
    7.98100, 5.58389, 3.81067, 2.75011, 2.05900, 1.58178, 1.24011, 0.80424,
    0.55144, 0.39021, 0.28203, 0.20701, 0.15369, 0.11570, 0.05985, 0.03329,
    0.01999, 0.01295, 0.00920, 0.00726, 0.00645, 0.00646, 0.00683, 0.00743,
    0.00833, 0.00942, 0.01038, 0.01112, 0.01187, 0.01290, 0.01451, 0.01415,
    0.01176, 0.01646,
    // PHSFNC_26
    4.10720, 3.38127, 2.61113, 2.06147, 1.66320, 1.36400, 1.13140, 0.79725,
    0.57362, 0.41947, 0.31103, 0.23368, 0.17766, 0.13659, 0.07413, 0.04288,
    0.02647, 0.01753, 0.01255, 0.00978, 0.00837, 0.00791, 0.00800, 0.00831,
    0.00888, 0.00974, 0.01087, 0.01219, 0.01347, 0.01448, 0.01468, 0.01320,
    0.01237, 0.01467,
    // PHSFNC_28
    1.99262, 1.84586, 1.62814, 1.42586, 1.24733, 1.09157, 0.95572, 0.73435,
    0.56567, 0.43699, 0.33874, 0.26371, 0.20635, 0.16235, 0.09181, 0.05440,
    0.03397, 0.02253, 0.01601, 0.01230, 0.01031, 0.00946, 0.00938, 0.00951,
    0.00985, 0.01043, 0.01123, 0.01222, 0.01328, 0.01405, 0.01405, 0.01340,
    0.01391, 0.01521,
    // PHSFNC_28
    1.99262, 1.84586, 1.62814, 1.42586, 1.24733, 1.09157, 0.95572, 0.73435,
    0.56567, 0.43699, 0.33874, 0.26371, 0.20635, 0.16235, 0.09181, 0.05440,
    0.03397, 0.02253, 0.01601, 0.01230, 0.01031, 0.00946, 0.00938, 0.00951,
    0.00985, 0.01043, 0.01123, 0.01222, 0.01328, 0.01405, 0.01405, 0.01340,
    0.01391, 0.01521,
    // PHSFNC_37
    1.15457, 1.13481, 1.08637, 1.02208, 0.95035, 0.87581, 0.80158, 0.66139,
    0.53812, 0.43402, 0.34834, 0.27905, 0.22360, 0.17950, 0.10538, 0.06398,
    0.04056, 0.02712, 0.01935, 0.01490, 0.01250, 0.01143, 0.01126, 0.01130,
    0.01153, 0.01191, 0.01242, 0.01301, 0.01357, 0.01399, 0.01422, 0.01450,
    0.01526, 0.01585,
    // PHSFNC_37
    1.15457, 1.13481, 1.08637, 1.02208, 0.95035, 0.87581, 0.80158, 0.66139,
    0.53812, 0.43402, 0.34834, 0.27905, 0.22360, 0.17950, 0.10538, 0.06398,
    0.04056, 0.02712, 0.01935, 0.01490, 0.01250, 0.01143, 0.01126, 0.01130,
    0.01153, 0.01191, 0.01242, 0.01301, 0.01357, 0.01399, 0.01422, 0.01450,
    0.01526, 0.01585,
  },
};
double	upp_strat_phas[UPP_N_PHAS] =
{
  // PHSFNC_20
  3.35750, 2.77033, 2.20467, 1.77208, 1.43950, 1.18167, 0.98083, 0.69731,
  0.51368, 0.38864, 0.29989, 0.23472, 0.18552, 0.14785, 0.08680, 0.05339,
  0.03443, 0.02340, 0.01689, 0.01308, 0.01097, 0.01015, 0.01022, 0.01062,
  0.01145, 0.01289, 0.01512, 0.01829, 0.02228, 0.02580, 0.02615, 0.02523,
  0.02990, 0.03617,
  // PHSFNC_20
  3.35750, 2.77033, 2.20467, 1.77208, 1.43950, 1.18167, 0.98083, 0.69731,
  0.51368, 0.38864, 0.29989, 0.23472, 0.18552, 0.14785, 0.08680, 0.05339,
  0.03443, 0.02340, 0.01689, 0.01308, 0.01097, 0.01015, 0.01022, 0.01062,
  0.01145, 0.01289, 0.01512, 0.01829, 0.02228, 0.02580, 0.02615, 0.02523,
  0.02990, 0.03617,
  // PHSFNC_37
  1.15457, 1.13481, 1.08637, 1.02208, 0.95035, 0.87581, 0.80158, 0.66139,
  0.53812, 0.43402, 0.34834, 0.27905, 0.22360, 0.17950, 0.10538, 0.06398,
  0.04056, 0.02712, 0.01935, 0.01490, 0.01250, 0.01143, 0.01126, 0.01130,
  0.01153, 0.01191, 0.01242, 0.01301, 0.01357, 0.01399, 0.01422, 0.01450,
  0.01526, 0.01585,
  // PHSFNC_37
  1.15457, 1.13481, 1.08637, 1.02208, 0.95035, 0.87581, 0.80158, 0.66139,
  0.53812, 0.43402, 0.34834, 0.27905, 0.22360, 0.17950, 0.10538, 0.06398,
  0.04056, 0.02712, 0.01935, 0.01490, 0.01250, 0.01143, 0.01126, 0.01130,
  0.01153, 0.01191, 0.01242, 0.01301, 0.01357, 0.01399, 0.01422, 0.01450,
  0.01526, 0.01585,
  // PHSFNC_24
  0.51072, 0.50922, 0.50476, 0.49744, 0.48752, 0.47514, 0.46072, 0.42698,
  0.38890, 0.34888, 0.30902, 0.27072, 0.23504, 0.20254, 0.13652, 0.09070,
  0.06070, 0.04186, 0.03042, 0.02372, 0.02000, 0.01816, 0.01773, 0.01753,
  0.01753, 0.01767, 0.01792, 0.01826, 0.01866, 0.01907, 0.01948, 0.01984,
  0.02009, 0.02019,
  // PHSFNC_23
  0.45555, 0.45332, 0.44686, 0.43683, 0.42405, 0.40929, 0.39319, 0.35887,
  0.32388, 0.28985, 0.25771, 0.22804, 0.20101, 0.17671, 0.12722, 0.09162,
  0.06691, 0.05037, 0.03979, 0.03345, 0.03009, 0.02875, 0.02860, 0.02871,
  0.02902, 0.02946, 0.03000, 0.03059, 0.03120, 0.03179, 0.03232, 0.03276,
  0.03305, 0.03315,
};
double	upp_meteo_phas[UPP_N_PHAS] =
{
  // PHSFNC_59
  56.57000,10.86983,4.30450, 2.32817, 1.48817, 1.05608, 0.80615, 0.53590,
  0.39175, 0.30123, 0.23833, 0.19167, 0.15528, 0.12653, 0.07717, 0.04870,
  0.03126, 0.02128, 0.01493, 0.01091, 0.00856, 0.00765, 0.00767, 0.00787,
  0.00840, 0.00973, 0.01199, 0.01623, 0.02446, 0.03802, 0.04421, 0.04801,
  0.05991, 0.06067,
  // PHSFNC_59
  56.57000,10.86983,4.30450, 2.32817, 1.48817, 1.05608, 0.80615, 0.53590,
  0.39175, 0.30123, 0.23833, 0.19167, 0.15528, 0.12653, 0.07717, 0.04870,
  0.03126, 0.02128, 0.01493, 0.01091, 0.00856, 0.00765, 0.00767, 0.00787,
  0.00840, 0.00973, 0.01199, 0.01623, 0.02446, 0.03802, 0.04421, 0.04801,
  0.05991, 0.06067,
  // PHSFNC_11
  15.36571,6.08614, 3.15543, 1.98500, 1.39671, 1.05749, 0.84297, 0.58904,
  0.44120, 0.34216, 0.27140, 0.21837, 0.17669, 0.14371, 0.08723, 0.05498,
  0.03568, 0.02444, 0.01753, 0.01339, 0.01087, 0.00986, 0.00983, 0.01019,
  0.01094, 0.01243, 0.01473, 0.01871, 0.02423, 0.03204, 0.03592, 0.03792,
  0.04492, 0.05004,
  // PHSFNC_11
  15.36571,6.08614, 3.15543, 1.98500, 1.39671, 1.05749, 0.84297, 0.58904,
  0.44120, 0.34216, 0.27140, 0.21837, 0.17669, 0.14371, 0.08723, 0.05498,
  0.03568, 0.02444, 0.01753, 0.01339, 0.01087, 0.00986, 0.00983, 0.01019,
  0.01094, 0.01243, 0.01473, 0.01871, 0.02423, 0.03204, 0.03592, 0.03792,
  0.04492, 0.05004,
  // PHSFNC_20
  3.35750, 2.77033, 2.20467, 1.77208, 1.43950, 1.18167, 0.98083, 0.69731,
  0.51368, 0.38864, 0.29989, 0.23472, 0.18552, 0.14785, 0.08680, 0.05339,
  0.03443, 0.02340, 0.01689, 0.01308, 0.01097, 0.01015, 0.01022, 0.01062,
  0.01145, 0.01289, 0.01512, 0.01829, 0.02228, 0.02580, 0.02615, 0.02523,
  0.02990, 0.03617,
  // PHSFNC_20
  3.35750, 2.77033, 2.20467, 1.77208, 1.43950, 1.18167, 0.98083, 0.69731,
  0.51368, 0.38864, 0.29989, 0.23472, 0.18552, 0.14785, 0.08680, 0.05339,
  0.03443, 0.02340, 0.01689, 0.01308, 0.01097, 0.01015, 0.01022, 0.01062,
  0.01145, 0.01289, 0.01512, 0.01829, 0.02228, 0.02580, 0.02615, 0.02523,
  0.02990, 0.03617,
};
// Parameters for control
int	cnt_version			= MOD_V4;		// MODTRAN version
int	cnt_cflg			= 0;			// Clear   flag
int	cnt_tmx[MIE_MAXCOMP]		= {0,0,0,0,0,0,0,0,0,0};// T-matrix flag
int	cnt_xmod 			= 0;			// Mixing  mode
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

#include "mod4_subs.c"
#include "mod5_subs.c"

int main(int argc,char **argv)
{
  int i,j;
  int err;
  double par[6];
  char command[MAXLINE];

  if(GetOpt(argc,argv) < 0) return -1;
  if(cnt_hp) {Usage(); return 0;}
  if(Init() < 0) return -1;

  if(sim_mode==SIM_MODE_DSR_CUSTOM_BO ||
     sim_mode==SIM_MODE_SSR_CUSTOM_SO ||
     sim_mode==SIM_MODE_SSR_CUSTOM_BO ||
     sim_mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    for(i=0; i<4; i++)
    {
      if(mie_aers_cmp[i][0] != '\0')
      {
        if((mie_n_comp=ReadComp(i,MIE_MAXCOMP,mie_wcom_com,mie_rmod_com,mie_lsgm_com,mie_refr_com,mie_refi_com)) <= 0) return -1;
        if(MieInit() < 0) return -1;
        if(MieTable(0,mie_n_comp-1) < 0) return -1;
        if(MieCalc() < 0) return -1;
        if(MixComp() < 0) return -1;
        if(MiePrintout() < 0) return -1;
        if(SetMie1(i) < 0) return -1;
      } else
      if(mie_aers_inp1[i][0] != '\0')
      {
        if(ReadMie(i) < 0) return -1;
        if(SetMie1(i) < 0) return -1;
      } else
      if(i == 0)
      {
        fprintf(stderr,"Error, no custom aerosol parameters\n");
        return -1;
      }
      else
      {
        if(upper_pfunc(i) < 0) return -1;
      }
      if(SetMie2(i) < 0) return -1;
    }
  }

  if(sim_n_dir > 0)
  {
    switch(sim_mode)
    {
      case SIM_MODE_DSR_MODTRAN_BO:
      case SIM_MODE_DSR_CUSTOM_BO:
        for(j=0; j<sim_n_dir; j++)
        {
          par[2] = sim_dir[0][j];
          if(dsr_n_wlen > 0)
          {
            par[0] = dsr_wlen[0]-sim_xmgn;
            par[1] = dsr_wlen[dsr_n_wlen-1]+sim_xmgn;
            err = 0;
            do
            {
              if(WriteTape5(par,sim_mode) < 0)
              {
                err = 1;
                break;
              }
              if(RunModtran() < 0)
              {
                err = 1;
                break;
              }
              if(ReadPlt_B(par,dsr_n_wlen,dsr_wlen,dsr_dsim) < 0)
              {
                err = 1;
                break;
              }
            }
            while(0);
            if(err)
            {
              fprintf(stderr,"ERROR!\n");
              return -1;
            }
            for(i=0; i<dsr_n_wlen; i++)
            {
              printf("%13.6e %13.6e\n",dsr_wlen[i],dsr_dsim[i]);
            }
          }
          else
          {
            par[0] = sim_wlen_min;
            par[1] = sim_wlen_max;
            err = 0;
            do
            {
              if(WriteTape5(par,sim_mode) < 0)
              {
                err = 1;
                break;
              }
              if(cnt_db > 0)
              {
                fprintf(stderr,"Tape5 is ready.\n");
              } else
              if(RunModtran() < 0)
              {
                err = 1;
                break;
              }
            }
            while(0);
            if(err)
            {
              fprintf(stderr,"ERROR!\n");
              return -1;
            }
          }
          snprintf(command,MAXLINE,"mv modtran.plt dsr%06d.plt",j);
          if(system(command) < 0)
          {
            fprintf(stderr,"ERROR!\n");
            return -1;
          }
        }
        break;
      case SIM_MODE_SSR_MODTRAN_BO:
      case SIM_MODE_SSR_CUSTOM_BO:
        for(j=0; j<sim_n_dir; j++)
        {
          par[2] = sim_dir[0][j];
          par[3] = sim_dir[1][j];
          par[4] = sim_dir[2][j];
          par[5] = sim_dir[3][j];
          sim_sbmp = sim_sbmp_init;
          if(ssr_n_wlen > 0)
          {
            par[0] = ssr_wlen[0]-sim_xmgn;
            par[1] = ssr_wlen[ssr_n_wlen-1]+sim_xmgn;
            err = 0;
            do
            {
              if(WriteTape5(par,sim_mode) < 0)
              {
                err = 1;
                break;
              }
              if(RunModtran() < 0)
              {
                err = 1;
                break;
              }
              if(ReadPlt_B(par,ssr_n_wlen,ssr_wlen,ssr_dsim) < 0)
              {
                err = 1;
                break;
              }
            }
            while(0);
            if(err)
            {
              fprintf(stderr,"ERROR!\n");
              return -1;
            }
            while(0);
            for(i=0; i<ssr_n_wlen; i++)
            {
              printf("%13.6e %13.6e\n",ssr_wlen[i],ssr_dsim[i]);
            }
          }
          else
          {
            par[0] = sim_wlen_min;
            par[1] = sim_wlen_max;
            err = 0;
            do
            {
              if(WriteTape5(par,sim_mode) < 0)
              {
                err = 1;
                break;
              }
              if(cnt_db > 0)
              {
                fprintf(stderr,"Tape5 is ready.\n");
              } else
              if(RunModtran() < 0)
              {
                err = 1;
                break;
              }
            }
            while(0);
            if(err)
            {
              fprintf(stderr,"ERROR!\n");
              return -1;
            }
          }
          snprintf(command,MAXLINE,"mv modtran.plt ssr%06d.plt",j);
          if(system(command) < 0)
          {
            fprintf(stderr,"ERROR!\n");
            return -1;
          }
        }
        break;
      case SIM_MODE_SSR_MODTRAN_SO:
      case SIM_MODE_SSR_CUSTOM_SO:
        for(j=0; j<sim_n_dir; j++)
        {
          par[2] = sim_dir[0][j];
          par[3] = sim_dir[1][j];
          par[4] = sim_dir[2][j];
          par[5] = sim_dir[3][j];
          if(ssr_n_wlen > 0)
          {
            for(i=0; i<ssr_n_wlen; i++)
            {
              par[0] = ssr_wlen[i];
              par[1] = (double)ssr_nsam[i];
              sim_sbmp = ssr_sbmp[i];
              err = 0;
              do
              {
                if(WriteTape5(par,sim_mode) < 0)
                {
                  err = 1;
                  break;
                }
                if(RunModtran() < 0)
                {
                  err = 1;
                  break;
                }
                if(ReadPlt_S(par,&ssr_dsim[i]) < 0)
                {
                  err = 1;
                  break;
                }
              }
              while(0);
              if(err)
              {
                fprintf(stderr,"ERROR!\n");
                return -1;
              }
              printf("%13.6e %13.6e\n",ssr_wlen[i],ssr_dsim[i]);
            }
          }
          else
          {
            par[0] = sim_wlen_min;
            par[1] = (double)sim_nsam_init;
            sim_sbmp = sim_sbmp_init;
            err = 0;
            do
            {
              if(WriteTape5(par,sim_mode) < 0)
              {
                err = 1;
                break;
              }
              if(cnt_db > 0)
              {
                fprintf(stderr,"Tape5 is ready.\n");
              } else
              if(RunModtran() < 0)
              {
                err = 1;
                break;
              }
            }
            while(0);
            if(err)
            {
              fprintf(stderr,"ERROR!\n");
              return -1;
            }
          }
        }
        break;
      case SIM_MODE_TRN_MODTRAN_BO:
      case SIM_MODE_TRN_CUSTOM_BO:
        for(j=0; j<sim_n_dir; j++)
        {
          par[2] = sim_dir[2][j];
          if(dsr_n_wlen > 0)
          {
            par[0] = dsr_wlen[0]-sim_xmgn;
            par[1] = dsr_wlen[dsr_n_wlen-1]+sim_xmgn;
            err = 0;
            do
            {
              if(WriteTape5(par,sim_mode) < 0)
              {
                err = 1;
                break;
              }
              if(RunModtran() < 0)
              {
                err = 1;
                break;
              }
              if(ReadPlt_B(par,dsr_n_wlen,dsr_wlen,dsr_dsim) < 0)
              {
                err = 1;
                break;
              }
            }
            while(0);
            if(err)
            {
              fprintf(stderr,"ERROR!\n");
              return -1;
            }
            for(i=0; i<dsr_n_wlen; i++)
            {
              printf("%13.6e %13.6e\n",dsr_wlen[i],dsr_dsim[i]);
            }
          }
          else
          {
            par[0] = sim_wlen_min;
            par[1] = sim_wlen_max;
            err = 0;
            do
            {
              if(WriteTape5(par,sim_mode) < 0)
              {
                err = 1;
                break;
              }
              if(cnt_db > 0)
              {
                fprintf(stderr,"Tape5 is ready.\n");
              } else
              if(RunModtran() < 0)
              {
                err = 1;
                break;
              }
            }
            while(0);
            if(err)
            {
              fprintf(stderr,"ERROR!\n");
              return -1;
            }
          }
          snprintf(command,MAXLINE,"mv modtran.plt trn%06d.plt",j);
          if(system(command) < 0)
          {
            fprintf(stderr,"ERROR!\n");
            return -1;
          }
        }
        break;
      default:
        fprintf(stderr,"Invalid mode >>> %d\n",sim_mode);
        return -1;
        break;
    }
  }
  else
  {
    switch(sim_mode)
    {
      case SIM_MODE_DSR_MODTRAN_BO:
      case SIM_MODE_DSR_CUSTOM_BO:
        par[2] = sim_th_sun;
        if(dsr_n_wlen > 0)
        {
          par[0] = dsr_wlen[0]-sim_xmgn;
          par[1] = dsr_wlen[dsr_n_wlen-1]+sim_xmgn;
          err = 0;
          do
          {
            if(WriteTape5(par,sim_mode) < 0)
            {
              err = 1;
              break;
            }
            if(RunModtran() < 0)
            {
              err = 1;
              break;
            }
            if(ReadPlt_B(par,dsr_n_wlen,dsr_wlen,dsr_dsim) < 0)
            {
              err = 1;
              break;
            }
          }
          while(0);
          if(err)
          {
            fprintf(stderr,"ERROR!\n");
            return -1;
          }
          for(i=0; i<dsr_n_wlen; i++)
          {
            printf("%13.6e %13.6e\n",dsr_wlen[i],dsr_dsim[i]);
          }
        }
        else
        {
          par[0] = sim_wlen_min;
          par[1] = sim_wlen_max;
          err = 0;
          do
          {
            if(WriteTape5(par,sim_mode) < 0)
            {
              err = 1;
              break;
            }
            if(cnt_db > 0)
            {
              fprintf(stderr,"Tape5 is ready.\n");
            } else
            if(RunModtran() < 0)
            {
              err = 1;
              break;
            }
          }
          while(0);
          if(err)
          {
            fprintf(stderr,"ERROR!\n");
            return -1;
          }
        }
        break;
      case SIM_MODE_SSR_MODTRAN_BO:
      case SIM_MODE_SSR_CUSTOM_BO:
        par[2] = sim_th_sun;
        par[3] = sim_ph_sun;
        par[4] = sim_th_los;
        par[5] = sim_ph_los;
        sim_sbmp = sim_sbmp_init;
        if(ssr_n_wlen > 0)
        {
          par[0] = ssr_wlen[0]-sim_xmgn;
          par[1] = ssr_wlen[ssr_n_wlen-1]+sim_xmgn;
          err = 0;
          do
          {
            if(WriteTape5(par,sim_mode) < 0)
            {
              err = 1;
              break;
            }
            if(RunModtran() < 0)
            {
              err = 1;
              break;
            }
            if(ReadPlt_B(par,ssr_n_wlen,ssr_wlen,ssr_dsim) < 0)
            {
              err = 1;
              break;
            }
          }
          while(0);
          if(err)
          {
            fprintf(stderr,"ERROR!\n");
            return -1;
          }
          while(0);
          for(i=0; i<ssr_n_wlen; i++)
          {
            printf("%13.6e %13.6e\n",ssr_wlen[i],ssr_dsim[i]);
          }
        }
        else
        {
          par[0] = sim_wlen_min;
          par[1] = sim_wlen_max;
          err = 0;
          do
          {
            if(WriteTape5(par,sim_mode) < 0)
            {
              err = 1;
              break;
            }
            if(cnt_db > 0)
            {
              fprintf(stderr,"Tape5 is ready.\n");
            } else
            if(RunModtran() < 0)
            {
              err = 1;
              break;
            }
          }
          while(0);
          if(err)
          {
            fprintf(stderr,"ERROR!\n");
            return -1;
          }
        }
        break;
      case SIM_MODE_SSR_MODTRAN_SO:
      case SIM_MODE_SSR_CUSTOM_SO:
        par[2] = sim_th_sun;
        par[3] = sim_ph_sun;
        par[4] = sim_th_los;
        par[5] = sim_ph_los;
        if(ssr_n_wlen > 0)
        {
          for(i=0; i<ssr_n_wlen; i++)
          {
            par[0] = ssr_wlen[i];
            par[1] = (double)ssr_nsam[i];
            sim_sbmp = ssr_sbmp[i];
            err = 0;
            do
            {
              if(WriteTape5(par,sim_mode) < 0)
              {
                err = 1;
                break;
              }
              if(RunModtran() < 0)
              {
                err = 1;
                break;
              }
              if(ReadPlt_S(par,&ssr_dsim[i]) < 0)
              {
                err = 1;
                break;
              }
            }
            while(0);
            if(err)
            {
              fprintf(stderr,"ERROR!\n");
              return -1;
            }
            printf("%13.6e %13.6e\n",ssr_wlen[i],ssr_dsim[i]);
          }
        }
        else
        {
          par[0] = sim_wlen_min;
          par[1] = (double)sim_nsam_init;
          sim_sbmp = sim_sbmp_init;
          err = 0;
          do
          {
            if(WriteTape5(par,sim_mode) < 0)
            {
              err = 1;
              break;
            }
            if(cnt_db > 0)
            {
              fprintf(stderr,"Tape5 is ready.\n");
            } else
            if(RunModtran() < 0)
            {
              err = 1;
              break;
            }
          }
          while(0);
          if(err)
          {
            fprintf(stderr,"ERROR!\n");
            return -1;
          }
        }
        break;
      case SIM_MODE_TRN_MODTRAN_BO:
      case SIM_MODE_TRN_CUSTOM_BO:
        par[2] = sim_th_los;
        if(dsr_n_wlen > 0)
        {
          par[0] = dsr_wlen[0]-sim_xmgn;
          par[1] = dsr_wlen[dsr_n_wlen-1]+sim_xmgn;
          err = 0;
          do
          {
            if(WriteTape5(par,sim_mode) < 0)
            {
              err = 1;
              break;
            }
            if(RunModtran() < 0)
            {
              err = 1;
              break;
            }
            if(ReadPlt_B(par,dsr_n_wlen,dsr_wlen,dsr_dsim) < 0)
            {
              err = 1;
              break;
            }
          }
          while(0);
          if(err)
          {
            fprintf(stderr,"ERROR!\n");
            return -1;
          }
          for(i=0; i<dsr_n_wlen; i++)
          {
            printf("%13.6e %13.6e\n",dsr_wlen[i],dsr_dsim[i]);
          }
        }
        else
        {
          par[0] = sim_wlen_min;
          par[1] = sim_wlen_max;
          err = 0;
          do
          {
            if(WriteTape5(par,sim_mode) < 0)
            {
              err = 1;
              break;
            }
            if(cnt_db > 0)
            {
              fprintf(stderr,"Tape5 is ready.\n");
            } else
            if(RunModtran() < 0)
            {
              err = 1;
              break;
            }
          }
          while(0);
          if(err)
          {
            fprintf(stderr,"ERROR!\n");
            return -1;
          }
        }
        break;
      default:
        fprintf(stderr,"Invalid mode >>> %d\n",sim_mode);
        return -1;
        break;
    }
  }

  Finish();

  return 0;
}

int Init(void)
{
  int i;

  if(dsr_n_wlen > 0)
  {
    if((dsr_dsim=(double*)malloc(dsr_n_wlen*sizeof(double))) == NULL)
    {
      fprintf(stderr,"Error in allocating memory for %s\n","dsr_dsim");
      return -1;
    }
  }
  if(ssr_n_wlen > 0)
  {
    if(ssr_n_sbmp == 0)
    {
      if((ssr_sbmp=(int*)malloc(ssr_n_wlen*sizeof(int))) == NULL)
      {
        fprintf(stderr,"Error in allocating memory for %s\n","ssr_sbmp");
        return -1;
      }
      for(i=0; i<ssr_n_wlen; i++)
      {
        ssr_sbmp[i] = sim_sbmp_init;
      }
      ssr_n_sbmp = ssr_n_wlen;
    } else
    if(ssr_n_sbmp != ssr_n_wlen)
    {
      fprintf(stderr,"Error, ssr_n_sbmp=%d, ssr_n_wlen=%d\n",ssr_n_sbmp,ssr_n_wlen);
      return -1;
    }
    if(ssr_n_nsam == 0)
    {
      if((ssr_nsam=(int*)malloc(ssr_n_wlen*sizeof(int))) == NULL)
      {
        fprintf(stderr,"Error in allocating memory for %s\n","ssr_nsam");
        return -1;
      }
      for(i=0; i<ssr_n_wlen; i++)
      {
        ssr_nsam[i] = sim_nsam_init;
      }
      ssr_n_nsam = ssr_n_wlen;
    } else
    if(ssr_n_nsam != ssr_n_wlen)
    {
      fprintf(stderr,"Error, ssr_n_nsam=%d, ssr_n_wlen=%d\n",ssr_n_nsam,ssr_n_wlen);
      return -1;
    }
    if((ssr_dsim=(double*)malloc(ssr_n_wlen*sizeof(double))) == NULL)
    {
      fprintf(stderr,"Error in allocating memory for %s\n","ssr_dsim");
      return -1;
    }
  }

  if(cnt_version > MOD_V4)
  {
    WriteTape5 = WriteTape5_V5;
    RunModtran = RunModtran_V5;
  }
  else
  {
    WriteTape5 = WriteTape5_V4;
    RunModtran = RunModtran_V4;
  }

  return 0;
}

void Finish(void)
{
  int i;
  int err = 0;

  // Deallocate memories
  free(sim_phas_wlen_um);
  for(i=0; i<4; i++)
  {
    free(sim_aers_wlen_um[i]);
    free(sim_aers_cext[i]);
    free(sim_aers_cabs[i]);
    free(sim_aers_asym[i]);
    free(sim_aers_phas[i]);
  }
  if(sim_n_dir > 0)
  {
    free(sim_dir[0]);
    free(sim_dir[1]);
    free(sim_dir[2]);
    free(sim_dir[3]);
  }
  if(dsr_wlen != NULL)
  {
    free(dsr_wlen);
    dsr_wlen = NULL;
  }
  if(dsr_dsim != NULL)
  {
    free(dsr_dsim);
    dsr_dsim = NULL;
  }
  if(ssr_wlen != NULL)
  {
    free(ssr_wlen);
    ssr_wlen = NULL;
  }
  if(ssr_dsim != NULL)
  {
    free(ssr_dsim);
    ssr_dsim = NULL;
  }
  if(ssr_sbmp != NULL)
  {
    free(ssr_sbmp);
    ssr_sbmp = NULL;
  }

  // Cleanup files
  if(cnt_cflg > 0)
  {
    err += system("rm -f modtran.7sc");
    err += system("rm -f modtran.7sr");
    err += system("rm -f modtran.mc");
    err += system("rm -f modtran.psc");
    err += system("rm -f modtran.tp6");
    err += system("rm -f modtran.tp7");
    err += system("rm -f modtran.tp8");
    if(cnt_version > MOD_V4)
    {
      err += system("rm -f mod5root.in");
    }
    else
    {
      err += system("rm -f modroot.in");
    }
    err += system("rm -f DATA");
  }
  if(cnt_cflg > 1)
  {
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

int MieInit(void)
{
  int i,j,n;
  char fnam[] = "MieInit";

  mie_iref = -1;
  for(i=0; i<mie_n_wlen; i++)
  {
    if(fabs(mie_wlen[i]-mie_wlen_ref) < EPSILON)
    {
      mie_iref = i;
      break;
    }
  }
  if(mie_iref < 0)
  {
    fprintf(stderr,"%s: failed in finding reference wavelength %13.6e\n",fnam,mie_wlen_ref);
    return -1;
  }

  mie_wlen_um = (double *)malloc(mie_n_wlen*sizeof(double));
  if(mie_wlen_um == NULL)
  {
    fprintf(stderr,"%s: failed in allocating memory\n",fnam);
    return -1;
  }
  for(i=0; i<mie_n_wlen; i++)
  {
    mie_wlen_um[i] = mie_wlen[i]*1.0e-3;
  }
  mie_aext = (double *)malloc(mie_n_wlen*sizeof(double));
  mie_asca = (double *)malloc(mie_n_wlen*sizeof(double));
  mie_asym = (double *)malloc(mie_n_wlen*sizeof(double));
  mie_phs1 = (double *)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
  mie_phs2 = (double *)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
  mie_phas = (double *)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
  if(mie_aext==NULL || mie_asca==NULL || mie_asym==NULL ||
     mie_phs1==NULL || mie_phs2==NULL || mie_phas==NULL)
  {
    fprintf(stderr,"%s: failed in allocating memory\n",fnam);
    return -1;
  }






  for(n=0; n<mie_n_comp; n++)
  {
    if(mie_size_func[n] == MIE_FUNC_FILE)
    {
      fprintf(stderr,"%s: mie_size_func[%d]=%c, not implemented yet.\n",fnam,n,mie_size_func[n]);
      mie_size_func[n] = '?';
      return -1;
    }
    mie_aext_com[n] = (double *)malloc(mie_n_wlen*sizeof(double));
    mie_asca_com[n] = (double *)malloc(mie_n_wlen*sizeof(double));
    mie_asym_com[n] = (double *)malloc(mie_n_wlen*sizeof(double));
    mie_phs1_com[n] = (double *)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
    mie_phs2_com[n] = (double *)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
    mie_phas_com[n] = (double *)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
    if(mie_aext_com[n]==NULL || mie_asca_com[n]==NULL || mie_asym_com[n]==NULL ||
       mie_phs1_com[n]==NULL || mie_phs2_com[n]==NULL || mie_phas_com[n]==NULL)
    {
      fprintf(stderr,"%s: failed in allocating memory\n",fnam);
      return -1;
    }
    mie_aext_tab[n] = NULL;
    mie_asca_tab[n] = NULL;
    mie_phs1_tab[n] = NULL;
    mie_phs2_tab[n] = NULL;
    mie_lrad_min[n] = NULL;
    mie_lrad_max[n] = NULL;
    mie_lrad_stp[n] = NULL;
    mie_lmod_com[n] = log10(mie_rmod_com[n]);
  }

  mie_angl_rad = (double *)malloc(mie_n_angl*sizeof(double));
  mie_angl_sin = (double *)malloc(mie_n_angl*sizeof(double));
  mie_angl_cos = (double *)malloc(mie_n_angl*sizeof(double));
  mie_angl_dif = (double *)malloc(mie_n_angl*sizeof(double));
  if(mie_angl_rad==NULL || mie_angl_sin==NULL || mie_angl_cos==NULL || mie_angl_dif==NULL)
  {
    fprintf(stderr,"%s: failed in allocating memory\n",fnam);
    return -1;
  }
  for(j=0; j<mie_n_angl; j++)
  {
    mie_angl_rad[j] = mie_angl[j]*D_TO_R;
    mie_angl_sin[j] = sin(mie_angl_rad[j]);
    mie_angl_cos[j] = cos(mie_angl_rad[j]);
  }
  for(j=1; j<mie_n_angl; j++)
  {
    mie_angl_dif[j] = PI*fabs(mie_angl_rad[j]-mie_angl_rad[j-1]);
  }

  return 0;
}

int MieTable(int n1,int n2)
{
  // IMPORTANT: This function MUST be called before MieCalc().
  //            (after setting mie_refr_com & mie_refi_com.)
  int i,j,k,m,n;
  int m1,m2,mt;
  int err = 0;
  int fmie,ftmx;
  double x,r,l;
  double s1,s2;
  int ANYANG,PERFCT,PRNT[2];
  int IPOLZN,MOMDIM,NUMANG,NMOM;
  double GQSC,MIMCUT,PMOM[2][2],QEXT,QSCA,SPIKE;
  double _Complex CREFIN,SFORW,SBACK,*S1,*S2,TFORW[2],TBACK[2];
  int iflg,ierr,ndistr,npnax,nkmax;
  int np,npna,ndgs,l1max[TMX_NAXMAX];
  double rat,axmax,r1,r2,b,gam,eps;
  double lam,mrr,mri,ddelt;
  double *reff,*veff,*cext,*csca,*walb,*gsym;
  double *alp1,*alp2,*alp3,*alp4,*bet1,*bet2;
  double *fmat,*angl,*f11,*f12,*a1,*b1;
  char fnam[] = "MieTable";

  // initialization
  fmie = ftmx = 0;
  for(n=n1; n<=n2; n++)
  {
    if(cnt_tmx[n])
    {
      ftmx = 1;
    }
    else
    {
      fmie = 1;
    }
  }
  if(fmie)
  {
    S1 = (double _Complex *)malloc(mie_n_angl*sizeof(double _Complex));
    S2 = (double _Complex *)malloc(mie_n_angl*sizeof(double _Complex));
    if(S1==NULL || S2==NULL)
    {
      fprintf(stderr,"%s: failed in allocating memory\n",fnam);
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
  }
  if(ftmx)
  {
    reff = (double*)malloc(TMX_NAXMAX*sizeof(double));
    veff = (double*)malloc(TMX_NAXMAX*sizeof(double));
    cext = (double*)malloc(TMX_NAXMAX*sizeof(double));
    csca = (double*)malloc(TMX_NAXMAX*sizeof(double));
    walb = (double*)malloc(TMX_NAXMAX*sizeof(double));
    gsym = (double*)malloc(TMX_NAXMAX*sizeof(double));
    alp1 = (double*)malloc(TMX_NALMAX*sizeof(double));
    alp2 = (double*)malloc(TMX_NALMAX*sizeof(double));
    alp3 = (double*)malloc(TMX_NALMAX*sizeof(double));
    alp4 = (double*)malloc(TMX_NALMAX*sizeof(double));
    bet1 = (double*)malloc(TMX_NALMAX*sizeof(double));
    bet2 = (double*)malloc(TMX_NALMAX*sizeof(double));
    fmat = (double*)malloc(TMX_NARMAX*sizeof(double));
    angl = (double*)malloc(TMX_NAMAX*sizeof(double));
    f11  = (double*)malloc(TMX_NAMAX*sizeof(double));
    f12  = (double*)malloc(TMX_NAMAX*sizeof(double));
    a1   = (double*)malloc(MIE_MAXDATA*sizeof(double));
    b1   = (double*)malloc(MIE_MAXDATA*sizeof(double));
    if(reff==NULL || veff==NULL || cext==NULL || csca==NULL || walb==NULL || gsym==NULL ||
       alp1==NULL || alp2==NULL || alp3==NULL || alp4==NULL || bet1==NULL || bet2==NULL ||
       fmat==NULL || angl==NULL ||  f11==NULL ||  f12==NULL ||   a1==NULL ||   b1==NULL)
    {
      fprintf(stderr,"%s: failed in allocating memory\n",fnam);
      return -1;
    }
    for(j=0; j<tmx_nang; j++)
    {
      angl[j] = (180.0/(tmx_nang-1)*j);
    }
  }

  for(n=n1; n<=n2; n++)
  {
    // allocate memory
    if(mie_aext_tab[n] != NULL) free(mie_aext_tab[n]);
    if(mie_asca_tab[n] != NULL) free(mie_asca_tab[n]);
    if(mie_phs1_tab[n] != NULL) free(mie_phs1_tab[n]);
    if(mie_phs2_tab[n] != NULL) free(mie_phs2_tab[n]);
    mie_aext_tab[n] = (double *)calloc(mie_n_wlen*mie_n_step,sizeof(double));
    mie_asca_tab[n] = (double *)calloc(mie_n_wlen*mie_n_step,sizeof(double));
    mie_phs1_tab[n] = (double *)calloc(mie_n_wlen*mie_n_step*mie_n_angl,sizeof(double));
    mie_phs2_tab[n] = (double *)calloc(mie_n_wlen*mie_n_step*mie_n_angl,sizeof(double));
    if(mie_aext_tab[n]==NULL || mie_asca_tab[n]==NULL ||
       mie_phs1_tab[n]==NULL || mie_phs2_tab[n]==NULL)
    {
      fprintf(stderr,"%s: error in allocating memory\n",fnam);
      err = 1;
      break;
    }
    if(mie_lrad_min[n] != NULL) free(mie_lrad_min[n]);
    if(mie_lrad_max[n] != NULL) free(mie_lrad_max[n]);
    if(mie_lrad_stp[n] != NULL) free(mie_lrad_stp[n]);
    mie_lrad_min[n] = (double *)malloc(mie_n_wlen*sizeof(double));
    mie_lrad_max[n] = (double *)malloc(mie_n_wlen*sizeof(double));
    mie_lrad_stp[n] = (double *)malloc(mie_n_wlen*sizeof(double));
    if(mie_lrad_min[n]==NULL || mie_lrad_max[n]==NULL || mie_lrad_stp[n]==NULL)
    {
      fprintf(stderr,"%s: error in allocating memory\n",fnam);
      err = 1;
      break;
    }

    // Mie calculation
    for(i=0; i<mie_n_wlen; i++)
    {
      if(isnan(mie_rmin))
      {
        mie_lrad_min[n][i] = mie_lmod_com[n]-mie_lsgm_com[n]*mie_wsgm;
      }
      else
      {
        mie_lrad_min[n][i] = log10(mie_rmin);
      }
      if(mie_lrad_min[n][i] < MIE_LRAD_MIN)
      {
        mie_lrad_min[n][i] = MIE_LRAD_MIN;
      }
      if(isnan(mie_rmax))
      {
        mie_lrad_max[n][i] = mie_lmod_com[n]+mie_lsgm_com[n]*mie_wsgm;
      }
      else
      {
        mie_lrad_max[n][i] = log10(mie_rmax);
      }
      if(mie_lrad_max[n][i] > MIE_LRAD_MAX)
      {
        mie_lrad_max[n][i] = MIE_LRAD_MAX;
      }
      if(cnt_tmx[n])
      {
        l = log10(TMX_RMAX);
      }
      else
      {
        l = log10(MIE_XMAX*mie_wlen_um[i]/PI2);
      }
      if(mie_lrad_max[n][i] > l)
      {
        mie_lrad_max[n][i] = l;
      }
      mie_lrad_stp[n][i] = (mie_lrad_max[n][i]-mie_lrad_min[n][i])/mie_n_step;
      if(cnt_tmx[n])
      {
        for(m=0; m<mie_n_step; m++)
        {
          l = mie_lrad_min[n][i]+mie_lrad_stp[n][i]*m;
          r = pow(10.0,l);
          x = PI2*r/(mie_wlen_um[i]);
          iflg   = cnt_vb;
          ierr   = 0;
          rat    = 0.5;
          ndistr = 4;
          axmax  = r;
          npnax  = 1;
          r1     = 0.9999999*r;
          r2     = 1.0000001*r;
          b      = 1.0e-1;
          gam    = 0.5;
          nkmax  = -1;
          eps    = tmx_reps[n];
          np     = tmx_shap[n];
          lam    = mie_wlen_um[i];
          mrr    = mie_refr_com[n][i];
          mri    = mie_refi_com[n][i];
          ddelt  = tmx_delt; // this will be modified by tmatrix_()!
          npna   = tmx_nang;
          ndgs   = tmx_ndgs;
          tmatrix_(&iflg,&ierr,&rat,&ndistr,&axmax,&npnax,&r1,&r2,&b,&gam,
                   &nkmax,&eps,&np,&lam,&mrr,&mri,&ddelt,&npna,&ndgs,
                   reff,veff,cext,csca,walb,gsym,
                   l1max,alp1,alp2,alp3,alp4,bet1,bet2,fmat);
          if(ierr)
          {
            fprintf(stderr,"%s: warning, ierr=%d, r=%13.6e, lam=%13.6f, x=%13.6e, eps=%13.6e\n",
                            fnam,ierr,r,lam,x,eps);
            break;
          }
          k = mie_n_step*i+m;
          mie_aext_tab[n][k] = cext[0];
          mie_asca_tab[n][k] = csca[0];
          for(j=0; j<tmx_nang; j++)
          {
            f11[j] = fmat[j*6+0];
            f12[j] = fmat[j*6+4];
          }
          SamplingE(tmx_nang,angl,f11,mie_n_angl,mie_angl,a1,1.0);
          SamplingE(tmx_nang,angl,f12,mie_n_angl,mie_angl,b1,1.0);
          for(j=0,k=mie_n_step*mie_n_angl*i+mie_n_angl*m; j<mie_n_angl; j++,k++)
          {
            mie_phs1_tab[n][k] = (a1[j]-b1[j])*csca[0];
            mie_phs2_tab[n][k] = (a1[j]+b1[j])*csca[0];
          }
        }
      }
      else
      {
        for(m=0; m<mie_n_step; m++)
        {
          l = mie_lrad_min[n][i]+mie_lrad_stp[n][i]*m;
          r = pow(10.0,l);
          x = PI2*r/(mie_wlen_um[i]);
          CREFIN = mie_refr_com[n][i]-fabs(mie_refi_com[n][i])*I;
          miev0_(&x,&CREFIN,&PERFCT,&MIMCUT,&ANYANG,&NUMANG,mie_angl_cos,
                 &NMOM,&IPOLZN,&MOMDIM,PRNT,&QEXT,&QSCA,&GQSC,
                 PMOM,&SFORW,&SBACK,S1,S2,TFORW,TBACK,&SPIKE);
          k = mie_n_step*i+m;
          mie_aext_tab[n][k] = QEXT*PI*r*r;
          mie_asca_tab[n][k] = QSCA*PI*r*r;
          for(j=0,k=mie_n_step*mie_n_angl*i+mie_n_angl*m; j<mie_n_angl; j++,k++)
          {
            s1 = cabs(S1[j]);
            s2 = cabs(S2[j]);
            mie_phs1_tab[n][k] = s1*s1;
            mie_phs2_tab[n][k] = s2*s2;
          }
        }
      }
    }
    if(err) break;
  }
  if(fmie)
  {
    free(S1);
    free(S2);
  }
  if(ftmx)
  {
    free(reff);
    free(veff);
    free(cext);
    free(csca);
    free(walb);
    free(gsym);
    free(alp1);
    free(alp2);
    free(alp3);
    free(alp4);
    free(bet1);
    free(bet2);
    free(fmat);
    free(angl);
    free(f11);
    free(f12);
    free(a1);
    free(b1);
  }
  if(err)
  {
    return -1;
  }

  return 0;
}

int MieCalc(void)
{
  int i,j,k,m,n,p;
  int err = 0;
  double w,x,y,r,l;
  double sw,sy,sp,sinv;
  double indx,cnst;
  double norm,fact;
  double lmin,lmax;
  double epsilon = 1.0e-1;
  char fnam[] = "MieCalc";

  // initialization
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

  // integrate over size
  for(n=0; n<mie_n_comp; n++)
  {
    if(mie_size_func[n] == MIE_FUNC_FILE)
    {
      lmin = log10(mie_xval_com[n][0]);
      lmax = log10(mie_xval_com[n][mie_n_size[n]-1]);
    }
    else
    {
      switch(mie_size_func[n])
      {
        case MIE_FUNC_LOGNORMAL:
        case MIE_FUNC_EFF_LOGNORMAL:
          cnst = 1.0/(sqrt(PI2)*mie_lsgm_com[n]);
          fact = -0.5/(mie_lsgm_com[n]*mie_lsgm_com[n]);
          break;
        case MIE_FUNC_GAMMA:
          indx = 1.0/mie_lsgm_com[n]-2.0;
          cnst = M_LNA/(pow(mie_rmod_com[n]*mie_lsgm_com[n],indx)*tgamma(indx));
          fact = -1.0/(mie_rmod_com[n]*mie_lsgm_com[n]);
          break;
        default:
          fprintf(stderr,"%s: error, mie_size_func[%d]=%c\n",fnam,n,mie_size_func[n]);
          err = 1;
          break;
      }
      if(err) break;
      lmin = mie_lmod_com[n]-mie_lsgm_com[n]*mie_wsgm;
      lmax = mie_lmod_com[n]+mie_lsgm_com[n]*mie_wsgm;
    }
    mie_vcom_com[n] = 0.0;
    for(i=0; i<mie_n_wlen; i++)
    {
      switch(mie_size_func[n])
      {
        case MIE_FUNC_FILE:
          switch(mie_size_ytype[n])
          {
            case MIE_YTYPE_NUMBER: // Number distribution
              break;
            case MIE_YTYPE_VOLUME: // Volume distribution
              break;
            default:
              fprintf(stderr,"%s: error, mie_size_ytype[%d]=%c\n",fnam,n,mie_size_ytype[n]);
              err = 1;
              break;
          }
          break;
        case MIE_FUNC_LOGNORMAL:
        case MIE_FUNC_EFF_LOGNORMAL:
          switch(mie_size_ytype[n])
          {
            case MIE_YTYPE_NUMBER: // Number distribution
              if(cnt_xmod == 2) // Volume mixing
              {
                norm = cnst*mie_lrad_stp[n][i];
                for(m=0,sw=0.0,sy=0.0; m<mie_n_step; m++)
                {
                  l = mie_lrad_min[n][i]+mie_lrad_stp[n][i]*m;
                  if(l < lmin) continue;
                  if(l > lmax) break;
                  r = pow(10.0,l);
                  x = PI2*r/(mie_wlen_um[i]);
                  y = norm*exp(fact*(l-mie_lmod_com[n])*(l-mie_lmod_com[n]));
                  w = y*r*r*r;
                  k = mie_n_step*i+m;
                  mie_aext_com[n][i] += mie_aext_tab[n][k]*y;
                  mie_asca_com[n][i] += mie_asca_tab[n][k]*y;
                  for(j=0,k=mie_n_angl*i,p=mie_n_step*mie_n_angl*i+mie_n_angl*m; j<mie_n_angl; j++,k++,p++)
                  {
                    mie_phs1_com[n][k] += mie_phs1_tab[n][p]*y;
                    mie_phs2_com[n][k] += mie_phs2_tab[n][p]*y;
                  }
                  sw += w;
                  sy += y;
                }
                mie_vcom_com[n] = MAX(sw/sy*PI4_3,mie_vcom_com[n]);
                sw = sy;
              }
              else // Number mixing
              {
                norm = cnst*mie_lrad_stp[n][i];
                for(m=0,sy=0.0; m<mie_n_step; m++)
                {
                  l = mie_lrad_min[n][i]+mie_lrad_stp[n][i]*m;
                  if(l < lmin) continue;
                  if(l > lmax) break;
                  r = pow(10.0,l);
                  x = PI2*r/(mie_wlen_um[i]);
                  y = norm*exp(fact*(l-mie_lmod_com[n])*(l-mie_lmod_com[n]));
                  k = mie_n_step*i+m;
                  mie_aext_com[n][i] += mie_aext_tab[n][k]*y;
                  mie_asca_com[n][i] += mie_asca_tab[n][k]*y;
                  for(j=0,k=mie_n_angl*i,p=mie_n_step*mie_n_angl*i+mie_n_angl*m; j<mie_n_angl; j++,k++,p++)
                  {
                    mie_phs1_com[n][k] += mie_phs1_tab[n][p]*y;
                    mie_phs2_com[n][k] += mie_phs2_tab[n][p]*y;
                  }
                  sy += y;
                }
                sw = sy;
              }
              break;
            case MIE_YTYPE_VOLUME: // Volume distribution
              norm = cnst*mie_lrad_stp[n][i];
              for(m=0,sw=0.0,sy=0.0; m<mie_n_step; m++)
              {
                l = mie_lrad_min[n][i]+mie_lrad_stp[n][i]*m;
                if(l < lmin) continue;
                if(l > lmax) break;
                r = pow(10.0,l);
                x = PI2*r/(mie_wlen_um[i]);
                w = norm*exp(fact*(l-mie_lmod_com[n])*(l-mie_lmod_com[n]));
                y = w/(PI4_3*r*r*r);
                k = mie_n_step*i+m;
                mie_aext_com[n][i] += mie_aext_tab[n][k]*y;
                mie_asca_com[n][i] += mie_asca_tab[n][k]*y;
                for(j=0,k=mie_n_angl*i,p=mie_n_step*mie_n_angl*i+mie_n_angl*m; j<mie_n_angl; j++,k++,p++)
                {
                  mie_phs1_com[n][k] += mie_phs1_tab[n][p]*y;
                  mie_phs2_com[n][k] += mie_phs2_tab[n][p]*y;
                }
                sw += w;
                sy += y;
              }
              mie_vcom_com[n] = MAX(sw/sy,mie_vcom_com[n]);
              break;
            default:
              fprintf(stderr,"%s: error, mie_size_ytype[%d]=%c\n",fnam,n,mie_size_ytype[n]);
              err = 1;
              break;
          }
          break;
        case MIE_FUNC_GAMMA:
          switch(mie_size_ytype[n])
          {
            case MIE_YTYPE_NUMBER: // Number distribution
              if(cnt_xmod == 2) // Volume mixing
              {
                norm = cnst*mie_lrad_stp[n][i];
                for(m=0,sw=0.0,sy=0.0; m<mie_n_step; m++)
                {
                  l = mie_lrad_min[n][i]+mie_lrad_stp[n][i]*m;
                  if(l < lmin) continue;
                  if(l > lmax) break;
                  r = pow(10.0,l);
                  x = PI2*r/(mie_wlen_um[i]);
                  y = norm*pow(r,indx)*exp(fact*r);
                  w = y*r*r*r;
                  k = mie_n_step*i+m;
                  mie_aext_com[n][i] += mie_aext_tab[n][k]*y;
                  mie_asca_com[n][i] += mie_asca_tab[n][k]*y;
                  for(j=0,k=mie_n_angl*i,p=mie_n_step*mie_n_angl*i+mie_n_angl*m; j<mie_n_angl; j++,k++,p++)
                  {
                    mie_phs1_com[n][k] += mie_phs1_tab[n][p]*y;
                    mie_phs2_com[n][k] += mie_phs2_tab[n][p]*y;
                  }
                  sw += w;
                  sy += y;
                }
                mie_vcom_com[n] = MAX(sw/sy*PI4_3,mie_vcom_com[n]);
                sw = sy;
              }
              else // Number mixing
              {
                norm = cnst*mie_lrad_stp[n][i];
                for(m=0,sy=0.0; m<mie_n_step; m++)
                {
                  l = mie_lrad_min[n][i]+mie_lrad_stp[n][i]*m;
                  if(l < lmin) continue;
                  if(l > lmax) break;
                  r = pow(10.0,l);
                  x = PI2*r/(mie_wlen_um[i]);
                  y = norm*pow(r,indx)*exp(fact*r);
                  k = mie_n_step*i+m;
                  mie_aext_com[n][i] += mie_aext_tab[n][k]*y;
                  mie_asca_com[n][i] += mie_asca_tab[n][k]*y;
                  for(j=0,k=mie_n_angl*i,p=mie_n_step*mie_n_angl*i+mie_n_angl*m; j<mie_n_angl; j++,k++,p++)
                  {
                    mie_phs1_com[n][k] += mie_phs1_tab[n][p]*y;
                    mie_phs2_com[n][k] += mie_phs2_tab[n][p]*y;
                  }
                  sy += y;
                }
                sw = sy;
              }
              break;
            case MIE_YTYPE_VOLUME: // Volume distribution
              norm = cnst*mie_lrad_stp[n][i];
              for(m=0,sw=0.0,sy=0.0; m<mie_n_step; m++)
              {
                l = mie_lrad_min[n][i]+mie_lrad_stp[n][i]*m;
                if(l < lmin) continue;
                if(l > lmax) break;
                r = pow(10.0,l);
                x = PI2*r/(mie_wlen_um[i]);
                w = norm*pow(r,indx)*exp(fact*r);
                y = w/(PI4_3*r*r*r);
                k = mie_n_step*i+m;
                mie_aext_com[n][i] += mie_aext_tab[n][k]*y;
                mie_asca_com[n][i] += mie_asca_tab[n][k]*y;
                for(j=0,k=mie_n_angl*i,p=mie_n_step*mie_n_angl*i+mie_n_angl*m; j<mie_n_angl; j++,k++,p++)
                {
                  mie_phs1_com[n][k] += mie_phs1_tab[n][p]*y;
                  mie_phs2_com[n][k] += mie_phs2_tab[n][p]*y;
                }
                sw += w;
                sy += y;
              }
              mie_vcom_com[n] = MAX(sw/sy,mie_vcom_com[n]);
              break;
            default:
              fprintf(stderr,"%s: error, mie_size_ytype[%d]=%c\n",fnam,n,mie_size_ytype[n]);
              err = 1;
              break;
          }
          break;
        default:
          fprintf(stderr,"%s: error, mie_size_func[%d]=%c\n",fnam,n,mie_size_func[n]);
          err = 1;
          break;
      }
      if(err) break;
      if(fabs(sw-1.0) > epsilon)
      {
        fprintf(stderr,"%s: warning, sw=%13.6e\n",fnam,sw);
      } else
      if(cnt_vb > 1)
      {
        fprintf(stderr,"n=%2d, i=%3d, sw=%13.6e\n",n,i,sw);
      }
      sinv = 1.0/sy; // average per particle
      mie_aext_com[n][i] *= sinv;
      mie_asca_com[n][i] *= sinv;
      for(j=0,k=mie_n_angl*i; j<mie_n_angl; j++,k++)
      {
        mie_phas_com[n][k] = 0.5*(mie_phs1_com[n][k]+mie_phs2_com[n][k]);
      }
      sp = 0.0;
      for(j=1,k=mie_n_angl*i+1; j<mie_n_angl; j++,k++)
      {
        sp += (mie_phas_com[n][k-1]*mie_angl_sin[j-1]+mie_phas_com[n][k]*mie_angl_sin[j])*mie_angl_dif[j];
      }
      sinv = 1.0/sp; // normalized for natural light
      for(j=0,k=mie_n_angl*i; j<mie_n_angl; j++,k++)
      {
        mie_phs1_com[n][k] *= sinv;
        mie_phs2_com[n][k] *= sinv;
        mie_phas_com[n][k] *= sinv;
      }
      for(j=1,k=mie_n_angl*i+1; j<mie_n_angl; j++,k++)
      {
        mie_asym_com[n][i] += (mie_phas_com[n][k-1]*mie_angl_sin[j-1]*mie_angl_cos[j-1]+
                               mie_phas_com[n][k]*mie_angl_sin[j]*mie_angl_cos[j])*mie_angl_dif[j];
      }
    }
    if(err) break;
  }
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
  double sp;
  double sinv;

  // calculate number mixing ratio
  if(cnt_xmod == 2)
  {
    for(r=0.0,n=0; n<mie_n_comp; n++)
    {
      mie_mixr_com[n] = mie_wcom_com[n]/mie_vcom_com[n];
      r += mie_mixr_com[n];
    }
    for(n=0; n<mie_n_comp; n++)
    {
      mie_mixr_com[n] /= r;
      if(cnt_vb > 2)
      {
        message(stderr,"Component#   : %2d\n",n);
        message(stderr,"Mixing ratio : %22.14e (%22.14e)\n",mie_mixr_com[n],mie_wcom_com[n]);
      }
    }
  } else
  if(cnt_xmod == 1)
  {
    for(r=0.0,n=0; n<mie_n_comp; n++)
    {
      mie_mixr_com[n] = mie_wcom_com[n]/mie_aext_com[n][mie_iref];
      r += mie_mixr_com[n];
    }
    for(n=0; n<mie_n_comp; n++)
    {
      mie_mixr_com[n] /= r;
      if(cnt_vb > 2)
      {
        message(stderr,"Component#   : %2d\n",n);
        message(stderr,"Mixing ratio : %22.14e (%22.14e)\n",mie_mixr_com[n],mie_wcom_com[n]);
      }
    }
  }
  else
  {
    for(n=0; n<mie_n_comp; n++)
    {
      mie_mixr_com[n] = mie_wcom_com[n];
    }
  }

  for(i=0; i<mie_n_wlen; i++)
  {
    sw = se = ss = sa = 0.0;
    for(n=0; n<mie_n_comp; n++)
    {
      sw += mie_mixr_com[n];
      se += mie_mixr_com[n]*mie_aext_com[n][i];
      ws  = mie_mixr_com[n]*mie_asca_com[n][i];
      ss += ws;
      sa += ws*mie_asym_com[n][i];
    }
    mie_aext[i] = se/sw;
    mie_asca[i] = ss/sw;
    mie_asym[i] = sa/ss;
    for(j=0,k=mie_n_angl*i; j<mie_n_angl; j++,k++)
    {
      s1 = s2 = s3 = 0.0;
      for(n=0; n<mie_n_comp; n++)
      {
        ws  = mie_mixr_com[n]*mie_asca_com[n][i];
        s1 += ws*mie_phs1_com[n][k];
        s2 += ws*mie_phs2_com[n][k];
        s3 += ws*mie_phas_com[n][k];
      }
      mie_phs1[k] = s1;
      mie_phs2[k] = s2;
      mie_phas[k] = s3;
    }
  }

  for(i=0; i<mie_n_wlen; i++)
  {
    sp = 0.0;
    for(j=1,k=mie_n_angl*i+1; j<mie_n_angl; j++,k++)
    {
      sp += (mie_phas[k-1]*mie_angl_sin[j-1]+mie_phas[k]*mie_angl_sin[j])*mie_angl_dif[j];
    }
    sinv = 1.0/sp;
    for(j=0,k=mie_n_angl*i; j<mie_n_angl; j++,k++)
    {
      mie_phs1[k] *= sinv;
      mie_phs2[k] *= sinv;
      mie_phas[k] *= sinv;
    }
  }

  return 0;
}

int ReadMie(int iaer)
{
  int i,j,k;
  int err;
  double v[5];
  double phs1[MIE_MAXDATA];
  double phs2[MIE_MAXDATA];
  double phas[MIE_MAXDATA];
  double *dp;
  char line[MAXLINE];
  char str[5][MAXLINE];
  char fnam[] = "ReadMie";
  char *p;
  FILE *fp;

  // allocate memory
  mie_aext = (double *)malloc(MIE_MAXDATA*sizeof(double));
  mie_asca = (double *)malloc(MIE_MAXDATA*sizeof(double));
  mie_asym = (double *)malloc(MIE_MAXDATA*sizeof(double));
  if(mie_aext==NULL || mie_asca==NULL || mie_asym==NULL)
  {
    fprintf(stderr,"%s: failed in allocating memory\n",fnam);
    return -1;
  }
  // read data
  err = 0;
  do
  {
    if((fp=fopen(mie_aers_inp1[iaer],"r")) == NULL)
    {
      fprintf(stderr,"%s: cannot open %s\n",fnam,mie_aers_inp1[iaer]);
      err = 1;
      break;
    }
    i = 0;
    while(fgets(line,MAXLINE,fp) != NULL)
    {
      if(sscanf(line,"%s%s%s%s%s",str[0],str[1],str[2],str[3],str[4]) != 5)
      {
        fprintf(stderr,"%s: read error (%s) >>> %s\n",fnam,mie_aers_inp1[iaer],line);
        err = 1;
        break;
      }
      errno = 0;
      for(j=0; j<5; j++)
      {
        v[j] = strtod(str[j],&p);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,mie_aers_inp1[iaer],line);
          err = 1;
          break;
        }
      }
      if(err) break;
      if(i >= MIE_MAXDATA)
      {
        fprintf(stderr,"%s: warning, #data exceed the limit %d (%s)\n",fnam,i,mie_aers_inp1[iaer]);
        break;
      }
      mie_wlen[i] = v[0];
      mie_aext[i] = v[1];
      mie_asca[i] = v[3]*v[1];
      mie_asym[i] = v[4];
      i++;
    }
    fclose(fp);
    if(err)
    {
      break;
    }
    mie_n_wlen = i;
    if(cnt_vb > 1)
    {
      fprintf(stderr,"%s: %d data have been read (%s)\n",fnam,mie_n_wlen,mie_aers_inp1[iaer]);
    }
    if(mie_n_wlen < 1)
    {
      fprintf(stderr,"%s: error, mie_n_wlen=%d (%s)\n",fnam,mie_n_wlen,mie_aers_inp1[iaer]);
      err = 1;
      break;
    }
    // resize memory
    if(mie_n_wlen != MIE_MAXDATA)
    {
      do
      {
        if((dp=(double*)realloc(mie_aext,mie_n_wlen*sizeof(double))) == NULL)
        {
          fprintf(stderr,"%s: error in allocating memory.\n",fnam);
          err = 1;
          break;
        }
        else
        {
          mie_aext = dp;
        }
        if((dp=(double*)realloc(mie_asca,mie_n_wlen*sizeof(double))) == NULL)
        {
          fprintf(stderr,"%s: error in allocating memory.\n",fnam);
          err = 1;
          break;
        }
        else
        {
          mie_asca = dp;
        }
        if((dp=(double*)realloc(mie_asym,mie_n_wlen*sizeof(double))) == NULL)
        {
          fprintf(stderr,"%s: error in allocating memory.\n",fnam);
          err = 1;
          break;
        }
        else
        {
          mie_asym = dp;
        }
      }
      while(0);
      if(err)
      {
        break;
      }
    }
    // read phase function data
    if((fp=fopen(mie_aers_inp2[iaer],"r")) == NULL)
    {
      fprintf(stderr,"%s: cannot open %s\n",fnam,mie_aers_inp2[iaer]);
      err = 1;
      break;
    }
    i = 0;
    while(fgets(line,MAXLINE,fp) != NULL)
    {
      if(sscanf(line,"%s%s%s%s%s",str[0],str[1],str[2],str[3],str[4]) != 5)
      {
        fprintf(stderr,"%s: read error (%s) >>> %s\n",fnam,mie_aers_inp2[iaer],line);
        err = 1;
        break;
      }
      errno = 0;
      for(j=0; j<5; j++)
      {
        v[j] = strtod(str[j],&p);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,mie_aers_inp2[iaer],line);
          err = 1;
          break;
        }
      }
      if(err) break;
      if(i >= MIE_MAXDATA)
      {
        fprintf(stderr,"%s: warning, #data exceed the limit %d (%s)\n",fnam,i,mie_aers_inp2[iaer]);
        break;
      }
      if(v[0] != mie_wlen[0])
      {
        ungets(line,strlen(line),fp);
        break;
      }
      mie_angl[i] = v[1];
      phs1[i] = v[2];
      phs2[i] = v[3];
      phas[i] = v[4];
      i++;
    }
    if(err)
    {
      fclose(fp);
      break;
    }
    mie_n_angl = i;
    if(mie_n_angl < 1)
    {
      fprintf(stderr,"%s: error, mie_n_angl=%d (%s)\n",fnam,mie_n_angl,mie_aers_inp2[iaer]);
      err = 1;
      break;
    }
    mie_phs1 = (double *)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
    mie_phs2 = (double *)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
    mie_phas = (double *)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
    if(mie_phs1==NULL || mie_phs2==NULL || mie_phas==NULL)
    {
      fprintf(stderr,"%s: failed in allocating memory\n",fnam);
      err = 1;
      break;
    }
    for(j=0; j<mie_n_angl; j++)
    {
      mie_phs1[j] = phs1[j];
      mie_phs2[j] = phs2[j];
      mie_phas[j] = phas[j];
    }
    for(i=1; i<mie_n_wlen; i++)
    {
      for(j=0; j<mie_n_angl; j++)
      {
        if(fgets(line,MAXLINE,fp) == NULL)
        {
          fprintf(stderr,"%s: read error (%s) >>> i=%d, j=%d\n",fnam,mie_aers_inp2[iaer],i,j);
          err = 1;
          break;
        }
        if(sscanf(line,"%s%s%s%s%s",str[0],str[1],str[2],str[3],str[4]) != 5)
        {
          fprintf(stderr,"%s: read error (%s) >>> %s\n",fnam,mie_aers_inp2[iaer],line);
          err = 1;
          break;
        }
        errno = 0;
        for(k=0; k<5; k++)
        {
          v[k] = strtod(str[k],&p);
          if(errno==ERANGE || *p!='\0')
          {
            fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,mie_aers_inp2[iaer],line);
            err = 1;
            break;
          }
        }
        if(err) break;
        if(v[0] != mie_wlen[i])
        {
          fprintf(stderr,"%s: error, v[0]=%s, mie_wlen[%d]=%9.4f\n",fnam,str[0],i,mie_wlen[i]);
          err = 1;
          break;
        }
        if(v[1] != mie_angl[j])
        {
          fprintf(stderr,"%s: error, v[1]=%s, mie_angl[%d]=%9.4f\n",fnam,str[1],j,mie_angl[j]);
          err = 1;
          break;
        }
        mie_phs1[i*mie_n_angl+j] = v[2];
        mie_phs2[i*mie_n_angl+j] = v[3];
        mie_phas[i*mie_n_angl+j] = v[4];
      }
    }
    if(fgets(line,MAXLINE,fp) != NULL)
    {
      fprintf(stderr,"%s: error, extra data detected (%s)\n",fnam,mie_aers_inp2[iaer]);
      err = 1;
    }
    fclose(fp);
    if(err)
    {
      break;
    }
  }
  while(0);
  if(err)
  {
    free(mie_aext);
    free(mie_asca);
    free(mie_asym);
    free(mie_phs1);
    free(mie_phs2);
    free(mie_phas);
    return -1;
  }

  mie_iref = -1;
  for(i=0; i<mie_n_wlen; i++)
  {
    if(fabs(mie_wlen[i]-mie_wlen_ref) < EPSILON)
    {
      mie_iref = i;
      break;
    }
  }
  if(mie_iref < 0)
  {
    fprintf(stderr,"%s: failed in finding reference wavelength %13.6e\n",fnam,mie_wlen_ref);
    return -1;
  }

  mie_wlen_um = (double *)malloc(mie_n_wlen*sizeof(double));
  if(mie_wlen_um == NULL)
  {
    fprintf(stderr,"%s: failed in allocating memory\n",fnam);
    return -1;
  }
  for(i=0; i<mie_n_wlen; i++)
  {
    mie_wlen_um[i] = mie_wlen[i]*1.0e-3;
  }

  mie_angl_rad = (double *)malloc(mie_n_angl*sizeof(double));
  mie_angl_sin = (double *)malloc(mie_n_angl*sizeof(double));
  mie_angl_cos = (double *)malloc(mie_n_angl*sizeof(double));
  mie_angl_dif = (double *)malloc(mie_n_angl*sizeof(double));
  if(mie_angl_rad==NULL || mie_angl_sin==NULL || mie_angl_cos==NULL || mie_angl_dif==NULL)
  {
    fprintf(stderr,"%s: failed in allocating memory\n",fnam);
    return -1;
  }
  for(j=0; j<mie_n_angl; j++)
  {
    mie_angl_rad[j] = mie_angl[j]*D_TO_R;
    mie_angl_sin[j] = sin(mie_angl_rad[j]);
    mie_angl_cos[j] = cos(mie_angl_rad[j]);
  }
  for(j=1; j<mie_n_angl; j++)
  {
    mie_angl_dif[j] = PI*fabs(mie_angl_rad[j]-mie_angl_rad[j-1]);
  }

  if(cnt_vb > 1)
  {
    for(i=0; i<mie_n_wlen; i++)
    {
      fprintf(stderr,"%9.4f %13.6e %13.6e %13.6e %13.6e\n",
                      mie_wlen[i],mie_aext[i],mie_aext[i]/mie_aext[mie_iref],
                      mie_asca[i]/mie_aext[i],mie_asym[i]);
    }
    for(i=0; i<mie_n_wlen; i++)
    {
      for(j=0; j<mie_n_angl; j++)
      {
        k = mie_n_angl*i+j;
        fprintf(stderr,"%9.4f %9.4f %13.6e %13.6e %13.6e\n",mie_wlen[i],mie_angl[j],mie_phs1[k],mie_phs2[k],mie_phas[k]);
      }
    }
  }

  return 0;
}

int SetMie1(int iaer)
{
  char fnam[] = "SetMie1";

  if(iaer<0 || iaer>3)
  {
    fprintf(stderr,"%s: error, iaer=%d\n",fnam,iaer);
    return -1;
  }
  sim_n_aers_wlen[iaer] = mie_n_wlen;
  sim_aers_wlen_um[iaer] = (double *)malloc(sim_n_aers_wlen[iaer]*sizeof(double));
  sim_aers_cext[iaer] = (double *)malloc(sim_n_aers_wlen[iaer]*sizeof(double));
  sim_aers_cabs[iaer] = (double *)malloc(sim_n_aers_wlen[iaer]*sizeof(double));
  sim_aers_asym[iaer] = (double *)malloc(sim_n_aers_wlen[iaer]*sizeof(double));
  if(sim_aers_wlen_um[iaer]==NULL || sim_aers_cext[iaer]==NULL ||
     sim_aers_cabs[iaer]==NULL || sim_aers_asym[iaer]==NULL)
  {
    fprintf(stderr,"%s: failed in allocating memory\n",fnam);
    return -1;
  }
  for(i=0; i<sim_n_aers_wlen[iaer]; i++)
  {
    sim_aers_wlen_um[i] = mie_wlen_um[i];
    sim_aers_cext[i] = mie_aext[i]/mie_aext[mie_iref];
    sim_aers_cabs[i] = (mie_aext[i]-mie_asca[i])/mie_aext[mie_iref];
    sim_aers_asym[i] = mie_asym[i];
  }

  return 0;
}

int SetMie2(int iaer)
{
  int n;
  char fnam[] = "SetMie2";

  if(iaer<0 || iaer>3)
  {
    fprintf(stderr,"%s: error, iaer=%d\n",fnam,iaer);
    return -1;
  }

  sim_phas_wlen_um = (double *)malloc(sim_n_phas_wlen*sizeof(double));
  sim_aers_phas[iaer] = (double *)malloc(sim_n_phas_wlen*sim_n_phas_angl*sizeof(double));
  if(sim_phas_wlen_um==NULL || sim_aers_phas[iaer]==NULL)
  {
    fprintf(stderr,"%s: failed in allocating memory\n",fnam);
    return -1;
  }
  for(i=0; i<sim_n_phas_wlen; i++)
  {
    sim_phas_wlen_um[i] = sim_phas_wlen[i]*1.0e-3;
  }

  if(Interp2D(mie_wlen,mie_angl,mie_phas,mie_n_wlen,mie_n_angl,sim_phas_wlen,sim_phas_angl,sim_aers_phas[iaer],sim_n_phas_wlen,sim_n_phas_angl,1) < 0)
  {
    return -1;
  }

  for(n=0; n<mie_n_comp; n++)
  {
    free(mie_refr_com[n]);
    free(mie_refi_com[n]);
    free(mie_aext_com[n]);
    free(mie_asca_com[n]);
    free(mie_asym_com[n]);
    free(mie_phs1_com[n]);
    free(mie_phs2_com[n]);
    free(mie_phas_com[n]);
    free(mie_aext_tab[n]);
    free(mie_asca_tab[n]);
    free(mie_phs1_tab[n]);
    free(mie_phs2_tab[n]);
    free(mie_lrad_min[n]);
    free(mie_lrad_max[n]);
    free(mie_lrad_stp[n]);
  }
  free(mie_wlen_um);
  free(mie_aext);
  free(mie_asca);
  free(mie_asym);
  free(mie_angl_rad);
  free(mie_angl_sin);
  free(mie_angl_cos);
  free(mie_angl_dif);
  free(mie_phs1);
  free(mie_phs2);
  free(mie_phas);

  return 0;
}

int MiePrintout(void)
{
  int i,j,k,n;
  char fnam[] = "MiePrintout";
  FILE *fp;

  if((fp=fopen(mie_out0,"w")) == NULL)
  {
    fprintf(stderr,"%s: cannot open %s\n",fnam,mie_out0);
  }
  else
  {
    for(n=0; n<mie_n_comp; n++)
    {
      fprintf(fp,"%22.14e %22.14e\n",mie_wcom_com[n],mie_mixr_com[n]);
    }
    for(n=0; n<mie_n_comp; n++)
    {
      for(i=0; i<mie_n_wlen; i++)
      {
        fprintf(fp,"%9.4f %13.5e %13.5e\n",mie_wlen[i],mie_refr_com[n][i],mie_refi_com[n][i]);
      }
    }
    fclose(fp);
  }

  if((fp=fopen(mie_out1,"w")) == NULL)
  {
    fprintf(stderr,"%s: cannot open %s\n",fnam,mie_out1);
  }
  else
  {
    for(i=0; i<mie_n_wlen; i++)
    {
      fprintf(fp,"%9.4f %13.6e %13.6e %13.6e %13.6e\n",mie_wlen[i],mie_aext[i],mie_aext[i]/mie_aext[mie_iref],
                                                       mie_asca[i]/mie_aext[i],mie_asym[i]);
    }
    fclose(fp);
  }

  if((fp=fopen(mie_out2,"w")) == NULL)
  {
    fprintf(stderr,"%s: cannot open %s\n",fnam,mie_out2);
  }
  else
  {
    for(i=0; i<mie_n_wlen; i++)
    {
      for(j=0; j<mie_n_angl; j++)
      {
        k = mie_n_angl*i+j;
        fprintf(fp,"%9.4f %9.4f %13.6e %13.6e %13.6e\n",mie_wlen[i],mie_angl[j],mie_phs1[k],mie_phs2[k],mie_phas[k]);
      }
    }
    fclose(fp);
  }

  return 0;
}

int upper_pfunc(int iaer)
{
  int i,j,k;
  int i_1,i_2;
  double p1;
  double *tropo_phas_tmp1 = NULL;
  double *tropo_phas_tmp2 = NULL;
  char fnam[] = "upper_pfunc";

  if(iaer<1 || iaer>3)
  {
    fprintf(stderr,"%s: error, iaer=%d\n",fnam,iaer);
    return -1;
  }

  mie_phas = (double*)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
  if(mie_phas == NULL)
  {
    fprintf(stderr,"%s: error in allocating memory\n",fnam);
    return -1;
  }

  switch(iaer)
  {
    case 1:
      i_1 = i_2 = -1;
      for(i=UPP_N_RHUM-1; i>=0; i--)
      {
        if(upp_rhum[i] <= upp_rh)
        {
          i_1 = i;
          break;
        }
      }
      if(i_1 < 0) i_1 = 0;
      for(i=0; i<UPP_N_RHUM; i++)
      {
        if(upp_rhum[i] >= upp_rh)
        {
          i_2 = i;
          break;
        }
      }
      if(i_2 < 0) i_2 = UPP_N_RHUM-1;
      if(i_1 != i_2)
      {
        tropo_phas_tmp1 = (double*)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
        tropo_phas_tmp2 = (double*)malloc(mie_n_wlen*mie_n_angl*sizeof(double));
        Interp2D(upp_wlen,upp_angl,upp_tropo_phas[i_1],UPP_N_WLEN,UPP_N_ANGL,mie_wlen,mie_angl,tropo_phas_tmp1,mie_n_wlen,mie_n_angl,1);
        Interp2D(upp_wlen,upp_angl,upp_tropo_phas[i_2],UPP_N_WLEN,UPP_N_ANGL,mie_wlen,mie_angl,tropo_phas_tmp2,mie_n_wlen,mie_n_angl,0);
        for(i=0; i<mie_n_wlen; i++)
        {
          for(j=0; j<mie_n_angl; j++)
          {
            k = mie_n_angl*i+j;
            mie_phas[k] = INTERP(upp_rh,upp_rhum[i_1],upp_rhum[i_2],tropo_phas_tmp1[k],tropo_phas_tmp2[k]);
          }
        }
        free(tropo_phas_tmp1);
        free(tropo_phas_tmp2);
      }
      else
      {
        Interp2D(upp_wlen,upp_angl,upp_tropo_phas[i_1],UPP_N_WLEN,UPP_N_ANGL,mie_wlen,mie_angl,mie_phas,mie_n_wlen,mie_n_angl,1);
      }
      break
    case 2:
      Interp2D(upp_wlen,upp_angl,upp_strat_phas,UPP_N_WLEN,UPP_N_ANGL,mie_wlen,mie_angl,mie_phas,mie_n_wlen,mie_n_angl,1);
      break
    case 3:
      Interp2D(upp_wlen,upp_angl,upp_meteo_phas,UPP_N_WLEN,UPP_N_ANGL,mie_wlen,mie_angl,mie_phas,mie_n_wlen,mie_n_angl,1);
      break
    default:
      fprintf(stderr,"%s: error, iaer=%d\n",fnam,iaer);
      return -1;
      break
  }

  for(i=0; i<mie_n_wlen; i++)
  {
    p1 = 0.0;
    for(j=1; j<mie_n_angl; j++)
    {
      k = mie_n_angl*i+j;
      p1 += (mie_phas[k-1]*mie_angl_sin[j-1]+mie_phas[k]*mie_angl_sin[j])*mie_angl_dif[j];
    }
    for(j=0; j<mie_n_angl; j++)
    {
      k = mie_n_angl*i+j;
      mie_phas[k] /= p1;
    }
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
  for(i=0; i<PRF_NMOLX; i++)
  {
    sim_prf_psclx[i] = 0;
    sim_prf_vsclx[i] = NAN;
    for(j=0; j<prf_n_alt; j++)
    {
      sim_prf_xmol[i][j] = 0.0;
    }
  }
  prf_jcharx = 'A';
  sim_prf_jcharx = (char)('0'+imod);
  if(cnt_version > MOD_V4)
  {
    for(i=0; i<PRF_NMOLY; i++)
    {
      sim_prf_pscly[i] = 0;
      sim_prf_vscly[i] = NAN;
      for(j=0; j<prf_n_alt; j++)
      {
        sim_prf_ymol[i][j] = 0.0;
      }
    }
    prf_jchary = 'A';
    sim_prf_jchary = '1';
  }

  for(i=0; i<PRF_NPAR; i++)
  {
    prf_jchar[i] = 'A';
    sim_prf_jchar[i] = (char)('0'+imod);
  }

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
    sim_2c |= 0x0001;
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
        if(i > 2) sim_2c |= 0x0010;
        for(j=0; j<prf_n_alt; j++)
        {
          sim_prf_wmol[i][j] = prf_wmol[i][j]*f;
        }
        sim_prf_jchar[i+2] = prf_jchar[i+2];
      }
    }
    for(i=0; i<PRF_NMOLX; i++)
    {
      if(sim_prf_psclx[i] != 0)
      {
        sim_2c |= 0x0100;
        for(j=0; j<prf_n_alt; j++)
        {
          sim_prf_xmol[i][j] = prf_xmol[i][j]*f;
        }
        sim_prf_jcharx = prf_jcharx;
      }
    }
    if(cnt_version > MOD_V4)
    {
      for(i=0; i<PRF_NMOLY; i++)
      {
        if(sim_prf_pscly[i] != 0)
        {
          sim_2c |= 0x0200;
          for(j=0; j<prf_n_alt; j++)
          {
            sim_prf_ymol[i][j] = prf_ymol[i][j]*f;
          }
          sim_prf_jchary = prf_jchary;
        }
      }
    }
  }

  if(!isnan(prf_temp_gnd))
  {
    sim_2c |= 0x0001;
    f = prf_temp_gnd/prf_temp[0];
    for(j=0; j<prf_n_alt; j++)
    {
      sim_prf_temp[j] = prf_temp[j]*f;
    }
    sim_prf_jchar[1] = prf_jchar[1];
  }

  return 0;
}

int PrfScaleMolecule(void)
{
  int i,j;

  for(i=0; i<PRF_NMOL; i++)
  {
    if(!isnan(sim_prf_vscl[i]))
    {
      sim_2c |= 0x0001;
      if(i > 2) sim_2c |= 0x0010;
      for(j=0; j<prf_n_alt; j++)
      {
        sim_prf_wmol[i][j] = prf_wmol[i][j]*sim_prf_vscl[i];
      }
      sim_prf_jchar[i+2] = prf_jchar[i+2];
    }
  }
  for(i=0; i<PRF_NMOLX; i++)
  {
    if(!isnan(sim_prf_vsclx[i]))
    {
      sim_2c |= 0x0100;
      for(j=0; j<prf_n_alt; j++)
      {
        sim_prf_xmol[i][j] = prf_xmol[i][j]*sim_prf_vsclx[i];
      }
      sim_prf_jcharx = prf_jcharx;
    }
  }
  if(cnt_version > MOD_V4)
  {
    for(i=0; i<PRF_NMOLY; i++)
    {
      if(!isnan(sim_prf_vscly[i]))
      {
        sim_2c |= 0x0200;
        for(j=0; j<prf_n_alt; j++)
        {
          sim_prf_ymol[i][j] = prf_ymol[i][j]*sim_prf_vscly[i];
        }
        sim_prf_jchary = prf_jchary;
      }
    }
  }

  return 0;
}

int PrfScaleAerosol(double t)
{
  int i,j;
  double f;

  if(!isnan(t))
  {
    sim_2c |= 0x1000;
    if(prf_aod_lo < 1.0e-100)
    {
      f = 0.0;
    }
    else
    {
      f = (t-prf_aod_hi)/prf_aod_lo;
      if(f < 0.0)
      {
        f = 0.0;
      }
    }
    for(i=0; i<PRF_NHAZ_LOW; i++)
    {
      for(j=0; j<prf_n_alt; j++)
      {
        sim_prf_haze[i][j] = prf_haze[i][j]*f;
      }
    }
    for(i=PRF_NHAZ_LOW; i<PRF_NHAZ; i++)
    {
      for(j=0; j<prf_n_alt; j++)
      {
        sim_prf_haze[i][j] = prf_haze[i][j];
      }
    }
  }

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
    if(fabs(v[0]-par[0]) < sim_dmax)
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
  nstp = (int)((xmax-xmin)/xstp+offset);
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
  nwid = (int)(xsgm*wsgm/xstp+offset);
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

int Interp2D(const double *x1,const double *y1,const double *z1,int nx1,int ny1,
             const double *x2,const double *y2,      double *z2,int nx2,int ny2,int f)
{
  // x1[nx1],y1[ny1],z1[nx1*ny1]
  // x2[nx2],y2[ny2],z2[nx2*ny2]
  // x1,y1 must be arranged in ascending order
  int i,j,k;
  static int flag = -9999;
  static int *i_1 = NULL;
  static int *i_2 = NULL;
  static int *j_1 = NULL;
  static int *j_2 = NULL;
  int k_1,k_2;
  double p1,p2;

  if(f<=0 || f!=flag)
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
    flag = abs(f);
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
  double epsilon;
  char fnam[] = "Sampling";

  // check input
  if(ninp < 2)
  {
    fprintf(stderr,"%s: error, ninp=%d\n",fnam,ninp);
    return -1;
  }
  // check whether data interval is constant or not
  xstp = xinp[1]-xinp[0];
  epsilon = xstp*DELTA;
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
      i1 = (int)((xout[i]-xinp[0])/xstp+DELTA);
      if(i1 < 0) i1 = 0;
      i2 = i1+1;
      if(i2 >= ninp)
      {
        i2 = ninp-1;
        i1 = i2-1;
      }
      yout[i] = (yinp[i1]+(yinp[i2]-yinp[i1])*(xout[i]-xinp[i1])/xstp)*yuni;
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
  double epsilon;
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
  epsilon = xstp*DELTA;
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
      i1 = (int)((xout[i]-xinp[0])/xstp+DELTA);
      if(i1 < 0) i1 = 0;
      i2 = i1+1;
      if(i2 >= ninp)
      {
        i2 = ninp-1;
        i1 = i2-1;
      }
      if(yinp[i1]<delta || yinp[i2]<delta)
      {
        yout[i] = (yinp[i1]+(yinp[i2]-yinp[i1])*(xout[i]-xinp[i1])/xstp)*yuni;
      }
      else
      {
        yout[i] = (yinp[i1]*pow(yinp[i2]/yinp[i1],(xout[i]-xinp[i1])/xstp))*yuni;
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
  if(sim_prf_mode == SIM_PRF_CUSTOM)
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
  char str[MAXITEM][MAXLINE];
  char fnam[] = "ReadConfig";
  char *p;
  FILE *fp;

  if(strcmp(cnt_conf,"")==0 || strcmp(cnt_conf,NONAME)==0)
  {
    if(PreConfig() < 0) return -1;
    if(PostConfig() < 0) return -1;
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
    for(n=nc=0,p=temp; n<MAXITEM; n++,p+=nc)
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
          if((prf_n_alt=Read1A(str[1],PRF_MAXNALT,num,uni,NULL,prf_alt)) < 1)
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
    if(strcasecmp(str[0],"mie_wlen") == 0)
    {
      num = 0;
      uni = 1.0;
      mie_wlen_min = MIE_WLEN_MIN;
      mie_wlen_max = MIE_WLEN_MAX;
      if(n > 2)
      {
        errno = 0;
        num = strtol(str[2],&p,10);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"%s: convert error >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 3)
      {
        errno = 0;
        uni = strtod(str[3],&p);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"%s: convert error >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 4)
      {
        errno = 0;
        mie_wlen_min = strtod(str[4],&p);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"%s: convert error >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 5)
      {
        errno = 0;
        mie_wlen_max = strtod(str[5],&p);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"%s: convert error >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 1)
      {
        if(strcmp(str[1],NONAME) != 0)
        {
          if((mie_n_wlen=Read1A(str[1],MIE_MAXDATA,num,uni,WaveSelect,mie_wlen)) < 1)
          {
            err = 1;
            break;
          }
        }
      }
      if(cnt_hp && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s %4d %13.6e %13.4f %13.4f\n",
                                               str[0],str[1],num,uni,mie_wlen_min,mie_wlen_max);
        cnt_n_cmnt++;
      }
    }
    if((strcasecmp(str[0],"sim_prf_vscl") == 0) ||
       (strcasecmp(str[0],"sim_prf_pscl") == 0) ||
       (strcasecmp(str[0],"sim_prf_vsclx") == 0) ||
       (strcasecmp(str[0],"sim_prf_psclx") == 0) ||
       (strcasecmp(str[0],"sim_prf_vscly") == 0) ||
       (strcasecmp(str[0],"sim_prf_pscly") == 0) ||
       (strcasecmp(str[0],"prf_falt") == 0) ||
       (strcasecmp(str[0],"prf_fpre") == 0) ||
       (strcasecmp(str[0],"prf_ftmp") == 0) ||
       (strcasecmp(str[0],"prf_fmol") == 0) ||
       (strcasecmp(str[0],"prf_fmolx") == 0) ||
       (strcasecmp(str[0],"prf_jcharx") == 0) ||
       (strcasecmp(str[0],"prf_fmoly") == 0) ||
       (strcasecmp(str[0],"prf_jchary") == 0) ||
       (strcasecmp(str[0],"prf_fhaz") == 0) ||
       (strcasecmp(str[0],"prf_pres_gnd") == 0) ||
       (strcasecmp(str[0],"prf_temp_gnd") == 0))
    {
      sim_prf_mode = SIM_PRF_CUSTOM;
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
    for(n=nc=0,p=temp; n<MAXITEM; n++,p+=nc)
    {
      if(sscanf(p,"%s%n",str[n],&nc) == EOF) break;
    }
    if(n < 1) continue;
    if(strcasecmp(str[0],"prf_falt") == 0)
    {
      continue;
    } else
    if(strcasecmp(str[0],"mie_wlen") == 0)
    {
      continue;
    } else
    if(strcasecmp(str[0],"dsr_wlen") == 0)
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
          if(dsr_wlen != NULL)
          {
            free(dsr_wlen);
            dsr_wlen = NULL;
          }
          if((dsr_n_wlen=Read1P(str[1],DSR_MAXWLEN,num,uni,NULL,&dsr_wlen)) < 1)
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
          if(ssr_wlen != NULL)
          {
            free(ssr_wlen);
            ssr_wlen = NULL;
          }
          if((ssr_n_wlen=Read1P(str[1],SSR_MAXWLEN,num,uni,NULL,&ssr_wlen)) < 1)
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
          if(ssr_sbmp != NULL)
          {
            free(ssr_sbmp);
            ssr_sbmp = NULL;
          }
          if((ssr_n_sbmp=ReadNP(str[1],SSR_MAXWLEN,num,&ssr_sbmp)) < 1)
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
    if(strcasecmp(str[0],"ssr_nsam") == 0)
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
          if(ssr_nsam != NULL)
          {
            free(ssr_nsam);
            ssr_nsam = NULL;
          }
          if((ssr_n_nsam=ReadNP(str[1],SSR_MAXWLEN,num,&ssr_nsam)) < 1)
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
    if(strcasecmp(str[0],"sim_modtran") == 0)
    {
      if(n > 1)
      {
        sim_modtran = str[1][0];
      }
      else
      {
        sim_modtran = SIM_MODTRAN;
      }
      if(cnt_hp && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30c\n",str[0],sim_modtran);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_speed") == 0)
    {
      if(n > 1)
      {
        sim_speed = str[1][0];
      }
      else
      {
        sim_speed = SIM_SPEED;
      }
      if(cnt_hp && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30c\n",str[0],sim_speed);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_itype") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0') sim_itype = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30d\n",str[0],sim_itype);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_mult") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0' && (abs(ntmp)<=1)) sim_mult = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30d\n",str[0],sim_mult);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_prnt") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>=-2 && ntmp<=1) sim_prnt = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30d\n",str[0],sim_prnt);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_wlen_min") == 0)
    {
      uni = 1.0;
      if(n > 2)
      {
        errno = 0;
        xtmp = strtod(str[2],&p);
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
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0' && xtmp>0.0) sim_wlen_min = xtmp*uni;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.14e\n",str[0],sim_wlen_min);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_wlen_max") == 0)
    {
      uni = 1.0;
      if(n > 2)
      {
        errno = 0;
        xtmp = strtod(str[2],&p);
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
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0' && xtmp>0.0) sim_wlen_max = xtmp*uni;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.14e\n",str[0],sim_wlen_max);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_phas_wlen") == 0)
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
          if((sim_n_phas_wlen=Read1A(str[1],SIM_MAX_PHAS_WLEN,num,uni,NULL,sim_phas_wlen)) < 1)
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
    if(strcasecmp(str[0],"sim_dmax") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') sim_dmax = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.14e\n",str[0],sim_dmax);
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
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.14e\n",str[0],sim_th_sun);
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
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.14e\n",str[0],sim_ph_sun);
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
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.14e\n",str[0],sim_th_los);
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
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.14e\n",str[0],sim_ph_los);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_fdir") == 0)
    {
      if(n > 1)
      {
        if(strcmp(str[1],NONAME) != 0)
        {
          if((sim_n_dir=Read4P(str[1],SIM_MAXNDIR,0,1,2,3,1.0,1.0,1.0,1.0,NULL,
                               &sim_dir[0],&sim_dir[1],&sim_dir[2],&sim_dir[3])) < 0)
          {
            err = 1;
            break;
          }
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],str[1]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_solcon") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') sim_solcon = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.14e\n",str[0],sim_solcon);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_iday") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0') sim_iday = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30d\n",str[0],sim_iday);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_range") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') sim_range = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.14e\n",str[0],sim_range);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_ro") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') sim_ro = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.14e\n",str[0],sim_ro);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_fsun") == 0)
    {
      if(n > 1)
      {
        strncpy(sim_fsun,str[1],MAXLINE);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],sim_fsun);
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
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30d\n",str[0],sim_salb);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_mlflx") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0') sim_mlflx = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30d\n",str[0],sim_mlflx);
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
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %2d %27.14e\n",str[0],idx,sim_prf_vscl[idx]);
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
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %2d %27d\n",str[0],idx,sim_prf_pscl[idx]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_prf_vsclx") == 0)
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
      if(idx<0 || idx>=PRF_NMOLX)
      {
        fprintf(stderr,"%s: index out of range >>> %s\n",fnam,line);
        err = 1;
        break;
      }
      if(n > 2)
      {
        errno = 0;
        xtmp = strtod(str[2],&p);
        if(errno!=ERANGE && *p=='\0') sim_prf_vsclx[idx] = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>2 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %2d %27.14e\n",str[0],idx,sim_prf_vsclx[idx]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_prf_psclx") == 0)
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
      if(idx<0 || idx>=PRF_NMOLX)
      {
        fprintf(stderr,"%s: index out of range >>> %s\n",fnam,line);
        err = 1;
        break;
      }
      if(n > 2)
      {
        errno = 0;
        ntmp = strtol(str[2],&p,10);
        if(errno!=ERANGE && *p=='\0') sim_prf_psclx[idx] = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>2 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %2d %27d\n",str[0],idx,sim_prf_psclx[idx]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_prf_vscly") == 0)
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
      if(idx<0 || idx>=PRF_NMOLY)
      {
        fprintf(stderr,"%s: index out of range >>> %s\n",fnam,line);
        err = 1;
        break;
      }
      if(n > 2)
      {
        errno = 0;
        xtmp = strtod(str[2],&p);
        if(errno!=ERANGE && *p=='\0') sim_prf_vscly[idx] = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>2 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %2d %27.14e\n",str[0],idx,sim_prf_vscly[idx]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"sim_prf_pscly") == 0)
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
      if(idx<0 || idx>=PRF_NMOLY)
      {
        fprintf(stderr,"%s: index out of range >>> %s\n",fnam,line);
        err = 1;
        break;
      }
      if(n > 2)
      {
        errno = 0;
        ntmp = strtol(str[2],&p,10);
        if(errno!=ERANGE && *p=='\0') sim_prf_pscly[idx] = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>2 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %2d %27d\n",str[0],idx,sim_prf_pscly[idx]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mod_path_v4") == 0)
    {
      if(n > 1)
      {
        strncpy(mod_path_v4,str[1],MAXLINE);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],mod_path_v4);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mod_path_v5") == 0)
    {
      if(n > 1)
      {
        strncpy(mod_path_v5,str[1],MAXLINE);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],mod_path_v5);
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
          if((nc=Read2P(str[1],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
          {
            err = 1;
            break;
          }
          if(SamplingE(nc,x,y,prf_n_alt,prf_alt,prf_pres,1.0) < 0)
          {
            err = 1;
          } else
          if(isnan(prf_pres_gnd))
          {
            prf_pres_gnd = prf_pres[0];
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
          if((nc=Read2P(str[1],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
          {
            err = 1;
            break;
          }
          if(Sampling(nc,x,y,prf_n_alt,prf_alt,prf_temp,1.0) < 0)
          {
            err = 1;
          } else
          if(isnan(prf_temp_gnd))
          {
            prf_temp_gnd = prf_temp[0];
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
          if((nc=Read2P(str[2],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
          {
            err = 1;
            break;
          }
          if(Sampling(nc,x,y,prf_n_alt,prf_alt,prf_wmol[idx],1.0) < 0)
          {
            err = 1;
          } else
          if(isnan(sim_prf_vscl[idx]))
          {
            sim_prf_vscl[idx] = 1.0;
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
    if(strcasecmp(str[0],"prf_fmolx") == 0)
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
      if(idx<0 || idx>=PRF_NMOLX)
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
          if((nc=Read2P(str[2],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
          {
            err = 1;
            break;
          }
          if(Sampling(nc,x,y,prf_n_alt,prf_alt,prf_xmol[idx],1.0) < 0)
          {
            err = 1;
          } else
          if(isnan(sim_prf_vsclx[idx]))
          {
            sim_prf_vsclx[idx] = 1.0;
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
    if(strcasecmp(str[0],"prf_jcharx") == 0)
    {
      if(n > 1)
      {
        prf_jcharx = str[1][0];
      }
      else
      {
        prf_jcharx = ' ';
      }
      if(cnt_hp && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30c\n",str[0],prf_jcharx);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"prf_fmoly") == 0)
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
      if(idx<0 || idx>=PRF_NMOLY)
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
          if((nc=Read2P(str[2],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
          {
            err = 1;
            break;
          }
          if(Sampling(nc,x,y,prf_n_alt,prf_alt,prf_ymol[idx],1.0) < 0)
          {
            err = 1;
          } else
          if(isnan(sim_prf_vscly[idx]))
          {
            sim_prf_vscly[idx] = 1.0;
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
    if(strcasecmp(str[0],"prf_jchary") == 0)
    {
      if(n > 1)
      {
        prf_jchary = str[1][0];
      }
      else
      {
        prf_jchary = ' ';
      }
      if(cnt_hp && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30c\n",str[0],prf_jchary);
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
          if((nc=Read2P(str[2],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
          {
            err = 1;
            break;
          }
          if(Sampling(nc,x,y,prf_n_alt,prf_alt,prf_haze[idx],1.0) < 0)
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
    if(strcasecmp(str[0],"mie_angl") == 0)
    {
      num = 0;
      uni = 1.0;
      mie_angl_min = MIE_ANGL_MIN;
      mie_angl_max = MIE_ANGL_MAX;
      if(n > 2)
      {
        errno = 0;
        num = strtol(str[2],&p,10);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"%s: convert error >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 3)
      {
        if(strncasecmp(str[3],"deg",3) == 0)
        {
          uni = 1.0;
        } else
        if(strncasecmp(str[3],"rad",3) == 0)
        {
          uni = R_TO_D;
        }
        else
        {
          fprintf(stderr,"%s: parameter error >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 4)
      {
        errno = 0;
        mie_angl_min = strtod(str[4],&p);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"%s: convert error >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 5)
      {
        errno = 0;
        mie_angl_max = strtod(str[5],&p);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"%s: convert error >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(n > 1)
      {
        if(strcmp(str[1],NONAME) != 0)
        {
          if((mie_n_angl=Read1A(str[1],MIE_MAXDATA,num,uni,AnglSelect,mie_angl)) < 1)
          {
            err = 1;
            break;
          }
        }
      }
      if(cnt_hp && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s %4d %s %13.4f %13.4f\n",str[0],str[1],
                                               num,fabs(uni-1.0)>DELTA?"rad":"deg",
                                               mie_angl_min,mie_angl_max);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_wlen_ref") == 0)
    {
      uni = 1.0;
      if(n > 2)
      {
        errno = 0;
        xtmp = strtod(str[2],&p);
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
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0' && xtmp>0.0) mie_wlen_ref = xtmp*uni;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.14e\n",str[0],mie_wlen_ref);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_n_step") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        ntmp = strtol(str[1],&p,10);
        if(errno!=ERANGE && *p=='\0' && ntmp>0) mie_n_step = ntmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30d\n",str[0],mie_n_step);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_wsgm") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0' && xtmp>0.0) mie_wsgm = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.4e\n",str[0],mie_wsgm);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_rmin") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0' && xtmp>0.0) mie_rmin = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.4e\n",str[0],mie_rmin);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_rmax") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0' && xtmp>0.0) mie_rmax = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.4e\n",str[0],mie_rmax);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_cmp1") == 0)
    {
      if(n > 1) // str[1] must be the file name
      {
        snprintf(mie_aers_cmp[0],MAXLINE,"%s",temp);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],temp);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_cmp2") == 0)
    {
      if(n > 1) // str[1] must be the file name
      {
        snprintf(mie_aers_cmp[1],MAXLINE,"%s",temp);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],temp);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_cmp3") == 0)
    {
      if(n > 1) // str[1] must be the file name
      {
        snprintf(mie_aers_cmp[2],MAXLINE,"%s",temp);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],temp);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_cmp4") == 0)
    {
      if(n > 1) // str[1] must be the file name
      {
        snprintf(mie_aers_cmp[3],MAXLINE,"%s",temp);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],temp);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_inp1") == 0)
    {
      if(n > 1)
      {
        snprintf(mie_aers_inp1[0],MAXLINE,"%s1.dat",str[1]);
        snprintf(mie_aers_inp2[0],MAXLINE,"%s2.dat",str[1]);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],str[1]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_inp2") == 0)
    {
      if(n > 1)
      {
        snprintf(mie_aers_inp1[1],MAXLINE,"%s1.dat",str[1]);
        snprintf(mie_aers_inp2[1],MAXLINE,"%s2.dat",str[1]);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],str[1]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_inp3") == 0)
    {
      if(n > 1)
      {
        snprintf(mie_aers_inp1[2],MAXLINE,"%s1.dat",str[1]);
        snprintf(mie_aers_inp2[2],MAXLINE,"%s2.dat",str[1]);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],str[1]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"mie_inp4") == 0)
    {
      if(n > 1)
      {
        snprintf(mie_aers_inp1[3],MAXLINE,"%s1.dat",str[1]);
        snprintf(mie_aers_inp2[3],MAXLINE,"%s2.dat",str[1]);
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30s\n",str[0],str[1]);
        cnt_n_cmnt++;
      }
    } else
    if(strcasecmp(str[0],"upp_rh") == 0)
    {
      if(n > 1)
      {
        errno = 0;
        xtmp = strtod(str[1],&p);
        if(errno!=ERANGE && *p=='\0') upp_rh = xtmp;
        else
        {
          fprintf(stderr,"%s: out of range >>> %s\n",fnam,line);
          err = 1;
          break;
        }
      }
      if(cnt_hp && n>1 && cnt_n_cmnt<CNT_MAXCMNT)
      {
        snprintf(cnt_cmnt[cnt_n_cmnt],MAXLINE,"%-14s: %30.4e\n",str[0],upp_rh);
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
  if(sim_prf_mode == SIM_PRF_CUSTOM)
  {
    if(PrfGround() < 0) return -1;
    if(PrfScaleMolecule() < 0) return -1;
    if(PrfCalcAOD() < 0) return -1;
    if(PrfScaleAerosol(sim_vtau) < 0) return -1;
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
        if(((!isnan(prf_pres_gnd))&&(sim_prf_pscl[j]!=0)) ||
           ((!isnan(sim_prf_vscl[j]))&&(fabs(sim_prf_vscl[j]-1.0)>EPSILON)))
        {
          flag = 1;
        }
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
          if(((!isnan(prf_pres_gnd))&&(sim_prf_pscl[j]!=0)) ||
             ((!isnan(sim_prf_vscl[j]))&&(fabs(sim_prf_vscl[j]-1.0)>EPSILON)))
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
            if(((!isnan(prf_pres_gnd))&&(sim_prf_pscl[j]!=0)) ||
               ((!isnan(sim_prf_vscl[j]))&&(fabs(sim_prf_vscl[j]-1.0)>EPSILON)))
            {
              fprintf(stderr," %9.2e",sim_prf_wmol[j][i]);
            }
          }
          fprintf(stderr,"\n");
        }
      }
      if((sim_2c&0x1000) > 0)
      {
        fprintf(stderr,"Aerosol profile:\n");
        for(i=0; i<prf_n_alt; i++)
        {
          fprintf(stderr,"%6.2f %13.10f %13.10f %13.10f %13.10f\n",prf_alt[i],
                          sim_prf_haze[0][i],sim_prf_haze[1][i],
                          sim_prf_haze[2][i],sim_prf_haze[3][i]);
        }
      }
    }
  }

  return 0;
}

int WaveSelect(double *w)
{
  if(w[0]<mie_wlen_min || w[0]>mie_wlen_max)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int AnglSelect(double *a)
{
  if(a[0]<mie_angl_min || a[0]>mie_angl_max)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int ReadComp(int iaer,int size,double *wcom,double *rmod,double *lsgm,double **refr,double **refi)
{
  int i,j,n;
  int nc,ns;
  int n_comp = 0;
  int cr = MIE_REAL_NUM;
  int ci = MIE_IMAG_NUM;
  int imin = MIE_IMIN;
  int imax = MIE_IMAX;
  int err;
  int flag;
  double u = 1.0;
  double v;
  double *x,*y;
  char line[MAXLINE];
  char str[MAXITEM][MAXLINE];
  char fnam[] = "ReadComp";
  char *s,*p;
  FILE *fp;

  s = mie_aers_cmp[iaer];
  for(ns=nc=0,p=s; ns<MAXITEM; ns++,p+=nc)
  {
    if(sscanf(p,"%s%n",str[ns],&nc) == EOF) break;
  }
  if(strcasecmp(str[0],"mie_cmp") != 0)
  {
    fprintf(stderr,"%s: error in input >>> %s\n",fnam,s);
    return -1;
  } else
  if(ns < 2)
  {
    fprintf(stderr,"%s: error, ns=%d (%s)\n",fnam,ns,s);
    return -1;
  }
  if(ns > 2)
  {
    errno = 0;
    cr = strtol(str[2],&p,10);
    if(errno==ERANGE || *p!='\0')
    {
      fprintf(stderr,"%s: convert error >>> %s\n",fnam,s);
      return -1;
    }
  }
  if(ns > 3)
  {
    errno = 0;
    ci = strtol(str[3],&p,10);
    if(errno==ERANGE || *p!='\0')
    {
      fprintf(stderr,"%s: convert error >>> %s\n",fnam,s);
      return -1;
    }
  }
  if(ns > 4)
  {
    errno = 0;
    u = strtod(str[4],&p);
    if(errno==ERANGE || *p!='\0')
    {
      fprintf(stderr,"%s: convert error >>> %s\n",fnam,s);
      return -1;
    }
  }
  if(ns > 5)
  {
    errno = 0;
    imin = strtol(str[5],&p,10);
    if(errno==ERANGE || *p!='\0')
    {
      fprintf(stderr,"%s: convert error >>> %s\n",fnam,s);
      return -1;
    }
  }
  if(ns > 6)
  {
    errno = 0;
    imax = strtol(str[6],&p,10);
    if(errno==ERANGE || *p!='\0')
    {
      fprintf(stderr,"%s: convert error >>> %s\n",fnam,s);
      return -1;
    }
  }

  if(strcmp(str[1],"")!=0 && strcmp(str[1],NONAME)!=0)
  {
    if(strcmp(str[1],"stdin") == 0)
    {
      fp = stdin;
      flag = 0;
    }
    else
    {
      if((fp=fopen(str[1],"r")) == NULL)
      {
        fprintf(stderr,"%s: cannot open %s\n",fnam,str[1]);
        return -1;
      }
      flag = 1;
    }
    i = n = 0;
    err = 0;
    while(fgets(line,MAXLINE,fp) != NULL)
    {
      if(i < imin)
      {
        strncpy(line,"",MAXLINE);
        i++;
        continue;
      } else
      if(i > imax)
      {
        break;
      }
      for(ns=nc=0,p=line; ns<MAXITEM; ns++,p+=nc)
      {
        if(sscanf(p,"%s%n",str[ns],&nc) == EOF) break;
      }
      if(ns == 0)
      {
        i++;
        continue;
      } else
      if(str[0][0] == '#')
      {
        i++;
        continue;
      }
      if(strlen(str[0]) != MIE_FLAG_SIZE)
      {
        fprintf(stderr,"%s: error in flag (%s) >>> %s\n",fnam,s,line);
        err = 1;
        break;
      }
      if(n >= size)
      {
        fprintf(stderr,"%s: warning, #data exceed the limit %d (%s)\n",fnam,n,s);
        break;
      }
      switch(str[0][0]) // Function
      {
        case MIE_FUNC_FILE:
        case MIE_FUNC_LOGNORMAL:
        case MIE_FUNC_EFF_LOGNORMAL:
        case MIE_FUNC_GAMMA:
          mie_size_func[n] = str[0][0];
          break;
        case '!':
          mie_size_func[n] = MIE_FUNC_LOGNORMAL;
          break;
        default:
          fprintf(stderr,"%s: error in function (%s) >>> %s\n",fnam,s,line);
          err = 1;
          break;
      }
      if(err) break;
      switch(str[0][1]) // X type
      {
        case MIE_XTYPE_R:
        case MIE_XTYPE_LOGR:
        case MIE_XTYPE_LNR:
          mie_size_xtype[n] = str[0][1];
          break;
        case '!':
          if(mie_size_func[n] == MIE_FUNC_LOGNORMAL)
          {
            mie_size_xtype[n] = MIE_XTYPE_LOGR;
          }
          else
          {
            mie_size_xtype[n] = MIE_XTYPE_R;
          }
          break;
        default:
          fprintf(stderr,"%s: error in X type (%s) >>> %s\n",fnam,s,line);
          err = 1;
          break;
      }
      if(err) break;
      switch(str[0][2]) // Y type
      {
        case MIE_YTYPE_NUMBER:
        case MIE_YTYPE_VOLUME:
          mie_size_ytype[n] = str[0][2];
          break;
        case '!':
          mie_size_ytype[n] = MIE_YTYPE_NUMBER;
          break;
        default:
          fprintf(stderr,"%s: error in Y type (%s) >>> %s\n",fnam,s,line);
          err = 1;
          break;
      }
      if(err) break;
      switch(str[0][3]) // Shape
      {
        case MIE_SHAPE_SPHERE:
        case MIE_SHAPE_SPHEROID:
          mie_size_shape[n] = str[0][3];
          break;
        case '!':
          mie_size_shape[n] = MIE_SHAPE_SPHERE;
          break;
        default:
          fprintf(stderr,"%s: error in shape (%s) >>> %s\n",fnam,s,line);
          err = 1;
          break;
      }
      if(err) break;
      switch(mie_size_func[n])
      {
        case MIE_FUNC_FILE: // flag wcom fnam refr refi (reps) comment
          // Aspect ratio
          switch(mie_size_shape[n])
          {
            case MIE_SHAPE_SPHERE:
              if(ns < 5)
              {
                fprintf(stderr,"%s: read error (%s) >>> %s\n",fnam,s,line);
                fprintf(stderr,"%s: mie_size_func[%d]=%c, ns=%d\n",fnam,n,mie_size_func[n],ns);
                err = 1;
                break;
              }
              break;
            case MIE_SHAPE_SPHEROID:
              if(ns < 6)
              {
                fprintf(stderr,"%s: read error (%s) >>> %s\n",fnam,s,line);
                fprintf(stderr,"%s: mie_size_func[%d]=%c, ns=%d\n",fnam,n,mie_size_func[n],ns);
                err = 1;
                break;
              }
              mie_reps_com[n] = strtod(str[5],&p);
              if(errno==ERANGE || *p!='\0')
              {
                fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
                err = 1;
                break;
              }
              if(mie_reps_com[n] <= 0.0)
              {
                fprintf(stderr,"%s: error in aspect ratio (%s) >>> %s\n",fnam,s,line);
                err = 1;
                break;
              }
              break;
            default:
              fprintf(stderr,"%s: error in shape (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
          }
          if(err) break;
          // Mixing ratio
          wcom[n] = strtod(str[1],&p);
          if(errno==ERANGE || *p!='\0')
          {
            fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          if(wcom[n] < 0.0)
          {
            fprintf(stderr,"%s: error in mixing ratio (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          // Size distribution
          if((mie_n_size[n]=Read2P(str[2],MIE_DMAX,0,1,1.0,1.0,NULL,&mie_xval_com[n],&mie_yval_com[n])) < 0)
          {
            err = 1;
            break;
          }
          if(mie_n_size[n] < 2)
          {
            fprintf(stderr,"%s: error, mie_n_size[%d]=%d\n",fnam,n,mie_n_size[n]);
            free(mie_xval_com[n]);
            free(mie_yval_com[n]);
            err = 1;
            break;
          }
          if(cnt_vb > 2)
          {
            for(j=0; j<mie_n_size[n]; j++)
            {
              fprintf(stderr,"%13.6e %13.6e\n",mie_xval_com[n][j],mie_yval_com[n][j]);
            }
          }
          // Refractive index (real)
          if(str[3][0] == '@')
          {
            v = strtod(&str[3][1],&p);
            if(errno==ERANGE || *p!='\0')
            {
              fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
            }
            refr[n] = NULL;
            if((refr[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            for(j=0; j<mie_n_wlen; j++)
            {
              refr[n][j] = v;
            }
          }
          else
          {
            x = NULL;
            y = NULL;
            if((j=Read2P(str[3],MIE_MAXDATA,0,cr,u,1.0,NULL,&x,&y)) < 0)
            {
              err = 1;
              break;
            }
            refr[n] = NULL;
            if((refr[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            if(Sampling(j,x,y,mie_n_wlen,mie_wlen,refr[n],1.0) < 0)
            {
              err = 1;
              break;
            }
            free(x);
            free(y);
          }
          // Refractive index (imag)
          if(str[4][0] == '@')
          {
            v = strtod(&str[4][1],&p);
            if(errno==ERANGE || *p!='\0')
            {
              fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
            }
            refi[n] = NULL;
            if((refi[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            for(j=0; j<mie_n_wlen; j++)
            {
              refi[n][j] = v;
            }
          }
          else
          {
            x = NULL;
            y = NULL;
            if((j=Read2P(str[4],MIE_MAXDATA,0,ci,u,1.0,NULL,&x,&y)) < 0)
            {
              err = 1;
              break;
            }
            refi[n] = NULL;
            if((refi[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            if(SamplingE(j,x,y,mie_n_wlen,mie_wlen,refi[n],1.0) < 0)
            {
              err = 1;
              break;
            }
            free(x);
            free(y);
          }
          break;
        case MIE_FUNC_LOGNORMAL: // flag wcom rmod lsgm refr refi (reps) comment
          // Aspect ratio
          switch(mie_size_shape[n])
          {
            case MIE_SHAPE_SPHERE:
              if(ns < 6)
              {
                fprintf(stderr,"%s: read error (%s) >>> %s\n",fnam,s,line);
                fprintf(stderr,"%s: mie_size_func[%d]=%c, ns=%d\n",fnam,n,mie_size_func[n],ns);
                err = 1;
                break;
              }
              break;
            case MIE_SHAPE_SPHEROID:
              if(ns < 7)
              {
                fprintf(stderr,"%s: read error (%s) >>> %s\n",fnam,s,line);
                fprintf(stderr,"%s: mie_size_func[%d]=%c, ns=%d\n",fnam,n,mie_size_func[n],ns);
                err = 1;
                break;
              }
              mie_reps_com[n] = strtod(str[6],&p);
              if(errno==ERANGE || *p!='\0')
              {
                fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
                err = 1;
                break;
              }
              if(mie_reps_com[n] <= 0.0)
              {
                fprintf(stderr,"%s: error in aspect ratio (%s) >>> %s\n",fnam,s,line);
                err = 1;
                break;
              }
              break;
            default:
              fprintf(stderr,"%s: error in shape (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
          }
          if(err) break;
          // Mixing ratio
          wcom[n] = strtod(str[1],&p);
          if(errno==ERANGE || *p!='\0')
          {
            fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          if(wcom[n] < 0.0)
          {
            fprintf(stderr,"%s: error in mixing ratio (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          // Mode radius
          rmod[n] = strtod(str[2],&p);
          if(errno==ERANGE || *p!='\0')
          {
            fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          if(rmod[n] <= 0.0)
          {
            fprintf(stderr,"%s: error in mode radius (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          // Sigma
          lsgm[n] = strtod(str[3],&p);
          if(errno==ERANGE || *p!='\0')
          {
            fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          if(lsgm[n] <= 0.0)
          {
            fprintf(stderr,"%s: error in sigma (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          if(mie_size_xtype[n] == MIE_XTYPE_LNR)
          {
            lsgm[n] *= M_1_LNA; // dY/dLnR -> dY/dLogR
          }
          // Refractive index (real)
          if(str[4][0] == '@')
          {
            v = strtod(&str[4][1],&p);
            if(errno==ERANGE || *p!='\0')
            {
              fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
            }
            refr[n] = NULL;
            if((refr[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            for(j=0; j<mie_n_wlen; j++)
            {
              refr[n][j] = v;
            }
          }
          else
          {
            x = NULL;
            y = NULL;
            if((j=Read2P(str[4],MIE_MAXDATA,0,cr,u,1.0,NULL,&x,&y)) < 0)
            {
              err = 1;
              break;
            }
            refr[n] = NULL;
            if((refr[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            if(Sampling(j,x,y,mie_n_wlen,mie_wlen,refr[n],1.0) < 0)
            {
              err = 1;
              break;
            }
            free(x);
            free(y);
          }
          // Refractive index (imag)
          if(str[5][0] == '@')
          {
            v = strtod(&str[5][1],&p);
            if(errno==ERANGE || *p!='\0')
            {
              fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
            }
            refi[n] = NULL;
            if((refi[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            for(j=0; j<mie_n_wlen; j++)
            {
              refi[n][j] = v;
            }
          }
          else
          {
            x = NULL;
            y = NULL;
            if((j=Read2P(str[5],MIE_MAXDATA,0,ci,u,1.0,NULL,&x,&y)) < 0)
            {
              err = 1;
              break;
            }
            refi[n] = NULL;
            if((refi[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            if(SamplingE(j,x,y,mie_n_wlen,mie_wlen,refi[n],1.0) < 0)
            {
              err = 1;
              break;
            }
            free(x);
            free(y);
          }
          break;
        case MIE_FUNC_EFF_LOGNORMAL: // flag wcom reff veff refr refi (reps) comment
          // Aspect ratio
          switch(mie_size_shape[n])
          {
            case MIE_SHAPE_SPHERE:
              if(ns < 6)
              {
                fprintf(stderr,"%s: read error (%s) >>> %s\n",fnam,s,line);
                fprintf(stderr,"%s: mie_size_func[%d]=%c, ns=%d\n",fnam,n,mie_size_func[n],ns);
                err = 1;
                break;
              }
              break;
            case MIE_SHAPE_SPHEROID:
              if(ns < 7)
              {
                fprintf(stderr,"%s: read error (%s) >>> %s\n",fnam,s,line);
                fprintf(stderr,"%s: mie_size_func[%d]=%c, ns=%d\n",fnam,n,mie_size_func[n],ns);
                err = 1;
                break;
              }
              mie_reps_com[n] = strtod(str[6],&p);
              if(errno==ERANGE || *p!='\0')
              {
                fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
                err = 1;
                break;
              }
              if(mie_reps_com[n] <= 0.0)
              {
                fprintf(stderr,"%s: error in aspect ratio (%s) >>> %s\n",fnam,s,line);
                err = 1;
                break;
              }
              break;
            default:
              fprintf(stderr,"%s: error in shape (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
          }
          if(err) break;
          // Mixing ratio
          wcom[n] = strtod(str[1],&p);
          if(errno==ERANGE || *p!='\0')
          {
            fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          if(wcom[n] < 0.0)
          {
            fprintf(stderr,"%s: error in mixing ratio (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          // Mode radius
          rmod[n] = strtod(str[2],&p);
          if(errno==ERANGE || *p!='\0')
          {
            fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          if(rmod[n] <= 0.0)
          {
            fprintf(stderr,"%s: error in effective radius (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          // Sigma
          lsgm[n] = strtod(str[3],&p);
          if(errno==ERANGE || *p!='\0')
          {
            fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          if(lsgm[n] <= 0.0)
          {
            fprintf(stderr,"%s: error in effective variance (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          v = log(lsgm[n]+1.0);
          rmod[n] = rmod[n]*exp(-2.5*v);
          lsgm[n] = sqrt(v)*M_1_LNA; // dY/dLogR
          // Refractive index (real)
          if(str[4][0] == '@')
          {
            v = strtod(&str[4][1],&p);
            if(errno==ERANGE || *p!='\0')
            {
              fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
            }
            refr[n] = NULL;
            if((refr[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            for(j=0; j<mie_n_wlen; j++)
            {
              refr[n][j] = v;
            }
          }
          else
          {
            x = NULL;
            y = NULL;
            if((j=Read2P(str[4],MIE_MAXDATA,0,cr,u,1.0,NULL,&x,&y)) < 0)
            {
              err = 1;
              break;
            }
            refr[n] = NULL;
            if((refr[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            if(Sampling(j,x,y,mie_n_wlen,mie_wlen,refr[n],1.0) < 0)
            {
              err = 1;
              break;
            }
            free(x);
            free(y);
          }
          // Refractive index (imag)
          if(str[5][0] == '@')
          {
            v = strtod(&str[5][1],&p);
            if(errno==ERANGE || *p!='\0')
            {
              fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
            }
            refi[n] = NULL;
            if((refi[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            for(j=0; j<mie_n_wlen; j++)
            {
              refi[n][j] = v;
            }
          }
          else
          {
            x = NULL;
            y = NULL;
            if((j=Read2P(str[5],MIE_MAXDATA,0,ci,u,1.0,NULL,&x,&y)) < 0)
            {
              err = 1;
              break;
            }
            refi[n] = NULL;
            if((refi[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            if(SamplingE(j,x,y,mie_n_wlen,mie_wlen,refi[n],1.0) < 0)
            {
              err = 1;
              break;
            }
            free(x);
            free(y);
          }
          break;
        case MIE_FUNC_GAMMA: // flag wcom reff veff refr refi (reps) comment
          // Aspect ratio
          switch(mie_size_shape[n])
          {
            case MIE_SHAPE_SPHERE:
              if(ns < 6)
              {
                fprintf(stderr,"%s: read error (%s) >>> %s\n",fnam,s,line);
                fprintf(stderr,"%s: mie_size_func[%d]=%c, ns=%d\n",fnam,n,mie_size_func[n],ns);
                err = 1;
                break;
              }
              break;
            case MIE_SHAPE_SPHEROID:
              if(ns < 7)
              {
                fprintf(stderr,"%s: read error (%s) >>> %s\n",fnam,s,line);
                fprintf(stderr,"%s: mie_size_func[%d]=%c, ns=%d\n",fnam,n,mie_size_func[n],ns);
                err = 1;
                break;
              }
              mie_reps_com[n] = strtod(str[6],&p);
              if(errno==ERANGE || *p!='\0')
              {
                fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
                err = 1;
                break;
              }
              if(mie_reps_com[n] <= 0.0)
              {
                fprintf(stderr,"%s: error in aspect ratio (%s) >>> %s\n",fnam,s,line);
                err = 1;
                break;
              }
              break;
            default:
              fprintf(stderr,"%s: error in shape (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
          }
          if(err) break;
          // Mixing ratio
          wcom[n] = strtod(str[1],&p);
          if(errno==ERANGE || *p!='\0')
          {
            fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          if(wcom[n] < 0.0)
          {
            fprintf(stderr,"%s: error in mixing ratio (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          // Effective radius
          rmod[n] = strtod(str[2],&p);
          if(errno==ERANGE || *p!='\0')
          {
            fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          if(rmod[n] <= 0.0)
          {
            fprintf(stderr,"%s: error in effective radius (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          // Effective variance
          lsgm[n] = strtod(str[3],&p);
          if(errno==ERANGE || *p!='\0')
          {
            fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          if(lsgm[n] <= 0.0)
          {
            fprintf(stderr,"%s: error in effective variance (%s) >>> %s\n",fnam,s,line);
            err = 1;
            break;
          }
          // Refractive index (real)
          if(str[4][0] == '@')
          {
            v = strtod(&str[4][1],&p);
            if(errno==ERANGE || *p!='\0')
            {
              fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
            }
            refr[n] = NULL;
            if((refr[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            for(j=0; j<mie_n_wlen; j++)
            {
              refr[n][j] = v;
            }
          }
          else
          {
            x = NULL;
            y = NULL;
            if((j=Read2P(str[4],MIE_MAXDATA,0,cr,u,1.0,NULL,&x,&y)) < 0)
            {
              err = 1;
              break;
            }
            refr[n] = NULL;
            if((refr[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            if(Sampling(j,x,y,mie_n_wlen,mie_wlen,refr[n],1.0) < 0)
            {
              err = 1;
              break;
            }
            free(x);
            free(y);
          }
          // Refractive index (imag)
          if(str[5][0] == '@')
          {
            v = strtod(&str[5][1],&p);
            if(errno==ERANGE || *p!='\0')
            {
              fprintf(stderr,"%s: convert error (%s) >>> %s\n",fnam,s,line);
              err = 1;
              break;
            }
            refi[n] = NULL;
            if((refi[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            for(j=0; j<mie_n_wlen; j++)
            {
              refi[n][j] = v;
            }
          }
          else
          {
            x = NULL;
            y = NULL;
            if((j=Read2P(str[5],MIE_MAXDATA,0,ci,u,1.0,NULL,&x,&y)) < 0)
            {
              err = 1;
              break;
            }
            refi[n] = NULL;
            if((refi[n]=(double *)malloc(mie_n_wlen*sizeof(double))) == NULL)
            {
              fprintf(stderr,"%s: error in allocating memory.\n",fnam);
              err = 1;
              break;
            }
            if(SamplingE(j,x,y,mie_n_wlen,mie_wlen,refi[n],1.0) < 0)
            {
              err = 1;
              break;
            }
            free(x);
            free(y);
          }
          break;
        default:
          fprintf(stderr,"%s: error in function (%s) >>> %s\n",fnam,s,line);
          err = 1;
          break;
      }
      if(err) break;
      strncpy(line,"",MAXLINE);
      n++;
      i++;
    }
    if(flag)
    {
      fclose(fp);
    }
    if(err)
    {
      return -1;
    }
    n_comp = n;
    fprintf(stderr,"%s: %d data have been read (%s)\n",fnam,n_comp,s);
    if(n_comp < 1)
    {
      fprintf(stderr,"%s: error, n_comp=%d (%s)\n",fnam,n_comp,s);
      return -1;
    }
  }

  return n_comp;
}

int Read1A(char *s,int size,int cx,double ux,int (*func)(double*),double *x)
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

int Read2A(char *s,int size,int cx,int cy,double ux,double uy,int (*func)(double*),double *x,double *y)
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

int Read3A(char *s,int size,int cx,int cy,int cz,double ux,double uy,double uz,
           int (*func)(double*),double *x,double *y,double *z)
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

int Read4A(char *s,int size,int cx,int cy,int cz,int cw,double ux,double uy,double uz,double uw,
           int (*func)(double*),double *x,double *y,double *z,double *w)
{
  int nd;
  int np = 4;
  int c[4];
  double u[4];
  double *d[4];

  // fill array
  c[0] = cx; u[0] = ux; d[0] = x;
  c[1] = cy; u[1] = uy; d[1] = y;
  c[2] = cz; u[2] = uz; d[2] = z;
  c[3] = cw; u[3] = uw; d[3] = w;
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

int Read1P(char *s,int size,int cx,double ux,int (*func)(double*),double **x)
{
  int n;
  int nd;
  int err;
  int np = 1;
  int c[1];
  double u[1];
  double *d[1];
  double **p[1];
  char fnam[] = "Read1P";

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

int Read2P(char *s,int size,int cx,int cy,double ux,double uy,int (*func)(double*),double **x,double **y)
{
  int n;
  int nd;
  int err;
  int np = 2;
  int c[2];
  double u[2];
  double *d[2];
  double **p[2];
  char fnam[] = "Read2P";

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

int Read3P(char *s,int size,int cx,int cy,int cz,double ux,double uy,double uz,
           int (*func)(double*),double **x,double **y,double **z)
{
  int n;
  int nd;
  int err;
  int np = 3;
  int c[3];
  double u[3];
  double *d[3];
  double **p[3];
  char fnam[] = "Read3P";

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

int Read4P(char *s,int size,int cx,int cy,int cz,int cw,double ux,double uy,double uz,double uw,
           int (*func)(double*),double **x,double **y,double **z,double **w)
{
  int n;
  int nd;
  int err;
  int np = 4;
  int c[4];
  double u[4];
  double *d[4];
  double **p[4];
  char fnam[] = "Read4P";

  // fill array
  c[0] = cx; u[0] = ux; d[0] = *x; p[0] = x;
  c[1] = cy; u[1] = uy; d[1] = *y; p[1] = y;
  c[2] = cz; u[2] = uz; d[2] = *z; p[2] = z;
  c[3] = cw; u[3] = uw; d[3] = *w; p[3] = w;
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
  double v[4];
  char line[MAXLINE];
  char temp[MAXLINE];
  char str[MAXITEM][MAXLINE];
  char fnam[] = "ReadDC";
  char *p;
  FILE *fp;

  // check input
  if(np<0 || np>4)
  {
    fprintf(stderr,"%s: error, np=%d\n",fnam,np);
    return -1;
  }
  for(n=0; n<np; n++)
  {
    if(c[n]<0 || c[n]>=MAXITEM)
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
    strncpy(temp,line,MAXLINE);
    if((p=strchr(temp,'#')) != NULL) *p = '\0';
    for(j=0,p=temp; j<=num; j++,p+=nc)
    {
      if(sscanf(p,"%s%n",str[j],&nc) == EOF)
      {
        break;
      }
    }
    if(j != num+1) continue;
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
    if(err) break;
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
  double v[4];
  char line[MAXLINE];
  char temp[MAXLINE];
  char str[MAXITEM][MAXLINE];
  char fnam[] = "ReadDA";
  char *p;
  FILE *fp;

  // check input
  if(np<0 || np>4)
  {
    fprintf(stderr,"%s: error, np=%d\n",fnam,np);
    return -1;
  }
  for(n=0; n<np; n++)
  {
    if(c[n]<0 || c[n]>=MAXITEM)
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
    strncpy(temp,line,MAXLINE);
    if((p=strchr(temp,'#')) != NULL) *p = '\0';
    for(j=0,p=temp; j<=num; j++,p+=nc)
    {
      if(sscanf(p,"%s%n",str[j],&nc) == EOF)
      {
        break;
      }
    }
    if(j != num+1) continue;
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
    if(err) break;
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

int ReadNA(char *s,int size,int cx,int *x)
{
  int nd;
  int np = 1;
  int c[1];
  int *d[1];

  // fill array
  c[0] = cx; d[0] = x;
  // read input
  nd = ReadIA(s,size,np,c,d);

  return nd;
}

int ReadNP(char *s,int size,int cx,int **x)
{
  int n;
  int nd;
  int err;
  int np = 1;
  int c[1];
  int *d[1];
  int **p[1];
  char fnam[] = "ReadNP";

  // fill array
  c[0] = cx; d[0] = *x; p[0] = x;
  // allocate memory
  for(n=0; n<np; n++)
  {
    if(d[n] == NULL)
    {
      if((d[n]=(int*)malloc(size*sizeof(int))) == NULL)
      {
        fprintf(stderr,"%s: failed in allocating memory (%s)\n",fnam,s);
        return -1;
      }
    }
  }
  // read input
  nd = ReadIA(s,size,np,c,d);
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
      if((*p[n]=(int*)realloc(d[n],nd*sizeof(int))) == NULL)
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

int ReadIA(char *s,int size,int np,const int *c,int **d)
{
  int i,j,n;
  int nc,nd;
  int num;
  int err;
  int flag;
  int v[4];
  char line[MAXLINE];
  char temp[MAXLINE];
  char str[MAXITEM][MAXLINE];
  char fnam[] = "ReadIA";
  char *p;
  FILE *fp;

  // check input
  if(np<0 || np>4)
  {
    fprintf(stderr,"%s: error, np=%d\n",fnam,np);
    return -1;
  }
  for(n=0; n<np; n++)
  {
    if(c[n]<0 || c[n]>=MAXITEM)
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
    strncpy(temp,line,MAXLINE);
    if((p=strchr(temp,'#')) != NULL) *p = '\0';
    for(j=0,p=temp; j<=num; j++,p+=nc)
    {
      if(sscanf(p,"%s%n",str[j],&nc) == EOF)
      {
        break;
      }
    }
    if(j != num+1) continue;
    for(n=0; n<np; n++)
    {
      errno = 0;
      v[n] = strtol(str[c[n]],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"%s: convert error >>> %s (%s)\n",fnam,line,s);
        err = 1;
        break;
      }
    }
    if(err) break;
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
    {"sim_grho",1,0,'R'},
    {"sim_h1",1,0,'G'},
    {"sim_h2",1,0,'T'},
    {"sim_xsgm",1,0,'w'},
    {"sim_dbmp",1,0,'b'},
    {"sim_sbmp",1,0,'B'},
    {"sim_nsam",1,0,'n'},
    {"sim_nstr",1,0,'N'},
    {"sim_mode",1,0,'S'},
    {"cnt_version",1,0,'M'},
    {"cnt_xmod",1,0,'m'},
    {"cnt_conf",1,0,'C'},
    {"cnt_cflg",0,0,'c'},
    {"sim_dflg",0,0,'D'},
    {"cnt_db",0,0,'d'},
    {"cnt_vb",0,0,'v'},
    {"cnt_hp",0,0,'h'}
  };

  rt = 0;
  while(1)
  {
    this_option_optind = optind?optind:1;
    c = getopt_long(argn,args,":U:V:W:O:P:A:a:k:R:G:T:w:b:B:n:N:S:M:m:C:cDdvh",long_options,&option_index);
    if(c == -1) break;

    switch(c)
    {
      case 'U':
        errno = 0;
        xtmp = strtod(optarg,&endp);
        if(errno!=ERANGE && *endp=='\0' && xtmp>0.0)
        {
          sim_vtau = xtmp;
          sim_prf_mode = SIM_PRF_CUSTOM;
        }
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
      case 'R':
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
      case 'G':
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
      case 'b':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && (ntmp==-1||ntmp==1||ntmp==5||ntmp==15)) sim_dbmp = ntmp;
        else
        {
          fprintf(stderr,"DSR Band model number -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'B':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && (ntmp==-1||ntmp==1||ntmp==5||ntmp==15)) sim_sbmp_init = ntmp;
        else
        {
          fprintf(stderr,"SSR Band model number -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'n':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && ntmp>0) sim_nsam_init = ntmp;
        else
        {
          fprintf(stderr,"SSR #Sampling -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'N':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && (ntmp==2||ntmp==4||ntmp==8||ntmp==16||ntmp==32)) sim_nstr = ntmp;
        else
        {
          fprintf(stderr,"Number of streams for DISORT -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'S':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && ntmp>=1 && ntmp<=8) sim_mode = ntmp;
        else
        {
          fprintf(stderr,"Simulation mode -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'M':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && (ntmp==MOD_V4||ntmp==MOD_V5)) cnt_version = ntmp;
        else
        {
          fprintf(stderr,"MODTRAN version -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'm':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && ntmp>=0 && ntmp<=2) cnt_xmod = ntmp;
        else
        {
          fprintf(stderr,"Mixing mode -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'C':
        strncpy(cnt_conf,optarg,MAXLINE);
        break;
      case 'c':
        cnt_cflg++;
        break;
      case 'D':
        sim_dflg = 1-sim_dflg;
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

  if(ReadConfig() < 0) return -1;

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
  fprintf(stderr,"run_modtran -[option] (argument) -[option] (argument) ...\n");
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
  fprintf(stderr," R -sim_grho      |%s|%s|%s| %f\n",As(e,"Ground albedo  ",n),As(a,"value",n),      Af(d,SIM_GRHO,n),sim_grho);
  fprintf(stderr," G -sim_h1        |%s|%s|%s| %f\n",As(e,"H1 altitude",n),    As(a,"km",n),         Af(d,SIM_H1,n),sim_h1);
  fprintf(stderr," T -sim_h2        |%s|%s|%s| %f\n",As(e,"H2 altitude",n),    As(a,"km",n),         Af(d,SIM_H2,n),sim_h2);
  fprintf(stderr," w -sim_xsgm      |%s|%s|%s| %e\n",As(e,"X sigma",n),        As(a,"value",n),      Af(d,SIM_XSGM,n),sim_xsgm);
  fprintf(stderr," b -sim_dbmp      |%s|%s|%s| %d\n",As(e,"DSR Band model",n), As(a,"-1|1|5|15",n),  Ad(d,SIM_DBMP,n),sim_dbmp);
  fprintf(stderr," B -sim_sbmp      |%s|%s|%s| %d\n",As(e,"SSR Band model",n), As(a,"-1|1|5|15",n),  Ad(d,SIM_SBMP,n),sim_sbmp_init);
  fprintf(stderr," n -sim_nsam      |%s|%s|%s| %d\n",As(e,"SSR #Sampling",n),  As(a,"#",n),          Ad(d,SIM_NSAM,n),sim_nsam_init);
  fprintf(stderr," N -sim_nstr      |%s|%s|%s| %d\n",As(e,"#Streams",n),       As(a,"2|4|8|16|32",n),Ad(d,SIM_NSTR,n),sim_nstr);
  fprintf(stderr," S -sim_mode      |%s|%s|%s| %d\n",As(e,"Execution mode",n), As(a,"1~8",n),        Ad(d,SIM_MODE_DSR_MODTRAN_BO,n),sim_mode);
  fprintf(stderr," M -cnt_version   |%s|%s|%s| %d\n",As(e,"MODTRAN version",n),As(a,"4000|5000",n),  Ad(d,MOD_V4,n),cnt_version);
  fprintf(stderr," m -cnt_xmod      |%s|%s|%s| %d\n",As(e,"Mixing mode",n),    As(a,"0|1|2",n),      Ad(d,0,n),cnt_xmod);
  fprintf(stderr," C -cnt_conf      |%s|%s|%s| %s\n",As(e,"Config  file",n),   As(a,"name",n),       As(d,CNT_CONF,n),cnt_conf);
  fprintf(stderr," c -cnt_cflg      |%s|%s|%s| %d\n",As(e,"Clear   flag",n),   As(a,"nothing",n),    Ad(d,0,n),cnt_cflg);
  fprintf(stderr," D -sim_dflg      |%s|%s|%s| %d\n",As(e,"DISORT  flag",n),   As(a,"nothing",n),    Ad(d,SIM_DFLG,n),sim_dflg);
  fprintf(stderr," d -cnt_db        |%s|%s|%s| %d\n",As(e,"Debug   mode",n),   As(a,"nothing",n),    Ad(d,0,n),cnt_db);
  fprintf(stderr," v -cnt_vb        |%s|%s|%s| %d\n",As(e,"Verbose mode",n),   As(a,"nothing",n),    Ad(d,0,n),cnt_vb);
  fprintf(stderr," h -cnt_hp        |%s|%s|%s| %d\n",As(e,"Help    mode",n),   As(a,"nothing",n),    Ad(d,0,n),1);
  fprintf(stderr,"------------------------------------------------------------------------------------\n");
  fprintf(stderr,"Atmosphere model: 1:Tropical 2:Mid-Latitude Summer 3:Mid-Latitude Winter\n");
  fprintf(stderr,"                  4:Sub-Arctic Summer 5:Sub-Arctic Winter 6:1976 US Standard\n");
  fprintf(stderr,"Aerosol model: 1,2:RURAL 3:NAVY MARITIME 4:MARITIME 5:URBAN 6:TROPOSPHERIC\n");
  fprintf(stderr,"               7:User-defined 8:Advective Fog 9:Radiative Fog 10:DESERT\n");
  fprintf(stderr,"Execution mode: 1:DSR_MODTRAN_B 2:DSR_CUSTOM_B\n");
  fprintf(stderr,"                3:SSR_MODTRAN_S 4:SSR_MODTRAN_B\n");
  fprintf(stderr,"                5:SSR_CUSTOM_S  6:SSR_CUSTOM_B\n");
  fprintf(stderr,"                7:TRN_MODTRAN_B 8:TRN_CUSTOM_B\n");
  fprintf(stderr,"Mixing mode: 0:Number ratio\n");
  fprintf(stderr,"             1:Extinction cross-section ratio\n");
  fprintf(stderr,"             2:Volume ratio\n");
  fprintf(stderr,"Config file (cnt_conf) format:\n");
  fprintf(stderr,"dsr_wlen      name # unit  | file name(%s),wlen column#(%d),unit in nm(%.1f)\n",NONAME,0,1.0);
  fprintf(stderr,"ssr_wlen      name # unit  | file name(%s),wlen column#(%d),unit in nm(%.1f)\n",NONAME,0,1.0);
  fprintf(stderr,"ssr_sbmp      name #       | file name(%s),sbmp column#(%d)\n",NONAME,0);
  fprintf(stderr,"ssr_nsam      name #       | file name(%s),nsam column#(%d)\n",NONAME,0);
  fprintf(stderr,"sim_modtran   value        | band model(%c)\n",SIM_MODTRAN);
  fprintf(stderr,"sim_speed     value        | speed(%c)\n",SIM_SPEED);
  fprintf(stderr,"sim_itype     #            | LOS type(%d)\n",SIM_ITYPE);
  fprintf(stderr,"sim_mult      value        | multiple scattering mode(%d)\n",SIM_MULT);
  fprintf(stderr,"sim_prnt      value        | NOPRNT flag(%d)\n",SIM_PRNT);
  fprintf(stderr,"sim_wlen_min  value unit   | min wlen(%.1f),unit in nm(%.1f)\n",SIM_WLEN_MIN,1.0);
  fprintf(stderr,"sim_wlen_max  value unit   | max wlen(%.1f),unit in nm(%.1f)\n",SIM_WLEN_MAX,1.0);
  fprintf(stderr,"sim_phas_wlen name # unit  | wlen for pfunc(%s),wlen column#(%d),unit in nm(%.1f)\n",NONAME,0,1.0);
  fprintf(stderr,"sim_dmax      value        | max wlen difference in nm(%.1f)\n",SIM_DMAX);
  fprintf(stderr,"sim_th_sun    value        | solar zenith  in deg(%.1f)\n",SIM_TH_SUN);
  fprintf(stderr,"sim_ph_sun    value        | solar azimuth in deg(%.1f)\n",SIM_PH_SUN);
  fprintf(stderr,"sim_th_los    value        | los   zenith  in deg(%.1f)\n",SIM_TH_LOS);
  fprintf(stderr,"sim_ph_los    value        | los   azimuth in deg(%.1f)\n",SIM_PH_LOS);
  fprintf(stderr,"sim_fdir      name         | file name(%s)\n",NONAME);
  fprintf(stderr,"sim_solcon    value        | Solar constant(%.1f)\n",SIM_SOLCON);
  fprintf(stderr,"sim_iday      value        | day of year(%d)\n",SIM_IDAY);
  fprintf(stderr,"sim_range     value        | range in km(%.1f)\n",SIM_RANGE);
  fprintf(stderr,"sim_ro        value        | Earth radius in km(%.1f)\n",SIM_RO);
  fprintf(stderr,"sim_fsun      name         | file name(%s)\n",NONAME);
  fprintf(stderr,"sim_falb      name         | file name(%s)\n",SIM_FALB);
  fprintf(stderr,"sim_salb      #            | albedo#(%d)\n",SIM_SALB);
  fprintf(stderr,"sim_mlflx     #            | #flux levels(%d)\n",SIM_MLFLX);
  fprintf(stderr,"sim_prf_vscl  # value      | mol#(%d),scale factor(%f)\n",0,NAN);
  fprintf(stderr,"sim_prf_pscl  # flag       | mol#(%d),scale flag(%d)\n",0,0);
  fprintf(stderr,"sim_prf_vsclx # value      | mol#(%d),scale factor(%f)\n",0,NAN);
  fprintf(stderr,"sim_prf_psclx # flag       | mol#(%d),scale flag(%d)\n",0,0);
  fprintf(stderr,"sim_prf_vscly # value      | mol#(%d),scale factor(%f)\n",0,NAN);
  fprintf(stderr,"sim_prf_pscly # flag       | mol#(%d),scale flag(%d)\n",0,0);
  fprintf(stderr,"mod_path_v4   name         | path name(%s)\n",MOD_PATH_V4);
  fprintf(stderr,"mod_path_v5   name         | path name(%s)\n",MOD_PATH_V5);
  fprintf(stderr,"prf_falt      name # unit  | file name(%s),column#(%d),unit in km(%.1f)\n",NONAME,0,1.0);
  fprintf(stderr,"prf_fpre      name # unit  | file name(%s),column#(%d),unit(%c)\n",NONAME,1,'A');
  fprintf(stderr,"prf_ftmp      name # unit  | file name(%s),column#(%d),unit(%c)\n",NONAME,1,'A');
  fprintf(stderr,"prf_fmol    # name # unit  | mol#(%d),file name(%s),column#(%d),unit(%c)\n",0,NONAME,1,'A');
  fprintf(stderr,"prf_fmolx   # name #       | mol#(%d),file name(%s),column#(%d)\n",0,NONAME,1);
  fprintf(stderr,"prf_jcharx    value        | jcharx(%c)\n",'A');
  fprintf(stderr,"prf_fmoly   # name #       | mol#(%d),file name(%s),column#(%d)\n",0,NONAME,1);
  fprintf(stderr,"prf_jchary    value        | jchary(%c)\n",'A');
  fprintf(stderr,"prf_fhaz    # name #       | haz#(%d),file name(%s),column#(%d)\n",0,NONAME,1);
  fprintf(stderr,"prf_pres_gnd  value        | Ground pressure(%.1f)\n",NAN);
  fprintf(stderr,"prf_temp_gnd  value        | Ground temperature(%.1f)\n",NAN);
  fprintf(stderr,"mie_angl      name # u m M | file name(%s),angl column#(%d),deg or rad(%s),min angl(%5.1f),max angl(%6.1f)\n",
                                               NONAME,0,"deg",MIE_ANGL_MIN,MIE_ANGL_MAX);
  fprintf(stderr,"mie_wlen      name # u m M | file name(%s),wlen column#(%d),unit in nm(%.1f),min wlen(%.1f),max wlen(%.1f)\n",
                                               NONAME,0,1.0,MIE_WLEN_MIN,MIE_WLEN_MAX);
  fprintf(stderr,"mie_wlen_ref  value unit   | ref wlen(%.1f),unit in nm(%.1f)\n",MIE_WLEN_REF,1.0);
  fprintf(stderr,"mie_n_step    value        | Log10(R) #steps(%d)\n",MIE_NSTP);
  fprintf(stderr,"mie_wsgm      value        | Log10(R) sigma(%.1f)\n",MIE_WSGM);
  fprintf(stderr,"mie_rmin      value        | min R in um(%.1f)\n",MIE_RMIN);
  fprintf(stderr,"mie_rmax      value        | max R in um(%.1f)\n",MIE_RMAX);
  fprintf(stderr,"mie_cmp1    name # # u # # | file name(%s),real column#(%d),imag column#(%d),wlen unit in nm(%.1f),"
                                               "min line#(%d),max line#(10^%.0f)\n",
                                               NONAME,MIE_REAL_NUM,MIE_IMAG_NUM,1.0,MIE_IMIN,log10((double)MIE_IMAX));
  fprintf(stderr,"mie_cmp2    name # # u # # | file name(%s),real column#(%d),imag column#(%d),wlen unit in nm(%.1f),"
                                               "min line#(%d),max line#(10^%.0f)\n",
                                               NONAME,MIE_REAL_NUM,MIE_IMAG_NUM,1.0,MIE_IMIN,log10((double)MIE_IMAX));
  fprintf(stderr,"mie_cmp3    name # # u # # | file name(%s),real column#(%d),imag column#(%d),wlen unit in nm(%.1f),"
                                               "min line#(%d),max line#(10^%.0f)\n",
                                               NONAME,MIE_REAL_NUM,MIE_IMAG_NUM,1.0,MIE_IMIN,log10((double)MIE_IMAX));
  fprintf(stderr,"mie_cmp4    name # # u # # | file name(%s),real column#(%d),imag column#(%d),wlen unit in nm(%.1f),"
                                               "min line#(%d),max line#(10^%.0f)\n",
                                               NONAME,MIE_REAL_NUM,MIE_IMAG_NUM,1.0,MIE_IMIN,log10((double)MIE_IMAX));
  fprintf(stderr,"mie_inp1      name         | file name(%s)\n",NONAME);
  fprintf(stderr,"mie_inp2      name         | file name(%s)\n",NONAME);
  fprintf(stderr,"mie_inp3      name         | file name(%s)\n",NONAME);
  fprintf(stderr,"mie_inp4      name         | file name(%s)\n",NONAME);
  fprintf(stderr,"upp_rh        value        | RH of upper atmosphere in %%(%.1f)\n",UPP_RH);
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
  fprintf(stderr,"Profile file (prf_fpre,prf_ftmp,prf_fmol,prf_fmolx,prf_fmoly,prf_fhaz) format: z v\n");
  fprintf(stderr,"    z  ... Altitude [km]\n");
  fprintf(stderr,"    v  ... Value\n");
  fprintf(stderr,"Component file (mie_fcmp) format: flag param1 param2 ...\n");
  fprintf(stderr,"A flag is composed of 4 characters.\n");
  fprintf(stderr,"  1st character ... Function (F=File,L=Lognormal,E=Effective Lognormal,G=Gamma,!=Lognormal)\n");
  fprintf(stderr,"  2nd character ... X type (R=Radius,L=Log10(R),N=Ln(R),!=Log10(R) if Function==L else Radius)\n");
  fprintf(stderr,"  3rd character ... Y type (N=Number distribution,V=Volume distribution,!=Number)\n");
  fprintf(stderr,"  4th character ... Shape (S=Sphere,T=Spheroid,!=Sphere)\n");
  fprintf(stderr,"Input data format (stdin) for File:                flag wcom fnam refr refi (reps) comment\n");
  fprintf(stderr,"Input data format (stdin) for Lognormal:           flag wcom rmod lsgm refr refi (reps) comment\n");
  fprintf(stderr,"Input data format (stdin) for Effective Lognormal: flag wcom reff veff refr refi (reps) comment\n");
  fprintf(stderr,"Input data format (stdin) for Gamma:               flag wcom reff veff refr refi (reps) comment\n");
  fprintf(stderr,"  flag ... Control flag composed of 4 characters\n");
  fprintf(stderr,"  wcom ... Weight of component [a.u.]\n");
  fprintf(stderr,"  rmod ... Mode radius [um]\n");
  fprintf(stderr,"  lsgm ... Sigma Log10(R [um]) or Sigma Ln(R [um])\n");
  fprintf(stderr,"  reff ... Effective radius [um]\n");
  fprintf(stderr,"  veff ... Effective variance [a.u.]\n");
  fprintf(stderr,"  fnam ... File name of size distribution\n");
  fprintf(stderr,"  refr ... File name or at mark (@) followed by single value of refractive index (real part)\n");
  fprintf(stderr,"  refi ... File name or at mark (@) followed by single value of refractive index (imaginary part)\n");
  fprintf(stderr,"  reps ... Aspect ratio of spheroid [a.u.]\n");
  fprintf(stderr,"  comment ... Name of component etc.\n");
  fprintf(stderr,"Direction file (sim_fdir) format: th_sun ph_sun th_los ph_los\n");
  fprintf(stderr,"  th_sun ... Solar zenith angle [deg]\n");
  fprintf(stderr,"  ph_sun ... Solar azimuth angle [deg]\n");
  fprintf(stderr,"  th_los ... LOS zenith angle [deg]\n");
  fprintf(stderr,"  ph_los ... LOS azimuth angle [deg]\n");

  return 0;
}
