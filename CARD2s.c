void CARD2s_write(const TAPE5_struct *t)
{
  int i;

  CARD2_write(t,stdout);
  if(strcasecmp(t->card2->APLUS,"A+") == 0)
  {
    CARD2AP_write(t,stdout);
  }
  if(t->card2->ICLD>0 && t->card2->ICLD!=11)
  {
    CARD2A_write(t,stdout);
  }
  if(t->card2->IVSA == 1)
  {
    CARD2B_write(t,stdout);
  }
  switch(mod_v)
  {
    case MOD_V4:
      if((t->card1->MODEL==0||t->card1->MODEL==7||t->card1->MODEL==8) && t->card1->IM==1)
      {
        CARD2C_write(t,stdout);
        for(i=0; i<t->card2c->ML; i++)
        {
          CARD2C1_write((t->card2c1)+i,stdout);
          if(t->card2c->IRD1 == 1)
          {
            CARD2C2_write((t->card2c2)+i,stdout);
            if(t->card1->MDEF == 2)
            {
              CARD2C2X_write((t->card2c2x)+i,stdout);
            }
          }
          if(t->card2c->IRD2==1 || t->card2c->IRD2==2)
          {
            CARD2C3_write(t,i,stdout);
          }
        }
      }
      break;
    case MOD_V5:
      if((t->card1->MODEL==0||t->card1->MODEL==7||t->card1->MODEL==8) && t->card1->I_RD2C==1)
      {
        CARD2C_write(t,stdout);
        for(i=0; i<t->card2c->ML; i++)
        {
          CARD2C1_write((t->card2c1)+i,stdout);
          if(t->card2c->IRD1 == 1)
          {
            CARD2C2_write((t->card2c2)+i,stdout);
            if(t->card1->MDEF == 2)
            {
              CARD2C2X_write((t->card2c2x)+i,stdout);
            }
          }
          if(t->card2c->IRD2==1 || t->card2c->IRD2==2)
          {
            CARD2C3_write(t,i,stdout);
          }
        }
      }
      break;
  }
  if(t->card2->IHAZE==7 || t->card2->ICLD==11 || strncmp(t->card2->ARUSS,"USS",3)==0)
  {
    CARD2D_write(t,stdout);
    for(i=0; i<MXIREG; i++)
    {
      if(t->card2d->IREG[i] > 0)
      {
        CARD2D1_write((t->card2d1)+i,stdout);
        if(t->card2d->IREG[i] > 1)
        {
          CARD2D2_JMAX = t->card2d->IREG[i];
        }
        else
        {
          CARD2D2_JMAX = MXNSPC;
        }
        CARD2D2_write((t->card2d2)+i*MXNSPC,stdout);
      }
    }
  }
  if((t->card2->ICLD>0&&t->card2->ICLD<11) && t->card2a->NCRALT>=3)
  {
    CARD2E1_write(t,stdout);
  }
  if((t->card2->ICLD>0&&t->card2->ICLD<11) && t->card2a->NCRSPC>=1)
  {
    CARD2E2_write(t,stdout);
  }
}

void CARD2s_print(const TAPE5_struct *t)
{
  int i,j,jmx;

  CARD2_print(t,stdout);
  if(strcasecmp(t->card2->APLUS,"A+") == 0)
  {
    CARD2AP_print(t,stdout);
  }
  if(t->card2->ICLD>0 && t->card2->ICLD!=11)
  {
    CARD2A_print(t,stdout);
  }
  if(t->card2->IVSA == 1)
  {
    CARD2B_print(t,stdout);
  }
  if((t->card1->MODEL==0||t->card1->MODEL==7||t->card1->MODEL==8) && t->card1->IM==1)
  {
    CARD2C_print(t,stdout);
    for(i=0; i<t->card2c->ML; i++)
    {
      CARD2C1_print((t->card2c1)+i,i,stdout);
      if(t->card2c->IRD1 == 1)
      {
        CARD2C2_print((t->card2c2)+i,i,stdout);
        if(t->card1->MDEF == 2)
        {
          CARD2C2X_print((t->card2c2x)+i,i,stdout);
        }
      }
      if(t->card2c->IRD2==1 || t->card2c->IRD2==2)
      {
        CARD2C3_print(t,i,stdout);
      }
    }
  }
  if(t->card2->IHAZE==7 || t->card2->ICLD==11 || strncmp(t->card2->ARUSS,"USS",3)==0)
  {
    CARD2D_print(t,stdout);
    for(i=0; i<MXIREG; i++)
    {
      if(t->card2d->IREG[i] > 0)
      {
        CARD2D1_print((t->card2d1)+i,i,stdout);
        if(t->card2d->IREG[i] > 1)
        {
          jmx = t->card2d->IREG[i];
        }
        else
        {
          jmx = MXNSPC;
        }
        for(j=0; j<jmx; j++)
        {
          CARD2D2_print((t->card2d2)+i*MXNSPC+j,i,j,stdout);
        }
      }
    }
  }
  if((t->card2->ICLD>0&&t->card2->ICLD<11) && t->card2a->NCRALT>=3)
  {
    CARD2E1_print(t,stdout);
  }
  if((t->card2->ICLD>0&&t->card2->ICLD<11) && t->card2a->NCRSPC>=1)
  {
    CARD2E2_print(t,stdout);
  }
}

