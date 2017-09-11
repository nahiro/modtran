// CARD 4 parameters
char	CARD4_ID[5]			= "4";

char	CARD4_comment[MAXLINE]		= "required";

char	CARD4_name[CARD4_NPAR][10]	=
{
  "V1",
  "V2",
  "DV",
  "FWHM",
  "YFLAG",
  "XFLAG",
  "DLIMIT",
  "FLAGS",
  "MLFLX",
};

CARD4_struct CARD4_default		=
{
  9523.80952,
  28571.42857,
  1.0,
  2.0,
  "R",
  "N",
  " ",
  " ",
  0,
};

char CARD4_description[CARD4_NPAR][MAXLINE] =
{
  "initial frequency (cm-1) or wavelength",
  "final frequency (cm-1) or wavelength",
  "frequency or wavelength increment",
  "slit function FWHM",
  "output transmittances or radiances",
  "units for output files",
  "separate character string",
  "flag made of seven characters",
  "number of atmospheric levels",
};

void CARD4_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp,"%10.0f%10.0f%10.0f%10.0f%1s%1s%8s%7s%3d\n",
              t->card4->V1,t->card4->V2,t->card4->DV,t->card4->FWHM,t->card4->YFLAG,t->card4->XFLAG,
              t->card4->DLIMIT,t->card4->FLAGS,t->card4->MLFLX);
}

void CARD4_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD4_NPAR; n++)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD4_ID,CARD4_name[n],CARD4_to_s(t->card4,n,s),CARD4_description[n]);
  }
}

int CARD4_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD4_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_dbl(line,n,10,&t->card4->V1)   < 0) rt = 1; n += 10;
  if(get_dbl(line,n,10,&t->card4->V2)   < 0) rt = 2; n += 10;
  if(get_dbl(line,n,10,&t->card4->DV)   < 0) rt = 3; n += 10;
  if(get_dbl(line,n,10,&t->card4->FWHM) < 0) rt = 4; n += 10;
  if(get_upp(line,n,1,t->card4->YFLAG)  < 0) rt = 5; n += 1;
  if(get_upp(line,n,1,t->card4->XFLAG)  < 0) rt = 6; n += 1;
  if(get_upp(line,n,8,t->card4->DLIMIT) < 0) rt = 7; n += 8;
  if(get_upp(line,n,7,t->card4->FLAGS)  < 0) rt = 8; n += 7;
  if(get_int(line,n,3,&t->card4->MLFLX) < 0) rt = 9;
  if(rt)
  {
    fprintf(stderr,"CARD4_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD4_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD4_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD4_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD4_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD4_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD4_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // convert value if required
  switch(n)
  {
    case 8:
      errno = 0;
      ntmp = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 0:
    case 1:
    case 2:
    case 3:
      errno = 0;
      xtmp = strtod(str[2],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD4: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      t->card4->V1 = xtmp;
      break;
    case 1:
      t->card4->V2 = xtmp;
      break;
    case 2:
      t->card4->DV = xtmp;
      break;
    case 3:
      t->card4->FWHM = xtmp;
      break;
    case 4:
      strncpy(t->card4->YFLAG,str[2],sizeof(t->card4->YFLAG));
      break;
    case 5:
      strncpy(t->card4->XFLAG,str[2],sizeof(t->card4->XFLAG));
      break;
    case 6:
      strncpy(t->card4->DLIMIT,str[2],sizeof(t->card4->DLIMIT));
      break;
    case 7:
      strncpy(t->card4->FLAGS,str[2],sizeof(t->card4->FLAGS));
      break;
    case 8:
      t->card4->MLFLX = ntmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD4_check(t->card4,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD4_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD4_NPAR; n++)
  {
    if(CARD4_check(t->card4,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD4_check(const CARD4_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD4: error in %s, %s\n",CARD4_name[n],c->MODTRN);
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

char *CARD4_to_s(const CARD4_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%.4f",c->V1);
      break;
    case 1:
      sprintf(s,"%.4f",c->V2);
      break;
    case 2:
      sprintf(s,"%.1f",c->DV);
      break;
    case 3:
      sprintf(s,"%.1f",c->FWHM);
      break;
    case 4:
      sprintf(s,"%s",c->YFLAG);
      break;
    case 5:
      sprintf(s,"%s",c->XFLAG);
      break;
    case 6:
      sprintf(s,"%s",c->DLIMIT);
      break;
    case 7:
      sprintf(s,"%s",c->FLAGS);
      break;
    case 8:
      sprintf(s,"%d",c->MLFLX);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
