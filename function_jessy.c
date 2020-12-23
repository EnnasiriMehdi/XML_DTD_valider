#include "function_jessy.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

void DTDTagExtraction(FILE *f, char *DTDRootTag, char *DTDMotherTag, char *DTDDaughterSymbol, char *DTDDaughterTag, char *DTDValueDaughter){
    char c;

    int rootOpeningTagCheck = 0;
    int motherOpeningTagCheck = 0;
    int daughterOpeningTagCheck = 0;
    int valueDaughterCheck = 0;
    int daughterTagSymbolCheck = 0;

    int cpt_name_balise = 0;

    while(c != EOF){
        c = fgetc(f);

        if(rootOpeningTagCheck == 0){
            if( c == 'E'){
                fseek(f, 1, SEEK_CUR);
                while(c != ' '){
                    c = fgetc(f);
                    cpt_name_balise++;
                }
                fseek(f, cpt_name_balise * -1, SEEK_CUR);
                fgets(DTDRootTag, cpt_name_balise, f);
                rootOpeningTagCheck = 1;
            }
        }

        if(rootOpeningTagCheck == 1 && motherOpeningTagCheck == 0){
            if( c == 'T'){
                cpt_name_balise = 0;
                fseek(f, 1, SEEK_CUR);
                while(c != ' '){
                    c = fgetc(f);
                    cpt_name_balise++;
                }
                fseek(f, cpt_name_balise * -1, SEEK_CUR);
                fgets(DTDMotherTag, cpt_name_balise, f);
                motherOpeningTagCheck = 1;
            }
        }

        if(motherOpeningTagCheck == 1 && daughterTagSymbolCheck == 0){
            if( c == ')'){
                fseek(f, -2, SEEK_CUR);
                fgets(DTDDaughterSymbol, 2, f);
                daughterTagSymbolCheck = 1;

                if(strcmp(DTDDaughterSymbol, "+") != 0 && strcmp(DTDDaughterSymbol, "?") != 0 && strcmp(DTDDaughterSymbol, "*") != 0){
                    strcpy(DTDDaughterSymbol, "EMPTY");
                }
            }
        }

        if(daughterTagSymbolCheck == 1 && daughterOpeningTagCheck == 0){
            if( c == 'T'){
                cpt_name_balise = 0;
                fseek(f, 1, SEEK_CUR);
                while(c != ' '){
                    c = fgetc(f);
                    cpt_name_balise++;
                }
                fseek(f, cpt_name_balise * -1, SEEK_CUR);
                fgets(DTDDaughterTag, cpt_name_balise, f);
                daughterOpeningTagCheck = 1;
            }
        }

        if(daughterOpeningTagCheck == 1 && valueDaughterCheck == 0){
            if( c == '('){
                cpt_name_balise = 0;
                fseek(f, 1, SEEK_CUR);
                while(c != ')'){
                    c = fgetc(f);
                    cpt_name_balise++;
                }
                fseek(f, cpt_name_balise * -1, SEEK_CUR);
                fgets(DTDValueDaughter, cpt_name_balise, f);
                valueDaughterCheck = 1;
            }
        }
    }
}

