#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "break_code.h"

/* ============================================================================== */
/* ============================================================================== */


// Main de la partie Crack
//
int main(int argc, char* argv[]){
    
    /* Necessary variables for the program */
    size_t key;
    char* fich_in;
    char* methode_crypt; 
    char* dico;
    
    char optstring[] = ":i:m:k:d:l:h";
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

    if (strcmp(methode_crypt, "c1") == 0){

        const char* output_filename = "key_candidates.bin";

        break_code_c1(fich_in, key,output_filename);
    }

    /* End of the program */
    return 0;
}