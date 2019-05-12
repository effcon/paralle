#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define numSteps  100000000
#define maxthread 40000
double steps = 1.0/ numSteps;
int  stepByThread;


pthread_t mythread[maxthread];
double sum[maxthread];
int thread_nums[100];
FILE* fpt;



void* calculate_Pi(void* arg)
{
    int* pArg = (int*)arg;

    int pos = *pArg;
    int begin =pos*stepByThread;
    int end = begin+stepByThread;

    int i;
    double x;



    for(i=begin;i<end;++i)
    {
        x = (i+0.5)*steps;
        sum[pos] = sum[pos]+4.0/(1+x*x);
    }

    delete pArg;
}


void calculate(int n)
{
    timeval start,end;
    int i;
    gettimeofday(&start,NULL);

    for(i=0;i<n;++i)
    {
        int * p =new int ;
        *p =i;
        sum[i]=0;
        pthread_create(&mythread[i],NULL,calculate_Pi,(void *)p);
    }


    double total =0.0;
    for(i=0;i<n;++i)
    {
        pthread_join(mythread[i],NULL);
        total =total+sum[i];
    }
    gettimeofday(&end,NULL);

    total =total*steps;

    const long persec =1000000;
    double costtime =(double)((end.tv_sec-start.tv_sec)*persec+end.tv_usec-start.tv_usec)/persec;



    fprintf(fpt,"thread num: %d cost time:%.6f ans %.15lf\n",n,(double)costtime,total);
}


int main()
{

    int i;

    for(i=0;i<10;++i)
        thread_nums[i]=i+1;
    for(i=10;i<100;++i)
        thread_nums[i]=thread_nums[i-1]*1.1;



     int n;
     fpt = fopen("result.txt","w");

     for(i=0;i<100;++i) {

         n= thread_nums[i];
         stepByThread = numSteps / n;
         calculate(n);
     }
}
