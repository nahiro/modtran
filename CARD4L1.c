// CARD 4L1 parameters
char	CARD4L1_ID[5]			= "4L1";

char	CARD4L1_comment[MAXLINE]	= "If SURREF = 'LAMBER'";

char	CARD4L1_name[CARD4L1_NPAR][10]	=
{
  "SALBFL",
};

CARD4L1_struct CARD4L1_default		=
{
  "?",
};

char CARD4L1_description[CARD4L1_NPAR][MAXLINE] =
{
  "spectral albedo data file name",
};

void CARD4L1_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp,"%-80s\n",t->card4l1->SALBFL);
}

void CARD4L1_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD4L1_NPAR; n++)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD4L1_ID,CARD4L1_name[n],CARD4L1_to_s(t->card4l1,n,s),CARD4L1_description[n]);
  }
}

int CARD4L1_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD4L1_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_chr(line,n,80,t->card4l1->SALBFL) < 0) rt = 1;
  if(rt)
  {
    fprintf(stderr,"CARD4L1_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD4L1_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD4L1_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD4L1_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD4L1_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD4L1_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD4L1_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // set value
  switch(n)
  {
    case 0:
      strncpy(t->card4l1->SALBFL,str[2],sizeof(t->card4l1->SALBFL));
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD4L1_check(t->card4l1,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD4L1_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD4L1_NPAR; n++)
  {
    if(CARD4L1_check(t->card4l1,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD4L1_check(const CARD4L1_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD4L1_name[n],c->MODTRN);
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

char *CARD4L1_to_s(const CARD4L1_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%s",c->SALBFL);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
