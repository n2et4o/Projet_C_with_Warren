//
// Created by 20220848 on 20/04/2024.
//
/*
 * str[N];
    mycol = create_column(INT, "My column_list");
    int o = 52, p = 44, n = 15, d = 18;
    int *nul = NULL;
    insert_value(mycol, &o);
    insert_value(mycol, &p);
    insert_value(mycol, &n);
    insert_value(mycol, nul);
    insert_value(mycol, &d);
    insert_value(mycol, NULL);
    print_col(mycol);
    printf("pre_conv \n");
    convert_value(mycol, 2, str, N);
    printf("%s\n", str);
    print_col(mycol);
    printf("post_conv\n");
    delete_column(&mycol);
 */
/*
 * void sort(COLUMN* col, int sort_dir) {
    quicksort(col->data, 0, col->size - 1, sort_dir);
    col->valid_index = 1; // Set index as valid after sorting
}
 */

/*
 * int partition(COL_TYPE **data, int left, int right, ENUM_TYPE type, int sort_dir) {
    COL_TYPE *pivot = data[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (compare(data[j], pivot, type, sort_dir) < 0) {
            i++;
            swap(&data[i], &data[j]);
        }
    }

    swap(&data[i + 1], &data[right]);
    return i + 1;
}
 */

/*
 * void swap(COL_TYPE **a, COL_TYPE **b) {
    COL_TYPE *temp = *a;
    *a = *b;
    *b = temp;
}
 */

/*
 * fgets(name_col, sizeof(name_col), stdin);
                            name_col[strcspn(name_col, "\n")] = 0;  // Enlève le newline potentiellement lu par fgets

 */

/*
 * if (mycol == NULL) {
                                printf("Échec de la creation de la colonne.\n");
                            } else {
                                printf("Colonne '%s' creee avec succes.\n", name_col);
                            }
 */

/*
 * case '5':
                            if (mycols[MAX_COLUMNS] == NULL){
                                printf("Aucune colonne n'a encore ete creee\n");
                                continue;

                            }
                            printf("Tri de colonnes...\n");
                            printf("Que voulez vous trier :\n1 - Une colonne en particuliere\n"
                                   "2 - Un nombre precis de colonnes\n3 - Toutes les colonnes\n4 - Retourner au menu precedent\n");
                            do{
                                scanf(" %c",dstep3);
                            } while (dstep3 < '1' || dstep3 > '4');
                            switch (dstep3) {
                                case '1':
                                    int nb_col;
                                    printf("Entrez le numero de colonne que vous voulez trier entre (0 et %d):\n", index-1);
                                    do {
                                        scanf("%d",&nb_col);
                                    } while (nb_col < 0 || nb_col > index-1);

                                case '2':
                                    int deb_int,fin_int;
                                    printf("Entrez le numero d'intervalle de colonne que vous voulez trier entre (0 et %d) separer par une virgule ex: 2,3:\n", index-1);
                                    do {
                                        scanf("%d,%d",deb_int,fin_int);
                                    }while(deb_int < 0 || deb_int > index-1 || deb_int > fin_int || fin_int < deb_int || fin_int > index || fin_int < 0);
                                    for (int i = deb_int; i < fin_int; i++) {

                                    }
                                case '3':
                                    for (int i = 0; i < index; i++) {
                                        ;
                                    }
                                    break;
                                case '4':
                                    continue;
                            }
                            break;
 */

// fonction de tri en LLC

/*
 *
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

 */

/*
 *
 */