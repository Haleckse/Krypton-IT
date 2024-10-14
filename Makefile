# Répertoires
SRC_DIR = Code_C
OBJ_DIR = src
BIN_DIR = bin

# Fichiers source et objets
SRC_FILES = $(wildcard "$(SRC_DIR)/*.c")
OBJ_FILES = $(patsubst "$(SRC_DIR)/%.c", "$(OBJ_DIR)/%.o", $(SRC_FILES))

# Nom de l'exécutable
TARGET = $(BIN_DIR)/main

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

# Cible par défaut
all: $(TARGET)

# Règle pour lier les fichiers objets et créer l'exécutable
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)  # Crée le dossier bin s'il n'existe pas
	$(CC) $(LDFLAGS) $^ -o $@

# Règle pour compiler les fichiers .c en .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)  # Crée le dossier src s'il n'existe pas
	$(CC) $(CFLAGS) -c $< -o $@

# Cible pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(TARGET)

# Cible pour supprimer tous les fichiers générés
fclean: clean

# Cible pour recompiler complètement
re: fclean all

.PHONY: all clean fclean re


