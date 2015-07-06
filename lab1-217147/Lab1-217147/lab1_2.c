//LAB 1 2

#include<stdio.h>
#include<pthread.h>
#include<ctype.h>
#include <string.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>
char buf[100];
sem_t senderS;
sem_t reciverS;
sem_t returnS;

void* sender()
{
    while(1)
    {
        sem_wait(&senderS);//1->0
        printf("Sender begins\n");
        printf("Write something: \n");
        scanf("%s",buf);

        if(strcmp(buf,"end")==0)
        {
            addM();
            sem_post(&reciverS);//0-->1
            pthread_exit(0);
        }
        else
        {
            sem_post(&reciverS);//0-->1
            sem_wait(&returnS);//1-->0
            addM();
            printf("The string is processed: %s\n",buf);

        }


    }
}
void* reciver()
{
    while(1)
    {

        sem_wait(&reciverS);//1-->0
        printf("Reciver begins\n");
        if(strcmp(buf,"end")==0)
        {
            pthread_exit(0);
        }
        int i=0;
        while(buf[i])
        {
            buf[i]=toupper(buf[i]);
            i++;
        }

        sem_post(&senderS);//0-->1
        sem_post(&returnS);
        addM();
        addM();
    }
}

int addM()
{
        int x;
        FILE *ptrf;
        ptrf=fopen("output1_2.bin", "rb");
        fread(&x, sizeof(int), 1, ptrf);
        fclose(ptrf);
        x++;
        ptrf =fopen("output1_2.bin", "wb");
        fwrite(&x, sizeof(int), 1, ptrf);
        fclose(ptrf);

}
void printM()
{
    int x;
    FILE *ptrf;
    ptrf=fopen("output1_2.bin", "rb");
    fread(&x, sizeof(int), 1, ptrf);
    fclose(ptrf);
    printf("Number of messages: %d \n",x);
}
int initM()
{
    int x=0;
    FILE *ptrf;
    ptrf =fopen("output1_2.bin", "wb");
    fwrite(&x, sizeof(int), 1, ptrf);
    fclose(ptrf);

}

int main()
{
    initM();

    pthread_t th1,th2;

    sem_init(&senderS,1,1);
    sem_init(&reciverS,1,0);
    sem_init(&returnS,1,0);

    pthread_create(&(th1), NULL, &sender, NULL);
    pthread_create(&(th2), NULL, &reciver, NULL);


    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printM();

    sem_destroy(&senderS);
    sem_destroy(&reciverS);
    sem_destroy(&returnS);

    return 0;
}
