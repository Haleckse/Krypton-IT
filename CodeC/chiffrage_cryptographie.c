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
#include <time.h>

#include "chiffrage_cryptographie.h"

/* ------ Fonctions de chiffrages/dechiffrages ------ */


// Fonction permettant de chiffrer un message caractères a caractères
// Retour de la fonction : message codé dans la variable crypted (code ASCII)
//
void xor(unsigned char* msg, unsigned char* key, int msg_length, char* crypted){
    
    int len_key = strlen((const char *)key);

    for (int i=0; i<msg_length; i++){
        crypted[i] = (msg[i] ^ key[i%len_key]);
    }
}


// Fonction permettant de déchiffrer un code crypte avec la cle (key)
// Retour de la fonction : message décodé dans la variable decrypted (code ASCII)
// 
void gen_key(int len, unsigned char *key){
  srand(time(0));

  for(int i = 0; i < len; i++){
    unsigned char random_char = (unsigned char)(rand() % 256);
    key[i] = random_char;
  }

  key[len] = '\0';
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
int xor_fichier(char* fich_in, char* fichier_out, unsigned char* key){
    int nbEcrit=0, nbLus=0;
    unsigned char bloc[TAILLE_BLOC];
    char bloc_crypt[TAILLE_BLOC];

    // Ouverture du fichier contenant le message à chiffrer
    int fichier_msg = open(fich_in, O_RDONLY);
    if(fichier_msg == -1){
        perror("Erreur ouverture fichier message");
    }

    // Ouverture ou création du fichier qui recevra le message chiffré
    int fichier_crypt = open(fichier_out, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IROTH);
    if(fichier_crypt == -1){
        perror("Erreur ouverture fichier crypte");
        close(fichier_msg);
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

// Fonction permettant la réalisation du chiffrement de Vernam
// Les fichiers seront ouverts dans la fonctions
//
int mask(const char* fich_in, const char* fich_out){
    /* Ouverture fichiers */
    FILE* msg = fopen(fich_in, "r");
    FILE* crypt = fopen(fich_out, "w");
    FILE* log_key = fopen("log_key.txt", "a");
    
    /* Initialisation des variables */
    int len_msg = strlen(fich_in);
    char msg_crypt[len_msg];
    char buffer[len_msg];
    unsigned char key[len_msg];

    if (msg != NULL){
        /* Initialisation */
        gen_key(len_msg, key);
        int len_key = strlen((char*) key);

        /* Lecture du message crypté*/
        fread(buffer, 1, len_msg, msg);

        for(int i = 0; i < len_msg; i++){
            msg_crypt[i] = (buffer[i] ^ key[i]);
        }

        /* Ecriture message crypté */
        fwrite(msg_crypt, 1, len_msg, crypt);

        /* Strockage de la cle dans le log */
        fwrite(key, 1, len_key, log_key);

        /* Fermeture Fichiers */
        fclose(msg);
        fclose(crypt);
        fclose(log_key);
    } else {
        /* Erreur ouverture fichier */
        return -1;
    }

    return 0;
}

