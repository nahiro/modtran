// CARD 3C2 parameters
char	CARD3C2_ID[5]			= "3C2";

char	CARD3C2_comment[MAXLINE]	= "If IPH = 1 and NWLF > 0";

char	CARD3C2_name[CARD3C2_NPAR][10]	=
{
  "WLF",
};

CARD3C2_struct CARD3C2_default;

char CARD3C2_description[CARD3C2_NPAR][MAXLINE] =
{
  "wavelength (um)",
};

void CARD3C2_write(const TAPE5_struct *t,FILE *fp)
{
  int i;

  for(i=0; i<t->card3b1->NWLF; i++)
  {
    fprintf(fp," %9.3f%s",(t->card3c2)->WLF[i],i==t->card3b1->NWLF-1?"\n":i%8==7?"\n":"");
  }
}

void CARD3C2_print(const TAPE5_struct *t,FILE *fp)
{
  int i,n;

  for(n=0; n<CARD3C2_NPAR; n++)
  {
    switch(n)
    {
      case 0:
        for(i=0; i<t->card3b1->NWLF; i++)
        {
          if(i == 0)
          {
            fprintf(fp,"%-4s %-10s %-7d %-15.6f # %s\n",CARD3C2_ID,CARD3C2_name[n],i,(t->card3c2)->WLF[i],CARD3C2_description[n]);
          }
          else
          {
            fprintf(fp,"%-4s %-10s %-7d %-15.6f\n",CARD3C2_ID,CARD3C2_name[n],i,(t->card3c2)->WLF[i]);
          }
        }
        break;
      default:
        break;
    }
  }
}

int CARD3C2_read(TAPE5_struct *t,FILE *fp)
{
  int i,n;
  int rt;
  char line[CARD_LENGTH];

  // initialization
  rt = 0;

  i = 0;
  while(i < t->card3b1->NWLF)
  {
    memset(line,'\n',CARD_LENGTH);
    // read line
    if(fgets(line,CARD_LENGTH,fp) == NULL)
    {
      fprintf(stderr,"CARD3C2_read: read error.\n");
      return -1;
    }
    // read values
    n = 1;
    if(get_dbl(line,n,9,&(t->card3c2)->WLF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c2)->WLF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c2)->WLF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c2)->WLF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c2)->WLF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c2)->WLF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c2)->WLF[i]) != 0) break; n += 10; i++;
    if(get_dbl(line,n,9,&(t->card3c2)->WLF[i]) != 0) break; i++;
  }
  if(i != t->card3b1->NWLF)
  {
    fprintf(stderr,"CARD3C2_read: error, i=%d, NWLF=%d\n",i,t->card3b1->NWLF);
    rt = -1;
  }

  // check values
  for(n=0; n<CARD3C2_NPAR; n++)
  {
    if(CARD3C2_check(t->card3c2,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD3C2_read: rt=%d\n",rt);
    rt = -1;
  }

  return rt;
}

int CARD3C2_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD3C2_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(ncol < NCOL4)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD3C2_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD3C2_name[i]) == 0)
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
        fprintf(stderr,"CARD3C2: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=MXANGL)
      {
        fprintf(stderr,"CARD3C2: index out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[3],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD3C2: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      (t->card3c2)->WLF[indx] = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD3C2_check(t->card3c2,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD3C2_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD3C2_NPAR; n++)
  {
    if(CARD3C2_check(t->card3c2,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD3C2_check(const CARD3C2_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD3C2_name[n],c->MODTRN);
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

char *CARD3C2_to_s(const CARD3C2_struct *c,int n,char *s)
{
  switch(n)
  {
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
