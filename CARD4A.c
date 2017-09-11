// CARD 4A parameters
char	CARD4A_ID[5]			= "4A";

char	CARD4A_comment[MAXLINE]		= "If SURREF = 'BRDF' or 'LAMBER'";

char	CARD4A_name[CARD4A_NPAR][10]	=
{
  "NSURF",
  "AATEMP",
};

CARD4A_struct CARD4A_default		=
{
  0,
  NAN,
};

char CARD4A_description[CARD4A_NPAR][MAXLINE] =
{
  "reflectance profiles",
  "area-averaged ground surface temperature",
};

void CARD4A_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp,"%1d%9.0f\n",t->card4a->NSURF,t->card4a->AATEMP);
}

void CARD4A_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD4A_NPAR; n++)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD4A_ID,CARD4A_name[n],CARD4A_to_s(t->card4a,n,s),CARD4A_description[n]);
  }
}

int CARD4A_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD4A_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_int(line,n,1,&t->card4a->NSURF)  < 0) rt = 1; n += 1;
  if(get_dbl(line,n,9,&t->card4a->AATEMP) < 0) rt = 2;
  if(rt)
  {
    fprintf(stderr,"CARD4A_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD4A_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD4A_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD4A_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD4A_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD4A_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD4A_name[i]) == 0)
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
      ntmp = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 1:
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
      t->card4a->NSURF = ntmp;
      break;
    case 1:
      t->card4a->AATEMP = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD4A_check(t->card4a,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD4A_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD4A_NPAR; n++)
  {
    if(CARD4A_check(t->card4a,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD4A_check(const CARD4A_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD4A_name[n],c->MODTRN);
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

char *CARD4A_to_s(const CARD4A_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%d",c->NSURF);
      break;
    case 1:
      sprintf(s,"%.3f",c->AATEMP);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
