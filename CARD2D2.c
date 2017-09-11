// CARD 2D2 parameters
char	CARD2D2_ID[5]			= "2D2";

char	CARD2D2_comment[MAXLINE]	= "If IHAZE = 7 or ICLD = 11";

char	CARD2D2_name[CARD2D2_NPAR][10]	=
{
  "VARSPC",
  "EXTC",
  "ABSC",
  "ASYM",
};

CARD2D2_struct CARD2D2_default		=
{
  NAN,
  NAN,
  NAN,
  NAN,
};

char CARD2D2_description[CARD2D2_NPAR][MAXLINE] =
{
  "wavelength for the coefficients",
  "extinction coefficients",
  "absorption coefficients",
  "asymmetry parameter",
};

int CARD2D2_JMAX;

void CARD2D2_write(const CARD2D2_struct *c,FILE *fp)
{
  int j;

  for(j=0; j<CARD2D2_JMAX; j++)
  {
    #ifdef OLDMODE
    if((c+j)->VARSPC > 99.99)
    {
      fprintf(fp,"%6.2f%7.5f%7.5f%6.4f%s",(c+j)->VARSPC,(c+j)->EXTC,(c+j)->ABSC,(c+j)->ASYM,j==CARD2D2_JMAX-1?"\n":j%3==2?"\n":"");
    }
    else
    {
      fprintf(fp,"%6.3f%7.5f%7.5f%6.4f%s",(c+j)->VARSPC,(c+j)->EXTC,(c+j)->ABSC,(c+j)->ASYM,j==CARD2D2_JMAX-1?"\n":j%3==2?"\n":"");
    }
    #else
    fprintf(fp,"%15.11f%15.13f%15.13f%15.13f%s",(c+j)->VARSPC,(c+j)->EXTC,(c+j)->ABSC,(c+j)->ASYM,j==CARD2D2_JMAX-1?"\n":j%3==2?"\n":"");
    #endif
  }
}

void CARD2D2_print(const CARD2D2_struct *c,int i,int j,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD2D2_NPAR; n++)
  {
    fprintf(fp,"%-4s %-10s %d %-5d %-15s # %s\n",CARD2D2_ID,CARD2D2_name[n],i,j,CARD2D2_to_s(c,n,s),CARD2D2_description[n]);
  }
}

