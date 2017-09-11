// CARD 1A parameters
char	CARD1A_ID[5]			= "1A";

char	CARD1A_comment[MAXLINE]		= "required";

char	CARD1A_name[CARD1A_NPAR][10]	=
{
  "DIS",
  "DISAZM",
  "NSTR",
  "LSUN",
  "ISUN",
  "CO2MX",
  "H2OSTR",
  "O3STR",
  "LSUNFL",
  "LBMNAM",
  "LFLTNM",
  "H2OAER",
  "SOLCON",
};

CARD1A_struct CARD1A_default		=
{
  "T",
  "T",
  16,
  "F",
  0.0,
  365.0,
  "0",
  "0",
  "F",
  "F",
  "F",
  "T",
  1.0,
};

char CARD1A_description[CARD1A_NPAR][MAXLINE] =
{
  "DISORT multiple scattering algorighm",
  "azimuth dependence flag",
  "number of streams",
  "solar irradiance from a file",
  "FWHM of the triangular scanning function",
  "CO2 mixing ratio (ppmv)",
  "vertical water vapor column (g/cm2)",
  "vertical ozone column (g/cm2)",
  "solar irradiance file name from CARD 1A1",
  "band model file name from CARD 1A2",
  "instrument filter file name from CARD 1A3",
  "",
  "scaling of the solar irradiance",
};

void CARD1A_write(const TAPE5_struct *t,FILE *fp)
{
  #ifdef OLDMODE
  fprintf(fp,"%1s%1s%3d%1s%4d%10.5f%10s%10s%1s%1s%1s%1s%1s%1s%1s%1s%2s%10.3f\n",
              t->card1a->DIS,t->card1a->DISAZM,t->card1a->NSTR,t->card1a->LSUN,t->card1a->ISUN,t->card1a->CO2MX,
              t->card1a->H2OSTR,t->card1a->O3STR,"",t->card1a->LSUNFL,"",t->card1a->LBMNAM,"",t->card1a->LFLTNM,"",
              t->card1a->H2OAER,"",t->card1a->SOLCON);
  #else
  fprintf(fp,"%1s%1s%3d%1s%4d%15.8f%15s%15s%1s%1s%1s%1s%1s%1s%1s%1s%2s%10.3f\n",
              t->card1a->DIS,t->card1a->DISAZM,t->card1a->NSTR,t->card1a->LSUN,t->card1a->ISUN,t->card1a->CO2MX,
              t->card1a->H2OSTR,t->card1a->O3STR,"",t->card1a->LSUNFL,"",t->card1a->LBMNAM,"",t->card1a->LFLTNM,"",
              t->card1a->H2OAER,"",t->card1a->SOLCON);
  #endif
}

