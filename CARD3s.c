void CARD3s_write(const TAPE5_struct *t)
{
  CARD3_write(t,stdout);
  if(t->card1->IEMSCT == 2)
  {
    CARD3A1_write(t,stdout);
    CARD3A2_write(t,stdout);
    if(t->card3a1->IPH == 1)
    {
      CARD3B1_write(t,stdout);
      if(t->card3b1->NWLF > 0)
      {
        CARD3C1_write(t,stdout);
        CARD3C2_write(t,stdout);
        CARD3C3_write(t,stdout);
        CARD3C4_write(t,stdout);
        CARD3C5_write(t,stdout);
        CARD3C6_write(t,stdout);
      }
      else
      {
        CARD3B2_write(t,stdout);
      }
    }
  }
}

void CARD3s_print(const TAPE5_struct *t)
{
  CARD3_print(t,stdout);
  if(t->card1->IEMSCT == 2)
  {
    CARD3A1_print(t,stdout);
    CARD3A2_print(t,stdout);
    if(t->card3a1->IPH == 1)
    {
      CARD3B1_print(t,stdout);
      if(t->card3b1->NWLF > 0)
      {
        CARD3C1_print(t,stdout);
        CARD3C2_print(t,stdout);
        CARD3C3_print(t,stdout);
        CARD3C4_print(t,stdout);
        CARD3C5_print(t,stdout);
        CARD3C6_print(t,stdout);
      }
      else
      {
        CARD3B2_print(t,stdout);
      }
    }
  }
}

int CARD3s_read(TAPE5_struct *t,FILE *fp)
{
  if(CARD3_read(t,fp) < 0) return -1;
  if(vb)
  {
    fprintf(stderr,"<CARD3>\n");
    CARD3_write(t,stderr);
  }
  if(t->card1->IEMSCT == 2)
  {
    if(CARD3A1_read(t,fp) < 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD3A1>\n");
      CARD3A1_write(t,stderr);
    }
    if(CARD3A2_read(t,fp) < 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD3A2>\n");
      CARD3A2_write(t,stderr);
    }
    if(t->card3a1->IPH == 1)
    {
      if(CARD3B1_read(t,fp) < 0) return -1;
      if(vb)
      {
        fprintf(stderr,"<CARD3B1>\n");
        CARD3B1_write(t,stderr);
      }
      if(t->card3b1->NWLF > 0)
      {
        if(CARD3C1_read(t,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD3C1>\n");
          CARD3C1_write(t,stderr);
        }
        if(CARD3C2_read(t,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD3C2>\n");
          CARD3C2_write(t,stderr);
        }
        if(CARD3C3_read(t,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD3C3>\n");
          CARD3C3_write(t,stderr);
        }
        if(CARD3C4_read(t,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD3C4>\n");
          CARD3C4_write(t,stderr);
        }
        if(CARD3C5_read(t,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD3C5>\n");
          CARD3C5_write(t,stderr);
        }
        if(CARD3C6_read(t,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD3C6>\n");
          CARD3C6_write(t,stderr);
        }
      }
      else
      {
        if(CARD3B2_read(t,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD3B2>\n");
          CARD3B2_write(t,stderr);
        }
      }
    }
  }

  return 0;
}
