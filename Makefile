# Répertoires
SRC_DIR = CodeC
OBJ_DIR = obj
DOC_DIR = Documents
DAT_DIR = Datas/Source
BIN_DIR = bin
CC = gcc
CFLAGS = -std=c99 -Wextra -Wall -Werror -pedantic 
LDFLAGS = -lm

ECHO = @
ifeq ($(VERBOSE),1)
    ECHO=
endif

ifeq ($(DEBUG),yes)
    CFLAGS += -g
    LDFLAGS +=
else
    CFLAGS += -O3 -DNDEBUG
    LDFLAGS +=
endif

# Nom de l'exécutable
EXEC = $(BIN_DIR)/sym_crypt

# Fichiers sources et objets (les objets sont placés dans obj/)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Cible par défaut : tout compiler
all: $(OBJ_DIR) $(BIN_DIR) $(EXEC)

# Création du répertoire obj si nécessaire
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Compilation de l'exécutable
$(EXEC): $(OBJ)
	$(ECHO)$(CC) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers objets dans obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(ECHO)$(CC) -o $@ -c $< $(CFLAGS)

# Nettoyage des fichiers objets
.PHONY: clean mrproper doc

clean:
	$(ECHO)rm -rf $(OBJ_DIR)/*.o
	rm -f $(EXEC) $(OBJ)

# Nettoyage complet : supprime aussi l'exécutable et la documentation
mrproper: clean
	$(ECHO)rm -rf $(BIN_DIR)/* $(DOC_DIR)/html $(OBJ_DIR)



# Dépendances spécifiques
$(OBJ_DIR)/chiffrage_cryptographie.o: $(SRC_DIR)/chiffrage_cryptographie.h
$(OBJ_DIR)/sym_crypt.o: $(SRC_DIR)/chiffrage_cryptographie.h
