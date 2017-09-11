void CARD4s_write(const TAPE5_struct *t)
{
  int i,j,kmx;
  int flag;

  if(abs(t->card5->IRPT) != 3)
  {
    CARD4_write(t,stdout);
  }

  flag = 0;
  if(strncasecmp(t->card1->SURREF,"BRDF",1) == 0)
  {
    flag = 1;
  } else
  if(strncasecmp(t->card1->SURREF,"LAMBER",1) == 0)
  {
    flag = 2;
  }
  if(flag)
  {
    CARD4A_write(t,stdout);
    if(flag == 1)
    {
      for(i=0; i<t->card4a->NSURF; i++)
      {
        CARD4B1_write((t->card4b1)+i,stdout);
        CARD4B2_write((t->card4b2)+i,stdout);
        if(strcasecmp(((t->card4b1)+i)->CBRDF,"Rahman")==0 ||
           strcasecmp(((t->card4b1)+i)->CBRDF,"Roujean")==0)
        {
          kmx = 3;
        } else
        if(strcasecmp(((t->card4b1)+i)->CBRDF,"Ross-Li")==0)
        {
          kmx = 5;
        }
        else
        {
          kmx = 4;
        }
        for(j=0; j<t->card4b2->NWVSRF; j++)
        {
          CARD4B3_write((t->card4b3)+i*MWVSRF+j,kmx,stdout);
        }
      }
    } else
    if(flag == 2)
    {
      for(i=0; i<t->card4a->NSURF; i++)
      {
        CARD4L2_write((t->card4l2)+i,stdout);
      }
    }
  }
}

void CARD4s_print(const TAPE5_struct *t)
{
  int i,j,kmx;
  int flag;

  if(abs(t->card5->IRPT) != 3)
  {
    CARD4_print(t,stdout);
  }

  flag = 0;
  if(strncasecmp(t->card1->SURREF,"BRDF",1) == 0)
  {
    flag = 1;
  } else
  if(strncasecmp(t->card1->SURREF,"LAMBER",1) == 0)
  {
    flag = 2;
  }
  if(flag)
  {
    CARD4A_print(t,stdout);
    if(flag == 1)
    {
      for(i=0; i<t->card4a->NSURF; i++)
      {
        CARD4B1_print((t->card4b1)+i,i,stdout);
        CARD4B2_print((t->card4b2)+i,i,stdout);
        if(strcasecmp(((t->card4b1)+i)->CBRDF,"Rahman")==0 ||
           strcasecmp(((t->card4b1)+i)->CBRDF,"Roujean")==0)
        {
          kmx = 3;
        } else
        if(strcasecmp(((t->card4b1)+i)->CBRDF,"Ross-Li")==0)
        {
          kmx = 5;
        }
        else
        {
          kmx = 4;
        }
        for(j=0; j<t->card4b2->NWVSRF; j++)
        {
          CARD4B3_print((t->card4b3)+i*MWVSRF+j,i,j,kmx,stdout);
        }
      }
    } else
    if(flag == 2)
    {
      for(i=0; i<t->card4a->NSURF; i++)
      {
        CARD4L2_print((t->card4l2)+i,i,stdout);
      }
    }
  }
}

int CARD4s_read(TAPE5_struct *t,FILE *fp)
{
  int i,j,kmx;
  int flag;

  if(abs(t->card5->IRPT) != 3)
  {
    if(CARD4_read(t,fp) != 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD4>\n");
      CARD4_write(t,stderr);
    }
  }

  flag = 0;
  if(strncasecmp(t->card1->SURREF,"BRDF",1) == 0)
  {
    flag = 1;
  } else
  if(strncasecmp(t->card1->SURREF,"LAMBER",1) == 0)
  {
    flag = 2;
  }
  if(flag)
  {
    if(CARD4A_read(t,fp) != 0) return -1;
    if(vb)
    {
      fprintf(stderr,"<CARD4A>\n");
      CARD4A_write(t,stderr);
    }
    if(flag == 1)
    {
      for(i=0; i<t->card4a->NSURF; i++)
      {
        if(CARD4B1_read((t->card4b1)+i,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD4B1>\n");
          CARD4B1_write((t->card4b1)+i,stderr);
        }
        if(CARD4B2_read((t->card4b2)+i,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD4B2>\n");
          CARD4B2_write((t->card4b2)+i,stderr);
        }
        if(strcasecmp(((t->card4b1)+i)->CBRDF,"Rahman")==0 ||
           strcasecmp(((t->card4b1)+i)->CBRDF,"Roujean")==0)
        {
          kmx = 3;
        } else
        if(strcasecmp(((t->card4b1)+i)->CBRDF,"Ross-Li")==0)
        {
          kmx = 5;
        }
        else
        {
          kmx = 4;
        }
        for(j=0; j<t->card4b2->NWVSRF; j++)
        {
          if(CARD4B3_read((t->card4b3)+i*MWVSRF+j,kmx,fp) < 0) return -1;
          if(vb)
          {
            fprintf(stderr,"<CARD4B3>\n");
            CARD4B3_write((t->card4b3)+i*MWVSRF+j,kmx,stderr);
          }
        }
      }
    } else
    if(flag == 2)
    {
      if(CARD4L1_read(t,fp) < 0) return -1;
      if(vb)
      {
        fprintf(stderr,"<CARD4L1>\n");
        CARD4L1_write(t,stderr);
      }
      for(i=0; i<t->card4a->NSURF; i++)
      {
        if(CARD4L2_read((t->card4l2)+i,fp) < 0) return -1;
        if(vb)
        {
          fprintf(stderr,"<CARD4L2>\n");
          CARD4L2_write((t->card4l2)+i,stderr);
        }
      }
    }
  }

  return 0;
}
