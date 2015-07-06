// LINUX LINUX LINUX LINUX LINUX LINUX LINUX LINUX LINUX LINUX
//Archivos de texto
#include<stdio.h>

	int main()
	{
		FILE *ptrf;
		int x;
        scanf("%d",&x);

		ptrf =fopen("output.txt", "w");

        fprintf(ptrf,"%d\n", x);

		fclose(ptrf);

		char buf[1000];

    		ptrf =fopen("output.txt","r");
    		if (!ptrf)
        		return 1;

    		while (fgets(buf,1000, ptrf)!=NULL)
        		printf("%s",buf);

		fclose(ptrf);

		return  0;
	}
