int WriteTape5_V5(double *par,int mode);
int WriteCard1_V5(FILE *fp,int mode);
int WriteCard2_V5(FILE *fp,int mode);
int WriteCard2C_V5(FILE *fp);
int WriteCard2D_V5(FILE *fp);
int WriteCard3_V5(FILE *fp,int mode,double *par);
int WriteCard3C_V5(FILE *fp);
int WriteCard4_V5(FILE *fp,int mode,double *par);
int WriteCard5_V5(FILE *fp);
int RunModtran_V5(void);

int WriteTape5_V5(double *par,int mode)
{
  int err;
  FILE *fp;
  char fnam[] = "WriteTape5_V5";

  if((fp=fopen("modtran.tp5","w")) == NULL)
  {
    fprintf(stderr,"Error, cannot open %s\n","modtran.tp5");
    return -1;
  }

  sim_mdef = ((sim_2c&0x0100)>0?2:0);
  sim_im = (sim_2c>0?1:0);
  err = 0;
  switch(mode)
  {
    case SIM_MODE_DSR_MODTRAN_BO: // DSR simulation with MODTRAN aerosol model
    case SIM_MODE_DSR_CUSTOM_BO:  // DSR simulation with custom  aerosol model
    case SIM_MODE_SSR_MODTRAN_SO: // SSR simulation with MODTRAN aerosol model
    case SIM_MODE_SSR_MODTRAN_BO: // SSR simulation with MODTRAN aerosol model
    case SIM_MODE_SSR_CUSTOM_SO:  // SSR simulation with custom  aerosol model
    case SIM_MODE_SSR_CUSTOM_BO:  // SSR simulation with custom  aerosol model
    case SIM_MODE_TRN_MODTRAN_BO: // Transmittance  with MODTRAN aerosol model
    case SIM_MODE_TRN_CUSTOM_BO:  // Transmittance  with custom  aerosol model
      // CARD 1
      if(WriteCard1_V5(fp,mode) < 0)
      {
        err = 1;
        break;
      }
      // CARD 2
      if(WriteCard2_V5(fp,mode) < 0)
      {
        err = 1;
        break;
      }
      // CARD 3
      if(WriteCard3_V5(fp,mode,par) < 0)
      {
        err = 1;
        break;
      }
      // CARD 4
      if(WriteCard4_V5(fp,mode,par) < 0)
      {
        err = 1;
        break;
      }
      // CARD 5
      if(WriteCard5_V5(fp) < 0)
      {
        err = 1;
        break;
      }
      break;
    default:
      fprintf(stderr,"%s: invalid mode >>> %d\n",fnam,mode);
      err = 1;
      break;
  }
  fclose(fp);
  if(err)
  {
    return -1;
  }

  return 0;
}

