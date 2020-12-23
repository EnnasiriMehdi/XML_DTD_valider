#include "functions.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

int super_cpt = 0;
int super_cpt_bis = 0;
int line = 0;
int colone = 0;

// open file  by filename
FILE* open_file(char* filename, char * option)
{
    FILE *file = NULL;
    file = fopen("my_dtd.txt", option);
    if (file == NULL)
    {

        //printf("impossible de lire le fichier / fic intrrouvable ");
        return 0;
    }
    else
    {
        //printf(" je suis dans open file : %s", file_to_string(file));
        return file;
    }
}


char * file_to_string (FILE * file)
{
    char c ;
    int i = 0;
    fseek(file,0,SEEK_SET);
    while ((c = getc(file)) != EOF)
    {
        i++;
    }
    fseek(file,0,SEEK_SET);
    char * string = NULL ;
    string = malloc(sizeof(char) * (i+1));
    for (int cpt = 0; cpt < i ; cpt ++)
    {
        string[cpt] = getc(file);
    }
    string[i] = '\0';
    return string ;
}

void clean_start(char * string, root_tag * root_tag)
{
    FILE * file = open_file(string, "r+");
    char * file_string = file_to_string(file);
    file_string = &file_string[root_tag->start];
    file = fopen("my_dtd.txt", "w");
    fputs(file_string, file);
    fclose(file);
}

root_tag * get_root_tag(char * string, root_tag* root_tag)
{
    int start = 0;
    for (int i = 0 ; i < strlen(string) ; i ++)
    {
        if ( string [i] == '<')
        {
            start = i ;
            break;
        }
    }
    int cpt = 0;
    for (int i = start ; i < strlen(string) ; i ++)
    {
        if ( string[i] != '[')
        {
            cpt++;
        }
        else
        {
            cpt++;
            break;
        }
    }
    char * balise = NULL;
    balise = malloc(sizeof(char)* (cpt+2));
    balise = get_string_in_string(string, start,cpt+1);
    balise[cpt+1] = '\0';

    if ( strstr(balise, "<!DOCTYPE") != NULL )
    {
        printf(" premiere occurence : %d  \n", strstr(string, "<!DOCTYPE") - &string[0] );
        string = &string[strlen("<!DOCTYPE")+1+ (strstr(string, "<!DOCTYPE") - &string[0]) ];
        root_tag->start += (strlen("<!DOCTYPE")+1);
        printf("  :::::: %s \n ",  string);
        root_tag->name = get_next_word(string,NULL);
        root_tag->start += strlen(root_tag->name)+2;
        printf("mont root name = %s \n", root_tag->name);
        if ( strcmp(root_tag->name , "[") == 0)
        {
            root_tag->name = NULL;
        }
        return root_tag;
    } else
    {
        root_tag->name = NULL;
        return root_tag;
    }
}

root_tag * is_xml_declaration(char * string)
{
    root_tag * root_tag = NULL;
    root_tag = malloc(sizeof(root_tag)* 1);
    root_tag->start = 0;

    int start = 0;
    for (int i = 0 ; i < strlen(string) ; i ++)
    {
        if ( string [i] == '<')
        {
            start = i ;
            break;
        }
    }
    int cpt = 0;
    for (int i = start ; i < strlen(string) ; i ++)
    {
        if ( string [i] != '>')
        {
            cpt++;
        }
        else
        {
            cpt++;
            break;
        }
    }
    char * balise = NULL;
    balise = malloc(sizeof(char)* (cpt+2));
    balise = get_string_in_string(string, start,cpt+1);
    balise[cpt+1] = '\0';

    if ( strstr(balise, "<?xml") != NULL && strstr(balise, "?>") != NULL)
    {
        string = &string[cpt+2];
        root_tag->start += cpt+2;
        return get_root_tag(string,root_tag);
    }
    return get_root_tag(string,root_tag);
}

root_tag * validation_root_tag(char * filename,int external_dtd)
{
    FILE * file = open_file("my_dtd.txt", "r");
    if (file != NULL)
    {
        char * string = file_to_string(file);
        return is_xml_declaration(string);
    }

    return NULL;
}

