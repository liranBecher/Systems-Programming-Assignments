#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "a_fifo"

int main()
{
    FILE *fdr;
    int number;
    int counter = 0, sum = 0;
    double avg = 0.0;

    // create fifo
    if (mkfifo(FIFO_NAME, 0666) == -1 && errno != EEXIST) {
        perror("Cannot create FIFO file");
        exit(EXIT_FAILURE);
    }

    // open fifo for reading
    if (!(fdr = fopen(FIFO_NAME, "r"))) {
        perror("Cannot open FIFO file for reading");
        exit(EXIT_FAILURE);
    }

    // read the fifo and calculate based on the integers in the file
    while (fscanf(fdr, "%d", &number) != EOF) {
        counter++;
        sum += number;
        avg = (double)sum / counter;

        printf("Received: %d | Sum = %d | Average = %.2f\n", number, sum, avg);
        fflush(stdout); // clear buffer
    }

    // close the fifo
    fclose(fdr);
    return EXIT_SUCCESS;
}
