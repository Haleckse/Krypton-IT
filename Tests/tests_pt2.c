#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void dh_test(int size_p) {
    printf("\n---------- Test simulation d'échange de clef de Diffie-Hellman avec p de taille %d bits----------\n\n", size_p);
    char command[128]; 
    snprintf(command, 128, "cd ..; ./bin/dh_gen_group -o param.txt -l %d", size_p); 
    system(command); 
    printf("\n"); 
    system("./checkfiles ../Output/alice_key.txt ../Output/bob_key.txt"); 
    
}

int main() {
    char next;

    

    dh_test(8);
    printf("Tapez 'n' pour passer à la simulation suivante...\n");
    scanf(" %c", &next);  // Lecture du caractère 'n' ou autre.

    if (next == 'n' || next == 'N') {
        dh_test(16);
    } else {
        printf("Simulation arrêtée.\n");
        return 0;
    }

    printf("Tapez 'n' pour passer à la simulation suivante...\n");
    scanf(" %c", &next);

    if (next == 'n' || next == 'N') {
        dh_test(24);
    } else {
        printf("Simulation arrêtée.\n");
    }

    printf("\n---------- Fin de la simulation ----------\n");
    return 0;
}