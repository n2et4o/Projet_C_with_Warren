//
// Created by 20220848 on 31/03/2024.
//
#include "column.h"

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

    int value = pos_val(mycol, 1);
    printf("La valeur a la position 1 est : %d\n", value);

    int a = 52 ;
    int occ = nb_occ(mycol, a);
    printf("Le nombre d'occurrences de la valeur %d est : %d\n", a,occ);

    a = 50;
    int count = nb_val_supe(mycol, a);
    printf("Le nombre de valeurs superieures a %d est : %d\n", a,count);

    count = nb_val_inf(mycol, a);
    printf("Le nombre de valeurs inferieures a %d est : %d\n", a,count);

    count = nb_val_egal(mycol, a);
    printf("Le nombre de valeurs egales a %d est : %d\n", a,count);

    mycol = create_column(CHAR, "My column");
    char j = 'A', c = 'C';
    insert_value(mycol, &j);
    insert_value(mycol, NULL);
    insert_value(mycol, &c);
    print_col(mycol);


#define N 7
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
    convert_value(mycol, 5, str, N);
    printf("%s\n", str);
    print_col(mycol);
    delete_column(&mycol);



    return 0;
};