int CARD2D2_read(CARD2D2_struct *c,FILE *fp)
{
  int j,n;
  int rt;
  char line[CARD_LENGTH];

  // initialization
  rt = 0;

  j = 0;
  while(j < CARD2D2_JMAX)
  {
    memset(line,'\n',CARD_LENGTH);
    // read line
    if(fgets(line,CARD_LENGTH,fp) == NULL)
    {
      fprintf(stderr,"CARD2D2_read: read error.\n");
      return -1;
    }
    // read values
    #ifdef OLDMODE
    n = 0;
    if(get_dbl(line,n,6,&(c+j)->VARSPC) != 0) break; n += 6;
    if(get_dbl(line,n,7,&(c+j)->EXTC)   != 0) break; n += 7;
    if(get_dbl(line,n,7,&(c+j)->ABSC)   != 0) break; n += 7;
    if(get_dbl(line,n,6,&(c+j)->ASYM)   != 0) break; n += 6; j++;
    if(get_dbl(line,n,6,&(c+j)->VARSPC) != 0) break; n += 6;
    if(get_dbl(line,n,7,&(c+j)->EXTC)   != 0) break; n += 7;
    if(get_dbl(line,n,7,&(c+j)->ABSC)   != 0) break; n += 7;
    if(get_dbl(line,n,6,&(c+j)->ASYM)   != 0) break; n += 6; j++;
    if(get_dbl(line,n,6,&(c+j)->VARSPC) != 0) break; n += 6;
    if(get_dbl(line,n,7,&(c+j)->EXTC)   != 0) break; n += 7;
    if(get_dbl(line,n,7,&(c+j)->ABSC)   != 0) break; n += 7;
    if(get_dbl(line,n,6,&(c+j)->ASYM)   != 0) break; j++;
    #else
    n = 0;
    if(get_dbl(line,n,15,&(c+j)->VARSPC) != 0) break; n += 15;
    if(get_dbl(line,n,15,&(c+j)->EXTC)   != 0) break; n += 15;
    if(get_dbl(line,n,15,&(c+j)->ABSC)   != 0) break; n += 15;
    if(get_dbl(line,n,15,&(c+j)->ASYM)   != 0) break; n += 15; j++;
    if(get_dbl(line,n,15,&(c+j)->VARSPC) != 0) break; n += 15;
    if(get_dbl(line,n,15,&(c+j)->EXTC)   != 0) break; n += 15;
    if(get_dbl(line,n,15,&(c+j)->ABSC)   != 0) break; n += 15;
    if(get_dbl(line,n,15,&(c+j)->ASYM)   != 0) break; n += 15; j++;
    if(get_dbl(line,n,15,&(c+j)->VARSPC) != 0) break; n += 15;
    if(get_dbl(line,n,15,&(c+j)->EXTC)   != 0) break; n += 15;
    if(get_dbl(line,n,15,&(c+j)->ABSC)   != 0) break; n += 15;
    if(get_dbl(line,n,15,&(c+j)->ASYM)   != 0) break; j++;
    #endif
  }
  if(j != CARD2D2_JMAX)
  {
    fprintf(stderr,"CARD2D2_read: error, j=%d, CARD2D2_JMAX=%d\n",j,CARD2D2_JMAX);
    rt = -1;
  }

  // check values
  for(n=0; n<CARD2D2_NPAR; n++)
  {
    if(CARD2D2_check(c,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD2D2_read: rt=%d\n",rt);
    rt = -1;
  }

  return rt;
}

int CARD2D2_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int ncol;
  int idx1 = -1;
  int idx2 = -1;
  double xtmp = NAN;
  char str[NCOL5][MAXLINE];
  char *p;

  // read line
  for(i=0,p=line; i<NCOL5; i++,p+=n)
  {
    if(sscanf(p,"%s%n",str[i],&n) == EOF) break;
  }
  ncol = i;
  if(ncol < 2)
  {
    return -1;
  }
  if(strcmp(CARD2D2_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(ncol < NCOL5)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2D2_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2D2_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // convert value if required
  switch(n)
  {
    case 0:
    case 1:
    case 2:
    case 3:
      errno = 0;
      idx1 = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2D2: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx1<0 || idx1>=MXIREG)
      {
        fprintf(stderr,"CARD2D2: index 1 out of range >>> %s\n",str[2]);
        return -1;
      }
      break;
      errno = 0;
      idx2 = strtol(str[3],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2D2: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx2<0 || idx2>=MXNSPC)
      {
        fprintf(stderr,"CARD2D2: index 2 out of range >>> %s\n",str[3]);
        return -1;
      }
      break;
      errno = 0;
      xtmp = strtod(str[4],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2D2: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      ((t->card2d2)+idx1*MXNSPC+idx2)->VARSPC = xtmp;
      break;
    case 1:
      ((t->card2d2)+idx1*MXNSPC+idx2)->EXTC = xtmp;
      break;
    case 2:
      ((t->card2d2)+idx1*MXNSPC+idx2)->ABSC = xtmp;
      break;
    case 3:
      ((t->card2d2)+idx1*MXNSPC+idx2)->ASYM = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2D2_check((t->card2d2)+idx1*MXNSPC+idx2,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2D2_check_all(const TAPE5_struct *t)
{
  int i,j,jmx,k,n;
  int rt;

  rt = 0;
  for(i=0; i<MXIREG; i++)
  {
    if(t->card2d->IREG[i] > 1)
    {
      jmx = t->card2d->IREG[i];
    }
    else
    {
      jmx = MXNSPC;
    }
    for(j=0; j<jmx; j++)
    {
      k = i*MXNSPC+j;
      for(n=0; n<CARD2D2_NPAR; n++)
      {
        if(CARD2D2_check((t->card2d2)+k,n) < 0)
        {
          rt = -1;
        }
      }
    }
  }

  return rt;
}

int CARD2D2_check(const CARD2D2_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD2D2: error in %s, %s\n",CARD2D2_name[n],c->MODTRN);
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

char *CARD2D2_to_s(const CARD2D2_struct *c,int n,char *s)
{
  switch(n)
  {
    #ifdef OLDMODE
    case 0:
      sprintf(s,"%.5f",c->VARSPC);
      break;
    case 1:
      sprintf(s,"%.5f",c->EXTC);
      break;
    case 2:
      sprintf(s,"%.5f",c->ABSC);
      break;
    case 3:
      sprintf(s,"%.5f",c->ASYM);
      break;
    default:
      sprintf(s,"?");
      break;
    #else
    case 0:
      sprintf(s,"%.11f",c->VARSPC);
      break;
    case 1:
      sprintf(s,"%.13f",c->EXTC);
      break;
    case 2:
      sprintf(s,"%.13f",c->ABSC);
      break;
    case 3:
      sprintf(s,"%.13f",c->ASYM);
      break;
    default:
      sprintf(s,"?");
      break;
    #endif
  }

  return s;
}
