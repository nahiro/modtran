// CARD 2C2X parameters
char	CARD2C2X_ID[5]			= "2C2X";

char	CARD2C2X_comment[MAXLINE]	= "If MDEF = 2 and IRD1 = 1";

char	CARD2C2X_name[CARD2C2X_NPAR][10]	=
{
  "WMOLX",
};

CARD2C2X_struct CARD2C2X_default		=
{
  {NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN},
};

char CARD2C2X_description[CARD2C2X_NPAR][MAXLINE] =
{
  "densities of heavy molecular species",
};

void CARD2C2X_write(const CARD2C2X_struct *c,FILE *fp)
{
  int i;

  #ifdef OLDMODE
  for(i=0; i<8; i++)
  {
    fprintf(fp,"%10.3e",c->WMOLX[i]);
  }
  fprintf(fp,"\n");
  for(; i<CARD2C2X_WMOLX_NINDX; i++)
  {
    fprintf(fp,"%10.3e",c->WMOLX[i]);
  }
  fprintf(fp,"\n");
  #else
  for(i=0; i<8; i++)
  {
    fprintf(fp,"%15.8e",c->WMOLX[i]);
  }
  fprintf(fp,"\n");
  for(; i<CARD2C2X_WMOLX_NINDX; i++)
  {
    fprintf(fp,"%15.8e",c->WMOLX[i]);
  }
  fprintf(fp,"\n");
  #endif
}

void CARD2C2X_print(const CARD2C2X_struct *c,int i,FILE *fp)
{
  int j,n;

  for(n=0; n<CARD2C2X_NPAR; n++)
  {
    switch(n)
    {
      case 0:
        for(j=0; j<CARD2C2X_WMOLX_NINDX; j++)
        {
          #ifdef OLDMODE
          fprintf(fp,"%-4s %-10s %-3d %-3d %-15.3e # %s\n",CARD2C2X_ID,CARD2C2X_name[n],i,j,c->WMOLX[i],CARD2C2X_description[n]);
          #else
          fprintf(fp,"%-4s %-10s %-3d %-3d %-15.8e # %s\n",CARD2C2X_ID,CARD2C2X_name[n],i,j,c->WMOLX[i],CARD2C2X_description[n]);
          #endif
        }
        break;
    }
  }
}

int CARD2C2X_read(CARD2C2X_struct *c,FILE *fp)
{
  int i,n;
  int rt;
  char line[CARD_LENGTH];

  // read line 1
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2C2X_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  #ifdef OLDMODE
  n = 0;
  for(i=0; i<8; i++)
  {
    if(get_dbl(line,n,10,&c->WMOLX[i]) < 0) rt = i+1; n += 10;
  }
  #else
  n = 0;
  for(i=0; i<8; i++)
  {
    if(get_dbl(line,n,15,&c->WMOLX[i]) < 0) rt = i+1; n += 15;
  }
  #endif
  if(rt)
  {
    fprintf(stderr,"CARD2C2X_read: rt=%d\n",rt);
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD2C2X_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  // read line 2
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD2C2X_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  #ifdef OLDMODE
  n = 0;
  for(i=8; i<CARD2C2X_WMOLX_NINDX; i++)
  {
    if(get_dbl(line,n,10,&c->WMOLX[i]) < 0) rt = -1; n += 10;
  }
  #else
  n = 0;
  for(i=8; i<CARD2C2X_WMOLX_NINDX; i++)
  {
    if(get_dbl(line,n,15,&c->WMOLX[i]) < 0) rt = -1; n += 15;
  }
  #endif

  // check values
  for(n=0; n<CARD2C2X_NPAR; n++)
  {
    if(CARD2C2X_check(c,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD2C2X_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD2C2X_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int ncol;
  int idx1,idx2;
  double xtmp;
  char str[NCOL5][MAXLINE];
  char *p;

  // read line
  for(i=0,p=line; i<NCOL5; i++,p+=n)
  {
    if(sscanf(p,"%s%n",str[i],&n) == EOF) break;
  }
  ncol = i;
  if(ncol < 2)
  {
    return -1;
  }
  if(strcmp(CARD2C2X_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(ncol < NCOL5)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2C2X_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2C2X_name[i]) == 0)
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
      idx1 = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C2X: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx1<0 || idx1>=LAYDIM)
      {
        fprintf(stderr,"CARD2C2X: index 1 out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      idx2 = strtol(str[3],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C2X: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx2<0 || idx2>=CARD2C2X_WMOLX_NINDX)
      {
        fprintf(stderr,"CARD2C2X: index 2 out of range >>> %s\n",str[3]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[4],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2C2X: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      ((t->card2c2x)+idx1)->WMOLX[idx2] = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD2C2X_check((t->card2c2x)+idx1,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD2C2X_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD2C2X_NPAR; n++)
  {
    if(CARD2C2X_check(t->card2c2x,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD2C2X_check(const CARD2C2X_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD2C2X_name[n],c->MODTRN);
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

char *CARD2C2X_to_s(const CARD2C2X_struct *c,int n,char *s)
{
  switch(n)
  {
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
