//
// Created by 20220848 on 31/03/2024.
//

#include "column_list.h"

// ============= Création de la fonction create_column ===============

COLUMN* create_column(ENUM_TYPE type, char* title) {
    COLUMN* new_column = (COLUMN*)malloc(sizeof(COLUMN));
    if (new_column == NULL) {
        printf("Erreur d'allocation de memoire pour la nouvelle colonne\n");
        return NULL;
    }

    new_column->title = strdup(title);
    if (new_column->title == NULL) {
        printf("Erreur d'allocation pour le titre\n");
        free(new_column);
        return NULL;
    }

    new_column->size = 0;
    new_column->column_type = type;
    new_column->head = NULL;  // Initialise la tête de la liste chaînée à NULL

    return new_column;
}


// ============= Création de la fonction insert_value ===============

int insert_value(COLUMN *col, void *value) {
    if (col == NULL) {
        return 0;  // Échec si la colonne n'existe pas
    }

    // Création d'un nouveau nœud pour la valeur à insérer
    LISTE * newNode = (LISTE*)malloc(sizeof(LISTE));
    if (newNode == NULL) {
        return 0;  // Échec de l'allocation mémoire pour le nouveau nœud
    }

    // Allocation et copie de la valeur en fonction du type de colonne
    switch (col->column_type) {
        case MY_INT:
            newNode->val = malloc(sizeof(int));
            if (newNode->val == NULL) {
                free(newNode);
                return 0;
            }
            *((int *)newNode->val) = *((int *)value);
            break;
        case MY_CHAR:
            newNode->val = malloc(sizeof(char));
            if (newNode->val == NULL) {
                free(newNode);
                return 0;
            }
            *((char *)newNode->val) = *((char *)value);
            break;
        case MY_FLOAT:
            newNode->val = malloc(sizeof(float));
            if (newNode->val == NULL) {
                free(newNode);
                return 0;
            }
            *((float *)newNode->val) = *((float *)value);
            break;
        case MY_DOUBLE:
            newNode->val = malloc(sizeof(double));
            if (newNode->val == NULL) {
                free(newNode);
                return 0;
            }
            *((double *)newNode->val) = *((double *)value);
            break;
        case STRING:
            newNode->val = strdup((char *)value);
            if (newNode->val == NULL) {
                free(newNode);
                return 0;
            }
            break;
        case STRUCTURE:
            newNode->val = malloc(sizeof(MyStruct));
            if (newNode->val == NULL) {
                free(newNode);
                return 0;
            }
            memcpy(newNode->val, value, sizeof(MyStruct));
            break;
        default:
            free(newNode);
            return 0;
    }

    newNode->succ = NULL;

    // Ajout du nouveau nœud à la fin de la liste
    if (col->head == NULL) {
        col->head = newNode;  // Premier élément de la liste
    } else {
        LISTE *current = col->head;
        while (current->succ != NULL) {
            current = current->succ;  // Parcourir jusqu'au dernier nœud
        }
        current->succ = newNode;  // Ajouter le nouveau nœud à la fin
    }

    col->size++;
    return 1;  // Succès
}


// ============ Création de la fonction delete_column =============

void delete_column(COLUMN** col) {
    if (col == NULL || *col == NULL) {
        printf("La colonne est déjà libérée ou n'existe pas\n");
        return;
    }

    // Libération de la mémoire allouée pour le titre
    free((*col)->title);

    // Libération de la mémoire allouée pour chaque élément de la liste chaînée
    LISTE* currentNode = (*col)->head;
    while (currentNode != NULL) {
        LISTE * temp = currentNode;
        currentNode = currentNode->succ;

        // Libérer la mémoire allouée pour les données du nœud
        free(temp->val);
        // Libérer le nœud lui-même
        free(temp);
    }

    // Libération de la mémoire allouée pour la structure de colonne elle-même
    free(*col);

    // Mettre le pointeur de la colonne à NULL
    *col = NULL;
}


// ================= Création de la fonction print_col ======================


