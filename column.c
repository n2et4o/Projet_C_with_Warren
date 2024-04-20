//
// Created by 20220848 on 31/03/2024.
//

#include "column.h"

// ============= Création de la fonction create_column ===============
COLUMN* create_column(ENUM_TYPE type, char* title) {
    COLUMN* new_column = (COLUMN*)malloc(sizeof(COLUMN));
    if (new_column == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    // Initialisation des attributs de la colonne
    new_column->title = strdup(title); // Duplication du titre
    new_column->size = 0; // Taille logique initiale à 0
    new_column->max_size = 0; // Taille physique initiale
    new_column->column_type = type; // Type de colonne initialisé à NULL
    new_column->data = NULL; // Pointeur sur le tableau de données initialisé à NULL
    new_column->index = NULL; // Pointeur sur le tableau d'index initialisé à NULL

    //printf("colonne cree");
    return new_column;
}

// ============= Création de la fonction insert_value ===============
int insert_value(COLUMN *col, void *value) {
    if (col == NULL) {
        return 0;
    }

    if (col->size >= col->max_size) {
        int new_max_size = col->max_size + REALOC_SIZE;
        void **new_data = realloc(col->data, new_max_size * sizeof(void *));
        if (new_data == NULL) {
            return 0;
        }
        col->data = new_data;
        col->max_size = new_max_size;
    }

    // Spécialement pour gérer les valeurs NULL
    if (value == NULL) {
        col->data[col->size] = NULL;
    } else {
        switch (col->column_type) {
            case UINT:
                col->data[col->size] = malloc(sizeof(unsigned int));
                if (col->data[col->size] == NULL) {
                    return 0;
                }
                *((unsigned int *)col->data[col->size]) = *((unsigned int *)value);
                break;
            case INT:
                col->data[col->size] = malloc(sizeof(int));
                if (col->data[col->size] == NULL) {
                    return 0;
                }
                *((int *)col->data[col->size]) = *((int *)value);
                break;
            case CHAR:
                col->data[col->size] = malloc(sizeof(char));
                if (col->data[col->size] == NULL) {
                    return 0;
                }
                *((char *)col->data[col->size]) = *((char *)value);
                break;
            case FLOAT:
                col->data[col->size] = malloc(sizeof(float));
                if (col->data[col->size] == NULL) {
                    return 0;
                }
                *((float *)col->data[col->size]) = *((float *)value);
                break;
            case DOUBLE:
                col->data[col->size] = malloc(sizeof(double));
                if (col->data[col->size] == NULL) {
                    return 0;
                }
                *((double *)col->data[col->size]) = *((double *)value);
                break;
            case STRING:
                col->data[col->size] = strdup((char *)value);
                if (col->data[col->size] == NULL) {
                    return 0;
                }
                break;
            case STRUCTURE:
                // À compléter selon la structure définie
                break;
            default:
                return 0;
        }
    }

    col->size++;
    return 1;
}

// ============ Création de la fonction delete_column =============
void delete_column(COLUMN** col) {
    if (*col == NULL) {
        printf("La colonne est déjà libérée ou n'existe pas\n");
        return;
    }

    // Libération de la mémoire allouée pour le titre
    free((*col)->title);

    // Libération de la mémoire allouée pour chaque élément du tableau de données
    if ((*col)->data != NULL) {
        for (unsigned int i = 0; i < (*col)->size; i++) {
            free((*col)->data[i]);
        }
        free((*col)->data);
    }

    // Libération de la mémoire allouée pour le tableau d'index
    if ((*col)->index != NULL) {
        free((*col)->index);
    }

    // Libération de la mémoire allouée pour la structure de colonne
    free(*col);

    // Affection de NULL au pointeur pour éviter les accès après libération
    *col = NULL;
    //printf("colonne supprimee\n");
}

// ================= Création de la fonction print_col ======================
void print_col(COLUMN* col) {
    if (col == NULL) {
        printf("Column is NULL\n");
        return;
    }

    //printf("[%u]\n", col->size); // Affichage de la taille logique de la colonne

    for (unsigned int i = 0; i < col->size; i++) {
        char str[20]; // Taille aléatoire pour stocker la valeur sous forme de chaîne
        if (col->data[i] == NULL) {
            printf("[%u] NULL\n", i);
        } else {
            convert_value(col, i, str, sizeof(str)); // Convertir la valeur en chaîne de caractères
            printf("[%u] %s\n", i, str);
        }
    }
}

// ========= Création de la fonction qui retourner le nombre de d’occurrences d’une valeur x (x donné en paramètre) ======
void nb_occ(COLUMN* col, void* x) {
    int count = 0;
    ENUM_TYPE type = col->column_type;

    for (unsigned int i = 0; i < col->size; i++) {
        if (x == NULL) {
            if (col->data[i] == NULL) {
                count++;
            }
        } else if (col->data[i] != NULL) {
            switch (type) {
                case INT:
                    if (*(int*)(col->data[i]) == *(int*)x) count++;
                    break;
                case FLOAT:
                    if (*(float*)(col->data[i]) == *(float*)x) count++;
                    break;
                case CHAR:
                    if (*(char*)(col->data[i]) == *(char*)x) count++;
                    break;
                case DOUBLE:
                    if (*(double*)(col->data[i]) == *(double*)x) count++;
                    break;
                case STRING:
                    if (strcmp((char*)(col->data[i]), (char*)x) == 0) count++;
                    break;
                case UINT:
                    if (*(unsigned int*)(col->data[i]) == *(unsigned int*)x) count++;
                    break;
                default:
                    // Potentially add cases for STRUCTURE or other custom types
                    break;
            }
        }
    }

    if (x == NULL) {
        printf("Le nombre d'occurrences de la valeur NULL est : %d\n", count);
    } else {
        // Assuming x is a pointer to a type that matches the column type
        switch (type) {
            case INT:
            case UINT:
                printf("Le nombre d'occurrences de la valeur %d est : %d\n", *(int*)x, count);
                break;
            case FLOAT:
                printf("Le nombre d'occurrences de la valeur %f est : %d\n", *(float*)x, count);
                break;
            case CHAR:
                printf("Le nombre d'occurrences de la valeur '%c' est : %d\n", *(char*)x, count);
                break;
            case DOUBLE:
                printf("Le nombre d'occurrences de la valeur %lf est : %d\n", *(double*)x, count);
                break;
            case STRING:
                printf("Le nombre d'occurrences de la valeur \"%s\" est : %d\n", (char*)x, count);
                break;
            default:
                printf("Type non supporte pour l'affichage\n");
                break;
        }
    }
}


// ========= Création de la fonction qui retourner la valeur présente à la position x (x donné en paramètre) ======
void pos_val(COLUMN* col, unsigned int x) {
    if (col == NULL || x >= col->size) {
        printf("Position invalide\n");
        exit(EXIT_FAILURE); // Or handle the error as appropriate
    }

    if (col->data[x] == NULL) {
        printf("La valeur a la position %u est : NULL\n", x);
        return;
    }

    // Determine and print the value based on the type stored in the column
    switch (col->column_type) {
        case INT:
            printf("La valeur a la position %u est : %d\n", x, *((int*)col->data[x]));
            break;
        case FLOAT:
            printf("La valeur a la position %u est : %f\n", x, *((float*)col->data[x]));
            break;
        case CHAR:
            printf("La valeur a la position %u est : '%c'\n", x, *((char*)col->data[x]));
            break;
        case DOUBLE:
            printf("La valeur a la position %u est : %lf\n", x, *((double*)col->data[x]));
            break;
        case STRING:
            printf("La valeur a la position %u est : \"%s\"\n", x, *((char**)col->data[x]));
            break;
        case UINT:
            printf("La valeur a la position %u est : %u\n", x, *((unsigned int*)col->data[x]));
            break;
        case STRUCTURE:
            // Assuming you have a way to stringify your structure
            printf("La valeur a la position %u est : Structure\n", x);
            break;
        default:
            printf("Type non supporte\n");
            break;
    }
}


void nb_val_supe(COLUMN* col, void* x) {
    if (col == NULL) {
        printf("Invalid column reference\n");
        return;
    }

    int count = 0;
    ENUM_TYPE type = col->column_type;

    if (x == NULL) {
        // Counting all non-NULL entries if x is NULL
        for (unsigned int i = 0; i < col->size; i++) {
            if (col->data[i] != NULL) {
                count++;
            }
        }
        printf("Le nombre de valeurs non-NULL est : %d\n", count);
    } else {
        // Perform type-appropriate comparisons
        for (unsigned int i = 0; i < col->size; i++) {
            if (col->data[i] != NULL) {
                switch (type) {
                    case INT:
                        if (*((int*)(col->data[i])) > *((int*)x)) count++;
                        break;
                    case FLOAT:
                        if (*((float*)(col->data[i])) > *((float*)x)) count++;
                        break;
                    case DOUBLE:
                        if (*((double*)(col->data[i])) > *((double*)x)) count++;
                        break;
                    case UINT:
                        if (*((unsigned int*)(col->data[i])) > *((unsigned int*)x)) count++;
                        break;
                    // Extend cases for other types if necessary
                    default:
                        printf("Type non supporté pour la comparaison\n");
                        return;
                }
            }
        }
        // Print the results based on the assumed type of `x` being int
        if (type == INT || type == UINT) {
            printf("Le nombre de valeurs superieures a %d est : %d\n", *((int*)x), count);
        } else if (type == FLOAT) {
            printf("Le nombre de valeurs superieures a %f est : %d\n", *((float*)x), count);
        } else if (type == DOUBLE) {
            printf("Le nombre de valeurs superieures a %lf est : %d\n", *((double*)x), count);
        }
    }
}


void nb_val_inf(COLUMN* col, void* x) {
    if (col == NULL || x == NULL) {
        printf("Invalid input\n");
        return;
    }

    int count = 0;
    ENUM_TYPE type = col->column_type;

    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i] != NULL) {
            switch (type) {
                case INT:
                    if (*((int*)(col->data[i])) < *((int*)x)) count++;
                    break;
                case FLOAT:
                    if (*((float*)(col->data[i])) < *((float*)x)) count++;
                    break;
                case DOUBLE:
                    if (*((double*)(col->data[i])) < *((double*)x)) count++;
                    break;
                default:
                    printf("Comparaison inferieure non supportee pour ce type\n");
                    return;
            }
        }
    }

    // Print the final count outside the loop
    switch (type) {
        case INT:
            printf("Le nombre de valeurs inferieures a %d est : %d\n", *((int*)x), count);
            break;
        case FLOAT:
            printf("Le nombre de valeurs inferieures a %f est : %d\n", *((float*)x), count);
            break;
        case DOUBLE:
            printf("Le nombre de valeurs inferieures a %lf est : %d\n", *((double*)x), count);
            break;
        default:
            printf("Type non supporte pour l'affichage\n");
            break;
    }
}


