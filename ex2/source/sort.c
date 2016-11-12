#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define MAX 1594323

int num[MAX];
int B[MAX];
int L[MAX+10];
int R[MAX+10];
int C[65536];

void load_nums(){
	int i;
	FILE *f=fopen("../input/input_nums.txt","rt");
	for(i=0;i<MAX;i++){
		fscanf(f,"%d",&num[i]);
	}
	fclose(f);
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
	for(i=0;i<N;i++){
		fprintf(f,"%d\n",num[i]);
	}
	fclose(f);
	fflush(stdout);
}

void bubble_sort(int N){
	int i,j,t;
	for(i=0;i<N-1;i++){
		for(j=0;j<N-i-1;j++){
			if(num[j]>num[j+1]){
				t=num[j];
				num[j]=num[j+1];
				num[j+1]=t;
			}
		}
	}
}

int partition(int p,int r){
	int x=num[r];
	int t;
	int i=p-1;
	int j;
	for(j=p;j<=r-1;j++){
		if(num[j]<=x){
			i++;
			t=num[i];
			num[i]=num[j];
			num[j]=t;
		}
	}
	t=num[i+1];
	num[i+1]=num[r];
	num[r]=t;
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

void merge(int p,int q,int r){
	int n1=q-p+1;
	int n2=r-q;
	int i,j,k;
	for(i=0;i<n1;i++)
		L[i]=num[p+i];
	for(j=0;j<n2;j++)
		R[j]=num[q+j+1];
	L[n1]=65536;
	R[n2]=65536;
	i=0;
	j=0;
	for(k=p;k<=r;k++){
		if(L[i]<=R[j]){
			num[k]=L[i];
			i++;
		}else{
			num[k]=R[j];
			j++;
		}
	}
}

void merge_sort(int p,int r){
	int q;
	if(p<r){
		q=(p+r)/2;
		merge_sort(p,q);
		merge_sort(q+1,r);
		merge(p,q,r);
	}
}

void radix_sort(int N,int d){
	int r,j;
	int C1,C0;
	for(r=0;r<d;r++){
		C1=C0=0;
		for(j=0;j<N;j++)
			if((num[j]>>r)&1)
				C1++;
			else
				C0++;
			C1+=C0;
		for(j=N-1;j>=0;j--){
			if((num[j]>>r)&1)
				C1--;
			else
				C0--;
			B[(num[j]>>r)&1?C1:C0]=num[j];
		}
		memcpy(num,B,N*sizeof(int));
	}
}

void counting_sort(int N,int k){
	int i,j;
	for(i=0;i<=k;i++)
		C[i]=0;
	for(j=0;j<N;j++)
		C[num[j]]++;
	for(i=1;i<=k;i++)
		C[i]+=C[i-1];
	for(j=N-1;j>=0;j--){
		C[num[j]]--;
		B[C[num[j]]]=num[j];
	}
	memcpy(num,B,N*sizeof(int));
}

int main(){
	int n,N;
	double diff;
	struct timespec start,stop;
	for(n=3;n<=13;n+=2){
		N=pow3(n);

		load_nums();
		clock_gettime(CLOCK_MONOTONIC,&start);
		bubble_sort(N);
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		printf("bubble_sort%d %.9fs\n",n,diff);
		output("bubble_sort",n);

		load_nums();
		clock_gettime(CLOCK_MONOTONIC,&start);
		quick_sort(0,N-1);
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		printf("quick_sort%d %.9fs\n",n,diff);
		output("quick_sort",n);

		load_nums();
		clock_gettime(CLOCK_MONOTONIC,&start);
		merge_sort(0,N-1);
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		printf("merge_sort%d %.9fs\n",n,diff);
		output("merge_sort",n);

		load_nums();
		clock_gettime(CLOCK_MONOTONIC,&start);
		radix_sort(N,16);
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		printf("radix_sort%d %.9fs\n",n,diff);
		output("radix_sort",n);

		load_nums();
		clock_gettime(CLOCK_MONOTONIC,&start);
		counting_sort(N,65535);
		clock_gettime(CLOCK_MONOTONIC,&stop);
		diff=(stop.tv_sec-start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec )/1000000000L;
		printf("counting_sort%d %.9fs\n",n,diff);
		output("counting_sort",n);
	}
	return 0;
}