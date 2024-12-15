#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void break_code_c1(const unsigned char* message, size_t message_len, size_t clef_len, const char* output_filename) {
    char clef_candidates[clef_len][MAX_CLEFS];
    
    for (size_t i = 0; i < clef_len; i++) {
        for (size_t j = 0; j < MAX_CLEFS; j++) {
            clef_candidates[i][j] = caracteres_possibles[j];
        }
    }

    for (size_t i = 0; i < clef_len; i++) {
        for (size_t j = 0; j < message_len; j += clef_len) {
            unsigned char decrypted_char = message[j] ^ clef_candidates[i][j % clef_len];
            if (!est_valide(decrypted_char)) {
                clef_candidates[i][j % clef_len] = '\0';
            }
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

int main(int argc, char *argv[]) {
    unsigned char message[] = "\"s(/1& !\"\"s(/1& !\"\"s(/1& !\"";
    size_t message_len = strlen((char*)message);
    size_t clef_len = 3;
    const char* output_filename = "key_candidates.bin";

    printf("Running C1 attack...\n");
    break_code_c1(message, message_len, clef_len, output_filename);
    return 0;
}