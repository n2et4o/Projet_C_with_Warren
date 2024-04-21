//
// Created by 20220848 on 20/04/2024.
//
/*
 * str[N];
    mycol = create_column(INT, "My column");
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
 * case '6':
                            if (mycols[MAX_COLUMNS] == NULL){
                                printf("Aucune colonne n'a encore ete creee\n");
                                continue;

                            }
                            printf("Suppression de colonnes...\n");
                            printf("Que voulez vous supprimer :\n1 - Une colonne en particuliere\n"
                                   "2 - Un nombre precis de colonnes\n3 - Toutes les colonnes\n4 - Retourner au menu precedent\n");
                            do{
                                scanf(" %c",dstep3);
                            } while (dstep3 < '1' || dstep3 > '4');
                            switch (dstep3) {
                                case '1':
                                    int nb_col;
                                    printf("Entrez le numero de colonne que vous voulez supprimer entre (0 et %d):\n", index-1);
                                    do {
                                        scanf("%d",&nb_col);
                                    } while (nb_col < 0 || nb_col > index-1);

                                case '2':
                                    int deb_int,fin_int;
                                    printf("Entrez le numero d'intervalle de colonne que vous voulez supprimer entre (0 et %d) separer par une virgule ex: 2,3:\n", index-1);
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