#define	NZCLD				16	// The maximum allowed value for NCRALT is 16,
						// parameter NZCLD in PARAM.LST.
#define	MXWVLN				788	// The maximum number of wavelengths (parameter MXWVLN in PARAM.LST)
						// is set to 788.
#define	MXANGL				50	// NANGLS = number of angles for the user-defined
						// phase functions (maximum of 50).
#define	MWVSRF				50	// The maximum allowed value for NWVSRF is defined
						// by the parameter MWVSRF in the PARAM.LST file.
						// If necessary, the user can increase MWVSRF and
						// then recompile MODTRAN. Upon delivery of MODTRAN,
						// MWVSRF is set to 50.
#define	NAERO				4	// NUMBER OF AEROSOLS
#define	MPARAM				5	// MAXIMUM NUMBER OF PARAMETERS IN BRDF REPRESENTATION.
#define	LAYDIM				502	// MAXIMUM NUMBER OF LAYER BOUNDARY.
#define	MXIREG				4
#define	MXNSRF				2
#define	MXNSPC				47
#define	NCOL3				3
#define	NCOL4				4
#define	NCOL5				5
#define	NCOL6				6
#define	MAXLINE				256
#define	MOD_V4				4000			// MODTRAN version 4
#define	MOD_V5				5000			// MODTRAN version 5
#define	NCARD				38
#define	CARD_LENGTH			256
#define	CARD2C1_JCHAR_NINDX		14
#define	CARD2C2_WMOL_NINDX		9
#define	CARD2C2X_WMOLX_NINDX		13
#define	CARD1_NPAR			20
#define	CARD1A_NPAR			13
#define	CARD1A1_NPAR			1
#define	CARD1A2_NPAR			1
#define	CARD1A3_NPAR			1
#define	CARD2_NPAR			14
#define	CARD2A_NPAR			11
#define	CARD2AP_NPAR			3
#define	CARD2B_NPAR			3
#define	CARD2C_NPAR			5
#define	CARD2C1_NPAR			8
#define	CARD2C2_NPAR			1
#define	CARD2C2X_NPAR			1
#define	CARD2C3_NPAR			8
#define	CARD2D_NPAR			1
#define	CARD2D1_NPAR			2
#define	CARD2D2_NPAR			4
#define	CARD2E1_NPAR			4
#define	CARD2E2_NPAR			10
#define	CARD3_NPAR			11
#define	CARD3A1_NPAR			4
#define	CARD3A2_NPAR			8
#define	CARD3B1_NPAR			2
#define	CARD3B2_NPAR			5
#define	CARD3C1_NPAR			1
#define	CARD3C2_NPAR			1
#define	CARD3C3_NPAR			1
#define	CARD3C4_NPAR			1
#define	CARD3C5_NPAR			1
#define	CARD3C6_NPAR			1
#define	CARD4_NPAR			9
#define	CARD4A_NPAR			2
#define	CARD4B1_NPAR			1
#define	CARD4B2_NPAR			3
#define	CARD4B3_NPAR			2
#define	CARD4L1_NPAR			1
#define	CARD4L2_NPAR			1
#define	CARD5_NPAR			1

int	mod_v	= MOD_V4;

typedef struct
{
  char		MODTRN[CARD_LENGTH];
  char		SPEED[CARD_LENGTH];
  char		BINARY[CARD_LENGTH];
  char		LYMOLC[CARD_LENGTH];
  int		MODEL;
  int		ITYPE;
  int		IEMSCT;
  int		IMULT;
  int		M1;
  int		M2;
  int		M3;
  int		M4;
  int		M5;
  int		M6;
  int		MDEF;
  int		IM;
  int		I_RD2C;
  int		NOPRNT;
  double	TPTEMP;
  char		SURREF[CARD_LENGTH];
} CARD1_struct;

typedef struct
{
  char		DIS[2];
  char		DISAZM[2];
  int		NSTR;
  char		LSUN[2];
  int		ISUN;
  double	CO2MX;
  char		H2OSTR[16];
  char		O3STR[16];
  char		LSUNFL[2];
  char		LBMNAM[2];
  char		LFLTNM[2];
  char		H2OAER[2];
  double	SOLCON;
} CARD1A_struct;

