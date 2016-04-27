#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS	50000

void *PrintHello(void *threadid)
{
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   double avarage_time = 0;
   clock_t begin, end;
   for(t=0;t<NUM_THREADS;t++){
     printf("In main: creating thread %ld\n", t);
     begin = clock();
     rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
     pthread_join(threads[t],NULL);
     end = clock();
     avarage_time += (double)(end - begin) / CLOCKS_PER_SEC;
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
       }
     }
    printf("%f\n", avarage_time/NUM_THREADS);

   /* Last thing that main() should do */
   pthread_exit(NULL);
}
