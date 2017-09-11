// CARD 4B2 parameters
char	CARD4B2_ID[5]			= "4B2";

char	CARD4B2_comment[MAXLINE]	= "If SURREF = 'BRDF'";

char	CARD4B2_name[CARD4B2_NPAR][10]	=
{
  "NWVSRF",
  "SURFZN",
  "SURFAZ",
};

CARD4B2_struct CARD4B2_default		=
{
  0,
  NAN,
  NAN,
};

char CARD4B2_description[CARD4B2_NPAR][MAXLINE] =
{
  "number of BRDF spectral grid points",
  "zenith angle (deg) of the surface normal",
  "azimuth angle (deg) of the surface normal",
};

void CARD4B2_write(const CARD4B2_struct *c,FILE *fp)
{
  fprintf(fp,"%5d%10.3f%10.3f\n",c->NWVSRF,c->SURFZN,c->SURFAZ);
}

void CARD4B2_print(const CARD4B2_struct *c,int i,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD4B2_NPAR; n++)
  {
    fprintf(fp,"%-4s %-10s %-7d %-15s # %s\n",CARD4B2_ID,CARD4B2_name[n],i,CARD4B2_to_s(c,n,s),CARD4B2_description[n]);
  }
}

int CARD4B2_read(CARD4B2_struct *c,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD4B2_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_int(line,n,5,&c->NWVSRF)  < 0) rt = 1; n += 5;
  if(get_dbl(line,n,10,&c->SURFZN) < 0) rt = 2; n += 10;
  if(get_dbl(line,n,10,&c->SURFAZ) < 0) rt = 3;
  if(rt)
  {
    fprintf(stderr,"CARD4B2_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  for(n=0; n<CARD4B2_NPAR; n++)
  {
    if(CARD4B2_check(c,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD4B2_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD4B2_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int indx;
  int ntmp = 0;
  double xtmp = NAN;
  char str[NCOL4][MAXLINE];
  char *p;

  // read line
  for(i=0,p=line; i<NCOL4; i++,p+=n)
  {
    if(sscanf(p,"%s%n",str[i],&n) == EOF) break;
  }
  if(i < 2)
  {
    return -1;
  }
  if(strcmp(CARD4B2_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL4)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD4B2_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD4B2_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // convert value if required
  switch(n)
  {
    case 0:
      errno = 0;
      indx = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4B2: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=MXNSRF)
      {
        fprintf(stderr,"CARD4B2: index out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      ntmp = strtol(str[3],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4B2: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 1:
    case 2:
      errno = 0;
      indx = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4B2: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=MXNSRF)
      {
        fprintf(stderr,"CARD4B2: index out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[3],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4B2: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      ((t->card4b2)+indx)->NWVSRF = ntmp;
      break;
    case 1:
      ((t->card4b2)+indx)->SURFZN = xtmp;
      break;
    case 2:
      ((t->card4b2)+indx)->SURFAZ = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD4B2_check((t->card4b2)+indx,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD4B2_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD4B2_NPAR; n++)
  {
    if(CARD4B2_check(t->card4b2,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD4B2_check(const CARD4B2_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD4B2_name[n],c->MODTRN);
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

char *CARD4B2_to_s(const CARD4B2_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%d",c->NWVSRF);
      break;
    case 1:
      sprintf(s,"%.3f",c->SURFZN);
      break;
    case 2:
      sprintf(s,"%.3f",c->SURFAZ);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
