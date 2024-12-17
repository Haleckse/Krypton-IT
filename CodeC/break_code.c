#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "break_code.h"

/* ============================================================================== */
/* ============================================================================== */

// double english_freq[26] = {
//     0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061,
//     0.070, 0.0015, 0.0077, 0.040, 0.024, 0.067, 0.075, 0.019,
//     0.00095, 0.060, 0.063, 0.090, 0.028, 0.0098, 0.024, 0.0015,
//     0.020, 0.00074
// };

const double french_fq[26] = {
    0.1467, 0.0764, 0.0326, 0.0367, 0.1470, 0.0107, 0.0087, 0.0074, 
    0.0753, 0.0061, 0.0005, 0.0546, 0.0297, 0.0701, 0.0574, 0.0253, 
    0.0136, 0.0669, 0.0793, 0.0724, 0.0631, 0.0184, 0.0003, 0.0044, 
    0.003, 0.0012
};

// Main de la partie Crack
//
int main(int argc, char* argv[]){
    
    /* Necessary variables for the program */
    unsigned int key;
    char* fich_in = NULL;
    const char* output_filename = NULL;
    char* methode_crypt = NULL; 
    char* dico = NULL;
    
    char optstring[] = ":i:o:m:k:d:l:h";
    int opt; 
    extern char *optarg;
    extern int optind, optopt;
    int hflag = 0, dflag = 0;

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
            /* Input file*/
                output_filename = optarg;
                break;

            case 'm': 
            /* Select method */
                methode_crypt = optarg;
                break;  

            case 'k':  
            /* Enter key for crypting */
                key = atoi(optarg);
                break;  

            case 'd':
                dflag = 1;
                dico = optarg;
                break;

            case 'l':  
            
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

    // Verification bon paramètres
    if (!key || !fich_in || !output_filename || !methode_crypt){
        perror("Il manque des arguments");
        exit(EXIT_FAILURE);
    }

    // Execute le crack C1
    if (strcmp(methode_crypt, "c1") == 0){

        printf("~~~ Running C1 Attack... ~~~\n\n");

        break_code_c1(fich_in, key, output_filename);

    // Realise l'ensemble des 3 cracks
    } else if (strcmp(methode_crypt, "all") == 0){
        
        printf("~~~ Running C1 Attack... ~~~\n\n");
        break_code_c1(fich_in, key, output_filename);

        printf("~~~ Running C2 Attack... ~~~\n\n");
        break_code_c2(fich_in, key, french_fq);

        printf("~~~ Running C3 Attack... ~~~\n\n");
        //break_code_c2(fich_in, key, french_fq);
    } else {
        fprintf(stderr,"Erreur, la méthode choisie n'existe pas.\n");
    }

    /* End of the program */
    return 0;
}