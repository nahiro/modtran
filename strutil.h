#ifdef __cplusplus
extern "C" {
#endif

#define	MAX_STR_LEN		256
#define	CPRINTF_TYPE_INT	1	// %d,%i,%o,%u,%x,%X,%c
#define	CPRINTF_TYPE_DOUBLE	2	// %e,%E,%f,%F,%g,%G,%a,%A
#define	CPRINTF_TYPE_STRING	3	// %s
#define	CPRINTF_TYPE_POINTER	4	// %p
#define	CPRINTF_TYPE_NOARG	5	// %n,%m,%%
#define	CPRINTF_TYPE_OTHER	0	// other
#define	CPRINTF_N_CONV		22	// #conversion specifiers

char *As(char *dst,const char *src,int size);
char *Ad(char *dst,int d,int size);
char *Af(char *dst,double f,int size);
char *Ae(char *dst,double e,int size);
int ungets(char *s, int size, FILE *stream);
int getarg(char *src,int size,char *dst,char *ptr[]);
int message(FILE *fp,char *format,...);
int cprintf(FILE *fp,const char *fmt,...);
int lprintf(FILE *fp,int nlen,int npre,double d);

#ifdef __cplusplus
}
#endif
