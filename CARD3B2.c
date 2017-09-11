// CARD 3B2 parameters
char	CARD3B2_ID[5]			= "3B2";

char	CARD3B2_comment[MAXLINE]	= "If IPH = 1 and NWLF = 0";

char	CARD3B2_name[CARD3B2_NPAR][10]	=
{
  "ANGF",
  "F1",
  "F2",
  "F3",
  "F4",
};

CARD3B2_struct CARD3B2_default		=
{
  NAN,
  NAN,
  NAN,
  NAN,
  NAN,
};

char CARD3B2_description[CARD3B2_NPAR][MAXLINE] =
{
  "scattering angle (deg)",
  "scattering phase function at boundary layer",
  "scattering phase function at troposphere",
  "scattering phase function at stratosphere",
  "scattering phase function at mesosphere",
};

void CARD3B2_write(const TAPE5_struct *t,FILE *fp)
{
  int i;

  for(i=0; i<t->card3b1->NANGLS; i++)
  {
    fprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e\n",((t->card3b2)+i)->ANGF,
                ((t->card3b2)+i)->F1,((t->card3b2)+i)->F2,((t->card3b2)+i)->F3,((t->card3b2)+i)->F4);
  }
}

void CARD3B2_print(const TAPE5_struct *t,FILE *fp)
{
  int i,n;
  char s[MAXLINE];

  for(i=0; i<t->card3b1->NANGLS; i++)
  {
    for(n=0; n<CARD3B2_NPAR; n++)
    {
      fprintf(fp,"%-4s %-10s %-7d %-15s # %s\n",CARD3B2_ID,CARD3B2_name[n],i,CARD3B2_to_s((t->card3b2)+i,n,s),CARD3B2_description[n]);
    }
  }
}

int CARD3B2_read(TAPE5_struct *t,FILE *fp)
{
  int i,n;
  int rt,er;
  char line[CARD_LENGTH];

  // initialization
  rt = 0;

  for(i=0; i<t->card3b1->NANGLS; i++)
  {
    // read line
    if(fgets(line,CARD_LENGTH,fp) == NULL)
    {
      fprintf(stderr,"CARD3B2_read: read error.\n");
      return -1;
    }
    er = 0;
    // read values
    n = 0;
    if(get_dbl(line,n,10,&((t->card3b2)+i)->ANGF) < 0) er = 1; n += 10;
    if(get_dbl(line,n,10,&((t->card3b2)+i)->F1)   < 0) er = 2; n += 10;
    if(get_dbl(line,n,10,&((t->card3b2)+i)->F2)   < 0) er = 3; n += 10;
    if(get_dbl(line,n,10,&((t->card3b2)+i)->F3)   < 0) er = 4; n += 10;
    if(get_dbl(line,n,10,&((t->card3b2)+i)->F4)   < 0) er = 5;
    // check values
    for(n=0; n<CARD3B2_NPAR; n++)
    {
      if(CARD3B2_check((t->card3b2)+i,n) < 0)
      {
        er = 6;
      }
    }
    // check error
    if(er)
    {
      fprintf(stderr,"CARD3B2_read: er=%d\n",er);
      fprintf(stderr,"CARD3B2_read: error in the following line.\n");
      fprintf(stderr,"%s\n",line);
      rt = -1;
    }
  }

  return rt;
}

int CARD3B2_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD3B2_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(ncol < NCOL4)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD3B2_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD3B2_name[i]) == 0)
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
      errno = 0;
      indx = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD3B2: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=MXANGL)
      {
        fprintf(stderr,"CARD3B2: index out of range >>> %s\n",str[2]);
        return -1;
      }
      break;
      errno = 0;
      xtmp = strtod(str[3],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD3B2: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      ((t->card3b2)+indx)->ANGF = xtmp;
      break;
    case 1:
      ((t->card3b2)+indx)->F1 = xtmp;
      break;
    case 2:
      ((t->card3b2)+indx)->F2 = xtmp;
      break;
    case 3:
      ((t->card3b2)+indx)->F3 = xtmp;
      break;
    case 4:
      ((t->card3b2)+indx)->F4 = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD3B2_check((t->card3b2)+indx,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD3B2_check_all(const TAPE5_struct *t)
{
  int i,n;
  int rt;

  rt = 0;
  for(i=0; i<t->card3b1->NANGLS; i++)
  {
    for(n=0; n<CARD3B2_NPAR; n++)
    {
      if(CARD3B2_check((t->card3b2)+i,n) < 0)
      {
        rt = -1;
      }
    }
  }

  return rt;
}

int CARD3B2_check(const CARD3B2_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD3B2: error in %s, %s\n",CARD3B2_name[n],c->MODTRN);
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

char *CARD3B2_to_s(const CARD3B2_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%.3e",c->ANGF);
      break;
    case 1:
      sprintf(s,"%.3e",c->F1);
      break;
    case 2:
      sprintf(s,"%.3e",c->F2);
      break;
    case 3:
      sprintf(s,"%.3e",c->F3);
      break;
    case 4:
      sprintf(s,"%.3e",c->F4);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
