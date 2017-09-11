void CARD1s_write(const TAPE5_struct *t)
{
  CARD1_write(t,stdout);
  CARD1A_write(t,stdout);
  if(t->card1a->LSUNFL[0] == 'T')
  {
    CARD1A1_write(t,stdout);
  }
  if(t->card1a->LBMNAM[0] == 'T')
  {
    CARD1A2_write(t,stdout);
  }
  if(t->card1a->LFLTNM[0] == 'T')
  {
    CARD1A3_write(t,stdout);
  }
}

void CARD1s_print(const TAPE5_struct *t)
{
  CARD1_print(t,stdout);
  CARD1A_print(t,stdout);
  if(t->card1a->LSUNFL[0] == 'T')
  {
    CARD1A1_print(t,stdout);
  }
  if(t->card1a->LBMNAM[0] == 'T')
  {
    CARD1A2_print(t,stdout);
  }
  if(t->card1a->LFLTNM[0] == 'T')
  {
    CARD1A3_print(t,stdout);
  }
}

int CARD1s_read(TAPE5_struct *t,FILE *fp)
{
  if(CARD1_read(t,fp) != 0) return -1;
  if(vb)
  {
    fprintf(stderr,"<CARD1>\n");
    CARD1_write(t,stderr);
  }
  if(CARD1A_read(t,fp) != 0) return -1;
  if(vb)
  {
    fprintf(stderr,"<CARD1A>\n");
    CARD1A_write(t,stderr);
  }
  if(t->card1a->LSUNFL[0] == 'T')
  {
    if(CARD1A1_read(t,fp) != 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD1A1>\n");
      CARD1A1_write(t,stderr);
    }
  }
  if(t->card1a->LBMNAM[0] == 'T')
  {
    if(CARD1A2_read(t,fp) != 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD1A2>\n");
      CARD1A2_write(t,stderr);
    }
  }
  if(t->card1a->LFLTNM[0] == 'T')
  {
    if(CARD1A3_read(t,fp) != 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD1A3>\n");
      CARD1A3_write(t,stderr);
    }
  }

  return 0;
}
