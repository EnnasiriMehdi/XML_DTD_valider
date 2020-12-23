#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_ines.h"

void example22(char* filename)
{
    FILE *filePointer;
    char ch;

    filePointer = fopen(filename, "r");

    if (filePointer == NULL)
    {
        printf("File is not available \n");
    }
    else
    {
        while ((ch = fgetc(filePointer)) != EOF)
        {
            printf("%c", ch);
        }
    }

    fclose(filePointer);


}

char * toBuffer(char* filename, char * bufferEntry){
printf("\nINFO : toBuffer");

//char buffer = 0;
long length;
FILE * f = fopen (filename, "rb");

if (f)
{
  fseek (f, 0, SEEK_END);
  length = ftell (f);
  fseek (f, 0, SEEK_SET);
  bufferEntry = malloc( sizeof(char)*length);
  if (bufferEntry)
  {
    fread (bufferEntry, 1, length, f);
  }
  bufferEntry[length] = '\0';

  //printf("%s", bufferEntry);
  fclose (f);
  return bufferEntry;
}
else {
    return "Fichier introuvable";
}
}

int checkWhiteSpaceChevron(char* filename){
printf("\nINFO : checkWhiteSpaceChevron");
char* fileToString = toBuffer(filename, fileToString);
int length = strlen(fileToString);
int line = 1;
int checkWhiteSpaceChevron = 1;
int vautRC = 0;

for(int i = 0; i < length; i++){

    if (fileToString[i] == '\n'){
        line++;
        vautRC = 1;
    }

    if(fileToString[i] == '<' && fileToString[i+1] == ' '){
        printf("\n Un chevron ouvrant ne doit pas etre suivi d'un blanc (espace, tab, retour chariot). Voir ligne %d", line);
        checkWhiteSpaceChevron = 0;
    }

    if(fileToString[i-2] == '<' && vautRC == 1){
        line--;
        printf("\n Un chevron ouvrant ne doit pas etre suivi d'un blanc (espace, tab, retour chariot). Voir ligne %d", line);
        checkWhiteSpaceChevron = 0;
    }

    if(fileToString[i] == '<' && fileToString[i+1] == '\t'){
        printf("\n Un chevron ouvrant ne doit pas etre suivi d'un blanc (espace, tab, retour chariot). Voir ligne %d", line);
        checkWhiteSpaceChevron = 0;
    }
    vautRC = 0;
}

return checkWhiteSpaceChevron;

}

int checkWhiteSpaceSlash(char* filename){
printf("\nINFO : checkWhiteSpaceSlash");

char* fileToString = toBuffer(filename, fileToString);
int length = strlen(fileToString);
int line = 1;
int checkWhiteSpaceSlash = 1;
int posChevronOp = 0;
int posChevronCl = 0;
int vautRC = 0;
for(int i = 0; i < length; i++){
    if (fileToString[i] == '\n'){
        line++;
        vautRC = 1;
    }

    if (fileToString[i] == '<'){
        posChevronOp = i;
    }

    if (fileToString[i] == '>'){
        posChevronCl = i;
    }

    if(posChevronOp > posChevronCl){
        if(fileToString[i] == '/' && fileToString[i+1] == ' '){
            printf("\n Un slash1 ne doit pas etre suivi d'un blanc (espace, tab, retour chariot). Voir ligne %d", line);
            checkWhiteSpaceSlash = 0;
        }
        if(fileToString[i-2] == '/' && vautRC == 1){
            line--;
            printf("\n Un slash2 ne doit pas etre suivi d'un blanc (espace, tab, retour chariot). Voir ligne %d", line);
            checkWhiteSpaceSlash = 0;

        }
        if(fileToString[i] == '/' && fileToString[i+1] == '\t'){
            printf("\n Un slash3  ne doit pas etre suivi d'un blanc (espace, tab, retour chariot). Voir ligne %d", line);
            checkWhiteSpaceSlash = 0;
        }
    }
    vautRC = 0;
}

return checkWhiteSpaceSlash;

}


