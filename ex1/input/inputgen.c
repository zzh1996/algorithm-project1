#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX 531441

int main(){
	int i,j,len;
	char s[18];
	FILE *f;
	srand(time(NULL));
	f=fopen("input_strings.txt","wt");
	for(i=0;i<MAX;i++){
		len=rand()%16+1;
		for(j=0;j<len;j++){
			s[j]='a'+rand()%26;
		}
		s[len]='\n';
		s[len+1]='\0';
		fputs(s,f);
	}
	fclose(f);
	return 0;
}