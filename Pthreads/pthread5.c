#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


pthread_cond_t wait  = PTHREAD_COND_INITIALIZER;
pthread_cond_t wake_up  = PTHREAD_COND_INITIALIZER;
pthread_cond_t start  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int tasks;

void *main_thread()
{
    int i = 0, rValue, t;
        //unlock mutex
        rValue = pthread_mutex_unlock(&mutex);

        //do stuff
        for(t = 1; t <= 5; t++){
            printf("creando tarea %d\n", t);
            rValue = pthread_cond_signal(&wake_up);
        }

        //signal condition of thread b

        //lock mutex
        rValue = pthread_mutex_lock(&mutex);

        //wait for turn

}
void *worker()
{
    int n = 0, rValue;

        //lock mutex
        rValue = pthread_mutex_lock(&mutex);

        //wait for turn
        while( pthread_cond_wait(&wake_up, &mutex) != 0 );

        //unlock mutex
        rValue = pthread_mutex_unlock(&mutex);

        //do stuff
        printf("tarea realizada \n");

        //signal condition a
        rValue = pthread_cond_signal(&wait);

}

/*void *worker(void *threadid)
{
    while (pthread_cond_wait(&wait, &mutex) != 0);


    /*pthread_mutex_lock(&mutex);
    while (!tasks)
        pthread_cond_wait(&wait, &mutex);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&wake_up);
    printf("Haciendo tarea \n");
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    tasks = 0;
    pthread_cond_signal(&wait);
    printf("dormir \n");
    pthread_mutex_unlock(&mutex);
}
void *main_thread(void *threadid)
{
    int n_tasks=5;
    int i;
    for (i = 0; i < n_tasks; ++i)
    {
        pthread_mutex_lock(&mutex);
        tasks = 1;
        pthread_cond_signal(&wake_up);
        pthread_mutex_unlock(&mutex);
    }
    int pthread_cond_broadcast(wake_up);
}*/

int main(int argc, char *argv[])
{
    int nthreads=atof(argv[1]);
    pthread_t threads[nthreads];
    int rc;
    long t;
    long i;
    pthread_create(&threads[0], NULL, main_thread, (void *)0);
    for (i = 1; i < nthreads; ++i)
    {
        pthread_create(&threads[i], NULL, worker, (void *)i);
    }
    for (i = 1; i < nthreads; ++i)
    {
        pthread_join(threads[i],NULL);
    }

    pthread_exit(NULL);
}
