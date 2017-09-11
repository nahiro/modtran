// CARD 3 parameters
char	CARD3_ID[5]			= "3";

char	CARD3_comment1[MAXLINE]		= "required";
char	CARD3_comment2[MAXLINE]		= "If IEMSCT = 3";

char	CARD3_name[CARD3_NPAR][10]	=
{
  "H1",
  "H2",
  "ANGLE",
  "RANGE",
  "BETA",
  "RO",
  "LENN",
  "PHI",
  "IDAY",
  "ISOURC",
  "ANGLEM",
};

CARD3_struct CARD3_default		=
{
  0.0,
  0.0,
  20.0,
  0.0,
  0.0,
  0.0,
  0,
  0.0,
  0,
  0,
  0.0,
};

char CARD3_description[CARD3_NPAR+3][MAXLINE] =
{
  "initial altitude (km)",
  "final altitude (km)",
  "initial zenith angle (deg)",
  "path length (km)",
  "earth center angle (deg)",
  "radius of the earth (km)",
  "switch to determine short and long paths",
  "zenith angle (deg)",
  "altitude of the observer",
  "tangent height of path to sun or moon",
  "apparent solar or lunar zenith angle at H1",
  "day of the year",
  "extra-terrestrial source",
  "phase angle of the moon (deg)",
};

void CARD3_write(const TAPE5_struct *t,FILE *fp)
{
  if(t->card1->IEMSCT == 3)
  {
    fprintf(fp,"%10.3f%10.3f%10.3f%5d%5s%10.3f%5d%10.3f\n",
                t->card3->H1,t->card3->H2,t->card3->ANGLE,t->card3->IDAY,"",t->card3->RO,t->card3->ISOURC,t->card3->ANGLEM);
  }
  else
  {
    fprintf(fp,"%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f%5d%5s%10.3f\n",
                t->card3->H1,t->card3->H2,t->card3->ANGLE,t->card3->RANGE,t->card3->BETA,t->card3->RO,t->card3->LENN,"",t->card3->PHI);
  }
}

void CARD3_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  if(t->card1->IEMSCT == 3)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3_ID,CARD3_name[ 0],CARD3_to_s(t->card3, 0,s),CARD3_description[ 8]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3_ID,CARD3_name[ 1],CARD3_to_s(t->card3, 1,s),CARD3_description[ 9]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3_ID,CARD3_name[ 2],CARD3_to_s(t->card3, 2,s),CARD3_description[10]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3_ID,CARD3_name[ 8],CARD3_to_s(t->card3, 8,s),CARD3_description[11]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3_ID,CARD3_name[ 5],CARD3_to_s(t->card3, 5,s),CARD3_description[ 5]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3_ID,CARD3_name[ 9],CARD3_to_s(t->card3, 9,s),CARD3_description[12]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3_ID,CARD3_name[10],CARD3_to_s(t->card3,10,s),CARD3_description[13]);
  }
  else
  {
    for(n=0; n<8; n++)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3_ID,CARD3_name[n],CARD3_to_s(t->card3,n,s),CARD3_description[n]);
    }
  }
}

int CARD3_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD3_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(t->card1->IEMSCT == 3)
  {
    if(get_dbl(line,n,10,&t->card3->H1)     < 0) rt =  1; n += 10;
    if(get_dbl(line,n,10,&t->card3->H2)     < 0) rt =  2; n += 10;
    if(get_dbl(line,n,10,&t->card3->ANGLE)  < 0) rt =  3; n += 10;
    if(get_int(line,n, 5,&t->card3->IDAY)   < 0) rt =  4; n += 10;
    if(get_dbl(line,n,10,&t->card3->RO)     < 0) rt =  5; n += 10;
    if(get_int(line,n, 5,&t->card3->ISOURC) < 0) rt =  6; n += 5;
    if(get_dbl(line,n,10,&t->card3->ANGLEM) < 0) rt =  7;
  }
  else
  {
    if(get_dbl(line,n,10,&t->card3->H1)     < 0) rt =  8; n += 10;
    if(get_dbl(line,n,10,&t->card3->H2)     < 0) rt =  9; n += 10;
    if(get_dbl(line,n,10,&t->card3->ANGLE)  < 0) rt = 10; n += 10;
    if(get_dbl(line,n,10,&t->card3->RANGE)  < 0) rt = 11; n += 10;
    if(get_dbl(line,n,10,&t->card3->BETA)   < 0) rt = 12; n += 10;
    if(get_dbl(line,n,10,&t->card3->RO)     < 0) rt = 13; n += 10;
    if(get_int(line,n, 5,&t->card3->LENN)   < 0) rt = 14; n += 10;
    if(get_dbl(line,n,10,&t->card3->PHI)    < 0) rt = 15;
  }
  if(rt)
  {
    fprintf(stderr,"CARD3_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD3_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD3_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD3_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD3_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD3_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD3_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // convert value if required
  switch(n)
  {
    case 6:
    case 8:
    case 9:
      errno = 0;
      ntmp = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD3: Convert error >>> %s\n",line);
        return -1;
      }
      break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 10:
      errno = 0;
      xtmp = strtod(str[2],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD3: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      t->card3->H1 = xtmp;
      break;
    case 1:
      t->card3->H2 = xtmp;
      break;
    case 2:
      t->card3->ANGLE = xtmp;
      break;
    case 3:
      t->card3->RANGE = xtmp;
      break;
    case 4:
      t->card3->BETA = xtmp;
      break;
    case 5:
      t->card3->RO = xtmp;
      break;
    case 6:
      t->card3->LENN = ntmp;
      break;
    case 7:
      t->card3->PHI = xtmp;
      break;
    case 8:
      t->card3->IDAY = ntmp;
      break;
    case 9:
      t->card3->ISOURC = ntmp;
      break;
    case 10:
      t->card3->ANGLEM = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD3_check(t->card3,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD3_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD3_NPAR; n++)
  {
    if(CARD3_check(t->card3,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD3_check(const CARD3_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD3: error in %s, %s\n",CARD3_name[n],c->MODTRN);
        rt = -1;
      }
      break;
  */
    default:
      //rt = -1;
      break;
  }

  return rt;
}

char *CARD3_to_s(const CARD3_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%.4f",c->H1);
      break;
    case 1:
      sprintf(s,"%.4f",c->H2);
      break;
    case 2:
      sprintf(s,"%.7f",c->ANGLE);
      break;
    case 3:
      sprintf(s,"%.3f",c->RANGE);
      break;
    case 4:
      sprintf(s,"%.3f",c->BETA);
      break;
    case 5:
      sprintf(s,"%.3f",c->RO);
      break;
    case 6:
      sprintf(s,"%d",c->LENN);
      break;
    case 7:
      sprintf(s,"%.3f",c->PHI);
      break;
    case 8:
      sprintf(s,"%d",c->IDAY);
      break;
    case 9:
      sprintf(s,"%d",c->ISOURC);
      break;
    case 10:
      sprintf(s,"%.3f",c->ANGLEM);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
