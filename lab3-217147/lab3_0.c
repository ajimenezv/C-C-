#include <stdio.h>
#include "pthread.h"
#include "semaphore.h"
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include "lab3_0.h"

#define N 5

sem_t *q;
Sem *server_sem;

int max_random (int max);

static void *
client (void *arg){
  int p, i, priority;
  struct timeval tv;

  pthread_detach (pthread_self ());

  for(i=0;i<5;i++){
    sleep(max_random(5));
    p = max_random (100);
    sem_post(q);
    gettimeofday(&tv, NULL);
    printf("Thread %u \t\t priority %d request at time %ld%ld\n", pthread_self(), p, tv.tv_sec, tv.tv_usec);
    priority = S_wait(server_sem, p);
    gettimeofday(&tv, NULL);
    printf("Thread %u priority %d served at time %ld%ld\n", pthread_self(), priority, tv.tv_sec, tv.tv_usec);
  }
}

int
main (int argc, char **argv){
  pthread_t th;
  void *retval;
  int i, k;
  struct timeval tv;

  setbuf(stdout,0);
  q = (sem_t *) malloc (sizeof (sem_t));
  sem_init (q, 0, 0);
  server_sem = S_init(1);

  gettimeofday(&tv, NULL);
  srand (tv.tv_usec * getpid ());

  for (i = 0; i < N; i++)
    pthread_create (&th, NULL, client, (void *)NULL);

  // server
  while(1){
  	sem_wait(q);
    sleep(1);
//    printf("request served\n");
    S_signal(server_sem);
  }

  exit (0);
}

int
max_random (int max)
{
  return (int) (random () % max);
}
