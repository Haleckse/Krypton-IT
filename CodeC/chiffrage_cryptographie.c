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
#include <ctype.h>

#include "chiffrage_cryptographie.h"

/* ------ Fonctions de chiffrages/dechiffrages ------ */


// Fonction permettant de chiffrer un message caractères a caractères
// Retour de la fonction : message codé dans la variable crypted (code ASCII)
//
void xor(unsigned char* msg, unsigned char* key, int msg_length, int key_length, unsigned char* crypted){
    for (int i=0; i<msg_length; i++){
        crypted[i] = (msg[i] ^ key[i%key_length]);
    }
}


// Fonction permettant de déchiffrer un code crypte avec la cle (key)
// Retour de la fonction : message décodé dans la variable decrypted (code ASCII)
// 
void gen_key(int len, unsigned char *key){
  srand(time(0));
  char random_char;
  int i=0;

  while (i != len){
    random_char = rand() % 123;
    if (isalnum(random_char)){
        key[i] = (unsigned char) random_char;
        i++;
    }
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


// Fonction affichant sur Stdout l'aide des commandes
// ParamIn null ; ParamOut message stdout
//
void commandes_affichage(){
    printf("\n\t--- AIDE DES COMMADES ---\n\n");
    printf("_ -i : fichier contenant le message (obligatoire)\n");
    printf("_ -o : fichier où l’on va écrire le chiffré (obligatoire)\n");
    printf("_ -k : clef chiffrement (obligatoire)\n");
    printf("       ou -f : fichier contenant la clef \n");
    printf("_ -m : fichier contenant la méthode de chiffrement (obligatoire)\n");
    printf("_ -v : fichier contenant le vecteur initialisation (CBC)\n");
    printf("_ -l : fichier log (facultatif)\n");
    printf("_ -h : affichage aide de commande\n");   
}


// Fonction permettant le cryptage à partir d'un fichier
// Les fichiers seront ouverts dans la fonction
// 
int xor_fichier(const char* fich_in, const char* fichier_out, unsigned char* key){
    int nbEcrit=0, nbLus=0;
    unsigned char bloc[TAILLE_BLOC];
    unsigned char bloc_crypt[TAILLE_BLOC];

    char path_output[256]; // Ensure it has enough space for the result
    snprintf(path_output, sizeof(path_output), "%s", fichier_out);

    // Ouverture du fichier contenant le message à chiffrer
    int fichier_msg = open(fich_in, O_RDONLY);
    if(fichier_msg == -1){
        perror("Erreur ouverture fichier message");
    }

    // Ouverture ou création du fichier qui recevra le message chiffré
    int fichier_crypt = open(path_output, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IROTH);
    if(fichier_crypt == -1){
        perror("Erreur ouverture fichier crypte");
        close(fichier_msg);
    }

    while((nbLus == nbEcrit) && (nbLus = read(fichier_msg, bloc, TAILLE_BLOC))>0){
        xor(bloc, key, nbLus, strlen((char *)key), bloc_crypt);
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
// Les fichiers seront ouverts dans la fonction
//
int mask(const char* fich_in, const char* fich_out){
    char out[256]; // Ensure it has enough space for the result
    snprintf(out, sizeof(out), "%s", fich_out);

    /* Ouverture fichiers */
    FILE* msg = fopen(fich_in, "r");
    FILE* crypt = fopen(out, "w");
    FILE* log_key = fopen("log_key.txt", "a");
    
    /* Initialisation des variables */
    /* Search for the size of the key */
    fseek(msg, 0L, SEEK_END);
    int size = ftell(msg);
    rewind(msg);

    char msg_crypt[size];
    char buffer[size];
    unsigned char key[size];

    if (msg != NULL){
        /* Initialisation */
        gen_key(size, key);
        int len_key = strlen((char*) key);

        /* Lecture du message crypté*/
        if (fread(buffer, 1, size, msg) < size){
            perror("fread");
        }

        for(int i = 0; i < size; i++){
            msg_crypt[i] = (buffer[i] ^ key[i]);
        }

        /* Ecriture message crypté */
        fwrite(msg_crypt, 1, size, crypt);

        /* Strockage de la cle dans le log */
        fwrite("\n", 1, 1, log_key);
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

// Max size des blocks CBC
//
#define BLOCK_SIZE 16

// Fonction réalisant le chiffrement de la méthode CBC 
// Fichiers ouverts dans la fonction
//
int cbc_crypt(char *msg, unsigned char* key, char* iv, char* res){
    /* Redirection du fichier sortant */
    char out[256];
    snprintf(out, sizeof(out), "%s", res);

    /* Ouverture des différents fichiers utilisés */
    FILE* fichier_in = fopen(msg, "rb");
    FILE* fichier_out = fopen(out, "wb");
    FILE* fichier_vecteur = fopen(iv, "rb");

    /* Check s'ils sont bien ouvert sinon traitement */
    if (!fichier_in || !fichier_out || !fichier_vecteur){
        fprintf(stderr,"\nerreur lors de l'ouverture d'un fichier");
        return -1;
    }

    /* Variables nécéssaires */
    size_t nbLus;
    unsigned char *buffer = (unsigned char*) malloc(BLOCK_SIZE);
    unsigned char *tampon = (unsigned char*) malloc(BLOCK_SIZE);
    unsigned char *prev_block = (unsigned char*) malloc(BLOCK_SIZE);
    unsigned char *chiffree = (unsigned char*) malloc(BLOCK_SIZE);
    unsigned char *iv_key = (unsigned char*) malloc(BLOCK_SIZE);

    /* mettre la clef a la bonne longueur ? */
    if (fread(iv_key, sizeof(char), BLOCK_SIZE, fichier_vecteur) < BLOCK_SIZE){
        perror("Error reading the IV from the file");
        return -1;
    }
    memcpy(prev_block, iv_key, BLOCK_SIZE);

    /* Traitement du fichier pour le crypté */
    while ( (nbLus = fread(buffer, 1, BLOCK_SIZE, fichier_in)) > 0){
        
        // Ajoute des espaces si le bloc n'est pas complet
        if (nbLus < BLOCK_SIZE){
            memset(buffer + nbLus, ' ', BLOCK_SIZE - nbLus);
        }

        // XOR le bloc avec le précédent        
        xor(buffer, prev_block, BLOCK_SIZE, BLOCK_SIZE, tampon);

        // Chiffre le bloc
        xor(tampon, key, BLOCK_SIZE, strlen((char*)key), chiffree);

        // Ajout du bloc crypté dans le fichier sortant
        fwrite(chiffree, 1, BLOCK_SIZE, fichier_out);
        
        // modifie le bloc précédent pour la prochaine itération
        memcpy(prev_block, chiffree, BLOCK_SIZE);
    }

    /* Fermeture des fichiers ouverts */
    fclose(fichier_in);
    fclose(fichier_out);
    fclose(fichier_vecteur);

    /* Libération de la mémoire utilisée */
    free(buffer);
    free(prev_block);
    free(tampon);
    free(chiffree);
    free(iv_key);

    return 0;
}


// Fonction permettant cette fois-ci le déchiffrement de la méthode CBC
// Fichiers ouverts dans la fonction
//
int cbc_decrypt(char *msg, unsigned char *key, char *iv, char *res){
    /* Redirection du fichier sortant */
    char out[256];
    snprintf(out, sizeof(out), "%s", res);

    /* Ouverture des fichiers nécessaire */
    FILE* fichier_in = fopen(msg, "rb");
    FILE* fichier_out = fopen(out, "wb");
    FILE* fichier_vecteur = fopen(iv, "rb");
    
    /* Check s'ils sont bien ouvert sinon traitement */
    if (!fichier_in || !fichier_out || !fichier_vecteur){
        fprintf(stderr,"\nerreur lors de l'ouverture d'un fichier");
        return -1;
    }

    /* Variables nécéssaires */
    size_t nbLus;
    unsigned char *buffer = (unsigned char*) malloc(BLOCK_SIZE);
    unsigned char *prev_block = (unsigned char*) malloc(BLOCK_SIZE);
    unsigned char *temp_block = (unsigned char*) malloc(BLOCK_SIZE);
    unsigned char *cypher_block = (unsigned char*) malloc(BLOCK_SIZE);
    unsigned char *iv_key = (unsigned char*) malloc(BLOCK_SIZE);

    /* Lit le fichier vecteur et traite en cas d'erreur */
    if (fread(iv_key, 1, BLOCK_SIZE, fichier_vecteur) < BLOCK_SIZE){
        perror("Error reading the IV from the file");
        return -1;
    }

    memcpy(prev_block, iv_key, BLOCK_SIZE);

    /* Traitement du fichier pour le décrypté */
    while ((nbLus = fread(buffer, 1, BLOCK_SIZE, fichier_in)) > 0){

        // Check si le block est plein
        if (nbLus < BLOCK_SIZE){
            fprintf(stderr,"Error block is not full ");
        }
        memcpy(temp_block, buffer, BLOCK_SIZE);

        // Decrypte le block
        xor(buffer, key, BLOCK_SIZE, strlen((char*)key), temp_block);

        // XOR du block avec le précédent
        xor(temp_block, prev_block, BLOCK_SIZE, BLOCK_SIZE, cypher_block);

        // Copie le block déchiffré dans le fichier OUT
        memcpy(prev_block, buffer, BLOCK_SIZE);

        // Ecrit le bloc déchiffré dans le fichier sortant
        fwrite(cypher_block, 1, BLOCK_SIZE, fichier_out);
    }

    /* Fermeture des fichiers ouverts */
    fclose(fichier_in);
    fclose(fichier_out);
    fclose(fichier_vecteur);

    /* Liberation de la mémoire utilisée */
    free(buffer);
    free(prev_block);
    free(temp_block);
    free(cypher_block);
    free(iv_key);

    return 0;
}