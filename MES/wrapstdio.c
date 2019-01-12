
/*
FILE: wrapstdio.c

UNIT: CNCO3002

PURPOSE: Input/Output handling

REFERENCE: 
Curtin University.(AU). Worksheet 05: Threads. In Advanced Computer Communications (Semester 2 2018 Sri Lanka Inst Info Tech - INT[1]). Retrieved from https://lms.curtin.edu.au

COMMENTS: Following wrapper functions helps to handle input/output functions.
*/

/*
 * Standard I/O wrapper functions.
 */

#include	"acc.h"

void
Fclose(FILE *fp)
{
	if (fclose(fp) != 0)
		err_sys("fclose error");
}

FILE *
Fdopen(int fd, const char *type)
{
	FILE	*fp;

	if ( (fp = fdopen(fd, type)) == NULL)
		err_sys("fdopen error");

	return(fp);
}

char *
Fgets(char *ptr, int n, FILE *stream)
{
	char	*rptr;
	

	if ( (rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
		err_sys("fgets error");	

	return (rptr);
}

FILE *
Fopen(const char *filename, const char *mode)
{
	FILE	*fp;

	if ( (fp = fopen(filename, mode)) == NULL)
		err_sys("fopen error");

	return(fp);
}

void
Fputs(const char *ptr, FILE *stream)
{
	
	if (fputs(ptr, stream) == EOF)
		err_sys("fputs error");

}
