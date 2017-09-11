int get_int(char *str,int start,int size,int *value);
int get_dbl(char *str,int start,int size,double *value);
int get_chr(char *str,int start,int size,char *value);
int get_upp(char *str,int start,int size,char *value);

int get_int(char *str,int start,int size,int *value)
{
  int i,j;
  char temp[MAXLINE];
  char *endp;

  if(start<0 || size<0)
  {
    fprintf(stderr,"get_int: invalid input >>> %d,%d\n",start,size);
    return -1;
  } else
  if(start+size >= MAXLINE)
  {
    fprintf(stderr,"get_int: out of range >>> %d,%d\n",start,size);
    return -1;
  }

  for(i=0; i<start; i++)
  {
    if(*(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') break;
  }
  if(i < start)
  {
    return 1;
  }
  for(i=start; i<start+size; i++)
  {
    if(*(str+i) != ' ') break;
  }
  if(i==start+size || *(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') 
  {
    return 0;
  }
  for(j=0; i<start+size; i++,j++)
  {
    if(*(str+i)==' ' || *(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') break;
    temp[j] = *(str+i);
  }
  temp[j] = '\0';

  errno = 0;
  *value = strtol(temp,&endp,10);
  if(errno==ERANGE || *endp!='\0')
  {
    fprintf(stderr,"get_int: convert error >>> %s\n",temp);
    return -1;
  }

  return 0;
}

int get_dbl(char *str,int start,int size,double *value)
{
  int i,j;
  char temp[MAXLINE];
  char *endp;

  if(start<0 || size<0)
  {
    fprintf(stderr,"get_dbl: invalid input >>> %d,%d\n",start,size);
    return -1;
  } else
  if(start+size >= MAXLINE)
  {
    fprintf(stderr,"get_dbl: out of range >>> %d,%d\n",start,size);
    return -1;
  }

  for(i=0; i<start; i++)
  {
    if(*(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') break;
  }
  if(i < start)
  {
    return 1;
  }
  for(i=start; i<start+size; i++)
  {
    if(*(str+i) != ' ') break;
  }
  if(i==start+size || *(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') 
  {
    return 0;
  }
  for(j=0; i<start+size; i++,j++)
  {
    if(*(str+i)==' ' || *(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') break;
    temp[j] = *(str+i);
  }
  temp[j] = '\0';

  errno = 0;
  *value = strtod(temp,&endp);
  if(errno==ERANGE || *endp!='\0')
  {
    fprintf(stderr,"get_dbl: convert error >>> %s\n",temp);
    return -1;
  }

  return 0;
}

int get_chr(char *str,int start,int size,char *value)
{
  int i,j;

  if(start<0 || size<0)
  {
    fprintf(stderr,"get_chr: invalid input >>> %d,%d\n",start,size);
    return -1;
  } else
  if(start+size >= MAXLINE)
  {
    fprintf(stderr,"get_chr: out of range >>> %d,%d\n",start,size);
    return -1;
  }

  for(i=0; i<start; i++)
  {
    if(*(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') break;
  }
  if(i < start)
  {
    return 1;
  }
  for(i=start; i<start+size; i++)
  {
    if(*(str+i) != ' ') break;
  }
  if(i==start+size || *(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') 
  {
    return 0;
  }
  for(j=0; i<start+size; i++,j++)
  {
    if(*(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') break;
    *(value+j) = *(str+i);
  }
  *(value+j) = '\0';

  return 0;
}

int get_upp(char *str,int start,int size,char *value)
{
  int i,j;

  if(start<0 || size<0)
  {
    fprintf(stderr,"get_chr: invalid input >>> %d,%d\n",start,size);
    return -1;
  } else
  if(start+size >= MAXLINE)
  {
    fprintf(stderr,"get_chr: out of range >>> %d,%d\n",start,size);
    return -1;
  }

  for(i=0; i<start; i++)
  {
    if(*(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') break;
  }
  if(i < start)
  {
    return 1;
  }
  for(i=start; i<start+size; i++)
  {
    if(*(str+i) != ' ') break;
  }
  if(i==start+size || *(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') 
  {
    return 0;
  }
  for(j=0; i<start+size; i++,j++)
  {
    if(*(str+i)=='\r' || *(str+i)=='\n' || *(str+i)=='\0') break;
    *(value+j) = toupper(*(str+i));
  }
  *(value+j) = '\0';

  return 0;
}
