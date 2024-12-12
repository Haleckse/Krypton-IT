#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "dh_prime.h"

#define MIN 1000000
#define MAX 10000000

// Affiche l'aide pour l'utilisation de dh_gen_group
void afficher_aide() {
    printf("Usage: dh_gen_group [-o <fichier>] [-h]\n");
    printf("Options:\n");
    printf("  -o <fichier> : spécifie le fichier de sortie pour p et g.\n");
    printf("  -h           : affiche cette aide.\n");
}

int main(int argc, char *argv[]) {
    char *nom_fichier = NULL;
    int cpt;
    long min = MIN, max = MAX; // Plage d'exemple pour la génération du nombre premier

    // Parsing des arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            nom_fichier = argv[++i];
        } else if (strcmp(argv[i], "-h") == 0) {
            afficher_aide();
            return 0;
        } else {
            fprintf(stderr, "Option inconnue : %s\n", argv[i]);
            afficher_aide();
            return 1;
        }
    }

    // Vérification que le fichier de sortie est bien spécifié
    if (!nom_fichier) {
        fprintf(stderr, "Erreur: le fichier de sortie doit être spécifié avec l'option -o.\n");
        afficher_aide();
        return 1;
    }

    // Génération du nombre premier de Sophie Germain et du générateur
    long p = genPrimeSophieGermain(min, max, &cpt);
    long g = seek_generator(2, p); // Cherche un générateur dans Z/pZ à partir de 2
    assert(g != -1); // Vérifie que le générateur a bien été trouvé

    // Écriture des résultats dans le fichier spécifié

    char path[256]; // Ensure it has enough space for the result
    snprintf(path, sizeof(path), "Output/%s", nom_fichier);

    FILE *fichier = fopen(path, "w");
    if (!fichier) {
        perror("Erreur d'ouverture du fichier de sortie");
        return 1;
    }
    fprintf(fichier, "p = %ld\n", p);
    fprintf(fichier, "g = %ld\n", g);
    fclose(fichier);

    printf("Nombre premier de Sophie Germain (p) et générateur (g) générés et enregistrés dans %s.\n", nom_fichier);

    // execution direct du programme python dh_genkey.py
    if (system("python3 CodeC/dh_genkey.py") == -1){
        perror("erreur redirection programme python dh_gen_group.c");
    }

    return 0;
}
