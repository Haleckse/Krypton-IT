#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "chiffrage_cryptographie.h"
#include "break_code.h"

/**************************/
/* Début du programme main*/
/**************************/

#define MAX 256

// Fonction affichant sur Stdout l'aide des commandes
// ParamIn null ; ParamOut message stdout
//
void display_help(){
    printf("\n\t--- AIDE DES COMMADES ---\n\n");
    printf("_ help : donne la liste des commandes\n");
    printf("_ list-keys : donne la liste des clefs générées et disponibles et indique celle qui ont déjà été utilisée\n");
    printf("_ gen-key < n > : génère une clef de longueur n\n");
    printf("_ del-key < key > : supprime la clef < key >\n");
    printf("_ diffie < param > [< key length >] : Execute l'échange Diffie-Hellman\n");
    printf("_ encrypt < in >< out >< key >< method > [< vecteur d'initialisation >]\n");
    printf("_ decrypt < in >< out >< key >< method > [< vecteur d'initialisation >]\n");
    printf("_ crack < in >< out >< length >< dico >\n");
    printf("_ quit\n\n");   
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
    display_help();

    /* Boucle de commandes */
    while (continuer) {

        // Demander la commande à l'utilisateur
        printf("\033[34m Saisir la commande souhaitée : ");
        printf("\033[37m");

        if (fgets(commande_user, MAX, stdin) == NULL) {
            printf("Erreur lors de la lecture de la commande.\n");
            break;
        } else {
            
            printf("\n --------------------------------------------------\n\n");

            // Extraire le premier mot dans la variable "executable"
            garderPremierMot(commande_user, executable);

            // Vérifier la commande pour quitter
            if (strcmp(executable, "quit") == 0) {
                continuer = 0;

            // Réafficher les commandes disponibles
            } else if (strcmp(executable, "help") == 0) {
                display_help();

            // Traitement de la méthode gen-key
            } else if (strcmp(executable, "gen-key") == 0){
                int size_key = atoi(commande_user + 8);
                unsigned char* key = malloc(size_key * sizeof(char));

                gen_key(size_key, key);

                printf("La clef générée est : \"%s\"\n", key);

            // Traitement de la méthode D-H
            } else if (strcmp(executable, "diffie") == 0){
                snprintf(command, sizeof(command), "bin/dh_gen_group %s", commande_user + 7);
                if (system(command) == -1){
                    perror("erreur redirection dh_gen_group.");
                }

            // Traitement des commandes encrypt ou decrypt
            } else if (strcmp(executable, "encrypt") == 0 || strcmp(executable, "decrypt") == 0){
                snprintf(command, sizeof(command), "bin/sym_crypt %s", commande_user + 8);
                if (system(command) == -1){
                    perror("erreur redirection sym_crypt.");
                }

            // Traitement des commandes Crack
            } else if (strcmp(executable, "crack") == 0){ 
                /* Il s'agit du crack C1*/
                snprintf(command, sizeof(command), "bin/break_code %s", commande_user + 6);
                if (system(command) == -1){
                    perror("erreur redirection break_code.");
                }                

            // Cas par défaut
            } else { 
                printf("Commande inconnue : \"%s\"\n", executable);
            }

            printf("\n --------------------------------------------------\n\n");
        }
    }

    /* Libération de la mémoire */
    free(commande_user);

    /* Fin du programme */
    printf("\nMERCI ET À BIENTÔT !\n");
    return 0;
}
