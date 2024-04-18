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
    // Vérification si la colonne existe et que le pointeur value n'est pas NULL
    if (col == NULL || value == NULL) {
        return 0;
    }

    // Vérification de la disponibilité de l'espace physique
    if (col->size >= col->max_size) {
        // Réallocation de mémoire pour augmenter la taille physique
        col->max_size += REALOC_SIZE;
        void **new_data = realloc(col->data, col->max_size * sizeof(void *));
        if (new_data == NULL) {
            return 0; // Échec de réallocation de mémoire
        }
        col->data = (COL_TYPE **) new_data;
    }

    // Insertion de la valeur dans la colonne
    switch (col->column_type) {
        /*case NULLVAL:
            col->data[col->size] = NULL;
            printf("a");
            break;*/
        case UINT:
            col->data[col->size] = malloc(sizeof(unsigned int));
            if (col->data[col->size] == NULL) {
                return 0; // Échec d'allocation de mémoire
            }
            *((unsigned int *)col->data[col->size]) = *((unsigned int *)value);
            break;
        case INT:
            col->data[col->size] = malloc(sizeof(int));
            if (col->data[col->size] == NULL) {
                return 0; // Échec d'allocation de mémoire
            }
            *((int *)col->data[col->size]) = *((int *)value);
            break;
        case CHAR:
            col->data[col->size] = malloc(sizeof(char));
            if (col->data[col->size] == NULL) {
                return 0; // Échec d'allocation de mémoire
            }
            *((char *)col->data[col->size]) = *((char *)value);
            break;
        case FLOAT:
            col->data[col->size] = malloc(sizeof(float));
            if (col->data[col->size] == NULL) {
                return 0; // Échec d'allocation de mémoire
            }
            *((float *)col->data[col->size]) = *((float *)value);
            break;
        case DOUBLE:
            col->data[col->size] = malloc(sizeof(double));
            if (col->data[col->size] == NULL) {
                return 0; // Échec d'allocation de mémoire
            }
            *((double *)col->data[col->size]) = *((double *)value);
            break;
        case STRING:
            col->data[col->size] = malloc(strlen((char *)value) + 1);
            if (col->data[col->size] == NULL) {
                return 0; // Échec d'allocation de mémoire
            }
            strcpy((char *)col->data[col->size], (char *)value);
            break;
        case STRUCTURE:
            // À compléter pour d'autres types de structure si nécessaire
            break;
        default:
            return 0; // Type de colonne non pris en charge
    }

    col->size++; // Mise à jour de la taille logique de la colonne
    return 1; // Insertion réussie
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

// Création de la fonction print_col
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
int nb_occ(COLUMN* col, int x) {
    int count = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i]->int_value == x) {
            count++;
        }
    }
    return count;
}

// ========= Création de la fonction qui retourner la valeur présente à la position x (x donné en paramètre) ======
int pos_val(COLUMN* col, unsigned int x) {
    // Vérifier si la position est valide
    if (x >= col->size) {
        printf("Position invalide\n");
        exit(EXIT_FAILURE);
    }
    // Retourner la valeur à la position x
    return col->data[x]->int_value;
}

int nb_val_supe(COLUMN* col, int x) {
    int count = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i]->int_value > x) {
            count++;
        }
    }
    return count;
}

int nb_val_inf(COLUMN* col, int x) {
    int count = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i]->int_value < x) {
            count++;
        }
    }
    return count;
}

int nb_val_egal(COLUMN* col, int x) {
    int count = 0;
    for (unsigned int i = 0; i < col->size; i++) {
        if (col->data[i]->int_value == x) {
            count++;
        }
    }
    return count;
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