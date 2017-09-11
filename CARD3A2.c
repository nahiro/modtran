// CARD 3A2 parameters
char	CARD3A2_ID[5]			= "3A2";

char	CARD3A2_comment[MAXLINE]	= "If IEMSCT = 2";

char	CARD3A2_name[CARD3A2_NPAR][10]	=
{
  "PARM1",
  "PARM2",
  "PARM3",
  "PARM4",
  "TIME",
  "PSIPO",
  "ANGLEM",
  "G",
};

CARD3A2_struct CARD3A2_default		=
{
  NAN,
  NAN,
  NAN,
  NAN,
  NAN,
  NAN,
  NAN,
  NAN,
};

char CARD3A2_description[CARD3A2_NPAR+7][MAXLINE] =
{
  "observer latitude (deg)", // 0
  "azimuth angle between LOS & observer-Sun (deg)", // 1
  "latitude at H2 (deg)", // 2
  "relative solar azimuth at H2 (deg)", // 3
  "observer longitude (deg)", // 4
  "solar zenith angle (deg)", // 5
  "longitude at H2 (deg)", // 6
  "solar zenith at H2 (deg)", // 7
  "source latitude (deg)", // 8
  "source longitude (deg)", // 9
  "Greenwich time (hour)", // 10
  "true path azimuth from H1 to H2 (deg)", // 11
  "true path azimuth from H2 to H1 (deg)", // 12
  "phase angle of the moon (deg)", // 13
  "HG phase function asymmetry factor", // 14
};

void CARD3A2_write(const TAPE5_struct *t,FILE *fp)
{
  fprintf(fp,"%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f\n",
              t->card3a2->PARM1,t->card3a2->PARM2,t->card3a2->PARM3,t->card3a2->PARM4,
              t->card3a2->TIME,t->card3a2->PSIPO,t->card3a2->ANGLEM,t->card3a2->G);
}

void CARD3A2_print(const TAPE5_struct *t,FILE *fp)
{
  char s[MAXLINE];

  if(t->card3a1->IPARM == 0)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[0],CARD3A2_to_s(t->card3a2,0,s),CARD3A2_description[0]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[1],CARD3A2_to_s(t->card3a2,1,s),CARD3A2_description[4]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[2],CARD3A2_to_s(t->card3a2,2,s),CARD3A2_description[8]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[3],CARD3A2_to_s(t->card3a2,3,s),CARD3A2_description[9]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[5],CARD3A2_to_s(t->card3a2,5,s),CARD3A2_description[11]);
    if(t->card3a1->ISOURC == 1)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[6],CARD3A2_to_s(t->card3a2,6,s),CARD3A2_description[13]);
    }
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[7],CARD3A2_to_s(t->card3a2,7,s),CARD3A2_description[14]);
  } else
  if(t->card3a1->IPARM == 1)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[0],CARD3A2_to_s(t->card3a2,0,s),CARD3A2_description[0]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[1],CARD3A2_to_s(t->card3a2,1,s),CARD3A2_description[4]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[4],CARD3A2_to_s(t->card3a2,4,s),CARD3A2_description[10]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[5],CARD3A2_to_s(t->card3a2,5,s),CARD3A2_description[11]);
    if(t->card3a1->ISOURC == 1)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[6],CARD3A2_to_s(t->card3a2,6,s),CARD3A2_description[13]);
    }
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[7],CARD3A2_to_s(t->card3a2,7,s),CARD3A2_description[14]);
  } else
  if(t->card3a1->IPARM == 2)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[0],CARD3A2_to_s(t->card3a2,0,s),CARD3A2_description[1]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[1],CARD3A2_to_s(t->card3a2,1,s),CARD3A2_description[5]);
    if(t->card3a1->ISOURC == 1)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[6],CARD3A2_to_s(t->card3a2,6,s),CARD3A2_description[13]);
    }
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[7],CARD3A2_to_s(t->card3a2,7,s),CARD3A2_description[14]);
  } else
  if(t->card3a1->IPARM == 10)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[0],CARD3A2_to_s(t->card3a2,0,s),CARD3A2_description[2]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[1],CARD3A2_to_s(t->card3a2,1,s),CARD3A2_description[6]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[2],CARD3A2_to_s(t->card3a2,2,s),CARD3A2_description[8]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[3],CARD3A2_to_s(t->card3a2,3,s),CARD3A2_description[9]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[5],CARD3A2_to_s(t->card3a2,5,s),CARD3A2_description[12]);
    if(t->card3a1->ISOURC == 1)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[6],CARD3A2_to_s(t->card3a2,6,s),CARD3A2_description[13]);
    }
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[7],CARD3A2_to_s(t->card3a2,7,s),CARD3A2_description[14]);
  } else
  if(t->card3a1->IPARM == 11)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[0],CARD3A2_to_s(t->card3a2,0,s),CARD3A2_description[2]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[1],CARD3A2_to_s(t->card3a2,1,s),CARD3A2_description[6]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[4],CARD3A2_to_s(t->card3a2,4,s),CARD3A2_description[10]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[5],CARD3A2_to_s(t->card3a2,5,s),CARD3A2_description[12]);
    if(t->card3a1->ISOURC == 1)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[6],CARD3A2_to_s(t->card3a2,6,s),CARD3A2_description[13]);
    }
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[7],CARD3A2_to_s(t->card3a2,7,s),CARD3A2_description[14]);
  } else
  if(t->card3a1->IPARM == 12)
  {
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[0],CARD3A2_to_s(t->card3a2,0,s),CARD3A2_description[3]);
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[1],CARD3A2_to_s(t->card3a2,1,s),CARD3A2_description[7]);
    if(t->card3a1->ISOURC == 1)
    {
      fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[6],CARD3A2_to_s(t->card3a2,6,s),CARD3A2_description[13]);
    }
    fprintf(fp,"%-4s %-18s %-15s # %s\n",CARD3A2_ID,CARD3A2_name[7],CARD3A2_to_s(t->card3a2,7,s),CARD3A2_description[14]);
  }
}

