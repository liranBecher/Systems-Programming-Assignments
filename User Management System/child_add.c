#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_USERNAME 30


int main (int argc, char *argv[]) {

    // check if the username and program name were entered correctly
    if (argc != 2) {
        printf("Invalid command argument.\n");
        exit(1);
    }

    char username_to_check[MAX_USERNAME];
    char username_read[MAX_USERNAME]; // string to store the username read in the file
    const char filename[] = "users.txt";
    int found;

    strcpy(username_to_check, argv[1]);

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Failed to open file '%s' \n", filename);
        return 0;

    }

    fprintf(file,"%s\n", username_to_check);
    fclose(file);

    printf("Username added succesfully\n");

    return 0;

}