typedef struct
{
  char		SUNFL2[MAXLINE];
} CARD1A1_struct;

typedef struct
{
  char		BMNAME[MAXLINE];
} CARD1A2_struct;

typedef struct
{
  char		FILTNM[MAXLINE];
} CARD1A3_struct;

typedef struct
{
  char		APLUS[3];
  int		IHAZE;
  char		CNOVAM[2];
  int		ISEASN;
  char		ARUSS[4];
  int		IVULCN;
  int		ICSTL;
  int		ICLD;
  int		IVSA;
  double	VIS;
  double	WSS;
  double	WHH;
  double	RAINRT;
  double	GNDALT;
} CARD2_struct;

typedef struct
{
  double	ZAER1[4];
  double	ZAER2[4];
  double	SCALE[4];
} CARD2AP_struct;

typedef struct
{
  double CTHIK;
  double CALT;
  double CEXT;
  int NCRALT;
  int NCRSPC;
  double CWAVLN;
  double CCOLWD;
  double CCOLIP;
  double CHUMID;
  double ASYMWD;
  double ASYMIP;
} CARD2A_struct;

typedef struct
{
  double ZCVSA;
  double ZTVSA;
  double ZINVSA;
} CARD2B_struct;

typedef struct
{
  int ML;
  int IRD1;
  int IRD2;
  char HMODEL[21];
  double REE;
} CARD2C_struct;

typedef struct
{
  double ZM;
  double P;
  double T;
  double WMOL1;
  double WMOL2;
  double WMOL3;
  char JCHAR[CARD2C1_JCHAR_NINDX][2];
  char JCHARX[2];
} CARD2C1_struct;

typedef struct
{
  double WMOL[CARD2C2_WMOL_NINDX];
} CARD2C2_struct;

typedef struct
{
  double WMOLX[CARD2C2X_WMOLX_NINDX];
} CARD2C2X_struct;

typedef struct
{
  double AHAZE[4];
  double EQLWCZ;
  double RRATZ;
  int IHA1;
  int ICLD1;
  int IVUL1;
  int ISEA1;
  int ICHR;
} CARD2C3_struct;

typedef struct
{
  int IREG[4];
} CARD2D_struct;

typedef struct
{
  double AWCCON;
  char TITLE[73];
} CARD2D1_struct;

typedef struct
{
  double VARSPC;
  double EXTC;
  double ABSC;
  double ASYM;
} CARD2D2_struct;

typedef struct
{
  double ZCLD;
  double CLD;
  double CLDICE;
  double RR;
} CARD2E1_struct;

typedef struct
{
  double WAVLEN;
  double EXTC6;
  double ABSC6;
  double ASYM6;
  double EXTC7;
  double ABSC7;
  double ASYM7;
  char CFILE[MAXLINE];
  char CLDTYP[MAXLINE];
  char CIRTYP[MAXLINE];
} CARD2E2_struct;

typedef struct
{
  double H1;
  double H2;
  double ANGLE;
  double RANGE;
  double BETA;
  double RO;
  int LENN;
  double PHI;
  int IDAY;
  int ISOURC;
  double ANGLEM;
} CARD3_struct;

typedef struct
{
  int IPARM;
  int IPH;
  int IDAY;
  int ISOURC;
} CARD3A1_struct;

typedef struct
{
  double PARM1;
  double PARM2;
  double PARM3;
  double PARM4;
  double TIME;
  double PSIPO;
  double ANGLEM;
  double G;
} CARD3A2_struct;

typedef struct
{
  int NANGLS;
  int NWLF;
} CARD3B1_struct;

typedef struct
{
  double ANGF;
  double F1;
  double F2;
  double F3;
  double F4;
} CARD3B2_struct;

typedef struct
{
  double ANGF[MXANGL];
} CARD3C1_struct;

typedef struct
{
  double WLF[MXWVLN];
} CARD3C2_struct;

