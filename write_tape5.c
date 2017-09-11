/*********************************************************/
/* write_tape5                                           */
/* Author: N.Manago Apr,2007                             */
/* $Revision: 1.40 $                                     */
/* $Date: 2008-08-05 07:15:06 $ (UTC)                    */
/*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "common.h"
#include "strutil.h"
#include "numutil.h"
#ifndef NAN
#define NAN				(0.0/0.0)
#endif

#define	DNAME				"default.in"

int		smod			= 0;				// Stdin   mode
int		rv			= 0;				// Reverse mode
int		vb			= 0;				// Verbose mode
int		db			= 0;				// Debug   mode
int		hp			= 0;				// Help    mode
char		dname[MAXLINE]		= DNAME;			// Default setup

#include "common.c"
#include "CARD1.c"
#include "CARD1A.c"
#include "CARD1A1.c"
#include "CARD1A2.c"
#include "CARD1A3.c"
#include "CARD2.c"
#include "CARD2AP.c"
#include "CARD2A.c"
#include "CARD2B.c"
#include "CARD2C.c"
#include "CARD2C1.c"
#include "CARD2C2.c"
#include "CARD2C2X.c"
#include "CARD2C3.c"
#include "CARD2D.c"
#include "CARD2D1.c"
#include "CARD2D2.c"
#include "CARD2E1.c"
#include "CARD2E2.c"
#include "CARD3.c"
#include "CARD3A1.c"
#include "CARD3A2.c"
#include "CARD3B1.c"
#include "CARD3B2.c"
#include "CARD3C1.c"
#include "CARD3C2.c"
#include "CARD3C3.c"
#include "CARD3C4.c"
#include "CARD3C5.c"
#include "CARD3C6.c"
#include "CARD4.c"
#include "CARD4A.c"
#include "CARD4B1.c"
#include "CARD4B2.c"
#include "CARD4B3.c"
#include "CARD4L1.c"
#include "CARD4L2.c"
#include "CARD5.c"
#include "CARD1s.c"
#include "CARD2s.c"
#include "CARD3s.c"
#include "CARD4s.c"
#include "CARD5s.c"

int (*CARD_gets[NCARD])(char *line,TAPE5_struct *t) =
{
  CARD1_gets,
  CARD1A_gets,
  CARD1A1_gets,
  CARD1A2_gets,
  CARD1A3_gets,
  CARD2_gets,
  CARD2AP_gets,
  CARD2A_gets,
  CARD2B_gets,
  CARD2C_gets,
  CARD2C1_gets,
  CARD2C2_gets,
  CARD2C2X_gets,
  CARD2C3_gets,
  CARD2D_gets,
  CARD2D1_gets,
  CARD2D2_gets,
  CARD2E1_gets,
  CARD2E2_gets,
  CARD3_gets,
  CARD3A1_gets,
  CARD3A2_gets,
  CARD3B1_gets,
  CARD3B2_gets,
  CARD3C1_gets,
  CARD3C2_gets,
  CARD3C3_gets,
  CARD3C4_gets,
  CARD3C5_gets,
  CARD3C6_gets,
  CARD4_gets,
  CARD4A_gets,
  CARD4B1_gets,
  CARD4B2_gets,
  CARD4B3_gets,
  CARD4L1_gets,
  CARD4L2_gets,
  CARD5_gets,
};

char CARD_ID[NCARD][5] =
{
  "1",
  "1A",
  "1A1",
  "1A2",
  "1A3",
  "2",
  "2AP",
  "2A",
  "2B",
  "2C",
  "2C1",
  "2C2",
  "2C2X",
  "2C3",
  "2D",
  "2D1",
  "2D2",
  "2E1",
  "2E2",
  "3",
  "3A1",
  "3A2",
  "3B1",
  "3B2",
  "3C1",
  "3C2",
  "3C3",
  "3C4",
  "3C5",
  "3C6",
  "4",
  "4A",
  "4B1",
  "4B2",
  "4B3",
  "4L1",
  "4L2",
  "5",
};

CARD1_struct    card1;
CARD1A_struct   card1a;
CARD1A1_struct  card1a1;
CARD1A2_struct  card1a2;
CARD1A3_struct  card1a3;
CARD2_struct    card2;
CARD2AP_struct  card2ap;
CARD2A_struct   card2a;
CARD2B_struct   card2b;
CARD2C_struct   card2c;
CARD2C1_struct  card2c1[LAYDIM];
CARD2C2_struct  card2c2[LAYDIM];
CARD2C2X_struct card2c2x[LAYDIM];
CARD2C3_struct  card2c3[LAYDIM];
CARD2D_struct   card2d;
CARD2D1_struct  card2d1[MXIREG];
CARD2D2_struct  card2d2[MXIREG*MXNSPC];
CARD2E1_struct  card2e1[NZCLD];
CARD2E2_struct  card2e2[MXWVLN];
CARD3_struct    card3;
CARD3A1_struct  card3a1;
CARD3A2_struct  card3a2;
CARD3B1_struct  card3b1;
CARD3B2_struct  card3b2[MXANGL];
CARD3C1_struct  card3c1;
CARD3C2_struct  card3c2;
CARD3C3_struct  card3c3;
CARD3C4_struct  card3c4;
CARD3C5_struct  card3c5;
CARD3C6_struct  card3c6;
CARD4_struct    card4;
CARD4A_struct   card4a;
CARD4B1_struct  card4b1[MXNSRF];
CARD4B2_struct  card4b2[MXNSRF];
CARD4B3_struct  card4b3[MXNSRF*MWVSRF];
CARD4L1_struct  card4l1;
CARD4L2_struct  card4l2[MXNSRF];
CARD5_struct    card5;
TAPE5_struct    tape5 =
{
  &card1,
  &card1a,
  &card1a1,
  &card1a2,
  &card1a3,
  &card2,
  &card2ap,
  &card2a,
  &card2b,
  &card2c,
  &card2c1[0],
  &card2c2[0],
  &card2c2x[0],
  &card2c3[0],
  &card2d,
  &card2d1[0],
  &card2d2[0],
  &card2e1[0],
  &card2e2[0],
  &card3,
  &card3a1,
  &card3a2,
  &card3b1,
  &card3b2[0],
  &card3c1,
  &card3c2,
  &card3c3,
  &card3c4,
  &card3c5,
  &card3c6,
  &card4,
  &card4a,
  &card4b1[0],
  &card4b2[0],
  &card4b3[0],
  &card4l1,
  &card4l2[0],
  &card5,
};

int Init(void);
int GetOpt(int argn,char **args);
int Usage(void);
void TAPE5_write(const TAPE5_struct *t);
void TAPE5_print(const TAPE5_struct *t);
int TAPE5_read(TAPE5_struct *t,FILE *fp);
int TAPE5_gets(char *line,TAPE5_struct *t);
int SelectCard(char *line);

int main(int argc,char **argv)
{
  int n = -1;
  char line[MAXLINE];
  char *p;

  if(GetOpt(argc,argv) < 0) exit(-1);
  if(hp) {Usage(); return 0;}
  if(Init() < 0) exit(-1);

  if(rv)
  {
    if(smod)
    {
      do
      {
        if(TAPE5_read(&tape5,stdin) < 0)
        {
          return -1;
        }
        TAPE5_print(&tape5);
      }
      while(card5.IRPT);
    }
    else
    {
      TAPE5_print(&tape5);
    }
  }
  else
  {
    if(smod)
    {
      while(fgets(line,MAXLINE,stdin) != NULL)
      {
        if((p=strchr(line,'#')) != NULL)
        {
          *p = '\0';
        }
        n = TAPE5_gets(line,&tape5);
        if(n < 0)
        {
          return -1;
        } else
        if(n == NCARD)
        {
          TAPE5_write(&tape5);
        }
      }
      if(n != NCARD)
      {
        TAPE5_write(&tape5);
      }
    }
    else
    {
      TAPE5_write(&tape5);
    }
  }

  return 0;
}

int Init(void)
{
  char line[MAXLINE];
  char *p;
  FILE *fp;
  struct timeval timeout;
  fd_set read_set;

  FD_ZERO(&read_set);
  FD_SET(0,&read_set);

  timeout.tv_sec = 0;
  timeout.tv_usec = 10;
  errno = 0;
  if(select(FD_SETSIZE,&read_set,(fd_set *)NULL,(fd_set *)NULL,&timeout) == -1)
  {
    perror("select failure");
    return -1;
  }

  if(FD_ISSET(0,&read_set))
  {
    fprintf(stderr,"Reading from stdin.\n");
    smod = 1;
  }
  else
  {
    fprintf(stderr,"Using default setup.\n");
  }

  if(smod)
  {
    if(fgets(line,MAXLINE,stdin) == NULL)
    {
      fprintf(stderr,"Error in reading from stdin.\n");
      return -1;
    }
    if(strchr(line,'#') == NULL)
    {
      fprintf(stderr,"Setting reverse mode on.\n");
      rv = 1;
    }
    ungets(line,MAXLINE,stdin);
  }

  card1    = CARD1_default;
  card1a   = CARD1A_default;
  card1a1  = CARD1A1_default;
  card1a2  = CARD1A2_default;
  card1a3  = CARD1A3_default;
  card2    = CARD2_default;
  card2ap  = CARD2AP_default;
  card2a   = CARD2A_default;
  card2b   = CARD2B_default;
  card2c   = CARD2C_default;
  card2d   = CARD2D_default;
  card3    = CARD3_default;
  card3a1  = CARD3A1_default;
  card3a2  = CARD3A2_default;
  card3b1  = CARD3B1_default;
  card4    = CARD4_default;
  card4a   = CARD4A_default;
  card4l1  = CARD4L1_default;
  card5    = CARD5_default;

  if((fp=fopen(dname,"r")) != NULL)
  {
    fprintf(stderr,"Reading default setup from %s\n",dname);
    if(fgets(line,MAXLINE,fp) == NULL)
    {
      fprintf(stderr,"Error in reading from %s\n",dname);
      return -1;
    }
    ungets(line,MAXLINE,fp);
    if(strchr(line,'#') == NULL)
    {
      do
      {
        if(TAPE5_read(&tape5,fp) < 0)
        {
          return -1;
        }
      }
      while(card5.IRPT);
    }
    else
    {
      while(fgets(line,MAXLINE,fp) != NULL)
      {
        if((p=strchr(line,'#')) != NULL)
        {
          *p = '\0';
        }
        if(TAPE5_gets(line,&tape5) < 0)
        {
          return -1;
        }
      }
    }
    fclose(fp);
  }

  return 0;
}

void TAPE5_write(const TAPE5_struct *t)
{
  CARD1s_write(t);
  CARD2s_write(t);
  CARD3s_write(t);
  CARD4s_write(t);
  CARD5s_write(t);

  return;
}

void TAPE5_print(const TAPE5_struct *t)
{
  CARD1s_print(t);
  CARD2s_print(t);
  CARD3s_print(t);
  CARD4s_print(t);
  CARD5s_print(t);

  return;
}

int TAPE5_read(TAPE5_struct *t,FILE *fp)
{
  if(abs(t->card5->IRPT) == 3)
  {
    if(CARD3s_read(t,fp) != 0) return -1;
    if(CARD4s_read(t,fp) != 0) return -1;
    if(CARD5s_read(t,fp) != 0) return -1;
  } else
  if(abs(t->card5->IRPT) == 4)
  {
    if(CARD4s_read(t,fp) != 0) return -1;
    if(CARD5s_read(t,fp) != 0) return -1;
  }
  else
  {
    if(CARD1s_read(t,fp) != 0) return -1;
    if(CARD2s_read(t,fp) != 0) return -1;
    if(CARD3s_read(t,fp) != 0) return -1;
    if(CARD4s_read(t,fp) != 0) return -1;
    if(CARD5s_read(t,fp) != 0) return -1;
  }

  return 0;
}

int TAPE5_gets(char *line,TAPE5_struct *t)
{
  int n;

  if((n=SelectCard(line)) <= 0)
  {
    return n;
  }

  if(CARD_gets[n-1](line,t) < 0)
  {
    return -1;
  }

  return n;
}

int SelectCard(char *line)
{
  int i,n;
  char str1[MAXLINE];
  char str2[MAXLINE];
  char str3[MAXLINE];

  if((n=sscanf(line,"%s%s%s",str1,str2,str3)) < 2)
  {
    return -1;
  } else
  if(n < 3)
  {
    return 0;
  }

  for(i=0; i<NCARD; i++)
  {
    if(strcmp(CARD_ID[i],str1) == 0)
    {
      return i+1;
    }
  }

  fprintf(stderr,"Ignoring %s\n",line);
  return -1;
}

int GetOpt(int argn,char **args)
{
  int c,rt;
  int ntmp;
  int option_index = 0;
  int this_option_optind;
  char *endp;
  struct option long_options[] =
  {
    {"version",1,0,'M'},
    {"default",0,0,'D'},
    {"debug",0,0,'d'},
    {"reverse",0,0,'r'},
    {"verbose",0,0,'v'},
    {"help",0,0,'h'}
  };

  rt = 0;
  while(1)
  {
    this_option_optind = optind?optind:1;
    c = getopt_long(argn,args,":D:drvh",long_options,&option_index);
    if(c == -1) break;

    switch(c)
    {
      case 'M':
        errno = 0;
        ntmp = strtol(optarg,&endp,10);
        if(errno!=ERANGE && *endp=='\0' && (ntmp==MOD_V4||ntmp==MOD_V5)) mod_v = ntmp;
        else
        {
          fprintf(stderr,"MODTRAN version -> out of range %s\n",optarg);
          rt = -1;
        }
        break;
      case 'D':
        strncpy(dname,optarg,MAXLINE);
        break;
      case 'd':
        db = 1;
        break;
      case 'r':
        rv = 1;
        break;
      case 'v':
        vb++;
        break;
      case 'h':
        hp = 1;
        break;
      case '?':
        if(optopt == '\0')
        {
          fprintf(stderr,"Invalid option : %s\n",args[this_option_optind]);
        }
        else
        {
          fprintf(stderr,"Invalid option : %c\n",optopt);
        }
        rt = -1;
        break;
      case ':':
        if(optopt == '\0')
        {
          fprintf(stderr,"Option requires an argument : %s\n",args[this_option_optind]);
        }
        else
        {
          fprintf(stderr,"Option requires an argument : %c\n",optopt);
        }
        rt = -1;
        break;
      default:
        fprintf(stderr,"?? getopt returned character code 0%o ??\n",c);
        rt = -1;
        break;
    }
  }

  if(optind < argn)
  {
    fprintf(stderr,"non-option ARGV-elements:\n");
    while(optind < argn)
    {
      fprintf(stderr,"%s\n",args[optind++]);
    }
    rt = -1;
  }

  if(hp) rt = 1;
  return rt;
}

int Usage(void)
{
  int n = 15;
  char e[MAXLINE];
  char a[MAXLINE];
  char d[MAXLINE];

  fprintf(stderr,"write_tape5 ... convert readable text into modtran tape5, and vice versa.\n");
  fprintf(stderr,"Usage:\n");
  fprintf(stderr,"write_tape5 -[option] (argument) -[option] (argument) ...\n");
  fprintf(stderr,"-----------------------------------------------------------------------------\n");
  fprintf(stderr,"   option   |%s|%s|%s| current\n",As(e,"",n),         As(a,"argument",n),   As(d,"default",n));
  fprintf(stderr," M -version |%s|%s|%s| %d\n",As(e,"MODTRAN version",n),As(a,"4000|5000",n), Ad(d,MOD_V4,n),mod_v);
  fprintf(stderr," D -default |%s|%s|%s| %s\n",As(e,"Default input",n), As(a,"file name",n),  As(d,DNAME,n),dname);
  fprintf(stderr," d -debug   |%s|%s|%s| %d\n",As(e,"Debug   mode",n),  As(a,"nothing",n),    Ad(d,0,n),db);
  fprintf(stderr," r -reverse |%s|%s|%s| %d\n",As(e,"Reverse mode",n),  As(a,"nothing",n),    Ad(d,0,n),rv);
  fprintf(stderr," v -verbose |%s|%s|%s| %d\n",As(e,"Verbose mode",n),  As(a,"nothing",n),    Ad(d,0,n),vb);
  fprintf(stderr," h -help    |%s|%s|%s| %d\n",As(e,"Help    mode",n),  As(a,"nothing",n),    Ad(d,0,n),1);
  fprintf(stderr,"-----------------------------------------------------------------------------\n");
  fprintf(stderr,"Reverse mode: tp5 -> readable text.\n");

  return 0;
}
