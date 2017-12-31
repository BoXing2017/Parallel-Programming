#include<stdio.h>
int main()
{
	FILE * fp;
	fp=fopen("a.txt","w");
	int n=0,i=0,j=0;
	for(i=0;i<1000;i++)
	{
		for(j=0;j<1000;j++)
		{
			n=((j+i)*91)%97;
			if(n==0) n=11;
			fprintf(fp,"%d ",n);
		}
		fputc('\n',fp);
	}
	fclose(fp);
	fp=fopen("b.txt","w");
	for(i=0;i<1000;i++)
	{
		for(j=0;j<1000;j++)
		{
			n=((j+i)*97)%99;
			if(n==0) n=13;
			fprintf(fp,"%d ",n);
		}
		fputc('\n',fp);
	}
	fclose(fp);
	printf("矩阵生成成功\n");
	return 0;
}