typedef struct
{
  double F1[MXANGL*MXWVLN];
} CARD3C3_struct;

typedef struct
{
  double F2[MXANGL*MXWVLN];
} CARD3C4_struct;

typedef struct
{
  double F3[MXANGL*MXWVLN];
} CARD3C5_struct;

typedef struct
{
  double F4[MXANGL*MXWVLN];
} CARD3C6_struct;

typedef struct
{
  double V1;
  double V2;
  double DV;
  double FWHM;
  char YFLAG[2];
  char XFLAG[2];
  char DLIMIT[9];
  char FLAGS[8];
  int MLFLX;
} CARD4_struct;

typedef struct
{
  int NSURF;
  double AATEMP;
} CARD4A_struct;

typedef struct
{
  char CBRDF[81];
} CARD4B1_struct;

typedef struct
{
  int NWVSRF;
  double SURFZN;
  double SURFAZ;
} CARD4B2_struct;

typedef struct
{
  double WVSURF;
  double PARAMS[MPARAM];
} CARD4B3_struct;

typedef struct
{
  char SALBFL[81];
} CARD4L1_struct;

typedef struct
{
  char CSALB[81];
} CARD4L2_struct;

typedef struct
{
  int IRPT;
} CARD5_struct;

typedef struct
{
  CARD1_struct    *card1;
  CARD1A_struct   *card1a;
  CARD1A1_struct  *card1a1;
  CARD1A2_struct  *card1a2;
  CARD1A3_struct  *card1a3;
  CARD2_struct    *card2;
  CARD2AP_struct  *card2ap;
  CARD2A_struct   *card2a;
  CARD2B_struct   *card2b;
  CARD2C_struct   *card2c;
  CARD2C1_struct  *card2c1;
  CARD2C2_struct  *card2c2;
  CARD2C2X_struct *card2c2x;
  CARD2C3_struct  *card2c3;
  CARD2D_struct   *card2d;
  CARD2D1_struct  *card2d1;
  CARD2D2_struct  *card2d2;
  CARD2E1_struct  *card2e1;
  CARD2E2_struct  *card2e2;
  CARD3_struct    *card3;
  CARD3A1_struct  *card3a1;
  CARD3A2_struct  *card3a2;
  CARD3B1_struct  *card3b1;
  CARD3B2_struct  *card3b2;
  CARD3C1_struct  *card3c1;
  CARD3C2_struct  *card3c2;
  CARD3C3_struct  *card3c3;
  CARD3C4_struct  *card3c4;
  CARD3C5_struct  *card3c5;
  CARD3C6_struct  *card3c6;
  CARD4_struct    *card4;
  CARD4A_struct   *card4a;
  CARD4B1_struct  *card4b1;
  CARD4B2_struct  *card4b2;
  CARD4B3_struct  *card4b3;
  CARD4L1_struct  *card4l1;
  CARD4L2_struct  *card4l2;
  CARD5_struct    *card5;
} TAPE5_struct;

// CARD 1 functions
void CARD1_write(const TAPE5_struct *t,FILE *fp);
void CARD1_print(const TAPE5_struct *t,FILE *fp);
int CARD1_read(TAPE5_struct *t,FILE *fp);
int CARD1_gets(char *line,TAPE5_struct *t);
int CARD1_check_all(const TAPE5_struct *t);
int CARD1_check(const CARD1_struct *c,int n);
char *CARD1_to_s(const CARD1_struct *c,int n,char *s);

// CARD 1A functions
void CARD1A_write(const TAPE5_struct *t,FILE *fp);
void CARD1A_print(const TAPE5_struct *t,FILE *fp);
int CARD1A_read(TAPE5_struct *t,FILE *fp);
int CARD1A_gets(char *line,TAPE5_struct *t);
int CARD1A_check_all(const TAPE5_struct *t);
int CARD1A_check(const CARD1A_struct *c,int n);
char *CARD1A_to_s(const CARD1A_struct *c,int n,char *s);

