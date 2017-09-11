#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "strutil.h"

char *As(char *dst,const char *src,int size)
{
  int i,n,p,q;

  if (size > MAX_STR_LEN) {
    fprintf(stderr,"Align center: size too big %d\n",size);
    return (char *)NULL;
  }

  n = strlen(src);
  if (n > size) {
    p = 0;
    q = size;
  } else {
    p = (size-n)/2;
    q = p+n;
  }

  for (i=0; i<p; i++) {
    *(dst+i) = ' ';
  }
  for (i=p; i<q; i++) {
    *(dst+i) = *(src+i-p);
  }
  for (i=q; i<size; i++) {
    *(dst+i) = ' ';
  }
  *(dst+size) = '\0';

  return dst;
}

char *Ad(char *dst,int d,int size)
{
  int i,n,p,q;
  char src[MAX_STR_LEN];

  if (size > MAX_STR_LEN) {
    fprintf(stderr,"Align center: size too big %d\n",size);
    return (char *)NULL;
  }

  sprintf(src,"%d",d);
  n = strlen(src);
  if (n > size) {
    p = 0;
    q = size;
  } else {
    p = (size-n)/2;
    q = p+n;
  }

  for (i=0; i<p; i++) {
    *(dst+i) = ' ';
  }
  for (i=p; i<q; i++) {
    *(dst+i) = *(src+i-p);
  }
  for (i=q; i<size; i++) {
    *(dst+i) = ' ';
  }
  *(dst+size) = '\0';

  return dst;
}

char *Af(char *dst,double f,int size)
{
  int i,n,p,q;
  char src[MAX_STR_LEN];

  if (size > MAX_STR_LEN) {
    fprintf(stderr,"Align center: size too big %d\n",size);
    return (char *)NULL;
  }

  sprintf(src,"%.2f",f);
  n = strlen(src);
  if (n > size) {
    p = 0;
    q = size;
  } else {
    p = (size-n)/2;
    q = p+n;
  }

  for (i=0; i<p; i++) {
    *(dst+i) = ' ';
  }
  for (i=p; i<q; i++) {
    *(dst+i) = *(src+i-p);
  }
  for (i=q; i<size; i++) {
    *(dst+i) = ' ';
  }
  *(dst+size) = '\0';

  return dst;
}

char *Ae(char *dst,double e,int size)
{
  int i,n,p,q;
  char src[MAX_STR_LEN];

  if (size > MAX_STR_LEN) {
    fprintf(stderr,"Align center: size too big %d\n",size);
    return (char *)NULL;
  }

  sprintf(src,"%.2e",e);
  n = strlen(src);
  if (n > size) {
    p = 0;
    q = size;
  } else {
    p = (size-n)/2;
    q = p+n;
  }

  for (i=0; i<p; i++) {
    *(dst+i) = ' ';
  }
  for (i=p; i<q; i++) {
    *(dst+i) = *(src+i-p);
  }
  for (i=q; i<size; i++) {
    *(dst+i) = ' ';
  }
  *(dst+size) = '\0';

  return dst;
}

int ungets(char *s, int size, FILE *stream)
{
  int i,end;
  char *c;

  end = strlen(s)-1;
  c = s+end;

  for (i=end; i>=0; i--,c--)
  {
    if (ungetc((int)*c,stream) != (int)*c)
    {
      fprintf(stderr,"Error in ungets >>> %s\n",s);
      return -1;
    }
  }

  return 0;
}

