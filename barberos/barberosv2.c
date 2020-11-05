#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define N_Barbers 3
#define N_Persons 100
#define SEATS 5


int waiting_list = 0;
sem_t customers; //numbers of waiters
sem_t barberos;  //free barbers
sem_t num_personas; //people left

pthread_mutex_t lock_seats; //to block person actions 
pthread_mutex_t personas;   //to block decreasing num_personas counter

pthread_t barbers[N_Barbers];
pthread_t persons[N_Persons];

void *barber(void* id){
    int ID = (int) id;
    while (sem_trywait(&num_personas) != -1) {
        sem_post(&barberos); /* one barber is now ready to cut hair */
        sem_wait(&customers); /* go to sleep if # of customers is 0 */
        pthread_mutex_lock(&lock_seats); /* acquire access to "waiting' */
        waiting_list--; /* decrement count of waiting customers */
        pthread_mutex_unlock(&lock_seats); /* release 'waiting' */
        printf("Cortando cabello, barber id: %d\n", ID); /* cut hair (outside critical region */
  }
    printf("FIN DEL DIA %d\n", temp);
}
void *person(void* id){
    int ID = (int) id;
    pthread_mutex_lock(&lock_seats);
    if (waiting_list < SEATS) { /* if there are no free chairs, leave */
        waiting_list++; /* increment count of waiting customers */
        pthread_mutex_unlock(&lock_seats); /* release access to 'waiting' */
        sem_wait(&barberos); /* go to sleep if # of free barbers is 0 */
        sem_post(&customers); /* wake up barber if necessary */
        printf("Recibieno corte de cabello, comensal id: %d\n", ID); /* be seated and be served */

    } else {
        printf("No recibio corte de cabello, comensal id: %d", ID);
        pthread_mutex_unlock(&lock_seats);/* shop is full; do not wait */
    }
}


int main(){
    pthread_mutex_init(&lock_seats,NULL);
    pthread_mutex_init(&personas,NULL);
    sem_init(&customers,0,0);
    sem_init(&barberos,0,0);
    sem_init(&num_personas,0,N_Persons);
    int i;
    for(i=0;i<N_Barbers;i++){
        pthread_create(&barbers[i],NULL,barber,i);
    }
    for(i=0;i<N_Persons;i++){
        printf("iteracion: %d\n",i);
        pthread_create(&persons[i],NULL,person,i);
    }
    
    for(i=0;i<N_Barbers;i++){
        pthread_join(barbers[i],NULL);
    }
    for(i=0;i<N_Persons;i++){
        pthread_join(persons[i],NULL);
    }


}