// CARD 1A1 functions
void CARD1A1_write(const TAPE5_struct *t,FILE *fp);
void CARD1A1_print(const TAPE5_struct *t,FILE *fp);
int CARD1A1_read(TAPE5_struct *t,FILE *fp);
int CARD1A1_gets(char *line,TAPE5_struct *t);
int CARD1A1_check_all(const TAPE5_struct *t);
int CARD1A1_check(const CARD1A1_struct *c,int n);
char *CARD1A1_to_s(const CARD1A1_struct *c,int n,char *s);

// CARD 1A2 functions
void CARD1A2_write(const TAPE5_struct *t,FILE *fp);
void CARD1A2_print(const TAPE5_struct *t,FILE *fp);
int CARD1A2_read(TAPE5_struct *t,FILE *fp);
int CARD1A2_gets(char *line,TAPE5_struct *t);
int CARD1A2_check_all(const TAPE5_struct *t);
int CARD1A2_check(const CARD1A2_struct *c,int n);
char *CARD1A2_to_s(const CARD1A2_struct *c,int n,char *s);

// CARD 1A3 functions
void CARD1A3_write(const TAPE5_struct *t,FILE *fp);
void CARD1A3_print(const TAPE5_struct *t,FILE *fp);
int CARD1A3_read(TAPE5_struct *t,FILE *fp);
int CARD1A3_gets(char *line,TAPE5_struct *t);
int CARD1A3_check_all(const TAPE5_struct *t);
int CARD1A3_check(const CARD1A3_struct *c,int n);
char *CARD1A3_to_s(const CARD1A3_struct *c,int n,char *s);

// CARD 2AP functions
void CARD2AP_write(const TAPE5_struct *t,FILE *fp);
void CARD2AP_print(const TAPE5_struct *t,FILE *fp);
int CARD2AP_read(TAPE5_struct *t,FILE *fp);
int CARD2AP_gets(char *line,TAPE5_struct *t);
int CARD2AP_check_all(const TAPE5_struct *t);
int CARD2AP_check(const CARD2AP_struct *c,int n);
char *CARD2AP_to_s(const CARD2AP_struct *c,int n,char *s);

// CARD 2 functions
void CARD2_write(const TAPE5_struct *t,FILE *fp);
void CARD2_print(const TAPE5_struct *t,FILE *fp);
int CARD2_read(TAPE5_struct *t,FILE *fp);
int CARD2_gets(char *line,TAPE5_struct *t);
int CARD2_check_all(const TAPE5_struct *t);
int CARD2_check(const CARD2_struct *c,int n);
char *CARD2_to_s(const CARD2_struct *c,int n,char *s);

// CARD 2A functions
void CARD2A_write(const TAPE5_struct *t,FILE *fp);
void CARD2A_print(const TAPE5_struct *t,FILE *fp);
int CARD2A_read(TAPE5_struct *t,FILE *fp);
int CARD2A_gets(char *line,TAPE5_struct *t);
int CARD2A_check_all(const TAPE5_struct *t);
int CARD2A_check(const CARD2A_struct *c,int n);
char *CARD2A_to_s(const CARD2A_struct *c,int n,char *s);

// CARD 2B functions
void CARD2B_write(const TAPE5_struct *t,FILE *fp);
void CARD2B_print(const TAPE5_struct *t,FILE *fp);
int CARD2B_read(TAPE5_struct *t,FILE *fp);
int CARD2B_gets(char *line,TAPE5_struct *t);
int CARD2B_check_all(const TAPE5_struct *t);
int CARD2B_check(const CARD2B_struct *c,int n);
char *CARD2B_to_s(const CARD2B_struct *c,int n,char *s);

// CARD 2C functions
void CARD2C_write(const TAPE5_struct *t,FILE *fp);
void CARD2C_print(const TAPE5_struct *t,FILE *fp);
int CARD2C_read(TAPE5_struct *t,FILE *fp);
int CARD2C_gets(char *line,TAPE5_struct *t);
int CARD2C_check_all(const TAPE5_struct *t);
int CARD2C_check(const CARD2C_struct *c,int n);
char *CARD2C_to_s(const CARD2C_struct *c,int n,char *s);

