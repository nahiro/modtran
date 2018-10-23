/*********************************************************/
/* run_modtran                                           */
/* Author: N.Manago Apr,30,2008                          */
/* $Revision: 228 $                                      */
/* $Date: 2014-04-18 16:10:27 +0900 (Fri, 18 Apr 2014) $ */
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
#define	ZERO				0.0			// 0
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
#define	DSR_MAXWLEN			100000			// Max #wavelengths for DSR
// Constants for Scattered Solar Radiation
#define	SSR_MAXWLEN			100000			// Max #wavelengths for SSR
// Constants for Simulation
#define	SIM_N_ANGL			50			// #Angles
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
#define	SIM_IDAY			95			// Day of year
#define	SIM_IHAZ			1			// Aerosol model#
#define	SIM_ICLD			0			// Cloud model#
#define	SIM_DBMP			1			// DSR Band model number
#define	SIM_SBMP			1			// SSR Band model number
#define	SIM_CFLG			0			// Correction flag
#define	SIM_DFLG			1			// DISORT flag
#define	SIM_NSTR			16			// Number of streams for DISORT
#define	SIM_WFLG			0			// H2OAER flag
#define	SIM_NSAM			4			// #Sampling
#define	SIM_MDEF			0			// CARD2C2X read flag
#define	SIM_IM				0			// CARD2C read flag
#define	SIM_2C				0			// CARD2C flag
#define	SIM_CMIX			365.0			// CO2 mixing ratio in ppmv
#define	SIM_NPAR			37			// #Parameters
#define	SIM_VTAU			NAN			// Aerosol optical depth
#define	SIM_VMIE			20.0			// Visibility in km
#define	SIM_WSCL			1.0			// Water scale
#define	SIM_OSCL			1.0			// Ozone scale
#define	SIM_GRHO			0.3			// Ground albedo
#define	SIM_H1				0.0			// H1 altitude
#define	SIM_H2				0.0			// H2 altitude
#define	SIM_RANGE			0.0			// Range
#define	SIM_XMGN			16.0			// X margin
#define	SIM_XSGM			4.0			// X sigma
#define	SIM_WSGM			20.0			// X width
#define	SIM_YUNI			10.0			// Y unit
#define	SIM_WLEN_MIN			350.0			// Min wavelength in nm
#define	SIM_WLEN_MAX			1050.0			// Max wavelength in nm
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
#define	SIM_MODE_TRN_MODTRAN_BO		7			// Simulation mode
#define	SIM_MODE_TRN_CUSTOM_BO		8			// Simulation mode
#define	SIM_PRF_CUSTOM			1			// Simulation mode
#define	SIM_PRF_MODTRAN			2			// Simulation mode
#define	SIM_PATH			"/usr/local/Mod5.2.0.0"	// Path
#define	SIM_FALB			"DATA/spec_alb.dat"	// Spectral albedo file
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
#define	MIE_N_WLEN			11			// #Wavelengths
#define	MIE_MAXDATA			1000			// Max #data
#define	MIE_WLEN_REF			550.0			// Reference wavelength in nm
#define	MIE_WLEN_MIN			250.0			// Min wavelength in nm
#define	MIE_WLEN_MAX			1500.0			// Max wavelength in nm
// Constants for Cloud calculation
#define	CLD_THIK			0.0			// Cloud thickness
#define	CLD_ALT				0.0			// Cloud altitude
#define	CLD_EXT				0.0			// Cloud extinction
// Constants for control
#define	CNT_CONF			NONAME
#define	CNT_MAXCMNT			30
#define	CNT_MAXFORM			30
#define	CNT_MAXNOPT			100			// Max #options
// Functions
#define	MAX(a,b)			((b)>(a)?(b):(a))
#define	INTERP(x,x1,x2,y1,y2)		(y1+(y2-y1)/(x2-x1)*(x-x1))

extern float aerprf_(int*,float*,int*,int*,int*);

