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
    
    printf("--- début programme ---\n");

    /* Variables nécessaire au programme*/
    unsigned char* key; 
    char* fich_in; char* fich_out;
    char* methode_crypt; char* fich_vecteur;
    
    char optstring[] = ":i:o:k:f::m:v::l::h::";
    int opt; int vflag = 0;
    extern char *optarg;
    extern int optind, optopt;

    /* Traitement des données en Entrée */
    while((opt = getopt(argc, argv, optstring)) != -1)  
    {  
        switch(opt)  
        {  
            case 'i': 
                fich_in = optarg;
                break; 
            case 'o': 
                fich_out = optarg;
                break; 
            case 'k':  
                key = (unsigned char*)optarg;
                break;  
            case 'f':
            case 'm':  
                methode_crypt = optarg;
                break;  
            case 'v':
                vflag = 1;
                fich_vecteur = optarg;
            case 'l':  
            case 'h':  
            
            /* Default cases */
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  
  
    /* extras arguments */
    for(; optind < argc; optind++){      
        printf("extra arguments: %s\n", argv[optind]);  
    } 

    /* Choix de la méthode de chiffrement/déchiffrement */
    printf("\n ~~~ Selection de la méthode %s ~~~\n", methode_crypt);

    if ( methode_crypt == "cbc-crypt" || methode_crypt == "cbc-decrypt"){
        /* Check vecteur initialisation present */
        if (vflag != 1) {
            fprintf(stderr,"Erreur, manque le vecteur d'initialisation methode CBC");
            exit(EXIT_FAILURE);
        } else {
            /* Methodes CBC*/
            if (methode_crypt == "cbc-crypt"){
                // fonction
            } else {
                // fonction
            }
        }
    } else {
        /* Methodes XOR ou MASK*/
        if (methode_crypt = "xor"){
            xor_fichier(fich_in, fich_out, key);
        } else {
            mask(fich_in, fich_out);
        }
    }

    printf("\n--- Fin programme ---\n");

    return 0;
}