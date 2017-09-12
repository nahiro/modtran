// CARD 1 parameters
char	CARD1_ID[5]			= "1";

char	CARD1_comment[MAXLINE]		= "required";

char	CARD1_name[CARD1_NPAR][10]	=
{
  "MODTRN",
  "SPEED",
  "BINARY",
  "LYMOLC",
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
  "I_RD2C",
  "NOPRNT",
  "TPTEMP",
  "SURREF",
};

CARD1_struct CARD1_default		=
{
  "T",
  "M",
  "F",
  "+",
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
  0,
  0,
  0,
  0.0,
  "0.0",
};

char CARD1_description[CARD1_NPAR][MAXLINE] =
{
  "band model algorithm",
  "calculation speed",
  "binary flag",
  "inclusion of auxiliary species",
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
  "read user input data",
  "tape6 and tape8 output control",
  "boundary temperature (K)",
  "surface albedo",
};

void CARD1_write(const TAPE5_struct *t,FILE *fp)
{
  switch(mod_v)
  {
    case MOD_V4:
      fprintf(fp,"%1s%1s%3d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%8.3f%7s\n",
                  t->card1->MODTRN,t->card1->SPEED,t->card1->MODEL,t->card1->ITYPE,t->card1->IEMSCT,t->card1->IMULT,
                  t->card1->M1,t->card1->M2,t->card1->M3,t->card1->M4,t->card1->M5,t->card1->M6,t->card1->MDEF,
                  t->card1->IM,t->card1->NOPRNT,t->card1->TPTEMP,t->card1->SURREF);
      break;
    case MOD_V5:
      fprintf(fp,"%1s%1s%1s%1s%1d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d%8.3f%7s\n",
                  t->card1->MODTRN,t->card1->SPEED,t->card1->BINARY,t->card1->LYMOLC,t->card1->MODEL,
                  t->card1->ITYPE,t->card1->IEMSCT,t->card1->IMULT,
                  t->card1->M1,t->card1->M2,t->card1->M3,t->card1->M4,t->card1->M5,t->card1->M6,t->card1->MDEF,
                  t->card1->I_RD2C,t->card1->NOPRNT,t->card1->TPTEMP,t->card1->SURREF);
      break;
  }
}