int getarg(char *src,int size,char *dst,char *ptr[])
{
  int i,j,k;
  int flg;
  char c;

  flg = 0;
  for(i=j=k=0; i<size; i++)
  {
    if(i == size-1)
    {
      *(dst+j) = '\0';
      break;
    }
    c = *(src+i);
    switch(c)
    {
      case '\n':
      case '\0':
        *(dst+j) = '\0';
        i = size;
        break;
      case ' ':
      case '\t':
        if(flg == 1)
        {
          *(dst+j) = c;
          if(j==0 || (j>0 &&(*(dst+j-1)=='\0')))
          {
            ptr[k++] = dst+j;
          }
          j++;
        }
        else
        {
          *(dst+j) = '\0';
          j++;
        }
        break;
      case '\\':
        if(i<size-1 && *(src+i+1)!='"')
        {
          *(dst+j) = c;
          if(j==0 || (j>0 &&(*(dst+j-1)=='\0')))
          {
            ptr[k++] = dst+j;
          }
          j++;
        }
        break;
      case '"':
        if(i>0 && *(src+i-1)=='\\')
        {
          *(dst+j) = c;
          if(j==0 || (j>0 &&(*(dst+j-1)=='\0')))
          {
            ptr[k++] = dst+j;
          }
          j++;
        }
        else
        {
          flg = 1-flg;
        }
        break;
      default:
        *(dst+j) = c;
        if(j==0 || (j>0 &&(*(dst+j-1)=='\0')))
        {
          ptr[k++] = dst+j;
        }
        j++;
        break;
    }
  }

  return k;
}

int message(FILE *fp,char *format,...)
{
  char strbuf[MAX_STR_LEN];
  va_list ap;

  va_start(ap,format);
  vsnprintf(strbuf,MAX_STR_LEN,format,ap);
  va_end(ap);

  fflush(stdout);
  fflush(stderr);
  fprintf(fp,"%s",strbuf);
  fflush(stdout);
  fflush(stderr);

  return 0;
}

