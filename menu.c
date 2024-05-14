//
// Created by 20220848 on 20/04/2024.
//
#include "cdataframe_list.h"


/*
 * void menu(){
    printf("Bonjour ! \n");
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
               "2 - Credit\n"
               "3 - Comment ça marche ?\n"
               "4 - C'est quoi un CDataframe et à quoi ça sert ?\n"
               "5 - Quitter\n");
        printf("Entrez votre choix : ");
        scanf(" %c", &choice);  // Assurez-vous de capturer les blancs avant le caractère
        printf("\n");

        switch (choice) {
            case '1':
                do {
                    printf("1 - Creer une colonne\n"
                           "2 - Inserer une valeur dans une colonne\n"
                           "3 - Afficher une colonne\n"
                           "4 - Rechercher dans une colonne\n"
                           "5 - Trier une colonne\n"
                           "6 - Supprimer une colonne\n"
                           "7 - Retourner au menu principal\n"
                           "8 - Quitter le programme\n");
                    printf("Entrer votre choix : ");
                    scanf(" %c", &step1);

                    switch(step1) {
                        case '1': {
                            if (index >= MAX_COLUMNS) {
                                printf("Nombre maximum de colonnes atteint.\n");
                                break;
                            }
                            printf("Création d'une colonne...\n");
                            char name_col[256];
                            printf("Nommer votre colonne: ");
                            scanf("%s",name_col);
                            fgets(name_col, sizeof(name_col), stdin);
                            name_col[strcspn(name_col, "\n")] = 0;

                            printf("Choisisser le type de colonne que vous voulez créer :\n"
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
                                    mycols[index] = create_column(MY_INT, name_col);
                                    break;
                                case '2':
                                    mycols[index] = create_column(MY_FLOAT, name_col);
                                    break;
                                case '3':
                                    mycols[index] = create_column(MY_CHAR, name_col);
                                    break;
                                case '4':
                                    mycols[index] = create_column(STRING, name_col);
                                    break;
                                case '5':
                                    mycols[index] = create_column(MY_DOUBLE, name_col);
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
                                printf("Echec de la creation de la colonne.\n");
                            }
                            break;
                        }
                        case '2': {
                            if (mycols[MAX_COLUMNS] == NULL){
                                printf("Aucune colonne n'a encore ete creee\n");
                                continue;
                            }
                            int col_index;
                            printf("Entrez l'index de la colonne pour insertion (0 a %d): ", index-1);
                            scanf("%d", &col_index);
                            if (col_index < 0 || col_index >= index) {
                                printf("Index invalide.\n");
                                break;
                            }
                            switch (mycols[col_index]->column_type) {
                                case MY_INT:
                                    // Exemple pour un entier, modifiez selon le type attendu
                                    int val;
                                    printf("Saississez une valeur entiere:\n");
                                    scanf("%d", &val);
                                    if (!insert_value(mycols[col_index], &val)) {
                                        printf("Echec de l'insertion.\n");
                                    } else {
                                        printf("Valeur inseree avec succes.\n");
                                    }
                                    break;
                                case MY_FLOAT:
                                    // Exemple pour un entier, modifiez selon le type attendu
                                    float fval;
                                    printf("Saississez une valeur reelle:\n");
                                    scanf("%f", &fval);
                                    if (!insert_value(mycols[col_index], &fval)) {
                                        printf("Echec de l'insertion.\n");
                                    } else {
                                        printf("Valeur inseree avec succes.\n");
                                    }
                                    break;
                                case MY_DOUBLE:
                                    // Exemple pour un entier, modifiez selon le type attendu
                                    double dval;
                                    printf("Saississez une valeur reelle longue:\n");
                                    scanf("%lf", &dval);
                                    if (!insert_value(mycols[col_index], &dval)) {
                                        printf("Echec de l'insertion.\n");
                                    } else {
                                        printf("Valeur inseree avec succes.\n");
                                    }
                                    break;
                                case MY_CHAR:
                                    // Exemple pour un entier, modifiez selon le type attendu
                                    char cval;
                                    printf("Saissisez un caractere :\n");
                                    scanf(" %c", &cval);
                                    if (!insert_value(mycols[col_index], &cval)) {
                                        printf("Echec de l'insertion.\n");
                                    } else {
                                        printf("Valeur inseree avec succes.\n");
                                    }
                                    break;
                                case STRING:
                                    // Exemple pour un entier, modifiez selon le type attendu
                                    char sval[256];
                                    printf("Saississez une chaine de caractere :\n");
                                    scanf("%s", sval);
                                    if (!insert_value(mycols[col_index], &sval)) {
                                        printf("Echec de l'insertion.\n");
                                    } else {
                                        printf("Valeur inseree avec succes.\n");
                                    }
                                    break;
                                case STRUCTURE:
                                    continue;
                            }

                        }
                        case '3':
                            if (mycols[MAX_COLUMNS] == NULL){
                                printf("Aucune colonne n'a encore ete creee\n");
                                continue;

                            }
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
                            if (mycols[MAX_COLUMNS] == NULL){
                                printf("Aucune colonne n'a encore ete creee\n");
                                continue;
                            }
                            printf("Recherche dans une colonne...\n");
                            int col_index;
                            printf("Entrez l'index de la colonne pour la recherche (0 a %d): ", index-1);
                            scanf("%d", &col_index);
                            if (col_index < 0 || col_index >= index) {
                                printf("Index invalide.\n");
                                break;
                            }
                            printf("1 - Recherchez la valeur presente a la position x\n2 - Recherchez le nombre de d’occurrences d’une valeur x\n"
                                   "3 - Recherchez le nombre de valeurs qui sont superieures a x\n4 - Recherchez le nombre de valeurs qui sont inferieures a x\n"
                                   "5 - Recherchez le nombre de valeurs qui sont egales a x\n6 - Retouner au menu precedent\n7 - Quittez\n");
                            do {
                                scanf(" %c",dstep4);
                            }while(dstep4 < '1' || dstep4 > '7');
                            switch (dstep4) {
                                case '1':
                                    printf("Entrez la position de la valeur recherchee :");

                            }
                            break;
                        case '5':
                            if (mycols[MAX_COLUMNS] == NULL){
                                printf("Aucune colonne n'a encore ete creee\n");
                                continue;
                            }
                            printf("Tri de colonnes...\n");
                            break;
                        case '6':
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
                        case '7':
                            printf("Retour au menu principal...\n\n");
                            break;
                        case '8':
                            printf("Au revoir !\n");
                            go = 0; // Sortie du programme
                            break;
                        default:
                            printf("Choix non valide. Veuillez reessayer.\n");
                            break;
                    }
                } while(step1 != '8');
                break;
            case '2':
                printf("Programme realise par \n\n");
                sleep(1);
                printf("Joss DOUNIAMA OKANA & Warren BADEAU\n\n");
                break;
            case '3':
                printf("Suivez les instructions le programme est tres intuitif et vous guide durant tout le long.\n\n");
                break;
            case '4':
                printf("Qu'est ce qu'un CDataframe ?\n");
                sleep(2.5);
                printf("Un CDataframe est un dataframe en languague C\n");
                sleep(2.5);
                printf("Alors qu'est ce qu'un Dataframe ?\n");
                sleep(2.5);
                printf("Dans les langages d'analyse de donnees,");
                sleep(2.5);
                printf( "un dataframe est une structure de donnees tabulaire,\nc'est-a-dire qu'elle est composee de lignes et de colonnes,\n");
                sleep(2.5);
                printf("ou chaque colonne peut contenir des donnees d'un type different. Les dataframes permettent de stocker,\n");
                sleep(2.5);
                printf("de manipuler et d'analyser des donnees structurees de maniere efficace et intuitive.\n\n");
                sleep(2.5);
                printf("A quoi sert un CDataframe ?\n");
                sleep(2.5);
                printf("* Stockage Tabulaire de Donnees: Comme un tableau ou un ensemble de tableaux qui stockent \n\t\t\tdes donnees structurees en lignes et en colonnes.\n");
                sleep(2.5);
                printf("* Heterogeneite des Types: La capacite de stocker differents types de donnees \n\t\t\t(par exemple, des entiers, des flottants, des chaines de caracteres) ");
                sleep(2.5);
                printf("\n\t\t\tdans differentes colonnes, ce qui est different d'un tableau C standard\n\t\t\t qui ne peut stocker qu'un seul type de donnees.\n");
                sleep(2.5);
                printf("* Manipulation de Donnees: Des fonctions associees pour trier, filtrer, regrouper \n\t\t\tet effectuer d'autres operations sur les donnees.\n");
                sleep(2.5);
                printf("* Analyse de Donnees: Des outils integres ou supplementaires qui permettent \n\t\t\tde calculer des statistiques descriptives, de construire des modeles, etc.\n\n");
                sleep(2.5);
                break;
            case '5':
                printf("Au revoir!\n");
                go = 0; // Sortie de la boucle while
                break;
            default:
                printf("Choix non valide. Veuillez reessayer.\n\n");
                break;
        }
    }
}
 */




