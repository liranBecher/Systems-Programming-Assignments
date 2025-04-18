#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_USERNAME 30

// Function to create a new child process
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

int main(int argc, char *argv[])
{
    // Check if the username and program name were entered correctly
    if (argc != 2)
    {
        printf("Invalid command argument.\n");
        exit(1);
    }

    char username_read[MAX_USERNAME]; // String to store the username read in the file
    const char filename[] = "users.txt";
    int found = 0; // Initialize `found` to 0

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open file '%s'\n", filename);
        return 0;
    }

    // Loop to read through all usernames and compare them to the input username
    while (fscanf(file, "%s", username_read) == 1)
    {
        if (strcmp(argv[1], username_read) == 0)
        {
            found = 1;
            break; // Exit the loop once a match is found
        }
    }
    fclose(file);

    if (found == 0) {

        printf("User doesn't exist in the system.\n");

        // Create a child process
        pid_t child = create_process();

        if (child == 0)
        {
            // Replace the child process with "program2"
            char *args[] = { "child_add", argv[1], NULL }; // Pass argv[1] to the program
            if (execv("child_add", args) != 0)
            {
                perror("execv() failed");
                exit(EXIT_FAILURE);
            }
        }

        else {
            int status;
            waitpid(child, &status,0);
        }
    }

    else
    {
        printf("User exists in the system.\n");
    }

    return 0;
}
