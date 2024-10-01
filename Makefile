CC=gcc
CFLAGS=-std=c99 -pedantic
LDFLAGS=-lm 


ifeq ($(DEBUG),yes)
	CFLAGS += -g
	LDFLAGS +=
else
	CFLAGS += -O3 -DNDEBUG
	LDFLAGS +=
endif

EXEC= main
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o
	@rm -f bin/* $(FILESDIR)/out.*

mrproper: clean
	@rm -rf $(EXEC) documentation/html *.dot *.pdf

chiffrage.o : chiffrage_cryptographie.h
main.o : chiffrage.o main_chiffrage.o

DELIVER_FMT=$(shell date "+$(shell id -un)_%d-%m-%y_%Hh%Mm%Ss")

deliver:
	zip -r $(DELIVER_FMT).zip .

