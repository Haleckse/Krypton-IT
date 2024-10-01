#ifndef _CHIFFRAGE_H
#define _CHIFFRAGE_H
#include <stdbool.h>

/* ============================================================================== */
/* ============================================================================== */

#define TAILLE_BLOC 512

/**
 *  @brief Fonction permettant le chiffrage d'un message
 *  @param : message a chiffré (:String), clé de chiffrage (:String)
 *  @param : taille du message (:int), variable qui contiendra le chiffrage (:String)
 *  @return null
 */
void chiffrage_char(char* msg, char* key, int msg_length, char* crypted);


/**
 *  @brief Fonction permettant le dechiffrage a partir d'un message
 *  @param : message chiffré (:String), clé de dechiffrage (:String)
 *  @param : taille du message (:int), variable qui contiendra le dechiffrage (:String)
 *  @return null
 */
void dechiffrage_char(char* crypted, char* key, int size, char* decrypted);


/**
 *  @brief Fonction permettant l'affiche sur STDOUT du message 
 *  passe en parametre
 *  @param : nom du fichier a afficher (:String)
 *  @return null
 */
 void afficher_message(char* affiche_msg);


/**
 *  @brief Fonction permettant le cryptage a partir d'un fichier
 *  Les fichiers seront ouverts dans la fonctions
 *  @param : nom du fichier msg (:String), cle de chiffrage (:String)
 *  @return 0 si succes, -1 sinon 
 */
int chiffrage_char_fichier(char* nom_fichier, char* key);


/**
 *  @brief Fonction permettant le decryptage a partir d'un fichier
 *  Les fichiers seront ouverts dans la fonctions
 *  @param : cle de dechiffrage (:String)
 *  @return 0 si succes, -1 sinon 
 */
int dechiffrage_char_fichier(char* key);

/** @} */



/** @} */

#endif