void nb_val_egal(COLUMN* col, void* x) {
    if (col == NULL || x == NULL) {
        printf("Invalid input or comparison to NULL is not supported for non-pointer types\n");
        return;
    }

    int count = 0;
    ENUM_TYPE type = col->column_type;

    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i] != NULL) {
            switch (type) {
                case INT:
                    if (*((int*)(col->data[i])) == *((int*)x)) count++;
                    break;
                case FLOAT:
                    if (*((float*)(col->data[i])) == *((float*)x)) count++;
                    break;
                case DOUBLE:
                    if (*((double*)(col->data[i])) == *((double*)x)) count++;
                    break;
                case UINT:
                    if (*((unsigned int*)(col->data[i])) == *((unsigned int*)x)) count++;
                    break;
                case CHAR:
                    if (*((char*)(col->data[i])) == *((char*)x)) count++;
                    break;
                default:
                    printf("Comparison not supported for this type\n");
                    return;
            }
        }
    }

    // Assuming x is of type int for the printf format specifier; adjust as necessary for other types
    if (type == INT || type == UINT) {
        printf("Le nombre de valeurs egales a %d est : %d\n", *((int*)x), count);
    } else if (type == FLOAT) {
        printf("Le nombre de valeurs egales a %f est : %d\n", *((float*)x), count);
    } else if (type == DOUBLE) {
        printf("Le nombre de valeurs egales a %lf est : %d\n", *((double*)x), count);
    } else if (type == CHAR) {
        printf("Le nombre de valeurs egales a '%c' est : %d\n", *((char*)x), count);
    }
}