char* remove_space_chevron(char* string, int cpt)
{
    for ( int i = 0; i < cpt-2 ; i ++)
    {
        if ( ( string[i+1] == ' ' && string[i+2] == '=') || (string[i] == '=' && string[i+1] == ' ' && string[i+2] != ' ') || (string[i] != ' ' && string[i+1] == ' ' && string[i+2] == '\n') || (string[i] != ' ' && string[i+1] == ' ' && string[i+2] == '>') || (string[i] != ' ' && string[i+1] == ' ' && string[i+2] == ')') || (string[i] == ' ' && string[i+1] == ' ' && string[i+2] == ' '))
        {
            for ( int j = i ; j < cpt; j++)
            {
                string[j+1] = string[j+2];
            }
        }
        else if ( string[i] == '\n' && string[i+1] == '\n')
        {
            string[i] = ' ';
        }
    }
    return string;
}

// overwrite the file with the text cleaned
void get_file_cleaned(FILE* file, int cpt, char* cleaned_file)
{
    char c;
    char previous_char = '*';

    while ((c = getc(file)) != EOF)
    {
        if (c == '\n'  && '<' != previous_char || (c == '\n' && previous_char == '\n') )
        {
            cleaned_file[cpt] = ' ';
            cpt++;
        }
        else if (c == ' ' && c != previous_char) {
            cleaned_file[cpt] = c;
            cpt++;
        }
        else if(c != ' ' && previous_char != ' ' )
        {
            cleaned_file[cpt] = c;
            cpt++;
        }
        else if(c != ' ' && previous_char == ' ' )
        {
            cleaned_file[cpt] = c;
            cpt++;
        }

        previous_char = c;
    }
    cleaned_file[cpt] = '\0';
    cleaned_file = remove_space_chevron(cleaned_file,cpt);
    file = fopen("my_dtd.txt", "w+");
    fputs(cleaned_file,file);
    fclose(file);
}

// clean the file by couting only 1 space between 2 carac
void clean_file(FILE* file)
{

    char current_char = getc(file);
    char next_char ;
    int cpt = 1;

    while ((next_char = getc(file)) != EOF)
    {
        cpt ++;
        if (current_char == ' ' && (next_char == ')' || next_char == '>'))
        {
            cpt--;
        }
        current_char = next_char;
    }

    char* cleaned_file = NULL;
    cleaned_file = malloc(sizeof(char)*(cpt+1));
    cpt = 0;
    fseek(file, 0, SEEK_SET);

    cleaned_file[cpt] = '\0';
    get_file_cleaned(file, cpt, cleaned_file);
    free(cleaned_file);
}

char * get_next_word(char * string, int * ptr_i )
{
    printf(" ma string  2 : %s \n ", string);
    int i = 0;

    while( string[i] != ' ' && string[i] != '>' && string[i] != '\n' && string[i] != '\0' )
    {
        i++;
    }
    char * name = malloc(sizeof(char)* (i+1));
    for (int j = 0; j < i ; j++)
    {
        name[j] = string[j];
    }
    name[i] = '\0';
    if (ptr_i != NULL) {
        *ptr_i = i;
    }

    return name;
}

// get a substring of a string by giving the index of the start and the end index
char* get_string_in_string(char * string, int start_cpt,int  cpt)
{
    char* name = malloc(sizeof(char)*(cpt-start_cpt+1));

    for (int i = 0  ; i < (cpt - start_cpt) ; i ++)
    {
        name[i] = string[start_cpt+i];
    }
    name[cpt-start_cpt] = '\0';

    return name;
}

void get_name_element(elements* element, char * string, int* ptr_cpt) {
    int cpt = *ptr_cpt;
    int start_cpt = cpt;

    while (string[cpt] != ' ' && string[cpt] != '\0') {
        cpt++;
    }

    element->name = get_string_in_string( string, start_cpt, cpt);
    *ptr_cpt = cpt+1;
}

