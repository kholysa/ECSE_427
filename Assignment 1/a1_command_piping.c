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
int main ()
{
    int fd[2];
    if (pipe(fd))
        return 0;
    
    int forkValue = fork();
    if (forkValue ==0 )
    {
        // Child : execute ls using execvp

        //close read end of pipe
        // close(fd[0]);

        //close stdout so child doesn't write to it
        close(1);

        //place write end of pipe in 1 (where a stdout would've been sent to)
        dup(fd[1]);
        
        char *cmd = "ls";
        char *argv[2];
        argv[0] = "ls";
        argv[1] = NULL;
        execvp(cmd, argv);
    }
    else
    {
        //parent process
        
        //variable to store output of pipe
        char outputFromLs[128];

        //read from the "read-end" of the pipe
        read(fd[0], outputFromLs, 128);
        printf("%s", outputFromLs);
    }
    return 0 ;
}