int cprintf(FILE *fp,const char *fmt,...)
{
  int i,j,n;
  int err;
  int nlen;
  int npre;
  int type;
  int c;
  double d;
  void *p;
  char *s;
  char form[MAX_STR_LEN];
  char temp[MAX_STR_LEN];
  char conv[CPRINTF_N_CONV] = {'d','i','o','u','x','X','e','E','f','F',
                               'g','G','a','A','c','s','C','S','p','n','m','%'};
  va_list ap;

  va_start(ap,fmt);
  i = 0;
  err = 0;
  while(1)
  {
    if(i >= MAX_STR_LEN) // error
    {
      fprintf(stderr,"cprintf: format length exceed the limit >>> %d\n",i);
      err = 1;
      break;
    }
    if(fmt[i] == '\0') // normal termination
    {
      break;
    } else
    if(fmt[i] == '%')
    {
      // get format string
      strncpy(form,"%",MAX_STR_LEN);
      j = 1;
      err = 1;
      while(err)
      {
        i++;
        if(i >= MAX_STR_LEN) // error
        {
          fprintf(stderr,"cprintf: format length exceed the limit >>> %d\n",i);
          break;
        }
        if(fmt[i] == '\0') // error
        {
          fprintf(stderr,"cprintf: unexpected end character.\n");
          break;
        }
        form[j++] = fmt[i];
        for(n=0; n<CPRINTF_N_CONV; n++) // find conversion specifier
        {
          if(fmt[i] == conv[n])
          {
            err = 0;
            break;
          }
        }
      }
      n = j;
      if(err) break;
      // get length, precision, type, etc.
      nlen = 0;
      npre = -1;
      type = CPRINTF_TYPE_OTHER;
      for(j=1; j<n; j++)
      {
        switch(form[j])
        {
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            if(npre < 0)
            {
              nlen *= 10;
              nlen += (int)(form[j]-'0');
            }
            else
            {
              npre *= 10;
              npre += (int)(form[j]-'0');
            }
            break;
          case '.':
            if(npre < 0)
            {
              npre = 0;
            }
            else // error
            {
              fprintf(stderr,"cprintf: unexpected floating point.\n");
              err = 1;
            }
            break;
          case 'd':
          case 'i':
          case 'o':
          case 'u':
          case 'x':
          case 'X':
          case 'c':
            type = CPRINTF_TYPE_INT;
            break;
          case 'e':
          case 'E':
          case 'f':
          case 'F':
          case 'g':
          case 'G':
          case 'a':
          case 'A':
            type = CPRINTF_TYPE_DOUBLE;
            break;
          case 's':
            type = CPRINTF_TYPE_STRING;
            break;
          case 'p':
            type = CPRINTF_TYPE_POINTER;
            break;
          case 'n':
          case 'm':
          case '%':
            type = CPRINTF_TYPE_NOARG;
            break;
          default: // ignore '+', '-', ' ', 'l', 'L', etc.
            break;
        }
        if(err) break;
      }
      if(err) break;
      if(type == CPRINTF_TYPE_OTHER) // error
      {
        fprintf(stderr,"cprintf: unsupported type >>> %s\n",form);
        err = 1;
        break;
      } else
      if(type == CPRINTF_TYPE_INT)
      {
        c = va_arg(ap,int);
        fprintf(fp,form,c);
        if(nlen > 0)
        {
          snprintf(temp,MAX_STR_LEN,form,c);
          if(strlen(temp) > nlen)
          {
            fprintf(stderr,"cprintf: too long integer %s for the format >>> %s\n",temp,form);
            err = 1;
            break;
          }
        }
      } else
      if(type == CPRINTF_TYPE_DOUBLE)
      {
        d = va_arg(ap,double);
        if(npre < 0) npre = 0;
        if(lprintf(fp,nlen,npre,d) < 0)
        {
          err = 1;
          break;
        }
      } else
      if(type == CPRINTF_TYPE_STRING)
      {
        s = va_arg(ap,char*);
        fprintf(fp,form,s);
      } else
      if(type == CPRINTF_TYPE_POINTER)
      {
        p = va_arg(ap,void*);
        fprintf(fp,form,p);
      }
      else
      {
        fprintf(fp,"%s",form); // temporary
      }
    }
    else
    {
      fprintf(fp,"%c",fmt[i]);
    }
    i++;
  }
  va_end(ap);

  if(err)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

int lprintf_gflg = 0;
int lprintf_vflg = 0;

int lprintf(FILE *fp,int nlen,int npre,double d)
{
  int i,j;
  int err;
  int flag;
  int sign;
  int nexp;
  int aexp;
  int nrpt;
  int nfig_e; // number of significant figures
  int nfig_f; // number of significant figures
  int nfig_g; // number of significant figures
  int nfig_o; // number of significant figures
  int ndig_e; // number of digits after '.'
  int ndig_f; // number of digits after '.'
  int ndig_g; // number of digits after '.'
  int aexp_e; // numbers after 'e'
  int aexp_g; // numbers after 'e'
  double a;
  char form_e[MAX_STR_LEN];
  char form_f[MAX_STR_LEN];
  char form_g[MAX_STR_LEN];
  char form_o[MAX_STR_LEN];
  char temp_e[MAX_STR_LEN];
  char temp_f[MAX_STR_LEN];
  char temp_g[MAX_STR_LEN];
  char dstr_e[MAX_STR_LEN];
  char dstr_f[MAX_STR_LEN];
  char dstr_g[MAX_STR_LEN];
  char *dstr_o;
  char cflg_o = '\0';

  // check input
  if(isnan(d))
  {
    snprintf(form_o,MAX_STR_LEN,"%%%ds\n",nlen);
    fprintf(fp,form_o,"nan");
    fprintf(stderr,"lprintf: invalid input.\n");
    return -1;
  }

  // get absolute value
  if(d < 0.0)
  {
    sign = -1;
    a = fabs(d);
  }
  else
  {
    sign = 1;
    a = d;
  }

  // get power index
  if(a == 0.0)
  {
    aexp = 1;
    nexp = -aexp;
  } else
  if(a < 1.0)
  {
    aexp = (int)(0.5-floor(log10(a)));
    nexp = -aexp;
  }
  else
  {
    aexp = (int)(0.5+floor(log10(a)));
    nexp = aexp;
  }

  // F format
  nfig_f = nlen-1;                  // '.'
  if(sign < 0)  nfig_f -= 1;        // '-'
  ndig_f = nfig_f;
  if(nexp < -1) nfig_f -= (aexp-1); // '0'
  if(nexp > -1) ndig_f -= (aexp+1); // '1','2',...,'9','0'
  nrpt = 0;
  err = 0;
  while(ndig_f>=0 && nfig_f>=0)
  {
    snprintf(form_f,MAX_STR_LEN,"%%.%df",ndig_f);
    snprintf(temp_f,MAX_STR_LEN,form_f,a);
    strncpy(dstr_f,"",MAX_STR_LEN);
    i = 0;
    if(sign < 0)
    {
      if(i >= MAX_STR_LEN)
      {
        fprintf(stderr,"lprintf: string length exceed the limit >>> %d\n",i);
        err = 1;
        break;
      }
      dstr_f[i++] = '-';
    }
    nfig_f = 0;
    flag = 0;
    for(j=0; j<strlen(temp_f); j++)
    {
      if(temp_f[j] == '.')
      {
        if(flag < 1)
        {
          flag = 1;
        }
      } else
      if(temp_f[j] == '0')
      {
        if(flag < 1)
        {
          continue;
        } else
        if(flag>1 || a==0.0)
        {
          nfig_f++;
        }
      }
      else // '1','2',...,'9'
      {
        flag = 2;
        nfig_f++;
      }
      if(i >= MAX_STR_LEN)
      {
        fprintf(stderr,"lprintf: string length exceed the limit >>> %d\n",i);
        err = 1;
        break;
      }
      dstr_f[i++] = temp_f[j];
    }
    if(err) break;
    if(ndig_f == 0)
    {
      if(i >= MAX_STR_LEN)
      {
        fprintf(stderr,"lprintf: string length exceed the limit >>> %d\n",i);
        err = 1;
        break;
      }
      dstr_f[i++] = '.';
    }
    if(strlen(dstr_f) < nlen)
    {
      if(nrpt > 2)
      {
        break;
      }
      else
      {
        ndig_f++;
      }
    } else
    if(strlen(dstr_f) > nlen)
    {
      ndig_f--;
    }
    else
    {
      break;
    }
    nrpt++;
  }
  if(err)
  {
    ndig_f = -1;
    nfig_f = -1;
  } else
  if(ndig_f < 0)
  {
    nfig_f = -1;
  }

  // E format
  nfig_e = nlen-2;                  // '.','e'
  if(sign < 0)  nfig_e -= 1;        // '-'
  if(aexp > 0)
  {
    nfig_e -= (int)(0.5+floor(log10((double)aexp))); // '1','2',...,'9','0'
    if(nexp < 0)
    {
      nfig_e -= 1;                  // '-'
    }
  }
  else
  {
    nfig_e -= 1;                    // '0'
  }
  ndig_e = nfig_e-1;                // '1','2',...,'9'
  nrpt = 0;
  err = 0;
  while(ndig_e>=0 && nfig_e>=0)
  {
    snprintf(form_e,MAX_STR_LEN,"%%.%de",ndig_e);
    snprintf(temp_e,MAX_STR_LEN,form_e,a);
    strncpy(dstr_e,"",MAX_STR_LEN);
    i = 0;
    if(sign < 0)
    {
      if(i >= MAX_STR_LEN)
      {
        fprintf(stderr,"lprintf: string length exceed the limit >>> %d\n",i);
        err = 1;
        break;
      }
      dstr_e[i++] = '-';
    }
    nfig_e = 0;
    aexp_e = 0;
    flag = 0;
    for(j=0; j<strlen(temp_e); j++)
    {
      if(temp_e[j] == '.')
      {
        // do nothing
      } else
      if(temp_e[j] == 'e')
      {
        flag = 1;
      } else
      if(temp_e[j] == '+')
      {
        continue;
      } else
      if(temp_e[j] == '-')
      {
        // do nothing
      } else
      if(temp_e[j] == '0')
      {
        if(flag < 1)
        {
          nfig_e++;
        }
        else
        {
          if(aexp_e > 0)
          {
            aexp_e *= 10;
            aexp_e += (int)(temp_e[j]-'0');
          }
          else
          {
            continue;
          }
        }
      }
      else // '1','2',...,'9'
      {
        if(flag < 1)
        {
          nfig_e++;
        }
        else
        {
          aexp_e *= 10;
          aexp_e += (int)(temp_e[j]-'0');
        }
      }
      if(temp_e[j]=='e' && ndig_e==0)
      {
        if(i >= MAX_STR_LEN)
        {
          fprintf(stderr,"lprintf: string length exceed the limit >>> %d\n",i);
          err = 1;
          break;
        }
        dstr_e[i++] = '.';
      }
      if(i >= MAX_STR_LEN)
      {
        fprintf(stderr,"lprintf: string length exceed the limit >>> %d\n",i);
        err = 1;
        break;
      }
      dstr_e[i++] = temp_e[j];
    }
    if(err) break;
    if(aexp_e == 0)
    {
      if(i >= MAX_STR_LEN)
      {
        fprintf(stderr,"lprintf: string length exceed the limit >>> %d\n",i);
        err = 1;
        break;
      }
      dstr_e[i++] = '0';
    }
    if(strlen(dstr_e) < nlen)
    {
      if(nrpt > 2)
      {
        break;
      }
      else
      {
        ndig_e++;
      }
    } else
    if(strlen(dstr_e) > nlen)
    {
      ndig_e--;
    }
    else
    {
      break;
    }
    nrpt++;
  }
  if(err)
  {
    ndig_e = -1;
    nfig_e = -1;
  } else
  if(ndig_e < 0)
  {
    nfig_e = -1;
  }

  // E format without '.'
  if(lprintf_gflg == 1)
  {
    nfig_g = nlen-1;                  // 'e'
    if(sign < 0)  nfig_g -= 1;        // '-'
    if(aexp > 0)
    {
      nfig_g -= (int)(0.5+floor(log10((double)aexp))); // '1','2',...,'9','0'
      if(nexp < 0)
      {
        nfig_g -= 1;                  // '-'
      }
    }
    else
    {
      nfig_g -= 1;                    // '0'
    }
    ndig_g = nfig_g-1;                // '1','2',...,'9'
    nrpt = 0;
    err = 0;
    while(ndig_g>=0 && nfig_g>=0)
    {
      snprintf(form_g,MAX_STR_LEN,"%%.%de",ndig_g);
      snprintf(temp_g,MAX_STR_LEN,form_g,a*pow(10.0,(double)(npre-ndig_g)));
      strncpy(dstr_g,"",MAX_STR_LEN);
      i = 0;
      if(sign < 0)
      {
        if(i >= MAX_STR_LEN)
        {
          fprintf(stderr,"lprintf: string length exceed the limit >>> %d\n",i);
          err = 1;
          break;
        }
        dstr_g[i++] = '-';
      }
      nfig_g = 0;
      aexp_g = 0;
      flag = 0;
      for(j=0; j<strlen(temp_g); j++)
      {
        if(temp_g[j] == '.')
        {
          continue;
        } else
        if(temp_g[j] == 'e')
        {
          flag = 1;
        } else
        if(temp_g[j] == '+')
        {
          continue;
        } else
        if(temp_g[j] == '-')
        {
          // do nothing
        } else
        if(temp_g[j] == '0')
        {
          if(flag < 1)
          {
            nfig_g++;
          }
          else
          {
            if(aexp_g > 0)
            {
              aexp_g *= 10;
              aexp_g += (int)(temp_g[j]-'0');
            }
            else
            {
              continue;
            }
          }
        }
        else // '1','2',...,'9'
        {
          if(flag < 1)
          {
            nfig_g++;
          }
          else
          {
            aexp_g *= 10;
            aexp_g += (int)(temp_g[j]-'0');
          }
        }
        if(i >= MAX_STR_LEN)
        {
          fprintf(stderr,"lprintf: string length exceed the limit >>> %d\n",i);
          err = 1;
          break;
        }
        dstr_g[i++] = temp_g[j];
      }
      if(err) break;
      if(aexp_g == 0)
      {
        if(i >= MAX_STR_LEN)
        {
          fprintf(stderr,"lprintf: string length exceed the limit >>> %d\n",i);
          err = 1;
          break;
        }
        dstr_g[i++] = '0';
      }
      if(strlen(dstr_g) < nlen)
      {
        if(nrpt > 2)
        {
          break;
        }
        else
        {
          ndig_g++;
        }
      } else
      if(strlen(dstr_g) > nlen)
      {
        ndig_g--;
      }
      else
      {
        break;
      }
      nrpt++;
    }
    if(err)
    {
      ndig_g = -1;
      nfig_g = -1;
    } else
    if(ndig_g < 0)
    {
      nfig_g = -1;
    }
  }
  else
  {
    ndig_g = -1;
    nfig_g = -1;
    strncpy(dstr_g,"nan",MAX_STR_LEN);
  }

  // output string
  err = 0;
  nfig_o = 0;
  if(ndig_f > nfig_o)
  {
    nfig_o = nfig_f;
    dstr_o = dstr_f;
    cflg_o = 'f';
  }
  if(nfig_e > nfig_o)
  {
    nfig_o = nfig_e;
    dstr_o = dstr_e;
    cflg_o = 'e';
  }
  if(nfig_g > nfig_o)
  {
    nfig_o = nfig_g;
    dstr_o = dstr_g;
    cflg_o = 'g';
  }
  snprintf(form_o,MAX_STR_LEN,"%%%ds",nlen);
  if(nfig_o <= 0)
  {
    fprintf(fp,form_o,"nan");
    fprintf(stderr,"lprintf: invalid input.\n");
    err = 1;
  }
  else
  {
    fprintf(fp,form_o,dstr_o);
  }

  if(lprintf_vflg == 1)
  {
    fprintf(stderr,"%5d %5d %30.15f ",nlen,npre,d);
    snprintf(form_o,MAX_STR_LEN,"%%5d %%5d %%15s");
    if(ndig_f<0 || nfig_f<=0)
    {
      fprintf(stderr,form_o,ndig_f,nfig_f,"error");
    }
    else
    {
      fprintf(stderr,form_o,ndig_f,nfig_f,dstr_f);
    }
    if(cflg_o == 'f')
    {
      fprintf(stderr,"* ");
    }
    else
    {
      fprintf(stderr,"  ");
    }
    if(ndig_e<0 || nfig_e<=0)
    {
      fprintf(stderr,form_o,ndig_e,nfig_e,"error");
    }
    else
    {
      fprintf(stderr,form_o,ndig_e,nfig_e,dstr_e);
    }
    if(cflg_o == 'e')
    {
      fprintf(stderr,"* ");
    }
    else
    {
      fprintf(stderr,"  ");
    }
    if(ndig_g<0 || nfig_g<=0)
    {
      fprintf(stderr,form_o,ndig_g,nfig_g,"error");
    }
    else
    {
      fprintf(stderr,form_o,ndig_g,nfig_g,dstr_g);
    }
    if(cflg_o == 'g')
    {
      fprintf(stderr,"*\n");
    }
    else
    {
      fprintf(stderr,"\n");
    }
  }

  if(err)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}
