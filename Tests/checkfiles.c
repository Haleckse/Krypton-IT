#include <stdio.h>
#include <stdlib.h>

// Définitions pour les couleurs ANSI
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

// Fonction pour comparer deux fichiers et indiquer s'ils sont identiques
void checkfiles(const char* file1, const char* file2) {
    FILE *f1 = fopen(file1, "rb");
    FILE *f2 = fopen(file2, "rb");

    if (f1 == NULL || f2 == NULL) {
        fprintf(stderr, "Erreur: Impossible d'ouvrir un des fichiers.\n");
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        exit(EXIT_FAILURE);
    }

    int ch1, ch2; // Utiliser int pour gérer EOF correctement
    int position = 0;
    int identical = 1; // Supposons que les fichiers sont identiques au départ

    while ((ch1 = fgetc(f1)) != EOF && (ch2 = fgetc(f2)) != EOF) {
        position++;
        if (ch1 != ch2) {
            identical = 0;
            break;
        }
    }

    // Vérifier si les fichiers ont des tailles différentes
    // if ((ch1 != EOF || ch2 != EOF)) {
    //     identical = 0;
    //     printf("Les fichiers ont des tailles différentes.\n");
    // }

    fclose(f1);
    fclose(f2);

    if (identical) {
        printf(GREEN "[OK] Les fichiers '%s' et '%s' sont identiques.\n", file1, file2);
    } else {
        printf(RED "[KO]  Les fichiers '%s' et '%s' sont différents.\n", file1, file2);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
        return EXIT_FAILURE;
    }

    checkfiles(argv[1], argv[2]);
    return EXIT_SUCCESS;
}
