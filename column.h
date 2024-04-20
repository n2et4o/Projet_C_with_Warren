//
// Created by 20220848 on 31/03/2024.
//

#ifndef PROJET_C_WITH_WARREN_COLUMN_H
#define PROJET_C_WITH_WARREN_COLUMN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Pour sleep()
#include <locale.h> // Utilisation de code ASCII etendu
#define REALOC_SIZE 256
#define ASC 0
#define DESC 1
#define MAX_COLUMNS 10  // Nous pouvez augmenter cette taille selon les besoins


void menu();

typedef struct {
    int id;
    char name[50];
    float value;
    double value1;
} MyStruct;


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
    // index valid
    // 0 : no index
    // -1 : invalid index
    // 1 : valid index
    int valid_index;
    // direction de tri Ascendant ou Déscendant
    // 0 : ASC
    // 1 : DESC
    int sort_dir;
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

void nb_occ(COLUMN* col, void* x);

void pos_val(COLUMN* col, unsigned int x);

void nb_val_supe(COLUMN* col, void* x);

void nb_val_inf(COLUMN* col, void* x);

void nb_val_egal(COLUMN* col, void* x);

void convert_value(COLUMN *col, unsigned long long int i, char *str, int size);

void quicksort(COLUMN *col, int left, int right);
void insertion_sort(COLUMN *col);
void sort(COLUMN* col, int sort_dir);
void print_col_by_index(COLUMN *col);
int compare(COLUMN *col, COL_TYPE *a, COL_TYPE *b);







#endif //PROJET_C_WITH_WARREN_COLUMN_H
