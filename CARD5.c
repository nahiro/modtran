// CARD 5 parameters
char	CARD5_ID[5]			= "5";

char	CARD5_comment[MAXLINE]		= "required";

char	CARD5_name[CARD5_NPAR][10]	=
{
  "IRPT",
};

CARD5_struct CARD5_default		=
{
  0,
};

char CARD5_description[CARD5_NPAR][MAXLINE] =
{
  "repeat run option",
};

void CARD5_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp,"%5d\n",t->card5->IRPT);
}

void CARD5_print(const TAPE5_struct *t,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD5_NPAR; n++)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD5_ID,CARD5_name[n],CARD5_to_s(t->card5,n,s),CARD5_description[n]);
  }
}

int CARD5_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD5_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_int(line,n,15,&t->card5->IRPT)  < 0) rt = 1;
  if(rt)
  {
    fprintf(stderr,"CARD5_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD5_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD5_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD5_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD5_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD5_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD5_name[i]) == 0)
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
      ntmp = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD5: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      t->card5->IRPT = ntmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD5_check(t->card5,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD5_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD5_NPAR; n++)
  {
    if(CARD5_check(t->card5,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD5_check(const CARD5_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD5: error in %s, %s\n",CARD5_name[n],c->MODTRN);
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

char *CARD5_to_s(const CARD5_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%d",c->IRPT);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
