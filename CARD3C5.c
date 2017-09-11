// CARD 3C5 parameters
char	CARD3C5_ID[5]			= "3C5";

char	CARD3C5_comment[MAXLINE]	= "If IPH = 1 and NWLF > 0";

char	CARD3C5_name[CARD3C5_NPAR][10]	=
{
  "F3",
};

CARD3C5_struct CARD3C5_default;

char CARD3C5_description[CARD3C5_NPAR][MAXLINE] =
{
  "scattering phase function for aerosol 3",
};

void CARD3C5_write(const TAPE5_struct *t,FILE *fp)
{
  int i,j;

  for(i=0; i<t->card3b1->NANGLS; i++)
  {
    for(j=0; j<t->card3b1->NWLF; j++)
    {
      #ifdef OLDMODE
      fprintf(fp," %9.3e%s",(t->card3c5)->F3[i*MXWVLN+j],j==t->card3b1->NWLF-1?"\n":j%8==7?"\n":"");
      #else
      fprintf(fp,"%15.9e%s",(t->card3c5)->F3[i*MXWVLN+j],j==t->card3b1->NWLF-1?"\n":j%8==7?"\n":"");
      #endif
    }
  }
}

void CARD3C5_print(const TAPE5_struct *t,FILE *fp)
{
  int i,j,n;

  for(n=0; n<CARD3C5_NPAR; n++)
  {
    switch(n)
    {
      case 0:
        for(i=0; i<t->card3b1->NANGLS; i++)
        {
          for(j=0; j<t->card3b1->NWLF; j++)
          {
            #ifdef OLDMODE
            if(i==0 && j==0)
            {
              fprintf(fp,"%-4s %-10s %-3d %-3d %-15.3e # %s\n",CARD3C5_ID,CARD3C5_name[n],i,j,(t->card3c5)->F3[i*MXWVLN+j],CARD3C5_description[n]);
            }
            else
            {
              fprintf(fp,"%-4s %-10s %-3d %-3d %-15.3e\n",CARD3C5_ID,CARD3C5_name[n],i,j,(t->card3c5)->F3[i*MXWVLN+j]);
            }
            #else
            if(i==0 && j==0)
            {
              fprintf(fp,"%-4s %-10s %-3d %-3d %-15.9e # %s\n",CARD3C5_ID,CARD3C5_name[n],i,j,(t->card3c5)->F3[i*MXWVLN+j],CARD3C5_description[n]);
            }
            else
            {
              fprintf(fp,"%-4s %-10s %-3d %-3d %-15.9e\n",CARD3C5_ID,CARD3C5_name[n],i,j,(t->card3c5)->F3[i*MXWVLN+j]);
            }
            #endif
          }
        }
        break;
      default:
        break;
    }
  }
}

int CARD3C5_read(TAPE5_struct *t,FILE *fp)
{
  int i,j,n;
  int rt;
  char line[CARD_LENGTH];

  // initialization
  rt = 0;

  for(i=0; i<t->card3b1->NANGLS; i++)
  {
    j = 0;
    while(j < t->card3b1->NWLF)
    {
      memset(line,'\n',CARD_LENGTH);
      // read line
      if(fgets(line,CARD_LENGTH,fp) == NULL)
      {
        fprintf(stderr,"CARD3C5_read: read error.\n");
        return -1;
      }
      // read values
      #ifdef OLDMODE
      n = 1;
      if(get_dbl(line,n,9,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 10; j++;
      if(get_dbl(line,n,9,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 10; j++;
      if(get_dbl(line,n,9,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 10; j++;
      if(get_dbl(line,n,9,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 10; j++;
      if(get_dbl(line,n,9,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 10; j++;
      if(get_dbl(line,n,9,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 10; j++;
      if(get_dbl(line,n,9,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 10; j++;
      if(get_dbl(line,n,9,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; j++;
      #else
      n = 0;
      if(get_dbl(line,n,15,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 15; j++;
      if(get_dbl(line,n,15,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 15; j++;
      if(get_dbl(line,n,15,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 15; j++;
      if(get_dbl(line,n,15,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 15; j++;
      if(get_dbl(line,n,15,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 15; j++;
      if(get_dbl(line,n,15,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 15; j++;
      if(get_dbl(line,n,15,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; n += 15; j++;
      if(get_dbl(line,n,15,&(t->card3c5)->F3[i*MXWVLN+j]) != 0) break; j++;
      #endif
    }
    if(j < t->card3b1->NWLF)
    {
      fprintf(stderr,"CARD3C5_read: error, j=%d, NWLF=%d\n",j,t->card3b1->NWLF);
      rt = -1;
    }
  }

  // check values
  for(n=0; n<CARD3C5_NPAR; n++)
  {
    if(CARD3C5_check(t->card3c5,n) < 0)
    {
      rt = -1;
    }
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD3C5_read: rt=%d\n",rt);
    rt = -1;
  }

  return rt;
}

int CARD3C5_gets(char *line,TAPE5_struct *t)
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
  if(strcmp(CARD3C5_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(ncol < NCOL5)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD3C5_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD3C5_name[i]) == 0)
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
        fprintf(stderr,"CARD3C5: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx1<0 || idx1>=MXANGL)
      {
        fprintf(stderr,"CARD3C5: index 1 out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      idx2 = strtol(str[3],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD3C5: Convert error >>> %s\n",line);
        return -1;
      }
      if(idx2<0 || idx2>=MXANGL)
      {
        fprintf(stderr,"CARD3C5: index 2 out of range >>> %s\n",str[3]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[4],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD3C5: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      (t->card3c5)->F3[idx1*MXWVLN+idx2] = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD3C5_check(t->card3c5,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD3C5_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD3C5_NPAR; n++)
  {
    if(CARD3C5_check(t->card3c5,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD3C5_check(const CARD3C5_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD3C5_name[n],c->MODTRN);
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

char *CARD3C5_to_s(const CARD3C5_struct *c,int n,char *s)
{
  switch(n)
  {
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