int WriteCard1_V5(FILE *fp,int mode)
{
  char sim_lymolc = '+';
  char fnam[] = "WriteCard1_V5";

  if(mode==SIM_MODE_DSR_MODTRAN_BO || mode==SIM_MODE_DSR_CUSTOM_BO)
  {
    sim_iemsct = 3;
  } else
  if(mode==SIM_MODE_SSR_MODTRAN_SO || mode==SIM_MODE_SSR_MODTRAN_BO ||
     mode==SIM_MODE_SSR_CUSTOM_SO  || mode==SIM_MODE_SSR_CUSTOM_BO)
  {
    if(sim_th_sun > 100.0)
    {
      sim_iemsct = 1;
    }
    else
    {
      sim_iemsct = 2;
    }
  } else
  if(mode==SIM_MODE_TRN_MODTRAN_BO || mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    sim_iemsct = 0;
    sim_yuni = 1.0;
  }
  else
  {
    fprintf(stderr,"%s: invalid mode >>> %d\n",fnam,mode);
    return -1;
  }
  if(sim_2c > 0)
  {
    sim_iatm = 7;
    if((sim_2c&0x0200) == 0)
    {
      sim_lymolc = ' ';
    }
  }

  // CARD 1
  if(sim_salb > 0)
  {
    fprintf(fp,"%c%cF%c%d%5d%5d%5d    0    0    0    0    0    0%5d%5d %4d    .000 LAMBER\n",
                sim_modtran,sim_speed,
                sim_lymolc,sim_iatm,sim_itype,sim_iemsct,sim_mult,sim_mdef,sim_im,sim_prnt);
  }
  else
  {
    fprintf(fp,"%c%cF%c%d%5d%5d%5d    0    0    0    0    0    0%5d%5d %4d    .000%7.4f\n",
                sim_modtran,sim_speed,
                sim_lymolc,sim_iatm,sim_itype,sim_iemsct,sim_mult,sim_mdef,sim_im,sim_prnt,sim_grho);
  }
  if(sim_iemsct == 3)
  {
    // CARD 1A
    #ifdef OLDMODE
    if(isnan(sim_solcon))
    {
      cprintf(fp,"tt   8 0.0%10.3f%10.0f%10.0f0%s t f %s\n",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"tt   8 0.0%10.3f%10.0f%10.0f0%s t f %s  %10.0f\n",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f",sim_solcon);
    }
    #else
    // To increase the precision, cd1a.f line 218 must be modified as following:
    // READ(IRD,'(A3,I2,A1,F4.0,F15.5,2A15,2A1,4(1X,A1),F10.0,A1,    &
    // Also, size of H2OSTR/O3STR must be changed to 15 in cd1a.f, cd2c.f, cd4.f,
    // cd5.f, driver.f, fnames.f, initcd.f, sclcol.f, wrtbuf.f, wtchan.f.
    if(isnan(sim_solcon))
    {
      cprintf(fp,"tt   8 0.0%15.8f%15.5f%15.5f0%s t f %s\n",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"tt   8 0.0%15.8f%15.5f%15.5f0%s t f %s  %10.0f\n",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f",sim_solcon);
    }
    #endif
    if(strcmp(sim_fsun,NONAME) != 0)
    {
      // CARD 1A1
      fprintf(fp,"%s\n",sim_fsun);
    }
    // CARD 1A2
    if(sim_dbmp < 0)
    {
      fprintf(fp,"p%d_2008\n",-sim_dbmp);
    }
    else
    {
      fprintf(fp,"%02d_2008\n",sim_dbmp);
    }
  } else
  if(sim_iemsct == 0)
  {
    // CARD 1A
    #ifdef OLDMODE
    if(isnan(sim_solcon))
    {
      cprintf(fp,"tt %3d 0.0%10.3f+%9.0f%10.0f0%s t f %s\n",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"tt %3d 0.0%10.3f+%9.0f%10.0f0%s t f %s  %10.0f\n",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f",sim_solcon);
    }
    #else
    // To increase the precision, cd1a.f line 218 must be modified as following:
    // READ(IRD,'(A3,I2,A1,F4.0,F15.5,2A15,2A1,4(1X,A1),F10.0,A1,    &
    // Also, size of H2OSTR/O3STR must be changed to 15 in cd1a.f, cd2c.f, cd4.f,
    // cd5.f, driver.f, fnames.f, initcd.f, sclcol.f, wrtbuf.f, wtchan.f.
    if(isnan(sim_solcon))
    {
      cprintf(fp,"tt %3d 0.0%15.8f+%14.5f%15.5f0%s t f %s\n",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"tt %3d 0.0%15.8f+%14.5f%15.5f0%s t f %s  %10.0f\n",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f",sim_solcon);
    }
    #endif
    if(strcmp(sim_fsun,NONAME) != 0)
    {
      // CARD 1A1
      fprintf(fp,"%s\n",sim_fsun);
    }
    // CARD 1A2
    if(sim_dbmp < 0)
    {
      fprintf(fp,"p%d_2008\n",-sim_dbmp);
    }
    else
    {
      fprintf(fp,"%02d_2008\n",sim_dbmp);
    }
  }
  else
  {
    // CARD 1A
    #ifdef OLDMODE
    if(isnan(sim_solcon))
    {
      cprintf(fp,"%st %3d 0.0%10.3f%10.0f%10.0f0%s t f %s\n",
                  sim_dflg==0?"f":"t",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"%st %3d 0.0%10.3f%10.0f%10.0f0%s t f %s  %10.0f\n",
                  sim_dflg==0?"f":"t",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f",sim_solcon);
    }
    #else
    // To increase the precision, cd1a.f line 218 must be modified as following:
    // READ(IRD,'(A3,I2,A1,F4.0,F15.5,2A15,2A1,4(1X,A1),F10.0,A1,    &
    // Also, size of H2OSTR/O3STR must be changed to 15 in cd1a.f, cd2c.f, cd4.f,
    // cd5.f, driver.f, fnames.f, initcd.f, sclcol.f, wrtbuf.f, wtchan.f.
    if(isnan(sim_solcon))
    {
      cprintf(fp,"%st %3d 0.0%15.8f%15.5f%15.5f0%s t f %s\n",
                  sim_dflg==0?"f":"t",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"%st %3d 0.0%15.8f%15.5f%15.5f0%s t f %s  %10.0f\n",
                  sim_dflg==0?"f":"t",
                  sim_nstr,sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f",sim_solcon);
    }
    #endif
    if(strcmp(sim_fsun,NONAME) != 0)
    {
      // CARD 1A1
      fprintf(fp,"%s\n",sim_fsun);
    }
    // CARD 1A2
    if(sim_sbmp < 0)
    {
      fprintf(fp,"p%d_2008\n",-sim_sbmp);
    }
    else
    {
      fprintf(fp,"%02d_2008\n",sim_sbmp);
    }
  }

  return 0;
}

int WriteCard2_V5(FILE *fp,int mode)
{
  // CARD 2
  #ifdef OLDMODE
  if(mode==SIM_MODE_DSR_CUSTOM_BO || mode==SIM_MODE_SSR_CUSTOM_SO || mode==SIM_MODE_SSR_CUSTOM_BO ||
     mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    cprintf(fp,"  %3d    0USS%2d    0   %2d    0%10.5f%10.5f%10.5f%10.5f%10.5f\n",
                sim_ihaz,sim_ivul,sim_icld,sim_vmie,ZERO,ZERO,ZERO,ZERO);
  }
  else
  {
    cprintf(fp,"  %3d    0   %2d    0   %2d    0%10.5f%10.5f%10.5f%10.5f%10.5f\n",
                sim_ihaz,sim_ivul,sim_icld,sim_vmie,ZERO,ZERO,ZERO,ZERO);
  }
  #else
  // To increase the precision, cd2.f line 132 must be modified as following:
  // READ(IRD,'(A2,I3,A1,I4,A3,I2,3I5,5F15.5)')
  if(mode==SIM_MODE_DSR_CUSTOM_BO || mode==SIM_MODE_SSR_CUSTOM_SO || mode==SIM_MODE_SSR_CUSTOM_BO ||
     mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    cprintf(fp,"  %3d    0USS%2d    0   %2d    0%15.10f%15.10f%15.10f%15.10f%15.10f\n",
                sim_ihaz,sim_ivul,sim_icld,sim_vmie,ZERO,ZERO,ZERO,ZERO);
  }
  else
  {
    cprintf(fp,"  %3d    0   %2d    0   %2d    0%15.10f%15.10f%15.10f%15.10f%15.10f\n",
                sim_ihaz,sim_ivul,sim_icld,sim_vmie,ZERO,ZERO,ZERO,ZERO);
  }
  #endif
  if(sim_icld > 0)
  {
    // CARD 2A
    if(sim_icld==18 || sim_icld==19)
    {
      cprintf(fp,"%8.3f%8.3f%8.3f\n",cld_thik,cld_alt,cld_ext);
    } else
    if(sim_icld>0 && sim_icld<11)
    {
      cprintf(fp,"%8.3f%8.3f%8.3f%4d%4d%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f\n",
                  (cld_thik>EPSILON?cld_thik:-9.0),
                  (cld_alt>EPSILON?cld_alt:-9.0),
                  (cld_ext>EPSILON?cld_ext:-9.0),
                  -9,-9,-9.0,-9.0,-9.0,-9.0,-9.0,-9.0);
    }
  }
  if(sim_2c>0 && sim_im>0)
  {
    // CARD 2C
    WriteCard2C_V5(fp);
  }
  if(mode==SIM_MODE_DSR_CUSTOM_BO || mode==SIM_MODE_SSR_CUSTOM_SO || mode==SIM_MODE_SSR_CUSTOM_BO ||
     mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    // CARD 2D
    WriteCard2D_V5(fp);
  }

  return 0;
}

int WriteCard2C_V5(FILE *fp)
{
  int i;
  int f2,f3,f4,f5;
  char jchar[15] = "";
  char jcharx[2] = " ";
  char jchary[2] = " ";

  if(sim_2c <= 0)
  {
    return 0;
  }
  else
  {
    f2 = (sim_2c&0x0010)|(sim_2c&0x0100)|(sim_2c&0x0200);
    f3 = (sim_2c&0x0100);
    f4 = (sim_2c&0x0200);
    f5 = (sim_2c&0x1000);
  }

  for(i=0; i<PRF_NPAR; i++)
  {
    jchar[i] = sim_prf_jchar[i];
  }
  jchar[i] = '\0';
  jcharx[0] = sim_prf_jcharx;
  jchary[0] = sim_prf_jchary;

  // CARD 2C
  fprintf(fp,"%5d%5d%5d%20s%10.0f%5d\n",prf_n_alt,(f2>0?1:0),(f5>0?2:0),"Custom",sim_ro,0);
  for(i=0; i<prf_n_alt; i++)
  {
    // CARD 2C1
    #ifdef OLDMODE
    fprintf(fp,"%10.3f%10.3e%10.3e%10.3e%10.3e%10.3e%14s %1s%1s\n",
                prf_alt[i],sim_prf_pres[i],sim_prf_temp[i],
                sim_prf_wmol[0][i],sim_prf_wmol[1][i],sim_prf_wmol[2][i],jchar,jcharx,jchary);
    #else
    // To increase the precision, aernsm.f line 340 must be modified as following:
    // READ(IRD,'(6F15.5,A17)')ZM(K),                        &
    fprintf(fp,"%15.8f%15.8e%15.8e%15.8e%15.8e%15.8e%14s %1s%1s\n",
                prf_alt[i],sim_prf_pres[i],sim_prf_temp[i],
                sim_prf_wmol[0][i],sim_prf_wmol[1][i],sim_prf_wmol[2][i],jchar,jcharx,jchary);
    #endif
    if(f2 > 0)
    {
      // CARD 2C2
      #ifdef OLDMODE
      fprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_wmol[3][i],sim_prf_wmol[4][i],sim_prf_wmol[5][i],sim_prf_wmol[6][i],
                  sim_prf_wmol[7][i],sim_prf_wmol[8][i],sim_prf_wmol[9][i],sim_prf_wmol[10][i]);
      fprintf(fp,"%10.3e\n",sim_prf_wmol[11][i]);
      #else
      // To increase the precision, aernsm.f line 349 must be modified as following:
      // READ(IRD,'((8F15.5))')(WMOL(I),I=4,12)
      fprintf(fp,"%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e\n",
                  sim_prf_wmol[3][i],sim_prf_wmol[4][i],sim_prf_wmol[5][i],sim_prf_wmol[6][i],
                  sim_prf_wmol[7][i],sim_prf_wmol[8][i],sim_prf_wmol[9][i],sim_prf_wmol[10][i]);
      fprintf(fp,"%15.8e\n",sim_prf_wmol[11][i]);
      #endif
    }
    if(f3 > 0)
    {
      // CARD 2C2X
      #ifdef OLDMODE
      fprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_xmol[0][i],sim_prf_xmol[1][i],sim_prf_xmol[2][i],sim_prf_xmol[3][i],
                  sim_prf_xmol[4][i],sim_prf_xmol[5][i],sim_prf_xmol[6][i],sim_prf_xmol[7][i]);
      fprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_xmol[8][i],sim_prf_xmol[9][i],sim_prf_xmol[10][i],sim_prf_xmol[11][i],
                  sim_prf_xmol[12][i]);
      #else
      // To increase the precision, aernsm.f line 359 must be modified as following:
      // READ(IRD,'((8F15.5))')(WMOLX(I),I=1,NMOLX)
      fprintf(fp,"%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e\n",
                  sim_prf_xmol[0][i],sim_prf_xmol[1][i],sim_prf_xmol[2][i],sim_prf_xmol[3][i],
                  sim_prf_xmol[4][i],sim_prf_xmol[5][i],sim_prf_xmol[6][i],sim_prf_xmol[7][i]);
      fprintf(fp,"%15.8e%15.8e%15.8e%15.8e%15.8e\n",
                  sim_prf_xmol[8][i],sim_prf_xmol[9][i],sim_prf_xmol[10][i],sim_prf_xmol[11][i],
                  sim_prf_xmol[12][i]);
      #endif
    }
    if(f4 > 0)
    {
      // CARD 2C2Y
      #ifdef OLDMODE
      fprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_ymol[0][i],sim_prf_ymol[1][i],sim_prf_ymol[2][i],sim_prf_ymol[3][i],
                  sim_prf_ymol[4][i],sim_prf_ymol[5][i],sim_prf_ymol[6][i],sim_prf_ymol[7][i]);
      fprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_ymol[8][i],sim_prf_ymol[9][i],sim_prf_ymol[10][i],sim_prf_ymol[11][i],
                  sim_prf_ymol[12][i],sim_prf_ymol[13][i],sim_prf_ymol[14][i],sim_prf_ymol[15][i]);
      #else
      // To increase the precision, aernsm.f line 370 must be modified as following:
      // READ(IRD,'((8F15.5))')(WMOLYS(I),I=1,NMOLYC)
      fprintf(fp,"%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e\n",
                  sim_prf_ymol[0][i],sim_prf_ymol[1][i],sim_prf_ymol[2][i],sim_prf_ymol[3][i],
                  sim_prf_ymol[4][i],sim_prf_ymol[5][i],sim_prf_ymol[6][i],sim_prf_ymol[7][i]);
      fprintf(fp,"%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e\n",
                  sim_prf_ymol[8][i],sim_prf_ymol[9][i],sim_prf_ymol[10][i],sim_prf_ymol[11][i],
                  sim_prf_ymol[12][i],sim_prf_ymol[13][i],sim_prf_ymol[14][i],sim_prf_ymol[15][i]);
      #endif
    }
    if(f5 > 0)
    {
      // CARD 2C3
      #ifdef OLDMODE
      cprintf(fp,"%10s%10.0f%10s%10.0f%10.0f%10.0f%10.0f\n","",sim_prf_haze[0][i],"",
                  0.0,sim_prf_haze[1][i],sim_prf_haze[2][i],sim_prf_haze[3][i]);
      #else
      // To increase the precision, aernsm.f line 417 must be modified as following:
      // READ(IRD,'(10X,F15.5,10X,4F15.5)')                &
      cprintf(fp,"%10s%15.5f%10s%15.5f%15.5f%15.5f%15.5f\n","",sim_prf_haze[0][i],"",
                  0.0,sim_prf_haze[1][i],sim_prf_haze[2][i],sim_prf_haze[3][i]);
      #endif
    }
  }

  return 0;
}

int WriteCard2D_V5(FILE *fp)
{
  int i;

  // CARD 2D
  fprintf(fp,"%5d%5d%5d%5d\n",mie_n_wlen,0,0,0);
  // CARD 2D1
  fprintf(fp,"%10.3e%70s\n",0.0,"title");
  // CARD 2D2
  for(i=0; i<mie_n_wlen; i++)
  {
    #ifdef OLDMODE
    cprintf(fp,"%6.2f%7.5f%7.5f%6.4f%s",mie_wlen_um[i],mie_aext[i]/mie_aext[mie_iref],
                                        (mie_aext[i]-mie_asca[i])/mie_aext[mie_iref],mie_asym[i],
                                        i==mie_n_wlen-1?"\n":(i%3)==2?"\n":"");
    #else
    // To increase the precision, aruexa.f line 85 must be modified as following:
    // READ(IRD,'(3(F15.11,2F15.13,F15.13))',IOSTAT=IOS) &
    cprintf(fp,"%15.11f%15.13f%15.13f%15.13f%s",mie_wlen_um[i],mie_aext[i]/mie_aext[mie_iref],
                                                (mie_aext[i]-mie_asca[i])/mie_aext[mie_iref],mie_asym[i],
                                                i==mie_n_wlen-1?"\n":(i%3)==2?"\n":"");
    #endif
  }

  return 0;
}

int WriteCard3_V5(FILE *fp,int mode,double *par)
{
  // DSR_MODTRAN_BO || DSR_CUSTOM_BO || SSR_MODTRAN_BO || SSR_CUSTOM_BO || SSR_MODTRAN_S0 || SSR_CUSTOM_S0
  // par[2] = solar zenith
  // SSR_MODTRAN_BO || SSR_CUSTOM_BO || SSR_MODTRAN_S0 || SSR_CUSTOM_S0
  // par[3] = solar azimuth
  // par[4] = los   zenith
  // par[5] = los   azimuth
  // TRN_MODTRAN_BO || TRN_CUSTOM_BO
  // par[2] = los   zenith
  double ph;
  char fnam[] = "WriteCard3_V5";

  if(mode==SIM_MODE_DSR_MODTRAN_BO || mode==SIM_MODE_DSR_CUSTOM_BO)
  {
    // CARD 3
    fprintf(fp,"%10.4f%10.4f%10.6f%5d%5s%10.3f%5d%10.3f\n",
                sim_h1,sim_h2,par[2],sim_iday,"",sim_ro,0,ZERO);
  } else
  if(mode==SIM_MODE_SSR_MODTRAN_SO || mode==SIM_MODE_SSR_MODTRAN_BO ||
     mode==SIM_MODE_SSR_CUSTOM_SO  || mode==SIM_MODE_SSR_CUSTOM_BO)
  {
    // CARD 3
    fprintf(fp,"%10.4f%10.4f%10.6f%10.4f%10.3f%10.3f%5d%5s%10.3f\n",
                sim_h1,sim_h2,par[4],sim_range,ZERO,sim_ro,0,"",ZERO);
    if(sim_iemsct == 2)
    {
      // CARD 3A1
      if(mode==SIM_MODE_SSR_CUSTOM_SO  || mode==SIM_MODE_SSR_CUSTOM_BO)
      {
        fprintf(fp,"%5d%5d%5d%5d\n",2,1,sim_iday,0);
      }
      else
      {
        fprintf(fp,"%5d%5d%5d%5d\n",2,2,sim_iday,0);
      }
      ph = par[3]-par[5];
      if(ph < -180.0) ph += 360.0;
      // CARD 3A2
      fprintf(fp,"%10.5f%10.6f%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f\n",
                  ph,par[2],ZERO,ZERO,ZERO,ZERO,ZERO,ZERO);
      if(mode==SIM_MODE_SSR_CUSTOM_SO  || mode==SIM_MODE_SSR_CUSTOM_BO)
      {
        // CARD 3B1
        fprintf(fp,"%5d%5d\n",sim_n_angl,sim_n_wlen);
        // CARD 3C
        WriteCard3C_V5(fp);
      }
    }
  } else
  if(mode==SIM_MODE_TRN_MODTRAN_BO || mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    // CARD 3
    fprintf(fp,"%10.4f%10.4f%10.6f%10.4f%10.3f%10.3f%5d%5s%10.3f\n",
                sim_h1,sim_h2,par[2],sim_range,ZERO,sim_ro,0,"",ZERO);
  }
  else
  {
    fprintf(stderr,"%s: invalid mode >>> %d\n",fnam,mode);
    return -1;
  }

  return 0;
}

int WriteCard3C_V5(FILE *fp)
{
  int i,j,k;

  // CARD 3C1
  for(i=0; i<sim_n_angl; i++)
  {
    fprintf(fp,"%10.5f%s",sim_angl[i],i==sim_n_angl-1?"\n":i%8==7?"\n":"");
  }
  // CARD 3C2
  for(i=0; i<sim_n_wlen; i++)
  {
    fprintf(fp,"%10.6f%s",sim_wlen_um[i],i==sim_n_wlen-1?"\n":i%8==7?"\n":"");
  }
  #ifdef OLDMODE
  // CARD 3C3
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<sim_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      cprintf(fp," %9.3e%s",sim_phas[k],i==sim_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C4
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<sim_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      fprintf(fp,"%10.3e%s",sim_tropo_phas[k],i==sim_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C5
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<sim_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      fprintf(fp,"%10.3e%s",sim_strat_phas[k],i==sim_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C6
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<sim_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      fprintf(fp,"%10.3e%s",sim_meteo_phas[k],i==sim_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  #else
  // To increase the precision, cd3b3c.f line 94 must be modified as following:
  // READ(IRD,'((8F15.5))')(F(IAER,IANG,IWAV),IWAV=1,NWLF)
  // CARD 3C3
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<sim_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      cprintf(fp,"%15.9e%s",sim_phas[k],i==sim_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C4
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<sim_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      fprintf(fp,"%15.9e%s",sim_tropo_phas[k],i==sim_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C5
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<sim_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      fprintf(fp,"%15.9e%s",sim_strat_phas[k],i==sim_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C6
  for(j=0; j<sim_n_angl; j++)
  {
    for(i=0; i<sim_n_wlen; i++)
    {
      k = sim_n_angl*i+j;
      fprintf(fp,"%15.9e%s",sim_meteo_phas[k],i==sim_n_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  #endif

  return 0;
}

int WriteCard4_V5(FILE *fp,int mode,double *par)
{
  // DSR_MODTRAN_BO || DSR_CUSTOM_BO || SSR_MODTRAN_BO || SSR_CUSTOM_BO ||
  // TRN_MODTRAN_BO || TRN_CUSTOM_BO
  // par[0] = min wavelength
  // par[1] = max wavelength
  // SSR_MODTRAN_S0 || SSR_CUSTOM_S0
  // par[0] = wavelength
  // par[1] = #data in modtran.plt
  int n1,n2;
  double band;
  char fnam[] = "WriteCard4_V5";

  // CARD 4
  if(mode==SIM_MODE_SSR_MODTRAN_SO || mode==SIM_MODE_SSR_CUSTOM_SO)
  {
    if(sim_sbmp < 0)
    {
      band = 0.1;
      n1 = (int)(band*((int)((1.0e7/(par[0]*band)-0.5*(par[1]-1.0))+0.5))); // temporary
      n2 = (int)(band*((int)(par[1]+0.5)))+n1-1; // temporary
    }
    else
    {
      band = (double)sim_sbmp;
      n1 = sim_sbmp*((int)((1.0e7/(par[0]*sim_sbmp)-0.5*(par[1]-1.0))+0.5));
      n2 = sim_sbmp*((int)(par[1]+0.5))+n1-sim_sbmp;
    }
    fprintf(fp,"%10d%10d%10.1f%10.1f",n1,n2,band,2.0*band);
  } else
  if(mode==SIM_MODE_DSR_MODTRAN_BO || mode==SIM_MODE_DSR_CUSTOM_BO ||
     mode==SIM_MODE_SSR_MODTRAN_BO || mode==SIM_MODE_SSR_CUSTOM_BO ||
     mode==SIM_MODE_TRN_MODTRAN_BO || mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    if(mode==SIM_MODE_SSR_MODTRAN_BO || mode==SIM_MODE_SSR_CUSTOM_BO)
    {
      if(sim_sbmp < 0)
      {
        band = 0.1;
      }
      else
      {
        band = (double)sim_sbmp;
      }
      fprintf(fp,"%10.0f%10.0f%10.1f%10.1f",1.0e7/par[1],1.0e7/par[0],band,2.0*band);
    }
    else
    {
      if(sim_dbmp < 0)
      {
        band = 0.1;
      }
      else
      {
        band = (double)sim_dbmp;
      }
      fprintf(fp,"%10.0f%10.0f%10.1f%10.1f",1.0e7/par[1],1.0e7/par[0],band,2.0*band);
    }
  }
  else
  {
    fprintf(stderr,"%s: invalid mode >>> %d\n",fnam,mode);
    return -1;
  }
  if(sim_mlflx >= 0)
  {
    fprintf(fp,"rn         t    f%3d\n",sim_mlflx);
  }
  else
  {
    fprintf(fp,"rn\n");
  }
  if(sim_salb > 0)
  {
    // CARD 4A
    fprintf(fp,"1      0.0\n");
    // CARD 4L1
    fprintf(fp,"%s\n",sim_falb);
    // CARD 4L2
    fprintf(fp,"%d\n",sim_salb);
  }

  return 0;
}

int WriteCard5_V5(FILE *fp)
{
  // CARD 5
  fprintf(fp,"    0\n");
  return 0;
}

int RunModtran_V5(void)
{
  char line[MAXLINE];
  FILE *fp;

  if((fp=fopen("mod5root.in","w")) == NULL)
  {
    fprintf(stderr,"Error, cannot open %s\n","mod5root.in");
    return -1;
  }
  fprintf(fp,"modtran\n");
  fclose(fp);
  if(access("DATA",R_OK) < 0)
  {
    snprintf(line,MAXLINE,"%s/DATA",mod_path_v5);
    if(symlink(line,"DATA") < 0)
    {
      fprintf(stderr,"Error, cannot create symlink to %s\n",line);
      return -1;
    }
  }
  snprintf(line,MAXLINE,"%s/Mod90_5.2.0.0.exe",mod_path_v5);
  if(system(line) != 0)
  {
    fprintf(stderr,"Error in MODTRAN.\n");
    return -1;
  }

  return 0;
}
