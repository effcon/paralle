#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>


#define dataSize   10000
int data[dataSize];

int main(int argc, char* argv[])// 每次并行时即时产生多个线程
{
    int i, j;
    struct timeval start,end;
     int thread = 1;
     FILE * fpt;

     fpt = fopen("result.txt","w");


    srand(2);

    for(thread =1;thread<16;++thread)
    {
        for(int k=0;k<4;++k)
        {
#pragma omp parallel for num_threads(thread)
            for (i = 0; i < dataSize; i++)
                data[i] = rand();

            gettimeofday(&start, NULL);
            for (i = 0; i < dataSize; i++) {
                if (i % 2) {
#pragma omp parallel for num_threads(thread) default(none) shared(data) private(j)
                    for (j = 1; j < dataSize - 1; j += 2) {
                        if (data[j + 1] < data[j])
                            data[j + 1] ^= data[j], data[j] ^= data[j + 1], data[j + 1] ^= data[j];
                    }
                } else {
#pragma omp parallel for num_threads(thread) default(none) shared(data) private(j)
                    for (j = 1; j < dataSize; j += 2) {
                        if (data[j - 1] > data[j])
                            data[j - 1] ^= data[j], data[j] ^= data[j - 1], data[j - 1] ^= data[j];
                    }
                }
            }
            gettimeofday(&end, NULL);

            const long persec = 1000000;
            double costtime = (double) ((end.tv_sec - start.tv_sec) * persec + end.tv_usec - start.tv_usec) / persec;
            fprintf(fpt, "thread num:%d finish cost time: %.6f s\n", thread, (double) costtime);
        }
    }
    fclose(fpt);

    return 0;
}

/*
int main1(int argc, char* argv[])// 预先产生需要的线程
{
    int i, j, threadReal, data[dataSize];
    clock_t time;
    if (argc > 1 && *argv[1] > '0'&& *argv[1] < '9')
        threadReal = atoi(argv[1]);
    else
        threadReal = thread;
    srand(2);
#pragma omp parallel for num_threads(thread)
    for (i = 0; i < dataSize; i++)
        data[i] = rand();

    time = clock();
#pragma omp parallel num_threads(thread) default(none) shared(data) private(i, j) // 预先说明并行的参数，特别是规定使用的线程数
    for (i = 0; i < dataSize; i++)                                                              // 注意这层没有 for 子句，且 i 也作为私有变量
    {
        if (i % 2)
        {
#pragma omp for// 每次并行时使用预先规定的线程数和参数，使用完毕后保留，直到退出外层循环再销毁
            for (j = 1; j < dataSize - 1; j += 2)
            {
                if (data[j + 1] < data[j])
                    data[j + 1] ^= data[j], data[j] ^= data[j + 1], data[j + 1] ^= data[j];
            }
        }
        else
        {
#pragma omp for
            for (j = 1; j < dataSize; j += 2)
            {
                if (data[j - 1] > data[j])
                    data[j - 1] ^= data[j], data[j] ^= data[j - 1], data[j - 1] ^= data[j];
            }
        }
    }
    time = clock() - time;
    printf("Sort finished!\n");
    for (i = 0; i < dataSize - 1; i++)
    {
        if (data[i] > data[i + 1])
        {
            printf("Sort error at %d, data[i] = %d,data[i+1] = %d, time = %ld ms\n", i, data[i], data[i + 1], time);
            break;
        }
    }
    if (i == dataSize - 1)
        printf("Sort correct, time = %ld ms\n", time);
    getchar();
    return 0;
}

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>

enum bool
{
    false,true
};

#define dataSize 20000
#define threads 4
int data[dataSize];


 void  displayArray( int* array)
{
int i;

for(i=0;i<dataSize;++i)
{
if(0==i%1000) {
    printf("%d  ", *(array + i));
}
}
}


enum bool checkArray(int* a,int length,enum bool reverse)
{
     int i;

     if(!reverse)
     {
         for(i=0;i<length-1;++i)
             if(a[i]>a[i+1])
                 return 0;
     }

     else{
         for(i=0;i<length-1;++i)
             if(a[i]<a[i+1])
                 return 0;
     }

     return 1;
}

int main() {



   enum bool issorted = false;
    struct timeval start,end;
   int i=0;





   for(int i=0;i<dataSize;++i)
       data[i]= random()%dataSize;


    int thread_num[4]={0,0,0,0};
    gettimeofday(&start,NULL);


    omp_set_num_threads(threads);

   while(!issorted){
       issorted=true;

    #pragma omp parallel for num_threads(threads)   private(i)  shared(data)
       for (i = 0; i < dataSize - 1; i += 2) {
           if (data[i] > data[i + 1]) {
               int temp;

               temp = data[i];
               data[i] = data[i + 1];
               data[i + 1] = temp;

               issorted = false;
           }

       }

#pragma omp parallel for num_threads(threads)  private(i)  shared(data)
           for ( i = 1; i < dataSize - 1; i += 2)
               if (data[i] > data[i + 1]) {
                   int temp;

                   temp = data[i];
                   data[i] = data[i + 1];
                   data[i + 1] = temp;

                   issorted = false;
               }
       }

    gettimeofday(&end,NULL);

    const long persec =1000000;
    double costtime =(double)((end.tv_sec-start.tv_sec)*persec+end.tv_usec-start.tv_usec)/persec;

   if(1==checkArray(data,dataSize,false))
       printf("correct\n");
   else
       printf("uncorrect\n");

   printf("%d %d %d %d\n",thread_num[0],thread_num[1],thread_num[2],thread_num[3]);
       printf("finish\ncost time: %.6fs",(double)costtime);
    return 0;
}*/