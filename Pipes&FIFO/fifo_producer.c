#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define FIFO_NAME "a_fifo"

int main()
{
	 FILE *fdw;
	 int number;
	 // create fifo
	 if (mkfifo(FIFO_NAME, S_IFIFO | 0644) == -1 && errno != EEXIST) {
		 perror("cannot create fifo file") ;
		 exit(EXIT_FAILURE) ;
	 }
	 //open fifo file for writing
	 if (!(fdw = fopen(FIFO_NAME, "w"))) {
		 perror("cannot open fifo file for w") ;
		 exit(EXIT_FAILURE) ;
	 }

     // loop for prompting the user to enter input
	 while (1) {
        printf("Enter a number: ");
        fflush(stdout); // clear buffer output

        if (scanf("%d", &number) == 1) {
            fprintf(fdw, "%d\n", number); // write the number given by the user into the fifo file
            fflush(fdw); // clear writing buffer
        } else { // if the user enter invalid input
            printf("Invalid input. Try again.\n");
            while (getchar() != '\n'); // Clear input buffer
        }
    }

 return EXIT_SUCCESS ;
}
