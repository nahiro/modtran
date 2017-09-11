// CARD 2 parameters
char	CARD2_ID[5]			= "2";

char	CARD2_comment[MAXLINE]		= "required";

char	CARD2_name[CARD2_NPAR][10]	=
{
  "APLUS",
  "IHAZE",
  "CNOVAM",
  "ISEASN",
  "ARUSS",
  "IVULCN",
  "ICSTL",
  "ICLD",
  "IVSA",
  "VIS",
  "WSS",
  "WHH",
  "RAINRT",
  "GNDALT",
};

CARD2_struct CARD2_default		=
{
  " ",
  4,
  " ",
  0,
  " ",
  0,
  3,
  0,
  0,
  0.0,
  0.0,
  0.0,
  0.0,
  0.0,
};

char CARD2_description[CARD2_NPAR][MAXLINE] =
{
  "Aerosol Plus option",
  "type of extinction and meteorological range",
  "use of the NOVAM",
  "seasonal aerosol profile",
  "user-defined aerosol optical properties",
  "type of extinction and profile",
  "air mass character",
  "cloud and rain model",
  "use of the VSA",
  "surface meteorological range (km)",
  "current wind speed (m/s)",
  "24-hour average wind speed (m/s)",
  "rain rate (mm/h)",
  "altitude of the surface (km)",
};

void CARD2_write(const TAPE5_struct *t,FILE *fp)
{
  #ifdef OLDMODE
  fprintf(fp,"%2s%3d%1s%4d%3s%2d%5d%5d%5d%10.5f%10.5f%10.5f%10.5f%10.5f\n",
              t->card2->APLUS,t->card2->IHAZE,t->card2->CNOVAM,t->card2->ISEASN,t->card2->ARUSS,t->card2->IVULCN,t->card2->ICSTL,
              t->card2->ICLD,t->card2->IVSA,t->card2->VIS,t->card2->WSS,t->card2->WHH,t->card2->RAINRT,t->card2->GNDALT);
  #else
  fprintf(fp,"%2s%3d%1s%4d%3s%2d%5d%5d%5d%15.10f%15.10f%15.10f%15.10f%15.10f\n",
              t->card2->APLUS,t->card2->IHAZE,t->card2->CNOVAM,t->card2->ISEASN,t->card2->ARUSS,t->card2->IVULCN,t->card2->ICSTL,
              t->card2->ICLD,t->card2->IVSA,t->card2->VIS,t->card2->WSS,t->card2->WHH,t->card2->RAINRT,t->card2->GNDALT);
  #endif
}

void CARD2_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD2_NPAR; n++)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD2_ID,CARD2_name[n],CARD2_to_s(t->card2,n,s),CARD2_description[n]);
  }
}

