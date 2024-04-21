//
// Created by 20220848 on 31/03/2024.
//

#include "column.h"

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

// ========= Création de la fonction qui retourner le nombre de valeurs qui sont supérieures à x (x donné en paramètre) =====
void nb_val_supe(COLUMN* col, void* x) {
    if (col == NULL) {
        printf("Invalid column reference\n");
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

int compare(COLUMN *col, COL_TYPE *a, COL_TYPE *b) {
    int sort_dir = col->sort_dir; // Direction of the sort stored in the COLUMN structure
    char char_a, char_b;

    switch (col->column_type) {
        case CHAR:
            char_a = a->char_value;
            char_b = b->char_value;
            if (sort_dir == ASC)
                return (char_a > char_b) - (char_a < char_b);
            else
                return (char_b > char_a) - (char_b < char_a);

        case STRING:
            char_a = a->string_value[0];
            char_b = b->string_value[0];
            if (sort_dir == ASC)
                return (char_a > char_b) - (char_a < char_b);
            else
                return (char_b > char_a) - (char_b < char_a);

        case INT:
            if (sort_dir == ASC)
                return (a->int_value > b->int_value) - (a->int_value < b->int_value);
            else
                return (b->int_value > a->int_value) - (b->int_value < a->int_value);

        case FLOAT:
            if (sort_dir == ASC)
                return (a->float_value > b->float_value) ? 1 : (a->float_value < b->float_value) ? -1 : 0;
            else
                return (b->float_value > a->float_value) ? 1 : (b->float_value < a->float_value) ? -1 : 0;

        case DOUBLE:
            if (sort_dir == ASC)
                return (a->double_value > b->double_value) ? 1 : (a->double_value < b->double_value) ? -1 : 0;
            else
                return (b->double_value > a->double_value) ? 1 : (b->double_value < a->double_value) ? -1 : 0;

        default:
            fprintf(stderr, "Unsupported data type for sorting.\n");
            exit(EXIT_FAILURE);
    }
}


void quicksort(COLUMN *col, int left, int right) {
    int i, j;
    COL_TYPE *pivot, *temp;

    if (left < right) {
        pivot = col->data[right];
        i = (left - 1);

        for (j = left; j < right; j++) {
            if (compare(col, col->data[j], pivot) <= 0) {
                i++;
                temp = col->data[i];
                col->data[i] = col->data[j];
                col->data[j] = temp;
            }
        }
        temp = col->data[i + 1];
        col->data[i + 1] = col->data[right];
        col->data[right] = temp;
        quicksort(col, left, i);  // Corrected to i instead of i+1
        quicksort(col, i + 1, right);  // Corrected to i+1 instead of i+2
    }
}

void insertion_sort(COLUMN *col) {
    int i, j;
    COL_TYPE *key;

    for (i = 1; i < col->size; i++) {
        key = col->data[i];
        j = i - 1;

        while (j >= 0 && compare(col, col->data[j], key) > 0) {
            col->data[j + 1] = col->data[j];
            j = j - 1;
        }
        col->data[j + 1] = key;
    }
}



void sort(COLUMN* col, int sort_dir) {
    col->sort_dir = sort_dir;

    if (col->valid_index == 0) {
        quicksort(col, 0, col->size - 1);
    } else if (col->valid_index == -1) {
        insertion_sort(col);
    }

    col->valid_index = 1;  // Column is now fully sorted
}

void print_col_by_index(COLUMN *col) {
    for (int i = 0; i < col->size; i++) {
        printf("[%llu] ", col->index[i]);
        switch (col->column_type) {
            case INT:
                printf("%d\n", col->data[i]->int_value);
                break;
            case FLOAT:
                printf("%f\n", col->data[i]->float_value);
                break;
            case STRING:
                printf("%s\n", col->data[i]->string_value);
                break;
            default:
                printf("Unknown Type\n");
        }
    }
}



