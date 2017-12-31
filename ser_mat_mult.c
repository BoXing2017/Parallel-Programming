#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define LEN 1000

int *a;
int *b;
int *res;
struct timeval begin,end;

void readin(void)
{
	int i=0,j=0;	
	a=(int*)malloc(sizeof(int)*LEN*LEN);
	b=(int*)malloc(sizeof(int)*LEN*LEN);
	res=(int*)malloc(sizeof(int)*LEN*LEN);
	memset(res,0,sizeof(int)*LEN*LEN);
	freopen ("a.txt","r",stdin);
	for (i=0;i<LEN;i++)
		for (j=0;j<LEN;j++)
			scanf("%d",(a+i*LEN+j));
	fclose(stdin);
	freopen ("b.txt","r",stdin);
	for (i=0;i<LEN;i++)
		for (j=0;j<LEN;j++)
			scanf("%d",(b+j*LEN+i));
	fclose(stdin);
	return;
}
int main(void)
{
	int i=0,j=0,k=0;
	gettimeofday(&begin,NULL);
	readin();

	for (i=0;i<LEN;i++)
		for (j=0;j<LEN;j++)
			for (k=0;k<LEN;k++)
			{
				*(res+i*LEN+j)+=*(a+i*LEN+k)*(*(b+j*LEN+k));
			}
	
	freopen ("c.txt","w",stdout);
	for (i=0;i<LEN;i++){
		for (j=0;j<LEN;j++)
			printf("%d ",*(res+i*LEN+j));
		printf("\n");
	}
	fclose(stdout);
	
	freopen("/dev/tty", "w", stdout);
	gettimeofday(&end,NULL);
	
	double timeUse = end.tv_sec - begin.tv_sec + (end.tv_usec - begin.tv_usec)/1000000.0;
	printf("Elapsed time = %e\n", timeUse);
	
	return 0;
}
