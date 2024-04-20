//
// Created by 20220848 on 31/03/2024.
//
#include "column.h"

#define N 5

int main(){
    //menu();
    COLUMN *mycol1 = create_column(STRUCTURE,"My column");
    int az = 1;char aq = 'e';float re = 3.5;
    insert_value(mycol1,&az);
    insert_value(mycol1,&aq);
    insert_value(mycol1,&re);
    print_col(mycol1);
    printf("start\n");
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
    nb_occ(mycol, NULL);

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

    printf("\ntest\n");
    char str[5];
    mycol = create_column(INT, "My column");
    int ac = 52, bc = 44, cc = 15, dc = 18;
    insert_value(mycol, &ac);
    insert_value(mycol, &bc);
    insert_value(mycol, &cc);
    insert_value(mycol, &dc);
    convert_value(mycol, 2, str, N);
    printf("%s \n", str);
    delete_column(&mycol);
    printf("end test\n\n");

    mycol = create_column(INT, "sorted column");
    int ab = 52;
    int bb = 44;
    int cb = 15;
    int db = 18;
    insert_value(mycol, &ab);
    insert_value(mycol, &bb);
    insert_value(mycol, &cb);
    insert_value(mycol, &db);
    printf("Column content before sorting : \n");
    print_col(mycol);
    printf("test\n");
    sort(mycol,DESC);
    printf("actually test\n");
    printf("Column content after sorting : \n");
    print_col(mycol);
    printf("end test\n");
    //print_col_by_index(mycol);
    printf("reussite\n");

    delete_column(&mycol);
    mycol = create_column(STRING, "String column");
    char et[234] = "Lima";
    char tr[234] ="Bravo";
    char ty[234] ="Zulu";
    char yt[234] ="Tango";
    insert_value(mycol, &et);
    insert_value(mycol, &tr);
    insert_value(mycol,&ty );
    insert_value(mycol, &yt);
    printf("Column content before sorting : \n");
    print_col(mycol);
    sort(mycol,ASC);
    printf("Column content after sorting : \n");
    //print_col_by_index(mycol);
    print_col(mycol);

    return 0;
};