void convert_value(COLUMN *col, unsigned long long int i, char *str, int size) {
    if (col == NULL || str == NULL || i >= col->size || size <= 0) {
        return;
    }

    switch(col->column_type) {
        case NULLVAL:
            snprintf(str, size, "%s", "NULL");
            break;
        case UINT:
            snprintf(str, size, "%u", *((unsigned int *)col->data[i]));
            break;
        case INT:
            snprintf(str, size, "%d", *((int *)col->data[i]));
            break;
        case CHAR:
            snprintf(str, size, "%c", *((char *)col->data[i]));
            break;
        case FLOAT:
            snprintf(str, size, "%f", *((float *)col->data[i]));
            break;
        case DOUBLE:
            snprintf(str, size, "%lf", *((double *)col->data[i]));
            break;
        case STRING:
            snprintf(str, size, "%s", (char *)col->data[i]);
            break;
        case STRUCTURE:
            // À compléter pour les types de données structurées
            break;
        default:
            snprintf(str, size, "%s", "UNKNOWN TYPE");
            break;
    }
}

void sort(COLUMN* col, int sort_dir) {
    if (col->valid_index == 0) {
        // If index is not valid (column not sorted), use quicksort
        quicksort(col->index, col->data, 0, col->size - 1, sort_dir);
    } else if (col->valid_index == -1) {
        // If index is partially valid, use insertion sort
        insertion_sort(col->index, col->data, col->size, sort_dir);
    }

    col->sort_dir = sort_dir;
}

