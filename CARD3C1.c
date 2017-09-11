// CARD 3C1 parameters
char	CARD3C1_ID[5]			= "3C1";

char	CARD3C1_comment[MAXLINE]	= "If IPH = 1 and NWLF > 0";

char	CARD3C1_name[CARD3C1_NPAR][10]	=
{
  "ANGF",
};

CARD3C1_struct CARD3C1_default;

char CARD3C1_description[CARD3C1_NPAR][MAXLINE] =
{
  "scattering angle (deg)",
};

void CARD3C1_write(const TAPE5_struct *t,FILE *fp)
{
  int i;

  for(i=0; i<t->card3b1->NANGLS; i++)
  {
    fprintf(fp," %9.3f%s",(t->card3c1)->ANGF[i],i==t->card3b1->NANGLS-1?"\n":i%8==7?"\n":"");
  }
}

void CARD3C1_print(const TAPE5_struct *t,FILE *fp)
{
  int i,n;

  for(n=0; n<CARD3C1_NPAR; n++)
  {
    switch(n)
    {
      case 0:
        for(i=0; i<t->card3b1->NANGLS; i++)
        {
          if(i == 0)
          {
            fprintf(fp,"%-4s %-10s %-7d %-15.5f # %s\n",CARD3C1_ID,CARD3C1_name[n],i,(t->card3c1)->ANGF[i],CARD3C1_description[n]);
          }
          else
          {
            fprintf(fp,"%-4s %-10s %-7d %-15.5f\n",CARD3C1_ID,CARD3C1_name[n],i,(t->card3c1)->ANGF[i]);
          }
        }
        break;
      default:
        break;
    }
  }
}

int CARD3C1_read(TAPE5_struct *t,FILE *fp)
{
  int i,n;
  int rt;
  char line[CARD_LENGTH];

  // initialization
  rt = 0;

  i = 0;
  while(i < t->card3b1->NANGLS)
  {
    memset(line,'\n',CARD_LENGTH);
    // read line
    if(fgets(line,CARD_LENGTH,fp) == NULL)
    {
      fprintf(stderr,"CARD3C1_read: read error.\n");
      return -1;
    }
    // read values
    n = 1;
    if(get_dbl(line,n,9,&(t->card3c1)->ANGF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c1)->ANGF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c1)->ANGF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c1)->ANGF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c1)->ANGF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c1)->ANGF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c1)->ANGF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c1)->ANGF[i]) != 0) break; i++;
  }
  if(i != t->card3b1->NANGLS)
  {
    fprintf(stderr,"CARD3C1_read: error, i=%d, NANGLS=%d\n",i,t->card3b1->NANGLS);
    rt = -1;
  }

  // check values
  for(n=0; n<CARD3C1_NPAR; n++)
  {
    if(CARD3C1_check(t->card3c1,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD3C1_read: rt=%d\n",rt);
    rt = -1;
  }

  return rt;
}

int CARD3C1_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int ncol;
  int indx;
  double xtmp;
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
  if(strcmp(CARD3C1_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(ncol < NCOL4)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD3C1_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD3C1_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // convert value if required
  switch(n)
  {
    case 0:
      errno = 0;
      indx = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD3C1: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=MXANGL)
      {
        fprintf(stderr,"CARD3C1: index out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[3],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD3C1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      (t->card3c1)->ANGF[indx] = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD3C1_check(t->card3c1,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD3C1_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD3C1_NPAR; n++)
  {
    if(CARD3C1_check(t->card3c1,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD3C1_check(const CARD3C1_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD3C1_name[n],c->MODTRN);
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

char *CARD3C1_to_s(const CARD3C1_struct *c,int n,char *s)
{
  switch(n)
  {
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
