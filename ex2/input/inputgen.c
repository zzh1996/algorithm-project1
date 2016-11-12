#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX 1594323

int main(){
	int i,j,len;
	char s[18];
	FILE *f;
	srand(time(NULL));
	f=fopen("input_nums.txt","wt");
	for(i=0;i<MAX;i++){
		fprintf(f,"%d\n",rand()%65535+1);
	}
	fclose(f);
	return 0;
}