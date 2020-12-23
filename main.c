#include <stdio.h>
#include <malloc.h>
#include "functions.h"
#include "function_ines.h"
#include "function_jessy.h"
#include "services.h"
#include <string.h>
#include <regex.h>



int main() {

    int external_dtd = 0;
    FILE* file = open_file("my_dtd.txt", "r");

    //int valider = isDocumentValid("my_dtd.txt");

    clean_file(file);
    fseek(file, 0, SEEK_SET);

    clean_file(file);
    fseek(file, 0, SEEK_SET);




    //printf("\n valide ==== %d \n", valider);
    root_tag * root_tag = NULL;
    root_tag = malloc(sizeof(root_tag)*1);

    root_tag = validation_root_tag("my_dtd.txt",external_dtd);
    fseek(file, 0, SEEK_SET);


    clean_start("my_dtd.txt", root_tag);



    if (file != NULL && root_tag != NULL )
    {
        liste_element *liste_element;
        liste_element = calloc(1, sizeof(liste_element));
        liste_element->first = NULL;
        liste_element->last = NULL;
        liste_element->numb_element = 0;
        get_elements(file, liste_element);

        liste_attributes *liste_attribute;
        liste_attribute = calloc(1, sizeof(liste_attribute));
        liste_attribute->first = NULL;
        liste_attribute->last = NULL;
        liste_attribute->numb_attributes = 0;
        fseek(file, 0, SEEK_SET);

        get_attributes(file, liste_attribute, liste_element);
        printf("il y a %d erreur(s) dans les attributs \n", liste_attribute->error);

        /*
        attributes *attibute;
        attibute = liste_attribute->first;

        elements *element;
        element = liste_element->first;

            //printf("::::::::::::::::::ROOT ELEMENT  \n");
          //  printf(" name root : %s \n", root_return->root_tag->name);
           // printf(" name root : %s \n", root_return->root_tag->url);

          //  printf(" \n");

          printf(" ********************************\n");
          printf("::::::::::::::::::ELEMENT \n");
          printf(" \n");
          while (element->next != NULL)
          {
              printf("name element : %s \n", element->name);
              printf("type element : %s \n", element->type);
              printf("nb child element : %d \n", element->size_list_childs);
              for (int i = 0; i < element->size_list_childs; i++)
              {
                  printf("<<<<<<<<< mon enfant %d : %s \n ", i, element->childs[i]);
                  printf("last char de mon enfant >>>>>>>>>>>>>>> %c \n ",element->childs[i][strlen(element->childs[i]) - 1]);
              }
              //printf("name element : %s \n", element->name);
              printf("ordered child element : %d \n", element->ordered);
              element = element->next;
              printf(" \n");
              printf(" ---- \n");
          }

          printf("name element : %s \n", element->name);
          printf("type element : %s \n", element->type);
          printf("nb child element : %d \n", element->size_list_childs);
          for (int i = 0; i < element->size_list_childs; i++)
          {
              printf("<<<<<<<<< mon enfant %d : %s \n ", i, element->childs[i]);
              printf("last char de mon enfant >>>>>>>>>>>>>>> %c \n ",element->childs[i][strlen(element->childs[i]) - 1]);
          }

          //printf("name element : %s \n", element->name);
          printf("ordered child element : %d \n", element->ordered);
          element = element->next;
          printf(" \n");
          printf(" ---- \n");


          printf(" ********************************\n");


          printf(":::::::::::::::::::::::::ATTRIBUTES \n");
          while (attibute->next != NULL)
          {
              printf(" \n");
              printf(" name : %s \n", attibute->name);
              printf(" error string :  %s \n", attibute->error_string);
              printf(" type : %s \n", attibute->type);
              printf(" nb liste  : %d \n", attibute->size_liste_values);
              for (int i = 0; i < attibute->size_liste_values; i++)
              {
                  printf(" """"""""""""""""""""""""""""""""    ma string %d : %s \n ", i, attibute->liste_values[i]);
              }
              printf(" obligation :  %s \n", attibute->obligation);
              printf(" default value : %s \n", attibute->default_value);
              printf(" ref element : %s \n", attibute->ref_element);
              printf(" \n");
              printf(" ---- \n");
              attibute = attibute->next;
          }

          printf(" \n");
          printf(" name : %s \n", attibute->name);
          printf(" error string :  %s \n", attibute->error_string);
          printf(" type : %s \n", attibute->type);
          printf(" nb liste  : %d \n", attibute->size_liste_values);
          for (int i = 0; i < attibute->size_liste_values; i++)
          {
              printf(" """"""""""""""""""""""""""""""""    ma string %d : %s \n ", i, attibute->liste_values[i]);
          }
          printf(" obligation :  %s \n", attibute->obligation);
          printf(" default value : %s \n", attibute->default_value);
          printf(" ref element : %s \n", attibute->ref_element);
          printf(" \n");
          printf(" ---- \n");
          */

        //printf(" mon profchain mot = %s \n ", get_next_word(file_to_string(file),NULL));
        //printf(" mon profchain mot = %s \n ", get_next_word(file_to_string(file),NULL));
        //printf(" mon profchain mot = %s \n ", get_next_word(file_to_string(file),NULL));
        //printf(" mon profchain mot = %s \n ", get_next_word(file_to_string(file),NULL));
        XML_reader(file, liste_element, liste_attribute, NULL);
    }

    fclose(file);

    return 0;
}
