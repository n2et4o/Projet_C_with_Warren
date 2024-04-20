//
// Created by 20220848 on 20/04/2024.
//
#include "column.h"


void menu(){
    printf("Bonjour !\n");
    sleep(1);
    printf("Bienvenue sur votre CDataframe.\n\n");
    sleep(1);
    printf("Choississez un chiffre \n");
    char start,step1,step2,step3,step4,step5,dstep1,dstep2,dstep3,dstep4,dstep5,dstep6;
    int go = 1, index = 0;
    char choice;
    COLUMN* mycols[MAX_COLUMNS] = {NULL};  // Tableau de pointeurs vers COLUMN, initialisé à NULL

    while (go) {
        printf("1 - Commencez\n"
               "2 - Options\n"
               "3 - Comment ca marche ?\n"
               "4 - C'est quoi un CDataframe et a quoi ca sert ?\n"
               "5 - Quitter\n");
        printf("Entrez votre choix : ");
        scanf(" %c", &choice);  // Assurez-vous de capturer les blancs avant le caractère
        printf("\n");

        switch (choice) {
            case '1':
                do {
                    printf("1 - Creez une colonne\n"
                           "2 - Inserez une valeur dans une colonne\n"
                           "3 - Affichez une colonne\n"
                           "4 - Recherchez dans une colonne\n"
                           "5 - Triez une colonne\n"
                           "6 - Retournez au menu principal\n"
                           "7 - Quittez le programme\n");
                    printf("Entrez votre choix : ");
                    scanf(" %c", &step1);

                    switch(step1) {
                        case '1': {
                            if (index >= MAX_COLUMNS) {
                                printf("Nombre maximum de colonnes atteint.\n");
                                break;
                            }
                            printf("Creation d'une colonne...\n");
                            char name_col[256];
                            printf("Nommez votre colonne: ");
                            scanf("%s",name_col);
                            fgets(name_col, sizeof(name_col), stdin);
                            name_col[strcspn(name_col, "\n")] = 0;

                            printf("Choisissez le type de colonne que vous voulez creer :\n"
                                   "1 - Colonne d'entier\n"
                                   "2 - Colonne de reel\n"
                                   "3 - Colonne de caracteres\n"
                                   "4 - Colonne de chaine de caracteres\n"
                                   "5 - Colonne de reel long\n"
                                   "6 - Colonne de types multiples\n"
                                   "7 - Retourner au menu precedent\n");
                            printf("Entrez votre choix : ");
                            scanf(" %c", &dstep1);

                            switch (dstep1) {
                                case '1':
                                    mycols[index] = create_column(INT, name_col);
                                    break;
                                case '2':
                                    mycols[index] = create_column(FLOAT, name_col);
                                    break;
                                case '3':
                                    mycols[index] = create_column(CHAR, name_col);
                                    break;
                                case '4':
                                    mycols[index] = create_column(STRING, name_col);
                                    break;
                                case '5':
                                    mycols[index] = create_column(DOUBLE, name_col);
                                    break;
                                case '6':
                                    mycols[index] = create_column(STRUCTURE, name_col);
                                    printf("Bientôt disponible.\n");
                                    break;
                                case '7':
                                    continue;  // Continue pour revenir directement au menu de création
                            }

                            if (mycols[index] != NULL && mycols[index]->column_type != STRUCTURE ) {
                                printf("Colonne '%s' creee avec succes.\n", name_col);
                                index++;
                            } else {
                                printf("Echec de la création de la colonne.\n");
                            }
                            break;
                        }
                        case '2': {
                            int col_index;
                            printf("Entrez l'index de la colonne pour insertion (0 à %d): ", index-1);
                            scanf("%d", &col_index);
                            if (col_index < 0 || col_index >= index) {
                                printf("Index invalide.\n");
                                break;
                            }
                            switch (mycols[col_index]->column_type) {
                                case INT:
                                    // Exemple pour un entier, modifiez selon le type attendu
                                    int val;
                                    printf("Saississez une valeur entiere:\n");
                                    scanf("%d", &val);
                                    if (!insert_value(mycols[col_index], &val)) {
                                        printf("Échec de l'insertion.\n");
                                    } else {
                                        printf("Valeur insérée avec succes.\n");
                                    }
                                    break;
                                case FLOAT:
                                    // Exemple pour un entier, modifiez selon le type attendu
                                    float fval;
                                    printf("Saississez une valeur reelle:\n");
                                    scanf("%f", &fval);
                                    if (!insert_value(mycols[col_index], &fval)) {
                                        printf("Échec de l'insertion.\n");
                                    } else {
                                        printf("Valeur insérée avec succes.\n");
                                    }
                                    break;
                                case DOUBLE:
                                    // Exemple pour un entier, modifiez selon le type attendu
                                    double dval;
                                    printf("Saississez une valeur reelle longue:\n");
                                    scanf("%lf", &dval);
                                    if (!insert_value(mycols[col_index], &dval)) {
                                        printf("Échec de l'insertion.\n");
                                    } else {
                                        printf("Valeur insérée avec succes.\n");
                                    }
                                    break;
                                case CHAR:
                                    // Exemple pour un entier, modifiez selon le type attendu
                                    char cval;
                                    printf("Saissisez un caractere :\n");
                                    scanf(" %c", &cval);
                                    if (!insert_value(mycols[col_index], &cval)) {
                                        printf("Échec de l'insertion.\n");
                                    } else {
                                        printf("Valeur insérée avec succes.\n");
                                    }
                                    break;
                                case STRING:
                                    // Exemple pour un entier, modifiez selon le type attendu
                                    char sval[256];
                                    printf("Saississez une chaine de caractere :\n");
                                    scanf("%s", sval);
                                    if (!insert_value(mycols[col_index], &sval)) {
                                        printf("Échec de l'insertion.\n");
                                    } else {
                                        printf("Valeur insérée avec succes.\n");
                                    }
                                    break;
                                case STRUCTURE:
                                    continue;
                            }

                        }
                        case '3':
                            printf("Affichage des colonnes...\n");
                            printf("Que voulez vous afficher :\n1 - Une colonne en particuliere\n"
                                   "2 - Un nombre precis de colonnes\n3 - Toutes les colonnes\n4 - Retourner au menu precedent\n");
                            do{
                                scanf(" %c",dstep3);
                            } while (dstep3 < '1' || dstep3 > '4');
                            switch (dstep3) {
                                case '1':
                                    int nb_col;
                                    printf("Entrez le numero de colonne que vous voulez afficher entre (0 et %d):\n", index-1);
                                    do {
                                        scanf("%d",&nb_col);
                                    } while (nb_col < 0 || nb_col > index-1);
                                    print_col(mycols[nb_col]);
                                case '2':
                                    int deb_int,fin_int;
                                    printf("Entrez le numero d'intervalle de colonne que vous voulez afficher entre (0 et %d) separer par une virgule ex: 2,3:\n", index-1);
                                    do {
                                        scanf("%d,%d",deb_int,fin_int);
                                    }while(deb_int < 0 || deb_int > index-1 || deb_int > fin_int || fin_int < deb_int || fin_int > index || fin_int < 0);
                                    for (int i = deb_int; i < fin_int; i++) {
                                        print_col(mycols[i]);
                                    }
                                case '3':
                                    for (int i = 0; i < index; i++) {
                                        print_col(mycols[i]);
                                    }
                                    break;
                                case '4':
                                    continue;
                            }
                            break;
                        case '4':
                            printf("Recherche dans une colonne...\n");
                            int col_index;
                            printf("Entrez l'index de la colonne pour la recherche (0 à %d): ", index-1);
                            scanf("%d", &col_index);
                            if (col_index < 0 || col_index >= index) {
                                printf("Index invalide.\n");
                                break;
                            }
                            printf("1 - Recherchez la valeur présente à la position x\n2 - Recherchez le nombre de d’occurrences d’une valeur x\n"
                                   "3 - Recherchez le nombre de valeurs qui sont supérieures à x\n4 - Recherchez le nombre de valeurs qui sont inférieures à x\n"
                                   "5 - Recherchez le nombre de valeurs qui sont égales à x\n6 - Retouner au menu precedent\n7 - Quittez\n");
                            do {
                                scanf(" %c",dstep4);
                            }while(dstep4 < '1' || dstep4 > '7');
                            switch (dstep4) {
                                case '1':
                                    printf("Entrez la position de la valeur recherchee :");

                            }
                            break;
                        case '5':
                            printf("Tri d'une colonne...\n");
                            break;
                        case '6':
                            printf("Retour au menu principal...\n\n");
                            break;
                        case '7':
                            printf("Au revoir !\n");
                            go = 0; // Sortie du programme
                            break;
                        default:
                            printf("Choix non valide. Veuillez réessayer.\n");
                            break;
                    }
                } while(step1 != '7');
                break;
            case '2':
                printf("Options - Pas encore implémenté.\n\n");
                break;
            case '3':
                printf("Suivez les instructions le programme est très intuitif et vous guide durant tout le long.\n\n");
                break;
            case '4':
                printf("CDataframe expliqué - Pas encore implémenté.\n\n");
                break;
            case '5':
                printf("Au revoir!\n");
                go = 0; // Sortie de la boucle while
                break;
            default:
                printf("Choix non valide. Veuillez réessayer.\n\n");
                break;
        }
    }
}