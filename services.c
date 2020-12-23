#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_ines.h"
#include "services.h"

int isDocumentValid(char* filename){

    int checkProlog;
    int hasPrologRes = hasProlog(filename);
    if(hasPrologRes == 1){
        checkProlog = checkPrologOK(filename) & checkPrologEndTag(filename);
    } else {
        checkProlog = 1;
    }
//
    int whiteSpace = checkWhiteSpaceSlash(filename);
    int whiteSpace2 = checkWhiteSpaceChevron(filename);
//    //int whiteSpace2 = collectTags(filename);
//
    int doubleOPChevronOKRes = checkDoubleOPChevron(filename);
//
    int doubleClosingChevronOKRes = checkDoubleClosingChevron(filename);

    int result = checkProlog & whiteSpace & doubleOPChevronOKRes & doubleClosingChevronOKRes & whiteSpace2;
    int rootTag = 0;
    if(result == 1){

    rootTag = collectTags(filename);

    }

    return rootTag;
//return whiteSpace2;
}
