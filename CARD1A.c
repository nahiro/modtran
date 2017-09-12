// CARD 1A parameters
char	CARD1A_ID[5]			= "1A";

char	CARD1A_comment[MAXLINE]		= "required";

char	CARD1A_name[CARD1A_NPAR][10]	=
{
  "DIS",
  "DISAZM",
  "DISALB",
  "NSTR",
  "LSUN",
  "ISUN",
  "SFWHM",
  "CO2MX",
  "H2OSTR",
  "O3STR",
  "C_PROF",
  "LSUNFL",
  "LBMNAM",
  "LFLTNM",
  "H2OAER",
  "CDTDIR",
  "SOLCON",
  "CDASTM",
  "ASTMC",
  "ASTMX",
  "ASTMO",
  "AERRH",
  "NSSALB",
};

CARD1A_struct CARD1A_default		=
{
  "T",
  "T",
  "F",
  16,
  "F",
  0,
  0.0,
  365.0,
  "0",
  "0",
  0,
  "F",
  "F",
  "F",
  "T",
  "F",
  1.0,
  "T",
  0.0,
  0.0,
  0.0,
  0.0,
  0,
};

char CARD1A_description[CARD1A_NPAR][MAXLINE] =
{
  "DISORT multiple scattering algorighm",
  "azimuth dependence flag",
  "spherical albedo flag"
  "number of streams",
  "solar irradiance from a file",
  "FWHM of the triangular scanning function",
  "FWHM of the triangular scanning function",
  "CO2 mixing ratio (ppmv)",
  "vertical water vapor column (g/cm2)",
  "vertical ozone column (g/cm2)",
  "scale default profiles",
  "solar irradiance file name from CARD 1A1",
  "band model file name from CARD 1A2",
  "instrument filter file name from CARD 1A3",
  "reflect the changes of RH profile",
  "directory name from CARD 1A4"
  "scaling of the solar irradiance",
  "perturb both the BL/tropospheric aerosol",
  "Angstrom coefficient",
  "Angstrom offset, used with perturbation",
  "Angstrom offset, not used with perturbation",
  "relative humidity",
  "number of SSA spectral grid points",
};

void CARD1A_write(const TAPE5_struct *t,FILE *fp)
{
  switch(mod_v)
  {
    case MOD_V4:
      #ifdef OLDMODE
      fprintf(fp,"%1s%1s%3d%1s%4d%10.5f%10s%10s %1s %1s %1s %1s  %10.3f\n",
                  t->card1a->DIS,t->card1a->DISAZM,t->card1a->NSTR,t->card1a->LSUN,t->card1a->ISUN,t->card1a->CO2MX,
                  t->card1a->H2OSTR,t->card1a->O3STR,t->card1a->LSUNFL,t->card1a->LBMNAM,t->card1a->LFLTNM,
                  t->card1a->H2OAER,t->card1a->SOLCON);
      #else
      fprintf(fp,"%1s%1s%3d%1s%4d%15.8f%15s%15s %1s %1s %1s %1s  %10.3f\n",
                  t->card1a->DIS,t->card1a->DISAZM,t->card1a->NSTR,t->card1a->LSUN,t->card1a->ISUN,t->card1a->CO2MX,
                  t->card1a->H2OSTR,t->card1a->O3STR,t->card1a->LSUNFL,t->card1a->LBMNAM,t->card1a->LFLTNM,
                  t->card1a->H2OAER,t->card1a->SOLCON);
      #endif
      break;
    case MOD_V5:
      #ifdef OLDMODE
      fprintf(fp,"%1s%1s%1s%3d%4.1f%10.5f%10s%10s%1d%1s %1s %1s %1s %1s%10.5f%1s%9.2f%10.3f%10.3f%10.3f%10d\n",
                  t->card1a->DIS,t->card1a->DISAZM,t->card1a->DISALB,t->card1a->NSTR,t->card1a->SFWHM,t->card1a->CO2MX,
                  t->card1a->H2OSTR,t->card1a->O3STR,t->card1a->C_PROF,t->card1a->LSUNFL,t->card1a->LBMNAM,t->card1a->LFLTNM,
                  t->card1a->H2OAER,t->card1a->CDTDIR,t->card1a->SOLCON,
                  t->card1a->CDASTM,t->card1a->ASTMC,t->card1a->ASTMX,t->card1a->ASTMO,t->card1a->AERRH,t->card1a->NSSALB);
      #else
      fprintf(fp,"%1s%1s%1s%3d%4.1f%15.8f%15s%15s%1d%1s %1s %1s %1s %1s%10.5f%1s%9.2f%10.3f%10.3f%10.3f%10d\n",
                  t->card1a->DIS,t->card1a->DISAZM,t->card1a->DISALB,t->card1a->NSTR,t->card1a->SFWHM,t->card1a->CO2MX,
                  t->card1a->H2OSTR,t->card1a->O3STR,t->card1a->C_PROF,t->card1a->LSUNFL,t->card1a->LBMNAM,t->card1a->LFLTNM,
                  t->card1a->H2OAER,t->card1a->CDTDIR,t->card1a->SOLCON,
                  t->card1a->CDASTM,t->card1a->ASTMC,t->card1a->ASTMX,t->card1a->ASTMO,t->card1a->AERRH,t->card1a->NSSALB);
      #endif
      break;
  }
}

