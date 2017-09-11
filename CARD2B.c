// CARD 2B parameters
char	CARD2B_ID[5]			= "2B";

char	CARD2B_comment[MAXLINE]		= "If IVSA = 1";

char	CARD2B_name[CARD2B_NPAR][10]	=
{
  "ZCVSA",
  "ZTVSA",
  "ZINVSA",
};

CARD2B_struct CARD2B_default		=
{
  NAN,
  NAN,
  NAN,
};

char CARD2B_description[CARD2B_NPAR][MAXLINE] =
{
  "cloud ceiling height (km)",
  "thickness of the cloud (km)",
  "height of the inversion layer (km)",
};

void CARD2B_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp,"%10.3f%10.3f%10.3f\n",t->card2b->ZCVSA,t->card2b->ZTVSA,t->card2b->ZINVSA);
}

void CARD2B_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD2B_NPAR; n++)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD2B_ID,CARD2B_name[n],CARD2B_to_s(t->card2b,n,s),CARD2B_description[n]);
  }
}

int CARD2B_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2B_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_dbl(line,n,10,&t->card2b->ZCVSA)  < 0) rt = 1; n += 10;
  if(get_dbl(line,n,10,&t->card2b->ZTVSA)  < 0) rt = 2; n += 10;
  if(get_dbl(line,n,10,&t->card2b->ZINVSA) < 0) rt = 3;
  if(rt)
  {
    fprintf(stderr,"CARD2B_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD2B_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD2B_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD2B_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  double xtmp;
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
  if(strcmp(CARD2B_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2B_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2B_name[i]) == 0)
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
      t->card2b->ZCVSA = xtmp;
      break;
    case 1:
      t->card2b->ZTVSA = xtmp;
      break;
    case 2:
      t->card2b->ZINVSA = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2B_check(t->card2b,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2B_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD2B_NPAR; n++)
  {
    if(CARD2B_check(t->card2b,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD2B_check(const CARD2B_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD2B_name[n],c->MODTRN);
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

char *CARD2B_to_s(const CARD2B_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%.3f",c->ZCVSA);
      break;
    case 1:
      sprintf(s,"%.3f",c->ZTVSA);
      break;
    case 2:
      sprintf(s,"%.3f",c->ZINVSA);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