void CARD1_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  switch(mod_v)
  {
    case MOD_V4:
      for(n=0; n<CARD1_NPAR; n++)
      {
        switch(n)
        {
          case I_CARD1_BINARY:
          case I_CARD1_LYMOLC:
          case I_CARD1_I_RD2C:
            break;
          default:
            fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD1_ID,CARD1_name[n],CARD1_to_s(t->card1,n,s),CARD1_description[n]);
            break;
        }
      }
      break;
    case MOD_V5:
      for(n=0; n<CARD1_NPAR; n++)
      {
        switch(n)
        {
          case I_CARD1_IM:
            break;
          default:
            fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD1_ID,CARD1_name[n],CARD1_to_s(t->card1,n,s),CARD1_description[n]);
            break;
        }
      }
      break;
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
  switch(mod_v)
  {
    case MOD_V4:
      if(get_upp(line,n,1,t->card1->MODTRN)  < 0) rt = I_CARD1_MODTRN; n += 1;
      if(get_upp(line,n,1,t->card1->SPEED)   < 0) rt = I_CARD1_SPEED;  n += 1;
      if(get_int(line,n,3,&t->card1->MODEL)  < 0) rt = I_CARD1_MODEL;  n += 3;
      if(get_int(line,n,5,&t->card1->ITYPE)  < 0) rt = I_CARD1_ITYPE;  n += 5;
      if(get_int(line,n,5,&t->card1->IEMSCT) < 0) rt = I_CARD1_IEMSCT; n += 5;
      if(get_int(line,n,5,&t->card1->IMULT)  < 0) rt = I_CARD1_IMULT;  n += 5;
      if(get_int(line,n,5,&t->card1->M1)     < 0) rt = I_CARD1_M1;     n += 5;
      if(get_int(line,n,5,&t->card1->M2)     < 0) rt = I_CARD1_M2;     n += 5;
      if(get_int(line,n,5,&t->card1->M3)     < 0) rt = I_CARD1_M3;     n += 5;
      if(get_int(line,n,5,&t->card1->M4)     < 0) rt = I_CARD1_M4;     n += 5;
      if(get_int(line,n,5,&t->card1->M5)     < 0) rt = I_CARD1_M5;     n += 5;
      if(get_int(line,n,5,&t->card1->M6)     < 0) rt = I_CARD1_M6;     n += 5;
      if(get_int(line,n,5,&t->card1->MDEF)   < 0) rt = I_CARD1_MDEF;   n += 5;
      if(get_int(line,n,5,&t->card1->IM)     < 0) rt = I_CARD1_IM;     n += 5;
      if(get_int(line,n,5,&t->card1->NOPRNT) < 0) rt = I_CARD1_NOPRNT; n += 5;
      if(get_dbl(line,n,8,&t->card1->TPTEMP) < 0) rt = I_CARD1_TPTEMP; n += 8;
      if(get_upp(line,n,7,t->card1->SURREF)  < 0) rt = I_CARD1_SURREF;
      break;
    case MOD_V5:
      if(get_upp(line,n,1,t->card1->MODTRN)  < 0) rt = I_CARD1_MODTRN; n += 1;
      if(get_upp(line,n,1,t->card1->SPEED)   < 0) rt = I_CARD1_SPEED;  n += 1;
      if(get_upp(line,n,1,t->card1->BINARY)  < 0) rt = I_CARD1_BINARY; n += 1;
      if(get_upp(line,n,1,t->card1->LYMOLC)  < 0) rt = I_CARD1_LYMOLC; n += 1;
      if(get_int(line,n,1,&t->card1->MODEL)  < 0) rt = I_CARD1_MODEL;  n += 1;
      if(get_int(line,n,5,&t->card1->ITYPE)  < 0) rt = I_CARD1_ITYPE;  n += 5;
      if(get_int(line,n,5,&t->card1->IEMSCT) < 0) rt = I_CARD1_IEMSCT; n += 5;
      if(get_int(line,n,5,&t->card1->IMULT)  < 0) rt = I_CARD1_IMULT;  n += 5;
      if(get_int(line,n,5,&t->card1->M1)     < 0) rt = I_CARD1_M1;     n += 5;
      if(get_int(line,n,5,&t->card1->M2)     < 0) rt = I_CARD1_M2;     n += 5;
      if(get_int(line,n,5,&t->card1->M3)     < 0) rt = I_CARD1_M3;     n += 5;
      if(get_int(line,n,5,&t->card1->M4)     < 0) rt = I_CARD1_M4;     n += 5;
      if(get_int(line,n,5,&t->card1->M5)     < 0) rt = I_CARD1_M5;     n += 5;
      if(get_int(line,n,5,&t->card1->M6)     < 0) rt = I_CARD1_M6;     n += 5;
      if(get_int(line,n,5,&t->card1->MDEF)   < 0) rt = I_CARD1_MDEF;   n += 5;
      if(get_int(line,n,5,&t->card1->I_RD2C) < 0) rt = I_CARD1_I_RD2C; n += 5;
      if(get_int(line,n,5,&t->card1->NOPRNT) < 0) rt = I_CARD1_NOPRNT; n += 5;
      if(get_dbl(line,n,8,&t->card1->TPTEMP) < 0) rt = I_CARD1_TPTEMP; n += 8;
      if(get_upp(line,n,7,t->card1->SURREF)  < 0) rt = I_CARD1_SURREF;
      break;
  }
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
    case I_CARD1_MODEL:
    case I_CARD1_ITYPE:
    case I_CARD1_IEMSCT:
    case I_CARD1_IMULT:
    case I_CARD1_M1:
    case I_CARD1_M2:
    case I_CARD1_M3:
    case I_CARD1_M4:
    case I_CARD1_M5:
    case I_CARD1_M6:
    case I_CARD1_MDEF:
    case I_CARD1_IM:
    case I_CARD1_I_RD2C:
    case I_CARD1_NOPRNT:
      errno = 0;
      ntmp = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case I_CARD1_TPTEMP:
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
  switch(mod_v)
  {
    case MOD_V4:
      switch(n)
      {
        case I_CARD1_MODTRN:
          strncpy(t->card1->MODTRN,str[2],sizeof(t->card1->MODTRN));
          break;
        case I_CARD1_SPEED:
          strncpy(t->card1->SPEED,str[2],sizeof(t->card1->SPEED));
          break;
        case I_CARD1_MODEL:
          t->card1->MODEL = ntmp;
          break;
        case I_CARD1_ITYPE:
          t->card1->ITYPE = ntmp;
          break;
        case I_CARD1_IEMSCT:
          t->card1->IEMSCT = ntmp;
          break;
        case I_CARD1_IMULT:
          t->card1->IMULT = ntmp;
          break;
        case I_CARD1_M1:
          t->card1->M1 = ntmp;
          break;
        case I_CARD1_M2:
          t->card1->M2 = ntmp;
          break;
        case I_CARD1_M3:
          t->card1->M3 = ntmp;
          break;
        case I_CARD1_M4:
          t->card1->M4 = ntmp;
          break;
        case I_CARD1_M5:
          t->card1->M5 = ntmp;
          break;
        case I_CARD1_M6:
          t->card1->M6 = ntmp;
          break;
        case I_CARD1_MDEF:
          t->card1->MDEF = ntmp;
          break;
        case I_CARD1_IM:
          t->card1->IM = ntmp;
          break;
        case I_CARD1_NOPRNT:
          t->card1->NOPRNT = ntmp;
          break;
        case I_CARD1_TPTEMP:
          t->card1->TPTEMP = xtmp;
          break;
        case I_CARD1_SURREF:
          strncpy(t->card1->SURREF,str[2],CARD_LENGTH);
          break;
        default:
          return -1;
          break;
      }
      break;
    case MOD_V5:
      switch(n)
      {
        case I_CARD1_MODTRN:
          strncpy(t->card1->MODTRN,str[2],sizeof(t->card1->MODTRN));
          break;
        case I_CARD1_SPEED:
          strncpy(t->card1->SPEED,str[2],sizeof(t->card1->SPEED));
          break;
        case I_CARD1_BINARY:
          strncpy(t->card1->BINARY,str[2],sizeof(t->card1->BINARY));
          break;
        case I_CARD1_LYMOLC:
          strncpy(t->card1->LYMOLC,str[2],sizeof(t->card1->LYMOLC));
          break;
        case I_CARD1_MODEL:
          t->card1->MODEL = ntmp;
          break;
        case I_CARD1_ITYPE:
          t->card1->ITYPE = ntmp;
          break;
        case I_CARD1_IEMSCT:
          t->card1->IEMSCT = ntmp;
          break;
        case I_CARD1_IMULT:
          t->card1->IMULT = ntmp;
          break;
        case I_CARD1_M1:
          t->card1->M1 = ntmp;
          break;
        case I_CARD1_M2:
          t->card1->M2 = ntmp;
          break;
        case I_CARD1_M3:
          t->card1->M3 = ntmp;
          break;
        case I_CARD1_M4:
          t->card1->M4 = ntmp;
          break;
        case I_CARD1_M5:
          t->card1->M5 = ntmp;
          break;
        case I_CARD1_M6:
          t->card1->M6 = ntmp;
          break;
        case I_CARD1_MDEF:
          t->card1->MDEF = ntmp;
          break;
        case I_CARD1_I_RD2C:
          t->card1->I_RD2C = ntmp;
          break;
        case I_CARD1_NOPRNT:
          t->card1->NOPRNT = ntmp;
          break;
        case I_CARD1_TPTEMP:
          t->card1->TPTEMP = xtmp;
          break;
        case I_CARD1_SURREF:
          strncpy(t->card1->SURREF,str[2],CARD_LENGTH);
          break;
        default:
          return -1;
          break;
      }
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
    case I_CARD1_MODTRN:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD1_name[n],c->MODTRN);
        rt = -1;
      }
      break;
    case I_CARD1_SPEED:
      if(strpbrk(c->SPEED,"S M") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD1_name[n],c->SPEED);
        rt = -1;
      }
      break;
    case I_CARD1_BINARY:
      if(strpbrk(c->BINARY,"F T") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD1_name[n],c->BINARY);
        rt = -1;
      }
      break;
    case I_CARD1_LYMOLC:
      if(strpbrk(c->LYMOLC,"+ ") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD1_name[n],c->LYMOLC);
        rt = -1;
      }
      break;
    case I_CARD1_MODEL:
      if(c->MODEL<0 || c->MODEL>8)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->MODEL);
        rt = -1;
      }
      break;
    case I_CARD1_ITYPE:
      if(c->ITYPE<1 || c->ITYPE>3)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->ITYPE);
        rt = -1;
      }
      break;
    case I_CARD1_IEMSCT:
      if(c->IEMSCT<0 || c->IEMSCT>3)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->IEMSCT);
        rt = -1;
      }
      break;
    case I_CARD1_IMULT:
      if(c->IMULT<-1 || c->IMULT>1)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->IMULT);
        rt = -1;
      }
      break;
    case I_CARD1_M1:
      if(c->M1<0 || c->M1>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M1);
        rt = -1;
      }
      break;
    case I_CARD1_M2:
      if(c->M2<0 || c->M2>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M2);
        rt = -1;
      }
      break;
    case I_CARD1_M3:
      if(c->M3<0 || c->M3>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M3);
        rt = -1;
      }
      break;
    case I_CARD1_M4:
      if(c->M4<0 || c->M4>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M4);
        rt = -1;
      }
      break;
    case I_CARD1_M5:
      if(c->M5<0 || c->M5>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M5);
        rt = -1;
      }
      break;
    case I_CARD1_M6:
      if(c->M6<0 || c->M6>6)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->M6);
        rt = -1;
      }
      break;
    case I_CARD1_MDEF:
      if(c->MDEF<0 || c->MDEF>2)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->MDEF);
        rt = -1;
      }
      break;
    case I_CARD1_IM:
      if(c->IM<0 || c->IM>1)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->IM);
        rt = -1;
      }
      break;
    case I_CARD1_I_RD2C:
      if(c->I_RD2C<0 || c->I_RD2C>1)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->I_RD2C);
        rt = -1;
      }
      break;
    case I_CARD1_NOPRNT:
      if(c->NOPRNT<-2 || c->NOPRNT>1)
      {
        fprintf(stderr,"CARD1: error in %s, %d\n",CARD1_name[n],c->NOPRNT);
        rt = -1;
      }
      break;
    case I_CARD1_TPTEMP:
      if(c->TPTEMP == NAN)
      {
        fprintf(stderr,"CARD1: error in %s, %13.4e\n",CARD1_name[n],c->TPTEMP);
        rt = -1;
      }
      break;
    case I_CARD1_SURREF:
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
          v *= -1.0;
          if(fabs(v-(double)((int)(v+0.5))) > 1.0e-10)
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
    case I_CARD1_MODTRN:
      sprintf(s,"%s",c->MODTRN);
      break;
    case I_CARD1_SPEED:
      sprintf(s,"%s",c->SPEED);
      break;
    case I_CARD1_BINARY:
      sprintf(s,"%s",c->BINARY);
      break;
    case I_CARD1_LYMOLC:
      sprintf(s,"%s",c->LYMOLC);
      break;
    case I_CARD1_MODEL:
      sprintf(s,"%d",c->MODEL);
      break;
    case I_CARD1_ITYPE:
      sprintf(s,"%d",c->ITYPE);
      break;
    case I_CARD1_IEMSCT:
      sprintf(s,"%d",c->IEMSCT);
      break;
    case I_CARD1_IMULT:
      sprintf(s,"%d",c->IMULT);
      break;
    case I_CARD1_M1:
      sprintf(s,"%d",c->M1);
      break;
    case I_CARD1_M2:
      sprintf(s,"%d",c->M2);
      break;
    case I_CARD1_M3:
      sprintf(s,"%d",c->M3);
      break;
    case I_CARD1_M4:
      sprintf(s,"%d",c->M4);
      break;
    case I_CARD1_M5:
      sprintf(s,"%d",c->M5);
      break;
    case I_CARD1_M6:
      sprintf(s,"%d",c->M6);
      break;
    case I_CARD1_MDEF:
      sprintf(s,"%d",c->MDEF);
      break;
    case I_CARD1_IM:
      sprintf(s,"%d",c->IM);
      break;
    case I_CARD1_I_RD2C:
      sprintf(s,"%d",c->I_RD2C);
      break;
    case I_CARD1_NOPRNT:
      sprintf(s,"%d",c->NOPRNT);
      break;
    case I_CARD1_TPTEMP:
      sprintf(s,"%.3f",c->TPTEMP);
      break;
    case I_CARD1_SURREF:
      sprintf(s,"%s",c->SURREF);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
