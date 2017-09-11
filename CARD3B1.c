// CARD 3B1 parameters
char	CARD3B1_ID[5]			= "3B1";

char	CARD3B1_comment[MAXLINE]	= "If IPH = 1";

char	CARD3B1_name[CARD3B1_NPAR][10]	=
{
  "NANGLS",
  "NWLF",
};

CARD3B1_struct CARD3B1_default		=
{
  0,
  0,
};

char CARD3B1_description[CARD3B1_NPAR][MAXLINE] =
{
  "number of angles",
  "number of wavelength",
};

void CARD3B1_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp,"%5d%5d\n",t->card3b1->NANGLS,t->card3b1->NWLF);
}

void CARD3B1_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD3B1_NPAR; n++)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3B1_ID,CARD3B1_name[n],CARD3B1_to_s(t->card3b1,n,s),CARD3B1_description[n]);
  }
}

int CARD3B1_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD3B1_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_int(line,n,5,&t->card3b1->NANGLS) < 0) rt = 1; n += 5;
  if(get_int(line,n,5,&t->card3b1->NWLF)   < 0) rt = 2;
  if(rt)
  {
    fprintf(stderr,"CARD3B1_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD3B1_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD3B1_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD3B1_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int ntmp;
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
  if(strcmp(CARD3B1_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD3B1_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD3B1_name[i]) == 0)
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
      errno = 0;
      ntmp = strtol(str[2],&p,10);
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
      t->card3b1->NANGLS = ntmp;
      break;
    case 1:
      t->card3b1->NWLF = ntmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD3B1_check(t->card3b1,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD3B1_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD3B1_NPAR; n++)
  {
    if(CARD3B1_check(t->card3b1,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD3B1_check(const CARD3B1_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
    case 0:
      if(c->NANGLS<1 || c->NANGLS>MXANGL)
      {
        fprintf(stderr,"CARD3B1: %s out of range >>> %d\n",CARD3B1_name[n],c->NANGLS);
        rt = -1;
      }
      break;
    case 1:
      if(c->NWLF<1 || c->NWLF>MXWVLN)
      {
        fprintf(stderr,"CARD3B1: %s out of range >>> %d\n",CARD3B1_name[n],c->NWLF);
        rt = -1;
      }
      break;
    default:
      rt = -1;
      break;
  }

  return rt;
}

char *CARD3B1_to_s(const CARD3B1_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%d",c->NANGLS);
      break;
    case 1:
      sprintf(s,"%d",c->NWLF);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