void CARD1A_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  switch(mod_v)
  {
    case MOD_V4:
      for(n=0; n<CARD1A_NPAR; n++)
      {
        switch(n)
        {
          case I_CARD1A_DISALB:
          case I_CARD1A_SFWHM:
          case I_CARD1A_C_PROF:
          case I_CARD1A_CDTDIR:
          case I_CARD1A_CDASTM:
          case I_CARD1A_ASTMC:
          case I_CARD1A_ASTMX:
          case I_CARD1A_ASTMO:
          case I_CARD1A_AERRH:
          case I_CARD1A_NSSALB:
            break;
          default:
            fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD1A_ID,CARD1A_name[n],CARD1A_to_s(t->card1a,n,s),CARD1A_description[n]);
            break;
        }
      }
      break;
    case MOD_V5:
      for(n=0; n<CARD1A_NPAR; n++)
      {
        switch(n)
        {
          case I_CARD1A_LSUN:
          case I_CARD1A_ISUN:
            break;
          default:
            fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD1A_ID,CARD1A_name[n],CARD1A_to_s(t->card1a,n,s),CARD1A_description[n]);
            break;
        }
      }
      break;
  }
}

int CARD1A_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD1A_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  switch(mod_v)
  {
    case MOD_V4:
      #ifdef OLDMODE
      n = 0;
      if(get_upp(line,n,1,t->card1a->DIS)      < 0) rt = I_CARD1A_DIS;    n += 1;
      if(get_upp(line,n,1,t->card1a->DISAZM)   < 0) rt = I_CARD1A_DISAZM; n += 1;
      if(get_int(line,n,3,&t->card1a->NSTR)    < 0) rt = I_CARD1A_NSTR;   n += 3;
      if(get_upp(line,n,1,t->card1a->LSUN)     < 0) rt = I_CARD1A_LSUN;   n += 1;
      if(get_int(line,n,4,&t->card1a->ISUN)    < 0) rt = I_CARD1A_ISUN;   n += 4;
      if(get_dbl(line,n,10,&t->card1a->CO2MX)  < 0) rt = I_CARD1A_CO2MX;  n += 10;
      if(get_upp(line,n,10,t->card1a->H2OSTR)  < 0) rt = I_CARD1A_H2OSTR; n += 10;
      if(get_upp(line,n,10,t->card1a->O3STR)   < 0) rt = I_CARD1A_O3STR;  n += 11;
      if(get_upp(line,n,1,t->card1a->LSUNFL)   < 0) rt = I_CARD1A_LSUNFL; n += 2;
      if(get_upp(line,n,1,t->card1a->LBMNAM)   < 0) rt = I_CARD1A_LBMNAM; n += 2;
      if(get_upp(line,n,1,t->card1a->LFLTNM)   < 0) rt = I_CARD1A_LFLTNM; n += 2;
      if(get_upp(line,n,1,t->card1a->H2OAER)   < 0) rt = I_CARD1A_H2OAER; n += 3;
      if(get_dbl(line,n,10,&t->card1a->SOLCON) < 0) rt = I_CARD1A_SOLCON;
      #else
      n = 0;
      if(get_upp(line,n,1,t->card1a->DIS)      < 0) rt = I_CARD1A_DIS;    n += 1;
      if(get_upp(line,n,1,t->card1a->DISAZM)   < 0) rt = I_CARD1A_DISAZM; n += 1;
      if(get_int(line,n,3,&t->card1a->NSTR)    < 0) rt = I_CARD1A_NSTR;   n += 3;
      if(get_upp(line,n,1,t->card1a->LSUN)     < 0) rt = I_CARD1A_LSUN;   n += 1;
      if(get_int(line,n,4,&t->card1a->ISUN)    < 0) rt = I_CARD1A_ISUN;   n += 4;
      if(get_dbl(line,n,15,&t->card1a->CO2MX)  < 0) rt = I_CARD1A_CO2MX;  n += 15;
      if(get_upp(line,n,15,t->card1a->H2OSTR)  < 0) rt = I_CARD1A_H2OSTR; n += 15;
      if(get_upp(line,n,15,t->card1a->O3STR)   < 0) rt = I_CARD1A_O3STR;  n += 16;
      if(get_upp(line,n,1,t->card1a->LSUNFL)   < 0) rt = I_CARD1A_LSUNFL; n += 2;
      if(get_upp(line,n,1,t->card1a->LBMNAM)   < 0) rt = I_CARD1A_LBMNAM; n += 2;
      if(get_upp(line,n,1,t->card1a->LFLTNM)   < 0) rt = I_CARD1A_LFLTNM; n += 2;
      if(get_upp(line,n,1,t->card1a->H2OAER)   < 0) rt = I_CARD1A_H2OAER; n += 3;
      if(get_dbl(line,n,10,&t->card1a->SOLCON) < 0) rt = I_CARD1A_SOLCON;
      #endif
      break;
    case MOD_V5:
      #ifdef OLDMODE
      n = 0;
      if(get_upp(line,n,1,t->card1a->DIS)      < 0) rt = I_CARD1A_DIS;    n += 1;
      if(get_upp(line,n,1,t->card1a->DISAZM)   < 0) rt = I_CARD1A_DISAZM; n += 1;
      if(get_upp(line,n,1,t->card1a->DISALB)   < 0) rt = I_CARD1A_DISALB; n += 1;
      if(get_int(line,n,3,&t->card1a->NSTR)    < 0) rt = I_CARD1A_NSTR;   n += 3;
      if(get_dbl(line,n,4,&t->card1a->SFWHM)   < 0) rt = I_CARD1A_SFWHM;  n += 4;
      if(get_dbl(line,n,10,&t->card1a->CO2MX)  < 0) rt = I_CARD1A_CO2MX;  n += 10;
      if(get_upp(line,n,10,t->card1a->H2OSTR)  < 0) rt = I_CARD1A_H2OSTR; n += 10;
      if(get_upp(line,n,10,t->card1a->O3STR)   < 0) rt = I_CARD1A_O3STR;  n += 10;
      if(get_int(line,n,1,&t->card1a->C_PROF)  < 0) rt = I_CARD1A_C_PROF; n += 1;
      if(get_upp(line,n,1,t->card1a->LSUNFL)   < 0) rt = I_CARD1A_LSUNFL; n += 2;
      if(get_upp(line,n,1,t->card1a->LBMNAM)   < 0) rt = I_CARD1A_LBMNAM; n += 2;
      if(get_upp(line,n,1,t->card1a->LFLTNM)   < 0) rt = I_CARD1A_LFLTNM; n += 2;
      if(get_upp(line,n,1,t->card1a->H2OAER)   < 0) rt = I_CARD1A_H2OAER; n += 2;
      if(get_upp(line,n,1,t->card1a->CDTDIR)   < 0) rt = I_CARD1A_CDTDIR; n += 1;
      if(get_dbl(line,n,10,&t->card1a->SOLCON) < 0) rt = I_CARD1A_SOLCON; n += 10;
      if(get_upp(line,n,1,t->card1a->CDASTM)   < 0) rt = I_CARD1A_CDASTM; n += 1;
      if(get_dbl(line,n,9,&t->card1a->ASTMC)   < 0) rt = I_CARD1A_ASTMC;  n += 9;
      if(get_dbl(line,n,10,&t->card1a->ASTMX)  < 0) rt = I_CARD1A_ASTMX;  n += 10;
      if(get_dbl(line,n,10,&t->card1a->ASTMO)  < 0) rt = I_CARD1A_ASTMO;  n += 10;
      if(get_dbl(line,n,10,&t->card1a->AERRH)  < 0) rt = I_CARD1A_AERRH;  n += 10;
      if(get_int(line,n,10,&t->card1a->NSSALB) < 0) rt = I_CARD1A_NSSALB;
      #else
      n = 0;
      if(get_upp(line,n,1,t->card1a->DIS)      < 0) rt = I_CARD1A_DIS;    n += 1;
      if(get_upp(line,n,1,t->card1a->DISAZM)   < 0) rt = I_CARD1A_DISAZM; n += 1;
      if(get_upp(line,n,1,t->card1a->DISALB)   < 0) rt = I_CARD1A_DISALB; n += 1;
      if(get_int(line,n,3,&t->card1a->NSTR)    < 0) rt = I_CARD1A_NSTR;   n += 3;
      if(get_dbl(line,n,4,&t->card1a->SFWHM)   < 0) rt = I_CARD1A_SFWHM;  n += 4;
      if(get_dbl(line,n,15,&t->card1a->CO2MX)  < 0) rt = I_CARD1A_CO2MX;  n += 15;
      if(get_upp(line,n,15,t->card1a->H2OSTR)  < 0) rt = I_CARD1A_H2OSTR; n += 15;
      if(get_upp(line,n,15,t->card1a->O3STR)   < 0) rt = I_CARD1A_O3STR;  n += 15;
      if(get_int(line,n,1,&t->card1a->C_PROF)  < 0) rt = I_CARD1A_C_PROF; n += 1;
      if(get_upp(line,n,1,t->card1a->LSUNFL)   < 0) rt = I_CARD1A_LSUNFL; n += 2;
      if(get_upp(line,n,1,t->card1a->LBMNAM)   < 0) rt = I_CARD1A_LBMNAM; n += 2;
      if(get_upp(line,n,1,t->card1a->LFLTNM)   < 0) rt = I_CARD1A_LFLTNM; n += 2;
      if(get_upp(line,n,1,t->card1a->H2OAER)   < 0) rt = I_CARD1A_H2OAER; n += 2;
      if(get_upp(line,n,1,t->card1a->CDTDIR)   < 0) rt = I_CARD1A_CDTDIR; n += 1;
      if(get_dbl(line,n,10,&t->card1a->SOLCON) < 0) rt = I_CARD1A_SOLCON; n += 10;
      if(get_upp(line,n,1,t->card1a->CDASTM)   < 0) rt = I_CARD1A_CDASTM; n += 1;
      if(get_dbl(line,n,9,&t->card1a->ASTMC)   < 0) rt = I_CARD1A_ASTMC;  n += 9;
      if(get_dbl(line,n,10,&t->card1a->ASTMX)  < 0) rt = I_CARD1A_ASTMX;  n += 10;
      if(get_dbl(line,n,10,&t->card1a->ASTMO)  < 0) rt = I_CARD1A_ASTMO;  n += 10;
      if(get_dbl(line,n,10,&t->card1a->AERRH)  < 0) rt = I_CARD1A_AERRH;  n += 10;
      if(get_int(line,n,10,&t->card1a->NSSALB) < 0) rt = I_CARD1A_NSSALB;
      #endif
      break;
  }
  if(rt)
  {
    fprintf(stderr,"CARD1A_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD1A_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD1A_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD1A_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int ntmp = 0;
  double xtmp = NAN;
  char str[NCOL3][MAXLINE];
  char *p;

  // read line
  for(i=0,p=line; i<NCOL3; i++,p+=n)
  {
    if(sscanf(p,"%s%n",str[i],&n) == EOF) break;
  }
  if(i < 2)
  {
    return -1;
  }
  if(strcmp(CARD1A_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD1A_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD1A_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // convert value if required
  switch(n)
  {
    case I_CARD1A_NSTR:
    case I_CARD1A_ISUN:
      errno = 0;
      ntmp = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case I_CARD1A_CO2MX:
    case I_CARD1A_SOLCON:
      errno = 0;
      xtmp = strtod(str[2],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case I_CARD1A_DIS:
      strncpy(t->card1a->DIS,str[2],sizeof(t->card1a->DIS));
      break;
    case I_CARD1A_DISAZM:
      strncpy(t->card1a->DISAZM,str[2],sizeof(t->card1a->DISAZM));
      break;
    case I_CARD1A_NSTR:
      t->card1a->NSTR = ntmp;
      break;
    case I_CARD1A_LSUN:
      strncpy(t->card1a->LSUN,str[2],sizeof(t->card1a->LSUN));
      break;
    case I_CARD1A_ISUN:
      t->card1a->ISUN = ntmp;
      break;
    case I_CARD1A_CO2MX:
      t->card1a->CO2MX = xtmp;
      break;
    case I_CARD1A_H2OSTR:
      strncpy(t->card1a->H2OSTR,str[2],sizeof(t->card1a->H2OSTR));
      break;
    case I_CARD1A_O3STR:
      strncpy(t->card1a->O3STR,str[2],sizeof(t->card1a->O3STR));
      break;
    case I_CARD1A_LSUNFL:
      strncpy(t->card1a->LSUNFL,str[2],sizeof(t->card1a->LSUNFL));
      break;
    case I_CARD1A_LBMNAM:
      strncpy(t->card1a->LBMNAM,str[2],sizeof(t->card1a->LBMNAM));
      break;
    case I_CARD1A_LFLTNM:
      strncpy(t->card1a->LFLTNM,str[2],sizeof(t->card1a->LFLTNM));
      break;
    case I_CARD1A_H2OAER:
      strncpy(t->card1a->H2OAER,str[2],sizeof(t->card1a->H2OAER));
      break;
    case I_CARD1A_SOLCON:
      t->card1a->SOLCON = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD1A_check(t->card1a,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD1A_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD1A_NPAR; n++)
  {
    if(CARD1A_check(t->card1a,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD1A_check(const CARD1A_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD1A_name[n],c->MODTRN);
        rt = -1;
      }
      break;
  */
    default:
  //    rt = -1;
      break;
  }

  return rt;
}

char *CARD1A_to_s(const CARD1A_struct *c,int n,char *s)
{
  switch(n)
  {
    #ifdef OLDMODE
    case I_CARD1A_DIS:
      sprintf(s,"%s",c->DIS);
      break;
    case I_CARD1A_DISAZM:
      sprintf(s,"%s",c->DISAZM);
      break;
    case I_CARD1A_NSTR:
      sprintf(s,"%d",c->NSTR);
      break;
    case I_CARD1A_LSUN:
      sprintf(s,"%s",c->LSUN);
      break;
    case I_CARD1A_ISUN:
      sprintf(s,"%d",c->ISUN);
      break;
    case I_CARD1A_CO2MX:
      sprintf(s,"%.5f",c->CO2MX);
      break;
    case I_CARD1A_H2OSTR:
      sprintf(s,"%s",c->H2OSTR);
      break;
    case I_CARD1A_O3STR:
      sprintf(s,"%s",c->O3STR);
      break;
    case I_CARD1A_LSUNFL:
      sprintf(s,"%s",c->LSUNFL);
      break;
    case I_CARD1A_LBMNAM:
      sprintf(s,"%s",c->LBMNAM);
      break;
    case I_CARD1A_LFLTNM:
      sprintf(s,"%s",c->LFLTNM);
      break;
    case I_CARD1A_H2OAER:
      sprintf(s,"%s",c->H2OAER);
      break;
    case I_CARD1A_SOLCON:
      sprintf(s,"%.3f",c->SOLCON);
      break;
    default:
      sprintf(s,"?");
      break;
    #else
    case I_CARD1A_DIS:
      sprintf(s,"%s",c->DIS);
      break;
    case I_CARD1A_DISAZM:
      sprintf(s,"%s",c->DISAZM);
      break;
    case I_CARD1A_NSTR:
      sprintf(s,"%d",c->NSTR);
      break;
    case I_CARD1A_LSUN:
      sprintf(s,"%s",c->LSUN);
      break;
    case I_CARD1A_ISUN:
      sprintf(s,"%d",c->ISUN);
      break;
    case I_CARD1A_CO2MX:
      sprintf(s,"%.8f",c->CO2MX);
      break;
    case I_CARD1A_H2OSTR:
      sprintf(s,"%s",c->H2OSTR);
      break;
    case I_CARD1A_O3STR:
      sprintf(s,"%s",c->O3STR);
      break;
    case I_CARD1A_LSUNFL:
      sprintf(s,"%s",c->LSUNFL);
      break;
    case I_CARD1A_LBMNAM:
      sprintf(s,"%s",c->LBMNAM);
      break;
    case I_CARD1A_LFLTNM:
      sprintf(s,"%s",c->LFLTNM);
      break;
    case I_CARD1A_H2OAER:
      sprintf(s,"%s",c->H2OAER);
      break;
    case I_CARD1A_SOLCON:
      sprintf(s,"%.3f",c->SOLCON);
      break;
    default:
      sprintf(s,"?");
      break;
    #endif
  }

  return s;
}
