#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>
#include <stdlib.h>

// word counts
int globalCountThe = 0, globalCountWas = 0, globalCountIs = 0, globalCountThis = 0, globalCountAre = 0;

// locks for multithreading in counters and in the file
pthread_mutex_t counterMutex;
pthread_mutex_t fileMutex;

// function to divide the text to separate blocks for each thread
char** divideText(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return NULL;
    }

    // count lines
    int total_lines = 0;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        total_lines++;
    }

    // divide lines by 4
    int lines_per_block = total_lines / 4;
    int remaining_lines = total_lines % 4;

    // memory allocation
    char** blocks = (char**)malloc(4 * sizeof(char*));
    if (blocks == NULL) {
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < 4; i++) {
        int block_lines = lines_per_block;
        if (i == 3)
            block_lines += remaining_lines; // remaining lines are added to the last block
        blocks[i] = (char*)malloc(block_lines * 100 * sizeof(char));
        if (blocks[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(blocks[j]);
            }
            free(blocks);
            fclose(file);
            return NULL;
        }
        blocks[i][0] = '\0';
    }

    rewind(file); //return to the start of the file in order to divide

    // divide the file into blocks
    int current_line = 0;
    int current_block = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL && current_block < 4) {
        // determine which block this line should go into
        current_block = current_line / lines_per_block;
        if (current_block >= 4)
            current_block = 3;  // put remaining lines in last block

        // add to appropriate block
        strcat(blocks[current_block], buffer);
        current_line++;
    }

    fclose(file);
    return blocks;
}

void freeBlocks(char** blocks) {
    if (blocks == NULL) return;
    for (int i = 0; i < 4; i++) {
        free(blocks[i]);
    }
    free(blocks);
}

// counting of words in a block by each thread
void *countWords(void *arg) {
    char *block = (char *)arg;
    char buffer[100];
    int i = 0;
    char c;

    // put the chars into a buffer
    for (int j = 0; block[j] != '\0'; j++) {
        c = block[j];
       // check if the next char is a letter to indicate end of word
        if (isalpha(c)) {
            buffer[i++] = tolower(c);
        } else {  // if the next char isn't a letter, put '\0' at the end of the buffer to indicate end of string
            if (i > 0) {
                buffer[i] = '\0';
                i = 0;

                // lock the mutex before modifying the counters
                pthread_mutex_lock(&counterMutex);

                if (strcmp(buffer, "the") == 0) {
                    globalCountThe++;
                } else if (strcmp(buffer, "was") == 0) {
                    globalCountWas++;
                } else if (strcmp(buffer, "is") == 0) {
                    globalCountIs++;
                } else if (strcmp(buffer, "this") == 0) {
                    globalCountThis++;
                } else if (strcmp(buffer, "are") == 0) {
                    globalCountAre++;
                }

                // unlock the mutex after updating the counters
                pthread_mutex_unlock(&counterMutex);
            }
        }
    }

    pthread_exit(NULL);
}

void *filterValue(void *arg) {
    char *block = (char *)arg;
    char bufferLine[1000];
    char bufferLowercase[1000];
    int i;

    char *blockCopy = strdup(block);
    if (blockCopy == NULL) {
        printf("Error: Memory allocation failed\n");
        pthread_exit(NULL);
    }

    //lock the mutex before accessing shared file
    pthread_mutex_lock(&fileMutex);
    FILE* filtered = fopen("filtered.txt", "a");
    if (filtered == NULL) {
        printf("Error: Could not open file filtered.txt\n");
        free(blockCopy);
        pthread_mutex_unlock(&fileMutex);
        pthread_exit(NULL);
    }

    char *line = strtok(blockCopy, "\n"); //divides the block to lines by indication of '\n'

    while (line != NULL) {
        strcpy(bufferLine, line); //copy the current line into the buffer
        for(i = 0; bufferLine[i] != '\0'; i++) {
            bufferLowercase[i] = tolower(bufferLine[i]); //turn the line to lowercase letters
        }
        bufferLowercase[i] = '\0';

        if(strstr(bufferLowercase, "value") == NULL) { //check if value is in the line
            fprintf(filtered, "%s\n", bufferLine);
        }

        line = strtok(NULL, "\n"); //start the line separation from where it left off
    }

    fclose(filtered);
    pthread_mutex_unlock(&fileMutex); //unlock mutex for shared file
    free(blockCopy);
    pthread_exit(NULL);
}

// function to send the threads to do parallel assigments
void *threadDo(void *arg) {
    pthread_t threads[2];

    pthread_create(&threads[0], NULL, countWords, arg);
    pthread_create(&threads[1], NULL, filterValue, arg);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    pthread_exit(NULL);

}


int main() {
    pthread_t threads[4];

    char **blocks = divideText("file.txt");
    if (blocks == NULL) {
        return 1;
    }

    pthread_mutex_init(&counterMutex, NULL);
    pthread_mutex_init(&fileMutex, NULL);

    //send each thread to the block processing function
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, threadDo, blocks[i]);
    }

    //wait for each thread to finish the block processing
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&counterMutex);
    pthread_mutex_destroy(&fileMutex);

    //write the results of the word counting into results.txt
    FILE* results = fopen("results.txt", "w");
    if (results == NULL) {
        printf("Error: Could not open file results.txt\n");
        freeBlocks(blocks);
        return 1;
    }

    fprintf(results, "Word counts:\n");
    fprintf(results, "the: %d\n", globalCountThe);
    fprintf(results, "was: %d\n", globalCountWas);
    fprintf(results, "is: %d\n", globalCountIs);
    fprintf(results, "this: %d\n", globalCountThis);
    fprintf(results, "are: %d\n", globalCountAre);

    fclose(results);

    printf("Word counts:\n");
    printf("the: %d\n", globalCountThe);
    printf("was: %d\n", globalCountWas);
    printf("is: %d\n", globalCountIs);
    printf("this: %d\n", globalCountThis);
    printf("are: %d\n", globalCountAre);

    freeBlocks(blocks);
    return 0;
}
