//
// Created by 20220848 on 21/04/2024.
//

#ifndef PROJET_C_WITH_WARREN_CDATAFRAME_LIST_H
#define PROJET_C_WITH_WARREN_CDATAFRAME_LIST_H
#include "column_list.h"

// Pour l'utilisation d'un dataframe chainée
typedef struct ColumnList {
    COLUMN *column;
    struct ColumnList *succ;
} ColumnList;

// Pour l'utilisation d'un dataframe dynanique
typedef struct {
    COLUMN** columns;
    ColumnList *head; // Pointeur vers la tête de la liste de colonnes
    int num_columns;
} CDATAFRAME;

CDATAFRAME *create_empty_cdataframe();

CDATAFRAME *create_cdataframe(ENUM_TYPE *cdftype, int size);

void delete_cdataframe(CDATAFRAME **cdf);

void delete_column_by_name(CDATAFRAME *cdf, char *col_name);

void delete_column_by_index(CDATAFRAME *dataframe, int index);

void delete_all_columns(CDATAFRAME *dataframe);

void delete_columns_range(CDATAFRAME *dataframe, int start, int end);

void create_column_process(CDATAFRAME *dataframe);

void insert_value_process(CDATAFRAME *dataframe);

int insert_single_value(COLUMN* col);

void print_columns_process(CDATAFRAME *dataframe);

void search_columns_process(CDATAFRAME *dataframe);

void sort_columns_process(CDATAFRAME *dataframe);

void sort_multiple_columns(CDATAFRAME *dataframe, int start, int end);

void sort_single_column(CDATAFRAME *dataframe, int index);

void delete_columns_process(CDATAFRAME *dataframe);

void menu2();

void explain_cdataframe();

void clean_stdin();

void print_multiple_cols(CDATAFRAME *dataframe, int start, int end);

void print_limited_rows(CDATAFRAME *dataframe);

void handle_column_display(CDATAFRAME *dataframe);

void print_single_row(CDATAFRAME *dataframe, int row_index);

void handle_row_display(CDATAFRAME *dataframe);



#endif //PROJET_C_WITH_WARREN_CDATAFRAME_LIST_H
