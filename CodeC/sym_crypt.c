#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "chiffrage_cryptographie.h"

// Fonction utilisant le getopt() pour extraire les différents composants
// int getopt(int argc, char * const argv[], const char *optstring);
// while ( (getopt() != -1) )
// test if() sur chaque option (-i, -o, -k/-f, -m, -v, -l, -h)

// Main du Programme 
//
int main(int argc, char* argv[]){
    if (argc > 2){
        perror(argv[0]);
    }

    printf("--- début programme ---\n");

    unsigned char* key = (unsigned char*)malloc((TAILLE_BLOC) * sizeof(char));
    char* message = argv[1];
    char* fich_crypt = "bin/crypte.txt";
    char* fich_decrypt = "bin/decrypte.txt";
        
    // Generation de la clé aleatoire stoké dans la variable "key"
    gen_key(10, key);

    printf("\n°°°°°° Traitement en cours °°°°°°\n");
   
    // Verification de la terminaison correcte du chiffrage
    if (xor_fichier(message, fich_crypt, key) == -1){
        perror("Erreur lors du chiffrage");
    }

    // // Verification de la terminaison correcte du dechiffrage
    if (xor_fichier(fich_crypt, fich_decrypt, key) == -1){
        perror("erreur de dechiffrage");
    }

    printf("\n--- Fin programme ---\n");

    mask("Datas/Source/msg1.txt", "test.txt");

    return 0;
}