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