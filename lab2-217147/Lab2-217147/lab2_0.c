#include <stdio.h>
#include <stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<ctype.h>
#include <string.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>


typedef struct {
    int i;
    int j;
}Position;

double **dmat1, **dmat2, **dmatprod;
int  nr1, nc1, nr2, nc2, dim, i, j, k,count=0;

void ** allocaM2d (int nr, int nc, int dim);
void* prodmat(void *arg);
void printM();

sem_t barrier;
sem_t mutex;


int main(void) {


    pthread_t *th;

    printf("Rows and Columns of the first matrix: ");
    scanf ("%d%d",&nr1,  &nc1);
    do {
      printf("Rows and Columns of the second matrix\n");
      printf("The number of rows must be equal to nc1 ");
      scanf ("%d%d",&nr2,  &nc2);
    } while (nr2 != nc1);

    //  dynamic allocation
    th = (pthread_t *) malloc (nr1*nc2 * sizeof (pthread_t));
    dim = sizeof(double);

    dmat1 = (double **) allocaM2d(nr1,  nc1, dim);
    dmat2 = (double **) allocaM2d(nr2,  nc2, dim);
    dmatprod = (double **) allocaM2d(nr1, nc2, dim);

    k=0;
    for(i=0;i<nr1;i++)
      for(j=0;j<nc1;j++)
        dmat1[i][j] = k++;

    for(i=0;i<nr1;i++){
      for(j=0;j<nc1;j++)
        printf("%lf ", dmat1[i][j]);
      printf("\n");
    }

    printf("\n");
    k=0;
    for(i=0;i<nr2;i++)
      for(j=0;j<nc2;j++)
        dmat2[i][j] = k++;

    for(i=0;i<nr2;i++){
      for(j=0;j<nc2;j++)
        printf("%lf ", dmat2[i][j]);
      printf("\n");
    }

    for(i=0;i<nr2;i++){
      for(j=0;j<nc2;j++)
        dmatprod[i][j]=0;
    }

    sem_init(&mutex,1,1);
    sem_init(&barrier,1,0);

    int w=0;
    Position *pos1;
    for(i=0;i<nr1;i++){
        for(j=0;j<nc2;j++){
            pos1=(Position *) malloc (sizeof (Position));
            pos1->i=i;
            pos1->j=j;
            pthread_create (&th[w], NULL, prodmat, pos1);
            w++;

        }
    }
    pthread_exit (0);
    sem_destroy(&mutex);
    sem_destroy(&barrier);


}

/************************************************************/

void **
allocaM2d (int nr, int nc, int dim){
int i;
void ** m;
char * vd;  //  byte pointer

m = (void **) malloc(nr * sizeof(void *));
vd = (char *) malloc(nr * nc * dim);

for(i=0;i<nr;i++)
  m[i] = &vd[i * nc * dim];

return (void **) m;	// return matrix pointer
}

/***********************************************************************/

void*
prodmat( void *arg)
{
    sem_wait(&mutex);
     Position *pos = (Position *) arg;
   // printf("i=%d j=%d \n",pos->i,pos->j);
    int k;
      for (k=0; k<nc1; k++)
        dmatprod[pos->i][pos->j] += dmat1[pos->i][k]*dmat2[k][pos->j];
    count++;
    if(count==nr1*nc2)
    {
        printM();
        sem_post(&barrier);
    }
    sem_post(&mutex);


    sem_wait(&barrier);
    sem_post(&barrier);

  return;
}

void printM(){

    printf("\n");
    for(i=0;i<nr1;i++){
      for(j=0;j<nc2;j++)
        printf("%lf ", dmatprod[i][j]);
      printf("\n");
    }
}
