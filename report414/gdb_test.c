#include<stdio.h>
#include<stdlib.h>
#define MAX 256

int main(void)
{
	int n;
	char* buf = (char *) malloc(sizeof(char) * MAX+1);
	FILE *fp;

	if((fp=fopen("/etc/hosts", "r"))==NULL){
		printf("can not open file\n");
		exit(1);
	}
	n = fread(buf, 1, MAX, fp);
	buf[n]='\0';
	printf("buf : %s\n", buf);
	free(buf);
	fclose(fp);
	return 0;
}