int compareXMLDTD(FILE *f, char *DTDRootTag, char *DTDMotherTag, char *DTDDaughterSymbol, char *DTDDaughterTag,  char *DTDValueDaughter){
    char c;
    char header[] = "<?xml version=\"1.0\"?>";

    char XMLRootTag [40] = "EMPTY";
    char XMLMotherTag [40] = "EMPTY";
    char XMLDaughterTag [40] = "EMPTY";
    char XMLValueDaughter [40] = "EMPTY";

    int cpt_name_balise = 0;

    int XMLRootTagCheck = 0;
    int XMLMotherTagCheck = 0;
    int XMLDaughterTagCheck = 0;
    int XMLValueDaughterCheck = 0;

    int nbDaughterTag = 0;

    fseek(f, strlen(header), SEEK_CUR);

    while(c != EOF){
        c = fgetc(f);
        /*------------------------------------COMPARER RACINE------------------------------------------*/
        if( XMLRootTagCheck == 0){
            if(c == '<' && fgetc(f) != '/'){
                cpt_name_balise = 0;
                fseek(f, -1, SEEK_CUR);
                while(c != '>'){
                    c = fgetc(f);
                    cpt_name_balise++;
                    if(c == ' ' ){
                        break;
                    }
                }
                fseek(f, cpt_name_balise * -1, SEEK_CUR);
                fgets(XMLRootTag, cpt_name_balise, f);
                printf("Balise racine dans le fichier XML : %s\n", XMLRootTag);
                if(strcmp(XMLRootTag, DTDRootTag) != 0){
                    printf("Les balises racines du fichier XML ne correspondent pas au DTD !\n");
                    return 0;
                }
                XMLRootTagCheck = 1;
            }
        }
        /*------------------------------------FIN COMPARER RACINE------------------------------------------*/
        /*------------------------------------COMPARER MERE------------------------------------------*/
        if(XMLRootTagCheck == 1 && XMLMotherTagCheck == 0){
            if( c == '<' && fgetc(f) != '/'){
                cpt_name_balise = 0;
                fseek(f, -1, SEEK_CUR);
                while(c != '>'){
                    c = fgetc(f);
                    cpt_name_balise++;
                    if(c == ' ' ){
                        break;
                    }
                }
                fseek(f, cpt_name_balise * -1, SEEK_CUR);
                fgets(XMLMotherTag, cpt_name_balise, f);
                printf("\tBalise mere dans la balise ESGI : %s\n", XMLMotherTag);
                if(strcmp(XMLMotherTag, DTDMotherTag) != 0){
                    printf("Les balises meres du fichier XML ne correspondent pas au DTD !\n");
                    return 0;
                }
                XMLMotherTagCheck = 1;
            }
        }
        /*------------------------------------FIN COMPARER MERE------------------------------------------*/
        /*------------------------------------COMPARER FILLES--------------------------------------------*/
        if(XMLMotherTagCheck == 1 && XMLDaughterTagCheck == 0){
            if(c == '<' && fgetc(f) != '/'){
                cpt_name_balise = 0;
                fseek(f, -1, SEEK_CUR);
                while(c != '>'){
                    c = fgetc(f);
                    cpt_name_balise++;
                    if(c == ' ' ){
                        break;
                    }
                }
                fseek(f, cpt_name_balise * -1, SEEK_CUR);
                fgets(XMLDaughterTag, cpt_name_balise, f);
                printf("\t\tBalise fille dans la balise classrooms: %s\n", XMLDaughterTag);
                if(strcmp(XMLDaughterTag, DTDDaughterTag) != 0){
                    printf("Les balises filles du fichier XML ne correspondent pas au DTD !\n");
                    return 0;
                }
                cpt_name_balise = 0;
                XMLDaughterTagCheck = 1;
                XMLValueDaughterCheck = 0;
                nbDaughterTag++;

            }
        }
        /*------------------------------------FIN COMPARER FILLES--------------------------------------------*/
        /*------------------------------------RECUPERATION VALEUR FILLE--------------------------------------------*/
        if(XMLDaughterTagCheck == 1 && XMLValueDaughterCheck == 0){
            if( c == '>'){
                cpt_name_balise = 0;
                while(c != '/'){
                    c = fgetc(f);
                    cpt_name_balise++;
                }
                fseek(f, cpt_name_balise * -1, SEEK_CUR);
                fgets(XMLValueDaughter, cpt_name_balise -1, f);
                strcpy(XMLValueDaughter,strltrim(XMLValueDaughter, '>'));
                printf("\t\t\tValeur de la balise fille : %s\n", XMLValueDaughter);

                if(strcmp(DTDValueDaughter, "PCDATA") == 0){
                    valueDaughterTest(XMLValueDaughter);
                }
                fseek(f, 1,SEEK_CUR);
                cpt_name_balise = 0;
                XMLValueDaughterCheck = 1;
                XMLDaughterTagCheck = 0;
            }
        }
        /*------------------------------------RECUPERATION VALEUR FILLE--------------------------------------------*/
    }

    printf("\tBalise mere dans la balise ESGI : %s\n", XMLMotherTag);

    if(strcmp(DTDDaughterSymbol,"+") == 0 && nbDaughterTag == 0){
        printf("Il faut minimum une balise !\n");
        return 0;
    }else if( strcmp(DTDDaughterSymbol,"?") == 0  && nbDaughterTag > 1){
        printf("Il y a trop de balises !\n");
        return 0;
    }else if(strcmp(DTDDaughterSymbol,"EMPTY") == 0 && nbDaughterTag != 1){
        printf("Il faut une seule balise !\n");
        return 0;
    }
    return 1;
}

char *strltrim(const char *str, char c) {
    char *trim = NULL;
    size_t i, len;

    if (str == NULL) {
        return trim;
    }

    len = strlen(str);
    if (len <= 0) {
        return trim;
    }

    /*
    i = 0;
    while (i < len && str[i] == c) {
        i++;
    }
    */

    for (i = 0; i < len && str[i] == c; i++);

    if (i == len) {
        return NULL;
    }

    trim = malloc(sizeof(char) * (len - i + 1));

    strcpy(trim, str + i);

    return trim;
}

void valueDaughterTest(char *value){
    for(int j = 0; j < strlen(value); j++){
        if((value[j] == '<')){
            printf("Attention ! La valeur ( [%c] ) de la fille n'est pas du texte !\n", value[j]);
        }
    }
}









