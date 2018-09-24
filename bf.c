/* A simple BF interpreter */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define CELLS	30000

unsigned char cells[CELLS];
int c;
int i;
char *buf;
long filesize;
long pc;

int main(int argc, char *argv[])
{
	extern unsigned char cells[];
	extern int c;
	extern long filesize;
	FILE *fp;
	char *filename;
	extern int i;
	int found=0;
	int debugMode=0;

	int obrackets=0;
	int cbrackets=0;

	if (argc<2) {
		printf("\nWoops! Filename required.\n");
		return 1;
	}


	filename = argv[1];
	fp =  fopen(filename,"r+");

	i=0;
	while (argv[2+i]!=NULL)
	{
		if (!strcmp(argv[2+i],"--debug"))
			debugMode=1;
	}

	if (fp==NULL) 
	{
		printf("Can't open file '%s'\n",filename);
		return 1;
	}

	printf("Opened file %s\n",filename);
	for (i=0; i<CELLS; i++) cells[i]=0; /* Initialize cells with 0 */
	fseek(fp,0,SEEK_END);
	filesize = ftell(fp);
	fseek(fp,0,SEEK_SET);
	buf = malloc(filesize+2);
	printf("Loading buffer:\n");
	for (i=0; i<filesize; i++) {
		buf[i]=fgetc(fp);
		putchar(buf[i]);
	}
	buf[i]=EOF;

	printf("Running...\n");

	i=0;
	pc=0;
	while ((c=buf[pc])!=EOF)
	{
		if (debugMode) putchar(c);
		switch (c)
		{
			case '>':
				i++;
				break;

			case '<':
				i--;
				break;

			case '+':
				cells[i]++;
				break;

			case '-':
				cells[i]--;
				break;

			case '.':
				if (
					!iscntrl(cells[i])
				||	cells[i]=='\n')
					putc(cells[i],stdout);
				break;

			case ',':
				cells[i]=getchar();
				break;

			case '[':
				if (cells[i]==0)
				{/*
					while (buf[pc]!=']')
						pc++;*/

					found=0;
					cbrackets=0;
					obrackets=1;
					while (!found)
					{
						pc++;
						if (buf[pc]==']') {
							cbrackets++;
						}
						if (buf[pc]=='[') {
							obrackets++;
						}
						if (cbrackets==obrackets) found=1;
					}
				}
				break;

			case ']':
				if (cells[i]!=0)
				{/*
					while (buf[pc]!='[')
						pc--;*/
					found=0;
					obrackets=0;
					cbrackets=1;
					while (!found)
					{
						pc--;
						if (buf[pc]=='[') {
							obrackets++;
						}
						if (buf[pc]==']') {
							cbrackets++;
						}
						if (cbrackets==obrackets) found=1;
					}
				}
				break;


		}
		pc++;
	}

	printf("\n\n *** \n\n");
	fclose(fp);
	return 0;
}
