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
int main(){

    printf("--- début programme ---\n\n");

    unsigned char* key = (unsigned char*)malloc((TAILLE_BLOC) * sizeof(char)); 
    char* message = malloc((TAILLE_BLOC) * sizeof(char));
    char* fich_crypt = "crypte.txt";
    char* fich_decrypt = "decrypte.txt";
        
    gen_key(10, key);

    // Recuperation du fichier contenant le message à chiffrer
    printf("Saisir le nom du fichier contenant le message : ");

    if (scanf("%s", message) > 0){
        printf("\n°°°°°° Traitement en cours °°°°°°\n");
    } else {
        printf("\nMauvais paramètre d'entrée ...\n");
    }

    // Verification de la terminaison correcte du chiffrage
    if (xor_fichier(message, fich_crypt, key) == -1){
        perror("Erreur lors du chiffrage :");
        //exit(1);
    }

    // Verification de la terminaison correcte du dechiffrage
    if (xor_fichier(fich_crypt, fich_decrypt, key) == -1){
        perror("erreur de dechiffrage :");
    }

    printf("\n--- Fin programme ---\n");

    free(message);
    return 0;
}