int hasProlog(char* filename)
{
printf("\nINFO : hasProlog");
    FILE *filePointer;
    char ch;
    int character = 0, line = 0, space = 0, tab = 0;
    int prologOK=0;

    filePointer = fopen(filename, "r");

    if (filePointer == NULL)
    {
        printf("Fichier introuvable \n");
    }
    else
    {
        int hasProlog = 0;
        int chevronOP = 0;
        while ((ch = fgetc(filePointer)) != EOF)
        {
            character++;
            if (ch == ' ')
                space++;
            if (ch == '\n')
                line++;
            if (ch == '\t')
                tab++;

            if(ch == '<'){
                chevronOP = 1;
                ch = fgetc(filePointer);
                if(ch == '?'){
                printf("le document contient un prolog \n");
                hasProlog = 1;

                break;
                }
            }

        }
        return hasProlog;
    }

    fclose(filePointer);

    return prologOK;

}

int checkPrologOK(char* filename)
{
printf("\nINFO : checkPrologOK");

    FILE *filePointer;
    char ch;
    int character = 0, line = 0, space = 0, tab = 0;
    int prologOK=0;

    filePointer = fopen(filename, "r");

    if (filePointer == NULL)
    {
        printf("Fichier introuvable \n");
    }
    else
    {
        int chevronOP = 0;
        while ((ch = fgetc(filePointer)) != EOF)
        {
            character++;
            if (ch == ' ')
                space++;
            if (ch == '\n')
                line++;
            if (ch == '\t')
                tab++;

            if(ch == '<'){
                chevronOP = 1;
            }
            if(chevronOP == 1 && ch == '?' && (character-space-line-tab) == 2){
                printf("le prolog est bien place \n");
                prologOK=1;
                chevronOP = 0;

            }else if (chevronOP == 1 && ch == '?' && (character-space-line-tab) != 2){
                printf("un prolog semble mal place. Voir %d \n", line);
                chevronOP = 0;
                prologOK=0;
            }
        }
    }

    fclose(filePointer);

    return prologOK;

}

int checkPrologEndTag(char* filename)
{
printf("\nINFO : checkPrologEndTag");
    FILE *filePointer;
    char ch;
    int character = 0, line = 0, space = 0, tab = 0;
    int prologOK=0;
    int characterTmp;

    filePointer = fopen(filename, "r");

    if (filePointer == NULL)
    {
        printf("Fichier introuvable \n");
    }
    else
    {
        int questionMark = 0;
        while ((ch = fgetc(filePointer)) != EOF)
        {
            character++;
            if (ch == ' ')
                space++;
            if (ch == '\n')
                line++;
            if (ch == '\t')
                tab++;

            if(ch == '?'){
                questionMark = 1;
                ch = fgetc(filePointer);
                characterTmp = character;
            }

            if(questionMark == 1 && ch == '>'){
                printf("\n le prolog a un endTag");
                prologOK=1;
                questionMark = 0;
                break;
            }else if(questionMark == 1 && ch != '>'){
                questionMark = 0;
            }

        }
        if (prologOK != 1){
                printf("\n le prolog ne semble pas avoir d'endTag");
            }
    }

    fclose(filePointer);

    return prologOK;

}


int checkDoubleOPChevron(char* filename)
{
printf("\nINFO : checkDoubleOPChevron");

    FILE *filePointer;
    char ch;
    int character = 0, line = 1;
    int chevronOpOK = 1;
    int characterTmp = 0;

    filePointer = fopen(filename, "r");

    if (filePointer == NULL)
    {
        printf("Fichier introuvable \n");
    }
    else
    {
        int chevronOP = 0;
        while ((ch = fgetc(filePointer)) != EOF)
        {

            if(ch == '\n'){
                line++;
            }
            if(ch == '<' && characterTmp == 0){
                chevronOP = 1;
                characterTmp = character;
            }

            if((chevronOP == 1) && (ch == '>')){
                chevronOP = 0;
                characterTmp = 0;
            }else if(chevronOP == 1 && ch == '['){
                chevronOP = 0;
                characterTmp = 0;
            } else if ((chevronOP == 1) && (ch == '<') && (characterTmp != 0 )&& (characterTmp != character )){
                printf("\n Un chevron < est mal place. Voir ligne %d", line);
                chevronOpOK = 0;
            }

            character++;
        }
        //if(chevronOpOK == 1){
          //  printf("\n chevronOpOK");
        //}
    }

    fclose(filePointer);

    return chevronOpOK;

}

