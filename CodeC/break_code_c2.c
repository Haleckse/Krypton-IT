#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "break_code.h"

#define ASCII_PRINTABLE_START 32
#define ASCII_PRINTABLE_END 126

// Check si la chaîne de caractère "text" est bien composée de caractères ASCII
// Retourne 1 si valide, 0 sinon
//
int is_valid_text(const char *text, int length) {
    for (int i = 0; i < length; i++) {
        if ((text[i] < ASCII_PRINTABLE_START || text[i] > ASCII_PRINTABLE_END) && text[i] != '\n') {
            return 0; // Texte invalide
        }
    }
    return 1;
}   

// Lit l'intégralité des clefs candidates contenues dans le fichier "filename"
// Retrourne un tableau contenant toutes les clefs
//
char** read_key_candidates_from_file(char *filename, int key_length, int* total_candidate_length) {
    // Fichier contenant les clefs candidates
    FILE* file = fopen(filename, "rb");

    // Traitement erreur ouverture fichier 
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }

    // Taille totale du fichier 
    fseek(file, 0, SEEK_END);
    *total_candidate_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Nombre de clef(s) candidate(s)
    *total_candidate_length = *total_candidate_length/(key_length+1);
    printf("Nombres de clefs candidates : %d\n",*total_candidate_length);

    // Tableau qui contiendra l'ensemble des clefs candidates
    char** key_candidates = malloc(*total_candidate_length * sizeof(char*));
    if (!key_candidates) {
        perror("Erreur allocation mémoire clef candidates C2");
        fclose(file);
        return NULL;
    }

    // Remplissage du tableau
    for (int i = 0; i < *total_candidate_length; i++) {
        // Allocation m�moire pour chaque clef
        key_candidates[i] = malloc((key_length + 1) * sizeof(char)); // (+1) pour '\0'
        
        // Traitement erreurs et lecture des clefs
        if (!key_candidates[i] || fread(key_candidates[i], 1, key_length, file) != key_length) {
            perror("Erreur lecture clef candidate C2");
            fclose(file);
            return NULL;
        }

        // Termine la cha�ne par '\0'
        key_candidates[i][key_length] = '\0';
    }

    fclose(file);
    return key_candidates;
}

// Calcul la fréquence d'apparition de caractères alphabétiques dans un texte
// Tableau retourné dans la variable "freq"
//
void calculate_frequencies(const char *text, int length, double *frequence) {
    memset(frequence, 0, 26 * sizeof(double));
    int total_alpha = 0;

    for (int i = 0; i < length; i++) {
        if (isalpha(text[i])) {
            int index = tolower(text[i]) - 'a';
            frequence[index]++;
            total_alpha ++;
        }
    }

    // Normalize frequencies
    for (int i = 0; i < 26; i++) {
        if (total_alpha > 0) {
            frequence[i] /= total_alpha;
        }
    }
}

// Calcul la distance entre la fréquence globale et celle calculée à la fonction précédente
// Retourne cette distance calculée
//
double calculate_distance(const double *freq_th, const double *freq) {
    double distance = 0.0;
    for (int i = 0; i < 26; i++) {
        distance += pow(freq[i] - freq_th[i], 2);
    }
    return distance;
}

// Trie par ordre décroissant les différentes clefs 
// Retourne un tableau triée 
//
char** sort_ascending(char** keys, int num_keys) {
    int ind = 0;
    char** sorted_keys = malloc(sizeof(char) * num_keys);

    for (int i = (num_keys-1); i >= 0; i--) {
        sorted_keys[ind++] = keys[i];
    }

    return sorted_keys;
}

// Affiche sur STDOUT les différentes clefs possibles
//
void affichage_clefs(int index, char **potential_keys){
    for (int i = 0; i < index; i++){
        printf("%s", potential_keys[i]);
        if (i!= index-1)
            printf(", ");
    }
    printf("\n");
}

// Execute le crack C2
//
void break_code_c2(const char* cyphered_file, char* key_file, int key_length, const double* freq_th) {
    int num_candidates;
    double freq[26];
    int index = 0; int trouve = 0;
    double min_distance = INFINITY;
    char best_key[key_length];
    
    // Ouverture fichier input
    FILE* input_file = fopen(cyphered_file, "rb");
    if(!input_file){
        perror("ouverture fichier input c2");
        return;
    }

    // Taille du fichier input
    fseek(input_file, 0, SEEK_END);
    int text_length = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    // Lecture du fichier
    char *buffer = malloc(text_length * sizeof(char));
    if (fread(buffer, 1, text_length, input_file) != text_length){
        perror("erreur lecture C2");
        fclose(input_file);
        exit(EXIT_FAILURE);
    }
    fclose(input_file);

    // Recup�ration des clefs candidates
    char** key_candidates = read_key_candidates_from_file(key_file, key_length, &num_candidates);
    char *decrypted_text = malloc((text_length + 1) * sizeof(char));
    char** potential_keys = malloc(sizeof(char) * num_candidates);
    
    for (int k = 0; k < num_candidates; k++) {
        // XOR dechiffrement
        for(int i=0; i<text_length;i++){
            decrypted_text[i] = buffer[i] ^ key_candidates[k][i%key_length];
        }

        // Ajout du caractère de fin
        decrypted_text[text_length] = '\0';

        if (is_valid_text(decrypted_text, text_length)) {

            // Calculer la fréquences et la distance
            calculate_frequencies(decrypted_text, text_length, freq);
            double distance = calculate_distance(freq_th, freq);

            // Check si la distance est celle minimale
            if (distance < min_distance) {
                trouve = 1;
                min_distance = distance;

                // Debug
                //printf("clef : %s dist : %f\n", key_candidates[k], distance);
                
                // Ajout et sauvegarde de la meilleure clef
                strncpy(best_key, key_candidates[k], key_length);
                best_key[key_length] = '\0';

                // Ajout des clefs candidates possibles dans un dico
                potential_keys[index] = malloc(key_length+1);
                strncpy(potential_keys[index], best_key, key_length+1);
                index++;
            }
        }
    }

    // Une clef à été trouvée
    if (trouve) {
        printf("Meilleure clé : %s\n", best_key);
    
    // Aucune clef trouvée
    } else {
        printf("Pas de clef valide trouvée pour C2.\n");
    }

    // Libération mémoire et fermeture fichiers
    for (int i = 0; i < num_candidates; i++) {
        free(key_candidates[i]);
    }
    free(key_candidates);
    free(buffer);
    free(decrypted_text);

    //Debugging : Print distances and keys array before and after being sorted
    printf("\nClefs non-triées : ");
    affichage_clefs(index, potential_keys);

    potential_keys = sort_ascending(potential_keys, index);

    printf("Clefs triées : ");
    affichage_clefs(index, potential_keys);

    // Libération mémoire du dictionnaire
    for(int i=0; i<index; i++){
        free(potential_keys[i]);
    }
}

// int main(int argc, char* argv[]) {
//     if(argc != 1){
//         perror("Manque des arguments");
//         exit(EXIT_FAILURE);
//     }

//     char* input = "1234_msg2.txt";
//     int key_length = 4;

//     printf("~~~ Running C2 Attack... ~~~\n\n");

//     break_code_c2(input, key_length, french_fq);

//     return 0;
// }
