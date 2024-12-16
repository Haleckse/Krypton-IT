#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "break_code.h"

#define MAX_CLEFS 62

const char caracteres_possibles[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int est_valide(char c) {
    return (isalnum(c) || ispunct(c) || c == ' ');
}

void decrypt_message(const unsigned char* message, size_t message_len, const char* clef, size_t clef_len) {
    for (size_t i = 0; i < message_len; i++) {
        unsigned char decrypted_char = message[i] ^ clef[i % clef_len];
        printf("%c", decrypted_char);
    }
    printf("\n");
}

void break_code_c1(const char* infile, size_t clef_len, const char* output_filename) {
    char clef_candidates[clef_len][MAX_CLEFS];
    int keyCar = true;
    int position = 0; int valXor;
    char* buffer = malloc(clef_len * sizeof(char));
    
    // Ouverture du fichier
    FILE* input = fopen(infile, "rb");
    if (!input){
        fprintf(stderr,"\nerreur lors de l'ouverture d'un fichier\n");
        exit(EXIT_FAILURE);
    } 

    for (size_t i = 0; i < clef_len; i++) {
        memcpy(clef_candidates[i], caracteres_possibles, MAX_CLEFS);
    }

    for (int pos = 0; pos < clef_len; pos++){
        for (int i=0; i<MAX_CLEFS; i++){
            while( (keyCar) && (fread(buffer, 1, clef_len, input) > 0) ){
                // Xor les caracteres
                valXor = buffer[pos] ^ clef_candidates[pos][i];

                // Si mauvais retour -> keyCar = 0 
                if (!est_valide(valXor))
                    keyCar = false;
            }

            if (!keyCar){
                // Suppression de l'element 
                for (int j = i - 1; j < strlen(clef_candidates[pos]); j++)
                    clef_candidates[pos][j] = clef_candidates[pos][j+1];
            }

            keyCar = true;
            rewind(input);
        }
    }

    FILE *file = fopen(output_filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    for (size_t i = 0; i < clef_len; i++) {
        fprintf(file, "%zu,", i);
        int first = 1;
        for (size_t j = 0; j < MAX_CLEFS; j++) {
            if (clef_candidates[i][j] != '\0') {
                if (!first) {
                    fprintf(file, ",");
                }
                fprintf(file, "%c", clef_candidates[i][j]);
                first = 0;
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Fichier de clés candidates généré : %s\n", output_filename);
}

// int main(int argc, char *argv[]) {
//     size_t clef_len = 3;
//     const char* output_filename = "key_candidates.bin";

//     printf("Running C1 attack...\n");
//     break_code_c1("message.txt", clef_len, output_filename);
//     return 0;
// }