int CARD2_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  #ifdef OLDMODE
  n = 0;
  if(get_upp(line,n,2,t->card2->APLUS)    < 0) rt =  1; n += 2;
  if(get_int(line,n,3,&t->card2->IHAZE)   < 0) rt =  2; n += 3;
  if(get_upp(line,n,1,t->card2->CNOVAM)   < 0) rt =  3; n += 1;
  if(get_int(line,n,4,&t->card2->ISEASN)  < 0) rt =  4; n += 4;
  if(get_upp(line,n,3,t->card2->ARUSS)    < 0) rt =  5; n += 3;
  if(get_int(line,n,2,&t->card2->IVULCN)  < 0) rt =  6; n += 2;
  if(get_int(line,n,5,&t->card2->ICSTL)   < 0) rt =  7; n += 5;
  if(get_int(line,n,5,&t->card2->ICLD)    < 0) rt =  8; n += 5;
  if(get_int(line,n,5,&t->card2->IVSA)    < 0) rt =  9; n += 5;
  if(get_dbl(line,n,10,&t->card2->VIS)    < 0) rt = 10; n += 10;
  if(get_dbl(line,n,10,&t->card2->WSS)    < 0) rt = 11; n += 10;
  if(get_dbl(line,n,10,&t->card2->WHH)    < 0) rt = 12; n += 10;
  if(get_dbl(line,n,10,&t->card2->RAINRT) < 0) rt = 13; n += 10;
  if(get_dbl(line,n,10,&t->card2->GNDALT) < 0) rt = 14;
  #else
  n = 0;
  if(get_upp(line,n,2,t->card2->APLUS)    < 0) rt =  1; n += 2;
  if(get_int(line,n,3,&t->card2->IHAZE)   < 0) rt =  2; n += 3;
  if(get_upp(line,n,1,t->card2->CNOVAM)   < 0) rt =  3; n += 1;
  if(get_int(line,n,4,&t->card2->ISEASN)  < 0) rt =  4; n += 4;
  if(get_upp(line,n,3,t->card2->ARUSS)    < 0) rt =  5; n += 3;
  if(get_int(line,n,2,&t->card2->IVULCN)  < 0) rt =  6; n += 2;
  if(get_int(line,n,5,&t->card2->ICSTL)   < 0) rt =  7; n += 5;
  if(get_int(line,n,5,&t->card2->ICLD)    < 0) rt =  8; n += 5;
  if(get_int(line,n,5,&t->card2->IVSA)    < 0) rt =  9; n += 5;
  if(get_dbl(line,n,10,&t->card2->VIS)    < 0) rt = 10; n += 15;
  if(get_dbl(line,n,10,&t->card2->WSS)    < 0) rt = 11; n += 15;
  if(get_dbl(line,n,10,&t->card2->WHH)    < 0) rt = 12; n += 15;
  if(get_dbl(line,n,10,&t->card2->RAINRT) < 0) rt = 13; n += 15;
  if(get_dbl(line,n,10,&t->card2->GNDALT) < 0) rt = 14;
  #endif
  if(rt)
  {
    fprintf(stderr,"CARD2_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD2_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD2_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD2_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD2_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // convert value if required
  switch(n)
  {
    case 1:
    case 3:
    case 5:
    case 6:
    case 7:
    case 8:
      errno = 0;
      ntmp = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
      errno = 0;
      xtmp = strtod(str[2],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      strncpy(t->card2->APLUS,str[2],sizeof(t->card2->APLUS));
      break;
    case 1:
      t->card2->IHAZE = ntmp;
      break;
    case 2:
      strncpy(t->card2->CNOVAM,str[2],sizeof(t->card2->CNOVAM));
      break;
    case 3:
      t->card2->ISEASN = ntmp;
      break;
    case 4:
      strncpy(t->card2->ARUSS,str[2],sizeof(t->card2->ARUSS));
      break;
    case 5:
      t->card2->IVULCN = ntmp;
      break;
    case 6:
      t->card2->ICSTL = ntmp;
      break;
    case 7:
      t->card2->ICLD = ntmp;
      break;
    case 8:
      t->card2->IVSA = ntmp;
      break;
    case 9:
      t->card2->VIS = xtmp;
      break;
    case 10:
      t->card2->WSS = xtmp;
      break;
    case 11:
      t->card2->WHH = xtmp;
      break;
    case 12:
      t->card2->RAINRT = xtmp;
      break;
    case 13:
      t->card2->GNDALT = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2_check(t->card2,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD2_NPAR; n++)
  {
    if(CARD2_check(t->card2,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD2_check(const CARD2_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
/*
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

    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD2: error in %s, %s\n",CARD2_name[n],c->MODTRN);
        rt = -1;
      }
      break;
*/
    default:
      //rt = -1;
      break;
  }

  return rt;
}

char *CARD2_to_s(const CARD2_struct *c,int n,char *s)
{
  switch(n)
  {
    #ifdef OLDMODE
    case 0:
      sprintf(s,"%s",c->APLUS);
      break;
    case 1:
      sprintf(s,"%d",c->IHAZE);
      break;
    case 2:
      sprintf(s,"%s",c->CNOVAM);
      break;
    case 3:
      sprintf(s,"%d",c->ISEASN);
      break;
    case 4:
      sprintf(s,"%s",c->ARUSS);
      break;
    case 5:
      sprintf(s,"%d",c->IVULCN);
      break;
    case 6:
      sprintf(s,"%d",c->ICSTL);
      break;
    case 7:
      sprintf(s,"%d",c->ICLD);
      break;
    case 8:
      sprintf(s,"%d",c->IVSA);
      break;
    case 9:
      sprintf(s,"%.6f",c->VIS);
      break;
    case 10:
      sprintf(s,"%.3f",c->WSS);
      break;
    case 11:
      sprintf(s,"%.3f",c->WHH);
      break;
    case 12:
      sprintf(s,"%.3f",c->RAINRT);
      break;
    case 13:
      sprintf(s,"%.3f",c->GNDALT);
      break;
    default:
      sprintf(s,"?");
      break;
    #else
    case 0:
      sprintf(s,"%s",c->APLUS);
      break;
    case 1:
      sprintf(s,"%d",c->IHAZE);
      break;
    case 2:
      sprintf(s,"%s",c->CNOVAM);
      break;
    case 3:
      sprintf(s,"%d",c->ISEASN);
      break;
    case 4:
      sprintf(s,"%s",c->ARUSS);
      break;
    case 5:
      sprintf(s,"%d",c->IVULCN);
      break;
    case 6:
      sprintf(s,"%d",c->ICSTL);
      break;
    case 7:
      sprintf(s,"%d",c->ICLD);
      break;
    case 8:
      sprintf(s,"%d",c->IVSA);
      break;
    case 9:
      sprintf(s,"%.11f",c->VIS);
      break;
    case 10:
      sprintf(s,"%.8f",c->WSS);
      break;
    case 11:
      sprintf(s,"%.8f",c->WHH);
      break;
    case 12:
      sprintf(s,"%.8f",c->RAINRT);
      break;
    case 13:
      sprintf(s,"%.8f",c->GNDALT);
      break;
    default:
      sprintf(s,"?");
      break;
    #endif
  }

  return s;
}
