// CARD 4B1 parameters
char	CARD4B1_ID[5]			= "4B1";

char	CARD4B1_comment[MAXLINE]	= "If SURREF = 'BRDF'";

char	CARD4B1_name[CARD4B1_NPAR][10]	=
{
  "CBRDF",
};

CARD4B1_struct CARD4B1_default		=
{
  "?",
};

char CARD4B1_description[CARD4B1_NPAR][MAXLINE] =
{
  "BRDF parametrization",
};

void CARD4B1_write(const CARD4B1_struct *c,FILE *fp)
{
  fprintf(fp,"%-80s\n",c->CBRDF);
}

void CARD4B1_print(const CARD4B1_struct *c,int i,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD4B1_NPAR; n++)
  {
    fprintf(fp,"%-4s %-10s %-7d %-15s # %s\n",CARD4B1_ID,CARD4B1_name[n],i,CARD4B1_to_s(c,n,s),CARD4B1_description[n]);
  }
}

int CARD4B1_read(CARD4B1_struct *c,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD4B1_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_chr(line,n,80,c->CBRDF) < 0) rt = 1;
  if(rt)
  {
    fprintf(stderr,"CARD4B1_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  for(n=0; n<CARD4B1_NPAR; n++)
  {
    if(CARD4B1_check(c,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD4B1_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD4B1_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int indx;
  char str[NCOL4][MAXLINE];
  char *p;

  // read line
  for(i=0,p=line; i<NCOL4; i++,p+=n)
  {
    if(sscanf(p,"%s%n",str[i],&n) == EOF) break;
  }
  if(i < 2)
  {
    return -1;
  }
  if(strcmp(CARD4B1_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL4)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD4B1_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD4B1_name[i]) == 0)
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
        fprintf(stderr,"CARD4B1: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=MXNSRF)
      {
        fprintf(stderr,"CARD4B1: index out of range >>> %s\n",str[2]);
        return -1;
      }
  }

  // set value
  switch(n)
  {
    case 0:
      strncpy(((t->card4b1)+indx)->CBRDF,str[3],sizeof(((t->card4b1)+indx)->CBRDF));
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD4B1_check((t->card4b1)+indx,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD4B1_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD4B1_NPAR; n++)
  {
    if(CARD4B1_check(t->card4b1,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD4B1_check(const CARD4B1_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD4B1_name[n],c->MODTRN);
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

char *CARD4B1_to_s(const CARD4B1_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%s",c->CBRDF);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
