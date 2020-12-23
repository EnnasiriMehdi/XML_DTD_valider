#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

typedef struct elements elements;
typedef struct attributes attributes;
typedef struct root_tag root_tag;
typedef struct xml_balise xml_balise;

struct xml_balise
{
    char * name;
    char ** child;
    char ** attribute;
    char ** attribute_value;
    int * occurence;
    int child_nb;
    int attribute_nb;
    bool text ;
    char * error_string;
};


struct root_tag
{
    char * name ;
    char * url ;
    int start;
};

struct elements
{
    char* name;
    char* type;
    bool ordered;
    int size_list_childs;
    char** childs;
    elements * next;
};

struct attributes
{
    char* ref_element;
    char* name;
    char* type;
    char** liste_values;
    int size_liste_values;
    char* obligation;
    char* default_value;
    attributes * next;
    char* error_string;
};

typedef struct liste_element
{
    elements * first ;
    int numb_element;
    elements * last ;
}liste_element;

typedef struct liste_attributes
{
    attributes * first ;
    int numb_attributes;
    int error;
    attributes * last ;
}liste_attributes;

root_tag * get_root_tag(char * string, root_tag * root_tag);
root_tag * validation_root_tag(char * filename,int external_dtd);
root_tag *  is_xml_declaration(char * string);
char * file_to_string (FILE * file);

void clean_start(char * string, root_tag * root_tag);
char* remove_space_chevron(char* string, int cpt);
char * get_next_word(char * string, int * i );


void get_elements(FILE * file, liste_element * liste );
int get_number_element(FILE* file,int cpt,liste_element * liste);
void get_last_line(FILE* file,int cpt, int* cpt_element_ptr);
void clean_file(FILE* file);
void get_file_cleaned(FILE* file, int cpt, char* cleaned_file);
FILE* open_file(char* filename, char * option);
void create_element(char* string, liste_element * liste);
void get_name_element(elements* element, char * string, int* ptr_cpt);
char* get_string_in_string(char * string, int start_cpt,int  cpt);
void get_type_element(elements* element, char * string, int* ptr_cpt);
void analyse_tmp_string(elements* element,char* tmp_string);
int verify_string(char* tmp_string);
void get_child_element(elements* element,char* tmp_string);
bool is_child_list_correct(char* tmp_string);
void get_child_list(elements* element,char* tmp_string, char value);
void get_attributes(FILE* file, liste_attributes * liste,liste_element * liste_element);
int get_number_attribute(FILE* file,int cpt,liste_element * liste_element, liste_attributes * liste);
void verify_attribute(char* string,liste_attributes * liste, liste_element *liste_element);
void get_name_attribute(char * string, int* ptr_cpt, liste_element* listeElement, liste_attributes* liste);
void exctract_attributes(char * string, liste_attributes * liste, char * ref);
void get_attribute_options(attributes * attribute,char*  string, int * ptr_i);
void analyse_type_attribute(char * string,char * type, attributes * attribute, int * ptr_i);
int is_validate_obligation(char * string, attributes * attribute,int id);
int switch_on_type_attribute(char * type);
void get_liste_type(char* type, attributes * attribute);



void XML_reader (FILE * file, liste_element * liste_element, liste_attributes * liste_attribute, root_tag * rootTag);
char * get_xml(FILE * file);
char * balise_builder(char * balise_name, int type);
char * get_balise(char * string);
void check_balise_attribute(char* string, xml_balise * balise);
void get_balise_attribute(char* tmp, xml_balise * balise);
void check_balise_content(char* balise,xml_balise * root, int mother_length);
void xml_reader_v2(char * xml);
int is_a_child(char * string);



#endif