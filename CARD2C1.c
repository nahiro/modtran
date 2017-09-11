// CARD 2C1 parameters
char	CARD2C1_ID[5]			= "2C1";

char	CARD2C1_comment[MAXLINE]	= "as required";

char	CARD2C1_name[CARD2C1_NPAR][10]	=
{
  "ZM",
  "P",
  "T",
  "WMOL1",
  "WMOL2",
  "WMOL3",
  "JCHAR",
  "JCHARX",
};

CARD2C1_struct CARD2C1_default		=
{
  NAN,
  NAN,
  NAN,
  NAN,
  NAN,
  NAN,
  {"?","?","?","?","?","?","?","?","?","?","?","?","?","?"},
  "?",
};

char CARD2C1_description[CARD2C1_NPAR][MAXLINE] =
{
  "altitude of layer boundary (km)",
  "pressure of layer boundary",
  "temperature of layer boundary",
  "density of H2O",
  "density of CO2",
  "density of O3",
  "units for P, T, and molecular constituents",
  "units for CFCs and other heavy molecules",
};

void CARD2C1_write(const CARD2C1_struct *c,FILE *fp)
{
  int i;

  #ifdef OLDMODE
  fprintf(fp,"%10.3f%10.3e%10.3e%10.3e%10.3e%10.3e",c->ZM,c->P,c->T,c->WMOL1,c->WMOL2,c->WMOL3);
  #else
  fprintf(fp,"%15.8f%15.8e%15.8e%15.8e%15.8e%15.8e",c->ZM,c->P,c->T,c->WMOL1,c->WMOL2,c->WMOL3);
  #endif
  for(i=0; i<CARD2C1_JCHAR_NINDX; i++)
  {
    fprintf(fp,"%1s",c->JCHAR[i]);
  }
  fprintf(fp," %1s\n",c->JCHARX);
}

void CARD2C1_print(const CARD2C1_struct *c,int i,FILE *fp)
{
  int j,n;
  char s[MAXLINE];

  for(n=0; n<CARD2C1_NPAR; n++)
  {
    switch(n)
    {
      case 6:
        for(j=0; j<CARD2C1_JCHAR_NINDX; j++)
        {
          fprintf(fp,"%-4s %-10s %-3d %-3d %-15s # %s\n",CARD2C1_ID,CARD2C1_name[n],i,j,c->JCHAR[j],CARD2C1_description[n]);
        }
        break;
      default:
        fprintf(fp,"%-4s %-10s %-7d %-15s # %s\n",CARD2C1_ID,CARD2C1_name[n],i,CARD2C1_to_s(c,n,s),CARD2C1_description[n]);
        break;
    }
  }
}

