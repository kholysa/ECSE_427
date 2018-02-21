#include<stdio.h> 
#include<unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {

FILE* file_ptr = fopen("redirect_out.txt", "w");

printf("First : Print to stdout\n");

int fileHandleNumber = open("redirect_out.txt", O_WRONLY);

int defOutCopy = dup(1);

close (1);

dup2(fileHandleNumber, 1);

printf ("Second : Print to redirect_out.txt\n");

close(1);

close (fileHandleNumber);

dup2(defOutCopy, 1);
    
close(defOutCopy);

printf ("Third : Print to stdout\n");

return 0; 
}