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
    //ColumnList *head; // Pointeur vers la tête de la liste de colonnes
    int num_columns;
} CDATAFRAME;

// ================ Prototypes de création ==================

CDATAFRAME *create_empty_cdataframe();

CDATAFRAME *create_cdataframe(ENUM_TYPE *cdftype, int size);

CDATAFRAME* load_from_csv(char *file_name, ENUM_TYPE *dftype, int size);

void save_into_csv(CDATAFRAME *cdf, char *file_name);

void rename_column(CDATAFRAME *cdf, int column_index, const char *new_title);


// ================ Prototypes de suppression ==================

void delete_cdataframe(CDATAFRAME **cdf);

void delete_column_by_name(CDATAFRAME *cdf, char *col_name);

void delete_column_by_index(CDATAFRAME *dataframe, int index);  // pour un CData en LLC

void delete_all_columns(CDATAFRAME *dataframe);   // pour un CData en LLC

void delete_columns_range(CDATAFRAME *dataframe, int start, int end);  // pour un CData en LLC

// ================ Prototypes du menu ==================

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

void create_column_m(CDATAFRAME *cdf);

void rename_column_ui(CDATAFRAME *cdf); // Fonction d'interface utilisateur pour renommer les colonnes

void load_dataframe(CDATAFRAME **cdf);

void save_dataframe(CDATAFRAME *cdf);

void create_row(CDATAFRAME *cdf);

void create_column_LDC(CDATAFRAME *dataframe);

// ================ Prototypes d'affichage ==================

void print_multiple_cols(CDATAFRAME *dataframe, int start, int end);  // affichage colonne

void print_limited_rows(CDATAFRAME *dataframe);

void handle_column_display(CDATAFRAME *dataframe);

void print_single_row(CDATAFRAME *dataframe, int row_index);

void handle_row_display(CDATAFRAME *dataframe);

// ================ Prototypes de tri ==================

void sort_multiple_columns(CDATAFRAME *dataframe, int start, int end); // pour un CData en LLC

void sort_single_column(CDATAFRAME *dataframe, int index);// pour un CData en LLC

#endif //PROJET_C_WITH_WARREN_CDATAFRAME_LIST_H
