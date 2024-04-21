//
// Created by 20220848 on 21/04/2024.
//

#ifndef PROJET_C_WITH_WARREN_CDATAFRAME_H
#define PROJET_C_WITH_WARREN_CDATAFRAME_H
#include "column.h"
#define MAX_TITLE_LENGTH 50


typedef struct {
    COLUMN** columns;
    int num_columns;
} CDATAFRAME;

CDATAFRAME *create_empty_cdataframe();

CDATAFRAME *create_cdataframe(ENUM_TYPE *cdftype, int size);

void delete_cdataframe(CDATAFRAME **cdf);

void delete_column_by_name(CDATAFRAME *cdf, char *col_name);

void create_column_process(CDATAFRAME *dataframe);

void insert_value_process(CDATAFRAME *dataframe);

int insert_single_value(COLUMN* col);

void print_columns_process(CDATAFRAME *dataframe);

void search_columns_process(CDATAFRAME *dataframe);

void sort_columns_process(CDATAFRAME *dataframe);

void delete_columns_process(CDATAFRAME *dataframe);

void menu2();

void explain_cdataframe();

void clean_stdin();

void print_multiple_cols(CDATAFRAME *dataframe, int start, int end);

#endif //PROJET_C_WITH_WARREN_CDATAFRAME_H
