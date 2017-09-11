// CARD 2E2 parameters
char	CARD2E2_ID[5]			= "2E2";

char	CARD2E2_comment1[MAXLINE]	= "If ICLD = 1 - 10, NCRSPC >= 2";
char	CARD2E2_comment2[MAXLINE]	= "If ICLD = 1 - 10, NCRSPC = 1";

char	CARD2E2_name[CARD2E2_NPAR][10]	=
{
  "WAVLEN",
  "EXTC6",
  "ABSC6",
  "ASYM6",
  "EXTC7",
  "ABSC7",
  "ASYM7",
  "CFILE",
  "CLDTYP",
  "CIRTYP",
};

CARD2E2_struct CARD2E2_default		=
{
  NAN,
  NAN,
  NAN,
  NAN,
  NAN,
  NAN,
  NAN,
  "?",
  "?",
  "?",
};

char CARD2E2_description[CARD2E2_NPAR][MAXLINE] =
{
  "wavelength (um)",
  "WD extinction coefficient (km-1m3/g)",
  "WD absorption coefficient (km-1m3/g)",
  "HG phase function asymmetry factor for WD",
  "IP extinction coefficient (km-1m3/g)",
  "IP absorption coefficient (km-1m3/g)",
  "HG phase function asymmetry factor for IP",
  "cloud spectral file name",
  "water cloud type",
  "ice cloud type",
};

void CARD2E2_write(const TAPE5_struct *t,FILE *fp)
{
  int i;

  if(t->card2a->NCRSPC == 1)
  {
    fprintf(fp,"%s\n",(t->card2e2)->CFILE);
    fprintf(fp,"%s\n",(t->card2e2)->CLDTYP);
    fprintf(fp,"%s\n",(t->card2e2)->CIRTYP);
  }
  else
  {
    for(i=0; i<t->card2a->NCRSPC; i++)
    {
      fprintf(fp,"%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f\n",
                  ((t->card2e2)+i)->WAVLEN,((t->card2e2)+i)->EXTC6,((t->card2e2)+i)->ABSC6,((t->card2e2)+i)->ASYM6,
                  ((t->card2e2)+i)->EXTC7,((t->card2e2)+i)->ABSC7,((t->card2e2)+i)->ASYM7);
    }
  }
}

void CARD2E2_print(const TAPE5_struct *t,FILE *fp)
{
  int i,n;
  char s[MAXLINE];

  if(t->card2a->NCRSPC == 1)
  {
    for(n=7; n<CARD2E2_NPAR; n++)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD2E2_ID,CARD2E2_name[n],CARD2E2_to_s(t->card2e2,n,s),CARD2E2_description[n]);
    }
  }
  else
  {
    for(i=0; i<t->card2a->NCRSPC; i++)
    {
      for(n=0; n<7; n++)
      {
        fprintf(fp,"%-4s %-10s %-7d %-15s # %s\n",CARD2E2_ID,CARD2E2_name[n],i,CARD2E2_to_s((t->card2e2)+i,n,s),CARD2E2_description[n]);
      }
    }
  }
}

int CARD2E2_read(TAPE5_struct *t,FILE *fp)
{
  int i,n;
  int rt,er;
  char line[CARD_LENGTH];

  // initialization
  rt = 0;

  if(t->card2a->NCRSPC == 1)
  {
    // read line
    if(fgets(line,CARD_LENGTH,fp) == NULL)
    {
      fprintf(stderr,"CARD2E2_read: read error.\n");
      return -1;
    }
    n = 0;
    if(get_chr(line,n,80,t->card2e2->CFILE) < 0)
    {
      fprintf(stderr,"CARD2E2_read: error in the following line.\n");
      fprintf(stderr,"%s\n",line);
      rt = -1;
    }
    // read line
    if(fgets(line,CARD_LENGTH,fp) == NULL)
    {
      fprintf(stderr,"CARD2E2_read: read error.\n");
      return -1;
    }
    n = 0;
    if(get_chr(line,n,80,t->card2e2->CLDTYP) < 0)
    {
      fprintf(stderr,"CARD2E2_read: error in the following line.\n");
      fprintf(stderr,"%s\n",line);
      rt = -1;
    }
    // read line
    if(fgets(line,CARD_LENGTH,fp) == NULL)
    {
      fprintf(stderr,"CARD2E2_read: read error.\n");
      return -1;
    }
    n = 0;
    if(get_chr(line,n,80,t->card2e2->CIRTYP) < 0)
    {
      fprintf(stderr,"CARD2E2_read: error in the following line.\n");
      fprintf(stderr,"%s\n",line);
      rt = -1;
    }
  }
  else
  {
    for(i=0; i<t->card2a->NCRSPC; i++)
    {
      // read line
      if(fgets(line,CARD_LENGTH,fp) == NULL)
      {
        fprintf(stderr,"CARD2E2_read: read error.\n");
        return -1;
      }
      er = 0;
      // read values
      n = 0;
      if(get_dbl(line,n,10,&((t->card2e2)+i)->WAVLEN) < 0) er = 1; n += 10;
      if(get_dbl(line,n,10,&((t->card2e2)+i)->EXTC6)  < 0) er = 2; n += 10;
      if(get_dbl(line,n,10,&((t->card2e2)+i)->ABSC6)  < 0) er = 3; n += 10;
      if(get_dbl(line,n,10,&((t->card2e2)+i)->ASYM6)  < 0) er = 4; n += 10;
      if(get_dbl(line,n,10,&((t->card2e2)+i)->EXTC7)  < 0) er = 5; n += 10;
      if(get_dbl(line,n,10,&((t->card2e2)+i)->ABSC7)  < 0) er = 6; n += 10;
      if(get_dbl(line,n,10,&((t->card2e2)+i)->ASYM7)  < 0) er = 7;
      // check values
      for(n=0; n<7; n++)
      {
        if(CARD2E2_check((t->card2e2)+i,n) < 0)
        {
          er = 8;
        }
      }
      // check error
      if(er)
      {
        fprintf(stderr,"CARD2E2_read: er=%d\n",er);
        fprintf(stderr,"CARD2E2_read: error in the following line.\n");
        fprintf(stderr,"%s\n",line);
        rt = -1;
      }
    }
  }

  return rt;
}

