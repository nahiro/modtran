// CARD 2E1 parameters
char	CARD2E1_ID[5]			= "2E1";

char	CARD2E1_comment[MAXLINE]	= "If ICLD = 1 - 10, NCRALT >= 3";

char	CARD2E1_name[CARD2E1_NPAR][10]	=
{
  "ZCLD",
  "CLD",
  "CLDICE",
  "RR",
};

CARD2E1_struct CARD2E1_default		=
{
  NAN,
  NAN,
  NAN,
  NAN,
};

char CARD2E1_description[CARD2E1_NPAR][MAXLINE] =
{
  "altitude of layer boundary (km)",
  "WD density (g/m3)",
  "IP density (g/m3)",
  "rain rate (mm/h)",
};

void CARD2E1_write(const TAPE5_struct *t,FILE *fp)
{
  int i;

  for(i=0; i<t->card2a->NCRALT; i++)
  {
    fprintf(fp,"%10.5f%10.5f%10.5f%10.5f\n",((t->card2e1)+i)->ZCLD,((t->card2e1)+i)->CLD,((t->card2e1)+i)->CLDICE,((t->card2e1)+i)->RR);
  }
}

void CARD2E1_print(const TAPE5_struct *t,FILE *fp)
{
  int i,n;
  char s[MAXLINE];

  for(i=0; i<t->card2a->NCRALT; i++)
  {
    for(n=0; n<CARD2E1_NPAR; n++)
    {
      fprintf(fp,"%-4s %-10s %-7d %-15s # %s\n",CARD2E1_ID,CARD2E1_name[n],i,CARD2E1_to_s((t->card2e1)+i,n,s),CARD2E1_description[n]);
    }
  }
}

int CARD2E1_read(TAPE5_struct *t,FILE *fp)
{
  int i,n;
  int rt,er;
  char line[CARD_LENGTH];

  // initialization
  rt = 0;

  for(i=0; i<t->card2a->NCRALT; i++)
  {
    // read line
    if(fgets(line,CARD_LENGTH,fp) == NULL)
    {
      fprintf(stderr,"CARD2E1_read: read error.\n");
      return -1;
    }
    er = 0;
    // read values
    n = 0;
    if(get_dbl(line,n,10,&((t->card2e1)+i)->ZCLD)   < 0) er = 1; n += 10;
    if(get_dbl(line,n,10,&((t->card2e1)+i)->CLD)    < 0) er = 2; n += 10;
    if(get_dbl(line,n,10,&((t->card2e1)+i)->CLDICE) < 0) er = 3; n += 10;
    if(get_dbl(line,n,10,&((t->card2e1)+i)->RR)     < 0) er = 4;
    // check values
    for(n=0; n<CARD2E1_NPAR; n++)
    {
      if(CARD2E1_check((t->card2e1)+i,n) < 0)
      {
        er = 5;
      }
    }
    // check error
    if(er)
    {
      fprintf(stderr,"CARD2E1_read: er=%d\n",er);
      fprintf(stderr,"CARD2E1_read: error in the following line.\n");
      fprintf(stderr,"%s\n",line);
      rt = -1;
    }
  }

  return rt;
}

int CARD2E1_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int ncol;
  int indx;
  double xtmp = NAN;
  char str[NCOL4][MAXLINE];
  char *p;

  // read line
  for(i=0,p=line; i<NCOL4; i++,p+=n)
  {
    if(sscanf(p,"%s%n",str[i],&n) == EOF) break;
  }
  ncol = i;
  if(ncol < 2)
  {
    return -1;
  }
  if(strcmp(CARD2E1_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(ncol < NCOL4)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2E1_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2E1_name[i]) == 0)
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
      indx = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2E1: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=NZCLD)
      {
        fprintf(stderr,"CARD2E1: index out of range >>> %s\n",str[2]);
        return -1;
      }
      break;
      errno = 0;
      xtmp = strtod(str[3],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2E1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      ((t->card2e1)+indx)->ZCLD = xtmp;
      break;
    case 1:
      ((t->card2e1)+indx)->CLD = xtmp;
      break;
    case 2:
      ((t->card2e1)+indx)->CLDICE = xtmp;
      break;
    case 3:
      ((t->card2e1)+indx)->RR = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2E1_check((t->card2e1)+indx,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2E1_check_all(const TAPE5_struct *t)
{
  int i,n;
  int rt;

  rt = 0;
  for(i=0; i<t->card2a->NCRALT; i++)
  {
    for(n=0; n<CARD2E1_NPAR; n++)
    {
      if(CARD2E1_check((t->card2e1)+i,n) < 0)
      {
        rt = -1;
      }
    }
  }

  return rt;
}

int CARD2E1_check(const CARD2E1_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD2E1: error in %s, %s\n",CARD2E1_name[n],c->MODTRN);
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

char *CARD2E1_to_s(const CARD2E1_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%.5f",c->ZCLD);
      break;
    case 1:
      sprintf(s,"%.5f",c->CLD);
      break;
    case 2:
      sprintf(s,"%.5f",c->CLDICE);
      break;
    case 3:
      sprintf(s,"%.5f",c->RR);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
