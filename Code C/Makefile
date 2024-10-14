CC=gcc
CFLAGS=-std=c99 -Wextra -Wall -Werror -pedantic 
LDFLAGS=-lm

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

EXEC=sym_crypt
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: 
ifeq ($(DEBUG),yes)
	@echo "Generating in debug mode"
else
	@echo "Generating in release mode"
endif
	@$(MAKE) $(EXEC)

$(EXEC): $(OBJ)
	$(ECHO)$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(ECHO)$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	$(ECHO)rm -rf *.o

mrproper: clean
	$(ECHO)rm -rf $(EXEC) documentation/html

doc: stack.h
	$(ECHO)doxygen documentation/TP2
	
chiffrage_cryptographie.o: chiffrage_cryptographie.h 

sym_crypt.o: chiffrage_cryptographie.h