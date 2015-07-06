#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/timeb.h>
#include <stdlib.h>

sem_t me;
sem_t mutex;
sem_t barrier;

int num_threads, vet_size, *vet, count=0,ex=0;
//{}
static void *
sorter (void *arg)
{
    while(1)
    {
        sem_wait(&mutex);

        int *a = (int *) arg;
            if(vet[*a+1]>vet[*a]){
                int aux=vet[*a];
                vet[*a]=vet[*a+1];
                vet[*a+1]=aux;
            }

        sem_post(&me);

        count++;

        if(count==num_threads)
        {
                count=0;
                ex=0;
                int i;
                for(i=0;i<vet_size;i++)
                {
                    if(vet[i+1]>vet[i])
                    {
                        ex=1;
                    }
                }
            if(ex==1)
            {
                sem_post(&barrier);


            }else {
                 printf("\n");
                int i;
                for (i = 0; i < vet_size; i++) {
                            printf ("%d ", vet[i]);
                          }
                exit(0);
                }
        }


        sem_post(&mutex);
        sem_wait(&barrier);
        sem_post(&barrier);
        }
}

int
main (int argc, char **argv)
{
  pthread_t *th;
  void *retval;
  int i, *pi;
  struct timeb tb;

  if (argc != 2) {
    printf ("Syntax: %s vet_size\n", argv[0]);
    return (1);
  }

  setbuf(stdout,0);  // we don't want stdout buffering
  vet_size = atoi (argv[1]);
  num_threads = vet_size - 1;
  vet = (int *) malloc (vet_size * sizeof (int));

  th = (pthread_t *) malloc (num_threads * sizeof (pthread_t));
  ftime (&tb);
  srand (tb.millitm * getpid ());
  for (i = 0; i < vet_size; i++) {
    vet[i] = max_random (1000);
    printf ("%d ", vet[i]);
  }
printf("\n");

  /* Create the threads */

  sem_init(&me,1,1);
  sem_init(&mutex,1,1);
    sem_init(&barrier,1,1);
  for (i = 0; i < num_threads; i++) {
    pi = (int *) malloc (sizeof (int));
    *pi = i;
    pthread_create (&th[i], NULL, sorter, pi);
  }

  sem_destroy(&me);
    sem_destroy(&mutex);
        sem_destroy(&barrier);
  pthread_exit (0);


}

int
max_random (int max)
{
  return (int) (random () % max);
}
