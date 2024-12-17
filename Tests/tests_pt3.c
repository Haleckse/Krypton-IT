#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

// Mock for your missing header, adjust as needed
#include "../CodeC/break_code.h"

#define MAX_PATH_LEN 256
#define MAX_CMD_LEN 512
#define NUM_FILES 1

#define BLUE   "\033[0;34m"     // Bleu standard
#define ORANGE "\033[38;5;214m" // Approximation d'orange (palette étendue 256 couleurs)
#define VIOLET "\033[0;35m"     // Violet standard
#define RESET  "\033[0m"        // Réinitialisation des couleurs

void test_c1();
void test_all();

void main(){
    test_c1();
    test_all();
}

void test_c1(){
    printf("\n---------- Tests du temps d'éxecution de la fonction break_code_c1 ----------\n\n"); 
    struct timeval start, end;

    // TEST CLEF 3
    gettimeofday(&start, NULL);
    
    printf(BLUE"\ttexte \'1234_msg2.txt\' avec une clef de 3\n\n"RESET);
    system("./../bin/break_code -i ../1234_msg2.txt -o ../Output/key_candidates1.bin -m c1 -k 3");
    
    gettimeofday(&end, NULL);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("Temps d'exécution : %.6f secondes\n\n", time_spent);

    // TEST CLEF 4
    gettimeofday(&start, NULL);
    
    printf(VIOLET"\ttexte \'1234_msg2.txt\' avec une clef de 4\n\n"RESET);
    system("./../bin/break_code -i ../1234_msg2.txt -o ../Output/key_candidates2.bin -m c1 -k 4");
    
    gettimeofday(&end, NULL);
    time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("Temps d'exécution : %.6f secondes\n\n", time_spent);

    // TEST CLEF 5
    gettimeofday(&start, NULL);
    
    printf(ORANGE"\ttexte \'1234_msg2.txt\' avec une clef de 5\n\n"RESET);
    system("./../bin/break_code -i ../1234_msg2.txt -o ../Output/key_candidates3.bin -m c1 -k 5");
    
    gettimeofday(&end, NULL);
    time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("Temps d'exécution : %.6f secondes\n\n", time_spent);
}

void test_all(){
    printf("\n---------- Tests du temps d'éxecution de la fonction break_code_c1 / c2 / c3 ----------\n\n"); 
    struct timeval start, end;

    // TEST CLEF 3
    gettimeofday(&start, NULL);
    
    printf(BLUE"\ttexte \'1234_msg2.txt\' avec une clef de 3\n\n"RESET);
    system("./../bin/break_code -i ../1234_msg2.txt -o ../Output/key_candidates1.bin -m all -k 3 -d ../dicoFrSA.txt");
    
    gettimeofday(&end, NULL);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("Temps d'exécution : %.6f secondes\n\n", time_spent);

    // TEST CLEF 4
    gettimeofday(&start, NULL);
    
    printf(VIOLET"\ttexte \'1234_msg2.txt\' avec une clef de 4\n\n"RESET);
    system("./../bin/break_code -i ../1234_msg2.txt -o ../Output/key_candidates2.bin -m all -k 4 -d ../dicoFrSA.txt");
    
    gettimeofday(&end, NULL);
    time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("Temps d'exécution : %.6f secondes\n\n", time_spent);

    // TEST CLEF 5
    gettimeofday(&start, NULL);
    
    printf(ORANGE"\ttexte \'1234_msg2.txt\' avec une clef de 5\n\n"RESET);
    system("./../bin/break_code -i ../1234_msg2.txt -o ../Output/key_candidates3.bin -m all -k 5 -d ../dicoFrSA.txt");
    
    gettimeofday(&end, NULL);
    time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("Temps d'exécution : %.6f secondes\n\n", time_spent);
}