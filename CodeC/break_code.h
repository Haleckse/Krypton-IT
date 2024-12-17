#ifndef _CRACK_H
#define _CRACK_H

#include <stdbool.h>

/* ============================================================================== */
/* ============================================================================== */

#define TAILLE_BLOC 512
#define maxCaractères 62

/**
 *  @brief Fonction permettant le crack en suivant la métode c1
 *  @param : fichier chiffré (:String)
 *  @param : taille clé à chercher (:int)
 *  @return null
 */
int break_code_c1(const char* infile, unsigned int key_length, const char* output_filename);

void break_code_c2(const char *ciphered_text, int key_length, const double *freq_th);

void break_code_c3();

// void break_code_c2;

// void break_code_c3;


/** @} */



/** @} */

#endif