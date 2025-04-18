#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int my_pipe[2];
    int status;
    int number;
    int counter = 0;
    int sum = 0;
    double avg = 0;

    // create pipe
    if (pipe(my_pipe) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    // create child process
    status = fork();
    if (status == -1) {
        perror("Fork failed");
        exit(1);
    }

    if (status > 0) {  // producer
        close(my_pipe[0]); // close the read descriptor

        // loop for prompting the user for input
        while (1) {
            usleep(10000); // delay so the child can print the results before prompting the user to enter another number
            printf("Enter a number: ");
            fflush(stdout); // clear output buffer

            if (scanf("%d", &number) == 1) {
                write(my_pipe[1], &number, sizeof(int));
            } else {
                printf("Invalid input. Try again.\n");
                while (getchar() != '\n'); // clear input buffer
            }
        }
    } else {  // consumer
        close(my_pipe[1]); // close write descriptor

        // loop for the consumer to calculate after each input
        while (read(my_pipe[0], &number, sizeof(int)) > 0) {
            counter++;
            sum += number;
            avg = (double)sum/counter;
            printf("Received: %d | Sum = %d | Average = %.2f\n", number, sum, avg);
            fflush(stdout); // clear output buffer
        }

        close(my_pipe[0]); // close read descriptor
        exit(0);
    }

    return 0;
}
