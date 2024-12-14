#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "./CodeC/dh_prime.h"

int read_int(const char* filename) {
    FILE *file = fopen(filename, "r"); // Ouvrir le fichier en mode lecture
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE); // Quitter le programme en cas d'erreur
    }

    int value;
    if (fscanf(file, "%d", &value) != 1) {
        perror("Erreur lors de la lecture de l'entier dans le fichier");
        fclose(file); // Fermer le fichier
        exit(EXIT_FAILURE); // Quitter le programme en cas d'erreur
    }

    fclose(file); // Fermer le fichier après utilisation
    return value; // Retourner l'entier lu
}

int bit_length(long num) {
    int bits = 0;

    // Vérifier si le nombre est nul
    if (num == 0) {
        return 1;  // Par convention, on retourne 1 bit pour zéro
    }

    // Décalage à droite jusqu'à ce que num soit nul
    while (num != 0) {
        num >>= 1; // Décalage à droite
        bits++;    // Incrémenter le compteur de bits
    }

    return bits;
}

void redirect_output_to_file(const char *filename) {
    // Sauvegarde de stdout original
    FILE *original_stdout = stdout;

    // Ouverture du fichier en écriture
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    // Redirection de stdout vers le fichier
    stdout = file;

    // Exemple de fonction avec plusieurs printf
    printf("Ceci est redirigé dans un fichier\n");
    printf("Deuxième ligne redirigée\n");

    // Restauration de stdout original
    fflush(stdout);
    fclose(file);
    stdout = original_stdout;
}

int main(void) {

   
    int ind = 0; 
    int keys[10]; 
    for (int i = 8; i < 32; i = i + 8){
        char command[256];  // Assurez-vous que la taille est suffisante
        
        snprintf(command, sizeof(command), "./bin/dh_gen_group -o param.txt -l %d", i); 
        for (int y = 0; y < 2; y++) {
            system(command); 
            int clef_lu = read_int("./Output/key.txt"); 
            keys[ind] = clef_lu; 
            ind++; 
        }
    }
    
    printf("\n----------Test de la longueur des clefs trouvées----------\n\n"); 
    
    for (int i = 0; i < 6; i++){
        printf("La clef %d est bien une clef de %d bits\n", keys[i], bit_length(keys[i])); 
    }
        
    
}