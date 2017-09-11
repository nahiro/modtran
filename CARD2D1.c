// CARD 2D1 parameters
char	CARD2D1_ID[5]			= "2D1";

char	CARD2D1_comment[MAXLINE]	= "If IHAZE = 7 or ICLD = 11";

char	CARD2D1_name[CARD2D1_NPAR][10]	=
{
  "AWCCON",
  "TITLE",
};

CARD2D1_struct CARD2D1_default		=
{
  NAN,
  "title",
};

char CARD2D1_description[CARD2D1_NPAR][MAXLINE] =
{
  "conversion factor from km-1 to g/m3",
  "title of the region",
};

void CARD2D1_write(const CARD2D1_struct *c,FILE *fp)
{
  fprintf(fp,"%10.3f%70s\n",c->AWCCON,c->TITLE);
}

void CARD2D1_print(const CARD2D1_struct *c,int i,FILE *fp)
{
  int n;
  char s[MAXLINE];

  for(n=0; n<CARD2D1_NPAR; n++)
  {
    fprintf(fp,"%-4s %-10s %-7d %-15s # %s\n",CARD2D1_ID,CARD2D1_name[n],i,CARD2D1_to_s(c,n,s),CARD2D1_description[n]);
  }
}

int CARD2D1_read(CARD2D1_struct *c,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2D1_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_dbl(line,n,10,&c->AWCCON) < 0) rt = 1; n += 10;
  if(get_chr(line,n,70,c->TITLE)   < 0) rt = 2;
  if(rt)
  {
    fprintf(stderr,"CARD2D1_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  for(n=0; n<CARD2D1_NPAR; n++)
  {
    if(CARD2D1_check(c,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD2D1_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD2D1_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int indx = 0;
  double xtmp = NAN;
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
  if(strcmp(CARD2D1_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL4)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2D1_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2D1_name[i]) == 0)
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
        fprintf(stderr,"CARD2D1: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=MXIREG)
      {
        fprintf(stderr,"CARD2D1: index out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[3],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2D1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      ((t->card2d1)+indx)->AWCCON = xtmp;
      break;
    case 1:
      strncpy(((t->card2d1)+indx)->TITLE,str[2],sizeof(((t->card2d1)+indx)->TITLE));
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2D1_check((t->card2d1)+indx,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2D1_check_all(const TAPE5_struct *t)
{
  int i,n;
  int rt;

  rt = 0;
  for(i=0; i<MXIREG; i++)
  {
    for(n=0; n<CARD2D1_NPAR; n++)
    {
      if(CARD2D1_check((t->card2d1)+i,n) < 0)
      {
        rt = -1;
      }
    }
  }

  return rt;
}

int CARD2D1_check(const CARD2D1_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD2D1_name[n],c->MODTRN);
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

char *CARD2D1_to_s(const CARD2D1_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%.3f",c->AWCCON);
      break;
    case 1:
      sprintf(s,"%s",c->TITLE);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
