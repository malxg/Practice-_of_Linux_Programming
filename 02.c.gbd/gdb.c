#include <stdio.h>
int add_range(int low,int high)
{
        int sum,i;
        for(i=low;i<=high;i++)
        {
                sum=sum+i;
        }
        return sum;
}
int main(void)
{
	int result[10];
	result[0]=add_range(1,10);
	result[1]=add_range(1,100);
	printf("result[0]=%d\nresult[1]=%d\n",result[0],result[1]);
	return 0;
}

