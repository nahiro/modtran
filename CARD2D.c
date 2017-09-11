// CARD 2D parameters
char	CARD2D_ID[5]			= "2D";

char	CARD2D_comment[MAXLINE]		= "If IHAZE = 7 or ICLD = 11";

char	CARD2D_name[CARD2D_NPAR][10]	=
{
  "IREG",
};

CARD2D_struct CARD2D_default		=
{
  {0,0,0,0},
};

char CARD2D_description[CARD2D_NPAR][MAXLINE] =
{
  "read parameters for the region N",
};

void CARD2D_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp,"%5d%5d%5d%5d\n",t->card2d->IREG[0],t->card2d->IREG[1],t->card2d->IREG[2],t->card2d->IREG[3]);
}

void CARD2D_print(const TAPE5_struct *t,FILE *fp)
{
  int i,n;

  for(n=0; n<CARD2D_NPAR; n++)
  {
    switch(n)
    {
      case 0:
        for(i=0; i<MXIREG; i++)
        {
          fprintf(fp,"%-4s %-10s %-7d %-15d # %s\n",CARD2D_ID,CARD2D_name[n],i,t->card2d->IREG[i],CARD2D_description[n]);
        }
        break;
    }
  }
}

int CARD2D_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2D_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_int(line,n,5,&t->card2d->IREG[0]) < 0) rt =  1; n += 5;
  if(get_int(line,n,5,&t->card2d->IREG[1]) < 0) rt =  1; n += 5;
  if(get_int(line,n,5,&t->card2d->IREG[2]) < 0) rt =  1; n += 5;
  if(get_int(line,n,5,&t->card2d->IREG[3]) < 0) rt =  1;
  if(rt)
  {
    fprintf(stderr,"CARD2D_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD2D_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD2D_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD2D_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int indx;
  int ntmp = -1;
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
  if(strcmp(CARD2D_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL4)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2D_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2D_name[i]) == 0)
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
        fprintf(stderr,"CARD2D: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=MXIREG)
      {
        fprintf(stderr,"CARD2D: index out of range >>> %s\n",str[2]);
        return -1;
      }
      break;
      errno = 0;
      ntmp = strtol(str[3],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2D: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      (t->card2d)->IREG[indx] = ntmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2D_check(t->card2d,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2D_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD2D_NPAR; n++)
  {
    if(CARD2D_check(t->card2d,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD2D_check(const CARD2D_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
/*
  char		APLUS[3];
  int		IHAZE;
  char		CNOVAM[2];
  int		ISEASN;
  char		ARUSS[4];
  int		IVULCN;
  int		ICSTL;
  int		ICLD;
  int		IVSA;
  double	VIS;
  double	WSS;
  double	WHH;
  double	RAINRT;
  double	GNDALT;

    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD2D: error in %s, %s\n",CARD2D_name[n],c->MODTRN);
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

char *CARD2D_to_s(const CARD2D_struct *c,int n,char *s)
{
  switch(n)
  {
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