void CARD1A_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD1A_NPAR; n++)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD1A_ID,CARD1A_name[n],CARD1A_to_s(t->card1a,n,s),CARD1A_description[n]);
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
  #ifdef OLDMODE
  n = 0;
  if(get_upp(line,n,1,t->card1a->DIS)      < 0) rt =  1; n += 1;
  if(get_upp(line,n,1,t->card1a->DISAZM)   < 0) rt =  2; n += 1;
  if(get_int(line,n,3,&t->card1a->NSTR)    < 0) rt =  3; n += 3;
  if(get_upp(line,n,1,t->card1a->LSUN)     < 0) rt =  4; n += 1;
  if(get_int(line,n,4,&t->card1a->ISUN)    < 0) rt =  5; n += 4;
  if(get_dbl(line,n,10,&t->card1a->CO2MX)  < 0) rt =  6; n += 10;
  if(get_upp(line,n,10,t->card1a->H2OSTR)  < 0) rt =  7; n += 10;
  if(get_upp(line,n,10,t->card1a->O3STR)   < 0) rt =  8; n += 11;
  if(get_upp(line,n,1,t->card1a->LSUNFL)   < 0) rt =  9; n += 2;
  if(get_upp(line,n,1,t->card1a->LBMNAM)   < 0) rt = 10; n += 2;
  if(get_upp(line,n,1,t->card1a->LFLTNM)   < 0) rt = 11; n += 2;
  if(get_upp(line,n,1,t->card1a->H2OAER)   < 0) rt = 12; n += 3;
  if(get_dbl(line,n,10,&t->card1a->SOLCON) < 0) rt = 13;
  #else
  n = 0;
  if(get_upp(line,n,1,t->card1a->DIS)      < 0) rt =  1; n += 1;
  if(get_upp(line,n,1,t->card1a->DISAZM)   < 0) rt =  2; n += 1;
  if(get_int(line,n,3,&t->card1a->NSTR)    < 0) rt =  3; n += 3;
  if(get_upp(line,n,1,t->card1a->LSUN)     < 0) rt =  4; n += 1;
  if(get_int(line,n,4,&t->card1a->ISUN)    < 0) rt =  5; n += 4;
  if(get_dbl(line,n,15,&t->card1a->CO2MX)  < 0) rt =  6; n += 15;
  if(get_upp(line,n,15,t->card1a->H2OSTR)  < 0) rt =  7; n += 15;
  if(get_upp(line,n,15,t->card1a->O3STR)   < 0) rt =  8; n += 16;
  if(get_upp(line,n,1,t->card1a->LSUNFL)   < 0) rt =  9; n += 2;
  if(get_upp(line,n,1,t->card1a->LBMNAM)   < 0) rt = 10; n += 2;
  if(get_upp(line,n,1,t->card1a->LFLTNM)   < 0) rt = 11; n += 2;
  if(get_upp(line,n,1,t->card1a->H2OAER)   < 0) rt = 12; n += 3;
  if(get_dbl(line,n,10,&t->card1a->SOLCON) < 0) rt = 13;
  #endif
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
    case 2:
    case 4:
      errno = 0;
      ntmp = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 5:
    case 12:
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
    case 0:
      strncpy(t->card1a->DIS,str[2],sizeof(t->card1a->DIS));
      break;
    case 1:
      strncpy(t->card1a->DISAZM,str[2],sizeof(t->card1a->DISAZM));
      break;
    case 2:
      t->card1a->NSTR = ntmp;
      break;
    case 3:
      strncpy(t->card1a->LSUN,str[2],sizeof(t->card1a->LSUN));
      break;
    case 4:
      t->card1a->ISUN = ntmp;
      break;
    case 5:
      t->card1a->CO2MX = xtmp;
      break;
    case 6:
      strncpy(t->card1a->H2OSTR,str[2],sizeof(t->card1a->H2OSTR));
      break;
    case 7:
      strncpy(t->card1a->O3STR,str[2],sizeof(t->card1a->O3STR));
      break;
    case 8:
      strncpy(t->card1a->LSUNFL,str[2],sizeof(t->card1a->LSUNFL));
      break;
    case 9:
      strncpy(t->card1a->LBMNAM,str[2],sizeof(t->card1a->LBMNAM));
      break;
    case 10:
      strncpy(t->card1a->LFLTNM,str[2],sizeof(t->card1a->LFLTNM));
      break;
    case 11:
      strncpy(t->card1a->H2OAER,str[2],sizeof(t->card1a->H2OAER));
      break;
    case 12:
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
    case 0:
      sprintf(s,"%s",c->DIS);
      break;
    case 1:
      sprintf(s,"%s",c->DISAZM);
      break;
    case 2:
      sprintf(s,"%d",c->NSTR);
      break;
    case 3:
      sprintf(s,"%s",c->LSUN);
      break;
    case 4:
      sprintf(s,"%d",c->ISUN);
      break;
    case 5:
      sprintf(s,"%.5f",c->CO2MX);
      break;
    case 6:
      sprintf(s,"%s",c->H2OSTR);
      break;
    case 7:
      sprintf(s,"%s",c->O3STR);
      break;
    case 8:
      sprintf(s,"%s",c->LSUNFL);
      break;
    case 9:
      sprintf(s,"%s",c->LBMNAM);
      break;
    case 10:
      sprintf(s,"%s",c->LFLTNM);
      break;
    case 11:
      sprintf(s,"%s",c->H2OAER);
      break;
    case 12:
      sprintf(s,"%.3f",c->SOLCON);
      break;
    default:
      sprintf(s,"?");
      break;
    #else
    case 0:
      sprintf(s,"%s",c->DIS);
      break;
    case 1:
      sprintf(s,"%s",c->DISAZM);
      break;
    case 2:
      sprintf(s,"%d",c->NSTR);
      break;
    case 3:
      sprintf(s,"%s",c->LSUN);
      break;
    case 4:
      sprintf(s,"%d",c->ISUN);
      break;
    case 5:
      sprintf(s,"%.8f",c->CO2MX);
      break;
    case 6:
      sprintf(s,"%s",c->H2OSTR);
      break;
    case 7:
      sprintf(s,"%s",c->O3STR);
      break;
    case 8:
      sprintf(s,"%s",c->LSUNFL);
      break;
    case 9:
      sprintf(s,"%s",c->LBMNAM);
      break;
    case 10:
      sprintf(s,"%s",c->LFLTNM);
      break;
    case 11:
      sprintf(s,"%s",c->H2OAER);
      break;
    case 12:
      sprintf(s,"%.3f",c->SOLCON);
      break;
    default:
      sprintf(s,"?");
      break;
    #endif
  }

  return s;
}
