#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_USERNAME 30

// function to create a new child process
static pid_t create_process(void)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Failed to fork\n");
        exit(1);
    }
    return pid;
}

int main(int argc, char *argv[]) { // argc = num of arguments (including program name), argv = the argument itself

    // check if the username and program name were entered correctly
    if (argc != 2) {
        printf("Invalid command argument.\n");
        exit(1);
    }

    char username_read[MAX_USERNAME]; // string to store the username read in the file
    const char filename[] = "users.txt";


    pid_t pid = create_process();

    if (pid == 0) { // if this is the child process
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            fprintf(stderr, "Failed to open file '%s' \n", filename);
            exit(2);
        }

        // loop to read through all usernames and compare them to the input username
        while (fscanf(file, "%s", username_read) == 1) {
            if (strcmp(argv[1], username_read) == 0) {
                fclose(file);
                exit(0); // if the username found, return 0 to parent process
            }
        }
        fclose(file);
        exit(1); // if the username not found, return 1 to parent process
    }

    else { // if this is the parent process
        int status; // the exit status of the child process will be stored here
        wait(&status); // wait for the child process to end and then store it's exit status

        if(WEXITSTATUS(status) == 0) { // if the exit status is 0
            printf("User exists in the system.\n");
        }
        else if (WEXITSTATUS(status) == 1) { // if the exit status is 1
            printf("User doesn't exist in the system.\n");
        }


    }

    return 0;

}

