// CARD 2A+ parameters
char	CARD2AP_ID[5]			= "2A+";

char	CARD2AP_comment[MAXLINE]	= "If APLUS = A+";

char	CARD2AP_name[CARD2AP_NPAR][10]	=
{
  "ZAER1",
  "ZAER2",
  "SCALE",
};

CARD2AP_struct CARD2AP_default		=
{
  {NAN,NAN,NAN,NAN},
  {NAN,NAN,NAN,NAN},
  {NAN,NAN,NAN,NAN},
};

char CARD2AP_description[CARD2AP_NPAR][MAXLINE] =
{
  "The base/bottom of aerosol i",
  "The top of aerosol i",
  "vertical profile scale",
};

void CARD2AP_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp," %9.0f %9.0f %9.0f%20s %9.0f %9.0f %9.0f\n", t->card2ap->ZAER1[0],t->card2ap->ZAER2[0],t->card2ap->SCALE[0],"",
                                                           t->card2ap->ZAER1[1],t->card2ap->ZAER2[1],t->card2ap->SCALE[1]);
  fprintf(fp," %9.0f %9.0f %9.0f%20s %9.0f %9.0f %9.0f\n", t->card2ap->ZAER1[2],t->card2ap->ZAER2[2],t->card2ap->SCALE[2],"",
                                                           t->card2ap->ZAER1[3],t->card2ap->ZAER2[3],t->card2ap->SCALE[3]);
}

void CARD2AP_print(const TAPE5_struct *t,FILE *fp)
{
  int i,n;

  for(i=0; i<NAERO; i++)
  {
    for(n=0; n<CARD2AP_NPAR; n++)
    {
      switch(n)
      {
        case 0:
          fprintf(fp,"%-4s %-10s %-7d %-15.0f # %s\n",CARD2AP_ID,CARD2AP_name[n],i,t->card2ap->ZAER1[i],CARD2AP_description[n]);
          break;
        case 1:
          fprintf(fp,"%-4s %-10s %-7d %-15.0f # %s\n",CARD2AP_ID,CARD2AP_name[n],i,t->card2ap->ZAER2[i],CARD2AP_description[n]);
          break;
        case 2:
          fprintf(fp,"%-4s %-10s %-7d %-15.0f # %s\n",CARD2AP_ID,CARD2AP_name[n],i,t->card2ap->SCALE[i],CARD2AP_description[n]);
          break;
        default:
          break;
      }
    }
  }
}

int CARD2AP_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt,er;
  char line[CARD_LENGTH];

  // initialization
  rt = 0;

  // read line 1
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2AP_read: read error.\n");
    return -1;
  }
  er = 0;

  // read values
  n = 1;
  if(get_dbl(line,n,9,&t->card2ap->ZAER1[0]) < 0) er = 1; n += 10;
  if(get_dbl(line,n,9,&t->card2ap->ZAER2[0]) < 0) er = 2; n += 10;
  if(get_dbl(line,n,9,&t->card2ap->SCALE[0]) < 0) er = 3; n += 30;
  if(get_dbl(line,n,9,&t->card2ap->ZAER1[1]) < 0) er = 4; n += 10;
  if(get_dbl(line,n,9,&t->card2ap->ZAER2[1]) < 0) er = 5; n += 10;
  if(get_dbl(line,n,9,&t->card2ap->SCALE[1]) < 0) er = 6;
  if(er)
  {
    fprintf(stderr,"CARD2AP_read: er=%d\n",er);
    fprintf(stderr,"CARD2AP_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
    rt = -1;
  }

  // read line 2
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2AP_read: read error.\n");
    return -1;
  }
  er = 0;

  // read values
  n = 1;
  if(get_dbl(line,n,9,&t->card2ap->ZAER1[2]) < 0) er =  7; n += 10;
  if(get_dbl(line,n,9,&t->card2ap->ZAER2[2]) < 0) er =  8; n += 10;
  if(get_dbl(line,n,9,&t->card2ap->SCALE[2]) < 0) er =  9; n += 30;
  if(get_dbl(line,n,9,&t->card2ap->ZAER1[3]) < 0) er = 10; n += 10;
  if(get_dbl(line,n,9,&t->card2ap->ZAER2[3]) < 0) er = 11; n += 10;
  if(get_dbl(line,n,9,&t->card2ap->SCALE[3]) < 0) er = 12;
  if(er)
  {
    fprintf(stderr,"CARD2AP_read: er=%d\n",er);
    fprintf(stderr,"CARD2AP_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
    rt = -1;
  }

  // check values
  er = 0;
  for(n=0; n<CARD2AP_NPAR; n++)
  {
    if(CARD2AP_check(t->card2ap,n) < 0)
    {
      er = -1;
    }
  }
  if(er)
  {
    fprintf(stderr,"CARD2AP_read: er=%d\n",er);
    rt = -1;
  }

  return rt;
}

int CARD2AP_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int ncol;
  int indx = 0;
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
  if(strcmp(CARD2AP_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(ncol < NCOL5)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2AP_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2AP_name[i]) == 0)
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
        fprintf(stderr,"CARD2AP: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=NAERO)
      {
        fprintf(stderr,"CARD2AP: index out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[3],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2AP: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      (t->card2ap)->ZAER1[indx] = xtmp;
      break;
    case 1:
      (t->card2ap)->ZAER2[indx] = xtmp;
      break;
    case 2:
      (t->card2ap)->SCALE[indx] = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2AP_check(t->card2ap,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2AP_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD2AP_NPAR; n++)
  {
    if(CARD2AP_check(t->card2ap,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD2AP_check(const CARD2AP_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD2AP_name[n],c->MODTRN);
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

char *CARD2AP_to_s(const CARD2AP_struct *c,int n,char *s)
{
  switch(n)
  {
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
