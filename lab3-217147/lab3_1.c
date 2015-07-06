#include <stdio.h>
#include "pthread.h"
#include "semaphore.h"
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include "priority_semaphore-template.h"

#define N 5

sem_t *q;
Sem *semUp;
Sem *semDown;
int max_random (int max);
int dir;
static void *
client (void *arg){
  int p, i, priority;
  struct timeval tv;

  pthread_detach (pthread_self ());


   if(dir==1){
     for(i=0;i<N;i++){
        sleep(max_random(1));
        p = max_random (100);
        sem_post(q);
        gettimeofday(&tv, NULL);
        printf("Thread %u \t\t priority %d request at time %ld%ld\n", pthread_self(), p, tv.tv_sec, tv.tv_usec);
        printf("U:Posicion: %d   Request: %d \n",semUp->pos,p);
        if(p>semUp->pos)
        {
            printf("Queued UP\n");
            priority = S_wait(semUp, p);
              gettimeofday(&tv, NULL);
        printf("Thread %u priority %d served at time %ld%ld\n", pthread_self(), priority, tv.tv_sec, tv.tv_usec);


        }else{

            printf("Queued DOWN\n");
            sem_wait(semUp->mutex);
            semDown->cnt--;
            enqueue_sorted(semDown->sem_list,-p);
            sem_post(semUp->mutex);
            printf("D:Size: %d\n",sizeList(semDown->sem_list));
        }
    }
    }else if(dir==0){

        for(i=0;i<N;i++){
            sleep(max_random(1));
            p = max_random (100);
            sem_post(q);
            gettimeofday(&tv, NULL);
            printf("Thread %u \t\t priority %d request at time %ld%ld\n", pthread_self(), p, tv.tv_sec, tv.tv_usec);
            printf("D:Posicion: %d   Request: %d %d\n",semDown->pos,p);


            if(p<semDown->pos)
            {
                printf("Queued DOWN\n");
                priority = S_wait(semDown, -p);
                  gettimeofday(&tv, NULL);
            printf("Thread %u priority %d served at time %ld%ld\n", pthread_self(), priority, tv.tv_sec, tv.tv_usec);


            }else{

                printf("Queued Up\n");
                sem_wait(semDown->mutex);
                semUp->cnt--;
                enqueue_sorted(semUp->sem_list,p);
                sem_post(semDown->mutex);
                printf("U:Size: %d\n",sizeList(semUp->sem_list));
            }
        }
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
  semUp = S_init(1);
  semDown = S_init(1);
  dir=1;

  gettimeofday(&tv, NULL);
  srand (tv.tv_usec * getpid ());
   for (i = 0; i < N; i++){
    pthread_create (&th, NULL, client, (void *)NULL);
}
  // server
while(1){
     while(1){
        sem_wait(q);
        sleep(1);
        if(dir==1){
            S_signal(semUp);
            if(sizeList(semUp->sem_list)==0){
                dir=0;
                break;
            }
        }
      }

    semDown->pos=semUp->pos;
        for (i = 0; i < N; i++){
            pthread_create (&th, NULL, client, (void *)NULL);
        }

        while(1){
            sem_wait(q);
            sleep(1);
            if(dir==0){
                S_signal(semDown);
                if(sizeList(semDown->sem_list)==0){
                    dir=1;
                    break;
                }
            }


        }
}
  exit (0);
}

int
max_random (int max)
{
  return (int) (random () % max);
}
