/************************************************************/
/*  Programme chiffrant les données en utilisant plusieurs  */
/* Méthodes : caractères a caractère, CBC et masque jetable */
/************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "chiffrage_cryptographie.h"

/* ------ Fonctions de chiffrages/dechiffrages ------ */


// Fonction permettant de chiffrer un message caractères a caractères
// Retour de la fonction : message codé dans la variable crypted (code ASCII)
//
void xor(char* msg, char* key, int msg_length, char* crypted){

     for (int i=0; i<msg_length; i++)
        crypted[i] = (msg[i] ^ key[i%strlen(key)]);
    
}


// Fonction permettant de déchiffrer un code crypte avec la cle (key)
// Retour de la fonction : message décodé dans la variable decrypted (code ASCII)
// 
char* gen_key(int length) {
    if (length <= 0) {
        return NULL;
    }

    // Définition des caractères alphanumériques
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int num_chars = sizeof(characters) - 1; 

    char* key = (char*)malloc((length + 1) * sizeof(char)); 
    if (key == NULL) {
        return NULL;
    }

    // Génération de la clé
    for (int i = 0; i < length; i++) {
        key[i] = characters[rand() % num_chars];
    }

    key[length] = '\0';

    return key;
}


// Fonction permettant l'affichage sur STDOUT d'une chaine de caractère 
// contenu dans la variable affiche_msg
// 
void afficher_message(char* affiche_msg){
    int length = strlen(affiche_msg);

    for (int i=0; i<length; i++){
        printf("%c", affiche_msg[i]);
    }
    printf("\n");
}


// Fonction permettant le cryptage à partir d'un fichier
// Les fichiers seront ouverts dans la fonctions
// 
int xor_fichier(char* fich_in, char* fichier_out, char* key){
    int nbEcrit=0, nbLus=0;
    char bloc[TAILLE_BLOC];
    char bloc_crypt[TAILLE_BLOC];

    // Ouverture du fichier contenant le message à chiffrer
    int fichier_msg = open(fich_in, O_RDONLY);
    if(fichier_msg == -1){
        perror("Erreur ouverture fichier message");
        return -1;
    }

    // Ouverture ou création du fichier qui recevra le message chiffré
    int fichier_crypt = open(fichier_out, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IROTH);
    if(fichier_crypt == -1){
        perror("Erreur ouverture fichier crypte");
        close(fichier_msg);
        return -1;
    }

    while((nbLus == nbEcrit) && (nbLus = read(fichier_msg, bloc, TAILLE_BLOC))>0){
        xor(bloc, key, nbLus, bloc_crypt);
        nbEcrit = write(fichier_crypt, bloc_crypt, nbLus);
    }

    // Fermeture fichiers
    close(fichier_msg);
    close(fichier_crypt);

    if ((nbLus >= 0) && (nbEcrit > 0)){
        return 0;
    } 
    return -1;
}


// Fonction permettant le decryptage d'un fichier
// Les fichiers seront ouverts dans la fonctions
//
int dechiffrage_char_fichier(char* nom_fichier, char* key){
    int nbLus = 0, nbEcrit = 0;
    char bloc2[TAILLE_BLOC];
    char bloc_decrypt[TAILLE_BLOC];

    int fichier_a_decrypt = open(nom_fichier, O_RDONLY);
    if (fichier_a_decrypt == -1){
        perror("Erreur ouverture fichier message");
        return -1;
    }

    int fichier_decrypt = open("decrypte.txt", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IRWXO);
    if( fichier_decrypt == -1){
        perror("Erreur ouverture fichier crypte");
        close(fichier_a_decrypt);
        return -1;
    }
    
    // Déchiffrage du contenu du fichier "fichier_a_decrypt" dans "fichier_decrypt"
    while((nbLus == nbEcrit) && (nbLus = read(fichier_a_decrypt, bloc2, TAILLE_BLOC))>0){
        xor(bloc2, key, nbLus, bloc_decrypt);
        nbEcrit = write(fichier_decrypt, bloc_decrypt, nbLus);
    }

    // Fermeture fichiers
    close(fichier_a_decrypt);
    close(fichier_decrypt);

    return 0;
}