void print_col(COLUMN *col) {
    if (col == NULL) {
        printf("La colonne est vide.\n");
        return;
    }

    LISTE *entry = col->head;
    int index = 0;
    while (entry != NULL) {
        char buffer[256];
        convert_value(col, index, buffer, sizeof(buffer));
        printf("[%d]: %s\t", index, buffer);
        entry = entry->succ;
        index++;
    }
    printf("\n");
}


void convert_value(COLUMN* col, unsigned long long int index, char* str, int size) {
    if (col == NULL || str == NULL) {
        snprintf(str, size, "%s", "ERROR");
        return;
    }

    memset(str, 0, size); // Initialisation de la chaîne de caractères pour éviter l'écriture de déchets

    LISTE* node = col->head;
    for (unsigned long long int count = 0; node != NULL && count < index; count++) {
        node = node->succ;
    }

    if (node == NULL) {
        snprintf(str, size, "%s", "INDEX OUT OF RANGE");
        return;
    }

    if (node->val == NULL) {
        snprintf(str, size, "%s", "NULL");
        return;
    }

    // Conversion de la valeur en fonction du type de la colonne
    switch(col->column_type) {
        case MY_UINT:
            snprintf(str, size, "%u", *((unsigned int*)node->val));
            break;
        case MY_INT:
            snprintf(str, size, "%d", *((int*)node->val));
            break;
        case MY_CHAR:
            snprintf(str, size, "%c", *((char*)node->val));
            break;
        case MY_FLOAT:
            snprintf(str, size, "%.2f", *((float*)node->val));
            break;
        case MY_DOUBLE:
            snprintf(str, size, "%.2lf", *((double*)node->val));
            break;
        case STRING:
            snprintf(str, size, "%s", (char*)node->val);
            break;
        case STRUCTURE:
            // Implémenter la conversion selon la structure définie
            break;
        default:
            snprintf(str, size, "%s", "UNKNOWN TYPE");
            break;
    }
}

// ========= Création de la fonction qui retourner le nombre de d’occurrences d’une valeur x (x donné en paramètre) ======

