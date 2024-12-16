#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction permettant la réalisation du crack via le masque
// 
void crack_mask(const char *chif1_file, const char *chif2_file, const char *test_file, const char *output_file) {
    // Ouverture des différents fichiers
    FILE *f1 = fopen(chif1_file, "r");
    FILE *f2 = fopen(chif2_file, "r");
    FILE *ftest = fopen(test_file, "r");

    // Vérification de la bonne ouverture
    if (!f1 || !f2 || !ftest) {
        fprintf(stderr, "Erreur lors de l'ouverture des fichiers.\n");
        exit(1);
    }

    // Cherche la taille de f1
    fseek(f1, 0, SEEK_END);
    long len1 = ftell(f1);
    fseek(f1, 0, SEEK_SET);
    char *chif1 = malloc(len1 + 1);

    // Lecture du fichier f1
    if (fread(chif1, 1, len1, f1) < len1){
        fprintf(stderr, "erreur fread chif1");
    }
    chif1[len1] = '\0';

    // Cherche la taille de f2
    fseek(f2, 0, SEEK_END);
    long len2 = ftell(f2);
    fseek(f2, 0, SEEK_SET);
    char *chif2 = malloc(len2 + 1);

    // Lecture du fichier f2
    if (fread(chif2, 1, len2, f2) < len2){
        fprintf(stderr, "erreur fread chif2");
    }
    chif2[len2] = '\0';

    // Cherche la taille de ftest
    fseek(ftest, 0, SEEK_END);
    long len_test = ftell(ftest);
    fseek(ftest, 0, SEEK_SET);
    char *test = malloc(len_test + 1);

    // Lecture du fichier ftest
    if (fread(test, 1, len_test, ftest) < len_test){
        fprintf(stderr, "erreur fread test");
    }
    test[len_test] = '\0';

    // Check si la taille des fichiers est uniforme
    if (len1 != len2 || len1 != len_test) {
        fprintf(stderr, "Les fichiers n'ont pas la même taille.\n");
        exit(1);
    }

    char *decrypted_message = malloc(len1 + 1);
    for (long i = 0; i < len1; i++) {
        decrypted_message[i] = chif1[i] ^ chif2[i];
    }
    decrypted_message[len1] = '\0';

    if (strcmp(decrypted_message, test) == 0) {
        printf("Le message a été correctement décrypté.\n");
    } else {
        printf("Le message décrypté ne correspond pas à l'exemple de test.\n");
    }

    // Ouverture fichier Output
    FILE *fout = fopen(output_file, "w");
    if (!fout) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier de sortie.\n");
        exit(1);
    }
    // Ecriture dans fout
    fwrite(decrypted_message, 1, len1, fout);

    // Fermeture des fichiers et libération mémoire
    fclose(f1);
    fclose(f2);
    fclose(ftest);
    fclose(fout);
    free(chif1);
    free(chif2);
    free(test);
    free(decrypted_message);
}

// int main(int argc, char *argv[]) {
//     if (argc != 5) {
//         fprintf(stderr, "Erreur d'arguments.\n");
//         return 1;
//     }

//     const char *chif1_file = argv[1];
//     const char *chif2_file = argv[2];
//     const char *test_file = argv[3];
//     const char *output_file = argv[4];

//     crack_mask(chif1_file, chif2_file, test_file, output_file);
//     return 0;
// }
