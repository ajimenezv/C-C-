#include<stdio.h>
#include<pthread.h>
#include<ctype.h>
#include <string.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>

int sender();
void reciver();

void catcherx()
{
    readAndPrint();
}
void catcherp(){}
void catcherc(){}
int addM()
{
        int x;
        FILE *ptrf;
        ptrf=fopen("output1_1.bin", "rb");
        fread(&x, sizeof(int), 1, ptrf);
        fclose(ptrf);
        x++;
        ptrf =fopen("output1_1.bin", "wb");
        fwrite(&x, sizeof(int), 1, ptrf);
        fclose(ptrf);

}
void printM()
{
    int x;
    FILE *ptrf;
    ptrf=fopen("output1_1.bin", "rb");
    fread(&x, sizeof(int), 1, ptrf);
    fclose(ptrf);
    printf("Number of messages: %d",x);
}
int initM()
{
    int x=0;
    FILE *ptrf;
    ptrf =fopen("output1_1.bin", "wb");
    fwrite(&x, sizeof(int), 1, ptrf);
    fclose(ptrf);

}
void saveChild(int id)
{

    FILE *ptrf;
    ptrf =fopen("out1_1_1.txt", "w");
    fprintf(ptrf,"%d\n", id);
    fclose(ptrf);
}

int getChild()
{
    int id;
    char buf[100];
    FILE *ptrf;
    ptrf =fopen("out1_1_1.txt", "r");
    fgets(buf,100, ptrf);
    id=atoi(buf);
    return id;
}


int main()
{
    initM();

    int id=fork();

    if(id!=0)
    {//parent
        int id2=fork();
        if(id2!=0)
        {
           saveChild(id2);
           signal(SIGUSR2,catcherc);

                sleep(1);
               printf("Parent begins\n");
               addM();
               kill(id,SIGUSR1);
               printf("Parent waits\n");
               pause();
               printM();

        }
        else
        {
            signal(SIGUSR1,catcherc);
            while(1)
            {
                pause();
                sleep(1);
                printf("Child 2 begins\n");
                reciver();
                addM();
                kill(id,SIGUSR2);

            }
        }
    }
    else
    {//Child
        signal(SIGUSR1,catcherc);
        signal(SIGUSR2,catcherx);
        while(1)
        {
            pause();
            sleep(1);
            addM();
            printf("Child 1 begins\n");
            if(sender()==0)
            {
                kill(getChild(),SIGUSR1);
            }
            else
            {
                kill(getppid(),SIGUSR2);
            }

        }
    }
    return 0;
}

int sender()

{
    printf("Write something\n");
    char buf[100];
    scanf("%s",buf);

    FILE *ptrf;
    ptrf =fopen("out1_1.txt", "w");
    fprintf(ptrf,"%s\n", buf);
    fclose(ptrf);
    if(strcmp(buf,"exit")==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

void reciver()
{
    int i=0;
    char buf[100];
    FILE *ptrf;
    ptrf =fopen("out1_1.txt", "r");
    fgets(buf,100, ptrf);

    while(buf[i])
    {
        buf[i]=toupper(buf[i]);
        i++;
    }

    fclose(ptrf);

    ptrf =fopen("out1_1.txt", "w");
    fprintf(ptrf,"%s\n", buf);
    fclose(ptrf);

}

void readAndPrint()
{
    char buf[100];
    FILE *ptrf;
    ptrf =fopen("out1_1.txt", "r");
    fgets(buf,100, ptrf);
    printf("The string is processed: %s",buf);
}