int CARD2C1_read(CARD2C1_struct *c,FILE *fp)
{
  int i,n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2C1_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  #ifdef OLDMODE
  n = 0;
  if(get_dbl(line,n,10,&c->ZM)    < 0) rt = 1; n += 10;
  if(get_dbl(line,n,10,&c->P)     < 0) rt = 2; n += 10;
  if(get_dbl(line,n,10,&c->T)     < 0) rt = 3; n += 10;
  if(get_dbl(line,n,10,&c->WMOL1) < 0) rt = 4; n += 10;
  if(get_dbl(line,n,10,&c->WMOL2) < 0) rt = 5; n += 10;
  if(get_dbl(line,n,10,&c->WMOL3) < 0) rt = 6; n += 10;
  #else
  n = 0;
  if(get_dbl(line,n,15,&c->ZM)    < 0) rt = 1; n += 15;
  if(get_dbl(line,n,15,&c->P)     < 0) rt = 2; n += 15;
  if(get_dbl(line,n,15,&c->T)     < 0) rt = 3; n += 15;
  if(get_dbl(line,n,15,&c->WMOL1) < 0) rt = 4; n += 15;
  if(get_dbl(line,n,15,&c->WMOL2) < 0) rt = 5; n += 15;
  if(get_dbl(line,n,15,&c->WMOL3) < 0) rt = 6; n += 15;
  #endif
  for(i=0; i<CARD2C1_JCHAR_NINDX; i++)
  {
    if(get_upp(line,n,1,c->JCHAR[i]) < 0) rt = 7; n += 1;
  }
  n += 1;
  if(get_upp(line,n,1,c->JCHARX) < 0) rt = 8;
  if(rt)
  {
    fprintf(stderr,"CARD2C1_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  for(n=0; n<CARD2C1_NPAR; n++)
  {
    if(CARD2C1_check(c,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD2C1_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD2C1_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int ncol;
  int idx1 = 0;
  int idx2 = 0;
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
  if(strcmp(CARD2C1_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(ncol < NCOL4)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2C1_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2C1_name[i]) == 0)
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
    case 4:
    case 5:
      errno = 0;
      idx1 = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C1: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx1<0 || idx1>=LAYDIM)
      {
        fprintf(stderr,"CARD2C1: index 1 out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[3],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 6:
      if(ncol < NCOL5)
      {
        return 0;
      }
      errno = 0;
      idx1 = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C1: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx1<0 || idx1>=LAYDIM)
      {
        fprintf(stderr,"CARD2C1: index 1 out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      idx2 = strtol(str[3],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C1: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx2<0 || idx2>=CARD2C1_JCHAR_NINDX)
      {
        fprintf(stderr,"CARD2C1: index 2 out of range >>> %s\n",str[3]);
        return -1;
      }
      break;
    case 7:
      errno = 0;
      idx1 = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C1: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx1<0 || idx1>=LAYDIM)
      {
        fprintf(stderr,"CARD2C1: index 1 out of range >>> %s\n",str[2]);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      ((t->card2c1)+idx1)->ZM = xtmp;
      break;
    case 1:
      ((t->card2c1)+idx1)->P = xtmp;
      break;
    case 2:
      ((t->card2c1)+idx1)->T = xtmp;
      break;
    case 3:
      ((t->card2c1)+idx1)->WMOL1 = xtmp;
      break;
    case 4:
      ((t->card2c1)+idx1)->WMOL2 = xtmp;
      break;
    case 5:
      ((t->card2c1)+idx1)->WMOL3 = xtmp;
      break;
    case 6:
      strncpy(((t->card2c1)+idx1)->JCHAR[idx2],str[4],sizeof(((t->card2c1)+idx1)->JCHAR[idx2]));
      break;
    case 7:
      strncpy(((t->card2c1)+idx1)->JCHARX,str[3],sizeof(((t->card2c1)+idx1)->JCHARX));
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2C1_check((t->card2c1)+idx1,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2C1_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD2C1_NPAR; n++)
  {
    if(CARD2C1_check(t->card2c1,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD2C1_check(const CARD2C1_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD2C1_name[n],c->MODTRN);
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

char *CARD2C1_to_s(const CARD2C1_struct *c,int n,char *s)
{
  switch(n)
  {
    #ifdef OLDMODE
    case 0:
      sprintf(s,"%.3f",c->ZM);
      break;
    case 1:
      sprintf(s,"%.3e",c->P);
      break;
    case 2:
      sprintf(s,"%.3e",c->T);
      break;
    case 3:
      sprintf(s,"%.3e",c->WMOL1);
      break;
    case 4:
      sprintf(s,"%.3e",c->WMOL2);
      break;
    case 5:
      sprintf(s,"%.3e",c->WMOL3);
      break;
    case 7:
      sprintf(s,"%s",c->JCHARX);
      break;
    default:
      sprintf(s,"?");
      break;
    #else
    case 0:
      sprintf(s,"%.8f",c->ZM);
      break;
    case 1:
      sprintf(s,"%.8e",c->P);
      break;
    case 2:
      sprintf(s,"%.8e",c->T);
      break;
    case 3:
      sprintf(s,"%.8e",c->WMOL1);
      break;
    case 4:
      sprintf(s,"%.8e",c->WMOL2);
      break;
    case 5:
      sprintf(s,"%.8e",c->WMOL3);
      break;
    case 7:
      sprintf(s,"%s",c->JCHARX);
      break;
    default:
      sprintf(s,"?");
      break;
    #endif
  }

  return s;
}
