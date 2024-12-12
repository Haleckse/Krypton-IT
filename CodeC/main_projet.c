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

// Fonction affichant sur Stdout l'aide des commandes
// ParamIn null ; ParamOut message stdout
//
void commandes_affichage(){
    printf("\n\t--- AIDE DES COMMADES ---\n\n");
    printf("_ help : donne la liste des commandes\n");
    printf("_ list-keys : donne la liste des clefs générées et disponibles et indique celle qui ont déjà été utilisée\n");
    printf("_ gen-key < n > : génère une clef de longueur n\n");
    printf("_ del-key < key > : supprime la clef < key >\n");
    printf("_ encrypt < in >< out >< key >< method > [< vecteurd0 initialisation >]\n");
    printf("_ decrypt < in >< out >< key >< method > [< vecteurd0 initialisation >]\n");
    printf("_ crack< in >< out >< length >< dico >\n");
    printf("_ quit\n");   
}

int main(){
    /* Variables locales */
    char* commande_user;
    unsigned char* key;

    /* Boucle de commandes */
    while (strcmp(commande_user, "quit") != 0){
        /* Affichage des commandes */
        commandes_affichage();

        /* Demande à l'utilisateur */
        printf("sasir la commande souhaitée : ");
        scanf("%s",commande_user);

        /* Verification de la saisie utilisateur */

        /* Séparation des différents cas en fonction du premier mot saisit */
        switch ("premier mot"){
            // Affiche des commandes
            case "help":
                commande_user();
                break;
                
            // Génère une clé de longueur N
            case "gen-key":
                //gen_key(N, key);
                break;

            // Supprime une clé 
            case "del-key":
                break;
                
            // Chiffrement d'un fichier 
            case "encrypt":
                break;

            // Dechiffrement d'un fichier
            case "decrypt":
                break;

            // Crack réalisé sur un fichier
            case "crack":
                break;
            
            // Cas par défaut => erreur dans la saisie
            default:
                printf("Le mot saisit : %s n'a pas d'équivalence. Merci de réessayer\n");
                break;
            }
    }

    /* Fin du programme */
    printf("\n MERCI ET A BIENTÔT ! \n");
    return 0;
}