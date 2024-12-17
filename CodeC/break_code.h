#ifndef _CRACK_H
#define _CRACK_H

#include <stdbool.h>

/* ============================================================================== */
/* ============================================================================== */

#define TAILLE_BLOC 512
#define maxCaractères 62

// double english_freq[26] = {
//     0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061,
//     0.070, 0.0015, 0.0077, 0.040, 0.024, 0.067, 0.075, 0.019,
//     0.00095, 0.060, 0.063, 0.090, 0.028, 0.0098, 0.024, 0.0015,
//     0.020, 0.00074
// };

/**
 *  @brief Fonction permettant le crack en suivant la métode c1
 *  @param : fichier chiffré (:String)
 *  @param : taille clé à chercher (:int)
 *  @return null
 */
int break_code_c1(const char* infile, unsigned int key_length, const char* output_filename);

void break_code_c2(const char* cyphered_file,char* key_file, int key_length, const double* freq_th);

void break_code_c3(char* dico);

// void break_code_c2;

// void break_code_c3;


/** @} */



/** @} */

#endif