int verify_string(char* tmp_string)
{
    for (int i = 0; i < strlen(tmp_string); i++)
    {
        if ( (tmp_string[i] == '(' && i == 0) || (tmp_string[strlen(tmp_string)-1] == ')' && i == (strlen(tmp_string)-1) ))
        {
            continue;
        }
        if (tmp_string[i] != '|' && tmp_string[i] != ',')
        {
            if ( !(tmp_string[i] >= 48 && tmp_string[i] <= 57) )
            {
                if ( !(tmp_string[i] >= 65 && tmp_string[i] <= 90) )
                {
                    if ( !(tmp_string[i] >= 97 && tmp_string[i] <= 122) )
                    {
                        if ( (tmp_string[i] == '*' || tmp_string[i] == '+' || tmp_string[i] == '?') && tmp_string[i+1] != ',' && tmp_string[i+1] != '|' && tmp_string[i+1] != ')') {
                            printf(" c'est pas bon : %c / %d \n", tmp_string[i], i);
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

bool is_child_list_correct(char* tmp_string)
{
    int cpt_1 = 0;
    int cpt_2 = 0;
    for (int j = 0 ; j < strlen(tmp_string); j++)
    {
        if( tmp_string[j] == ',')
        {
            cpt_1 ++;
        }
        if( tmp_string[j] == '|')
        {
            cpt_2 ++;
        }
    }
    if (cpt_1 != 0 && cpt_2 != 0) {
        //printf("impossible d'avoir ',' et '|' \n ");
        return false;
    }
    else
    {
        return true;
    }
}

void get_child_list(elements* element, char* tmp_string, char value)
{
    int cpt = 0;
    int cpt_bis = 0;
    int tab[10] = {0};
    for (int i = 0; i < strlen(tmp_string); i++)
    {
        if ( tmp_string[i] == value)
        {
            cpt ++;
            cpt_bis++;
        } else
        {
            tab[cpt_bis] ++;
        }
    }
    element->size_list_childs = cpt+1;
    element->childs = malloc(sizeof(char)* (cpt+1));
    int cursor = 0;
    for ( int j = 0; j <= cpt ; j++)
    {
        element->childs[j] = malloc(sizeof(char*) * (tab[j]+1) );
        if (j == 0 )
        {
            strcpy(element->childs[j], get_string_in_string(tmp_string,cursor+1,tab[j]));
        } else if (j == cpt)
        {
            strcpy(element->childs[j], get_string_in_string(tmp_string,cursor,tab[j]+cursor-1));
        }
        else {
            strcpy(element->childs[j], get_string_in_string(tmp_string, cursor, tab[j] + cursor));
        }
        element->childs[j][tab[j]] = '\0';

        cursor += tab[j]+1;
    }

}

void get_child_element(elements* element,char* tmp_string)
{

    if(is_child_list_correct(tmp_string))
    {
        if(strchr(tmp_string,','))
        {
            get_child_list(element,tmp_string, ',');
            element->ordered = true;
        }
        else if(strchr(tmp_string,'|'))
        {
            get_child_list(element,tmp_string, '|');
            element->ordered = false;
        }
        else
        {
            element->childs = malloc(sizeof(char)*1);
            element->childs[0] = malloc(sizeof(char*)*strlen(tmp_string-1));
            element->childs[0] = get_string_in_string(tmp_string,1,strlen(tmp_string)-1);
            element->size_list_childs = 1;
        }
    }
}

void analyse_tmp_string(elements* element,char* tmp_string)
{

    if ( strstr(tmp_string,"(#PCDATA)"))
    {
        element->type = "(#PCDATA)";

    }
    else if ( strstr(tmp_string,"EMPTY") )
    {
        element->type = "EMPTY";
    }
    else if ( strstr(tmp_string,"ANY") )
    {
        element->type = "ANY";
    }
    else if ( tmp_string[0] == '(' && tmp_string[strlen(tmp_string)-1] == ')')
    {
        element->type = "LISTE";
        if ( verify_string(tmp_string) )
        {
            get_child_element(element, tmp_string);
        }
    }
}

void get_type_element(elements* element, char * string, int* ptr_cpt) {
    int cpt = *ptr_cpt;
    int start_cpt = cpt;

    while (string[cpt] != '>' && string[cpt] != ' ' && string[cpt] != '\0') {
        cpt++;
    }
    char* tmp_string = malloc(sizeof(char)*(cpt+1));
    tmp_string = get_string_in_string(string, start_cpt, cpt);

    analyse_tmp_string(element, tmp_string);
}

void create_element(char* string, liste_element * liste)
{
    int cpt = strlen("<!ELEMENT")+1;
    int* ptr_cpt = &cpt;

    elements * element = calloc(1,sizeof(elements));

    get_name_element(element,string, ptr_cpt);
    get_type_element(element,string, ptr_cpt);
    element->next = liste->first;
    liste->first = element;
}

void get_type_attribute(elements* element, char * string, int* ptr_cpt) {
    int cpt = *ptr_cpt;
    int start_cpt = cpt;

    while (string[cpt] != '>' && string[cpt] != ' ' && string[cpt] != '\0') {
        cpt++;
    }
    char* tmp_string = malloc(sizeof(char)*(cpt+1));
    tmp_string = get_string_in_string(string, start_cpt, cpt);

    analyse_tmp_string(element, tmp_string);

    *ptr_cpt = cpt+1;
}

int is_validate_obligation(char * string, attributes * attribute, int id)
{
    if (strcmp(string,"#IMPLIED") == 0)
    {
        attribute->obligation = string;
        string = &string[strlen(string)+1];
        super_cpt += strlen(attribute->obligation)+1;
    }
    else if (strcmp(string,"#REQUIRED") == 0)
    {
        attribute->obligation = string;
        string = &string[strlen(string)+1];
        super_cpt += strlen(attribute->obligation)+1;

    }
    else if (strcmp(string,"#FIXED") == 0)
    {
        attribute->obligation = string;
        string = &string[strlen(string)+1];
        super_cpt += strlen(attribute->obligation)+1;
        if ( id == 1 )
        {
            attribute->error_string = "Un attribut de type ID ne peux etre #FIXED";
        }
    }
    else
    {
        attribute->obligation = string;
        string = &string[strlen(string)+1];
        super_cpt += strlen(attribute->obligation)+1;
    }
    return 1;
}

int switch_on_type_attribute(char * type)
{
    if ( strcmp(type, "ID") == 0 )
    {
        return 1;
    }
    else if ( strcmp(type, "IDREF") == 0 )
    {
        return 2;
    }
    else if ( strcmp(type, "IDREFS") == 0 )
    {
        return 3;
    }
    else if ( strcmp(type, "NMTOKEN") == 0 )
    {
        return 4;
    }
    else if ( strcmp(type, "NMTOKENS") == 0)
    {
        return 5;
    }
    else
    {
        return 6;
    }
}

void get_liste_type(char* type, attributes * attribute)
{
    int cpt = 0;
    int array[15] = {0};
    int j = 0 ;
    int size = 1;
    for (int i = 0; i < strlen(type) ; i ++)
    {
        if (type[i] == '|')
        {
            cpt ++;
            j++;
        }
        if ( type[i] != '(' && type[i] != '|' && type[i] != ')' )
        {
            array[j] ++;
        }
    }
    attribute->size_liste_values = cpt+1;
    attribute->liste_values = malloc(sizeof(char)* (cpt+1));
    for (int i = 0; i < cpt+1; ++i)
    {
        attribute->liste_values[i] = malloc(sizeof(char*)*array[i]+1);

        attribute->liste_values[i] = get_string_in_string(type,size,size+array[i]);
        strcat(attribute->liste_values[i], "\0");
        size += (array[i]+1);
    }

}


void analyse_type_attribute(char * string,char * type, attributes * attribute, int * ptr_i)
{
    attribute->type = type;
    int i = 0 ;
    int * ptr_j = &i;
    char * next_token = get_next_word(string,ptr_j);
    int id = switch_on_type_attribute(type);
    if ( (strcmp(type, "CDATA") == 0) || (strcmp(type, "ID") == 0) || (strcmp(type, "IDREFS") == 0) || (strcmp(type, "NMTOKEN") == 0) || (strcmp(type, "NMTOKENS") == 0) || type[0] == '(' )
    {
        if ( type[0] == '(')
        {
            verify_string(type);
            get_liste_type(type, attribute);
        }
        string = &string[strlen(next_token)+1];
        if (next_token[0] == '"')
        {
            attribute->default_value = get_string_in_string(next_token,1,strlen(next_token)-1);
            string = &string[strlen(attribute->default_value)+3];
            super_cpt += strlen(attribute->default_value)+3;
            if ( id == 1 )
            {
                attribute->error_string = "Un attribut de type ID doit etre #REQUIRED ou #IMPLIED";
            }
        }
        else if (next_token[0] == '#')
        {

            if ( is_validate_obligation(next_token,attribute,id))
            {

                *ptr_j = 0;
                next_token = get_string_in_string(get_next_word(string,ptr_j ),0,strlen(get_next_word(string,ptr_j )));
                attribute->default_value = next_token;
                if (next_token[0] == '"')
                {

                    attribute->default_value = get_string_in_string(next_token,0,strlen(next_token));
                    string = &string[strlen(attribute->default_value)+3];
                    super_cpt += strlen(attribute->default_value)+1;
                    if ( (strcmp(attribute->obligation, "#REQUIRED") == 0) || (strcmp(attribute->obligation, "#IMPLIED") == 0) )
                    {
                        attribute->error_string = "Un attribut #REQUIRED ou #IMPLIED ne peut pas avoir de valeur par defaut";
                    }
                }
            }
        }
    }
    else if ( strcmp(type, "ID") == 0 )
    {
        // peut pas etre fixed  que required ou implied
    }
    else if ( strcmp(type, "IDREF") == 0 )
    {
        // peut pas etre fixed  que required ou implied
    }
    else if ( strcmp(type, "IDREFS") == 0 )
    {
        // peut pas etre fixed  que required ou implied
    }
    else if ( strcmp(type, "NMTOKEN") == 0 )
    {
        // Need default value si pas d'obligation
    }
    else if ( strcmp(type, "NMTOKENS") == 0)
    {
        // Need default value si pas d'obligation
    }
}

void get_attribute_options(attributes * attribute,char* string, int * ptr_i)
{
    int i = 0 ;
    int * ptr_j = &i;
    char * type = get_next_word(string,ptr_j);
    string = &string[i+1];
    super_cpt += i+1;
    analyse_type_attribute(string, type, attribute,ptr_i);
}

void exctract_attributes(char * string, liste_attributes * liste,char * ref)
{
    int i=0;
    int * ptr_i = &i ;
    char* name = get_next_word(string,ptr_i);
    liste->numb_attributes += 1;
    attributes * attribute = calloc(1,sizeof(attributes));
    attribute->ref_element = ref;
    attribute->name = name;
    string= &string[strlen(name)+1];
    super_cpt += strlen(name)+1;
    get_attribute_options(attribute, string,ptr_i);
    if(strlen(string) != 0) {

        if(liste->first == NULL)
        {
            liste->first = attribute;
        } else
        {
            attribute->next = liste->first;
            liste->first = attribute;
        }
    }
}

void get_name_attribute(char * string, int* ptr_cpt, liste_element* liste_element, liste_attributes* liste)
{

    int cpt = *ptr_cpt;
    int start_cpt = cpt;
    while (string[cpt] != ' ' && string[cpt] != '\0' && string[cpt] != '>') {
        cpt++;
    }

    char * test = malloc(sizeof(char)*(strlen(get_string_in_string( string, start_cpt, cpt))+1));
    test = strcpy(test,get_string_in_string( string, start_cpt, cpt));
    test[strlen(test)+1] = '\0';
    bool found_element = false ;
    string = &string[strlen(test)+1+*ptr_cpt];
    elements * element ;
    element = liste_element->first;
    super_cpt = 0;
    //char ** ptr_string = &string;
    while ( element != NULL)
    {
        if ( (strcmp( element->name, test) == 0))
        {
            found_element = true;

            while ( strlen(string) > 0 )
            {
                exctract_attributes(string, liste, test);
                string = &string[super_cpt];

                super_cpt = 0;
            }
        }
        element = element->next;
    }
    if ( found_element == false)
    {
        //attribute->error_string = "pas d'element correspondant";
        liste->error +=1;
    }
    *ptr_cpt = cpt+1;

}

void verify_attribute(char* string,liste_attributes * liste, liste_element *liste_element)
{
    int cpt = strlen("<!ATTLIST")+1;
    int* ptr_cpt = &cpt;
    liste->error = 0;
    get_name_attribute(string, ptr_cpt,liste_element,liste);

    //get_type_attribute(attribute,string, ptr_cpt,liste_element);

}

int get_number_element(FILE* file,int cpt,liste_element * liste)
{
    char c ;
    while ( (c = getc(file)) != '>')
    {
        cpt ++;
    }
    cpt ++;
    fseek(file, -cpt, SEEK_CUR);
    char* string = malloc(sizeof(char)*(cpt+1));

    for (int i = 0 ; i < cpt ; i ++)
    {
        string[i] = fgetc(file);

    }
    string[cpt] = '\0';
    if (strstr(string,"ELEMENT "))
    {
        if (strstr(string,"<!ELEMENT "))
        {
            liste->numb_element +=1;
            create_element(string,liste);
            return 1;
        }
        else
        {
            //printf("Erreur syntax element ");
        }
    }
    return 0;
}

int get_number_attribute(FILE* file,int cpt,liste_element * liste_element, liste_attributes * liste)
{
    char c ;
    while ( (c = getc(file)) != '>')
    {
        cpt ++;
    }
    cpt ++;
    fseek(file, -cpt, SEEK_CUR);
    char* string = malloc(sizeof(char)*(cpt+1));
    for (int i = 0 ; i < cpt ; i ++)
    {
        string[i] = fgetc(file);
    }
    string[cpt] = '\0';

    if (strstr(string,"ATTLIST "))
    {
        if (strstr(string,"<!ATTLIST "))
        {
            verify_attribute(string,liste,liste_element);
            return 1;
        }
        else
        {
            printf("Erreur syntax attribut ");
        }
    }
    return 0;
}

void get_elements(FILE* file, liste_element * liste)
{
    char c ;
    int cpt = 0 ;
    int cpt_element = 0;
    fseek(file, 0, SEEK_SET);
    while ( (c = getc(file)) != EOF)
    {
        if ( c == '<')
        {
            cpt++;
            cpt_element += get_number_element(file,cpt,liste);
            cpt = 0 ;
        }
    }
    printf("il y a %d element \n", liste->numb_element);
}


void get_attributes(FILE* file, liste_attributes * liste,liste_element * liste_element)
{
    char c ;
    int cpt = 0 ;
    int cpt_attribute = 0;

    while ( (c = getc(file)) != EOF)
    {
        if ( c == '<')
        {
            cpt++;
            cpt_attribute += get_number_attribute(file,cpt,liste_element,liste);
            cpt = 0 ;
        }
    }
    printf("il y a %d attribute \n", liste->numb_attributes);
}

void get_balise_attribute(char* tmp, xml_balise * balise)
{
    printf("get_balise_attribute\n" );
    int i = 0;
    while (tmp[i] != '=')
    {
        i++;
    }

    balise->attribute = realloc(balise->attribute, 1 * sizeof(*balise->attribute));
    balise->attribute_nb ++;
    balise->attribute_value = realloc(balise->attribute_value, 1 * sizeof(*balise->attribute_value));
    balise->attribute[balise->attribute_nb-1] = get_string_in_string(tmp,0,i);
    printf(" mon name attribute 1 %s \n", balise->attribute[balise->attribute_nb-1]);
    printf(" ma tmp 1 %s \n", tmp);
    if (strlen(balise->attribute[balise->attribute_nb-1]) != strlen(tmp) ) {
        tmp = &tmp[strlen(balise->attribute[balise->attribute_nb - 1]) + 1];
    }
    printf(" ma tmp 2 %s \n", tmp);
    balise->attribute_value[balise->attribute_nb-1] = get_string_in_string(tmp, 1, strlen(tmp)-1);
    printf(" la velur de mon attribute = %s \n ", balise->attribute_value[balise->attribute_nb-1] );

}

// prendre en compte les erreurs de syntaxe
void check_balise_attribute(char* string, xml_balise * balise)
{
    balise->name = get_next_word(string,NULL);
    string = &string[strlen(balise->name)+1];

    char * tmp;
    while (strlen(string) > 0)
    {
        tmp = get_next_word(string,NULL);
        get_balise_attribute(tmp, balise);
        string = &string[strlen(tmp)+1];
    }
}

void check_balise_content(char* balise,xml_balise * root, int mother_length) {
    char *balise_content;
    char *new_balise_content;
    char *open_balise = balise_builder(root->name, 0);
    char *closed_balise = balise_builder(root->name, 1);
    printf(" mes balise = %s \n ", balise);
    balise_content = get_string_in_string(balise, mother_length, strstr(balise, closed_balise) - balise);
    printf(" mes balise = %s \n ", balise_content);
    printf(" is a child present %d \n", is_a_child(balise_content));
    if (is_a_child(balise_content) == 0) {
        printf(" je n'i pas d'enfant \n");
        if (strlen(new_balise_content) > 0) {
            root->text = 1;
        }
        balise_content[0] = '\0';
        printf(" ma taille :::::: %s \n", balise_content);
        super_cpt_bis = 1;

    }

    printf(" ma taille 2  :::::: %d \n", strlen(balise_content));

if (super_cpt_bis == 0)
    {
    while (strlen(balise_content) > 1) {
        root->child = realloc(root->attribute, 1 * sizeof(*root->attribute));
        root->child_nb++;
        root->child[root->child_nb - 1] = get_balise(balise_content);
        printf(" mon nom enfant : %s \n ", root->child[root->child_nb - 1]);
        char *child_balise = balise_builder(root->child[root->child_nb - 1], 1);
        printf("test : %s \n", balise_content);
        
        if (strstr(balise_content, child_balise) != NULL) {
            new_balise_content = get_string_in_string(balise_content, 1,
                                                      strstr(balise_content, child_balise) + strlen(child_balise) -
                                                      balise_content);
            printf("je rentre dans le if  : %s \n", new_balise_content);
            if (is_a_child(new_balise_content) && super_cpt_bis == 0) {
                printf(" HIHIHIHIHIHIHIHHIHIHIHIHIHIH \n");
                xml_reader_v2(new_balise_content);
            } else {
                if (strlen(new_balise_content) > 0) {
                    root->text = 1;
                }
                new_balise_content[0] = '\0';
            }
            //new_balise_content = calloc(1, sizeof(char) * (strlen(balise_content) - strlen(child_balise) + 1));
            //printf("test : %s \n", balise_content);
            //new_balise_content = get_string_in_string(balise_content, 0,strlen(balise_content) - strlen(child_balise) - 1);
            //printf("test : %s \n", new_balise_content);
            //new_balise_content[strlen(new_balise_content) - 1] = '\0';
        } else {
            root->error_string = "il y a une erreur au niveau des balises enfant";
            balise_content[0] = '\0';
            printf(" strlen de balise : %d \n", strlen(balise_content));
        }
    }

    }
}

int is_a_child(char * string)
{
    int cpt_1 = 0;
    int cpt_2 = 0;
    for (int i = 0 ; i < strlen(string); i ++)
    {
        if ( string[i] == '<')
        {
            cpt_1 ++;
        }
        if ( string[i] == '>')
        {
            cpt_2 ++;
        }
    }
    if ( cpt_1 == cpt_2 && cpt_1 != 0 )
    {
        return 1;
    }

    return 0;
}

void XML_reader (FILE * file, liste_element * liste_element, liste_attributes * liste_attribute, root_tag * rootTag)
{
    char * xml ;
    char * balise;
    xml = get_xml(file);
    balise = get_balise(xml);
    printf(" mon nom de balise : %s \n", get_next_word(balise,NULL));

    xml_balise * root ;
    root = calloc(1, sizeof(xml_balise));
    check_balise_attribute(balise, root);
    printf(" ///////////////// %s \n", xml);
    printf(" ///////////////// %s \n", balise);
    check_balise_content(xml,root,strlen(balise)+2);
}

void xml_reader_v2(char * xml)
{
    char *balise = get_balise(&(*xml));
    printf(" mon nom de balise : %s \n", get_next_word(balise,NULL));

    xml_balise * root ;
    root = calloc(1, sizeof(xml_balise));
    check_balise_attribute(balise, root);
    printf(" ///////////////// %s \n", xml);
    if ( super_cpt_bis == 1)
    {
        return;
    }
    printf(" ///////////////// %s \n", balise);
    check_balise_content(xml,root,strlen(balise)+2);
}

char * get_balise(char * string)
{
    super_cpt = 0;
    int i = 0;
    while (string[i] != '<')
    {

        i ++;
    }
    i++;
    super_cpt = i;
    while ( string [super_cpt] != '>' )
    {
        super_cpt ++ ;
    }

    char * balise = malloc(sizeof(char)* (super_cpt-i));
    balise = get_string_in_string(string,i,super_cpt);

    return balise;
}

char * get_xml(FILE * file)
{
    super_cpt = 0;
    char* my_file = file_to_string(file);
    printf(" %d ", strlen(my_file));
    for (int i = 0 ; i < strlen(my_file); i ++)
    {
        super_cpt ++ ;
        if ( my_file[i] == ']')
        {
            printf(" ] %d ", i );
            super_cpt +=2;
            break;
        }
    }
    return get_string_in_string(my_file,super_cpt,strlen(my_file));
}

char * balise_builder(char * balise_name, int type)
{
    char * balise;
    printf(" ma balise name : %s \n" , balise_name);
    if ( type == 1) {
        balise = malloc(sizeof(char) * (strlen(balise_name) + 3));
        strcat(balise, "</");
        strcat(balise, balise_name);
        strcat(balise, ">");
    }
    else
    {
        balise = malloc(sizeof(char) * (strlen(balise_name) + 2));
        strcat(balise, "<");
        strcat(balise, balise_name);
        strcat(balise, ">");
    }

    return balise;
}


