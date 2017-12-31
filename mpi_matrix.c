#include<stdio.h>
#include<stdlib.h>
#include <memory.h> 
#include<time.h>
#include<mpi.h>

#define length 1000
#define count_thread 4

int *data_a;           //声明用于计算和储存结果的变量
int *data_b;
int *changed_data_b;
int *result;

MPI_Comm comm=MPI_COMM_WORLD;

void init()                //数据初始化，从文件中读取数据
{
	data_b = (int*)malloc(sizeof(int)*length*length);        //为各个变量分配空间
	data_a = (int*)malloc(sizeof(int)*length*length);
	result = (int*)malloc(sizeof(int)*length*length);
	memset(result,0,length);                           //初始化内存空间
	
	FILE *fp1,*fp2;
	fp1=fopen("a.txt","r");
	fp2=fopen("b.txt","r");
	int i,j;
	for(i=0;i<length;i++)
	{
		for(j=0;j<length;j++)
		{
			fscanf(fp1,"%d",&data_a[i*length+j]);
			fscanf(fp2,"%d",&data_b[j*length+i]);
		}
	}
	
	fclose(fp1);
	fclose(fp2);
}

void* calc()                   //矩阵乘法的运算
{
	int comm_sz;
	int my_rank;	
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	
	/*init();
	MPI_Bcast(data_a,length*length,MPI_INT,0,comm);
	MPI_Bcast(data_b,length*length,MPI_INT,0,comm);*/
	
	int start,end,i,j,k;
	int skip = length/comm_sz;
	start = skip*my_rank;
	end = start+skip;
	if(end > length*length)
		end = length*length;
		
	int sub_a[length*skip];
	int sub_b[length*skip];
	int sub_result[length*skip];
	
	if(my_rank == 0)
		init();
	else data_b = (int*)malloc(sizeof(int)*length*length);  
	MPI_Scatter(data_a,length*skip,MPI_INT,sub_a,length*skip,MPI_INT,0,comm);
	MPI_Bcast(data_b,length*length,MPI_INT,0,comm);
	
	for(i=0;i<end-start;i++)
	{
		for(j=0;j<length;j++)
		{
		int sum = 0;
			for(k=0;k<length;k++)
				 sum += sub_a[i*length+k] * data_b[j*length+k]; 		
		sub_result[i*length+j] = sum;
		}
		
	}
	MPI_Gather(sub_result,length*skip,MPI_INT,result,length*skip,MPI_INT,0,comm);

	if (my_rank==0)
	{
	
	FILE *fp;
	fp=fopen("c.txt","w");
	int i,j;
	for(i=0;i<length;i++)
	{
		for(j=0;j<length;j++)
			fprintf(fp,"%d ",result[i*length+j]);
		fprintf(fp,"\n");
	}
	fclose(fp);
	
	if(result)
		free(result);	
	if(data_a)
		free(data_a);
	if(data_b)
		free(data_b);
	}
}

int main()
{
	struct timeval start,end;
	
	gettimeofday(&start,NULL);
	calc();
	gettimeofday(&end,NULL);
	double timeUse = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
	
	printf("the time is %lf\n",timeUse);
	return 0;
}
