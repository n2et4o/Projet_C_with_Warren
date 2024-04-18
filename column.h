//
// Created by 20220848 on 31/03/2024.
//

#ifndef PROJET_C_WITH_WARREN_COLUMN_H
#define PROJET_C_WITH_WARREN_COLUMN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define REALOC_SIZE 256


enum enum_type
{
    NULLVAL = 1 , UINT, INT, CHAR, FLOAT, DOUBLE, STRING, STRUCTURE
};
typedef enum enum_type ENUM_TYPE;

union column_type{
    unsigned int uint_value;
    signed int int_value;
    char char_value;
    float float_value;
    double double_value;
    char* string_value;
    void* struct_value;
};
typedef union column_type COL_TYPE ;

struct column {
    char *title;
    unsigned int size; //logical size
    unsigned int max_size; //physical size
    ENUM_TYPE column_type;
    COL_TYPE **data; // array of pointers to stored data
    unsigned long long int *index; // array of integers
};
typedef struct column COLUMN;


/**
* Create a column
* @param1 : Column title
* @return : Pointer to created column
*/
COLUMN* create_column(ENUM_TYPE type, char* title);

/**
* @brief : Add a new value to a column
* @param1 : Pointer to a column
* @param2 : The value to be added
* @return : 1 if the value is added 0 otherwise
*/
int insert_value(COLUMN *col, void *value);

/**
* @brief : Free allocated memory
* @param1 : Pointer to a column
*/
void delete_column(COLUMN **col);

/**
* @brief: Print a column content
* @param: Pointer to a column
*/
void print_col(COLUMN* col);

int nb_occ(COLUMN* col, int x);

int pos_val(COLUMN* col, unsigned int x);

int nb_val_supe(COLUMN* col, int x);

int nb_val_inf(COLUMN* col, int x);

int nb_val_egal(COLUMN* col, int x);

void convert_value(COLUMN *col, unsigned long long int i, char *str, int size);





#endif //PROJET_C_WITH_WARREN_COLUMN_H
