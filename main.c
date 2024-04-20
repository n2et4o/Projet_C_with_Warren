//
// Created by 20220848 on 31/03/2024.
//
#include "column.h"
#define N 7

int main(){

    COLUMN *mycol = create_column(INT,"My column");
    int val = 5;
    if (insert_value(mycol, &val)){
        printf("Value added successfully to my column\n");}
    else{
        printf("Error adding value to my column\n");}

    delete_column(&mycol);
    int b = 53;
    mycol = create_column(INT,"My column");
    insert_value(mycol, &b);
    b =42;
    insert_value(mycol, &b);
    b = 34;
    insert_value(mycol, NULL);
    print_col(mycol);

    int z = 2;
    pos_val(mycol,z);
    
    int a = 52 ;
    nb_occ(mycol, &a);
    
    a = 50;
    nb_val_supe(mycol, &a);
    
    nb_val_inf(mycol, &a);

    nb_val_egal(mycol, &a);

    mycol = create_column(CHAR, "My column");
    char j = 'A', c = 'C';
    insert_value(mycol, &j);
    insert_value(mycol, NULL);
    insert_value(mycol, &c);
    print_col(mycol);

    char str[N];
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
    convert_value(mycol, 5, str, N);
    printf("post_conv\n");
    printf("%s\n", str);
    print_col(mycol);
    delete_column(&mycol);

    mycol = create_column(INT, "sorted column");
    int ab = 52;
    int bb = 44;
    int cb = 15;
    int db = 18;
    insert_value(mycol, &a);
    insert_value(mycol, &b);
    insert_value(mycol, &c);
    insert_value(mycol, &d);
    printf("Column content before sorting : \n");
    print_col(mycol);
    sort(mycol,ASC);
    printf("Column content after sorting : \n");
    print_col_by_index(mycol);




    return 0;
};



