// CARD 2C parameters
char	CARD2C_ID[5]			= "2C";

char	CARD2C_comment[MAXLINE]		= "If MODEL = 0, 7 or 8, and IM = 1";

char	CARD2C_name[CARD2C_NPAR][10]	=
{
  "ML",
  "IRD1",
  "IRD2",
  "HMODEL",
  "REE",
};

CARD2C_struct CARD2C_default		=
{
  0,
  0,
  0,
  "?",
  NAN,
};

char CARD2C_description[CARD2C_NPAR][MAXLINE] =
{
  "number of atmospheric levels",
  "controls reading of WMOL (4-12)",
  "controls reading CARD 2C3",
  "identification of new model atmosphere",
  "earth radius (km)",
};

void CARD2C_write(const TAPE5_struct *t,FILE *fp)
{
  if(t->card1->MODEL == 8)
  {
    fprintf(fp,"%5d%5d%5d%20s%10.0f\n",t->card2c->ML,t->card2c->IRD1,t->card2c->IRD2,t->card2c->HMODEL,t->card2c->REE);
  }
  else
  {
    fprintf(fp,"%5d%5d%5d%20s\n",t->card2c->ML,t->card2c->IRD1,t->card2c->IRD2,t->card2c->HMODEL);
  }
}

void CARD2C_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  if(t->card1->MODEL == 8)
  {
    for(n=0; n<CARD2C_NPAR; n++)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD2C_ID,CARD2C_name[n],CARD2C_to_s(t->card2c,n,s),CARD2C_description[n]);
    }
  }
  else
  {
    for(n=0; n<CARD2C_NPAR-1; n++)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD2C_ID,CARD2C_name[n],CARD2C_to_s(t->card2c,n,s),CARD2C_description[n]);
    }
  }
}

int CARD2C_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2C_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_int(line,n,5,&t->card2c->ML)     < 0) rt = 1; n += 5;
  if(get_int(line,n,5,&t->card2c->IRD1)   < 0) rt = 2; n += 5;
  if(get_int(line,n,5,&t->card2c->IRD2)   < 0) rt = 3; n += 5;
  if(get_upp(line,n,20,t->card2c->HMODEL) < 0) rt = 4; n += 20;
  if(t->card1->MODEL == 8)
  {
    if(get_dbl(line,n,10,&t->card2c->REE)   < 0) rt = 5;
  }
  if(rt)
  {
    fprintf(stderr,"CARD2C_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD2C_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD2C_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD2C_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD2C_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2C_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2C_name[i]) == 0)
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
      ntmp = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 4:
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
      t->card2c->ML = ntmp;
      break;
    case 1:
      t->card2c->IRD1 = ntmp;
      break;
    case 2:
      t->card2c->IRD2 = ntmp;
      break;
    case 3:
      strncpy(t->card2c->HMODEL,str[2],sizeof(t->card2c->HMODEL));
      break;
    case 4:
      t->card2c->REE = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2C_check(t->card2c,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2C_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD2C_NPAR; n++)
  {
    if(CARD2C_check(t->card2c,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD2C_check(const CARD2C_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD2C_name[n],c->MODTRN);
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

char *CARD2C_to_s(const CARD2C_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%d",c->ML);
      break;
    case 1:
      sprintf(s,"%d",c->IRD1);
      break;
    case 2:
      sprintf(s,"%d",c->IRD2);
      break;
    case 3:
      sprintf(s,"%s",c->HMODEL);
      break;
    case 4:
      sprintf(s,"%.0f",c->REE);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
