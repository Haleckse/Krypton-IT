#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chiffrage_cryptographie.h"

#define maxCaractères 62

// Fonction permettant de regrouper tous les caractères alphanumériques
// Retourne un tableau conteant les 62 caractères (char*)
//
char* init_caracteres(){
    char* tableau = malloc(maxCaractères * sizeof(char*));
    int index = 0;

    // Ajout des chiffres (0-9)
    for (char c = '0'; c <= '9'; c++) {
        tableau[index++] = c;
    }

    // Ajout des lettres majuscules (A-Z)
    for (char c = 'A'; c <= 'Z'; c++) {
        tableau[index++] = c;
    }

    // Ajout des lettres minuscules (a-z)
    for (char c = 'a'; c <= 'z'; c++) {
        tableau[index++] = c;
    }

    return tableau;
}

// Fonction permettant la création d'un tableau 2D
// Retourne un tableau 2D alloué dynamiquement de `key_length` chaînes.
//
char** tableau2D(unsigned int key_length){
    // Initialisation du tableau 2D
    char** tabCaractere = malloc(key_length * sizeof(char*));
    if (tabCaractere == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire échouée\n");
        exit(EXIT_FAILURE);
    }

    // tableau de caracteres
    char * tab = init_caracteres();

    // remplissage du tableau avec un tableau alphanumérique
    for (unsigned int i = 0; i < key_length; i++) {
        tabCaractere[i] = malloc(maxCaractères * sizeof(char*));
        strcpy(tabCaractere[i], tab);
        if (tabCaractere[i] == NULL) {
            fprintf(stderr, "Erreur : allocation mémoire échouée\n");
            exit(EXIT_FAILURE);
        }
    }

    return tabCaractere;
}


// Fonction permettant la réalisation du premier crack
// Retourne un tableau contenant les clefs possibles
//
char* break_code_c1(const char* infile, unsigned int key_length) {
    /* Allocations mémoires tableaux */
   char** tableauCaractere = tableau2D(key_length);
   char* key_tab = malloc(555*sizeof(char*));

   /* Ouverture du fichier */
   FILE* input = fopen(infile, "rb");
    if (!input){
        fprintf(stderr,"\nerreur lors de l'ouverture d'un fichier");
        exit(EXIT_FAILURE);
    }

    // ensemble des caractères par indice de clef
    for (int i=0; i<key_length; i++){
        // ensemble des candidats pour clef[i]
        for (int car=0; car<strlen(tableauCaractere[i]); i++){
            
        }
    }

    return key_tab;
}

int main(int argc, char* argv[]) {
    char** res;
    unsigned int key_length = 4;

    res = tableau2D(key_length);

    printf("Résultats des clefs générées :\n");
    for (unsigned int i = 0; i < key_length; i++) {
        printf("%s %ld\n", res[i], strlen(res[i]));
        free(res[i]);
    }

    free(res);

    return 0;
}
