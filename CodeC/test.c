#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void delete_key(char* input_file, char* delete_key) {
    // Fichiers temporaire et source
    FILE *file = fopen(input_file, "r");
    FILE *temp_file = fopen("temp.txt", "w");

    // Vérification des erreurs d'ouverture des fichiers
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        return;
    }
    if (!temp_file) {
        perror("Erreur lors de la création du fichier temporaire");
        fclose(file);
        return;
    }

    char line[256]; // Buffer pour lire chaque ligne

    // Lire le fichier ligne par ligne
    while (fgets(line, sizeof(line), file)) {
        // Supprimer les caractères de fin de ligne (si présents)
        line[strcspn(line, "\r\n")] = '\0';

        // Comparer la clé actuelle avec la clé à supprimer
        if (strcmp(line, delete_key) != 0) {
            // Si la clé n'est pas celle à supprimer, l'écrire dans le fichier temporaire
            fprintf(temp_file, "%s\n", line);
        }
    }

    // Fermer les fichiers
    fclose(file);
    fclose(temp_file);

    // Remplacer le fichier original par le fichier temporaire
    if (remove(input_file) != 0) {
        perror("Erreur lors de la suppression du fichier original");
        return;
    }
    if (rename("temp.txt", input_file) != 0) {
        perror("Erreur lors du renommage du fichier temporaire");
        return;
    }

    printf("Clé '%s' supprimée avec succès du fichier '%s'.\n", delete_key, input_file);
}


int main() {
    char* filename = "key_log.txt";
    char* key_to_delete = "hQli";

    delete_key(filename, key_to_delete);

    return 0;
}
