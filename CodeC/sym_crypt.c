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
    char* fich_in; char* fich_out;
    char* methode_crypt; 
    char* fich_vecteur;
    FILE* fich_key;
    
    char optstring[] = ":i:o:k:f:m:v:l:h:";
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
                fich_key = fopen(optarg, "r");
                if(fread(key, sizeof( char ), 512, fich_key) <= 0){
                    perror("ouverture fichier clef");
                }
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

    //printf("cle : %s\n", key);
  
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

        if ( (methode_crypt = "cbc-crypt")|| (methode_crypt = "cbc-decrypt") ){
            /* Check initialisating vector if present */
            if (vflag != 1) {
                fprintf(stderr,"\nErreur, manque le vecteur d'initialisation methode CBC\n");
                
            } else {
                /* CBC Methods */
                if (methode_crypt = "cbc-crypt"){
                    /* CBC-crypting method */
                    if(cbc_crypt(fich_in, key, fich_vecteur, fich_out) == -1){
                        fprintf(stderr,"\nerreur cbc-crypt\n");
                    }

                } else {
                    // fonction
                }
            }
        } else {
            /* XOR or MASK Methods */
            if (methode_crypt = "xor"){
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