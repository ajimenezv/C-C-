// Quicksort

//******************************************************************************

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct {
    int left;
    int right;
}Pos;

int *v;

void *Quicksort (void *arg);

int
main (int argc, char ** argv) {

  pthread_t th;
  int fd;
  int i, n, len;
  struct stat stat_buf;
  char *paddr;
  void *ret;

  if (argc != 2) {
    printf ("Syntax: %s filename", argv[0]);
    return (1);
  }

  if ((fd = open (argv[1], O_RDWR)) == -1)
    perror ("open");

  if (fstat (fd, &stat_buf) == -1)
    perror ("fstat");
  len = stat_buf.st_size;
  n = len / sizeof(int);

  paddr = mmap ((caddr_t) 0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (paddr == (caddr_t) - 1)
    perror ("mmap");

  close (fd);

  v =(int *) paddr;


  for (i=0; i<n;i++)
    printf("%d ", v[i]);

printf("\n------------------\n");

  //Quicksort(vet, 0, n-1);
  Pos *lol;
  lol= (Pos *) malloc (sizeof(Pos));
  lol->right= n-1;
  lol->left=0;

  pthread_create (&th, NULL, Quicksort, lol);
  pthread_join (th, &ret);

  for (i=0; i<n;i++)
    printf("%d ", v[i]);

printf("\n ");
  return (0);
}


void *
Quicksort (void *argv) {
  pthread_t thleft,thright;
  void *ret;
  Pos *pos = (Pos *) argv;
  int p = pos->left;
  int r = pos->right;
  int i, j, x, tmp;

  if (p >= r)
    return;

  x = v[p];
  i = p - 1;
  j = r + 1;

  while (i < j) {
    while (v[--j] > x);
    while (v[++i] < x);
    if (i < j)	{
	    tmp = v[i];
      v[i] = v[j];
      v[j] = tmp;
    }
  }
//  Quicksort (v, p, j);
  Pos *left=(Pos *)malloc (sizeof(Pos));
  left->left=p;
  left->right=j;
  pthread_create (&thleft, NULL, Quicksort, left);

//Quicksort (v, j + 1, r);
  Pos *right=(Pos *)malloc (sizeof(Pos));
  right->left=j+1;
  right->right=r;
  pthread_create (&thright, NULL, Quicksort, right);

  pthread_join (thleft, &ret);
  pthread_join (thright, &ret);

}
