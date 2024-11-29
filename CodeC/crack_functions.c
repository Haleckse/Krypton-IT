#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "chiffrage_cryptographie.h"

#define maxCaractères 62

// structure Threads
struct Info{
    int id;
    char* tab;
    unsigned int length;
};

// Variables globales 
FILE* input;
pthread_mutex_t mutex;

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


// Teste la validité d'un caractère 
// Retourne 1 si valide, 0 sinon
//
int valide(int xor){
    if ( (32<=xor && xor<=63) || (65<=xor && xor<=90) || (96<=xor && xor<=122) ){
        return 1;
    }
    return 0;
}

// Code exécuté par les threads
// Recherche toutes les possibilités pour code[i] + code[i+key_length] + ...
// 
void* rechercherClefThread(void* param){
    struct Info* info = malloc(sizeof(struct Info));
    info = (struct Info*) param;

    char* buffer = malloc(info->length * sizeof(char));
    int position = 0; int valXor;
    int keyCar = true;
        
    // ensemble des candidats pour clef[i]
    for (int i=0; i<maxCaractères; i++){

        pthread_mutex_lock(&mutex);
        while( (keyCar) && (fread(buffer, 1, info->length, input) > 0) ){
            // Xor les caracteres buffer[info->id] avec info->tab[i]
            valXor = buffer[info->id] ^ info->tab[i];

            // Si mauvais retour -> keyCar = 0 
            if (valide(valXor) != 1)
                keyCar = false;
        }

        if (!keyCar){
            // Suppression de l'element 
            for (int j = i - 1; j < strlen(info->tab); j++)
                info->tab[j] = info->tab[j+1];
        }

        keyCar = true;
        rewind(input);
        pthread_mutex_unlock(&mutex);
    }

    free(buffer);

    pthread_exit((void*)info);
}


// Fonction permettant la réalisation du premier crack
// Retourne un tableau contenant les clefs possibles
//
char* break_code_c1(const char* infile, unsigned int key_length) {
    // Allocations mémoires tableaux 
   char** tableauCaractere = tableau2D(key_length);
   char* key_tab = malloc(555*sizeof(char*));
   pthread_t tab_thread[key_length];
   struct Info info[key_length];
   struct Info* res;

   // Ouverture du fichier
    input = fopen(infile, "rb");
    if (!input){
        fprintf(stderr,"\nerreur lors de l'ouverture d'un fichier\n");
        exit(EXIT_FAILURE);
    } 

    // Initialisation du sémaphore mutex 
    pthread_mutex_init(&mutex, NULL);

    // ensemble des caractères par indice de clef
    for (int i=0; i<key_length; i++){
        info[i].id = i;
        info[i].length = key_length;
        info[i].tab = tableauCaractere[i];
        pthread_create(&tab_thread[i], NULL, rechercherClefThread, (void*)&info[i]);
    }

    // Recuperation retour threads
    for (int i=0; i<key_length; i++){
        if(pthread_join(tab_thread[i], (void**) &res) == -1){
            fprintf(stderr,"\nerreur join thread\n");
            exit(EXIT_FAILURE);
        }

        tableauCaractere[i] = res->tab;
        printf("Thead (%ld) => tab  : %s\n", tab_thread[i], tableauCaractere[i]);
    }

    // Free memoire

    return key_tab;
}

int main(int argc, char* argv[]) {
    char* res;
    unsigned int key_length = 4;

    res = break_code_c1("Output/crypt.txt", 5);

    return 0;
}