int CARD2s_read(TAPE5_struct *t,FILE *fp)
{
  int i;

  if(CARD2_read(t,fp) < 0) return -1;
  if(vb)
  {
    fprintf(stderr,"<CARD2>\n");
    CARD2_write(t,stderr);
  }
  if(strcasecmp(t->card2->APLUS,"A+") == 0)
  {
    if(CARD2AP_read(t,fp) < 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD2AP>\n");
      CARD2AP_write(t,stderr);
    }
  }
  if(t->card2->ICLD>0 && t->card2->ICLD!=11)
  {
    if(CARD2A_read(t,fp) < 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD2A>\n");
      CARD2A_write(t,stderr);
    }
  }
  if(t->card2->IVSA == 1)
  {
    if(CARD2B_read(t,fp) < 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD2B>\n");
      CARD2B_write(t,stderr);
    }
  }
  if((t->card1->MODEL==0||t->card1->MODEL==7||t->card1->MODEL==8) && t->card1->IM==1)
  {
    if(CARD2C_read(t,fp) < 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD2C>\n");
      CARD2C_write(t,stderr);
    }
    for(i=0; i<t->card2c->ML; i++)
    {
      if(CARD2C1_read((t->card2c1)+i,fp) < 0) return -1;
      if(vb)
      {
        fprintf(stderr,"<CARD2C1>\n");
        CARD2C1_write((t->card2c1)+i,stderr);
      }
      if(t->card2c->IRD1 == 1)
      {
        if(CARD2C2_read((t->card2c2)+i,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD2C2>\n");
          CARD2C2_write((t->card2c2)+i,stderr);
        }
        if(t->card1->MDEF == 2)
        {
          if(CARD2C2X_read((t->card2c2x)+i,fp) < 0) return -1;
          if(vb)
          {
            fprintf(stderr,"<CARD2C2X>\n");
            CARD2C2X_write((t->card2c2x)+i,stderr);
          }
        }
      }
      if(t->card2c->IRD2==1 || t->card2c->IRD2==2)
      {
        if(CARD2C3_read(t,i,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD2C3>\n");
          CARD2C3_write(t,i,stderr);
        }
      }
    }
  }
  if(t->card2->IHAZE==7 || t->card2->ICLD==11 || strncmp(t->card2->ARUSS,"USS",3)==0)
  {
    if(CARD2D_read(t,fp) < 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD2D>\n");
      CARD2D_write(t,stderr);
    }
    for(i=0; i<MXIREG; i++)
    {
      if(t->card2d->IREG[i] > 0)
      {
        if(CARD2D1_read((t->card2d1)+i,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD2D1>\n");
          CARD2D1_write((t->card2d1)+i,stderr);
        }
        if(t->card2d->IREG[i] > 1)
        {
          CARD2D2_JMAX = t->card2d->IREG[i];
        }
        else
        {
          CARD2D2_JMAX = MXNSPC;
        }
        if(CARD2D2_read((t->card2d2)+i*MXNSPC,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD2D2>\n");
          CARD2D2_write((t->card2d2)+i*MXNSPC,stderr);
        }
      }
    }
  }
  if((t->card2->ICLD>0&&t->card2->ICLD<11) && t->card2a->NCRALT>=3)
  {
    if(CARD2E1_read(t,fp) < 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD2E1>\n");
      CARD2E1_write(t,stderr);
    }
  }
  if((t->card2->ICLD>0&&t->card2->ICLD<11) && t->card2a->NCRSPC>=1)
  {
    if(CARD2E2_read(t,fp) < 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD2E2>\n");
      CARD2E2_write(t,stderr);
    }
  }

  return 0;
}