void menu2() {
    // Changer le code page de la console en UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // Activer le support pour les séquences d'échappement ANSI qui peut aider dans certains cas
    system("chcp 65001");
    // Assurez-vous que votre éditeur enregistre ce fichier en UTF-8
    printf("Bonjour !\n");
    sleep(1);
    printf("Bienvenue sur votre CDataframe.\n\n");
    sleep(1);
    printf("Choisissez un chiffre :\n");

    // Création d'une dataframe vide initial
    CDATAFRAME *dataframe = create_empty_cdataframe();
    if (dataframe == NULL) {
        fprintf(stderr, "XzzzzX.\n");
        return;  // En cas d'échec de la création, arrête le programme
    }    int go = 1;
    char choice;

    while (go) {
        printf(
                "1 - Commencer\n"
               "2 - Credit\n"
               "3 - Comment ça marche ?\n"
               "4 - C'est quoi un CDataframe et à quoi ça sert ?\n"
               "5 - Quitter\n");
        printf("Entrez votre choix : ");
        scanf(" %c", &choice);  // Assurez-vous de capturer les blancs avant le caractère
        printf("\n");

        switch (choice) {
            case '1': {
                char step1;
                do {
                    printf("1 - Création & Gestion\n"
                           "2 - Inserer une valeur dans une colonne\n"
                           "3 - Afficher \n"
                           "4 - Rechercher dans une colonne\n"
                           "5 - Trier une colonne\n"
                           "6 - Supprimer une colonne\n"
                           "7 - Retourner au menu principal\n"
                           "8 - Quitter le programme\n");
                    printf("Entrer votre choix : ");
                    scanf(" %c", &step1);

                    switch(step1) {
                        case '1':
                            create_column_process(dataframe);
                            printf("\n");
                            break;
                        case '2':
                            // Processus d'insertion de valeur
                            insert_value_process(dataframe);
                            printf("\n");
                            break;
                        case '3':
                            // Processus d'affichage de colonnes
                            print_columns_process(dataframe);
                            printf("\n");
                            break;
                        case '4':
                            // Processus de recherche dans les colonnes
                            search_columns_process(dataframe);
                            printf("\n");
                            break;
                        case '5':
                            // Processus de tri des colonnes
                            sort_columns_process(dataframe);
                            //printf("bientot disponible\n");
                            printf("\n");
                            break;
                        case '6':
                            delete_columns_process(dataframe);
                            printf("\n");
                            break;
                        case '7':
                            printf("Retour au menu principal...\n\n");
                            //continue;
                            break;
                        case '8':
                            go = 0;  // Sortie du programme
                            break;
                        default:
                            printf("Choix non valide. Veuillez reessayer.\n");
                            break;
                    }
                } while (step1 != '7'&& step1 != '8');
                break;
            }
            case '2':
                printf("Programme realise par:\nJoss DOUNIAMA OKANA & Warren BADEAU\n\n");
                break;
            case '3':
                printf("Suivez les instructions le programme est tres intuitif et vous guide durant tout le parcours.\n\n");
                break;
            case '4':
               explain_cdataframe();
                break;
            case '5':
                printf("Au revoir!\n");
                go = 0;  // Sortie de la boucle while
                break;
            default:
                printf("Choix non valide. Veuillez reessayer.\n\n");
                break;
        }
    }

    delete_cdataframe(&dataframe);  // Libérer toute la mémoire allouée au dataframe
    printf("Au revoir !\n");
}




