#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "chiffrage_cryptographie.h"

// Main du Programme 
//

int main(int argc, char* argv[]){

    printf("--- début programme ---\n\n");

    char* key = "rutabaga";
    char* message = malloc(TAILLE_BLOC);
    
    // Recuperation du fichier contenant le message à chiffrer
    printf("Saisir le nom du fichier contenant le message : ");

    if (scanf("%s", message) > 0){
        printf("\n°°°°°° Traitement en cours °°°°°°\n");
    } else {
        printf("\nMauvais paramètre d'entrée ...\n");
    }

    // Verification de la terminaison correcte du chiffrage
    if (chiffrage_char_fichier(message, key) == -1){
        perror("Erreur lors du chiffrage\n");
        //exit(1);
    }

    // Verification de la terminaison correcte du dechiffrage
    if (dechiffrage_char_fichier(key) == -1){
        perror("erreur de dechiffrage");
    }


    printf("\n--- Fin programme ---\n");

    free(message);
    return 0;
}