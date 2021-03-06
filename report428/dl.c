#include <sys/types.h> 
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<fcntl.h>
#include <time.h>

char type(mode_t);
char *perm(mode_t);
void printStat(char*, char*, struct stat*);

//파일 내용
void ls_lblock(struct stat *st){printf("%5ld ", st->st_blocks);}
void ls_ltype(struct stat *st){ printf("%c%s ", type(st->st_mode), perm(st->st_mode)); }
void ls_llink(struct stat *st){ printf("%ld ", st->st_nlink); }
void ls_lugid(struct stat *st){ printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name); }
void ls_lsize(struct stat *st){ printf("%5ld ", st->st_size); }
void ls_ltime(struct stat *st){ printf("%.12s ", ctime(&st->st_mtime)+4); }
void ls_lnode(struct stat *st){printf("%17ld ",  st->st_ino);}

void ls_lhuman(struct stat *st)
{
	double i = (st->st_size);
	if (i >= 1000){
		i /= 1000;
		printf("%4.f", i);
		printf("K ");
	}
	else
		ls_lsize(st);
}

//명령에 맞게 호출
void Is_option(struct dirent d, struct stat st, char * option)
{	
	if((strcmp(option, "-al") == 0) || (strcmp(option, "-la") == 0))
	{
		ls_ltype(&st);
		ls_llink(&st);
		ls_lugid(&st);
		ls_lsize(&st);
		ls_ltime(&st);
	}
	else if ((strcmp(option, "-ai")==0) || (strcmp(option, "-ia")==0))
	{
		ls_lnode(&st);
	}
	else if (strcmp(option,"-alh")==0)
	{
		ls_ltype(&st); 
		ls_llink(&st);
		ls_lugid(&st);
		ls_lhuman(&st);
		ls_ltime(&st);
	}
	
	
}

// 	참고 블로그 : https://go-it.tistory.com/4 *	//

/* 디렉토리 내용을 자세히 리스트한다. */
int main(int argc, char *argv[]) 
{
 	char * dir = (char *)malloc(sizeof(char) * 1024);
	memset(dir, 0, 1024);
	
	DIR *dp = NULL;			
 	struct dirent *d = NULL;		
 	struct stat st;		
  	char path[BUFSIZ+1];	
	
	getcwd(dir, 1024);

	if ((dp = opendir(dir)) == NULL)  // 디렉토리 열기 
		perror(dir);

	while ((d = readdir(dp)) != NULL) {	// 디렉토리 내의 각 파일에 대해 
		sprintf(path, "%s/%s", dir, d->d_name); // 파일 경로명 만들기 
		if (lstat(path, &st) < 0) 	// 파일 상태 정보 가져오기  
			perror(path);
		if(argc>1)	
			Is_option(*d, st, argv[1]);				
		printf("%s\n", d->d_name);			
	}
	
	free(dir);
	closedir(dp);
	exit(0);
}

/* 파일 타입을 리턴 */
char type(mode_t mode) {

	if (S_ISREG(mode)) 
		return('-');
	if (S_ISDIR(mode)) 
		return('d');
	if (S_ISCHR(mode)) 
		return('c');
	if (S_ISBLK(mode)) 
		return('b');
	if (S_ISLNK(mode)) 
		return('l');
	if (S_ISFIFO(mode)) 
		return('p');
	if (S_ISSOCK(mode)) 
		return('s');
}

/* 파일 허가권을 리턴 */
char* perm(mode_t mode) {
	int i;
	static char perms[10]; 

	strcpy(perms, "---------");

	for (i=0; i < 3; i++) {
		if (mode & (S_IREAD >> i*3)) 
			perms[i*3] = 'r';
		if (mode & (S_IWRITE >> i*3)) 
			perms[i*3+1] = 'w';
		if (mode & (S_IEXEC >> i*3)) 
			perms[i*3+2] = 'x';
	}
	return(perms);
}
