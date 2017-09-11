// CARD 2C3 parameters
char	CARD2C3_ID[5]			= "2C3";

char	CARD2C3_comment[MAXLINE]	= "If IRD2 = 1 or IRD2 = 2";

char	CARD2C3_name[CARD2C3_NPAR][10]	=
{
  "AHAZE",
  "EQLWCZ",
  "RRATZ",
  "IHA1",
  "ICLD1",
  "IVUL1",
  "ISEA1",
  "ICHR",
};

CARD2C3_struct CARD2C3_default		=
{
  {NAN,NAN,NAN,NAN},
  NAN,
  NAN,
  0,
  0,
  0,
  0,
  0,
};

char CARD2C3_description[CARD2C3_NPAR][MAXLINE] =
{
  "aerosol or cloud scaling factor",
  "equivalent liquid water content (g/m3)",
  "rain rate (mm/h)",
  "range control",
  "cloud extinction control",
  "profile and extinction control",
  "aerosol season control",
  "boundary change",
};

void CARD2C3_write(const TAPE5_struct *t,int i,FILE *fp)
{
  CARD2C3_struct *c;

  c = (t->card2c3)+i;

  if(t->card2c->IRD2 == 2)
  {
    #ifdef OLDMODE
    fprintf(fp,"%10s%10.3f%10s%10.3f%10.3f%10.3f%10.3f\n","",c->AHAZE[0],"",
                c->RRATZ,c->AHAZE[1],c->AHAZE[2],c->AHAZE[3]);
    #else
    fprintf(fp,"%10s%15.8f%10s%15.8f%15.8f%15.8f%15.8f\n","",c->AHAZE[0],"",
                c->RRATZ,c->AHAZE[1],c->AHAZE[2],c->AHAZE[3]);
    #endif
  }
  else
  {
    fprintf(fp,"%10s%10.3f%10.3f%10.3f%5d%5d%5d%5d%5d\n","",
                c->AHAZE[0],c->EQLWCZ,c->RRATZ,c->IHA1,
                c->ICLD1,c->IVUL1,c->ISEA1,c->ICHR);
  }
}

void CARD2C3_print(const TAPE5_struct *t,int i,FILE *fp)
{
  int j,n;
  char s[MAXLINE];
  CARD2C3_struct *c;

  c = (t->card2c3)+i;

  if(t->card2c->IRD2 == 2)
  {
    #ifdef OLDMODE
    n = 0;
    for(j=0; j<NAERO; j++)
    {
      fprintf(fp,"%-4s %-10s %-3d %-3d %-15.3f # %s\n",CARD2C3_ID,CARD2C3_name[n],i,j,c->AHAZE[j],CARD2C3_description[n]);
    }
    n = 2;
    fprintf(fp,"%-4s %-10s %-7d %-15s # %s\n",CARD2C3_ID,CARD2C3_name[n],i,CARD2C3_to_s(c,n,s),CARD2C3_description[n]);
    #else
    n = 0;
    for(j=0; j<NAERO; j++)
    {
      fprintf(fp,"%-4s %-10s %-3d %-3d %-15.8f # %s\n",CARD2C3_ID,CARD2C3_name[n],i,j,c->AHAZE[j],CARD2C3_description[n]);
    }
    n = 2;
    fprintf(fp,"%-4s %-10s %-7d %-15.8f # %s\n",CARD2C3_ID,CARD2C3_name[n],i,c->RRATZ,CARD2C3_description[n]);
    #endif
  }
  else
  {
    for(n=0; n<CARD2C3_NPAR; n++)
    {
      fprintf(fp,"%-4s %-10s %-7d %-15s # %s\n",CARD2C3_ID,CARD2C3_name[n],i,CARD2C3_to_s(c,n,s),CARD2C3_description[n]);
    }
  }
}

