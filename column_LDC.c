//
// Created by 20220848 on 10/05/2024.
//

#include "column_list.h"

COLUMN* create_column(ENUM_TYPE type, char* title) {
    COLUMN *new_column = (COLUMN*) malloc(sizeof(COLUMN));
    if (new_column == NULL) {
        fprintf(stderr, "Memory allocation failed for new column\n");
        return NULL;
    }

    new_column->title = strdup(title);
    new_column->size = 0;
    new_column->max_size = 10; // Initial physical size
    new_column->column_type = type;
    new_column->data = (COL_TYPE**) calloc(new_column->max_size, sizeof(COL_TYPE*));
    new_column->index = (unsigned long long int*) calloc(new_column->max_size, sizeof(unsigned long long int));
    new_column->valid_index = 0;
    new_column->sort_dir = 0;
    new_column->head = NULL;
    new_column->next = NULL;

    if (new_column->data == NULL || new_column->index == NULL) {
        free(new_column->title);
        free(new_column->data);
        free(new_column->index);
        free(new_column);
        fprintf(stderr, "Memory allocation failed for column data\n");
        return NULL;
    }

    return new_column;
}

int insert_value(COLUMN *col, void *value, int position) {
    if (col == NULL) {
        return 0;
    }

    // Création d'un nouveau noeud pour la liste doublement chaînée
    LISTE *new_node = malloc(sizeof(LISTE));
    if (new_node == NULL) {
        return 0;
    }

    // Allocation et initialisation de la valeur selon le type de colonne
    switch (col->column_type) {
        case MY_UINT:
            new_node->val = malloc(sizeof(unsigned int));
            if (new_node->val == NULL) {
                free(new_node);
                return 0;
            }
            *((unsigned int *)new_node->val) = *((unsigned int *)value);
            break;
        case MY_INT:
            new_node->val = malloc(sizeof(int));
            if (new_node->val == NULL) {
                free(new_node);
                return 0;
            }
            *((int *)new_node->val) = *((int *)value);
            break;
        case MY_CHAR:
            new_node->val = malloc(sizeof(char));
            if (new_node->val == NULL) {
                free(new_node);
                return 0;
            }
            *((char *)new_node->val) = *((char *)value);
            break;
        case MY_FLOAT:
            new_node->val = malloc(sizeof(float));
            if (new_node->val == NULL) {
                free(new_node);
                return 0;
            }
            *((float *)new_node->val) = *((float *)value);
            break;
        case MY_DOUBLE:
            new_node->val = malloc(sizeof(double));
            if (new_node->val == NULL) {
                free(new_node);
                return 0;
            }
            *((double *)new_node->val) = *((double *)value);
            break;
        case STRING:
            new_node->val = strdup((char *)value);
            if (new_node->val == NULL) {
                free(new_node);
                return 0;
            }
            break;
        case STRUCTURE:
            // Assumant que MyStruct est défini quelque part
            new_node->val = malloc(sizeof(MyStruct));
            if (new_node->val == NULL) {
                free(new_node);
                return 0;
            }
            memcpy(new_node->val, value, sizeof(MyStruct));
            break;
        default:
            free(new_node);
            return 0;
    }

    if (position == 0) {  // Insertion en tête
        new_node->succ = col->head;
        new_node->prev = NULL;
        if (col->head != NULL) {
            col->head->prev = new_node;
        }
        col->head = new_node;
    } else {
        LISTE *temp = col->head;
        int count = 0;
        while (temp != NULL && count < position - 1) {
            temp = temp->succ;
            count++;
        }

        if (temp == NULL && position != 0) {  // Position est invalide (pas assez d'éléments)
            free(new_node->val);
            free(new_node);
            return 0;
        }

        new_node->succ = temp->succ;
        new_node->prev = temp;
        if (temp->succ != NULL) {
            temp->succ->prev = new_node;
        }
        temp->succ = new_node;
    }

    col->size++;
    return 1;
}

void delete_column(COLUMN** col) {
    if (*col == NULL) {
        printf("La colonne est déjà libérée ou n'existe pas\n");
        return;
    }

    // Libération de la mémoire allouée pour le titre
    free((*col)->title);

    // Parcourir et libérer chaque noeud de la liste chaînée
    LISTE *current = (*col)->head;
    while (current != NULL) {
        LISTE *next = current->succ;

        // Libération de la valeur stockée dans le noeud
        free(current->val);

        // Libération du noeud lui-même
        free(current);

        current = next; // Passer au noeud suivant
    }

    // Si les index sont utilisés, libérer cette mémoire
    if ((*col)->index != NULL) {
        free((*col)->index);
    }

    // Libération de la structure de colonne elle-même
    free(*col);

    // Mettre le pointeur de la colonne à NULL
    *col = NULL;
}



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

