#ifndef _CRACK_H
#define _CRACK_H

#include <stdbool.h>

/* ============================================================================== */
/* ============================================================================== */

#define TAILLE_BLOC 512


/**
 *  @brief Fonction permettant le crack en suivant la métode c1
 *  @param : fichier chiffré (:String)
 *  @param : taille clé à chercher (:int)
 *  @return null
 */
void break_code_c1(unsigned char* infile, unsigned int key_length);

// void break_code_c2;

// void break_code_c3;


/** @} */



/** @} */

#endif