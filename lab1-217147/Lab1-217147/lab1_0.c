#include<stdio.h>

	int main()
	{
		FILE *ptrf;
		int x;
        scanf("%d",&x);
		ptrf =fopen("output1.bin", "wb");
        fwrite(&x, sizeof(int), 1, ptrf);
		fclose(ptrf);

		ptrf =fopen("output1.bin", "rb");
        fread(&x, sizeof(int), 1, ptrf);
        printf("%d\n",x);
		fclose(ptrf);

		return  0;
	}
