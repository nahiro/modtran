// CARD 3A1 parameters
char	CARD3A1_ID[5]			= "3A1";

char	CARD3A1_comment[MAXLINE]	= "If IEMSCT = 2";

char	CARD3A1_name[CARD3A1_NPAR][10]	=
{
  "IPARM",
  "IPH",
  "IDAY",
  "ISOURC",
};

CARD3A1_struct CARD3A1_default		=
{
  0,
  0,
  0,
  0,
};

char CARD3A1_description[CARD3A1_NPAR][MAXLINE] =
{
  "method of specifying the source geometry",
  "aerosol phase function",
  "day of year",
  "extra-terrestrial source",
};

void CARD3A1_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp,"%5d%5d%5d%5d\n",t->card3a1->IPARM,t->card3a1->IPH,t->card3a1->IDAY,t->card3a1->ISOURC);
}

void CARD3A1_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD3A1_NPAR; n++)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A1_ID,CARD3A1_name[n],CARD3A1_to_s(t->card3a1,n,s),CARD3A1_description[n]);
  }
}

int CARD3A1_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD3A1_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_int(line,n,5,&t->card3a1->IPARM)  < 0) rt = 1; n += 5;
  if(get_int(line,n,5,&t->card3a1->IPH)    < 0) rt = 2; n += 5;
  if(get_int(line,n,5,&t->card3a1->IDAY)   < 0) rt = 3; n += 5;
  if(get_int(line,n,5,&t->card3a1->ISOURC) < 0) rt = 4;
  if(rt)
  {
    fprintf(stderr,"CARD3A1_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD3A1_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD3A1_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD3A1_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD3A1_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD3A1_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD3A1_name[i]) == 0)
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
      t->card3a1->IPARM = ntmp;
      break;
    case 1:
      t->card3a1->IPH = ntmp;
      break;
    case 2:
      t->card3a1->IDAY = ntmp;
      break;
    case 3:
      t->card3a1->ISOURC = ntmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD3A1_check(t->card3a1,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD3A1_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD3A1_NPAR; n++)
  {
    if(CARD3A1_check(t->card3a1,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD3A1_check(const CARD3A1_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD3A1_name[n],c->MODTRN);
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

char *CARD3A1_to_s(const CARD3A1_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%d",c->IPARM);
      break;
    case 1:
      sprintf(s,"%d",c->IPH);
      break;
    case 2:
      sprintf(s,"%d",c->IDAY);
      break;
    case 3:
      sprintf(s,"%d",c->ISOURC);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
