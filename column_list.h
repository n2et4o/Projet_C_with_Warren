//
// Created by 20220848 on 31/03/2024.
//

#ifndef PROJET_C_WITH_WARREN_COLUMN_LIST_H
#define PROJET_C_WITH_WARREN_COLUMN_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Pour sleep()
#include <windows.h> // Utilisation de code ASCII etendu
#define REALOC_SIZE 256
#define ASC 0
#define DESC 1
#define MAX_COLUMNS 10  // Nous pouvez augmenter cette taille selon les besoins
#define MAX_TITLE_LENGTH 50
#define MAX_COLUMNS 50  // Définissez une limite sur le nombre de colonnes


void menu();

typedef struct {
    int id;
    char name[50];
    float value;
    double value1;
} MyStruct;


enum enum_type
{
    NULLVAL = 1 , MY_UINT, MY_INT, MY_CHAR, MY_FLOAT, MY_DOUBLE, STRING, STRUCTURE
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
    int value;
};
typedef union column_type COL_TYPE ;


typedef struct liste {
    void* val;
    struct liste* succ;
} LISTE;


struct column_list {
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
    LISTE * head;    // Tête de la liste chaînée des données
    struct column_list *next; // Prochaine colonne dans la liste chaînée
};
typedef struct column_list COLUMN;



/**
* Create a column_list
* @param1 : Column title
* @return : Pointer to created column_list
*/
COLUMN* create_column(ENUM_TYPE type, char* title);

/**
* @brief : Add a new value to a column_list
* @param1 : Pointer to a column_list
* @param2 : The value to be added
* @return : 1 if the value is added 0 otherwise
*/
int insert_value(COLUMN *col, void *value);

/**
* @brief : Free allocated memory
* @param1 : Pointer to a column_list
*/
void delete_column(COLUMN **col);

/**
* @brief: Print a column_list content
* @param: Pointer to a column_list
*/
void print_col(COLUMN* col);

void nb_occ(COLUMN* col, void* x);

void pos_val(COLUMN* col, unsigned int x);

void nb_val_supe(COLUMN* col, void* x);

void nb_val_inf(COLUMN* col, void* x);

void nb_val_egal(COLUMN* col, void* x);

void convert_value(COLUMN *col, unsigned long long int i, char *str, int size);

//void quicksort(COLUMN *col, int left, int right);
//void insertion_sort(COLUMN *col);
void sort(COLUMN* col, int sort_dir);
void print_col_by_index(COLUMN *col);
//int compare(COLUMN *col, COL_TYPE *a, COL_TYPE *b);


void quicksort(COL_TYPE **data, int left, int right, COLUMN *col);
void insertion_sort(COL_TYPE **data, int n, COLUMN *col);
int compare(COL_TYPE *a, COL_TYPE *b, COLUMN *col);

void quickSort(COLUMN* col);
LISTE* quickSortRecur(LISTE* head, LISTE* end, COLUMN* col);
LISTE* partition(LISTE* head, LISTE* end, LISTE** newHead, LISTE** newEnd, COLUMN* col);
void linked_list_insertion_sort(COLUMN* col);
LISTE* find_previous(LISTE* head, LISTE* node);





#endif //PROJET_C_WITH_WARREN_COLUMN_LIST_H
