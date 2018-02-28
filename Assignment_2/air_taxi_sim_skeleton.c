/*
 ----------------- COMP 310/ECSE 427 Winter 2018 -----------------
 Dimitri Gallos
 Assignment 2 skeleton
 
 -----------------------------------------------------------------
 I declare that the awesomeness below is a genuine piece of work
 and falls under the McGill code of conduct, to the best of my knowledge.
 -----------------------------------------------------------------
 */

//Please enter your name and McGill ID below
//Name: <Saif Elkholy>
//McGill ID: <260606967>

 

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>
//Added by me to have true defined by default
#include <stdbool.h>


int BUFFER_SIZE = 100; //size of queue
// int INT_MIN = 0;
sem_t FULL;  
sem_t EMPTY; 
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
};
 
// function to create a queue of given capacity. 
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}
 
// Queue is full when size becomes equal to the capacity 
int isFull(struct Queue* queue)
{
    return ((queue->size ) >= queue->capacity);
}
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}
 
// Function to add an item to the queue.  
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue. 
// It changes front and size
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return 10;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return 0;
    return queue->array[queue->front];
}
 
// Function to get rear of queue
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return 10;
    return queue->array[queue->rear];
}

void print(struct Queue* queue){
    if (queue->size == 0){
        return;
    }
    
    for (int i = queue->front; i < queue->front +queue->size; i++){
        
        printf(" Element at position %d is %d \n ", i % (queue->capacity ), queue->array[i % (queue->capacity)]);
    }
    
}

struct Queue* queue;

/*Producer Function: Simulates an Airplane arriving and dumping 5-10 passengers to the taxi platform */
void *FnAirplane(void* cl_id)
{
    int *planeId = (int *)cl_id;
    while( true ){
        printf("Airplane %d arrives\n", *planeId); 
        int randomNumberOfPassengers = (rand()%5 + 5);
        for(int i=0; i < randomNumberOfPassengers; i++){
            //forloop
            //add random number of passengers to queue, and print out their Ids
            //wait buffer mutex
            pthread_mutex_lock(&MUTEX);
            int passengerId = rear(queue) +1;
            enqueue(queue, passengerId);
            pthread_mutex_unlock(&MUTEX);
            printf("Passenger %d of airplane %d arrives to platform\n", passengerId, *planeId);
            if (isFull(queue)){
                printf("Platform is full: Rest of passengers of plane %d take the bus\n", *planeId);
                break;
            }  
        }
    }
}

/* Consumer Function: simulates a taxi that takes n time to take a passenger home and come back to the airport */
void *FnTaxi(void* pr_id)
{
    while(true){
        printf("Taxi driver %p arrives", pr_id);   
    }
}

int main(int argc, char *argv[])
{

  int num_airplanes;
  int num_taxis;

  num_airplanes=atoi(argv[1]);
  num_taxis=atoi(argv[2]);
  
  printf("You entered: %d airplanes per hour\n",num_airplanes);
  printf("You entered: %d taxis\n", num_taxis);
  
  
  //initialize queue
  queue = createQueue(BUFFER_SIZE);
  
  //declare arrays of threads and initialize semaphore(s)
  pthread_t taxiThreads[num_taxis];
  pthread_t airplaneThreads[num_airplanes];

  pthread_mutex_init(&MUTEX, NULL);

  //producer semaphore, will be initialised to the max buffer size
  // will be incremented by n each time a plane adds n people to buffer/queue
  //or will be decremented by n each time a taxi takes n people to buffer/queue
  sem_init(&FULL,0,BUFFER_SIZE); 

  //consumer semaphore, will be initialised to the 0
  // will be incremented by n each time a taxi takes n people to buffer/queue
  //or will be decremented by n each time a plane adds n people to buffer/queue
  sem_init(&EMPTY,0,0);

  //create arrays of integer pointers to ids for taxi / airplane threads
  int *taxi_ids[num_taxis];
  int *airplane_ids[num_airplanes];
    
  //create threads for airplanes
    for(int i=0; i < num_airplanes; i++){
        printf("Creating airplane thread %d\n", i);
        pthread_create(&airplaneThreads[i], NULL, FnAirplane, &i);
    }
  //create threads for taxis
    for(int j =0; j < num_taxis; j++) {

    }
  pthread_exit(NULL);
}