//FIXME
int checkDoubleClosingChevron(char* filename)
{
printf("\nINFO : checkDoubleClosingChevron");
    FILE *filePointer;
    char ch;
    int character = 0, line = 1;
    int closingChevronOK=1;
    int characterTmp = 0;

    filePointer = fopen(filename, "r");

    if (filePointer == NULL)
    {
        printf("Fichier introuvable \n");
    }
    else
    {
        int chevronOP = 0;
        while ((ch = fgetc(filePointer)) != EOF)
        {
            if(ch == '\n'){
                line++;
            }

            if(ch == '>' && characterTmp == 0){
                chevronOP = 1;
                characterTmp = character;
            }

            if((chevronOP == 1) && (ch == '<')){
                chevronOP = 0;
                characterTmp = 0;
            }else if(chevronOP == 1 && ch == ']'){
                chevronOP = 0;
                characterTmp = 0;
            } else if ((chevronOP == 1) && (ch == '>') && (characterTmp != 0 )&& (characterTmp != character )){
                printf("\n Un chevron > semble mal place. Voir ligne %d", line);
                closingChevronOK = 0;
            }

            character++;
        }
    }

    fclose(filePointer);

    return closingChevronOK;

}

////On d�termine la longueur maximum des balises ainsi que leur nombre
void collectInfoTags(FILE *filePointer, int* max_length, int* countST)
{
printf("\nINFO : collectInfoTags");
        char ch;
        int max_lengthTmp = 0; //longueur maximum d'une balise
        int countSTTmp=0; //nombre de balise
        int chevronOP = 0;
        int countCHTmp =0;//compteur de character que contient une balise

        while ((ch = fgetc(filePointer)) != EOF){

            if(ch == '<'){
                chevronOP = 1;
                ch = fgetc(filePointer);
                countCHTmp++;
            }

            if(chevronOP == 1 && ch != '>' && ch != '[' ){

                    countCHTmp++;

            } else if(chevronOP == 1 && ch == '>'){

                countSTTmp++;
                if(countCHTmp > max_lengthTmp){

                    max_lengthTmp = countCHTmp;
                }
                countCHTmp = 0;
                chevronOP = 0;
            }

        }
        *max_length = max_lengthTmp;
        *countST = countSTTmp;
}


////On v�rifie la balise racine en v�rifiant la pr�sence ou non d'un prolog
////on ignore �g�lament la DTD
void checkRootTag(FILE * filePointer, char** tags, int countST, int max_length, int* tagsOK){
printf("\nINFO : checkRootTag");

    int rootTagsOK = 1;
    int start = 0;
    for(int i = 0; i < countST; i++){
        if(tags[i][1] == '?'){
            start++;
        } else if(tags[i][1] == '!'){
            start++;
        }
    }

    if(start < countST){

        for(int j = 1; j < max_length ; j++){

            if(tags[start][j] != tags[countST-1][j+1]){
                rootTagsOK = 0;
                printf("La balise racine de fin ne correspond pas a la balise de debut");
                *tagsOK = 0;
            }
        }
        if(rootTagsOK = 1){
        printf("balises racine : OK");
        }

    } else {
        printf("Il n'y a pas de tag xml");
    }
}