int Init(void);
void Finish(void);
int GetOpt(int argn,char **args);
int Usage(void);
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
int	*ssr_sbmp			= NULL;			// SSR Band model number
double	*ssr_wlen			= NULL;			// Wavelength
double	*ssr_dsim			= NULL;			// Simulation data
// Parameters for Simulation
int	sim_mode			= SIM_MODE_DSR_MODTRAN_BO; // Simulation mode
int	sim_n_angl			= SIM_N_ANGL;		// #Angles
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
int	sim_prf_mode			= SIM_PRF_MODTRAN;	// Simulation mode
int	sim_cflg			= SIM_CFLG;		// Correction flag
int	sim_dflg			= SIM_DFLG;		// DISORT flag
int	sim_nstr			= SIM_NSTR;		// Number of streams for DISORT
int	sim_wflg			= SIM_WFLG;		// H2OAER flag
int	sim_nsam			= SIM_NSAM;		// #Sampling
int	sim_mdef			= SIM_MDEF;		// CARD2C2X read flag
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
double	sim_range			= SIM_RANGE;		// Range
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
double	sim_wlen_min			= SIM_WLEN_MIN;		// Min wavelength in nm
double	sim_wlen_max			= SIM_WLEN_MAX;		// Max wavelength in nm
double	sim_th_sun			= SIM_TH_SUN;		// Solar zenith
double	sim_ph_sun			= SIM_PH_SUN;		// Solar azimuth (N=0,CW)
double	sim_th_los			= SIM_TH_LOS;		// LOS zenith
double	sim_ph_los			= SIM_PH_LOS;		// LOS azimuth (N=0,CW)
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
char	sim_path[MAXLINE]		= SIM_PATH;
char	sim_fsun[MAXLINE]		= NONAME;		// TOA solar irradiance file
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
double	mie_wlen[MIE_MAXDATA]		=
{
  300.0, 337.1, 400.0,  488.0,  514.5, 550.0,
  632.8, 694.3, 860.0, 1060.0, 1300.0,
};
double	*mie_wlen_um			= NULL;
double	*mie_aext			= NULL;
double	*mie_asca			= NULL;
double	*mie_asym			= NULL;
// Parameters for Cloud calculation
double	cld_thik			= CLD_THIK;		// Cloud thickness
double	cld_alt				= CLD_ALT;		// Cloud altitude
double	cld_ext				= CLD_EXT;		// Cloud extinction
// Parameters for control
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
  int i;
  int err;
  double par[6];

  if(GetOpt(argc,argv) < 0) return -1;
  if(cnt_hp) {Usage(); return 0;}
  if(Init() < 0) return -1;

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
      par[1] = (double)sim_nsam;
      par[2] = sim_th_sun;
      par[3] = sim_ph_sun;
      par[4] = sim_th_los;
      par[5] = sim_ph_los;
      if(ssr_n_wlen > 0)
      {
        for(i=0; i<ssr_n_wlen; i++)
        {
          sim_sbmp = ssr_sbmp[i];
          par[0] = ssr_wlen[i];
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
    if((ssr_dsim=(double*)malloc(ssr_n_wlen*sizeof(double))) == NULL)
    {
      fprintf(stderr,"Error in allocating memory for %s\n","ssr_dsim");
      return -1;
    }
  }

  return 0;
}

void Finish(void)
{
  int err = 0;

  // Deallocate memories
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
    err += system("rm -f modtran.tp6");
    err += system("rm -f modtran.tp7");
    err += system("rm -f modtran.tp8");
  }
  if(cnt_cflg > 1)
  {
    err += system("rm -f DATA");
    err += system("rm -f mod5root.in");
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
  for(i=0; i<PRF_NMOLY; i++)
  {
    sim_prf_pscly[i] = 0;
    sim_prf_vscly[i] = NAN;
    for(j=0; j<prf_n_alt; j++)
    {
      sim_prf_ymol[i][j] = 0.0;
    }
  }

  for(i=0; i<PRF_NPAR; i++)
  {
    prf_jchar[i] = 'A';
    sim_prf_jchar[i] = (char)('0'+imod);
  }
  prf_jcharx = 'A';
  sim_prf_jcharx = (char)('0'+imod);
  prf_jchary = 'A';
  sim_prf_jchary = '1';

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

  return 0;
}

