#include <stdio.h>
#include "pthread.h"
#include "semaphore.h"
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>


typedef struct List_elem
{
  sem_t *sem;
  int priority;
  struct List_elem *next;
} List_elem;

typedef struct Sem
{
  int cnt;
  List_elem *sem_list;
  sem_t *mutex;
} Sem;

Sem *S_init (int value);
int S_wait (Sem * s, int prio);
void S_signal (Sem * s);
List_elem *enqueue_sorted (List_elem * head, int priority);
List_elem *dequeue_sorted (List_elem * head);

// Sem *S1, *S2;  Virtual semaphore declaration


List_elem *
enqueue_sorted (List_elem * head, int priority)
{
  List_elem *p, *new_elem;
  p = head;
  while (priority > p->next->priority)
    p = p->next;

  new_elem = (List_elem *) malloc (sizeof (List_elem));
  new_elem->sem = (sem_t *) malloc (sizeof (sem_t));
  sem_init (new_elem->sem, 0, 0);
  new_elem->priority = priority;
  new_elem->next = p->next;
  p->next = new_elem;

  return new_elem;
}

List_elem *
dequeue_sorted (List_elem * head)
{
  List_elem *p;
  p = head->next;
  sem_post(head->next->sem);
  head->next = head->next->next;
  return p;
}

Sem *S_init (int value){
    Sem *x=(Sem *)malloc (sizeof(Sem));
    x->cnt=0;
    x->mutex=(sem_t *) malloc (sizeof (sem_t));
    sem_init (x->mutex, 0, value);
    x->sem_list= (List_elem *) malloc (sizeof (List_elem));
    x->sem_list->priority=-1;
    x->sem_list->next=(List_elem *) malloc (sizeof (List_elem));
    x->sem_list->next->priority=101;
    return x;
}
int S_wait (Sem * s, int prio){
    List_elem *d;

    sem_wait(s->mutex);
    s->cnt--;
    if(s->cnt<0){
       d=enqueue_sorted(s->sem_list,prio);
       sem_post(s->mutex);
       sem_wait(d->sem);
       free(d);
    }
    else{
        sem_post(s->mutex);
    }


    return prio;
}
void S_signal (Sem * s){
    sem_wait(s->mutex);
    s->cnt++;
    if(s->cnt<=0){
        dequeue_sorted(s->sem_list);
    }
    sem_post(s->mutex);
/*
    List_elem *a=s->sem_list;
    while(a->next!=NULL){
        printf("El =%d\n",a->priority);
        a=a->next;
    }*/

}
