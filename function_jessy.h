#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include <stdio.h>

/*
    La fonction extrait les �l�ments recherch�s et les places dans les variables plac�es en param�tres
*/
void DTDTagExtraction(FILE *f, char *DTDRootTag, char *DTDMotherTag, char *DTDDaughterSymbol, char *DTDDaughterTag,  char *DTDValueDaughter);

/*
    La fonction compare les �l�ments r�cup�r�s du fichier DTD aux �l�ments du fichier XML
*/
int compareXMLDTD(FILE *f, char *DTDRootTag, char *DTDMotherTag, char *DTDDaughterSymbol, char *DTDDaughterTag,  char *DTDValueDaughter);

/*
    La fonction permet de supprimer tous les caract�res se trouvant � gauche du texte sp�cifi� dans les param�tres
*/
char *strltrim(const char *str, char c);

/*
    La fonction regarde si le texte plac� en param�tre contient le caract�re '<'
*/
void valueDaughterTest(char *value);

#endif // FUNCTION_H_INCLUDED
