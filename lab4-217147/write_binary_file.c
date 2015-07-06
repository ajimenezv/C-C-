#include <stdio.h>
#include <unistd.h>


int
max_random (int max)
{
  return (int) (random () % max);
}

int
main (int argc, char ** argv) {
  FILE * fpi, *fpo;
  int i, len,j,x;
  char buf[50];
  fpi=fopen("in.txt","wb");
  for(j=0;j<100;j++){
        i=max_random(100);
    	fprintf(fpi,"%d\n", i);
  }
  fclose(fpi);

/*if (argc != 3) {
    printf ("Syntax: %s filename_input, filename_output", argv[0]);
    return (1);
  }*/
  len = 0;
  if ((fpi = fopen ("in.txt", "rt")) == NULL)
    perror ("file_input open");
  if ((fpo = fopen (argv[1], "wb")) == NULL)
    perror ("file_output open %");

  while(fgets(buf,sizeof(buf),fpi)){
  	sscanf(buf, "%d", &i);
  	fwrite(&i, sizeof(int), 1, fpo);
  }

  return (0);
}