int collectTags(char* filename)
{
    printf("\nINFO : collectTags");
    FILE *filePointer;
    filePointer = fopen(filename, "r");

    int tagsOK = 1;

    if (filePointer == NULL)
    {
        printf("File is not available \n");
    }
    else
    {
        char ch;
        int max_length = 0; //longueur maximum d'une balise
        int countST=0; //nombre de balise
        int chevronOP = 0;
        int countCH =0;//compteur de character que contient une balise

        collectInfoTags(filePointer,  &max_length, &countST);

        rewind(filePointer);

////On initialise un tableau de caract�ere dans lequel on stockera par la suite les balises
        char **tags;
        tags = (char**)malloc(countST * sizeof(char*));
        for (int i = 0; i < countST; i++)
            tags[i] = (char*)malloc((max_length+1) * sizeof(char));

        for(int i =0; i < countST; i++){
            for(int j = 0; j < max_length + 1; j++){
                tags[i][j] = '1';
            }
        }
        chevronOP = 0;
        countCH = 0;
        countST = 0;
        while ((ch = fgetc(filePointer)) != EOF){

            if(ch == '<'){
                chevronOP = 1;
                tags[countST][countCH] = ch;
                countCH++;
                ch = fgetc(filePointer);
            }

            if(chevronOP == 1 && ch != '>' && ch != '['){

                tags[countST][countCH] = ch;
                countCH++;

            } else if(chevronOP == 1 && (ch == '>' || ch != '[')){

                tags[countST][countCH] = '\0';
                countST++;
                countCH = 0;
                chevronOP = 0;
            }
        }

////On v�rifie d'abord la balise racine en v�rifiant la pr�sence ou non d'un prolog
////on ignore �g�lament la DTD
    checkRootTag(filePointer, tags, countST, max_length, &tagsOK);

////On compare chaque balise ouvrante avec les balise fermantes
////si on trouve une �galit� on retourne 1 sinon 0
////si une balise fermante correspondante est trouv�e on remplace alors '<' par '0'
////pour ne pas repeter les comparaisons
        for(int i = 0; i < countST - 1; i++){

            int globalFound = 0;

            if(tags[i][strlen(tags[i])-1] == '/'){

                printf("\n Succes : une balise '%s' est acceptee", tags[i]);

            } else if(tags[i][0] != '0' && tags[i][1] != '/' && tags[i][1] != '?' && tags[i][1] != '!'){

                for(int j = i+1; j < countST; j++){

                    int found = 1;

                    if(tags[j][1] == '/'){

                        for(int k = 1; k < max_length -1; k++){

                            if(tags[i][k] != ' ' && tags[i][k] != '\n' && tags[i][k] != '\t'){
                                if(tags[j][1] == '/' && tags[i][k] != tags[j][k+1]){

                                    found = 0;
                                    break;
                                } else if (tags[j][1] != '/'){
                                    break;
                                }
                            } else {
                                break;
                            }
                        }

                        if(found == 1){

                            tags[j][0] = '0';
                        }
                        globalFound = globalFound | found ;

                        if(globalFound ==1){
                            break;
                        }
                    }
                }

                if(globalFound == 0){

                    printf("\n Echec : une balise '%s' n'a pas de balise fermante", tags[i]);
                    globalFound = 0;
                    tagsOK = 0;
                } else if (globalFound == 1){
                    printf("\n Succes : une balise '%s' a une balise fermante", tags[i]);
                }
            }
        }

        //for(int i = 0; i < countST; i++){

            //printf("\n str %s", tags[i]);
        //}

////S'il reste des balises fermantes commencants par '/' dans le tableaux de balises
////c'est que celles-ci n'ont pas de balises ouvrantes
	    for(int i = 0; i<countST; i++){

            if(tags[i][0]== '<' && tags[i][1]== '/'){

                printf("\n Echec. Une balise de fin '%s' n'a pas de balise de debut correspondante", tags[i]);
                tagsOK = 0;
            }
        }


        for (int i = 0; i < countST; i++)
            free(tags[i]);

        free(tags);
    }

    fclose(filePointer);

    return tagsOK;
}

