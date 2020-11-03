
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define N_Barbers 3
#define N_Persons 10
#define SEATS 5


int waiting_list = 0;
int num_personas = N_Persons; // missing people
sem_t customers; //numbers of waiters
sem_t barberos;  // free barbers

pthread_mutex_t lock_seats; //to block 
pthread_mutex_t personas;

pthread_t barbers[N_Barbers];
pthread_t persons[N_Persons];

void *barber(void* id){
    int ID = (int) id; 
    while (num_personas) {
        sem_wait(&customers); /* go to sleep if # of customers is 0 */
        pthread_mutex_lock(&lock_seats); /* acquire access to "waiting' */
        waiting_list--; /* decrement count of waiting customers */
        sem_post(&barberos); /* one barber is now ready to cut hair */
        pthread_mutex_unlock(&lock_seats); /* release 'waiting' */
        printf("Cortando cabello, barber id: %d\n", ID); /* cut hair (outside critical region */
    }
    printf("FIN DEL DIA\n");
}
void *person(void* id){
    int ID = (int) id;
    pthread_mutex_lock(&lock_seats);
    if (waiting_list < SEATS) { /* if there are no free chairs, leave */
        waiting_list++; /* increment count of waiting customers */
        sem_post(&customers); /* wake up barber if necessary */
        pthread_mutex_unlock(&lock_seats); /* release access to 'waiting' */
        sem_wait(&barberos); /* go to sleep if # of free barbers is 0 */
        printf("Recibieno corte de cabello, comensal id: %d\n", ID); /* be seated and be served */
        pthread_mutex_lock(&personas);
        num_personas--;
        pthread_mutex_unlock(&personas);

    } else {
        num_personas--;
        printf("No recibio corte de cabello, comensal id: %d", ID);
        pthread_mutex_lock(&lock_seats);/* shop is full; do not wait */
    }
    //MUTEX_LOCK
    if (num_personas == 0){
        for(int i=0;i<N_Barbers;i++){
            sem_post(&customers);
        }
    }
    
}


int main(){
    pthread_mutex_init(&lock_seats,NULL);
    pthread_mutex_init(&personas,NULL);
    sem_init(&customers,1,0);
    sem_init(&barberos,1,0);
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
