int WriteTape5_V4(double *par,int mode);
int WriteCard1_V4(FILE *fp,int mode);
int WriteCard2_V4(FILE *fp,int mode);
int WriteCard2C_V4(FILE *fp);
int WriteCard2D_V4(FILE *fp);
int WriteCard3_V4(FILE *fp,int mode,double *par);
int WriteCard3C_V4(FILE *fp);
int WriteCard4_V4(FILE *fp,int mode,double *par);
int WriteCard5_V4(FILE *fp);
int RunModtran_V4(void);

int WriteTape5_V4(double *par,int mode)
{
  int err;
  FILE *fp;
  char fnam[] = "WriteTape5_V4";

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
      if(WriteCard1_V4(fp,mode) < 0)
      {
        err = 1;
        break;
      }
      // CARD 2
      if(WriteCard2_V4(fp,mode) < 0)
      {
        err = 1;
        break;
      }
      // CARD 3
      if(WriteCard3_V4(fp,mode,par) < 0)
      {
        err = 1;
        break;
      }
      // CARD 4
      if(WriteCard4_V4(fp,mode,par) < 0)
      {
        err = 1;
        break;
      }
      // CARD 5
      if(WriteCard5_V4(fp) < 0)
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

int WriteCard1_V4(FILE *fp,int mode)
{
  char fnam[] = "WriteCard1_V4";

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
  }

  // CARD 1
  if(sim_salb > 0)
  {
    fprintf(fp,"%c%c%3d%5d%5d%5d    0    0    0    0    0    0%5d%5d%5d    .000 LAMBER\n",
                sim_modtran,sim_speed,
                sim_iatm,sim_itype,sim_iemsct,sim_mult,sim_mdef,sim_im,sim_prnt);
  }
  else
  {
    fprintf(fp,"%c%c%3d%5d%5d%5d    0    0    0    0    0    0%5d%5d%5d    .000%7.4f\n",
                sim_modtran,sim_speed,
                sim_iatm,sim_itype,sim_iemsct,sim_mult,sim_mdef,sim_im,sim_prnt,sim_grho);
  }
  if(sim_iemsct == 3)
  {
    // CARD 1A
    #ifdef OLDMODE
    if(isnan(sim_solcon))
    {
      cprintf(fp,"tt  8%s   0%10.3f%10.0f%10.0f %s t f %s\n",
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"tt  8%s   0%10.3f%10.0f%10.0f %s t f %s%2s%10.0f\n",
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f","",sim_solcon);
    }
    #else
    // To increase the precision, driver.f line 527 must be modified as following:
    // READ(IRD,'(A3,I2,A1,I4,F15.8,2A15,5(1X,A1),F10.3)')           &
    // Also, size of H2OSTR/O3STR must be changed to 15 in driver.f, sclcol.f, m3dcrd.f, wrtbuf.f
    if(isnan(sim_solcon))
    {
      cprintf(fp,"tt  8%s   0%15.8f%15.5f%15.5f %s t f %s\n",
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"tt  8%s   0%15.8f%15.5f%15.5f %s t f %s%2s%10.0f\n",
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f","",sim_solcon);
    }
    #endif
    if(strcmp(sim_fsun,NONAME) != 0)
    {
      // CARD 1A1
      fprintf(fp,"%s\n",sim_fsun);
    }
    // CARD 1A2
    fprintf(fp,"DATA/B2001_%02d.BIN\n",sim_dbmp);
  } else
  if(sim_iemsct == 0)
  {
    // CARD 1A
    #ifdef OLDMODE
    if(isnan(sim_solcon))
    {
      cprintf(fp,"%st%3d%s   0%10.3f+%9.0f%10.0f %s t f %s\n",
                  sim_dflg==0?"f":"t",sim_nstr,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"%st%3d%s   0%10.3f+%9.0f%10.0f %s t f %s%2s%10.0f\n",
                  sim_dflg==0?"f":"t",sim_nstr,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f","",sim_solcon);
    }
    #else
    // To increase the precision, driver.f line 527 must be modified as following:
    // READ(IRD,'(A3,I2,A1,I4,F15.8,2A15,5(1X,A1),F10.3)')           &
    // Also, size of H2OSTR/O3STR must be changed to 15 in driver.f, sclcol.f, m3dcrd.f, wrtbuf.f
    if(isnan(sim_solcon))
    {
      cprintf(fp,"%st%3d%s   0%15.8f+%14.5f%15.5f %s t f %s\n",
                  sim_dflg==0?"f":"t",sim_nstr,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"%st%3d%s   0%15.8f+%14.5f%15.5f %s t f %s%2s%10.0f\n",
                  sim_dflg==0?"f":"t",sim_nstr,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f","",sim_solcon);
    }
    #endif
    if(strcmp(sim_fsun,NONAME) != 0)
    {
      // CARD 1A1
      fprintf(fp,"%s\n",sim_fsun);
    }
    // CARD 1A2
    fprintf(fp,"DATA/B2001_%02d.BIN\n",sim_dbmp);
  }
  else
  {
    // CARD 1A
    #ifdef OLDMODE
    if(isnan(sim_solcon))
    {
      cprintf(fp,"%st%3d%s   0%10.3f%10.0f%10.0f %s t f %s\n",
                  sim_dflg==0?"f":"t",sim_nstr,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"%st%3d%s   0%10.3f%10.0f%10.0f %s t f %s%2s%10.0f\n",
                  sim_dflg==0?"f":"t",sim_nstr,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f","",sim_solcon);
    }
    #else
    // To increase the precision, driver.f line 527 must be modified as following:
    // READ(IRD,'(A3,I2,A1,I4,F15.8,2A15,5(1X,A1),F10.3)')           &
    // Also, size of H2OSTR/O3STR must be changed to 15 in driver.f, sclcol.f, m3dcrd.f, wrtbuf.f
    if(isnan(sim_solcon))
    {
      cprintf(fp,"%st%3d%s   0%15.8f%15.5f%15.5f %s t f %s\n",
                  sim_dflg==0?"f":"t",sim_nstr,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f");
    }
    else
    {
      cprintf(fp,"%st%3d%s   0%15.8f%15.5f%15.5f %s t f %s%2s%10.0f\n",
                  sim_dflg==0?"f":"t",sim_nstr,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_cmix,sim_wscl,sim_oscl,
                  strcmp(sim_fsun,NONAME)==0?"f":"t",
                  sim_wflg==1?"t":"f","",sim_solcon);
    }
    #endif
    if(strcmp(sim_fsun,NONAME) != 0)
    {
      // CARD 1A1
      fprintf(fp,"%s\n",sim_fsun);
    }
    // CARD 1A2
    fprintf(fp,"DATA/B2001_%02d.BIN\n",sim_sbmp);
  }

  return 0;
}

int WriteCard2_V4(FILE *fp,int mode)
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
  // To increase the precision, driver.f line 725 must be modified as following:
  // READ(IRD,'(A2,I3,A1,I4,A3,I2,3I5,5F15.10)')                   &
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
    WriteCard2C_V4(fp);
  }
  if(mode==SIM_MODE_DSR_CUSTOM_BO || mode==SIM_MODE_SSR_CUSTOM_SO || mode==SIM_MODE_SSR_CUSTOM_BO ||
     mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    // CARD 2D
    WriteCard2D_V4(fp);
  }

  return 0;
}

int WriteCard2C_V4(FILE *fp)
{
  int i;
  int f2,f3,f4;
  char jchar[15] = "";
  char jcharx[2] = " ";

  if(sim_2c <= 0)
  {
    return 0;
  }
  else
  {
    f2 = (sim_2c&0x0010)|(sim_2c&0x0100);
    f3 = (sim_2c&0x0100);
    f4 = (sim_2c&0x1000);
  }

  for(i=0; i<PRF_NPAR; i++)
  {
    jchar[i] = sim_prf_jchar[i];
  }
  jchar[i] = '\0';
  jcharx[0] = sim_prf_jcharx;

  // CARD 2C
  fprintf(fp,"%5d%5d%5d%20s%10.0f\n",prf_n_alt,(f2>0?1:0),(f4>0?2:0),"Custom",sim_ro);
  for(i=0; i<prf_n_alt; i++)
  {
    // CARD 2C1
    #ifdef OLDMODE
    cprintf(fp,"%10.3f%10.3e%10.3e%10.3e%10.3e%10.3e%14s %1s\n",
                prf_alt[i],sim_prf_pres[i],sim_prf_temp[i],
                sim_prf_wmol[0][i],sim_prf_wmol[1][i],sim_prf_wmol[2][i],jchar,jcharx);
    #else
    // To increase the precision, aernsm.f line 269 must be modified as following:
    // READ(IRD,'(F15.5,5E15.8,A16)')                        &
    cprintf(fp,"%15.8f%15.8e%15.8e%15.8e%15.8e%15.8e%14s %1s\n",
                prf_alt[i],sim_prf_pres[i],sim_prf_temp[i],
                sim_prf_wmol[0][i],sim_prf_wmol[1][i],sim_prf_wmol[2][i],jchar,jcharx);
    #endif
    if(f2 > 0)
    {
      // CARD 2C2
      #ifdef OLDMODE
      cprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_wmol[3][i],sim_prf_wmol[4][i],sim_prf_wmol[5][i],sim_prf_wmol[6][i],
                  sim_prf_wmol[7][i],sim_prf_wmol[8][i],sim_prf_wmol[9][i],sim_prf_wmol[10][i]);
      cprintf(fp,"%10.3e\n",sim_prf_wmol[11][i]);
      #else
      // To increase the precision, aernsm.f line 278 must be modified as following:
      // READ(IRD,'((8F15.5))')(WMOL(I),I=4,12)
      cprintf(fp,"%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e\n",
                  sim_prf_wmol[3][i],sim_prf_wmol[4][i],sim_prf_wmol[5][i],sim_prf_wmol[6][i],
                  sim_prf_wmol[7][i],sim_prf_wmol[8][i],sim_prf_wmol[9][i],sim_prf_wmol[10][i]);
      cprintf(fp,"%15.8e\n",sim_prf_wmol[11][i]);
      #endif
    }
    if(f3 > 0)
    {
      // CARD 2C2X
      #ifdef OLDMODE
      cprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_xmol[0][i],sim_prf_xmol[1][i],sim_prf_xmol[2][i],sim_prf_xmol[3][i],
                  sim_prf_xmol[4][i],sim_prf_xmol[5][i],sim_prf_xmol[6][i],sim_prf_xmol[7][i]);
      cprintf(fp,"%10.3e%10.3e%10.3e%10.3e%10.3e\n",
                  sim_prf_xmol[8][i],sim_prf_xmol[9][i],sim_prf_xmol[10][i],sim_prf_xmol[11][i],
                  sim_prf_xmol[12][i]);
      #else
      // To increase the precision, aernsm.f line 288 must be modified as following:
      // READ(IRD,'((8F15.5))')(WMOLX(I),I=1,NMOLX)
      cprintf(fp,"%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e%15.8e\n",
                  sim_prf_xmol[0][i],sim_prf_xmol[1][i],sim_prf_xmol[2][i],sim_prf_xmol[3][i],
                  sim_prf_xmol[4][i],sim_prf_xmol[5][i],sim_prf_xmol[6][i],sim_prf_xmol[7][i]);
      cprintf(fp,"%15.8e%15.8e%15.8e%15.8e%15.8e\n",
                  sim_prf_xmol[8][i],sim_prf_xmol[9][i],sim_prf_xmol[10][i],sim_prf_xmol[11][i],
                  sim_prf_xmol[12][i]);
      #endif
    }
    if(f4 > 0)
    {
      // CARD 2C3
      #ifdef OLDMODE
      cprintf(fp,"%10s%10.0f%10s%10.0f%10.0f%10.0f%10.0f\n","",sim_prf_haze[0][i],"",
                  0.0,sim_prf_haze[1][i],sim_prf_haze[2][i],sim_prf_haze[3][i]);
      #else
      // To increase the precision, aernsm.f line 329 must be modified as following:
      // READ(IRD,'(10X,F15.8,10X,4F15.8)')                &
      cprintf(fp,"%10s%15.5f%10s%15.5f%15.5f%15.5f%15.5f\n","",sim_prf_haze[0][i],"",
                  0.0,sim_prf_haze[1][i],sim_prf_haze[2][i],sim_prf_haze[3][i]);
      #endif
    }
  }

  return 0;
}

int WriteCard2D_V4(FILE *fp)
{
  int i,n;
  char line[MAXLINE];

  // CARD 2D
  fprintf(fp,"%5d%5d%5d%5d\n",sim_n_aers_wlen[0],sim_n_aers_wlen[1],sim_n_aers_wlen[2],sim_n_aers_wlen[3]);
  for(n=0; n<4; n++)
  {
    if(sim_n_aers_wlen[n] < 1)
    {
      continue;
    }
    // CARD 2D1
    snprintf(line,70,"AERSOL#%d",n+1);
    fprintf(fp,"%10.3e%70s\n",0.0,line);
    // CARD 2D2
    for(i=0; i<sim_n_aers_wlen[n]; i++)
    {
      #ifdef OLDMODE
      cprintf(fp,"%6.2f%7.5f%7.5f%6.4f%s",sim_aers_wlen_um[n][i],sim_aers_cext[n][i],sim_aers_cabs[n][i],sim_aers_asym[n][i],
                                          i==sim_n_aers_wlen[n]-1?"\n":(i%3)==2?"\n":"");
      #else
      // To increase the precision, aruexa.f line 70 must be modified as following:
      // READ(IRD, '((3(F15.11,2F15.13,F15.13)))')                &
      cprintf(fp,"%15.11f%15.13f%15.13f%15.13f%s",sim_aers_wlen_um[n][i],sim_aers_cext[n][i],sim_aers_cabs[n][i],sim_aers_asym[n][i],
                                                  i==sim_n_aers_wlen[n]-1?"\n":(i%3)==2?"\n":"");
      #endif
    }
  }

  return 0;
}

int WriteCard3_V4(FILE *fp,int mode,double *par)
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
  char fnam[] = "WriteCard3_V4";

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
        fprintf(fp,"%5d%5d\n",sim_n_phas_angl,sim_n_phas_wlen);
        // CARD 3C
        WriteCard3C_V4(fp);
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

int WriteCard3C_V4(FILE *fp)
{
  int i,j,k;

  // CARD 3C1
  for(i=0; i<sim_n_phas_angl; i++)
  {
    fprintf(fp,"%10.5f%s",sim_phas_angl[i],i==sim_n_phas_angl-1?"\n":i%8==7?"\n":"");
  }
  // CARD 3C2
  for(i=0; i<sim_n_phas_wlen; i++)
  {
    fprintf(fp,"%10.6f%s",sim_phas_wlen_um[i],i==sim_n_phas_wlen-1?"\n":i%8==7?"\n":"");
  }
  #ifdef OLDMODE
  // CARD 3C3
  for(j=0; j<sim_n_phas_angl; j++)
  {
    for(i=0; i<sim_n_phas_wlen; i++)
    {
      k = sim_n_phas_angl*i+j;
      cprintf(fp," %9.3e%s",sim_aer1_phas[k],i==sim_n_phas_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C4
  for(j=0; j<sim_n_phas_angl; j++)
  {
    for(i=0; i<sim_n_phas_wlen; i++)
    {
      k = sim_n_phas_angl*i+j;
      fprintf(fp,"%10.3e%s",sim_aer2_phas[k],i==sim_n_phas_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C5
  for(j=0; j<sim_n_phas_angl; j++)
  {
    for(i=0; i<sim_n_phas_wlen; i++)
    {
      k = sim_n_phas_angl*i+j;
      fprintf(fp,"%10.3e%s",sim_aer3_phas[k],i==sim_n_phas_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C6
  for(j=0; j<sim_n_phas_angl; j++)
  {
    for(i=0; i<sim_n_phas_wlen; i++)
    {
      k = sim_n_phas_angl*i+j;
      fprintf(fp,"%10.3e%s",sim_aer4_phas[k],i==sim_n_phas_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  #else
  // To increase the precision, driver.f line 1192 must be modified as following:
  // READ(IRD,'(8E15.9)')                      &
  // CARD 3C3
  for(j=0; j<sim_n_phas_angl; j++)
  {
    for(i=0; i<sim_n_phas_wlen; i++)
    {
      k = sim_n_phas_angl*i+j;
      cprintf(fp,"%15.9e%s",sim_aer1_phas[k],i==sim_n_phas_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C4
  for(j=0; j<sim_n_phas_angl; j++)
  {
    for(i=0; i<sim_n_phas_wlen; i++)
    {
      k = sim_n_phas_angl*i+j;
      fprintf(fp,"%15.9e%s",sim_aer2_phas[k],i==sim_n_phas_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C5
  for(j=0; j<sim_n_phas_angl; j++)
  {
    for(i=0; i<sim_n_phas_wlen; i++)
    {
      k = sim_n_phas_angl*i+j;
      fprintf(fp,"%15.9e%s",sim_aer3_phas[k],i==sim_n_phas_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  // CARD 3C6
  for(j=0; j<sim_n_phas_angl; j++)
  {
    for(i=0; i<sim_n_phas_wlen; i++)
    {
      k = sim_n_phas_angl*i+j;
      fprintf(fp,"%15.9e%s",sim_aer4_phas[k],i==sim_n_phas_wlen-1?"\n":i%8==7?"\n":"");
    }
  }
  #endif

  return 0;
}

int WriteCard4_V4(FILE *fp,int mode,double *par)
{
  // DSR_MODTRAN_BO || DSR_CUSTOM_BO || SSR_MODTRAN_BO || SSR_CUSTOM_BO ||
  // TRN_MODTRAN_BO || TRN_CUSTOM_BO
  // par[0] = min wavelength
  // par[1] = max wavelength
  // SSR_MODTRAN_S0 || SSR_CUSTOM_S0
  // par[0] = wavelength
  // par[1] = #data in modtran.plt
  int n1,n2;
  char fnam[] = "WriteCard4_V4";

  // CARD 4
  if(mode==SIM_MODE_SSR_MODTRAN_SO || mode==SIM_MODE_SSR_CUSTOM_SO)
  {
    n1 = sim_sbmp*((int)((1.0e7/(par[0]*sim_sbmp)-0.5*(par[1]-1.0))+0.5));
    n2 = sim_sbmp*((int)(par[1]+0.5))+n1-sim_sbmp;
    fprintf(fp,"%10d%10d%10.1f%10.1f",n1,n2,1.0*sim_sbmp,2.0*sim_sbmp);
  } else
  if(mode==SIM_MODE_DSR_MODTRAN_BO || mode==SIM_MODE_DSR_CUSTOM_BO ||
     mode==SIM_MODE_TRN_MODTRAN_BO || mode==SIM_MODE_TRN_CUSTOM_BO)
  {
    fprintf(fp,"%10.0f%10.0f%10.1f%10.1f",1.0e7/par[1],1.0e7/par[0],1.0*sim_dbmp,2.0*sim_dbmp);
  } else
  if(mode==SIM_MODE_SSR_MODTRAN_BO || mode==SIM_MODE_SSR_CUSTOM_BO)
  {
    fprintf(fp,"%10.0f%10.0f%10.1f%10.1f",1.0e7/par[1],1.0e7/par[0],1.0*sim_sbmp,2.0*sim_sbmp);
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

int WriteCard5_V4(FILE *fp)
{
  // CARD 5
  fprintf(fp,"    0\n");
  return 0;
}

int RunModtran_V4(void)
{
  char line[MAXLINE];
  FILE *fp;

  if((fp=fopen("modroot.in","w")) == NULL)
  {
    fprintf(stderr,"Error, cannot open %s\n","modroot.in");
    return -1;
  }
  fprintf(fp,"modtran\n");
  fclose(fp);
  if(access("DATA",R_OK) < 0)
  {
    snprintf(line,MAXLINE,"%s/DATA",mod_path_v4);
    if(symlink(line,"DATA") < 0)
    {
      fprintf(stderr,"Error, cannot create symlink to %s\n",line);
      return -1;
    }
  }
  snprintf(line,MAXLINE,"%s/Mod4v2r1_F90.exe",mod_path_v4);
  if(system(line) != 0)
  {
    fprintf(stderr,"Error in MODTRAN.\n");
    return -1;
  }

  return 0;
}
