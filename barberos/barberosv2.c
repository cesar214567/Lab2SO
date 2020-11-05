#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define N_Barbers 3
#define N_Persons 10
#define SEATS 5


sem_t customers; //numbers of waiters
sem_t barberos;  //free barbers
sem_t num_personas; //people left
sem_t waiting_list; //people waiting

//pthread_mutex_t lock_seats; //to block person actions 

pthread_t barbers[N_Barbers];
pthread_t persons[N_Persons];

void *barber(void* id){
    int ID = (int) id;
    while (sem_trywait(&num_personas) != -1) {
        sem_post(&barberos); /* one barber is now ready to cut hair */
        sem_wait(&customers); /* go to sleep if # of customers is 0 */
        sem_wait(&waiting_list);
        printf("Cortando cabello, barber id: %d\n", ID); /* cut hair (outside critical region */
  }
    printf("FIN DEL DIA\n");
}
void *person(void* id){
    int ID = (int) id;
    int temp = 0;
    sem_getvalue(&waiting_list, &temp);
    if (temp < SEATS) { /* if there are no free chairs, leave */
        sem_post(&waiting_list);
        sem_wait(&barberos); /* go to sleep if # of free barbers is 0 */
        sem_post(&customers); /* wake up barber if necessary */
        printf("Recibieno corte de cabello, comensal id: %d\n", ID); /* be seated and be served */

    } else {
        printf("No recibio corte de cabello, comensal id: %d", ID);
    }
}


int main(){
    //pthread_mutex_init(&lock_seats,NULL);
    sem_init(&customers,0,0);
    sem_init(&barberos,0,0);
    sem_init(&num_personas,0,N_Persons);
    sem_init(&waiting_list,0,0);
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
