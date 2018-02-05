/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
*/ 

//Please enter your name and McGill ID below
//Name: <Saif Elkholy>
//McGill ID: <260606967>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main ()
{
    //Create new text file
    FILE* file_ptr = fopen("redirect_out.txt", "w");
    //getting fileDescriptor value for newly created txt file
    int fileDescriptor = open("redirect_out.txt", O_WRONLY);
    //making a copy of the stdout file descriptor
    int stdOutCopy = dup(1);

    printf("First : Print to stdout \n");

    int pipedFileDescriptors = dup2(fileDescriptor, 1);
    if (pipedFileDescriptors < 0)
        return 0;
    printf("Second : Print to redirect_out.txt \n");
    
    dup2(stdOutCopy, 1);
    printf ("Third : Print to stdout \n");
    
    close(fileDescriptor);
    return 0;
}