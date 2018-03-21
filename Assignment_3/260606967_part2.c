#include <stdio.h>  //for printf and scanf
#include <stdlib.h> //for malloc

#define LOW 0
#define HIGH 199
#define START 53

//compare function for qsort
//you might have to sort the request array
//use the qsort function 
// an argument to qsort function is a function that compares 2 quantities
//use this there.
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

//function to swap 2 integers
void swap(int *a, int *b)
{
    if (*a != *b)
    {
        *a = (*a ^ *b);
        *b = (*a ^ *b);
        *a = (*a ^ *b);
        return;
    }
}

//Prints the sequence and the performance metric
void printSeqNPerformance(int *request, int numRequest)
{
    int i, last, acc = 0;
    last = START;
    printf("\n");
    printf("%d", START);
    for (i = 0; i < numRequest; i++)
    {
        printf(" -> %d", request[i]);
        acc += abs(last - request[i]);
        last = request[i];
    }
    printf("\nPerformance : %d\n", acc);
    return;
}

//access the disk location in FCFS
void accessFCFS(int *request, int numRequest)
{
    //simplest part of assignment
    printf("\n----------------\n");
    printf("FCFS :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SSTF
void accessSSTF(int *request, int numRequest)
{
    //write your logic here
    //want to make a new array of [START, ...request] to simiplify the alogrithm below
    int *newRequest = malloc((numRequest+1) * sizeof(int));
    newRequest[0] = START;
    
    for(int i=0; i< numRequest; i++){
        newRequest[i+1] = request[i];
    }


    for(int i = 0; i <numRequest; i++){
        int indexOfClosestRequest = 0;
        int closestIndexDistance = HIGH-LOW;
        for(int j=i+1; j< numRequest+1; j++){
            //we have found a closest-request pair
            if(abs(newRequest[i] - newRequest[j]) < closestIndexDistance){
                indexOfClosestRequest = j;
                closestIndexDistance = abs(newRequest[i] - newRequest[j]);
            }
        }
        //swap the closest index with the i+1 index
        int temp = newRequest[indexOfClosestRequest];
        newRequest[indexOfClosestRequest] = newRequest[i+1];
        newRequest[i+1] = temp;
    }

    for(int i=0; i< numRequest; i++){
        request[i]=newRequest[i+1] ;
    }

    printf("\n----------------\n");
    printf("SSTF :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

// https://www.geeksforgeeks.org/write-a-program-to-reverse-an-array-or-string/
// Method below taken from the url given
void revreseArray(int arr[], int start, int end)
{
    int temp;
    while (start < end)
    {
        temp = arr[start];   
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }   
}     

//access the disk location in SCAN
void accessSCAN(int *request, int numRequest)
{
    
	//write your logic here
    int requestsLargerThanStartingPoint, requestsSmallerThanStartingPoint;
    requestsLargerThanStartingPoint = 0;
    requestsSmallerThanStartingPoint = 0;

    for(int i=0; i< numRequest; i++){
        if(request[i] > START || request[i] == START ){
            requestsLargerThanStartingPoint++;
        } else 
            requestsSmallerThanStartingPoint++;
    }
    int *requestsLarger = malloc((requestsLargerThanStartingPoint) * sizeof(int));
    int *requestsSmaller = malloc((requestsSmallerThanStartingPoint) * sizeof(int));
    
    int largerIndex = 0;
    int smallerIndex = 0;

    for(int i=0; i< numRequest; i++){
        if(request[i] > START || request[i] == START ){
            requestsLarger[largerIndex] = request[i];
            largerIndex++;
        } else{ 
            requestsSmaller[smallerIndex] = request[i];
            smallerIndex++;
        }
    }

    qsort(requestsLarger, requestsLargerThanStartingPoint, sizeof(int), cmpfunc);
    qsort(requestsSmaller, requestsSmallerThanStartingPoint, sizeof(int), cmpfunc);

    /*
    there are 3 scenarios
    1- we have requests larger than and less than the starting point 
    2- We have requests only larger than the starting point
    3 We have requests only smaller than the starting point

    For 1,3 we will hit an extremity at location HIGH, so build the array accordingly    
    */
    int sizeOfNewRequest=0;
    int *newRequest;
    revreseArray(requestsSmaller, 0, requestsSmallerThanStartingPoint-1);
        
    if(requestsLargerThanStartingPoint > 0 && requestsSmallerThanStartingPoint > 0){
        
        sizeOfNewRequest = numRequest + 1;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        for (int i =0; i< requestsLargerThanStartingPoint; i++){
            newRequest[i] = requestsLarger[i];
        }
        newRequest[requestsLargerThanStartingPoint] = HIGH;

        smallerIndex= 0;
        for (int i =requestsLargerThanStartingPoint + 1; i<sizeOfNewRequest; i++){
            printf("%d",requestsSmaller[smallerIndex]);
            newRequest[i] = requestsSmaller[smallerIndex];
            smallerIndex++;
        }

    } else if (requestsLargerThanStartingPoint > 0 && requestsSmallerThanStartingPoint == 0){
        sizeOfNewRequest = numRequest;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        for (int i =0; i< sizeOfNewRequest; i++){
            newRequest[i] = requestsLarger[i];
        }
    } else if (requestsLargerThanStartingPoint == 0 && requestsSmallerThanStartingPoint > 0){
        sizeOfNewRequest = numRequest + 1;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        newRequest[0] = HIGH;
        for(int i=1; i< sizeOfNewRequest; i++){
            newRequest[i] = requestsSmaller[i-1];
        }
    }

    printf("\n----------------\n");
    printf("SCAN :");
    printSeqNPerformance(newRequest, sizeOfNewRequest);
    printf("----------------\n");
    return;
}

//access the disk location in CSCAN
void accessCSCAN(int *request, int numRequest)
{
    //write your logic here
    int requestsLargerThanStartingPoint, requestsSmallerThanStartingPoint;
    requestsLargerThanStartingPoint = 0;
    requestsSmallerThanStartingPoint = 0;

    for(int i=0; i< numRequest; i++){
        if(request[i] > START || request[i] == START ){
            requestsLargerThanStartingPoint++;
        } else 
            requestsSmallerThanStartingPoint++;
    }
    int *requestsLarger = malloc((requestsLargerThanStartingPoint) * sizeof(int));
    int *requestsSmaller = malloc((requestsSmallerThanStartingPoint) * sizeof(int));
    
    int largerIndex = 0;
    int smallerIndex = 0;

    for(int i=0; i< numRequest; i++){
        if(request[i] > START || request[i] == START ){
            requestsLarger[largerIndex] = request[i];
            largerIndex++;
        } else{ 
            requestsSmaller[smallerIndex] = request[i];
            smallerIndex++;
        }
    }

    qsort(requestsLarger, requestsLargerThanStartingPoint, sizeof(int), cmpfunc);
    qsort(requestsSmaller, requestsSmallerThanStartingPoint, sizeof(int), cmpfunc);

    /*
    there are 3 scenarios
    1- we have requests larger than and less than the starting point 
    2- We have requests only larger than the starting point
    3 We have requests only smaller than the starting point

    For 1,3 we will hit an extremity at location HIGH and LOW so build the array accordingly    
    */
    int sizeOfNewRequest=0;
    int *newRequest;
        
    if(requestsLargerThanStartingPoint > 0 && requestsSmallerThanStartingPoint > 0){
        
        sizeOfNewRequest = numRequest + 2;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        for (int i =0; i< requestsLargerThanStartingPoint; i++){
            newRequest[i] = requestsLarger[i];
        }
        newRequest[requestsLargerThanStartingPoint] = HIGH;
        newRequest[requestsLargerThanStartingPoint+1] = LOW;
        
        smallerIndex= 0;
        for (int i =requestsLargerThanStartingPoint + 2; i<sizeOfNewRequest; i++){
            printf("%d",requestsSmaller[smallerIndex]);
            newRequest[i] = requestsSmaller[smallerIndex];
            smallerIndex++;
        }

    } else if (requestsLargerThanStartingPoint > 0 && requestsSmallerThanStartingPoint == 0){
        sizeOfNewRequest = numRequest;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        for (int i =0; i< sizeOfNewRequest; i++){
            newRequest[i] = requestsLarger[i];
        }
    } else if (requestsLargerThanStartingPoint == 0 && requestsSmallerThanStartingPoint > 0){
        sizeOfNewRequest = numRequest + 2;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        newRequest[0] = HIGH;
        newRequest[1] = LOW;
        for(int i=2; i< sizeOfNewRequest; i++){
            newRequest[i] = requestsSmaller[i-2];
        }
    }
    printf("\n----------------\n");
    printf("CSCAN :");
    printSeqNPerformance(newRequest, sizeOfNewRequest);
    printf("----------------\n");
    return;
}

//access the disk location in LOOK
void accessLOOK(int *request, int numRequest)
{
        //write your logic here
    int requestsLargerThanStartingPoint, requestsSmallerThanStartingPoint;
    requestsLargerThanStartingPoint = 0;
    requestsSmallerThanStartingPoint = 0;

    for(int i=0; i< numRequest; i++){
        if(request[i] > START || request[i] == START ){
            requestsLargerThanStartingPoint++;
        } else 
            requestsSmallerThanStartingPoint++;
    }
    int *requestsLarger = malloc((requestsLargerThanStartingPoint) * sizeof(int));
    int *requestsSmaller = malloc((requestsSmallerThanStartingPoint) * sizeof(int));
    
    int largerIndex = 0;
    int smallerIndex = 0;

    for(int i=0; i< numRequest; i++){
        if(request[i] > START || request[i] == START ){
            requestsLarger[largerIndex] = request[i];
            largerIndex++;
        } else{ 
            requestsSmaller[smallerIndex] = request[i];
            smallerIndex++;
        }
    }

    qsort(requestsLarger, requestsLargerThanStartingPoint, sizeof(int), cmpfunc);
    qsort(requestsSmaller, requestsSmallerThanStartingPoint, sizeof(int), cmpfunc);

    /*
    there are 3 scenarios
    1- we have requests larger than and less than the starting point 
    2- We have requests only larger than the starting point
    3 We have requests only smaller than the starting point

    For 1,3 we will hit an extremity at location HIGH and LOW so build the array accordingly    
    */
    int sizeOfNewRequest=0;
    int *newRequest;
    revreseArray(requestsSmaller, 0, requestsSmallerThanStartingPoint-1);
    
    if(requestsLargerThanStartingPoint > 0 && requestsSmallerThanStartingPoint > 0){
        
        sizeOfNewRequest = numRequest;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        for (int i =0; i< requestsLargerThanStartingPoint; i++){
            newRequest[i] = requestsLarger[i];
        }
        smallerIndex= 0;
        for (int i =requestsLargerThanStartingPoint; i<sizeOfNewRequest; i++){
            printf("%d",requestsSmaller[smallerIndex]);
            newRequest[i] = requestsSmaller[smallerIndex];
            smallerIndex++;
        }

    } else if (requestsLargerThanStartingPoint > 0 && requestsSmallerThanStartingPoint == 0){
        sizeOfNewRequest = numRequest;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        for (int i =0; i< sizeOfNewRequest; i++){
            newRequest[i] = requestsLarger[i];
        }
    } else if (requestsLargerThanStartingPoint == 0 && requestsSmallerThanStartingPoint > 0){
        sizeOfNewRequest = numRequest;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        newRequest[0] = HIGH;
        newRequest[1] = LOW;
        for(int i=0; i< sizeOfNewRequest; i++){
            newRequest[i] = requestsSmaller[i];
        }
    }
    printf("\n----------------\n");
    printf("LOOK :");
    printSeqNPerformance(newRequest, sizeOfNewRequest);
    printf("----------------\n");
    return;
}

//access the disk location in CLOOK
void accessCLOOK(int *request, int numRequest)
{
    //write your logic here
    int requestsLargerThanStartingPoint, requestsSmallerThanStartingPoint;
    requestsLargerThanStartingPoint = 0;
    requestsSmallerThanStartingPoint = 0;

    for(int i=0; i< numRequest; i++){
        if(request[i] > START || request[i] == START ){
            requestsLargerThanStartingPoint++;
        } else 
            requestsSmallerThanStartingPoint++;
    }
    int *requestsLarger = malloc((requestsLargerThanStartingPoint) * sizeof(int));
    int *requestsSmaller = malloc((requestsSmallerThanStartingPoint) * sizeof(int));
    
    int largerIndex = 0;
    int smallerIndex = 0;

    for(int i=0; i< numRequest; i++){
        if(request[i] > START || request[i] == START ){
            requestsLarger[largerIndex] = request[i];
            largerIndex++;
        } else{ 
            requestsSmaller[smallerIndex] = request[i];
            smallerIndex++;
        }
    }

    qsort(requestsLarger, requestsLargerThanStartingPoint, sizeof(int), cmpfunc);
    qsort(requestsSmaller, requestsSmallerThanStartingPoint, sizeof(int), cmpfunc);

    /*
    there are 3 scenarios
    1- we have requests larger than and less than the starting point 
    2- We have requests only larger than the starting point
    3 We have requests only smaller than the starting point

    For 1,3 we will hit an extremity at location HIGH and LOW so build the array accordingly    
    */
    int sizeOfNewRequest=0;
    int *newRequest;
        
    if(requestsLargerThanStartingPoint > 0 && requestsSmallerThanStartingPoint > 0){
        
        sizeOfNewRequest = numRequest + 1;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        for (int i =0; i< requestsLargerThanStartingPoint; i++){
            newRequest[i] = requestsLarger[i];
        }
        newRequest[requestsLargerThanStartingPoint] = LOW;
        
        smallerIndex= 0;
        for (int i =requestsLargerThanStartingPoint + 1; i<sizeOfNewRequest; i++){
            printf("%d",requestsSmaller[smallerIndex]);
            newRequest[i] = requestsSmaller[smallerIndex];
            smallerIndex++;
        }

    } else if (requestsLargerThanStartingPoint > 0 && requestsSmallerThanStartingPoint == 0){
        sizeOfNewRequest = numRequest;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        for (int i =0; i< sizeOfNewRequest; i++){
            newRequest[i] = requestsLarger[i];
        }
    } else if (requestsLargerThanStartingPoint == 0 && requestsSmallerThanStartingPoint > 0){
        sizeOfNewRequest = numRequest + 1;
        newRequest = malloc((sizeOfNewRequest) * sizeof(int));
        newRequest[0] = LOW;
        for(int i=1; i< sizeOfNewRequest; i++){
            newRequest[i] = requestsSmaller[i-1];
        }
    }
    printf("\n----------------\n");
    printf("CLOOK :");
    printSeqNPerformance(newRequest,sizeOfNewRequest);
    printf("----------------\n");
    return;
}

int main()
{
    int *request, numRequest, i,ans;

    //allocate memory to store requests
    printf("Enter the number of disk access requests : ");
    scanf("%d", &numRequest);
    request = malloc(numRequest * sizeof(int));

    printf("Enter the requests ranging between %d and %d\n", LOW, HIGH);
    for (i = 0; i < numRequest; i++)
    {
        scanf("%d", &request[i]);
    }

    printf("\nSelect the policy : \n");
    printf("----------------\n");
    printf("1\t FCFS\n");
    printf("2\t SSTF\n");
    printf("3\t SCAN\n");
    printf("4\t CSCAN\n");
    printf("5\t LOOK\n");
    printf("6\t CLOOK\n");
    printf("----------------\n");
    scanf("%d",&ans);

    switch (ans)
    {
    //access the disk location in FCFS
    case 1: accessFCFS(request, numRequest);
        break;

    //access the disk location in SSTF
    case 2: accessSSTF(request, numRequest);
        break;

        //access the disk location in SCAN
     case 3: accessSCAN(request, numRequest);
        break;

        //access the disk location in CSCAN
    case 4: accessCSCAN(request,numRequest);
        break;

    //access the disk location in LOOK
    case 5: accessLOOK(request,numRequest);
        break;

    //access the disk location in CLOOK
    case 6: accessCLOOK(request,numRequest);
        break;

    default:
        break;
    }
    return 0;
}