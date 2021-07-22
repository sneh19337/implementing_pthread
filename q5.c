#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/shm.h>
#include<sys/ipc.h>  
#include<stdint.h>
#define num 5
#define nu 1
#define no 2
    sem_t mutex,writes;
    int data=0;
    int rcount=0;
 void *writer(void *arg)
{
    int f;
    f=((int)(intptr_t)arg);
    sem_wait(&writes);
    data++;
    printf("The data written by the writer %d is %d \n",f,data);
    sem_post(&writes);

}
void *reader(void *arg);
    int main()
    {
        int i;
        sem_init(&writes,0,nu);
        pthread_t rtid[num],wtid[num];
        sem_init(&mutex,0,nu);
        key_t  key = 15;  
        int    shmid_1;  
        if ((shmid_1=shmget(key, 1000, 0640|IPC_CREAT)) == -1){  
        return 1;
         } 
               
        i=0;
        while(i<nu)
        {
		pthread_create(&wtid[i],NULL,writer,(void *)(intptr_t)i);
		i++;
        }
        for(i=0;i<=no;i++)
        {
		pthread_create(&rtid[i],NULL,reader,(void *)(intptr_t)i);
		}
        pthread_join(wtid[0],NULL);
        for(i=0;i<=no;i++)
        {
		pthread_join(rtid[i],NULL);
		}

    }
       void *reader(void *arg)
{
    int f;
    int y=nu-1;
    f=((int)(intptr_t)arg);

    sem_wait(&mutex);

    rcount=rcount+nu;
    if(rcount==nu)
        sem_wait(&writes);

    sem_post(&mutex);

    printf("The data read by the reader %d is %d \n",f,data);

    sem_wait(&mutex);

    rcount=rcount-nu;

    if(rcount==y)
        sem_post(&writes);

    sem_post(&mutex);
}