int CARD2C3_read(TAPE5_struct *t,int i,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];
  CARD2C3_struct *c;

  c = (t->card2c3)+i;

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2C3_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(t->card2c->IRD2 == 2)
  {
    #ifdef OLDMODE
    if(get_dbl(line,n,20,&c->AHAZE[0])< 0) rt = 1; n += 20;
    if(get_dbl(line,n,20,&c->RRATZ)   < 0) rt = 2; n += 20;
    if(get_dbl(line,n,10,&c->AHAZE[1])< 0) rt = 3; n += 10;
    if(get_dbl(line,n,10,&c->AHAZE[2])< 0) rt = 4; n += 10;
    if(get_dbl(line,n,10,&c->AHAZE[3])< 0) rt = 5;
    #else
    if(get_dbl(line,n,25,&c->AHAZE[0])< 0) rt = 1; n += 25;
    if(get_dbl(line,n,25,&c->RRATZ)   < 0) rt = 2; n += 25;
    if(get_dbl(line,n,15,&c->AHAZE[1])< 0) rt = 3; n += 15;
    if(get_dbl(line,n,15,&c->AHAZE[2])< 0) rt = 4; n += 15;
    if(get_dbl(line,n,15,&c->AHAZE[3])< 0) rt = 5;
    #endif
  }
  else
  {
    if(get_dbl(line,n,20,&c->AHAZE[0])< 0) rt = 1; n += 20;
    if(get_dbl(line,n,10,&c->EQLWCZ)  < 0) rt = 2; n += 10;
    if(get_dbl(line,n,10,&c->RRATZ)   < 0) rt = 3; n += 10;
    if(get_int(line,n,5,&c->IHA1)     < 0) rt = 4; n += 5;
    if(get_int(line,n,5,&c->ICLD1)    < 0) rt = 5; n += 5;
    if(get_int(line,n,5,&c->IVUL1)    < 0) rt = 6; n += 5;
    if(get_int(line,n,5,&c->ISEA1)    < 0) rt = 7; n += 5;
    if(get_int(line,n,5,&c->ICHR)     < 0) rt = 8;
  }
  if(rt)
  {
    fprintf(stderr,"CARD2C3_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  for(n=0; n<CARD2C3_NPAR; n++)
  {
    if(CARD2C3_check(c,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD2C3_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD2C3_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int idx1 = 0;
  int idx2 = 0;
  int ntmp = 0;
  double xtmp = NAN;
  char str[NCOL5][MAXLINE];
  char *p;

  // read line
  for(i=0,p=line; i<NCOL5; i++,p+=n)
  {
    if(sscanf(p,"%s%n",str[i],&n) == EOF) break;
  }
  if(i < 2)
  {
    return -1;
  }
  if(strcmp(CARD2C3_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL4)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2C3_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2C3_name[i]) == 0)
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
    case 5:
    case 6:
    case 7:
      errno = 0;
      idx1 = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C3: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx1<0 || idx1>=LAYDIM)
      {
        fprintf(stderr,"CARD2C3: index out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      ntmp = strtol(str[3],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C3: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 0:
      if(t->card2c->IRD2 == 2)
      {
        errno = 0;
        idx1 = strtol(str[2],&p,10);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"CARD2C3: Convert error >>> %s\n",line);
          return -1;
        }
        if(idx1<0 || idx1>=LAYDIM)
        {
          fprintf(stderr,"CARD2C3: index out of range >>> %s\n",str[2]);
          return -1;
        }
        errno = 0;
        idx2 = strtol(str[3],&p,10);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"CARD2C3: Convert error >>> %s\n",line);
          return -1;
        }
        if(idx2<0 || idx2>=NAERO)
        {
          fprintf(stderr,"CARD2C3: index out of range >>> %s\n",str[3]);
          return -1;
        }
        errno = 0;
        xtmp = strtod(str[4],&p);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"CARD2C3: Convert error >>> %s\n",line);
          return -1;
        }
      }
      else
      {
        errno = 0;
        idx1 = strtol(str[2],&p,10);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"CARD2C3: Convert error >>> %s\n",line);
          return -1;
        }
        if(idx1<0 || idx1>=LAYDIM)
        {
          fprintf(stderr,"CARD2C3: index out of range >>> %s\n",str[2]);
          return -1;
        }
        errno = 0;
        xtmp = strtod(str[3],&p);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"CARD2C3: Convert error >>> %s\n",line);
          return -1;
        }
      }
      break;
    case 1:
    case 2:
      errno = 0;
      idx1 = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C3: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx1<0 || idx1>=LAYDIM)
      {
        fprintf(stderr,"CARD2C3: index out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[3],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C3: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      if(t->card2c->IRD2 == 2)
      {
        ((t->card2c3)+idx1)->AHAZE[idx2] = xtmp;
      }
      else
      {
        ((t->card2c3)+idx1)->AHAZE[0] = xtmp;
      }
      break;
    case 1:
      ((t->card2c3)+idx1)->EQLWCZ = xtmp;
      break;
    case 2:
      ((t->card2c3)+idx1)->RRATZ = xtmp;
      break;
    case 3:
      ((t->card2c3)+idx1)->IHA1 = ntmp;
      break;
    case 4:
      ((t->card2c3)+idx1)->ICLD1 = ntmp;
      break;
    case 5:
      ((t->card2c3)+idx1)->IVUL1 = ntmp;
      break;
    case 6:
      ((t->card2c3)+idx1)->ISEA1 = ntmp;
      break;
    case 7:
      ((t->card2c3)+idx1)->ICHR = ntmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2C3_check((t->card2c3)+idx1,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2C3_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD2C3_NPAR; n++)
  {
    if(CARD2C3_check(t->card2c3,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD2C3_check(const CARD2C3_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD2C3_name[n],c->MODTRN);
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

char *CARD2C3_to_s(const CARD2C3_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%.3f",c->AHAZE[0]);
      break;
    case 1:
      sprintf(s,"%.3f",c->EQLWCZ);
      break;
    case 2:
      sprintf(s,"%.3f",c->RRATZ);
      break;
    case 3:
      sprintf(s,"%d",c->IHA1);
      break;
    case 4:
      sprintf(s,"%d",c->ICLD1);
      break;
    case 5:
      sprintf(s,"%d",c->IVUL1);
      break;
    case 6:
      sprintf(s,"%d",c->ISEA1);
      break;
    case 7:
      sprintf(s,"%d",c->ICHR);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
