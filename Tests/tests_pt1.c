#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

// Mock for your missing header, adjust as needed
#include "../CodeC/chiffrage_cryptographie.h"

#define MAX_PATH_LEN 256
#define MAX_CMD_LEN 512
#define NUM_FILES 2

char* create_command(const char* method, const char* fich_in, const char* extension) {
    // Allocate memory for the command string
    char* command = (char*)malloc(MAX_CMD_LEN);
    if (!command) {
        perror("Failed to allocate memory for command");
        exit(EXIT_FAILURE);
    }

    // Generate the command based on the provided arguments
    if (strcmp(method, "xor") == 0) {
        snprintf(command, MAX_CMD_LEN, 
                 "./../bin/sym_crypt -i %s -o ../Output/fich_out.%s -m xor -f ../key.txt", 
                 fich_in, extension);
    } else {
        fprintf(stderr, "Unknown method: %s\n", method);
        free(command);
        return NULL;
    }

    return command;
}

int main(void) {
    // Define the paths for input files
    char* path[NUM_FILES] = {
        "../Datas/Source/chaton.jpg",
        "../Datas/Source/ring.txt",
    };

    // Define an array for the commands
    char* commands[NUM_FILES];

    // Generate commands for each file
    for (int i = 0; i < NUM_FILES; i++) {
        const char* extension = strrchr(path[i], '.'); // Extract file extension
        if (extension) {
            extension++; // Skip the dot
        } else {
            fprintf(stderr, "File %s has no extension. Skipping.\n", path[i]);
            continue;
        }

        commands[i] = create_command("xor", path[i], extension);
        printf("command : %s\n", commands[i]); 
    }

    char checkfile_command[MAX_CMD_LEN];
    
    // Execute commands
    for (int i = 0; i < NUM_FILES; i++) {
        if (commands[i]) {
            printf("Executing: %s\n", commands[i]);
            system(commands[i]); // Run the command
            snprintf(checkfile_command, MAX_CMD_LEN, "./checkfiles %s %s", "../Output/fich_out", path[i]);
            system(checkfile_command); 
            
            free(commands[i]);   // Free allocated memory
        }
    }

    return 0;
}
