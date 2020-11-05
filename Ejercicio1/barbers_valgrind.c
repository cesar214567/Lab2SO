#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define N_Barbers 10
#define N_Persons 1000
#define SEATS 5


sem_t barberos;  //free barbers
sem_t num_personas; //people left
sem_t waiting_list; //people waiting

pthread_mutex_t waiting;  // mutex to preserve waiting list semaphore coherency on multuple customers

pthread_t barbers[N_Barbers];
pthread_t persons[N_Persons];

void *barber(void* id){
    int ID = *(int*) id;
    while (sem_trywait(&num_personas) != -1) { /* will stay in loop until num_personas is empty */
        sem_post(&barberos); /* one barber is now ready to cut hair */
        sem_wait(&waiting_list); /* decreases customer being attended from waiting seats, else it waits for available customers */
  }
}
void *person(void* id){
    int ID = *(int*) id;
    pthread_mutex_lock(&waiting); // Locks waiting mutex to access waiting list as reader and writer
    int temp = 0;
    sem_getvalue(&waiting_list, &temp); //gets number of customers in waiting list at the moment
    if (temp < SEATS) { /* if there are no free chairs, leave */
        sem_post(&waiting_list); // Increases customers in waiting seats 
        pthread_mutex_unlock(&waiting); // Unlocks waiting lock for other threads to use waiting list
        sem_wait(&barberos); /* Decreases number of available barberos, or waits until there is one available */

    } else {
        sem_trywait(&num_personas); // Decreases total number of people
        pthread_mutex_unlock(&waiting); // Unlocks waiting lock for other threads to use waiting list
    }
}


int main(){
    pthread_mutex_init(&waiting, NULL);
    sem_init(&barberos,0,0);
    sem_init(&num_personas,0,N_Persons);
    sem_init(&waiting_list,0,0);
    
    int id_barbers[N_Barbers];
    int id_persons[N_Persons];

    int i;
    for(i=0; i<N_Barbers; i++){
        id_barbers[i] = i+1;
        pthread_create(&barbers[i],NULL,barber,&id_barbers[i]);
    }
    for(i=0;i<N_Persons;i++){
        id_persons[i] = i+1;
        pthread_create(&persons[i],NULL,person,&id_persons[i]);
    }
    for(i=0;i<N_Barbers;i++)
        pthread_join(barbers[i],NULL);

    for(i=0;i<N_Persons;i++)
        pthread_join(persons[i],NULL);

}
