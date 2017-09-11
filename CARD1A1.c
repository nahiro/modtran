// CARD 1A1 parameters
char	CARD1A1_ID[5]			= "1A1";

char	CARD1A1_comment[MAXLINE]	= "If LSUNFL = True";

char	CARD1A1_name[CARD1A1_NPAR][10]	=
{
  "SUNFL2",
};

CARD1A1_struct CARD1A1_default		=
{
  "newkur.dat",
};

char CARD1A1_description[CARD1A1_NPAR][MAXLINE] =
{
  "solar irradiance file name",
};

void CARD1A1_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp,"%-80s\n",t->card1a1->SUNFL2);
}

void CARD1A1_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD1A1_NPAR; n++)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD1A1_ID,CARD1A1_name[n],CARD1A1_to_s(t->card1a1,n,s),CARD1A1_description[n]);
  }
}

int CARD1A1_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD1A1_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_chr(line,n,80,t->card1a1->SUNFL2) < 0) rt = 1;
  if(rt)
  {
    fprintf(stderr,"CARD1A1_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD1A1_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD1A1_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD1A1_gets(char *line,TAPE5_struct *t)
{
  int i,n;
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
  if(strcmp(CARD1A1_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD1A1_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD1A1_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // set value
  switch(n)
  {
    case 0:
      strncpy(t->card1a1->SUNFL2,str[2],sizeof(t->card1a1->SUNFL2));
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD1A1_check(t->card1a1,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD1A1_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD1A1_NPAR; n++)
  {
    if(CARD1A1_check(t->card1a1,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD1A1_check(const CARD1A1_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD1A1_name[n],c->MODTRN);
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

char *CARD1A1_to_s(const CARD1A1_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%s",c->SUNFL2);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
