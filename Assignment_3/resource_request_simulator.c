/*
getUserInput() //get: 
- number of processes
- number of resourecs types
- number of each resource type
- maximum resource claim per process

Each process will be a thread:
- process i start
- while (true) request[i,j]:
    - i is a random number from 0->remaining amount of  resources for this process
    - j is the resource type
- boolean isSafe = BANKERS(&request, global resources in system)
- if isSafe{aquire resources and check for process termination}
- else process is blocked, continue the execution
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>
#include <stdbool.h>

pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

int PROCESSESS;
int RESOURCE_TYPES;
int *RESOURCE_TYPE_AVAILABILITIES;
int *PROCESS_REQUESTS;

int *HOLD;
int *AVAIL;
int *NEED;
int *REQ;

void getUserInput()
{
    printf("How many processess would you like to create?\n");
    scanf("%d", &PROCESSESS);

    printf("How many resource types would you like to create?\n");
    scanf("%d", &RESOURCE_TYPES);

    RESOURCE_TYPE_AVAILABILITIES = (int *)malloc(sizeof (int) * RESOURCE_TYPES);
    for (int i=0; i < RESOURCE_TYPES; i++){
        printf("How many resources for resource type: %d would you like to create?\n", i);
        scanf("%d", &*RESOURCE_TYPE_AVAILABILITIES + i);
    }

    // int PROCESS_REQUESTS[PROCESSESS][RESOURCE_TYPES];
    PROCESS_REQUESTS =(int *)malloc(sizeof (int) * PROCESSESS * RESOURCE_TYPES);
    for(int i=0; i< PROCESSESS; i++){
        for(int j=0; j< RESOURCE_TYPES; j++){
            printf("Specfiy how many resources of type %d, you would like process %d to use:\n",j, i);
            scanf("%d", &*(PROCESS_REQUESTS + i*RESOURCE_TYPES + j));
        }
    }
}
/*three possible states
    1- -1 if REQ > NEED, abort everything and quite all processess
    2- 0 not safe
    3- 1 safe
*/
int BankersAlgorithm(int process, int* requestVector)
{
    for(int i=0; i < RESOURCE_TYPES; i++){
        if (*(requestVector+i) > *(NEED+RESOURCE_TYPES*process + i)){
            return -1;
        } else if (*(requestVector+i) > *(AVAIL+i)){
            //busy waiting, step 2 of bankers algo
            while(*(requestVector+i) > *(AVAIL+i));
        } 
        printf("%d\n",*(requestVector+i));
    }
    return true;
}
void *fnProcess(void* pr_id)
{
    // Each process will be a thread:
    // - process i start
    // - while (true) request[i]:
    //     - i is a random number from 0->remaining amount of  resources for this process
    // - boolean isSafe = BANKERS(&request, global resources in system)
    // - if isSafe{aquire resources and check for process termination}
    // - else process is blocked, continue the execution
    int *processId = (int *) pr_id;
    // while (true){
        int* processRequest = (int *)malloc(sizeof(int) * RESOURCE_TYPES);
        for(int i=0; i < RESOURCE_TYPES; i++){
            int remainingAmountOfResourcesForThisProcessAndResourceType = *(NEED + (*processId)*RESOURCE_TYPES + i);
            printf("Process %d needs %d more resources of type %d\n", *processId, remainingAmountOfResourcesForThisProcessAndResourceType, i);
            int randomRequestForResourceI = rand() % remainingAmountOfResourcesForThisProcessAndResourceType;
            *(processRequest+i) = randomRequestForResourceI;
        }
        //here we have a fully populated request vector, offload to banker
        int isSafe = BankersAlgorithm(*processId, processRequest);
    // }
}



int main()
{
    getUserInput();
    
    AVAIL =(int *)malloc(sizeof (int) * RESOURCE_TYPES);
    for(int i=0; i< RESOURCE_TYPES; i++)
    {
        *(AVAIL+i) = *(RESOURCE_TYPE_AVAILABILITIES + i);
    }
    HOLD =(int *)malloc(sizeof (int) * PROCESSESS * RESOURCE_TYPES);
    NEED =(int *)malloc(sizeof (int) * PROCESSESS * RESOURCE_TYPES);
    for(int i=0; i<PROCESSESS; i++){
        for(int j=0; j< RESOURCE_TYPES; j++){
            *(HOLD + i*RESOURCE_TYPES + j) = 0;
            *(NEED + i*RESOURCE_TYPES + j) = *(PROCESS_REQUESTS + i*RESOURCE_TYPES + j);
        }
    }

    pthread_mutex_init(&MUTEX, NULL);
    pthread_t * processThreads = malloc(sizeof(pthread_t)*PROCESSESS);
    int *process_ids[PROCESSESS];

    for(int j =0; j < PROCESSESS; j++) {
        process_ids[j] = malloc(sizeof(int));
        *process_ids[j] = j;
        // Print an error if one occurs
        printf("Creating process thread %d\n", j);
        if (pthread_create(&processThreads[j], NULL, fnProcess, process_ids[j]) != 0 ){
            printf("Error with pthread create for process %i\n", j);
        }
    }
    for(int j =0; j < PROCESSESS; j++) {
        pthread_join(processThreads[j], NULL);
    }
    pthread_exit(NULL);

    return 0;
}