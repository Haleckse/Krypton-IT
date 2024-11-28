#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "chiffrage_cryptographie.h"

/* ============================================================================== */
/* ============================================================================== */


// Main du Programme 
//
int main(int argc, char* argv[]){
    
    printf("\n--- début programme ---\n");

    /* Necessary variables for the program */
    unsigned char* key;
    char* nom;
    char* fich_in; char* fich_out;
    char* methode_crypt; 
    char* fich_vecteur;
    
    char optstring[] = ":i:o:k:f:m:v:l:h";
    int opt; 
    extern char *optarg;
    extern int optind, optopt;
    int vflag = 0, hflag = 0, kflag = 0;

    /* Datas processing */
    while((opt = getopt(argc, argv, optstring)) != -1)  
    {  
        switch(opt)  
        {  
            case 'i': 
            /* Input file*/
                fich_in = optarg;
                break; 
            case 'o': 
            /* Output file */
                fich_out = optarg;
                break; 
            case 'k':  
            /* Enter key for crypting */
                key = (unsigned char*)optarg;
                break;  
            case 'f':
                kflag = 1;
                nom = optarg;
                break;
            case 'm':  
            /* Select method */
                methode_crypt = optarg;
                break;  
            case 'v':
            /* vector cbc file */
                vflag = 1;
                fich_vecteur = optarg;
                break;
            case 'l': 
            /* Log file if needed */ 
        
            case 'h':  
            /* stdout for commands */
                hflag = 1;
                break;

            /* Default cases */
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  
  
    if (kflag == 1){
        /* Opening of the key-file */
        FILE* fich_key = fopen(nom, "rb");

        /* Check if the file opened correctly */
        if (fich_key == NULL){
            perror("erreur ouverture fichier clef ");
        }

        /* Search for the size of the key */
        fseek(fich_key, 0L, SEEK_END);
        int size = ftell(fich_key);
        rewind(fich_key);

        /* keys variables */ 
        unsigned char* key_fichier = (unsigned char*) malloc(size);
        key = (unsigned char*) malloc(size);

        /* read the file containing the key */
        if(fread(key_fichier,  sizeof( char ), size, fich_key) < size){
            perror("ouverture fichier clef");
        }

        memcpy(key, key_fichier, size);
    }
    
    /* extras arguments */
    for(; optind < argc; optind++){      
        printf("extra arguments: %s\n", argv[optind]);  
    } 

    /* Print the commands help in the terminal */
    if (hflag == 1){
        commandes_affichage();

    } else {
        /* pick the method asked by the user */
        printf("\n ~~~ Selection de la méthode %s ~~~\n", methode_crypt);

        if ( (strcmp(methode_crypt, "cbc-crypt") == 0) || (strcmp(methode_crypt, "cbc-decrypt") == 0) ){
            /* Check initialisating vector if present */
            if (vflag != 1) {
                fprintf(stderr,"\nErreur, manque le vecteur d'initialisation methode CBC\n");
                
            } else {
                /* CBC Methods */
                if ( strcmp(methode_crypt,"cbc-crypt") == 0 ){
                    /* CBC-crypting method */
                    if(cbc_crypt(fich_in, key, fich_vecteur, fich_out) == -1){
                        fprintf(stderr,"\nerreur cbc-crypt\n");
                    }

                } else {
                    /* CBC-uncrypting method */
                    if( cbc_decrypt(fich_in, key, fich_vecteur, fich_out) == -1){
                        fprintf(stderr,"\nerreur cbc-crypt\n");
                    }
                }
            }
        } else {
            /* XOR or MASK Methods */
            if (strcmp(methode_crypt, "xor") == 0){
                xor_fichier(fich_in, fich_out, key);
            } else {
                mask(fich_in, fich_out);
            }
        }
    }

    printf("\n--- Fin programme ---\n");

    /* End of the program */
    return 0;
}