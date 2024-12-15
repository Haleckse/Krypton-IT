#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "chiffrage_cryptographie.h"
#include "crack_functions.h"

/**************************/
/* Début du programme main*/
/**************************/

#define MAX 256

// Fonction affichant sur Stdout l'aide des commandes
// ParamIn null ; ParamOut message stdout
//
void commandes_affichage(){
    printf("\n\t--- AIDE DES COMMADES ---\n\n");
    printf("_ help : donne la liste des commandes\n");
    printf("_ list-keys : donne la liste des clefs générées et disponibles et indique celle qui ont déjà été utilisée\n");
    printf("_ gen-key < n > : génère une clef de longueur n\n");
    printf("_ del-key < key > : supprime la clef < key >\n");
    printf("_ encrypt < in >< out >< key >< method > [< vecteur d'initialisation >]\n");
    printf("_ decrypt < in >< out >< key >< method > [< vecteur d'initialisation >]\n");
    printf("_ crack< in >< out >< length >< dico >\n");
    printf("_ quit\n");   
}

void garderPremierMot(const char* source, char* destination) {
    int i = 0;

    // Copier le premier mot jusqu'au premier espace
    while (source[i] != '\0' && source[i] != ' ' && source[i] != '\n') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0'; 
}

int main() {
    /* Variables locales */
    char* commande_user = malloc(MAX * sizeof(char));
    if (commande_user == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        return 1;
    }

    char executable[MAX];
    char command[256];
    int continuer = 1;   

    /* Affichage des commandes */
    commandes_affichage();

    /* Boucle de commandes */
    while (continuer) {
        // Demander la commande à l'utilisateur
        printf("Saisir la commande souhaitée : ");
        if (fgets(commande_user, MAX, stdin) == NULL) {
            printf("Erreur lors de la lecture de la commande.\n");
            break;
        }

        // Extraire le premier mot dans la variable "executable"
        garderPremierMot(commande_user, executable);

        // Vérifier la commande pour quitter
        if (strcmp(executable, "quit") == 0) {
            continuer = 0;

        } else if (strcmp(executable, "help") == 0) { // Réafficher les commandes disponibles
            commandes_affichage();
        
        } else if (strcmp(executable, "gen-key") == 0){
            unsigned char* key;
            gen_key(10, key);

        } else if (strcmp(executable, "encrypt") == 0 || strcmp(executable, "decrypt") == 0){ // encrypt ou decrypt
            snprintf(command, sizeof(command), "bin/sym_crypt %s", commande_user);
            system(command);

        } else { // Cas par défaut 
            printf("Commande inconnue : \"%s\"\n", executable);
        }
    }

    /* Libération de la mémoire */
    free(commande_user);

    /* Fin du programme */
    printf("\nMERCI ET À BIENTÔT !\n");
    return 0;
}
