# Répertoires
SRC_DIR = CodeC
OBJ_DIR = obj
DOC_DIR = Documents
DAT_DIR = Datas/Source
BIN_DIR = bin
OUT_DIR = Output
CC = gcc
CFLAGS = -std=c99 -pedantic 
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

# Noms des exécutables
EXEC1 = $(BIN_DIR)/sym_crypt
EXEC2 = $(BIN_DIR)/dh_gen_group
EXEC3 = $(BIN_DIR)/main
EXEC4 = $(BIN_DIR)/break_code

# Fichiers sources spécifiques pour chaque exécutable
SRC1 = $(SRC_DIR)/sym_crypt.c $(SRC_DIR)/chiffrage_cryptographie.c
SRC2 = $(SRC_DIR)/dh_gen_group.c $(SRC_DIR)/dh_prime.c
SRC3 = $(SRC_DIR)/main_projet.c $(SRC_DIR)/chiffrage_cryptographie.c
SRC4 = $(SRC_DIR)/break_code.c $(SRC_DIR)/break_code_c1.c $(SRC_DIR)/break_code_c2.c $(SRC_DIR)/break_code_c3.c $(SRC_DIR)/break_code_masque.c

# Fichiers objets associés à chaque exécutable
OBJ1 = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC1))
OBJ2 = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC2))
OBJ3 = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC3))
OBJ4 = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC4))

# Cible par défaut : tout compiler
all: $(OBJ_DIR) $(BIN_DIR) $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4)

# Création du répertoire obj si nécessaire
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Compilation des exécutables
$(EXEC1): $(OBJ1)
	$(ECHO)$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC2): $(OBJ2)
	$(ECHO)$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC3): $(OBJ3)
	$(ECHO)$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC4): $(OBJ4)
	$(ECHO)$(CC) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers objets dans obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(ECHO)$(CC) -o $@ -c $< $(CFLAGS)

# Nettoyage des fichiers objets
.PHONY: clean mrproper doc

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/*
	rm -rf $(OUT_DIR)/*
	
# Nettoyage complet : supprime aussi les exécutables et la documentation
mrproper: clean
	$(ECHO)rm -rf $(BIN_DIR)/* $(DOC_DIR)/html $(OBJ_DIR)

# Dépendances spécifiques
$(OBJ_DIR)/chiffrage_cryptographie.o: $(SRC_DIR)/chiffrage_cryptographie.h
$(OBJ_DIR)/dh_gen_group.o: $(SRC_DIR)/dh_prime.h
$(OBJ_DIR)/break_code.o: $(SRC_DIR)/break_code.h