void nb_occ(COLUMN* col, void* x) {
    if (col == NULL) {
        printf("Column is NULL\n");
        return;
    }

    int count = 0;
    ENUM_TYPE type = col->column_type;

    // Parcours de la liste chaînée pour compter les occurrences
    LISTE* currentNode = col->head;
    while (currentNode != NULL) {
        if (x == NULL) {
            if (currentNode->val == NULL) {
                count++;
            }
        } else if (currentNode->val != NULL) {
            switch (type) {
                case MY_INT:
                    if (*(int*)(currentNode->val) == *(int*)x) count++;
                    break;
                case MY_FLOAT:
                    if (*(float*)(currentNode->val) == *(float*)x) count++;
                    break;
                case MY_CHAR:
                    if (*(char*)(currentNode->val) == *(char*)x) count++;
                    break;
                case MY_DOUBLE:
                    if (*(double*)(currentNode->val) == *(double*)x) count++;
                    break;
                case STRING:
                    if (strcmp((char*)(currentNode->val), (char*)x) == 0) count++;
                    break;
                case MY_UINT:
                    if (*(unsigned int*)(currentNode->val) == *(unsigned int*)x) count++;
                    break;
                default:
                    // Potentially add cases for STRUCTURE or other custom types
                    break;
            }
        }
        currentNode = currentNode->succ; // Passer au nœud suivant
    }

    // Affichage du nombre d'occurrences
    if (x == NULL) {
        printf("Le nombre d'occurrences de la valeur NULL est : %d\n", count);
    } else {
        // Affichage en fonction du type de données
        switch (type) {
            case MY_INT:
            case MY_UINT:
                printf("Le nombre d'occurrences de la valeur %d est : %d\n", *(int*)x, count);
                break;
            case MY_FLOAT:
                printf("Le nombre d'occurrences de la valeur %f est : %d\n", *(float*)x, count);
                break;
            case MY_CHAR:
                printf("Le nombre d'occurrences de la valeur '%c' est : %d\n", *(char*)x, count);
                break;
            case MY_DOUBLE:
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
    if (col == NULL) {
        printf("Column is NULL\n");
        exit(EXIT_FAILURE);
    }

    LISTE* node = col->head;
    unsigned int count = 0;

    // Avancer dans la liste jusqu'à la position x
    while (node != NULL && count < x) {
        node = node->succ;
        count++;
    }

    if (node == NULL) {
        printf("Position invalide\n");
        return;
    }

    if (node->val == NULL) {
        printf("La valeur a la position %u est : NULL\n", x);
        return;
    }

    // Determine and print the value based on the type stored in the column_list
    switch (col->column_type) {
        case MY_INT:
            printf("La valeur a la position %u est : %d\n", x, *((int*)node->val));
            break;
        case MY_FLOAT:
            printf("La valeur a la position %u est : %f\n", x, *((float*)node->val));
            break;
        case MY_CHAR:
            printf("La valeur a la position %u est : '%c'\n", x, *((char*)node->val));
            break;
        case MY_DOUBLE:
            printf("La valeur a la position %u est : %lf\n", x, *((double*)node->val));
            break;
        case STRING:
            printf("La valeur a la position %u est : \"%s\"\n", x, (char*)node->val);
            break;
        case MY_UINT:
            printf("La valeur a la position %u est : %u\n", x, *((unsigned int*)node->val));
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
    LISTE* node = col->head;

    if (x == NULL) {
        // Comptage de toutes les entrées non NULL si x est NULL
        while (node != NULL) {
            if (node->val != NULL) {
                count++;
            }
            node = node->succ;
        }
        printf("Le nombre de valeurs non-NULL est : %d\n", count);
    } else {
        // Effectuer des comparaisons appropriées au type
        while (node != NULL) {
            if (node->val != NULL) {
                switch (type) {
                    case MY_INT:
                        if (*((int*)(node->val)) > *((int*)x)) count++;
                        break;
                    case MY_FLOAT:
                        if (*((float*)(node->val)) > *((float*)x)) count++;
                        break;
                    case MY_DOUBLE:
                        if (*((double*)(node->val)) > *((double*)x)) count++;
                        break;
                    case MY_UINT:
                        if (*((unsigned int*)(node->val)) > *((unsigned int*)x)) count++;
                        break;
                        // Étendre les cas à d'autres types si nécessaire
                    default:
                        printf("Type non supporté pour la comparaison\n");
                        return;
                }
            }
            node = node->succ;
        }

        // Print the results based on the type of `x`
        if (type == MY_INT || type == MY_UINT) {
            printf("Le nombre de valeurs superieures a %d est : %d\n", *((int*)x), count);
        } else if (type == MY_FLOAT) {
            printf("Le nombre de valeurs superieures a %f est : %d\n", *((float*)x), count);
        } else if (type == MY_DOUBLE) {
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
    LISTE* node = col->head;

    while (node != NULL) {
        if (node->val != NULL) {
            switch (type) {
                case MY_INT:
                    if (*((int*)(node->val)) < *((int*)x)) count++;
                    break;
                case MY_FLOAT:
                    if (*((float*)(node->val)) < *((float*)x)) count++;
                    break;
                case MY_DOUBLE:
                    if (*((double*)(node->val)) < *((double*)x)) count++;
                    break;
                case MY_UINT:
                    if (*((unsigned int*)(node->val)) < *((unsigned int*)x)) count++;
                    break;
                default:
                    printf("Comparaison inferieure non supportee pour ce type\n");
                    return;
            }
        }
        node = node->succ;
    }

    // Print the final count based on the data type of x
    switch (type) {
        case MY_INT:
        case MY_UINT:  // Assuming you want to handle unsigned ints as well
            printf("Le nombre de valeurs inferieures a %d est : %d\n", *((int*)x), count);
            break;
        case MY_FLOAT:
            printf("Le nombre de valeurs inferieures a %f est : %d\n", *((float*)x), count);
            break;
        case MY_DOUBLE:
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
    LISTE* node = col->head;

    while (node != NULL) {
        if (node->val != NULL) {
            switch (type) {
                case MY_INT:
                    if (*((int*)(node->val)) == *((int*)x)) count++;
                    break;
                case MY_FLOAT:
                    if (*((float*)(node->val)) == *((float*)x)) count++;
                    break;
                case MY_DOUBLE:
                    if (*((double*)(node->val)) == *((double*)x)) count++;
                    break;
                case MY_UINT:
                    if (*((unsigned int*)(node->val)) == *((unsigned int*)x)) count++;
                    break;
                case MY_CHAR:
                    if (*((char*)(node->val)) == *((char*)x)) count++;
                    break;
                case STRING:
                    if (strcmp((char*)node->val, (char*)x) == 0) count++;  // Compare strings
                    break;
                default:
                    printf("Comparison not supported for this type\n");
                    return;
            }
        }
        node = node->succ;
    }

    // Print results based on the type of x
    if (type == MY_INT || type == MY_UINT) {
        printf("Le nombre de valeurs egales a %d est : %d\n", *((int*)x), count);
    } else if (type == MY_FLOAT) {
        printf("Le nombre de valeurs egales a %f est : %d\n", *((float*)x), count);
    } else if (type == MY_DOUBLE) {
        printf("Le nombre de valeurs egales a %lf est : %d\n", *((double*)x), count);
    } else if (type == MY_CHAR) {
        printf("Le nombre de valeurs egales a '%c' est : %d\n", *((char*)x), count);
    } else if (type == STRING) {
        printf("Le nombre de valeurs egales a \"%s\" est : %d\n", (char*)x, count);
    }
}



LISTE* find_previous(LISTE* head, LISTE* node) {
    LISTE* current = head;
    if (current == node) return NULL;  // Le premier élément n'a pas de précédent
    while (current && current->succ != node) {
        current = current->succ;
    }
    return current;
}

void linked_list_insertion_sort(COLUMN* col) {
    if (!col || !col->head) {
        printf("Invalid column_list or column_list data.\n");
        return;
    }

    LISTE* sorted = NULL;
    LISTE* current = col->head;
    while (current != NULL) {
        LISTE* next = current->succ;

        // Insertion dans la liste triée
        if (sorted == NULL || compare((COL_TYPE*)sorted->val, (COL_TYPE*)current->val, col) >= 0) {
            current->succ = sorted;
            sorted = current;
        } else {
            LISTE* temp = sorted;
            while (temp->succ != NULL && compare((COL_TYPE*)temp->succ->val, (COL_TYPE*)current->val, col) < 0) {
                temp = temp->succ;
            }
            current->succ = temp->succ;
            temp->succ = current;
        }

        current = next;
    }
    col->head = sorted;
    col->valid_index = 1;  // Marquer la colonne comme triée
    printf("Colonne triée avec succès.\n");
}

LISTE* partition(LISTE* head, LISTE* end, LISTE** newHead, LISTE** newEnd, COLUMN* col) {
    LISTE* pivot = end;
    LISTE* prev = NULL, *cur = head, *tail = pivot;

    // Parcourir jusqu'au pivot, réorganiser les éléments avant le pivot
    // ceux < pivot sont mis avant le pivot
    // ceux >= pivot sont mis après le pivot
    while (cur != pivot) {
        if (compare((COL_TYPE*)cur->val, (COL_TYPE*)pivot->val, col) < 0) {
            // Le premier élément que cur est inférieur au pivot
            if ((*newHead) == NULL) (*newHead) = cur;

            prev = cur;
            cur = cur->succ;
        } else {  // Si l'élément cur est plus grand ou égal au pivot
            if (prev) {
                prev->succ = cur->succ;
            }
            LISTE* tmp = cur->succ;
            cur->succ = NULL;
            tail->succ = cur;
            tail = cur;
            cur = tmp;
        }
    }

    // Si le pivot est le plus petit élément
    if (*newHead == NULL) *newHead = pivot;

    // Mise à jour newEnd au dernier élément de la liste inférieure
    *newEnd = tail;

    // Retourner le pivot
    return pivot;
}

int compare(COL_TYPE *a, COL_TYPE *b, COLUMN *col) {
    if (col->sort_dir == ASC) {  // Ascending order
        switch (col->column_type) {
            case MY_INT:
                return (*(int *)a - *(int *)b);
            case MY_FLOAT:
                return (*(float *)a > *(float *)b) - (*(float *)a < *(float *)b);
            case MY_CHAR:
                return (*(char *)a - *(char *)b);
            case MY_DOUBLE:
                return (*(double *)a > *(double *)b) - (*(double *)a < *(double *)b);
            case STRING:
                return strcmp((char *)a, (char *)b);
            default:
                printf("Unsupported column_list type for sorting.\n");
                return 0;
        }
    } else {  // Descending order
        switch (col->column_type) {
            case MY_INT:
                return (*(int *)b - *(int *)a);
            case MY_FLOAT:
                return (*(float *)b > *(float *)a) - (*(float *)b < *(float *)a);
            case MY_CHAR:
                return (*(char *)b - *(char *)a);
            case MY_DOUBLE:
                return (*(double *)b > *(double *)a) - (*(double *)b < *(double *)a);
            case STRING:
                return strcmp((char *)b, (char *)a);
            default:
                printf("Unsupported column_list type for sorting.\n");
                return 0;
        }
    }
}



LISTE* quickSortRecur(LISTE* head, LISTE* end, COLUMN* col) {
    if (!head || head == end)
        return head;

    LISTE* newHead = NULL, *newEnd = NULL;

    // Partitionner la liste, et retourner le pivot
    LISTE* pivot = partition(head, end, &newHead, &newEnd, col);

    // Si le pivot est le plus petit élément, il n'y a pas besoin de trier la partie avant pivot
    if (newHead != pivot) {
        // Gardez la partition avant pivot
        LISTE* tmp = newHead;
        while (tmp->succ != pivot)
            tmp = tmp->succ;
        tmp->succ = NULL;

        // Recur pour la liste avant pivot
        newHead = quickSortRecur(newHead, tmp, col);

        // Changer succ de la dernière node de la partie gauche
        find_previous(newHead, NULL)->succ = pivot;
    }

    // Recur pour la liste après pivot
    pivot->succ = quickSortRecur(pivot->succ, newEnd, col);

    return newHead;
}

void quickSort(COLUMN* col) {
    col->head = quickSortRecur(col->head, find_previous(col->head, NULL), col);
    printf("Colonne triée avec succès.\n");
}

void sort(COLUMN* col, int sort_dir) {
    if (!col || !col->head) {
        printf("Invalid column_list or column_list data.\n");
        return;
    }

    col->sort_dir = sort_dir;  // Définir la direction du tri

    if (col->valid_index == 0) {  // Pas du tout trié
        quickSort(col);
    } else if (col->valid_index == -1) {  // Partiellement trié (tout sauf le dernier élément)
        // La logique d'insertion dans une liste partiellement triée nécessiterait une insertion adaptée.
        // Ici, nous choisissons de simplement appliquer un tri rapide pour simplifier.
        // quickSort(col);
        linked_list_insertion_sort(col);  // Utilisation du tri par insertion pour liste chaînée
    } else if (col->valid_index == 1) {  // Déjà trié, juste trier si la direction a changé
        if (sort_dir != col->sort_dir) {
            quickSort(col);
        }
    }

    col->valid_index = 1;  // Marquer la colonne comme entièrement triée
    printf("Colonne triée avec succès.\n");
}



void print_col_by_index(COLUMN *col) {
    for (int i = 0; i < col->size; i++) {
        printf("[%llu] ", col->index[i]);
        switch (col->column_type) {
            case MY_INT:
                printf("%d\n", col->data[i]->int_value);
                break;
            case MY_FLOAT:
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



