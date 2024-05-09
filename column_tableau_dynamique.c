//
// Created by 20220848 on 08/05/2024.
//
#include "column_list.h"

// ATTENTION pour utiliser des colonnes dynaniques, vous devriez mettre en commentaire le fichier "column_list.c" en ajoutant /* au debut et */ à la fin

/*  // Supprimer cette ligne et la dernière pour pouvoir utiliser des colonnes dynaniques

// ============= Création de la fonction create_column ===============

COLUMN* create_column(ENUM_TYPE type, char* title) {
    COLUMN* new_column = (COLUMN*)malloc(sizeof(COLUMN));
    if (new_column == NULL) {
        printf("Erreur d'allocation de memoire pour la nouvelle colonne\n");
        return NULL;  // Retourner NULL en cas d'échec
    }

    new_column->title = strdup(title);
    if (new_column->title == NULL) {
        printf("Erreur d'allocation pour le titre\n");
        free(new_column);
        return NULL;
    }

    new_column->size = 0;
    new_column->max_size = 10;
    new_column->column_type = type;
    new_column->data = malloc(new_column->max_size * sizeof(void*));
    if (new_column->data == NULL) {
        printf("Erreur d'allocation pour les donnees\n");
        free(new_column->title);
        free(new_column);
        return NULL;
    }
    new_column->index = NULL;

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
                col->data[col->size] = malloc(sizeof(MyStruct));  // Allocate memory for the structure
                if (col->data[col->size] == NULL) {
                    return 0;
                }
                memcpy(col->data[col->size], value, sizeof(MyStruct));  // Copy the structure

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

    // Libération de la mémoire allouée pour la structure de colonne elle-même
    free(*col);

    // Mettre le pointeur de la colonne à NULL
    *col = NULL;
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

void convert_value(COLUMN* col, unsigned long long int i, char* str, int size) {
    if (col == NULL || str == NULL || i >= col->size) {
        snprintf(str, size, "%s", "ERROR");
        return;
    }

    // Assurez-vous que la chaîne est nulle au départ pour éviter l'écriture de déchets
    memset(str, 0, size);

    if (col->data[i] == NULL) {
        snprintf(str, size, "%s", "NULL");
        return;
    }

    switch(col->column_type) {
        case UINT:
            snprintf(str, size, "%u", *((unsigned int*)col->data[i]));
            break;
        case INT:
            snprintf(str, size, "%d", *((int*)col->data[i]));
            break;
        case CHAR:
            snprintf(str, size, "%c", *((char*)col->data[i]));
            break;
        case FLOAT:
            // Pour les floats, vous pourriez vouloir contrôler la précision, par exemple "%.2f" pour deux décimales.
            snprintf(str, size, "%f", *((float*)col->data[i]));
            break;
        case DOUBLE:
            // De même, "%.lf" pour les doubles avec une précision contrôlée si nécessaire.
            snprintf(str, size, "%lf", *((double*)col->data[i]));
            break;
        case STRING:
            snprintf(str, size, "%s", (char*)col->data[i]);
            break;
        case STRUCTURE:
            // Vous devrez implémenter la logique de conversion de votre structure en chaîne ici.
            // Par exemple, si vous avez une structure avec un entier et un flottant, vous pourriez faire :
            // snprintf(str, size, "{int: %d, float: %f}", your_structure->int_field, your_structure->float_field);
            break;
        default:
            snprintf(str, size, "%s", "UNKNOWN TYPE");
            break;
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
        // Assuming x is a pointer to a type that matches the column_list type
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

    // Determine and print the value based on the type stored in the column_list
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


// ========= Création de la fonction qui retourner le nombre de valeurs qui sont supérieures à x (x donné en paramètre) =====
void nb_val_supe(COLUMN* col, void* x) {
    if (col == NULL) {
        printf("Invalid column_list reference\n");
        return;
    }

    int count = 0;
    ENUM_TYPE type = col->column_type;

    if (x == NULL) {
        // Comptage de toutes les entrées non NULL si x est NULL
        for (unsigned int i = 0; i < col->size; i++) {
            if (col->data[i] != NULL) {
                count++;
            }
        }
        printf("Le nombre de valeurs non-NULL est : %d\n", count);
    } else {
        // Effectuer des comparaisons appropriées au type
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
                    // Étendre les cas à d'autres types si nécessaire
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


// ========= Création de la fonction qui retourner le nombre de valeurs qui sont inférieures à x (x donné en paramètre) ======

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



// ========= Création de la fonction qui retourner le nombre de valeurs qui sont égales à x (x donné en paramètre) ======


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
                case STRING:
                    if (strcmp((char*)col->data[i], (char*)x) == 0) count++;  // Compare strings
                    break;
                default:
                    printf("Comparison not supported for this type\n");
                    return;
            }
        }
    }

    // Print results based on the type of x, assuming x is of a basic data type
    if (type == INT || type == UINT) {
        printf("Le nombre de valeurs egales a %d est : %d\n", *((int*)x), count);
    } else if (type == FLOAT) {
        printf("Le nombre de valeurs egales a %f est : %d\n", *((float*)x), count);
    } else if (type == DOUBLE) {
        printf("Le nombre de valeurs egales a %lf est : %d\n", *((double*)x), count);
    } else if (type == CHAR) {
        printf("Le nombre de valeurs egales a '%c' est : %d\n", *((char*)x), count);
    } else if (type == STRING) {
        printf("Le nombre de valeurs egales a \"%s\" est : %d\n", (char*)x, count);
    }
}

// Function to compare two COL_TYPE values based on the column_list type
int compare(COL_TYPE *a, COL_TYPE *b, COLUMN *col) {
    if (col->sort_dir == ASC) {  // Ascending order
        switch (col->column_type) {
            case INT:
                return (*(int *)a - *(int *)b);
            case FLOAT:
                return (*(float *)a > *(float *)b) - (*(float *)a < *(float *)b);
            case CHAR:
                return (*(char *)a - *(char *)b);
            case DOUBLE:
                return (*(double *)a > *(double *)b) - (*(double *)a < *(double *)b);
            case STRING:
                return strcmp((char *)a, (char *)b);
            default:
                printf("Unsupported column_list type for sorting.\n");
                return 0;
        }
    } else {  // Descending order
        switch (col->column_type) {
            case INT:
                return (*(int *)b - *(int *)a);
            case FLOAT:
                return (*(float *)b > *(float *)a) - (*(float *)b < *(float *)a);
            case CHAR:
                return (*(char *)b - *(char *)a);
            case DOUBLE:
                return (*(double *)b > *(double *)a) - (*(double *)b < *(double *)a);
            case STRING:
                return strcmp((char *)b, (char *)a);
            default:
                printf("Unsupported column_list type for sorting.\n");
                return 0;
        }
    }
}

// Quick Sort Algorithm
void quicksort(COL_TYPE **data, int left, int right, COLUMN *col) {
    if (left < right) {
        int i = left, j = right;
        COL_TYPE *pivot = data[(left + right) / 2];

        while (i <= j) {
            while (compare(data[i], pivot, col) < 0) i++;
            while (compare(data[j], pivot, col) > 0) j--;
            if (i <= j) {
                COL_TYPE *tmp = data[i];
                data[i] = data[j];
                data[j] = tmp;
                i++;
                j--;
            }
        }

        if (left < j)
            quicksort(data, left, j, col);
        if (i < right)
            quicksort(data, i, right, col);
    }
}

// Insertion Sort Algorithm
void insertion_sort(COL_TYPE **data, int n, COLUMN *col) {
    for (int i = 1; i < n; i++) {
        COL_TYPE *key = data[i];
        int j = i - 1;

        while (j >= 0 && compare(data[j], key, col) > 0) {
            data[j + 1] = data[j];
            j = j - 1;
        }
        data[j + 1] = key;
    }
}
void sort(COLUMN* col, int sort_dir) {
    if (!col || !col->data) {
        printf("Invalid column_list or column_list data.\n");
        return;
    }

    col->sort_dir = sort_dir;  // Set the sorting direction
    int n = col->size;

    if (col->valid_index == 0) {  // Not sorted at all
        quicksort(col->data, 0, n - 1, col);
    } else if (col->valid_index == -1) {  // Partially sorted (all but the last element)
        insertion_sort(col->data, n, col);
    } else if (col->valid_index == 1) {  // Already sorted, just sort if direction changed
        if (sort_dir != col->sort_dir) {
            quicksort(col->data, 0, n - 1, col);
        }
    }

    col->valid_index = 1;  // Mark the column_list as fully sorted
    printf("Column sorted successfully.\n");
}

 */