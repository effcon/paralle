#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static long numSteps = 10000000;
double steps = 1.0/double (numSteps);
double sum =0.0;
int  stepByThread;
pthread_mutex_t sum_lock = PTHREAD_MUTEX_INITIALIZER;


void* calculate_pi(void*);



void  calculate_thread_num(int thread_num)
{
    pthread_t mythread[thread_num];
    int pos[thread_num];

    time_t start,end;

    start = clock();

    stepByThread = numSteps/thread_num;

    int i;
    for(i=0;i<thread_num;++i)
    {

        pos[i]=i*stepByThread;
        pthread_create(&mythread[i],NULL,calculate_pi,(void*)(pos+i));
        pthread_join(mythread[i],NULL);
    }

    end =clock();


    sum = sum*steps;
    double costTime = double(end -start)/CLOCKS_PER_SEC;

    printf("cost time:%lf s\n",costTime);
    printf("ans:%.15f\n",sum);
}

void* calculate_pi(void* arg)
{
    int a =*(int*)arg;

    int i;
    for(i=a;i<a+stepByThread;++i)
    {
       double x;

       x =(i+0.5)*steps;

       pthread_mutex_lock(&sum_lock);
       sum =sum+4/(1+x*x);
       pthread_mutex_unlock(&sum_lock);
    }

    return NULL;
}


int main()
{
/*
    pthread_t mythread[n];
    int pos[n];

    //   pthread_create(&mythread,NULL,calculate_pi,(int*)&start);


    time_t start,end;

    start = clock();


    int i;
    for(i=0;i<n;++i)
    {

        pos[i]=i*stepByThread;
        pthread_create(&mythread[i],NULL,calculate_pi,(void*)(pos+i));
        pthread_join(mythread[i],NULL);
    }

    end =clock();


    sum = sum*steps;
    printf("cost time:%lf s\n",double(end -start)/CLOCKS_PER_SEC);
    printf("ans:%.15f\n",sum);*/

   calculate_thread_num(3);
}