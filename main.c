//
// Created by 20220848 on 31/03/2024.
//
#include "CDataframe.h"

int main(){

    COLUMN *mycol = create_column("My column");
    int val = 5;
    if (insert_value(mycol, val))
        printf("Value added successfully to my column\n");
    else
        printf("Error adding value to my column\n");

    return 0;
};


