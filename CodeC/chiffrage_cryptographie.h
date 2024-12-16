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
void xor(unsigned char* msg, unsigned char* key, int msg_length, int key_length, unsigned char* crypted);


/**
 *  @brief Fonction permettant la création d'une clé de chiffrement
 *         alphanumérique aléatoire.
 *  @param : taille de la clé souhaitée (:int), @ mémoire qui contiendra la clé
 *  @return null
 */
void gen_key(int len, unsigned char *key);


/**
 *  @brief Fonction permettant l'affichage sur STDOUT du message 
 *         passe en parametre
 *  @param : nom du fichier a afficher (:String)
 *  @return null
 */
 void afficher_message(char* affiche_msg);


/**
 *  @brief Fonction permettant l'affichage sur STDOUT de l'aide des 
 *         commandes. Aucun scanf ne sera fait après.
 *  @param : Auncun paramètre d'entrée. Message invariable.
 *  @return Affichage sur STDOUT.
 */
void commandes_affichage();


/**
 *  @brief Fonction permettant le cryptage a partir d'un fichier
 *         Les fichiers seront ouverts dans la fonctions
 *  @param : nom du fichier msg (:String), cle de chiffrage (:String)
 *  @return 0 si succes, -1 sinon 
 */
int xor_fichier(const char* fich_in, const char* fichier_out, unsigned char* key);


/**
 *  @brief Fonction permettant le cryptage en utilisant la notion de
 *         maque jetable (cf chiffrement de Vernam).
 *  @param : Nom du fichier (:String), taille de la cle (:int)
 *  @return 0 si succes, -1 sinon 
 */
int mask(const char* fich_in, const char* fich_out);


/**
 *  @brief Fonction permettant le chiffrement en utilisant la notion de
 *         Cypher Bloc Chain (CBC)
 *  @param : Nom du fichier (:String), clef de chiffrement (:int)
 *  @param : fichier vecteur (:String), taille (:size_t)
 *  @param : fichier où l'on va écrire le chiffré 
 *  @return  0 si succes, -1 sinon 
 */
int cbc_crypt(char *msg, unsigned char* key, char* iv, char* res);


/**
 *  @brief Fonction permettant le dechiffrement de la notion de
 *         Cypher Bloc Chain (CBC)
 *  @param : Nom du fichier (:String), clef de chiffrement (:int)
 *  @param : fichier vecteur (:String), taille (:size_t)
 *  @param : fichier où l'on va écrire le chiffré 
 *  @return  0 si succes, -1 sinon 
 */
int cbc_decrypt(char *msg, unsigned char* key, char* iv, char* res);


/** @} */



/** @} */

#endif