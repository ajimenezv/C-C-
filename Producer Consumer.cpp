//Lab N°10
//Exercise N°2
//Andres Camilo Jimenez
//217147

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>   
#include <windows.h>   
#include <stdio.h>   
#include <malloc.h>


HANDLE me;               
HANDLE full, empty;         

int *product;     
int bufferSize;
int buffer = 0;                     

DWORD WINAPI Producer(LPVOID lpPara)
{
	while (TRUE){

		WaitForSingleObject(empty, INFINITE);
		WaitForSingleObject(me, INFINITE);

		int res;
		if (buffer <= bufferSize)
		{
			product[buffer] = 1;
			buffer++;
			res= 1;
		}else{
			res= 0;
		}

		if (res == 1)
		{
			printf("Element produced, buffer now: %d of %d \n\n", buffer,bufferSize);
			if (buffer == bufferSize) printf("\n Buffer is full!!!\n\n");
			ReleaseMutex(me);
			ReleaseSemaphore(full, 1, NULL);
			Sleep(1000);
		}else{
			ReleaseMutex(me);
		}
	}

	return 0;
}


DWORD WINAPI Consumer(LPVOID lpPara)
{
	while (TRUE){
		WaitForSingleObject(full, INFINITE);
		WaitForSingleObject(me, INFINITE);

		int res;
		if (0 < buffer)
		{
			product[buffer] = 0;
			buffer--;
			res= 1;
		}
		else{
			res= 0;
		}

		if (res>0)
		{
			printf("Product consumed, buffer now: %d of %d \n\n", buffer,bufferSize);
			product[buffer] = 0;
			if (buffer == 0) printf("\n Buffer is empty!!!\n\n");

			ReleaseMutex(me);
			ReleaseSemaphore(empty, 1, NULL);
			Sleep(1000);

		}else{
			ReleaseMutex(me);
		}
	}
	return 0;
}


int main()
{
	int i,nP,nC;

	HANDLE producers[100];
	HANDLE consumers[100];

	me = CreateMutex(NULL, FALSE, NULL);
	full = CreateSemaphore(NULL, 0, bufferSize, NULL);
	empty = CreateSemaphore(NULL, bufferSize, bufferSize, NULL);

	printf("Write the number of producers\n");
	scanf("%d", &nP);
	printf("Write the number of consumers\n");
	scanf("%d", &nC);
	printf("Write the size of the buffer\n");
	scanf("%d", &bufferSize);
	printf("\n\n");

	product = (int*)malloc(bufferSize-1);

	for (i = 0; i <= nP - 1; i++)
	{
		producers[i] = CreateThread(NULL, 0, Producer, NULL, i, NULL);
	}
	for (i = 0; i <= nC - 1; i++)
	{
		consumers[i] = CreateThread(NULL, 0, Consumer, NULL, i, NULL);
	}
	while (TRUE){
	}
	return 0;
}