// CARD 2C1 functions
void CARD2C1_write(const CARD2C1_struct *c,FILE *fp);
void CARD2C1_print(const CARD2C1_struct *c,int i,FILE *fp);
int CARD2C1_read(CARD2C1_struct *c,FILE *fp);
int CARD2C1_gets(char *line,TAPE5_struct *t);
int CARD2C1_check_all(const TAPE5_struct *t);
int CARD2C1_check(const CARD2C1_struct *c,int n);
char *CARD2C1_to_s(const CARD2C1_struct *c,int n,char *s);

// CARD 2C2 functions
void CARD2C2_write(const CARD2C2_struct *c,FILE *fp);
void CARD2C2_print(const CARD2C2_struct *c,int i,FILE *fp);
int CARD2C2_read(CARD2C2_struct *c,FILE *fp);
int CARD2C2_gets(char *line,TAPE5_struct *t);
int CARD2C2_check_all(const TAPE5_struct *t);
int CARD2C2_check(const CARD2C2_struct *c,int n);
char *CARD2C2_to_s(const CARD2C2_struct *c,int n,char *s);

// CARD 2C2X functions
void CARD2C2X_write(const CARD2C2X_struct *c,FILE *fp);
void CARD2C2X_print(const CARD2C2X_struct *c,int i,FILE *fp);
int CARD2C2X_read(CARD2C2X_struct *c,FILE *fp);
int CARD2C2X_gets(char *line,TAPE5_struct *t);
int CARD2C2X_check_all(const TAPE5_struct *t);
int CARD2C2X_check(const CARD2C2X_struct *c,int n);
char *CARD2C2X_to_s(const CARD2C2X_struct *c,int n,char *s);

// CARD 2C3 functions
void CARD2C3_write(const TAPE5_struct *t,int i,FILE *fp);
void CARD2C3_print(const TAPE5_struct *t,int i,FILE *fp);
int CARD2C3_read(TAPE5_struct *t,int i,FILE *fp);
int CARD2C3_gets(char *line,TAPE5_struct *t);
int CARD2C3_check_all(const TAPE5_struct *t);
int CARD2C3_check(const CARD2C3_struct *c,int n);
char *CARD2C3_to_s(const CARD2C3_struct *c,int n,char *s);

// CARD 2D functions
void CARD2D_write(const TAPE5_struct *t,FILE *fp);
void CARD2D_print(const TAPE5_struct *t,FILE *fp);
int CARD2D_read(TAPE5_struct *t,FILE *fp);
int CARD2D_gets(char *line,TAPE5_struct *t);
int CARD2D_check_all(const TAPE5_struct *t);
int CARD2D_check(const CARD2D_struct *c,int n);
char *CARD2D_to_s(const CARD2D_struct *c,int n,char *s);

// CARD 2D1 functions
void CARD2D1_write(const CARD2D1_struct *c,FILE *fp);
void CARD2D1_print(const CARD2D1_struct *c,int i,FILE *fp);
int CARD2D1_read(CARD2D1_struct *c,FILE *fp);
int CARD2D1_gets(char *line,TAPE5_struct *t);
int CARD2D1_check_all(const TAPE5_struct *t);
int CARD2D1_check(const CARD2D1_struct *c,int n);
char *CARD2D1_to_s(const CARD2D1_struct *c,int n,char *s);

// CARD 2D2 functions
void CARD2D2_write(const CARD2D2_struct *c,FILE *fp);
void CARD2D2_print(const CARD2D2_struct *c,int i,int j,FILE *fp);
int CARD2D2_read(CARD2D2_struct *c,FILE *fp);
int CARD2D2_gets(char *line,TAPE5_struct *t);
int CARD2D2_check_all(const TAPE5_struct *t);
int CARD2D2_check(const CARD2D2_struct *c,int n);
char *CARD2D2_to_s(const CARD2D2_struct *c,int n,char *s);

