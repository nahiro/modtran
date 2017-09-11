// CARD 1 parameters
char	CARD1_ID[5]			= "1";

char	CARD1_comment[MAXLINE]		= "required";

char	CARD1_name[CARD1_NPAR][10]	=
{
  "MODTRN",
  "SPEED",
  "MODEL",
  "ITYPE",
  "IEMSCT",
  "IMULT",
  "M1",
  "M2",
  "M3",
  "M4",
  "M5",
  "M6",
  "MDEF",
  "IM",
  "NOPRNT",
  "TPTEMP",
  "SURREF",
};

CARD1_struct CARD1_default		=
{
  "T",
  "M",
  2,
  2,
  2,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  0,
  0.0,
  "0.0",
};

char CARD1_description[CARD1_NPAR][MAXLINE] =
{
  "band model algorithm",
  "calculation speed",
  "atmosphere model",
  "type of atmospheric line-of-sight path",
  "mode of execution of the program",
  "inclusion of multiple scattering",
  "altitude profile of temperature & pressure",
  "altitude profile of H2O",
  "altitude profile of O3",
  "altitude profile of CH4",
  "altitude profile of N2O",
  "altitude profile of CO",
  "altitude profile of CO2, O2, etc.",
  "read user input data",
  "tape6 and tape8 output control",
  "boundary temperature (K)",
  "surface albedo",
};

void CARD1_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp,"%1s%1s%3d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%8.3f%7s\n",
              t->card1->MODTRN,t->card1->SPEED,t->card1->MODEL,t->card1->ITYPE,t->card1->IEMSCT,t->card1->IMULT,
              t->card1->M1,t->card1->M2,t->card1->M3,t->card1->M4,t->card1->M5,t->card1->M6,t->card1->MDEF,
              t->card1->IM,t->card1->NOPRNT,t->card1->TPTEMP,t->card1->SURREF);
}

void CARD1_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD1_NPAR; n++)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD1_ID,CARD1_name[n],CARD1_to_s(t->card1,n,s),CARD1_description[n]);
  }
}

