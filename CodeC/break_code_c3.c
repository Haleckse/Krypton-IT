#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_LINE_LENGTH 256


int charger_dictionnaire(const char* fichier, char dictionnaire[][MAX_WORD_LENGTH], int max_mots) {
    FILE *file = fopen(fichier, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier de dictionnaire");
        return 0;
    }
    
    int i = 0;
    while (fgets(dictionnaire[i], MAX_WORD_LENGTH, file) != NULL && i < max_mots) {
        dictionnaire[i][strcspn(dictionnaire[i], "\n")] = '\0';
        for (int j = 0; dictionnaire[i][j]; j++) {
            dictionnaire[i][j] = tolower(dictionnaire[i][j]);
        }
        i++;
    }
    fclose(file);
    return i; 
}

int est_valide_c3(const char* mot, char dictionnaire[][MAX_WORD_LENGTH], int nombre_mots) {
    for (int i = 0; i < nombre_mots; i++) {
        if (strcmp(mot, dictionnaire[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void decoder_cesar(const char* texte, char* texte_decode, int clef) {
    for (int i = 0; texte[i] != '\0'; i++) {
        if (isalpha(texte[i])) {
            char base = (islower(texte[i])) ? 'a' : 'A';
            texte_decode[i] = (texte[i] - base - clef + 26) % 26 + base;
        } else {
            texte_decode[i] = texte[i];
        }
    }
}

int separer_mots(const char* texte, char mots[][MAX_WORD_LENGTH]) {
    int i = 0;
    const char* start = texte;
    while (*start) {
        while (*start && !isalpha(*start)) start++;
        if (*start == '\0') break;

        const char* end = start;
        while (isalpha(*end)) end++;

        int len = end - start;
        strncpy(mots[i], start, len);
        mots[i][len] = '\0';
        i++;
        start = end;
    }
    return i;
}

void break_code_c3(){
    char dictionnaire[83500][MAX_WORD_LENGTH];
    int nombre_mots = charger_dictionnaire("dicoFrSA.txt", dictionnaire, 83500);
    if (nombre_mots == 0) {
        printf("Erreur : aucun mot chargé dans le dictionnaire.\n");
    }

    const char* texte_crypted = "message à décoder";
    int clefs[] = {1, 3, 5};
    int n_clefs = sizeof(clefs) / sizeof(clefs[0]);

    for (int j = 0; j < n_clefs; j++) {
        char texte_decode[MAX_LINE_LENGTH];
        decoder_cesar(texte_crypted, texte_decode, clefs[j]);

        char mots[MAX_LINE_LENGTH][MAX_WORD_LENGTH];
        int n_mots = separer_mots(texte_decode, mots);

        int score = 0;
        for (int i = 0; i < n_mots; i++) {
            for (int k = 0; mots[i][k]; k++) {
                mots[i][k] = tolower(mots[i][k]);
            }
            if (est_valide_c3(mots[i], dictionnaire, nombre_mots)) {
                score++;
            }
        }
        printf("Clé: %d, Score: %d\n", clefs[j], score);
    }
}