int CARD2E2_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  int ncol;
  int indx = 0;
  double xtmp = NAN;
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
  if(strcmp(CARD2E2_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(ncol < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD2E2_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD2E2_name[i]) == 0)
    {
      n = i;
      break;
    }
  }

  // convert value if required
  switch(n)
  {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      if(ncol < NCOL4)
      {
        return 0;
      }
      errno = 0;
      indx = strtol(str[2],&p,10);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2E2: Convert error >>> %s\n",line);
        return -1;
      }
      if(indx<0 || indx>=MXWVLN)
      {
        fprintf(stderr,"CARD2E2: index out of range >>> %s\n",str[2]);
        return -1;
      }
      errno = 0;
      xtmp = strtod(str[3],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD2E2: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      ((t->card2e2)+indx)->WAVLEN = xtmp;
      break;
    case 1:
      ((t->card2e2)+indx)->EXTC6 = xtmp;
      break;
    case 2:
      ((t->card2e2)+indx)->ABSC6 = xtmp;
      break;
    case 3:
      ((t->card2e2)+indx)->ASYM6 = xtmp;
      break;
    case 4:
      ((t->card2e2)+indx)->EXTC7 = xtmp;
      break;
    case 5:
      ((t->card2e2)+indx)->ABSC7 = xtmp;
      break;
    case 6:
      ((t->card2e2)+indx)->ASYM7 = xtmp;
      break;
    case 7:
      strncpy((t->card2e2)->CFILE,str[2],sizeof((t->card2e2)->CFILE));
      break;
    case 8:
      strncpy((t->card2e2)->CLDTYP,str[2],sizeof((t->card2e2)->CLDTYP));
      break;
    case 9:
      strncpy((t->card2e2)->CIRTYP,str[2],sizeof((t->card2e2)->CIRTYP));
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(n < 7)
  {
    if(CARD2E2_check((t->card2e2)+indx,n) < 0)
    {
      return -1;
    }
  }
  else
  {
    if(CARD2E2_check(t->card2e2,n) < 0)
    {
      return -1;
    }
  }

  return 0;
}

int CARD2E2_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD2E2_NPAR; n++)
  {
    if(CARD2E2_check(t->card2e2,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD2E2_check(const CARD2E2_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD2E2: error in %s, %s\n",CARD2E2_name[n],c->MODTRN);
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

char *CARD2E2_to_s(const CARD2E2_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%.5f",c->WAVLEN);
      break;
    case 1:
      sprintf(s,"%.5f",c->EXTC6);
      break;
    case 2:
      sprintf(s,"%.5f",c->ABSC6);
      break;
    case 3:
      sprintf(s,"%.5f",c->ASYM6);
      break;
    case 4:
      sprintf(s,"%.5f",c->EXTC7);
      break;
    case 5:
      sprintf(s,"%.5f",c->ABSC7);
      break;
    case 6:
      sprintf(s,"%.5f",c->ASYM7);
      break;
    case 7:
      sprintf(s,"%s",c->CFILE);
      break;
    case 8:
      sprintf(s,"%s",c->CLDTYP);
      break;
    case 9:
      sprintf(s,"%s",c->CIRTYP);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
