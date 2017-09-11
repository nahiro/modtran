// CARD 4B3 parameters
char	CARD4B3_ID[5]			= "4B3";

char	CARD4B3_comment[MAXLINE]	= "If SURREF = 'BRDF'";

char	CARD4B3_name[CARD4B3_NPAR][10]	=
{
  "WVSURF",
  "PARAMS",
};

CARD4B3_struct CARD4B3_default		=
{
  NAN,
  {NAN,NAN,NAN,NAN,NAN},
};

char CARD4B3_description[CARD4B3_NPAR][MAXLINE] =
{
  "BRDF spectral wavelength (um)",
  "BRDF parameters",
};

void CARD4B3_write(const CARD4B3_struct *c,int kmx,FILE *fp)
{
  int k;

  fprintf(fp,"%10.3e",c->WVSURF);
  for(k=0; k<kmx; k++)
  {
    fprintf(fp," %10.3e%s",c->PARAMS[k],k==kmx-1?"\n":"");
  }
}

void CARD4B3_print(const CARD4B3_struct *c,int i,int j,int kmx,FILE *fp)
{
  int k,n;
  char s[MAXLINE];

  for(n=0; n<CARD4B3_NPAR; n++)
  {
    switch(n)
    {
      case 0:
        fprintf(fp,"%-4s %-10s %-3d %-3d %-15s # %s\n",CARD4B3_ID,CARD4B3_name[n],i,j,CARD4B3_to_s(c,n,s),CARD4B3_description[n]);
        break;
      case 1:
        for(k=0; k<kmx; k++)
        {
          if(k == 0)
          {
            fprintf(fp,"%-4s %-10s %1d %-3d %1d %-15.3e # %s\n",CARD4B3_ID,CARD4B3_name[n],i,j,k,c->PARAMS[j],CARD4B3_description[n]);
          }
          else
          {
            fprintf(fp,"%-4s %-10s %1d %-3d %1d %-15.3e\n",CARD4B3_ID,CARD4B3_name[n],i,j,k,c->PARAMS[j]);
          }
        }
        break;
      default:
        break;
    }
  }
}

int CARD4B3_read(CARD4B3_struct *c,int kmx,FILE *fp)
{
  int i,imx,n;
  int nc;
  int rt;
  double xtmp;
  char line[CARD_LENGTH];
  char temp[CARD_LENGTH];
  char *p,*endp;

  // initialization
  imx = kmx+1;
  rt = 0;

  i = 0;
  while(i < imx)
  {
    // read line
    if(fgets(line,CARD_LENGTH,fp) == NULL)
    {
      fprintf(stderr,"CARD4B3_read: read error.\n");
      return -1;
    }
    // read values
    for(nc=0,p=line;; p+=nc)
    {
      if(sscanf(p,"%s%n",temp,&nc) == EOF) break;
      errno = 0;
      xtmp = strtod(temp,&endp);
      if(errno==ERANGE || *endp!='\0')
      {
        fprintf(stderr,"CARD4B3_read: convert error >>> %s\n",line);
        rt = -1;
        break;
      }
      if(i == 0)
      {
        c->WVSURF = xtmp;
      } else
      if(i < imx)
      {
        c->PARAMS[i-1] = xtmp;
      }
    }
  }
  if(i != imx) rt = -1;

  // check values
  for(n=0; n<CARD4B3_NPAR; n++)
  {
    if(CARD4B3_check(c,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD4B3_read: rt=%d\n",rt);
    rt = -1;
  }

  return rt;
}

int CARD4B3_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int ncol;
  int idx1 = 0;
  int idx2 = 0;
  int idx3 = 0;
  double xtmp = NAN;
  char str[NCOL6][MAXLINE];
  char *p;

  // read line
  for(i=0,p=line; i<NCOL6; i++,p+=n)
  {
    if(sscanf(p,"%s%n",str[i],&n) == EOF) break;
  }
  ncol = i;
  if(ncol < 2)
  {
    return -1;
  }
  if(strcmp(CARD4B3_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(ncol < NCOL5)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD4B3_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD4B3_name[i]) == 0)
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
      idx1 = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4B3: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx1<0 || idx1>=MXANGL)
      {
        fprintf(stderr,"CARD4B3: index 1 out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      idx2 = strtol(str[3],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4B3: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx2<0 || idx2>=MXANGL)
      {
        fprintf(stderr,"CARD4B3: index 2 out of range >>> %s\n",str[3]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[4],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4B3: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 1:
      errno = 0;
      idx1 = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4B3: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx1<0 || idx1>=MXANGL)
      {
        fprintf(stderr,"CARD4B3: index 1 out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      idx2 = strtol(str[3],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4B3: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx2<0 || idx2>=MXANGL)
      {
        fprintf(stderr,"CARD4B3: index 2 out of range >>> %s\n",str[3]);
        return -1;
      }
      errno = 0;
      idx3 = strtol(str[4],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4B3: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx3<0 || idx3>=MXANGL)
      {
        fprintf(stderr,"CARD4B3: index 3 out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[5],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4B3: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      ((t->card4b3)+idx1*MWVSRF+idx2)->WVSURF = xtmp;
      break;
    case 1:
      ((t->card4b3)+idx1*MWVSRF+idx2)->PARAMS[idx3] = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD4B3_check((t->card4b3)+idx1*MWVSRF+idx2,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD4B3_check_all(const TAPE5_struct *t)
{
  int i,j,n;
  int rt;

  rt = 0;
  for(i=0; i<t->card4a->NSURF; i++)
  {
    for(j=0; j<t->card4b2->NWVSRF; j++)
    {
      for(n=0; n<CARD4B3_NPAR; n++)
      {
        if(CARD4B3_check((t->card4b3)+i*MWVSRF+j,n) < 0)
        {
          rt = -1;
        }
      }
    }
  }

  return rt;
}

int CARD4B3_check(const CARD4B3_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD4B3_name[n],c->MODTRN);
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

char *CARD4B3_to_s(const CARD4B3_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%.3e",c->WVSURF);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
