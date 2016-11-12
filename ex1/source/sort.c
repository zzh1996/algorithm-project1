#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define MAX 531441

typedef struct{
	int l;
	char s[18];
} str;

str *s[MAX];
str *L[MAX+10];
str *R[MAX+10];
str data[MAX];

void load_strings(){
	int i;
	FILE *f=fopen("../input/input_strings.txt","rt");
	for(i=0;i<MAX;i++){
		fgets(data[i].s,18,f);
		data[i].l=strlen(data[i].s)-1;
	}
	fclose(f);
}

void restore_order(){
	int i;
	for(i=0;i<MAX;i++){
		s[i]=&data[i];
	}
}

int cmp(str *a,str *b){
	if(a==NULL&&b==NULL)
		return 0;
	else if(a==NULL)
		return 1;
	else if(b==NULL)
		return -1;
	else if(a->l!=b->l)
		return a->l-b->l;
	else
		return strcmp(a->s,b->s);
}

int pow3(int n){
	int i,r=1;
	for(i=0;i<n;i++){
		r*=3;
	}
	return r;
}

void output(char *name,int n){
	char filename[100];
	int i;
	int N=pow3(n);
	sprintf(filename,"../output/%s/result_%d.txt",name,n);
	FILE *f=fopen(filename,"wt");
	if(!f){
		printf("Open file failed!\n");
		exit(-1);
	}
	for(i=0;i<N;i++){
		fputs(s[i]->s,f);
	}
	fclose(f);
	fflush(stdout);
}

void insertion_sort(int N){
	int i,j;
	str *k;
	for(j=1;j<N;j++){
		k=s[j];
		i=j-1;
		while(i>=0&&cmp(s[i],k)>0){
			s[i+1]=s[i];
			i--;
		}
		s[i+1]=k;
	}
}

int heap_size;

void max_heapify(int i){
	int l=2*i+1;
	int r=2*i+2;
	int largest;
	str *t;
	if(l<heap_size&&cmp(s[l],s[i])>0)
		largest=l;
	else
		largest=i;
	if(r<heap_size&&cmp(s[r],s[largest])>0)
		largest=r;
	if(largest!=i){
		t=s[i];
		s[i]=s[largest];
		s[largest]=t;
		max_heapify(largest);
	}
}

void build_max_heap(int N){
	int i;
	heap_size=N;
	for(i=N/2-1;i>=0;i--)
		max_heapify(i);
}

void heap_sort(int N){
	int i;
	str *t;
	build_max_heap(N);
	for(i=N-1;i>=1;i--){
		t=s[0];
		s[0]=s[i];
		s[i]=t;
		heap_size--;
		max_heapify(0);
	}
}

void merge(int p,int q,int r){
	int n1=q-p+1;
	int n2=r-q;
	int i,j,k;
	for(i=0;i<n1;i++)
		L[i]=s[p+i];
	for(j=0;j<n2;j++)
		R[j]=s[q+j+1];
	L[n1]=NULL;
	R[n2]=NULL;
	i=0;
	j=0;
	for(k=p;k<=r;k++){
		if(cmp(L[i],R[j])<=0){
			s[k]=L[i];
			i++;
		}else{
			s[k]=R[j];
			j++;
		}
	}
	/*printf("Merge result %d %d %d\n",p,q,r);
	for(int x=p;x<=r;x++){
		printf("%p ",s[x]);
	}
	printf("\n");*/
}

void merge_sort(int p,int r){
	int q;
	if(p<r){
		q=(p+r)/2;
		merge_sort(p,q);
		merge_sort(q+1,r);
		merge(p,q,r);
	}
	//printf("finished %d %d\n",p,r);
}

int partition(int p,int r){
	str *x=s[r];
	str *t;
	int i=p-1;
	int j;
	for(j=p;j<=r-1;j++){
		if(cmp(s[j],x)<=0){
			i++;
			t=s[i];
			s[i]=s[j];
			s[j]=t;
		}
	}
	t=s[i+1];
	s[i+1]=s[r];
	s[r]=t;
	return i+1;
}

void quick_sort(int p,int r){
	int q;
	if(p<r){
		q=partition(p,r);
		quick_sort(p,q-1);
		quick_sort(q+1,r);
	}
}

void shell_sort(int N){
	int gaps[5]={21,15,7,3,1};
	int i,j,g,gap;
	str *t;
	for(g=0;g<5;g++){
		gap=gaps[g];
		for(i=gap;i<N;i++){
			t=s[i];
			for(j=i;j>=gap&&cmp(s[j-gap],t)>0;j-=gap)
				s[j]=s[j-gap];
			s[j]=t;
		}
	}
}

int main(){
	int n,N;
	double diff;
	struct timespec start,stop;
	load_strings();
	for(n=2;n<=12;n+=2){
	//for(n=2;n<=10;n+=2){
		N=pow3(n);

		restore_order();
		clock_gettime(CLOCK_MONOTONIC,&start);
		insertion_sort(N);
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		printf("insertion_sort%d %.9fs\n",n,diff);
		output("insertion_sort",n);

		restore_order();
		clock_gettime(CLOCK_MONOTONIC,&start);
		heap_sort(N);
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		printf("heap_sort%d %.9fs\n",n,diff);
		output("heap_sort",n);

		restore_order();
		clock_gettime(CLOCK_MONOTONIC,&start);
		merge_sort(0,N-1);
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		printf("merge_sort%d %.9fs\n",n,diff);
		output("merge_sort",n);

		restore_order();
		clock_gettime(CLOCK_MONOTONIC,&start);
		quick_sort(0,N-1);
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		printf("quick_sort%d %.9fs\n",n,diff);
		output("quick_sort",n);

		restore_order();
		clock_gettime(CLOCK_MONOTONIC,&start);
		shell_sort(N);
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		printf("shell_sort%d %.9fs\n",n,diff);
		output("shell_sort",n);
	}
	return 0;
}