// Quicksort implementation
void quicksort(unsigned long long *index, COL_TYPE **data, int left, int right, int sort_dir) {
    if (left < right) {
        int pivot_index = left;
        unsigned long long pivot = index[right];

        for (int i = left; i < right; i++) {
            if ((sort_dir == ASC && index[i] < pivot) || (sort_dir == DESC && index[i] > pivot)) {
                unsigned long long temp_index = index[i];
                index[i] = index[pivot_index];
                index[pivot_index] = temp_index;

                COL_TYPE *temp_data = data[i];
                data[i] = data[pivot_index];
                data[pivot_index] = temp_data;

                pivot_index++;
            }
        }

        unsigned long long temp_index = index[right];
        index[right] = index[pivot_index];
        index[pivot_index] = temp_index;

        COL_TYPE *temp_data = data[right];
        data[right] = data[pivot_index];
        data[pivot_index] = temp_data;

        quicksort(index, data, left, pivot_index - 1, sort_dir);
        quicksort(index, data, pivot_index + 1, right, sort_dir);
    }
}

// Insertion sort implementation
void insertion_sort(unsigned long long *index, COL_TYPE **data, int size, int sort_dir) {
    int i, j;
    unsigned long long key;
    COL_TYPE *data_key;

    for (i = 1; i < size; i++) {
        key = index[i];
        data_key = data[i];
        j = i - 1;

        while (j >= 0 && ((sort_dir == ASC && index[j] > key) || (sort_dir == DESC && index[j] < key))) {
            index[j + 1] = index[j];
            data[j + 1] = data[j];
            j = j - 1;
        }

        index[j + 1] = key;
        data[j + 1] = data_key;
    }
}

void print_col_by_index(COLUMN *col) {
    if (col->valid_index != 1) {
        // L'index n'est pas valide, affichez un message d'erreur
        printf("Error: Index is not valid.\n");
        return;
    }

    // Afficher les valeurs selon l'ordre séquentiel de l'index
    if (col->sort_dir == ASC) {
        for (unsigned long long i = 0; i < col->size; i++) {
            unsigned long long index = col->index[i];
            printf("[%llu] %s\n", index, col->data[index]->string_value);
        }
    } else {
        for (long long i = col->size - 1; i >= 0; i--) {
            unsigned long long index = col->index[i];
            printf("[%llu] %s\n", index, col->data[index]->string_value);
        }
    }
}