int CARD1_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD1_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_upp(line,n,1,t->card1->MODTRN)  < 0) rt =  1; n += 1;
  if(get_upp(line,n,1,t->card1->SPEED)   < 0) rt =  2; n += 1;
  if(get_int(line,n,3,&t->card1->MODEL)  < 0) rt =  3; n += 3;
  if(get_int(line,n,5,&t->card1->ITYPE)  < 0) rt =  4; n += 5;
  if(get_int(line,n,5,&t->card1->IEMSCT) < 0) rt =  5; n += 5;
  if(get_int(line,n,5,&t->card1->IMULT)  < 0) rt =  6; n += 5;
  if(get_int(line,n,5,&t->card1->M1)     < 0) rt =  7; n += 5;
  if(get_int(line,n,5,&t->card1->M2)     < 0) rt =  8; n += 5;
  if(get_int(line,n,5,&t->card1->M3)     < 0) rt =  9; n += 5;
  if(get_int(line,n,5,&t->card1->M4)     < 0) rt = 10; n += 5;
  if(get_int(line,n,5,&t->card1->M5)     < 0) rt = 11; n += 5;
  if(get_int(line,n,5,&t->card1->M6)     < 0) rt = 12; n += 5;
  if(get_int(line,n,5,&t->card1->MDEF)   < 0) rt = 13; n += 5;
  if(get_int(line,n,5,&t->card1->IM)     < 0) rt = 14; n += 5;
  if(get_int(line,n,5,&t->card1->NOPRNT) < 0) rt = 15; n += 5;
  if(get_dbl(line,n,8,&t->card1->TPTEMP) < 0) rt = 16; n += 8;
  if(get_upp(line,n,7,t->card1->SURREF)  < 0) rt = 17;
  if(rt)
  {
    fprintf(stderr,"CARD1_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD1_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD1_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD1_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD1_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD1_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD1_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // convert value if required
  switch(n)
  {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
      errno = 0;
      ntmp = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 15:
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
      strncpy(t->card1->MODTRN,str[2],sizeof(t->card1->MODTRN));
      break;
    case 1:
      strncpy(t->card1->SPEED,str[2],sizeof(t->card1->SPEED));
      break;
    case 2:
      t->card1->MODEL = ntmp;
      break;
    case 3:
      t->card1->ITYPE = ntmp;
      break;
    case 4:
      t->card1->IEMSCT = ntmp;
      break;
    case 5:
      t->card1->IMULT = ntmp;
      break;
    case 6:
      t->card1->M1 = ntmp;
      break;
    case 7:
      t->card1->M2 = ntmp;
      break;
    case 8:
      t->card1->M3 = ntmp;
      break;
    case 9:
      t->card1->M4 = ntmp;
      break;
    case 10:
      t->card1->M5 = ntmp;
      break;
    case 11:
      t->card1->M6 = ntmp;
      break;
    case 12:
      t->card1->MDEF = ntmp;
      break;
    case 13:
      t->card1->IM = ntmp;
      break;
    case 14:
      t->card1->NOPRNT = ntmp;
      break;
    case 15:
      t->card1->TPTEMP = xtmp;
      break;
    case 16:
      strncpy(t->card1->SURREF,str[2],CARD_LENGTH);
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD1_check(t->card1,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD1_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD1_NPAR; n++)
  {
    if(CARD1_check(t->card1,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD1_check(const CARD1_struct *c,int n)
{
  int rt;
  double v;
  char *p;

  rt = 0;
  switch(n)
  {
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD1_name[n],c->MODTRN);
        rt = -1;
      }
      break;
    case 1:
      if(strpbrk(c->SPEED,"S M") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD1_name[n],c->SPEED);
        rt = -1;
      }
      break;
    case 2:
      if(c->MODEL<0 || c->MODEL>8)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->MODEL);
        rt = -1;
      }
      break;
    case 3:
      if(c->ITYPE<1 || c->ITYPE>3)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->ITYPE);
        rt = -1;
      }
      break;
    case 4:
      if(c->IEMSCT<0 || c->IEMSCT>3)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->IEMSCT);
        rt = -1;
      }
      break;
    case 5:
      if(c->IMULT<-1 || c->IMULT>1)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->IMULT);
        rt = -1;
      }
      break;
    case 6:
      if(c->M1<0 || c->M1>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M1);
        rt = -1;
      }
      break;
    case 7:
      if(c->M2<0 || c->M2>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M2);
        rt = -1;
      }
      break;
    case 8:
      if(c->M3<0 || c->M3>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M3);
        rt = -1;
      }
      break;
    case 9:
      if(c->M4<0 || c->M4>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M4);
        rt = -1;
      }
      break;
    case 10:
      if(c->M5<0 || c->M5>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M5);
        rt = -1;
      }
      break;
    case 11:
      if(c->M6<0 || c->M6>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M6);
        rt = -1;
      }
      break;
    case 12:
      if(c->MDEF<0 || c->MDEF>2)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->MDEF);
        rt = -1;
      }
      break;
    case 13:
      if(c->IM<0 || c->IM>1)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->IM);
        rt = -1;
      }
      break;
    case 14:
      if(c->NOPRNT<-2 || c->NOPRNT>1)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->NOPRNT);
        rt = -1;
      }
      break;
    case 15:
      if(c->TPTEMP == NAN)
      {
        fprintf(stderr,"CARD1: error in %s, %13.4e\n",CARD1_name[n],c->TPTEMP);
        rt = -1;
      }
      break;
    case 16:
      if(strncasecmp(c->SURREF,"BRDF",1)!=0 && strncasecmp(c->SURREF,"LAMBER",1)!=0)
      {
        errno = 0;
        v = strtod(c->SURREF,&p);
        if(errno==ERANGE || *p!='\0')
        {
          fprintf(stderr,"CARD1: error in %s, %s\n",CARD1_name[n],c->SURREF);
          rt = -1;
        }
        if(v < 0.0)
        {
          if(fabs(v-(double)((int)(v+0.5))) > 1.0e-13)
          {
            fprintf(stderr,"CARD1: error in %s, %s\n",CARD1_name[n],c->SURREF);
            rt = -1;
          }
        } else
        if(v > 1.0)
        {
          fprintf(stderr,"CARD1: %s out of range >>> %s\n",CARD1_name[n],c->SURREF);
          rt = -1;
        }
      }
      break;
    default:
      rt = -1;
      break;
  }

  return rt;
}

char *CARD1_to_s(const CARD1_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%s",c->MODTRN);
      break;
    case 1:
      sprintf(s,"%s",c->SPEED);
      break;
    case 2:
      sprintf(s,"%d",c->MODEL);
      break;
    case 3:
      sprintf(s,"%d",c->ITYPE);
      break;
    case 4:
      sprintf(s,"%d",c->IEMSCT);
      break;
    case 5:
      sprintf(s,"%d",c->IMULT);
      break;
    case 6:
      sprintf(s,"%d",c->M1);
      break;
    case 7:
      sprintf(s,"%d",c->M2);
      break;
    case 8:
      sprintf(s,"%d",c->M3);
      break;
    case 9:
      sprintf(s,"%d",c->M4);
      break;
    case 10:
      sprintf(s,"%d",c->M5);
      break;
    case 11:
      sprintf(s,"%d",c->M6);
      break;
    case 12:
      sprintf(s,"%d",c->MDEF);
      break;
    case 13:
      sprintf(s,"%d",c->IM);
      break;
    case 14:
      sprintf(s,"%d",c->NOPRNT);
      break;
    case 15:
      sprintf(s,"%.3f",c->TPTEMP);
      break;
    case 16:
      sprintf(s,"%s",c->SURREF);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
