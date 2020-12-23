#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include <stdio.h>

/*
    La fonction extrait les éléments recherchés et les places dans les variables placées en paramètres
*/
void DTDTagExtraction(FILE *f, char *DTDRootTag, char *DTDMotherTag, char *DTDDaughterSymbol, char *DTDDaughterTag,  char *DTDValueDaughter);

/*
    La fonction compare les éléments récupérés du fichier DTD aux éléments du fichier XML
*/
int compareXMLDTD(FILE *f, char *DTDRootTag, char *DTDMotherTag, char *DTDDaughterSymbol, char *DTDDaughterTag,  char *DTDValueDaughter);

/*
    La fonction permet de supprimer tous les caractères se trouvant à gauche du texte spécifié dans les paramètres
*/
char *strltrim(const char *str, char c);

/*
    La fonction regarde si le texte placé en paramètre contient le caractère '<'
*/
void valueDaughterTest(char *value);

#endif // FUNCTION_H_INCLUDED
