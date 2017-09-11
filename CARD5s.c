void CARD5s_write(const TAPE5_struct *t)
{
  CARD5_write(t,stdout);
}

void CARD5s_print(const TAPE5_struct *t)
{
  CARD5_print(t,stdout);
}

int CARD5s_read(TAPE5_struct *t,FILE *fp)
{
  if(CARD5_read(t,fp) != 0) return -1;
  if(vb)
  {
    fprintf(stderr,"<CARD5>\n");
    CARD5_write(t,stderr);
  }

  return 0;
}