// CARD 2E1 functions
void CARD2E1_write(const TAPE5_struct *t,FILE *fp);
void CARD2E1_print(const TAPE5_struct *t,FILE *fp);
int CARD2E1_read(TAPE5_struct *t,FILE *fp);
int CARD2E1_gets(char *line,TAPE5_struct *t);
int CARD2E1_check_all(const TAPE5_struct *t);
int CARD2E1_check(const CARD2E1_struct *c,int n);
char *CARD2E1_to_s(const CARD2E1_struct *c,int n,char *s);

// CARD 2E2 functions
void CARD2E2_write(const TAPE5_struct *t,FILE *fp);
void CARD2E2_print(const TAPE5_struct *t,FILE *fp);
int CARD2E2_read(TAPE5_struct *t,FILE *fp);
int CARD2E2_gets(char *line,TAPE5_struct *t);
int CARD2E2_check_all(const TAPE5_struct *t);
int CARD2E2_check(const CARD2E2_struct *c,int n);
char *CARD2E2_to_s(const CARD2E2_struct *c,int n,char *s);

// CARD 3 functions
void CARD3_write(const TAPE5_struct *t,FILE *fp);
void CARD3_print(const TAPE5_struct *t,FILE *fp);
int CARD3_read(TAPE5_struct *t,FILE *fp);
int CARD3_gets(char *line,TAPE5_struct *t);
int CARD3_check_all(const TAPE5_struct *t);
int CARD3_check(const CARD3_struct *c,int n);
char *CARD3_to_s(const CARD3_struct *c,int n,char *s);

// CARD 3A1 functions
void CARD3A1_write(const TAPE5_struct *t,FILE *fp);
void CARD3A1_print(const TAPE5_struct *t,FILE *fp);
int CARD3A1_read(TAPE5_struct *t,FILE *fp);
int CARD3A1_gets(char *line,TAPE5_struct *t);
int CARD3A1_check_all(const TAPE5_struct *t);
int CARD3A1_check(const CARD3A1_struct *c,int n);
char *CARD3A1_to_s(const CARD3A1_struct *c,int n,char *s);

// CARD 3A2 functions
void CARD3A2_write(const TAPE5_struct *t,FILE *fp);
void CARD3A2_print(const TAPE5_struct *t,FILE *fp);
int CARD3A2_read(TAPE5_struct *t,FILE *fp);
int CARD3A2_gets(char *line,TAPE5_struct *t);
int CARD3A2_check_all(const TAPE5_struct *t);
int CARD3A2_check(const CARD3A2_struct *c,int n);
char *CARD3A2_to_s(const CARD3A2_struct *c,int n,char *s);

// CARD 3B1 functions
void CARD3B1_write(const TAPE5_struct *t,FILE *fp);
void CARD3B1_print(const TAPE5_struct *t,FILE *fp);
int CARD3B1_read(TAPE5_struct *t,FILE *fp);
int CARD3B1_gets(char *line,TAPE5_struct *t);
int CARD3B1_check_all(const TAPE5_struct *t);
int CARD3B1_check(const CARD3B1_struct *c,int n);
char *CARD3B1_to_s(const CARD3B1_struct *c,int n,char *s);

// CARD 3B2 functions
void CARD3B2_write(const TAPE5_struct *t,FILE *fp);
void CARD3B2_print(const TAPE5_struct *t,FILE *fp);
int CARD3B2_read(TAPE5_struct *t,FILE *fp);
int CARD3B2_gets(char *line,TAPE5_struct *t);
int CARD3B2_check_all(const TAPE5_struct *t);
int CARD3B2_check(const CARD3B2_struct *c,int n);
char *CARD3B2_to_s(const CARD3B2_struct *c,int n,char *s);

// CARD 3C1 functions
void CARD3C1_write(const TAPE5_struct *t,FILE *fp);
void CARD3C1_print(const TAPE5_struct *t,FILE *fp);
int CARD3C1_read(TAPE5_struct *t,FILE *fp);
int CARD3C1_gets(char *line,TAPE5_struct *t);
int CARD3C1_check_all(const TAPE5_struct *t);
int CARD3C1_check(const CARD3C1_struct *c,int n);
char *CARD3C1_to_s(const CARD3C1_struct *c,int n,char *s);