void nb_occ(COLUMN* col, void* x) {
    if (col == NULL) {
        printf("La colonne est NULL\n");
        return;
    }

    int count = 0;
    ENUM_TYPE type = col->column_type;
    LISTE* current = col->head;

    while (current != NULL) {
        if (x == NULL) {
            if (current->val == NULL) {
                count++;
            }
        } else if (current->val != NULL) {
            switch (type) {
                case MY_INT:
                    if (*(int*)(current->val) == *(int*)x) count++;
                    break;
                case MY_FLOAT:
                    if (*(float*)(current->val) == *(float*)x) count++;
                    break;
                case MY_CHAR:
                    if (*(char*)(current->val) == *(char*)x) count++;
                    break;
                case MY_DOUBLE:
                    if (*(double*)(current->val) == *(double*)x) count++;
                    break;
                case STRING:
                    if (strcmp((char*)(current->val), (char*)x) == 0) count++;
                    break;
                case MY_UINT:
                    if (*(unsigned int*)(current->val) == *(unsigned int*)x) count++;
                    break;
                default:
                    // Additional cases for STRUCTURE or other custom types can be implemented here
                    break;
            }
        }
        current = current->succ;
    }

    // Display the result
    if (x == NULL) {
        printf("Le nombre d'occurrences de la valeur NULL est : %d\n", count);
    } else {
        // Display the count based on type
        // Same switch as above can be used to format output based on type
        printf("Le nombre d'occurrences de la valeur spécifiée est : %d\n", count);
    }
}

void pos_val(COLUMN* col, unsigned int x) {
    if (col == NULL) {
        printf("La colonne est NULL\n");
        return;
    }

    if (x >= col->size) {
        printf("Position invalide\n");
        return;
    }

    LISTE* current = col->head;
    unsigned int count = 0;

    // Move to the x-th position
    while (count < x && current != NULL) {
        current = current->succ;
        count++;
    }

    if (current == NULL) {
        printf("La valeur à la position %u est : NULL\n", x);
    } else {
        // Assuming current->val points to the correct data type
        switch (col->column_type) {
            case MY_INT:
                printf("La valeur à la position %u est : %d\n", x, *((int*)current->val));
                break;
            case MY_FLOAT:
                printf("La valeur à la position %u est : %f\n", x, *((float*)current->val));
                break;
            case MY_CHAR:
                printf("La valeur à la position %u est : '%c'\n", x, *((char*)current->val));
                break;
            case MY_DOUBLE:
                printf("La valeur à la position %u est : %lf\n", x, *((double*)current->val));
                break;
            case STRING:
                printf("La valeur à la position %u est : \"%s\"\n", x, *((char**)current->val));
                break;
            case MY_UINT:
                printf("La valeur à la position %u est : %u\n", x, *((unsigned int*)current->val));
                break;
            case STRUCTURE:
                // Structure handling
                break;
            default:
                printf("Type non supporté\n");
                break;
        }
    }
}

void nb_val_supe(COLUMN* col, void* x) {
    if (col == NULL || x == NULL) {
        printf("Invalid column or comparison value\n");
        return;
    }

    int count = 0;
    ENUM_TYPE type = col->column_type;
    LISTE* current = col->head;

    while (current != NULL) {
        if (current->val != NULL) {
            switch (type) {
                case MY_INT:
                    if (*((int*)(current->val)) > *((int*)x)) count++;
                    break;
                case MY_FLOAT:
                    if (*((float*)(current->val)) > *((float*)x)) count++;
                    break;
                case MY_DOUBLE:
                    if (*((double*)(current->val)) > *((double*)x)) count++;
                    break;
                case MY_UINT:
                    if (*((unsigned int*)(current->val)) > *((unsigned int*)x)) count++;
                    break;
                default:
                    printf("Type non supporté pour la comparaison\n");
                    return;
            }
        }
        current = current->succ;
    }

    printf("Le nombre de valeurs superieures a %d est : %d\n", *((int*)x), count);
}

void nb_val_inf(COLUMN* col, void* x) {
    if (col == NULL || x == NULL) {
        printf("Invalid input\n");
        return;
    }

    int count = 0;
    ENUM_TYPE type = col->column_type;
    LISTE* current = col->head;

    while (current != NULL) {
        if (current->val != NULL) {
            switch (type) {
                case MY_INT:
                    if (*((int*)(current->val)) < *((int*)x)) count++;
                    break;
                case MY_FLOAT:
                    if (*((float*)(current->val)) < *((float*)x)) count++;
                    break;
                case MY_DOUBLE:
                    if (*((double*)(current->val)) < *((double*)x)) count++;
                    break;
                default:
                    printf("Comparaison inferieure non supportee pour ce type\n");
                    return;
            }
        }
        current = current->succ;
    }

    printf("Le nombre de valeurs inferieures a %d est : %d\n", *((int*)x), count);
}

void nb_val_egal(COLUMN* col, void* x) {
    if (col == NULL || x == NULL) {
        printf("Invalid input or comparison to NULL is not supported for non-pointer types\n");
        return;
    }

    int count = 0;
    ENUM_TYPE type = col->column_type;
    LISTE* current = col->head;

    while (current != NULL) {
        if (current->val != NULL) {
            switch (type) {
                case MY_INT:
                    if (*((int*)(current->val)) == *((int*)x)) count++;
                    break;
                case MY_FLOAT:
                    if (*((float*)(current->val)) == *((float*)x)) count++;
                    break;
                case MY_DOUBLE:
                    if (*((double*)(current->val)) == *((double*)x)) count++;
                    break;
                case MY_UINT:
                    if (*((unsigned int*)(current->val)) == *((unsigned int*)x)) count++;
                    break;
                case MY_CHAR:
                    if (*((char*)(current->val)) == *((char*)x)) count++;
                    break;
                case STRING:
                    if (strcmp((char*)current->val, (char*)x) == 0) count++;  // Compare strings
                    break;
                default:
                    printf("Comparison not supported for this type\n");
                    return;
            }
        }
        current = current->succ;
    }

    printf("Le nombre de valeurs egales a %d est : %d\n", *((int*)x), count);
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
        quickSort(col);  // Utilisation du tri par insertion pour liste chaînée
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

