#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define ASCII_PRINTABLE_START 32
#define ASCII_PRINTABLE_END 126

int is_valid_text(const char *text, int length) {
    for (int i = 0; i < length; i++) {
        if (text[i] < ASCII_PRINTABLE_START || text[i] > ASCII_PRINTABLE_END) {
            return 0;
        }
    }
    return 1;
}   

char **read_key_candidates_from_file(const char *filename, int key_length, int *num_candidates) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }

    char **key_candidates = malloc(sizeof(char *) * key_length);
    for (int i = 0; i < key_length; i++) {
        key_candidates[i] = malloc(ASCII_PRINTABLE_END - ASCII_PRINTABLE_START + 1);
    }

    int line_number = 0;
    while (line_number < key_length) {
        for (int i = 0; i < (ASCII_PRINTABLE_END - ASCII_PRINTABLE_START + 1); i++) {
            unsigned char byte;
            if (fread(&byte, sizeof(unsigned char), 1, file) != 1) {
                break;
            }
            key_candidates[line_number][i] = byte;
        }
        line_number++;
    }

    fclose(file);
    *num_candidates = line_number;
    return key_candidates;
}

const double FREQ_TH_FR[26] = {0.1467, 0.0764, 0.0326, 0.0367, 0.1470, 0.0107, 0.0087, 0.0074, 
                               0.0753, 0.0061, 0.0005, 0.0546, 0.0297, 0.0701, 0.0574, 0.0253, 
                               0.0136, 0.0669, 0.0793, 0.0724, 0.0631, 0.0184, 0.0003, 0.0044, 
                               0.003, 0.0012};

void calculate_frequencies(const char *text, int length, double *freq) {
    int count[26] = {0};
    int total = 0;

    for (int i = 0; i < length; i++) {
        if (isalpha(text[i])) {
            count[tolower(text[i]) - 'a']++;
            total++;
        }
    }

    for (int i = 0; i < 26; i++) {
        freq[i] = (total > 0) ? (double)count[i] / total : 0.0;
    }
}

double calculate_distance(const double *freq_th, const double *freq) {
    double distance = 0.0;
    for (int i = 0; i < 26; i++) {
        distance += pow(freq_th[i] - freq[i], 2);
    }
    return distance;
}

void generate_dictionnary(const char *filename, char **key_candidates, int key_length, int num_candidates) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur d'ouverture du fichier de dictionnaire");
        return;
    }

    for (int i = 0; i < num_candidates; i++) {
        for (int j = 0; j < (ASCII_PRINTABLE_END - ASCII_PRINTABLE_START + 1); j++) {
            fprintf(file, "%c", key_candidates[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void break_code_c2(const char *ciphered_text, int text_length, int key_length, const double *freq_th) {
    char *best_key = malloc((key_length + 1) * sizeof(char));
    double min_distance = INFINITY;
    int num_candidates;

    char **key_candidates = read_key_candidates_from_file("key_candidates.bin", key_length, &num_candidates);

    for (int k = 0; k < key_length; k++) {
        double current_distance = 0.0;
        char decrypted_text[text_length + 1];

        for (int i = 0; i < text_length; i++) {
            decrypted_text[i] = ciphered_text[i] ^ key_candidates[k][i % key_length];
        }
        decrypted_text[text_length] = '\0';

        double freq[26] = {0.0};
        calculate_frequencies(decrypted_text, text_length, freq);

        current_distance = calculate_distance(freq_th, freq);

        if (current_distance < min_distance) {
            min_distance = current_distance;
            strncpy(best_key, key_candidates[k], key_length);
        }
    }

    best_key[key_length] = '\0';

    printf("Meilleure clé : %s\n", best_key);

    generate_dictionnary("dictionnaire.txt", key_candidates, key_length, num_candidates);
}


// int main() {
//     const char *ciphered_text = "\x4f\x52\x53\x51";
//     int text_length = strlen(ciphered_text);
//     int key_length = 3;

//     break_code_c2(ciphered_text, text_length, key_length, FREQ_TH_FR);


//     // for (int i = 0; i < key_length; i++) {
//     //     free(candidates[i]);
//     // }
//     // free(candidates);
//     // free(best_key);

//     return 0;
// }