// CARD 3C2 functions
void CARD3C2_write(const TAPE5_struct *t,FILE *fp);
void CARD3C2_print(const TAPE5_struct *t,FILE *fp);
int CARD3C2_read(TAPE5_struct *t,FILE *fp);
int CARD3C2_gets(char *line,TAPE5_struct *t);
int CARD3C2_check_all(const TAPE5_struct *t);
int CARD3C2_check(const CARD3C2_struct *c,int n);
char *CARD3C2_to_s(const CARD3C2_struct *c,int n,char *s);

// CARD 3C3 functions
void CARD3C3_write(const TAPE5_struct *t,FILE *fp);
void CARD3C3_print(const TAPE5_struct *t,FILE *fp);
int CARD3C3_read(TAPE5_struct *t,FILE *fp);
int CARD3C3_gets(char *line,TAPE5_struct *t);
int CARD3C3_check_all(const TAPE5_struct *t);
int CARD3C3_check(const CARD3C3_struct *c,int n);
char *CARD3C3_to_s(const CARD3C3_struct *c,int n,char *s);

// CARD 3C4 functions
void CARD3C4_write(const TAPE5_struct *t,FILE *fp);
void CARD3C4_print(const TAPE5_struct *t,FILE *fp);
int CARD3C4_read(TAPE5_struct *t,FILE *fp);
int CARD3C4_gets(char *line,TAPE5_struct *t);
int CARD3C4_check_all(const TAPE5_struct *t);
int CARD3C4_check(const CARD3C4_struct *c,int n);
char *CARD3C4_to_s(const CARD3C4_struct *c,int n,char *s);

// CARD 3C5 functions
void CARD3C5_write(const TAPE5_struct *t,FILE *fp);
void CARD3C5_print(const TAPE5_struct *t,FILE *fp);
int CARD3C5_read(TAPE5_struct *t,FILE *fp);
int CARD3C5_gets(char *line,TAPE5_struct *t);
int CARD3C5_check_all(const TAPE5_struct *t);
int CARD3C5_check(const CARD3C5_struct *c,int n);
char *CARD3C5_to_s(const CARD3C5_struct *c,int n,char *s);

// CARD 3C6 functions
void CARD3C6_write(const TAPE5_struct *t,FILE *fp);
void CARD3C6_print(const TAPE5_struct *t,FILE *fp);
int CARD3C6_read(TAPE5_struct *t,FILE *fp);
int CARD3C6_gets(char *line,TAPE5_struct *t);
int CARD3C6_check_all(const TAPE5_struct *t);
int CARD3C6_check(const CARD3C6_struct *c,int n);
char *CARD3C6_to_s(const CARD3C6_struct *c,int n,char *s);

// CARD 4 functions
void CARD4_write(const TAPE5_struct *t,FILE *fp);
void CARD4_print(const TAPE5_struct *t,FILE *fp);
int CARD4_read(TAPE5_struct *t,FILE *fp);
int CARD4_gets(char *line,TAPE5_struct *t);
int CARD4_check_all(const TAPE5_struct *t);
int CARD4_check(const CARD4_struct *c,int n);
char *CARD4_to_s(const CARD4_struct *c,int n,char *s);

// CARD 4A functions
void CARD4A_write(const TAPE5_struct *t,FILE *fp);
void CARD4A_print(const TAPE5_struct *t,FILE *fp);
int CARD4A_read(TAPE5_struct *t,FILE *fp);
int CARD4A_gets(char *line,TAPE5_struct *t);
int CARD4A_check_all(const TAPE5_struct *t);
int CARD4A_check(const CARD4A_struct *c,int n);
char *CARD4A_to_s(const CARD4A_struct *c,int n,char *s);