int PrfScaleAerosol(double t)
{
  int i,j;
  double f;

  if(!isnan(t))
  {
    sim_2c |= 0x1000;
    f = (t-prf_aod_hi)/prf_aod_lo;
    for(i=0; i<PRF_NHAZ_LOW; i++)
    {
      for(j=0; j<prf_n_alt; j++)
      {
        sim_prf_haze[i][j] = prf_haze[i][j]*f;
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

  sim_mdef = ((sim_2c&0x0100)>0?2:0);
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
    case SIM_MODE_TRN_MODTRAN_BO: // Transmittance  with MODTRAN aerosol model
    case SIM_MODE_TRN_CUSTOM_BO:  // Transmittance  with custom  aerosol model
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
  char sim_lymolc = '+';
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
  } else
  if(mode==SIM_MODE_TRN_MODTRAN_BO || mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    sim_iemsct = 0;
    sim_yuni = 1.0;
  }
  else
  {
    fprintf(stderr,"%s: invalid mode >>> %d\n",fnam,mode);
    return -1;
  }
  if(sim_2c > 0)
  {
    sim_iatm = 7;
    if((sim_2c&0x0200) == 0)
    {
      sim_lymolc = ' ';
    }
  }

  // CARD 1
  if(sim_salb > 0)
  {
    fprintf(fp,"MMF%c%d%5d%5d%5d    0    0    0    0    0    0%5d%5d %4d    .000 LAMBER\n",
                sim_lymolc,sim_iatm,sim_itype,sim_iemsct,sim_mult,sim_mdef,sim_im,sim_prnt);
  }
  else
  {
    fprintf(fp,"MMF%c%d%5d%5d%5d    0    0    0    0    0    0%5d%5d %4d    .000%7.4f\n",
                sim_lymolc,sim_iatm,sim_itype,sim_iemsct,sim_mult,sim_mdef,sim_im,sim_prnt,sim_grho);
  }
  if(sim_iemsct == 3)
  {
    if(strcmp(sim_fsun,NONAME) != 0)
    {
      // CARD 1A
      cprintf(fp,"tt   8 0.0%10.3f%10.0f%10.0f0t t f %s\n",
                  sim_cmix,sim_wscl,sim_oscl,sim_wflg==1?"t":"f");
      // CARD 1A1
      fprintf(fp,"%s\n",sim_fsun);
    }
    else
    {
      // CARD 1A
      cprintf(fp,"tt   8 0.0%10.3f%10.0f%10.0f0f t f %s\n",
                  sim_cmix,sim_wscl,sim_oscl,sim_wflg==1?"t":"f");
    }
    // CARD 1A2
    if(sim_dbmp < 0)
    {
      fprintf(fp,"p%d_2008\n",-sim_dbmp);
    }
    else
    {
      fprintf(fp,"%02d_2008\n",sim_dbmp);
    }
  } else
  if(sim_iemsct == 0)
  {
    if(strcmp(sim_fsun,NONAME) != 0)
    {
      // CARD 1A
      cprintf(fp,"tt %3d 0.0%10.3f+%9.0f%10.0f0t t f %s\n",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,sim_wflg==1?"t":"f");
      // CARD 1A1
      fprintf(fp,"%s\n",sim_fsun);
    }
    else
    {
      // CARD 1A
      cprintf(fp,"tt %3d 0.0%10.3f+%9.0f%10.0f0f t f %s\n",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,sim_wflg==1?"t":"f");
    }
    // CARD 1A2
    if(sim_dbmp < 0)
    {
      fprintf(fp,"p%d_2008\n",-sim_dbmp);
    }
    else
    {
      fprintf(fp,"%02d_2008\n",sim_dbmp);
    }
  }
  else
  {
    if(strcmp(sim_fsun,NONAME) != 0)
    {
      // CARD 1A
      cprintf(fp,"%st %3d 0.0%10.3f%10.0f%10.0f0t t f %s\n",sim_dflg==0?"f":"t",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,sim_wflg==1?"t":"f");
      // CARD 1A1
      fprintf(fp,"%s\n",sim_fsun);
    }
    else
    {
      // CARD 1A
      cprintf(fp,"%st %3d 0.0%10.3f%10.0f%10.0f0f t f %s\n",sim_dflg==0?"f":"t",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,sim_wflg==1?"t":"f");
    }
    // CARD 1A2
    if(sim_sbmp < 0)
    {
      fprintf(fp,"p%d_2008\n",-sim_sbmp);
    }
    else
    {
      fprintf(fp,"%02d_2008\n",sim_sbmp);
    }
  }

  return 0;
}

int WriteCard2(FILE *fp,int mode)
{
  // CARD 2
  cprintf(fp,"  %3d    0   %2d    0   %2d    0%10.5f      .000      .000      .000      .000\n",
          sim_ihaz,sim_ivul,sim_icld,sim_vmie);
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
  if(mode==SIM_MODE_DSR_CUSTOM_BO || mode==SIM_MODE_SSR_CUSTOM_SO || mode==SIM_MODE_SSR_CUSTOM_BO ||
     mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    // CARD 2D
    WriteCard2D(fp);
  }

  return 0;
}

int WriteCard2C(FILE *fp)
{
  int i;
  int f2,f3,f4,f5;
  char jchar[15] = "";
  char jcharx[2] = " ";
  char jchary[2] = " ";

  if(sim_2c <= 0)
  {
    return 0;
  }
  else
  {
    f2 = (sim_2c&0x0010)|(sim_2c&0x0100)|(sim_2c&0x0200);
    f3 = (sim_2c&0x0100);
    f4 = (sim_2c&0x0200);
    f5 = (sim_2c&0x1000);
  }

  for(i=0; i<PRF_NPAR; i++)
  {
    jchar[i] = sim_prf_jchar[i];
  }
  jchar[i] = '\0';
  jcharx[0] = sim_prf_jcharx;
  jchary[0] = sim_prf_jchary;

  // CARD 2C
  fprintf(fp,"%5d%5d%5d%20s%10.0f%5d\n",prf_n_alt,(f2>0?1:0),(f5>0?2:0),"Custom",0.0,0);
  for(i=0; i<prf_n_alt; i++)
  {
    // CARD 2C1
    fprintf(fp,"%10.3f%10.3e%10.3e%10.3e%10.3e%10.3e%14s %1s%1s\n",
                prf_alt[i],sim_prf_pres[i],sim_prf_temp[i],
                sim_prf_wmol[0][i],sim_prf_wmol[1][i],sim_prf_wmol[2][i],jchar,jcharx,jchary);
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
      // CARD 2C2X
      fprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_xmol[0][i],sim_prf_xmol[1][i],sim_prf_xmol[2][i],sim_prf_xmol[3][i],
                  sim_prf_xmol[4][i],sim_prf_xmol[5][i],sim_prf_xmol[6][i],sim_prf_xmol[7][i]);
      fprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_xmol[8][i],sim_prf_xmol[9][i],sim_prf_xmol[10][i],sim_prf_xmol[11][i],
                  sim_prf_xmol[12][i]);
    }
    if(f4 > 0)
    {
      // CARD 2C2Y
      fprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_ymol[0][i],sim_prf_ymol[1][i],sim_prf_ymol[2][i],sim_prf_ymol[3][i],
                  sim_prf_ymol[4][i],sim_prf_ymol[5][i],sim_prf_ymol[6][i],sim_prf_ymol[7][i]);
      fprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_ymol[8][i],sim_prf_ymol[9][i],sim_prf_ymol[10][i],sim_prf_ymol[11][i],
                  sim_prf_ymol[12][i],sim_prf_ymol[13][i],sim_prf_ymol[14][i],sim_prf_ymol[15][i]);
    }
    if(f5 > 0)
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
  // TRN_MODTRAN_BO || TRN_CUSTOM_BO
  // par[2] = los   zenith
  double ph;
  char fnam[] = "WriteCard3";

  if(mode==SIM_MODE_DSR_MODTRAN_BO || mode==SIM_MODE_DSR_CUSTOM_BO)
  {
    // CARD 3
    fprintf(fp,"%10.4f%10.4f%10.6f%5d%5s%10.3f%5d%10.3f\n",
                sim_h1,sim_h2,par[2],sim_iday,"",ZERO,0,ZERO);
  } else
  if(mode==SIM_MODE_SSR_MODTRAN_SO || mode==SIM_MODE_SSR_MODTRAN_BO ||
     mode==SIM_MODE_SSR_CUSTOM_SO  || mode==SIM_MODE_SSR_CUSTOM_BO)
  {
    // CARD 3
    fprintf(fp,"%10.4f%10.4f%10.6f%10.4f%10.3f%10.3f%5d%5s%10.3f\n",
                sim_h1,sim_h2,par[4],sim_range,ZERO,ZERO,0,"",ZERO);
    if(sim_iemsct == 2)
    {
      // CARD 3A1
      if(mode==SIM_MODE_SSR_CUSTOM_SO  || mode==SIM_MODE_SSR_CUSTOM_BO)
      {
        fprintf(fp,"%5d%5d%5d%5d\n",2,1,sim_iday,0);
      }
      else
      {
        fprintf(fp,"%5d%5d%5d%5d\n",2,2,sim_iday,0);
      }
      ph = par[3]-par[5];
      if(ph < -180.0) ph += 360.0;
      // CARD 3A2
      fprintf(fp,"%10.5f%10.6f%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f\n",
                  ph,par[2],ZERO,ZERO,ZERO,ZERO,ZERO,ZERO);
      if(mode==SIM_MODE_SSR_CUSTOM_SO  || mode==SIM_MODE_SSR_CUSTOM_BO)
      {
        // CARD 3B1
        fprintf(fp,"%5d%5d\n",sim_n_angl,mie_n_wlen);
        // CARD 3C
        WriteCard3C(fp);
      }
    }
  } else
  if(mode==SIM_MODE_TRN_MODTRAN_BO || mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    // CARD 3
    fprintf(fp,"%10.4f%10.4f%10.6f%10.4f%10.3f%10.3f%5d%5s%10.3f\n",
                sim_h1,sim_h2,par[2],sim_range,ZERO,ZERO,0,"",ZERO);
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
  // DSR_MODTRAN_BO || DSR_CUSTOM_BO || SSR_MODTRAN_BO || SSR_CUSTOM_BO ||
  // TRN_MODTRAN_BO || TRN_CUSTOM_BO
  // par[0] = min wavelength
  // par[1] = max wavelength
  // SSR_MODTRAN_S0 || SSR_CUSTOM_S0
  // par[0] = wavelength
  // par[1] = #data in modtran.plt
  int n1,n2;
  double band;
  char fnam[] = "WriteCard4";

  // CARD 4
  if(mode==SIM_MODE_SSR_MODTRAN_SO || mode==SIM_MODE_SSR_CUSTOM_SO)
  {
    if(sim_sbmp < 0)
    {
      band = 0.1;
      n1 = (int)(band*((int)((1.0e7/(par[0]*band)-0.5*(par[1]-1.0))+0.5))); // temporary
      n2 = (int)(band*((int)(par[1]+0.5)))+n1-1; // temporary
    }
    else
    {
      band = (double)sim_sbmp;
      n1 = sim_sbmp*((int)((1.0e7/(par[0]*sim_sbmp)-0.5*(par[1]-1.0))+0.5));
      n2 = sim_sbmp*((int)(par[1]+0.5))+n1-sim_sbmp;
    }
    fprintf(fp,"%10d%10d%10.1f%10.1frn\n",n1,n2,band,2.0*band);
  } else
  if(mode==SIM_MODE_DSR_MODTRAN_BO || mode==SIM_MODE_DSR_CUSTOM_BO ||
     mode==SIM_MODE_SSR_MODTRAN_BO || mode==SIM_MODE_SSR_CUSTOM_BO ||
     mode==SIM_MODE_TRN_MODTRAN_BO || mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    if(mode==SIM_MODE_SSR_MODTRAN_BO || mode==SIM_MODE_SSR_CUSTOM_BO)
    {
      if(sim_sbmp < 0)
      {
        band = 0.1;
      }
      else
      {
        band = (double)sim_sbmp;
      }
      fprintf(fp,"%10.0f%10.0f%10.1f%10.1frn\n",1.0e7/par[1],1.0e7/par[0],band,2.0*band);
    }
    else
    {
      if(sim_dbmp < 0)
      {
        band = 0.1;
      }
      else
      {
        band = (double)sim_dbmp;
      }
      fprintf(fp,"%10.0f%10.0f%10.1f%10.1frn\n",1.0e7/par[1],1.0e7/par[0],band,2.0*band);
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

  if((fp=fopen("mod5root.in","w")) == NULL)
  {
    fprintf(stderr,"Error, cannot open %s\n","mod5root.in");
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
  snprintf(line,MAXLINE,"%s/Mod90_5.2.0.0.exe",sim_path);
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
    for(n=nc=0,p=temp; n<MAXENTR; n++,p+=nc)
    {
      if(sscanf(p,"%s%n",str[n],&nc) == EOF) break;
    }
    if(n < 1) continue;
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
          if((dsr_n_wlen=ReadXP(str[1],DSR_MAXWLEN,num,uni,NULL,&dsr_wlen)) < 1)
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
          if((ssr_n_wlen=ReadXP(str[1],SSR_MAXWLEN,num,uni,NULL,&ssr_wlen)) < 1)
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30d\n",str[0],sim_itype);
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30d\n",str[0],sim_mult);
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30.14e\n",str[0],sim_wlen_min);
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30.14e\n",str[0],sim_wlen_max);
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30d\n",str[0],sim_iday);
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30.14e\n",str[0],sim_range);
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %2d %27.14e\n",str[0],idx,sim_prf_vsclx[idx]);
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %2d %27d\n",str[0],idx,sim_prf_psclx[idx]);
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %2d %27.14e\n",str[0],idx,sim_prf_vscly[idx]);
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %2d %27d\n",str[0],idx,sim_prf_pscly[idx]);
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
          if((nc=ReadXYP(str[1],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
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
          if((nc=ReadXYP(str[2],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
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
          if((nc=ReadXYP(str[2],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30c\n",str[0],prf_jcharx);
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
          if((nc=ReadXYP(str[2],PRF_MAXNALT,0,num,1.0,1.0,NULL,&x,&y)) < 0)
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
        sprintf(cnt_cmnt[cnt_n_cmnt],"%-14s: %30c\n",str[0],prf_jchary);
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
  int v[3];
  char line[MAXLINE];
  char str[MAXENTR][MAXLINE];
  char fnam[] = "ReadIA";
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
      v[n] = strtol(str[c[n]],&p,10);
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
    {"sim_nstr",1,0,'N'},
    {"sim_mode",1,0,'S'},
    {"cnt_conf",1,0,'C'},
    {"sim_dflg",0,0,'D'},
    {"cnt_cflg",0,0,'c'},
    {"cnt_db",0,0,'d'},
    {"cnt_vb",0,0,'v'},
    {"cnt_hp",0,0,'h'}
  };

  rt = 0;
  while(1)
  {
    this_option_optind = optind?optind:1;
    c = getopt_long(argn,args,":U:V:W:O:P:A:a:k:G:g:T:w:m:M:N:S:C:Dcdvh",long_options,&option_index);
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
        if(errno!=ERANGE && *endp=='\0' && (ntmp==-1||ntmp==1||ntmp==5||ntmp==15)) sim_dbmp = ntmp;
        else
        {
          fprintf(stderr,"DSR Band model number -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'M':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && (ntmp==-1||ntmp==1||ntmp==5||ntmp==15)) sim_sbmp_init = ntmp;
        else
        {
          fprintf(stderr,"SSR Band model number -> out of range %s\n",optarg);
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
      case 'C':
        strncpy(cnt_conf,optarg,MAXLINE);
        break;
      case 'D':
        sim_dflg = 1-sim_dflg;
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
  fprintf(stderr,"run_modtran5 -[option] (argument) -[option] (argument) ...\n");
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
  fprintf(stderr," m -sim_dbmp      |%s|%s|%s| %d\n",As(e,"DSR Band model",n), As(a,"-1|1|5|15",n),  Ad(d,SIM_DBMP,n),sim_dbmp);
  fprintf(stderr," M -sim_sbmp_init |%s|%s|%s| %d\n",As(e,"SSR Band model",n), As(a,"-1|1|5|15",n),  Ad(d,SIM_SBMP,n),sim_sbmp_init);
  fprintf(stderr," N -sim_nstr      |%s|%s|%s| %d\n",As(e,"#Streams",n),       As(a,"2|4|8|16|32",n),Ad(d,SIM_NSTR,n),sim_nstr);
  fprintf(stderr," S -sim_mode      |%s|%s|%s| %d\n",As(e,"Execution mode",n), As(a,"1~8",n),        Ad(d,SIM_MODE_DSR_MODTRAN_BO,n),sim_mode);
  fprintf(stderr," D -sim_dflg      |%s|%s|%s| %d\n",As(e,"DISORT  flag",n),   As(a,"nothing",n),    Ad(d,SIM_DFLG,n),sim_dflg);
  fprintf(stderr," C -cnt_conf      |%s|%s|%s| %s\n",As(e,"Config  file",n),   As(a,"name",n),       As(d,CNT_CONF,n),cnt_conf);
  fprintf(stderr," c -cnt_cflg      |%s|%s|%s| %d\n",As(e,"Clear   flag",n),   As(a,"nothing",n),    Ad(d,0,n),cnt_cflg);
  fprintf(stderr," d -cnt_db        |%s|%s|%s| %d\n",As(e,"Debug   mode",n),   As(a,"nothing",n),    Ad(d,0,n),cnt_db);
  fprintf(stderr," v -cnt_vb        |%s|%s|%s| %d\n",As(e,"Verbose mode",n),   As(a,"nothing",n),    Ad(d,0,n),cnt_vb);
  fprintf(stderr," h -cnt_hp        |%s|%s|%s| %d\n",As(e,"Help    mode",n),   As(a,"nothing",n),    Ad(d,0,n),1);
  fprintf(stderr,"------------------------------------------------------------------------------------\n");
  fprintf(stderr,"Execution mode: 1:DSR_MODTRAN_B 2:DSR_CUSTOM_B\n");
  fprintf(stderr,"                3:SSR_MODTRAN_S 4:SSR_MODTRAN_B\n");
  fprintf(stderr,"                5:SSR_CUSTOM_S  6:SSR_CUSTOM_B\n");
  fprintf(stderr,"                7:TRN_MODTRAN_B 8:TRN_CUSTOM_B\n");
  fprintf(stderr,"Config file (cnt_conf) format:\n");
  fprintf(stderr,"dsr_wlen      name # unit  | file name(%s),wlen column#(%d),unit in nm(%.1f)\n",NONAME,0,1.0);
  fprintf(stderr,"ssr_wlen      name # unit  | file name(%s),wlen column#(%d),unit in nm(%.1f)\n",NONAME,0,1.0);
  fprintf(stderr,"ssr_sbmp      name #       | file name(%s),sbmp column#(%d)\n",NONAME,0);
  fprintf(stderr,"sim_itype     #            | LOS type(%d)\n",SIM_ITYPE);
  fprintf(stderr,"sim_mult      value        | multiple scattering mode(%d)\n",SIM_MULT);
  fprintf(stderr,"sim_wlen_min  value unit   | min wlen(%.1f),unit in nm(%.1f)\n",SIM_WLEN_MIN,1.0);
  fprintf(stderr,"sim_wlen_max  value unit   | max wlen(%.1f),unit in nm(%.1f)\n",SIM_WLEN_MAX,1.0);
  fprintf(stderr,"sim_th_sun    value        | solar zenith  in deg(%.1f)\n",SIM_TH_SUN);
  fprintf(stderr,"sim_ph_sun    value        | solar azimuth in deg(%.1f)\n",SIM_PH_SUN);
  fprintf(stderr,"sim_th_los    value        | los   zenith  in deg(%.1f)\n",SIM_TH_LOS);
  fprintf(stderr,"sim_ph_los    value        | los   azimuth in deg(%.1f)\n",SIM_PH_LOS);
  fprintf(stderr,"sim_iday      value        | day of year(%d)\n",SIM_IDAY);
  fprintf(stderr,"sim_range     value        | range in km(%.1f)\n",SIM_RANGE);
  fprintf(stderr,"sim_path      name         | path name(%s)\n",SIM_PATH);
  fprintf(stderr,"sim_fsun      name         | file name(%s)\n",NONAME);
  fprintf(stderr,"sim_falb      name         | file name(%s)\n",SIM_FALB);
  fprintf(stderr,"sim_salb      #            | albedo#(%d)\n",SIM_SALB);
  fprintf(stderr,"sim_prf_vscl  # value      | mol#(%d),scale factor(%f)\n",0,NAN);
  fprintf(stderr,"sim_prf_pscl  # flag       | mol#(%d),scale flag(%d)\n",0,0);
  fprintf(stderr,"sim_prf_vsclx # value      | mol#(%d),scale factor(%f)\n",0,NAN);
  fprintf(stderr,"sim_prf_psclx # flag       | mol#(%d),scale flag(%d)\n",0,0);
  fprintf(stderr,"sim_prf_vscly # value      | mol#(%d),scale factor(%f)\n",0,NAN);
  fprintf(stderr,"sim_prf_pscly # flag       | mol#(%d),scale flag(%d)\n",0,0);
  fprintf(stderr,"prf_falt      name # unit  | file name(%s),column#(%d),unit in km(%.1f)\n",NONAME,0,1.0);
  fprintf(stderr,"prf_fpre      name # unit  | file name(%s),column#(%d),unit(%c)\n",NONAME,1,'A');
  fprintf(stderr,"prf_ftmp      name # unit  | file name(%s),column#(%d),unit(%c)\n",NONAME,1,'A');
  fprintf(stderr,"prf_fmol    # name # unit  | mol#(%d),file name(%s),column#(%d),unit(%c)\n",0,NONAME,1,'A');
  fprintf(stderr,"prf_fmolx   # name #       | mol#(%d),file name(%s),column#(%d)\n",0,NONAME,1);
  fprintf(stderr,"prf_jcharx value           | jcharx(%c)\n",'A');
  fprintf(stderr,"prf_fmoly   # name #       | mol#(%d),file name(%s),column#(%d)\n",0,NONAME,1);
  fprintf(stderr,"prf_jchary value           | jchary(%c)\n",'A');
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
  fprintf(stderr,"Profile file (prf_fpre,prf_ftmp,prf_fmol,prf_fmolx,prf_fmoly,prf_fhaz) format: z v\n");
  fprintf(stderr,"    z  ... Altitude [km]\n");
  fprintf(stderr,"    v  ... Value\n");

  return 0;
}
