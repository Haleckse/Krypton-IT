#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define ASCII_PRINTABLE_START 32
#define ASCII_PRINTABLE_END 126

// Fréquences de lettres pour la langue française
const double french_freq[26] = {
    0.1467, 0.0764, 0.0326, 0.0367, 0.1470, 0.0107, 0.0087, 0.0074, 
    0.0753, 0.0061, 0.0005, 0.0546, 0.0297, 0.0701, 0.0574, 0.0253, 
    0.0136, 0.0669, 0.0793, 0.0724, 0.0631, 0.0184, 0.0003, 0.0044, 
    0.0030, 0.0012
};

// Vérifie si le texte est valide (composé de caractères ASCII imprimables ou d'un saut de ligne)
int is_valid_text(const char *text, int length) {
    for (int i = 0; i < length; i++) {
        if ((text[i] < ASCII_PRINTABLE_START || text[i] > ASCII_PRINTABLE_END) && text[i] != '\n') {
            return 0; // Texte invalide
        }
    }
    return 1;
}

// Lit les clés candidates depuis un fichier
char** read_key_candidates_from_file(const char *filename, int key_length, int* num_candidates) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur d'ouverture du fichier des clés candidates");
        return NULL;
    }

    // Taille totale du fichier
    fseek(file, 0, SEEK_END);
    int total_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Calcul du nombre de clés candidates
    *num_candidates = total_length / (key_length + 2); // (+2) pour ',' et '\n'
    char** key_candidates = malloc(*num_candidates * sizeof(char*));
    if (!key_candidates) {
        perror("Erreur d'allocation mémoire pour les clés candidates");
        fclose(file);
        return NULL;
    }

    // Lecture des clés
    for (int i = 0; i < *num_candidates; i++) {
        key_candidates[i] = malloc((key_length + 1) * sizeof(char)); // (+1) pour '\0'
        if (!key_candidates[i] || fread(key_candidates[i], 1, key_length, file) != key_length) {
            perror("Erreur lors de la lecture d'une clé");
            fclose(file);
            return NULL;
        }
        key_candidates[i][key_length] = '\0'; // Terminer par '\0'
        fseek(file, 2, SEEK_CUR); // Sauter ',' et '\n'
    }

    fclose(file);
    return key_candidates;
}

// Calcule les fréquences de lettres dans le texte
void calculate_frequencies(const char *text, int length, double *freq) {
    memset(freq, 0, 26 * sizeof(double));
    int total_alpha = 0;

    for (int i = 0; i < length; i++) {
        if (isalpha(text[i])) {
            int index = tolower(text[i]) - 'a';
            freq[index]++;
            total_alpha++;
        }
    }

    // Normaliser les fréquences
    for (int i = 0; i < 26; i++) {
        if (total_alpha > 0) {
            freq[i] /= total_alpha;
        }
    }
}

// Calcule la distance quadratique entre deux distributions de fréquences
double calculate_distance(const double *freq_th, const double *freq) {
    double distance = 0.0;
    for (int i = 0; i < 26; i++) {
        distance += pow(freq[i] - freq_th[i], 2);
    }
    return distance;
}

// Function to sort distances and keys arrays in ascending order

// Déchiffre le message chiffré avec XOR
void break_code_c2(const char* cyphered_file, int key_length, const double* freq_th) {
    int num_candidates;
    int index = 0;
    double freq[26];
    double min_distance = INFINITY;

    // Lecture du fichier chiffré
    FILE* input_file = fopen(cyphered_file, "rb");
    if (!input_file) {
        perror("Erreur d'ouverture du fichier chiffré");
        return;
    }

    fseek(input_file, 0, SEEK_END);
    int text_length = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    char *buffer = malloc(text_length);
    fread(buffer, 1, text_length, input_file);
    fclose(input_file);

    // Lecture des clés candidates
    char **key_candidates = read_key_candidates_from_file("key_candidates.bin", key_length, &num_candidates);
    if (!key_candidates) {
        free(buffer);
        return;
    }

    printf("nombres de clefs : %d\n", num_candidates);

    char* best_key = malloc(key_length * sizeof(double));
    char* decrypted_text = malloc(text_length + 1);

    // Déchiffrement et analyse
    for (int k = 0; k < num_candidates; k++) {
        for (int i = 0; i < text_length; i++) {
            decrypted_text[i] = buffer[i] ^ key_candidates[k][i % key_length];
        }
        decrypted_text[text_length] = '\0';

        if (is_valid_text(decrypted_text, text_length)) {
            calculate_frequencies(decrypted_text, text_length, freq);
            double distance = calculate_distance(freq_th, freq);
            if(distance < min_distance){
                min_distance = distance;
                best_key = key_candidates[k];
            }
        }
    }

    // Trier les clés candidates
    //sort_candidates(key_candidates, distances, num_candidates);

    // Afficher les résultats
    printf("Meilleure clé : %s (distance : %f)\n", best_key, min_distance);

    // Libérer la mémoire
    free(buffer);
    free(decrypted_text);
    for (int i = 0; i < num_candidates; i++) {
        free(key_candidates[i]);
    }
    free(key_candidates);
    free(best_key);
}

int main() {
    int key_length = 4;
    break_code_c2("1234_msg2.txt", key_length, french_freq);
    return 0;
}