// CARD 4B1 functions
void CARD4B1_write(const CARD4B1_struct *c,FILE *fp);
void CARD4B1_print(const CARD4B1_struct *c,int i,FILE *fp);
int CARD4B1_read(CARD4B1_struct *c,FILE *fp);
int CARD4B1_gets(char *line,TAPE5_struct *t);
int CARD4B1_check_all(const TAPE5_struct *t);
int CARD4B1_check(const CARD4B1_struct *c,int n);
char *CARD4B1_to_s(const CARD4B1_struct *c,int n,char *s);

// CARD 4B2 functions
void CARD4B2_write(const CARD4B2_struct *c,FILE *fp);
void CARD4B2_print(const CARD4B2_struct *c,int i,FILE *fp);
int CARD4B2_read(CARD4B2_struct *c,FILE *fp);
int CARD4B2_gets(char *line,TAPE5_struct *t);
int CARD4B2_check_all(const TAPE5_struct *t);
int CARD4B2_check(const CARD4B2_struct *c,int n);
char *CARD4B2_to_s(const CARD4B2_struct *c,int n,char *s);

// CARD 4B3 functions
void CARD4B3_write(const CARD4B3_struct *c,int kmx,FILE *fp);
void CARD4B3_print(const CARD4B3_struct *c,int i,int j,int kmx,FILE *fp);
int CARD4B3_read(CARD4B3_struct *c,int kmx,FILE *fp);
int CARD4B3_gets(char *line,TAPE5_struct *t);
int CARD4B3_check_all(const TAPE5_struct *t);
int CARD4B3_check(const CARD4B3_struct *c,int n);
char *CARD4B3_to_s(const CARD4B3_struct *c,int n,char *s);

// CARD 4L1 functions
void CARD4L1_write(const TAPE5_struct *t,FILE *fp);
void CARD4L1_print(const TAPE5_struct *t,FILE *fp);
int CARD4L1_read(TAPE5_struct *t,FILE *fp);
int CARD4L1_gets(char *line,TAPE5_struct *t);
int CARD4L1_check_all(const TAPE5_struct *t);
int CARD4L1_check(const CARD4L1_struct *c,int n);
char *CARD4L1_to_s(const CARD4L1_struct *c,int n,char *s);

// CARD 4L2 functions
void CARD4L2_write(const CARD4L2_struct *c,FILE *fp);
void CARD4L2_print(const CARD4L2_struct *c,int i,FILE *fp);
int CARD4L2_read(CARD4L2_struct *c,FILE *fp);
int CARD4L2_gets(char *line,TAPE5_struct *t);
int CARD4L2_check_all(const TAPE5_struct *t);
int CARD4L2_check(const CARD4L2_struct *c,int n);
char *CARD4L2_to_s(const CARD4L2_struct *c,int n,char *s);

// CARD 5 functions
void CARD5_write(const TAPE5_struct *t,FILE *fp);
void CARD5_print(const TAPE5_struct *t,FILE *fp);
int CARD5_read(TAPE5_struct *t,FILE *fp);
int CARD5_gets(char *line,TAPE5_struct *t);
int CARD5_check_all(const TAPE5_struct *t);
int CARD5_check(const CARD5_struct *c,int n);
char *CARD5_to_s(const CARD5_struct *c,int n,char *s);

// CARDs functions
void CARD1s_write(const TAPE5_struct *t);
void CARD1s_print(const TAPE5_struct *t);
int CARD1s_read(TAPE5_struct *t,FILE *fp);
void CARD2s_write(const TAPE5_struct *t);
void CARD2s_print(const TAPE5_struct *t);
int CARD2s_read(TAPE5_struct *t,FILE *fp);
void CARD3s_write(const TAPE5_struct *t);
void CARD3s_print(const TAPE5_struct *t);
int CARD3s_read(TAPE5_struct *t,FILE *fp);
void CARD4s_write(const TAPE5_struct *t);
void CARD4s_print(const TAPE5_struct *t);
int CARD4s_read(TAPE5_struct *t,FILE *fp);
void CARD5s_write(const TAPE5_struct *t);
void CARD5s_print(const TAPE5_struct *t);
int CARD5s_read(TAPE5_struct *t,FILE *fp);
