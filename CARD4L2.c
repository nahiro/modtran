// CARD 4L2 parameters
char	CARD4L2_ID[5]			= "4L2";

char	CARD4L2_comment[MAXLINE]	= "If SURREF = 'LAMBER'";

char	CARD4L2_name[CARD4L2_NPAR][10]	=
{
  "CSALB",
};

CARD4L2_struct CARD4L2_default		=
{
  "?",
};

char CARD4L2_description[CARD4L2_NPAR][MAXLINE] =
{
  "spectral albedo curve",
};

void CARD4L2_write(const CARD4L2_struct *c,FILE *fp)
{
  fprintf(fp,"%-80s\n",c->CSALB);
}

void CARD4L2_print(const CARD4L2_struct *c,int i,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD4L2_NPAR; n++)
  {
    fprintf(fp,"%-4s %-10s %-7d %-15s # %s\n",CARD4L2_ID,CARD4L2_name[n],i,CARD4L2_to_s(c,n,s),CARD4L2_description[n]);
  }
}

int CARD4L2_read(CARD4L2_struct *c,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD4L2_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_chr(line,n,80,c->CSALB) < 0) rt = 1;
  if(rt)
  {
    fprintf(stderr,"CARD4L2_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  for(n=0; n<CARD4L2_NPAR; n++)
  {
    if(CARD4L2_check(c,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD4L2_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD4L2_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD4L2_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL4)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD4L2_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD4L2_name[i]) == 0)
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
        fprintf(stderr,"CARD4L2: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=MXNSRF)
      {
        fprintf(stderr,"CARD4L2: index out of range >>> %s\n",str[2]);
        return -1;
      }
  }

  // set value
  switch(n)
  {
    case 0:
      strncpy(((t->card4l2)+indx)->CSALB,str[3],sizeof(((t->card4l2)+indx)->CSALB));
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD4L2_check((t->card4l2)+indx,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD4L2_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD4L2_NPAR; n++)
  {
    if(CARD4L2_check(t->card4l2,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD4L2_check(const CARD4L2_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD4L2_name[n],c->MODTRN);
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

char *CARD4L2_to_s(const CARD4L2_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%s",c->CSALB);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
