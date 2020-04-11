#include<stdio.h>
#include<string.h>
#include"abc.h"
void copy(char from[], char to[])
{
	int i;
   	i = 0;
    	while ((to[i] = from[i]) != '\0')
		++i;
}