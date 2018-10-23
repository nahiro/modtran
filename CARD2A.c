// CARD 2A parameters
char	CARD2A_ID[5]			= "2A";

char	CARD2A_comment[2][MAXLINE]	=
{
  "If ICLD = 18 or 19",
  "If ICLD = 1 - 10",
};

char	CARD2A_name[CARD2A_NPAR][10]	=
{
  "CTHIK",
  "CALT",
  "CEXT",
  "NCRALT",
  "NCRSPC",
  "CWAVLN",
  "CCOLWD",
  "CCOLIP",
  "CHUMID",
  "ASYMWD",
  "ASYMIP",
};

CARD2A_struct CARD2A_default		=
{
  0.0,
  0.0,
  0.0,
  0,
  0,
  0.0,
  0.0,
  0.0,
  0.0,
  0.0,
  0.0,
};

char CARD2A_description[CARD2A_NPAR+3][MAXLINE] =
{
  "cirrus thickness (km)",
  "cirrus base altitude (km)",
  "extinction coefficient (km-1) at 0.55 micron",
  "cloud vertical thickness (km)",
  "cloud base altitude relative to ground level (km)",
  "cloud vertical extinction (km-1)",
  "number of layer boundary altitudes",
  "number of wavelength entries",
  "reference wavelength (um) for extinction",
  "WD cloud vertical column density (kmg/m3)",
  "IP cloud vertical column density (kmg/m3)",
  "relative humidity (%)",
  "HG phase function asymmetry factor for WD",
  "HG phase function asymmetry factor for IP",
};

void CARD2A_write(const TAPE5_struct *t,FILE *fp)
{
  if(t->card2->ICLD <= 10)
  {
    fprintf(fp,"%8.3f%8.3f%8.3f%4d%4d%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f\n",
                t->card2a->CTHIK,t->card2a->CALT,t->card2a->CEXT,t->card2a->NCRALT,t->card2a->NCRSPC,
                t->card2a->CWAVLN,t->card2a->CCOLWD,t->card2a->CCOLIP,t->card2a->CHUMID,t->card2a->ASYMWD,t->card2a->ASYMIP);
  }
  else
  {
    fprintf(fp,"%8.3f%8.3f%8.3f\n",t->card2a->CTHIK,t->card2a->CALT,t->card2a->CEXT);
  }
}

void CARD2A_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  if(t->card2->ICLD <= 10)
  {
    for(n=3; n<CARD2A_NPAR+3; n++)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD2A_ID,CARD2A_name[n-3],CARD2A_to_s(t->card2a,n-3,s),CARD2A_description[n]);
    }
  }
  else
  {
    for(n=0; n<3; n++)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD2A_ID,CARD2A_name[n],CARD2A_to_s(t->card2a,n,s),CARD2A_description[n]);
    }
  }
}

int CARD2A_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2A_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(t->card2->ICLD <= 10)
  {
    if(get_dbl(line,n,8,&t->card2a->CTHIK)  < 0) rt =  1; n += 8;
    if(get_dbl(line,n,8,&t->card2a->CALT)   < 0) rt =  2; n += 8;
    if(get_dbl(line,n,8,&t->card2a->CEXT)   < 0) rt =  3; n += 8;
    if(get_int(line,n,4,&t->card2a->NCRALT) < 0) rt =  4; n += 4;
    if(get_int(line,n,4,&t->card2a->NCRSPC) < 0) rt =  5; n += 4;
    if(get_dbl(line,n,8,&t->card2a->CWAVLN) < 0) rt =  6; n += 8;
    if(get_dbl(line,n,8,&t->card2a->CCOLWD) < 0) rt =  7; n += 8;
    if(get_dbl(line,n,8,&t->card2a->CCOLIP) < 0) rt =  8; n += 8;
    if(get_dbl(line,n,8,&t->card2a->CHUMID) < 0) rt =  9; n += 8;
    if(get_dbl(line,n,8,&t->card2a->ASYMWD) < 0) rt = 10; n += 8;
    if(get_dbl(line,n,8,&t->card2a->ASYMIP) < 0) rt = 11;
  }
  else
  {
    if(get_dbl(line,n,8,&t->card2a->CTHIK) < 0) rt = 12; n += 8;
    if(get_dbl(line,n,8,&t->card2a->CALT)  < 0) rt = 13; n += 8;
    if(get_dbl(line,n,8,&t->card2a->CEXT)  < 0) rt = 14;
  }
  if(rt)
  {
    fprintf(stderr,"CARD2A_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD2A_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD2A_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD2A_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD2A_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2A_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2A_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // convert value if required
  switch(n)
  {
    case 3:
    case 4:
      errno = 0;
      ntmp = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 0:
    case 1:
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
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
      t->card2a->CTHIK = xtmp;
      break;
    case 1:
      t->card2a->CALT = xtmp;
      break;
    case 2:
      t->card2a->CEXT = xtmp;
      break;
    case 3:
      t->card2a->NCRALT = ntmp;
      break;
    case 4:
      t->card2a->NCRSPC = ntmp;
      break;
    case 5:
      t->card2a->CWAVLN = xtmp;
      break;
    case 6:
      t->card2a->CCOLWD = xtmp;
      break;
    case 7:
      t->card2a->CCOLIP = xtmp;
      break;
    case 8:
      t->card2a->CHUMID = xtmp;
      break;
    case 9:
      t->card2a->ASYMWD = xtmp;
      break;
    case 10:
      t->card2a->ASYMIP = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2A_check(t->card2a,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2A_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD2A_NPAR; n++)
  {
    if(CARD2A_check(t->card2a,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD2A_check(const CARD2A_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD2A_name[n],c->MODTRN);
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

char *CARD2A_to_s(const CARD2A_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%.3f",c->CTHIK);
      break;
    case 1:
      sprintf(s,"%.3f",c->CALT);
      break;
    case 2:
      sprintf(s,"%.3f",c->CEXT);
      break;
    case 3:
      sprintf(s,"%d",c->NCRALT);
      break;
    case 4:
      sprintf(s,"%d",c->NCRSPC);
      break;
    case 5:
      sprintf(s,"%.3f",c->CWAVLN);
      break;
    case 6:
      sprintf(s,"%.3f",c->CCOLWD);
      break;
    case 7:
      sprintf(s,"%.3f",c->CCOLIP);
      break;
    case 8:
      sprintf(s,"%.3f",c->CHUMID);
      break;
    case 9:
      sprintf(s,"%.3f",c->ASYMWD);
      break;
    case 10:
      sprintf(s,"%.3f",c->ASYMIP);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