int CARD3A2_read(TAPE5_struct *t,FILE *fp)
{
  int n;
  int rt;
  char line[CARD_LENGTH];

  // read line
  if(fgets(line,CARD_LENGTH,fp) == NULL)
  {
    fprintf(stderr,"CARD3A2_read: read error.\n");
    return -1;
  }
  rt = 0;

  // read values
  n = 0;
  if(get_dbl(line,n,10,&t->card3a2->PARM1)  < 0) rt = 1; n += 10;
  if(get_dbl(line,n,10,&t->card3a2->PARM2)  < 0) rt = 2; n += 10;
  if(get_dbl(line,n,10,&t->card3a2->PARM3)  < 0) rt = 3; n += 10;
  if(get_dbl(line,n,10,&t->card3a2->PARM4)  < 0) rt = 4; n += 10;
  if(get_dbl(line,n,10,&t->card3a2->TIME)   < 0) rt = 5; n += 10;
  if(get_dbl(line,n,10,&t->card3a2->PSIPO)  < 0) rt = 6; n += 10;
  if(get_dbl(line,n,10,&t->card3a2->ANGLEM) < 0) rt = 7; n += 10;
  if(get_dbl(line,n,10,&t->card3a2->G)      < 0) rt = 8;
  if(rt)
  {
    fprintf(stderr,"CARD3A2_read: rt=%d\n",rt);
    rt = -1;
  }

  // check values
  if(CARD3A2_check_all(t) < 0)
  {
    rt = -1;
  }

  // check error
  if(rt)
  {
    fprintf(stderr,"CARD3A2_read: error in the following line.\n");
    fprintf(stderr,"%s\n",line);
  }

  return rt;
}

int CARD3A2_gets(char *line,TAPE5_struct *t)
{
  int i,n;
  double xtmp;
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
  if(strcmp(CARD3A2_ID,str[0]) != 0)
  {
    return 1;
  } else
  if(i < NCOL3)
  {
    return 0;
  }

  // check name
  n = -1;
  for(i=0; i<CARD3A2_NPAR; i++)
  {
    if(strcasecmp(str[1],CARD3A2_name[i]) == 0)
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
    case 7:
      errno = 0;
      xtmp = strtod(str[2],&p);
      if(errno==ERANGE || *p!='\0')
      {
        fprintf(stderr,"CARD1: Convert error >>> %s\n",line);
        return -1;
      }
      break;
  }

  // set value
  switch(n)
  {
    case 0:
      t->card3a2->PARM1 = xtmp;
      break;
    case 1:
      t->card3a2->PARM2 = xtmp;
      break;
    case 2:
      t->card3a2->PARM3 = xtmp;
      break;
    case 3:
      t->card3a2->PARM4 = xtmp;
      break;
    case 4:
      t->card3a2->TIME = xtmp;
      break;
    case 5:
      t->card3a2->PSIPO = xtmp;
      break;
    case 6:
      t->card3a2->ANGLEM = xtmp;
      break;
    case 7:
      t->card3a2->G = xtmp;
      break;
    default:
      return -1;
      break;
  }

  // check value
  if(CARD3A2_check(t->card3a2,n) < 0)
  {
    return -1;
  }

  return 0;
}

int CARD3A2_check_all(const TAPE5_struct *t)
{
  int n;
  int rt;

  rt = 0;
  for(n=0; n<CARD3A2_NPAR; n++)
  {
    if(CARD3A2_check(t->card3a2,n) < 0)
    {
      rt = -1;
    }
  }

  return rt;
}

int CARD3A2_check(const CARD3A2_struct *c,int n)
{
  int rt;

  rt = 0;
  switch(n)
  {
  /*
    case 0:
      if(strpbrk(c->MODTRN,"TM CKFL") == NULL)
      {
        fprintf(stderr,"CARD1: error in %s, %s\n",CARD3A2_name[n],c->MODTRN);
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

char *CARD3A2_to_s(const CARD3A2_struct *c,int n,char *s)
{
  switch(n)
  {
    case 0:
      sprintf(s,"%.5f",c->PARM1);
      break;
    case 1:
      sprintf(s,"%.7f",c->PARM2);
      break;
    case 2:
      sprintf(s,"%.3f",c->PARM3);
      break;
    case 3:
      sprintf(s,"%.3f",c->PARM4);
      break;
    case 4:
      sprintf(s,"%.3f",c->TIME);
      break;
    case 5:
      sprintf(s,"%.3f",c->PSIPO);
      break;
    case 6:
      sprintf(s,"%.3f",c->ANGLEM);
      break;
    case 7:
      sprintf(s,"%.3f",c->G);
      break;
    default:
      sprintf(s,"?");
      break;
  }

  return s;
}
