#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

// Mock for your missing header, adjust as needed
#include "../CodeC/chiffrage_cryptographie.h"

#define MAX_PATH_LEN 256
#define MAX_CMD_LEN 512
#define NUM_FILES 1

#define BLUE   "\033[0;34m"     // Bleu standard
#define ORANGE "\033[38;5;214m" // Approximation d'orange (palette étendue 256 couleurs)
#define VIOLET "\033[0;35m"     // Violet standard
#define RESET  "\033[0m"        // Réinitialisation des couleurs

void test_xor(); 
void test_cbc(); 


int main(void) {

    test_xor(); 
    test_cbc(); 
 }

 void test_xor() {
    printf("\n----------Tests des fonction de cryptage et de decryptage par la methode xor----------\n\n"); 

    printf(ORANGE"Cryptage de chaton.jpg...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Datas/Source/chaton.jpg -o ../Output/chaton_crypte_xor.jpg -m xor -f ../key.txt"); 
    printf(BLUE"Decryptage de l'output obtenu...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Output/chaton_crypte_xor.jpg -o ../Output/chaton_decrypte_xor.jpg -m xor -f ../key.txt"); 
    system("./checkfiles ../Datas/Source/chaton.jpg ../Output/chaton_decrypte_xor.jpg"); 

    printf(ORANGE"Cryptage de flower.jpg...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Datas/Source/flower.jpg -o ../Output/flower_crypte_xor.jpg -m xor -f ../key.txt"); 
    printf(BLUE"Decryptage de l'output obtenu...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Output/flower_crypte_xor.jpg -o ../Output/flower_derypte_xor.jpg -m xor -f ../key.txt"); 
    system("./checkfiles ../Datas/Source/flower.jpg ../Output/flower_derypte_xor.jpg"); 

    printf(ORANGE"Cryptage de ring.txt...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Datas/Source/ring.txt -o ../Output/ring_crypte_xor.txt -m xor -f ../key.txt"); 
    printf(BLUE"Decryptage de l'output obtenu...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Output/ring_crypte_xor.txt -o ../Output/ring_derypte_xor.txt -m xor -f ../key.txt"); 
    system("./checkfiles ../Datas/Source/ring.txt ../Output/ring_derypte_xor.txt"); 

}

void test_cbc() {
    printf("\n----------Tests des fonction de cryptage et de decryptage par la methode cbc----------\n\n");

    printf(ORANGE"Cryptage de chaton.jpg...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Datas/Source/chaton.jpg -o ../Output/chaton_crypte_cbc.jpg -m cbc-crypt -f ../key.txt -v ../vecteur.txt"); 
    printf(BLUE"Decryptage de l'output obtenu...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Output/chaton_crypte_cbc.jpg -o ../Output/chaton_decrypte_cbc.jpg -m cbc-decrypt -f ../key.txt -v ../vecteur.txt"); 
    system("./checkfiles ../Datas/Source/chaton.jpg ../Output/chaton_decrypte_cbc.jpg"); 

    printf(ORANGE"Cryptage de flower.jpg...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Datas/Source/flower.jpg -o ../Output/flower_crypte_cbc.jpg -m cbc-crypt -f ../key.txt -v ../vecteur.txt"); 
    printf(BLUE"Decryptage de l'output obtenu...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Output/flower_crypte_cbc.jpg -o ../Output/flower_decrypte_cbc.jpg -m cbc-decrypt -f ../key.txt -v ../vecteur.txt"); 
    system("./checkfiles ../Datas/Source/flower.jpg ../Output/flower_decrypte_cbc.jpg"); 

    printf(ORANGE"Cryptage de ring.txt...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Datas/Source/ring.txt -o ../Output/ring_crypte_cbc.txt -m cbc-crypt -f ../key.txt -v ../vecteur.txt"); 
    printf(BLUE"Decryptage de l'output obtenu...\n"RESET); 
    sleep(1); 
    system("./../bin/sym_crypt -i ../Output/ring_crypte_cbc.txt -o ../Output/ring_decrypte_cbc.txt -m cbc-decrypt -f ../key.txt -v ../vecteur.txt"); 
    system("./checkfiles ../Datas/Source/ring.txt ../Output/ring_decrypte_cbc.txt"); 
}
