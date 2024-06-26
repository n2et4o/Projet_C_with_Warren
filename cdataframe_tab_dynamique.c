//
// Created by 20220848 on 08/05/2024.
//

#include "cdataframe_list.h"

// ATTENTION pour utiliser des colonnes dynaniques, vous devriez mettre en commentaire le fichier "cdataframe_list.c" en ajoutant /* au debut et */ à la fin

 // Supprimer cette ligne et la dernière pour pouvoir utiliser un CDataframe dynanique

// ================ Fonctions de création ==================

CDATAFRAME *create_empty_cdataframe() {
    CDATAFRAME *cdf = (CDATAFRAME *)malloc(sizeof(CDATAFRAME));
    if (cdf == NULL) {
        fprintf(stderr, "Erreur: Echec de l'allocation pour le dataframe.\n");
        return NULL;
    }

    // Alloue de l'espace pour un nombre maximum de colonnes, mais commence sans aucune colonne
    cdf->columns = (COLUMN **)calloc(MAX_COLUMNS, sizeof(COLUMN *));
    if (cdf->columns == NULL) {
        fprintf(stderr, "Erreur: Echec de l'allocation pour les colonnes du dataframe.\n");
        free(cdf);
        return NULL;
    }

    cdf->num_columns = 0;  // Commence avec zéro colonne
    return cdf;
}


void rename_column(CDATAFRAME *cdf, int column_index, const char *new_title) {
    if (cdf == NULL || column_index < 0 || column_index >= cdf->num_columns) {
        fprintf(stderr, "Invalid column index or null dataframe\n");
        return;
    }

    COLUMN *column = cdf->columns[column_index];
    if (column == NULL) {
        fprintf(stderr, "Column at index %d is null\n", column_index);
        return;
    }

    // Libérer l'ancien titre si nécessaire
    free(column->title);

    // Allouer de la mémoire pour le nouveau titre et le copier
    column->title = malloc(strlen(new_title) + 1);
    if (column->title == NULL) {
        fprintf(stderr, "Memory allocation failed for new title\n");
        return;
    }
    strcpy(column->title, new_title);
}


// Helper function to parse the data based on the type
void* parse_data(char *data, ENUM_TYPE type) {
    void *parsed_data = NULL;
    switch(type) {
        case MY_UINT:
            parsed_data = malloc(sizeof(unsigned int));
            *(unsigned int *)parsed_data = (unsigned int)strtoul(data, NULL, 10);
            break;
        case MY_INT:
            parsed_data = malloc(sizeof(int));
            *(int *)parsed_data = atoi(data);
            break;
        case MY_FLOAT:
            parsed_data = malloc(sizeof(float));
            *(float *)parsed_data = atof(data);
            break;
        case MY_DOUBLE:
            parsed_data = malloc(sizeof(double));
            *(double *)parsed_data = strtod(data, NULL);
            break;
        case STRING:
            parsed_data = strdup(data);
            break;
            // Add other cases as necessary
        default:
            parsed_data = NULL;
    }
    return parsed_data;
}

CDATAFRAME* load_from_csv(char *file_name, ENUM_TYPE *dftype, int size) {
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file %s\n", file_name);
        return NULL;
    }

    CDATAFRAME *cdf = create_cdataframe(dftype, size);

    char line[1024];
    char *token;
    int row_count = 0;
    while (fgets(line, 1024, fp)) {
        if (row_count == 0) {  // skip header row
            row_count++;
            continue;
        }
        token = strtok(line, ",");
        for (int i = 0; i < size; i++) {
            if (token == NULL) break;
            cdf->columns[i]->data[cdf->columns[i]->size] = parse_data(token, dftype[i]);
            token = strtok(NULL, ",");
            cdf->columns[i]->size++;
        }
        row_count++;
    }

    fclose(fp);
    return cdf;
}

void save_into_csv(CDATAFRAME *cdf, char *file_name) {
    FILE *fp = fopen(file_name, "w");
    if (!fp) {
        fprintf(stderr, "Cannot open file %s\n", file_name);
        return;
    }

    // Assuming first row to be header names
    for (int i = 0; i < cdf->num_columns; i++) {
        fprintf(fp, "%s", cdf->columns[i]->title);
        if (i < cdf->num_columns - 1) fprintf(fp, ",");
    }
    fprintf(fp, "\n");

    // Find the max number of rows in any column
    int max_rows = 0;
    for (int i = 0; i < cdf->num_columns; i++) {
        if (cdf->columns[i]->size > max_rows) {
            max_rows = cdf->columns[i]->size;
        }
    }

    // Buffer for storing converted values
    char buffer[256]; // Adjust size as needed

    // Output each row
    for (int row = 0; row < max_rows; row++) {
        for (int col = 0; col < cdf->num_columns; col++) {
            COLUMN *current_col = cdf->columns[col];
            if (row < current_col->size) {
                // Use convert_value to get string representation
                convert_value(current_col, row, buffer, sizeof(buffer));
                fprintf(fp, "%s", buffer);
            } else {
                fprintf(fp, "%s", "");  // Handle empty cells if rows are uneven
            }
            if (col < cdf->num_columns - 1) fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}


CDATAFRAME *create_cdataframe(ENUM_TYPE *cdftype, int size) {
    CDATAFRAME *cdf = (CDATAFRAME *)malloc(sizeof(CDATAFRAME));
    if (!cdf) {
        fprintf(stderr, "Erreur: Echec de l'allocation pour le dataframe.\n");
        return NULL;
    }

    cdf->columns = (COLUMN **)malloc(sizeof(COLUMN *) * size);
    if (!cdf->columns) {
        fprintf(stderr, "Erreur: Echec de l'allocation pour les colonnes du dataframe.\n");
        free(cdf); // Libération du dataframe en cas d'échec
        return NULL;
    }

    cdf->num_columns = size;
    char col_title[MAX_TITLE_LENGTH];

    for (int i = 0; i < size; i++) {
        snprintf(col_title, MAX_TITLE_LENGTH, "Column %d", i + 1);  // Construction sécurisée du titre
        cdf->columns[i] = create_column(cdftype[i], col_title);
        if (!cdf->columns[i]) {
            fprintf(stderr, "Erreur: Echec de la creation de la colonne %d.\n", i + 1);
            for (int j = 0; j < i; j++) {
                delete_column(&(cdf->columns[j]));  // Utilisation de delete_column pour nettoyer les colonnes précédemment créées
            }
            free(cdf->columns);
            free(cdf);
            return NULL;
        }
    }

    return cdf;
}

// ================ Fonctions de suppression ==================

void delete_cdataframe(CDATAFRAME **cdf) {
    if (cdf == NULL || *cdf == NULL) {
        printf("Le dataframe est deja libere ou n'existe pas\n");
        return;
    }

    // Parcourir chaque colonne et la libérer en utilisant la fonction delete_column
    for (int i = 0; i < (*cdf)->num_columns; i++) {
        if ((*cdf)->columns[i] != NULL) {
            delete_column(&((*cdf)->columns[i]));
        }
    }

    // Libérer le tableau de colonnes
    free((*cdf)->columns);

    // Libérer la structure du dataframe
    free(*cdf);

    // Mettre le pointeur à NULL pour éviter des références à de la mémoire libérée
    *cdf = NULL;
    printf("Dataframe supprime avec succes\n");
}


//========

void delete_column_by_name(CDATAFRAME *cdf, char *col_name) {
    if (cdf == NULL || cdf->columns == NULL) {
        printf("Le dataframe est vide ou n'existe pas.\n");
        return;
    }

    int col_index = -1;
    for (int i = 0; i < cdf->num_columns; i++) {
        if (cdf->columns[i] != NULL && strcmp(cdf->columns[i]->title, col_name) == 0) {
            col_index = i;
            break;
        }
    }

    if (col_index == -1) {
        printf("Colonne '%s' non trouvee.\n", col_name);
        return;
    }

    // Libérer la colonne trouvée
    COLUMN *col = cdf->columns[col_index];
    if (col->title) free(col->title);
    if (col->data) {
        for (unsigned int i = 0; i < col->size; i++) {
            if (col->data[i] != NULL) {
                free(col->data[i]);  // Supposer que c'est un pointeur, ajustez selon le type stocké
            }
        }
        free(col->data);
    }
    if (col->index) free(col->index);
    free(col);

    // Ajuster les références dans le tableau de colonnes
    for (int i = col_index; i < cdf->num_columns - 1; i++) {
        cdf->columns[i] = cdf->columns[i + 1];
    }
    cdf->columns[cdf->num_columns - 1] = NULL;
    cdf->num_columns--;

    printf("Colonne '%s' supprimee avec succes.\n", col_name);
}

// ================ Fonction du menu ==================

const char* getTypeName(ENUM_TYPE type) {
    switch(type) {
        case MY_INT: return "INT";
        case MY_FLOAT: return "FLOAT";
        case MY_CHAR: return "CHAR";
        case STRING: return "STRING";
        case MY_DOUBLE: return "DOUBLE";
        case STRUCTURE: return "STRUCTURE";
        default: return "UNKNOWN";
    }
}

void rename_column_ui(CDATAFRAME *cdf) {
    if (cdf == NULL || cdf->num_columns == 0) {
        printf("Dataframe non initialise ou vide.\n");
        return;
    }

    int col_index;
    char new_name[256];
    printf("Entrez l'indice de la colonne à renommer (0 à %d) : ", cdf->num_columns - 1);
    scanf("%d", &col_index);
    clean_stdin(); // Nettoyer le buffer après la lecture d'un entier

    if (col_index < 0 || col_index >= cdf->num_columns) {
        printf("Indice de colonne invalide.\n");
        return;
    }

    printf("Entrez le nouveau nom pour la colonne : ");
    fgets(new_name, sizeof(new_name), stdin);
    new_name[strcspn(new_name, "\n")] = 0; // Enlever le newline à la fin

    rename_column(cdf, col_index, new_name);
    printf("Le nom de la colonne a été changé en '%s'.\n", new_name);
}

void load_dataframe(CDATAFRAME **cdf) {
    if (*cdf == NULL) {
        printf("Aucun dataframe à sauvegarder.\n");
        return;
    }
    char file_name[256];
    printf("Entrez le nom du fichier CSV à charger : ");
    scanf("%s",file_name);
    clean_stdin();  // Nettoyer le buffer après la lecture d'un entier
    file_name[strcspn(file_name, "\n")] = 0; // Enlever le newline à la fin

    // Simule les types pour chaque colonne ici, doit être dynamique selon le besoin
    ENUM_TYPE dftype[] = {MY_INT, MY_FLOAT}; // Exemple de types
    int num_cols = sizeof(dftype)/sizeof(dftype[0]);

    *cdf = load_from_csv(file_name, dftype, num_cols);
    if (*cdf != NULL) {
        printf("Dataframe chargé avec succès depuis '%s'.\n", file_name);
    } else {
        printf("Échec du chargement du dataframe depuis '%s'.\n", file_name);
    }
}

void save_dataframe(CDATAFRAME *cdf) {
    if (cdf == NULL) {
        printf("Aucun dataframe à sauvegarder.\n");
        return;
    }

    char file_name[256];
    printf("Entrez le nom du fichier CSV pour sauvegarder le dataframe : ");
    scanf("%s",file_name);
    clean_stdin();  // Nettoyer le buffer après la lecture d'un entier
    //fgets(file_name, sizeof(file_name), stdin);
    file_name[strcspn(file_name, "\n")] = 0; // Enlever le newline à la fin

    save_into_csv(cdf, file_name);
    printf("Dataframe sauvegardé dans '%s'.\n", file_name);
}

void create_row(CDATAFRAME *cdf) {
    if (cdf != NULL) {
    for (int i = 0; i < cdf->num_columns; i++) {
        printf("Entrez une valeur pour la colonne '%s' (%s): ", cdf->columns[i]->title, getTypeName(cdf->columns[i]->column_type));
        char input[256];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Enlever le newline à la fin
        void *data = parse_data(input, cdf->columns[i]->column_type);
        cdf->columns[i]->data[cdf->columns[i]->size++] = data;
    }
    printf("Nouvelle ligne ajoutée avec succès.\n");
    }
    else {
        printf("Aucun dataframe à sauvegarder.\n");
        return;
    }
}

void create_column_LDC(CDATAFRAME *dataframe){
        int num_columns_to_add = 0;
        printf("Combien de colonnes souhaitez-vous creer : ");
        scanf("%d", &num_columns_to_add);
        clean_stdin();  // Nettoyer le buffer après la lecture d'un entier
        printf("AZERTY");
        if (num_columns_to_add < 1) {
            printf("Nombre de colonnes non valide. Retour au menu precedent.\n");
            return;
        }

        if (dataframe->num_columns + num_columns_to_add > MAX_COLUMNS) {
            printf("Impossible de creer %d colonnes, cela depasse le maximum autorise de %d colonnes.\n", num_columns_to_add, MAX_COLUMNS);
            return;
        }
        int nb;
        for (int i = 0; i < num_columns_to_add; ++i) {
            if (dataframe->num_columns >= MAX_COLUMNS) {
                printf("Nombre maximum de colonnes atteint (%d colonnes).\n", MAX_COLUMNS);
                break;
            }

            printf("\nCreation de la colonne %d...\n", i);
            char name_col[256];
            printf("Nommez votre colonne : ");

            if (fgets(name_col, sizeof(name_col), stdin) == NULL) {
                printf("Erreur lors de la lecture du nom de la colonne.\n");
                continue;  // Continue avec la prochaine colonne
            }

            name_col[strcspn(name_col, "\n")] = 0;  // Supprime le newline
            printf("Vous avez nomme votre colonne : '%s'\n", name_col);

            ENUM_TYPE col_type;
            printf("Choisissez le type de colonne que vous voulez creer :\n"
                   "1 - Colonne d'entier (INT)\n"
                   "2 - Colonne de reel (FLOAT)\n"
                   "3 - Colonne de caracteres (CHAR)\n"
                   "4 - Colonne de chaine de caracteres (STRING)\n"
                   "5 - Colonne de reel long (DOUBLE)\n"
                   "6 - Colonne de types multiples (STRUCTURE)\n"
                   "7 - Annuler la creation de cette colonne et retourner au menu precedent\n");
            printf("Entrez votre choix : ");
            char choice;
            scanf(" %c", &choice);
            clean_stdin();  // Nettoyer le buffer après la lecture d'un caractère

            switch (choice) {
                case '1': col_type = MY_INT; break;
                case '2': col_type = MY_FLOAT; break;
                case '3': col_type = MY_CHAR; break;
                case '4': col_type = STRING; break;
                case '5': col_type = MY_DOUBLE; break;
                case '6': col_type = STRUCTURE; printf("Bientot disponible.\n"); continue;
                case '7': printf("Creation annulee. Retour.\n"); return;
                default: printf("Choix invalide. Retour au menu de creation.\n"); continue;
            }

            COLUMN* new_column = create_column(col_type, name_col);
            if (new_column == NULL) {
                printf("Echec de la creation de la colonne '%s'.\n", name_col);
            } else {
                dataframe->columns[dataframe->num_columns++] = new_column;
                printf("Colonne '%s' creee avec succes et ajoutee au dataframe.\n", name_col);
            }
            if ( i > 5) {  // Ask if more insertions are expected, unless it's the last one planned
                printf("Voulez-vous continuer ? (Oui: o / Non: n): ");
                char decision;
                scanf(" %c", &decision);
                getchar();  // Consume the newline character
                if (decision == 'n' || decision == 'N') {
                    break;  // Break the loop if user decides not to continue
                }
            }
            nb = i +1;
        }
        if (nb > 1){
            printf("Vous avez cree %d colonnes\n",nb);
        }
        else{
            printf("Vous avez cree %d colonne\n",nb);
        }
}

void clean_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}  // Cette boucle vide le tampon stdin.
}

void create_column_process(CDATAFRAME *dataframe) {
    printf("Gestion du Dataframe:\n"
           "1 - Créer une colonne\n"
           "2 - Créer une ligne\n"
           "3 - Renommer une colonne\n"
           "4 - Charger un Dataframe\n"
           "5 - Sauvegarder un Dataframe\n"
           "6 - Retour\n");
    int Nchoix;
    printf("Entrer votre choix : ");
    scanf("%d", &Nchoix);
    //clean_stdin();
    printf("12AZEret\n");
    switch (Nchoix) {
        case 1:
            create_column_LDC(dataframe);
            printf("\n");
            break;
        case 2:
            create_row(dataframe);
            printf("\n");
            break;
        case 3:
            rename_column_ui(dataframe);
            printf("\n");
            break;
        case 4:
            load_dataframe(&dataframe);
            printf("\n");
            break;
        case 5:
            save_dataframe(dataframe);
            printf("\n");
            break;
        case 6:
            break;
        default: printf("Choix invalide. Retour au menu de creation.\n");
        break;
    }
}


int position;

void insert_value_process(CDATAFRAME *dataframe) {
    if (dataframe == NULL || dataframe->num_columns == 0) {
        printf("Aucune colonne n'a encore ete creee.\n");
        return;
    }

    int col_index;
    printf("Entrez l'index de la colonne pour insertion (0 a %d): ", dataframe->num_columns - 1);
    scanf("%d", &col_index);
    getchar();  // Consume the newline character

    if (col_index < 0 || col_index >= dataframe->num_columns) {
        printf("Index invalide.\n");
        return;
    }

    COLUMN *col = dataframe->columns[col_index];
    if (col == NULL) {
        printf("La colonne %d n'existe pas.\n", col_index);
        return;
    }

    if (col->size == 0){
        position = 0;
    }
    else{
        printf("Choisissez le mode d'insertion :\n"
               "1 - En tête\n"
               "2 - En queue\n"
               "3 - À une position précise\n");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                position = 0;
                break;
            case 2:
                position = col->size;
                break;
            case 3:
                printf("Entrez la position (entre 0 et %d) :\n", col->size);
                scanf("%d", &position);
                if (position < 0 || position > col->size) {
                    printf("Position invalide.\n");
                    //return 0;
                }
                break;
            default:
                printf("Choix invalide.\n");
                //return 0;
        }
    }

    printf("Combien d'insertions voulez-vous realiser ?\n"
           "1 - Une seule\n"
           "2 - Plusieurs\n"
           "3 - Retourner au menu precedent\n");
    char insert_choice;
    scanf(" %c", &insert_choice);
    getchar();  // Consume the newline character



    int count = 1;  // Default is one insertion unless specified otherwise
    if (insert_choice == '2') {
        printf("Combien d'insertions souhaitez-vous effectuer : ");
        scanf("%d", &count);
        getchar();  // Consume the newline character
    } else if (insert_choice == '3') {
        return;  // Return to the previous menu
    }

    printf("Insertion dans la colonne '%s' de type '%s'.\n", col->title, getTypeName(col->column_type));

    for (int i = 0; i < count; i++) {
    if (!insert_single_value(col)) {
        printf("Echec de l'insertion a l'indice %d.\n", i);
        break;  // Stop further insertions on failure
    } else {
        printf("Valeur inseree avec succes a l'indice %d dans '%s'.\n", i, col->title);
    }

    // Poser la question après chaque tranche de 5 insertions, sauf après la première
    if ((i + 1) % 5 == 0 && i != count - 1) {  // Checking i != count - 1 prevents asking when insertion is done
        printf("Voulez-vous continuer ? (Oui: o / Non: n): ");
        char decision;
        scanf(" %c", &decision);
        getchar();  // Consume the newline character
        if (decision == 'n' || decision == 'N') {
            break;  // Break the loop if user decides not to continue
        }
    }
}

}


int insert_single_value(COLUMN* col) {

    switch (col->column_type) {
        case MY_INT: {
            int val;
            printf("Saisissez une valeur entiere:\n");
            scanf("%d", &val);
            getchar();  // Consume the newline character
            return insert_value(col, &val,position);  // Assuming insert_value returns 1 on success, 0 on failure
        }
        case MY_FLOAT: {
            float fval;
            printf("Saisissez une valeur reelle:\n");
            scanf("%f", &fval);
            getchar();  // Consume the newline character
            return insert_value(col, &fval,position);
        }
        case MY_DOUBLE: {
            double dval;
            printf("Saisissez une valeur reelle longue:\n");
            scanf("%lf", &dval);
            getchar();  // Consume the newline character
            return insert_value(col, &dval,position);
        }
        case MY_CHAR: {
            char cval;
            printf("Saisissez un caractere:\n");
            scanf(" %c", &cval);  // Notice the space before %c to consume any leftover newline characters
            getchar();  // Consume the newline character
            return insert_value(col, &cval,position);
        }
        case STRING: {
            char sval[256];
            printf("Saisissez une chaine de caracteres:\n");
            scanf(" %255s", sval);  // Use buffer limit to prevent overflow
            getchar();  // Consume the newline character
            return insert_value(col, sval,position);
        }
        case STRUCTURE:
            printf("Insertion de structures non supportee actuellement.\n");
            return 0;  // Return failure for STRUCTURE type
        default:
            printf("Type non supporte pour l'insertion.\n");
            return 0;
    }
}


void print_columns_process(CDATAFRAME *dataframe) {
    if (dataframe == NULL || dataframe->num_columns == 0) {
        printf("Aucune colonne n'a encore ete creee.\n");
        return;
    }
    printf("1 - Afficher le CDataframe\n"
           "2 - Afficher une colonne\n"
           "3 - Afficher une ligne\n"
           "4 - Afficher le nombre de lignes\n"
           "5 - Afficher le nombre de colonnes\n "
           "6 - Retourner au menu precedent\n");
    char option1;
    printf("Entrez votre choix : ");
    scanf(" %c", &option1);

    switch (option1) {
        case'1': // Gestion de l'affichage du CDataframe
            print_multiple_cols(dataframe, 0, dataframe->num_columns - 1);
            break;
        case '2': // Gestion de l'affichage des colonnes
            handle_column_display(dataframe);
            break;
        case '3': // Gestion de l'affichage des lignes
            handle_row_display(dataframe);
            break;
        case'4':
            if (dataframe == NULL || dataframe->columns == NULL) {
                printf("Dataframe non initialisé ou vide.\n");
                return;
            }

            int max_rows = 0;
            for (int i = 0; i < dataframe->num_columns; i++) {
                if (dataframe->columns[i]->size > max_rows) {
                    max_rows = dataframe->columns[i]->size;
                }
            }
            printf("Nombre de lignes : %d\n", max_rows);
            break;
        case'5':
            if (dataframe == NULL || dataframe->columns == NULL) {
                printf("Dataframe non initialisé ou vide.\n");
                return;
            }
            printf("Nombre de colonnes : %d\n", dataframe->num_columns);
            break;
        case '6':
            return;
        default:
            printf("Choix non valide.\n");
            break;
    }
}

void handle_column_display(CDATAFRAME *dataframe) {
    printf("Affichage des colonnes...\n");
    printf("Que voulez-vous afficher :\n"
           "1 - Une colonne en particulier\n"
           "2 - Un nombre precis de colonnes\n"
           "3 - Toutes les colonnes\n"
           "4 - Retourner au menu precedent\n");

    char option;
    printf("Entrez votre choix : ");
    scanf(" %c", &option);

    int col_index, start, end;
    switch (option) {
        case '1':
            printf("Entrez le numero de la colonne que vous voulez afficher (0 a %d) : ", dataframe->num_columns - 1);
            scanf("%d", &col_index);
            if (col_index < 0 || col_index >= dataframe->num_columns) {
                printf("Index invalide.\n");
            } else {
                printf("\nColonne %d : %s\t", col_index, dataframe->columns[col_index]->title);
                printf("\n");
                print_col(dataframe->columns[col_index]);
            }
            break;
        case '2':
            printf("Entrez l'indice de debut et de fin pour l'affichage des colonnes (ex: 1 3 pour afficher les colonnes 1 a 3) : ");
            scanf("%d %d", &start, &end);
            if (start < 0 || end >= dataframe->num_columns || start > end) {
                printf("Indices invalides.\n");
            } else {
                print_multiple_cols(dataframe, start, end);
            }
            break;
        case '3':
            print_multiple_cols(dataframe, 0, dataframe->num_columns - 1);
            break;
        case '4':
            return; // Retourner au menu précédent
        default:
            printf("Choix non valide.\n");
            break;
    }
}

void print_multiple_cols(CDATAFRAME *dataframe, int start, int end) {
    // Trouver le nombre maximal de lignes dans les colonnes à afficher
    int max_rows = 0;
    for (int i = start; i <= end; i++) {
        if (dataframe->columns[i]->size > max_rows) {
            max_rows = dataframe->columns[i]->size;
        }
    }

    // Afficher les titres des colonnes
    for (int i = start; i <= end; i++) {
        printf("Colonne %d : %-3s\t", i, dataframe->columns[i]->title); // Ajustez -20 selon la longueur maximale du titre
    }
    printf("\n");

    // Afficher les données des colonnes
    for (int row = 0; row < max_rows; row++) {
        for (int col = start; col <= end; col++) {
            char str[20]; // Taille suffisante pour stocker la conversion de la valeur
            if (row < dataframe->columns[col]->size) {
                convert_value(dataframe->columns[col], row, str, sizeof(str));
                printf(" [%d] : %-10s \t\t", row, str); // Utilisez %-20 pour justifier à gauche et ajustez selon le besoin
            } else {
                printf(" [%d] : %-10s \t\t", row, "NULL");  // Afficher NULL pour les indices hors limites, avec ajustement de l'espace
            }
        }
        printf("\n");
    }
}

void handle_row_display(CDATAFRAME *dataframe) {
    if (dataframe == NULL || dataframe->num_columns == 0) {
        printf("Aucune donnée disponible.\n");
        return;
    }

    printf("Affichage des lignes...\n"
           "1 - Afficher une ligne spécifique\n"
           "2 - Afficher un nombre limité de lignes\n"
           "3 - Afficher toutes les lignes\n"
           "4 - Retourner au menu précédent\n");

    char option;
    printf("Entrez votre choix : ");
    scanf(" %c", &option);

    int row_index;
    switch (option) {
        case '1':
            printf("Entrez le numéro de la ligne que vous voulez afficher (0 à %d) : ", dataframe->columns[0]->size - 1);
            scanf("%d", &row_index);
            if (row_index < 0 || row_index >= dataframe->columns[0]->size) {
                printf("Numéro de ligne invalide.\n");
            } else {
                print_single_row(dataframe, row_index);
            }
            break;

        case '2':
            print_limited_rows(dataframe);
            break;

        case '3':
            print_multiple_cols(dataframe, 0, dataframe->num_columns - 1);
            break;

        case '4':
            // Retourner au menu précédent
            return;

        default:
            printf("Choix non valide.\n");
            break;
    }
}

void print_single_row(CDATAFRAME *dataframe, int row_index) {
    if (row_index < 0 || row_index >= dataframe->columns[0]->size) {
        printf("Numéro de ligne hors limite.\n");
        return;
    }
    // Afficher les titres des colonnes
    for (int i = 0; i < dataframe->num_columns; i++) {
        printf("Colonne %-7s |\t", dataframe->columns[i]->title);
    }
    printf("\n");

    // Afficher les données pour la ligne spécifiée
    for (int col = 0; col < dataframe->num_columns; col++) {
        char str[256]; // Taille suffisante pour la conversion des valeurs
        if (row_index < dataframe->columns[col]->size) {
            convert_value(dataframe->columns[col], row_index, str, sizeof(str));
            printf(" [%d] : %-7s |\t\t", col,str);
        } else {
            printf(" [%d] : %-7s |\t\t", col,"NULL");
        }
    }
    printf("\n");
}


void print_limited_rows(CDATAFRAME *dataframe) {
    if (dataframe == NULL) {
        printf("Le dataframe est vide.\n");
        return;
    }

    int limit;
    printf("Entrez le nombre de lignes à afficher entre (0 à %d) : ",dataframe->columns[0]->size - 1);
    scanf("%d", &limit);

    if (limit <= 0) {
        printf("Nombre invalide de lignes à afficher.\n");
        return;
    }

    // Assurez-vous que la limite n'est pas supérieure au nombre de lignes dans n'importe quelle colonne
    int max_rows = 0;
    for (int i = 0; i < dataframe->num_columns; i++) {
        if (dataframe->columns[i]->size > max_rows) {
            max_rows = dataframe->columns[i]->size;
        }
    }

    if (limit > max_rows) {
        limit = max_rows;
    }

    // Afficher les titres des colonnes
    for (int i = 0; i < dataframe->num_columns; i++) {
        printf("Colonne %-7s |\t", dataframe->columns[i]->title);  // Justifier le titre à gauche et ajuster la largeur si nécessaire
    }
    printf("\n");

    // Afficher les lignes pour chaque colonne jusqu'à la limite
    for (int row = 0; row < limit; row++) {
        for (int col = 0; col < dataframe->num_columns; col++) {
            char str[256];  // Taille suffisante pour stocker la conversion de la valeur
            if (row < dataframe->columns[col]->size) {
                convert_value(dataframe->columns[col], row, str, sizeof(str));
                printf(" [%d] : %-7s |\t\t",row, str); // Justifier le texte à gauche
            } else {
                printf(" [%d] : %-7s |\t\t", row,"NULL"); // Afficher NULL pour les indices hors limites
            }
        }
        printf("\n");
    }
}


void search_columns_process(CDATAFRAME *dataframe) {
    if (dataframe == NULL || dataframe->num_columns == 0) {
        printf("Aucune colonne n'a encore ete creee.\n");
        return;
    }

    int col_index;
    printf("Recherche dans une colonne...\n");
    printf("Entrez l'index de la colonne pour la recherche (0 a %d): ", dataframe->num_columns - 1);
    scanf("%d", &col_index);

    if (col_index < 0 || col_index >= dataframe->num_columns) {
        printf("Index invalide.\n");
        return;
    }

    COLUMN *col = dataframe->columns[col_index];
    if (col == NULL) {
        printf("La colonne %d n'existe pas.\n", col_index);
        return;
    }

    char option;
    printf("Options de recherche:\n"
           "1 - Valeur a la position x\n"
           "2 - Nombre d'occurrences d'une valeur x\n"
           "3 - Nombre de valeurs superieures a x\n"
           "4 - Nombre de valeurs inferieures a x\n"
           "5 - Nombre de valeurs egales a x\n"
           "6 - Retourner au menu precedent\n"
           "Entrez votre choix : ");
    scanf(" %c", &option);

    switch (option) {
        case '1': {
            unsigned int pos;
            printf("Entrez la position de la valeur recherchee entre(0 à %d) : ",col->size);
            scanf("%u", &pos);
            pos_val(col, pos);
            break;
        }
        case '2': {
            void *x = malloc(col->size);  // Assuming `type_size` is defined in COLUMN to manage type sizes dynamically
            printf("Entrez la valeur x pour la recherche d'occurrences : ");
            scanf("%d", (int *)x);  // This assumes the user knows the type and the input is valid for the type
            nb_occ(col, x);
            free(x);
            break;
        }
        case '3': {
            void *x = malloc(col->size);
            printf("Entrez la valeur x pour comparer (superieures) : ");
            scanf("%d", (int *)x);
            nb_val_supe(col, x);
            free(x);
            break;
        }
        case '4': {
            void *x = malloc(col->size);
            printf("Entrez la valeur x pour comparer (inferieures) : ");
            scanf("%d", (int *)x);
            nb_val_inf(col, x);
            free(x);
            break;
        }
        case '5': {
            void *x = malloc(col->size);
            printf("Entrez la valeur x pour comparer (egales) : ");
            scanf("%d", (int *)x);
            nb_val_egal(col, x);
            free(x);
            break;
        }
        case '6':
            return;  // Retour au menu précédent
        default:
            printf("Option non valide.\n");
            break;
    }
}

void delete_columns_process(CDATAFRAME *dataframe) {
    if (dataframe == NULL || dataframe->num_columns == 0) {
        printf("Aucune colonne n'a encore ete creee.\n");
        return;
    }

    printf("Suppression de colonnes...\n");
    printf("Que voulez-vous supprimer :\n"
           "1 - Une colonne en particulier par nom\n"
           "2 - Une colonne en particulier par indice\n"
           "3 - Un intervalle de colonnes\n"
           "4 - Toutes les colonnes\n"
           "5 - Retourner au menu precedent\n");
    char option;
    printf("Entrez votre choix : ");
    scanf(" %c", &option);
    getchar();  // Consume the newline character left in the input buffer

    int start, end;
    char col_name[256];

    switch (option) {
        case '1':
            printf("Entrez le nom de la colonne que vous voulez supprimer: ");
            fgets(col_name, sizeof(col_name), stdin);
            col_name[strcspn(col_name, "\n")] = 0;  // Remove the newline character at the end
            delete_column_by_name(dataframe, col_name);
            break;

        case '2':
            printf("Entrez l'indice de la colonne que vous voulez supprimer (0 a %d) : ", dataframe->num_columns - 1);
            scanf("%d", &start);
            if (start < 0 || start >= dataframe->num_columns) {
                printf("Indice invalide.\n");
            } else {
                delete_column(&(dataframe->columns[start]));
                // Shift all columns left from the deleted point
                for (int i = start; i < dataframe->num_columns - 1; i++) {
                    dataframe->columns[i] = dataframe->columns[i + 1];
                }
                dataframe->columns[dataframe->num_columns - 1] = NULL;
                dataframe->num_columns--;
                printf("Colonne a l'indice %d supprimee avec succes.\n", start);
            }
            break;

        case '3':
            printf("Entrez l'indice de debut et de fin pour la suppression (ex: 1 3 pour supprimer les colonnes 1 a 3 inclus) : ");
            scanf("%d %d", &start, &end);
            if (start < 0 || end >= dataframe->num_columns || start > end) {
                printf("Indices invalides.\n");
            } else {
                for (int i = end; i >= start; i--) {
                    delete_column(&(dataframe->columns[i]));
                }
                // Shift all columns left from the start point
                int shift_count = end - start + 1;
                for (int i = start; i < dataframe->num_columns - shift_count; i++) {
                    dataframe->columns[i] = dataframe->columns[i + shift_count];
                }
                for (int i = dataframe->num_columns - shift_count; i < dataframe->num_columns; i++) {
                    dataframe->columns[i] = NULL;
                }
                dataframe->num_columns -= shift_count;
                printf("Colonnes de %d a %d supprimees avec succes.\n", start, end);
            }
            break;

        case '4':
            for (int i = 0; i < dataframe->num_columns; i++) {
                delete_column(&(dataframe->columns[i]));
            }
            free(dataframe->columns);
            dataframe->columns = NULL;
            dataframe->num_columns = 0;
            printf("Toutes les colonnes ont ete supprimees.\n");
            break;

        case '5':
            // Simply return to the previous menu
            return;

        default:
            printf("Choix non valide. Veuillez reessayer.\n");
            break;
    }
}

void sort_columns_process(CDATAFRAME *dataframe) {
    if (dataframe == NULL || dataframe->num_columns == 0) {
        printf("Aucune colonne n'a encore ete creee.\n");
        return;
    }

    printf("Tri des colonnes...\n");
    printf("Voulez-vous trier :\n"
           "1 - Une colonne en particulier\n"
           "2 - Plusieurs colonnes\n"
           "3 - Toutes les colonnes\n"
           "4 - Retourner au menu precedent\n");
    int choice;
    printf("Entrez votre choix : ");
    scanf("%d", &choice);

    int index, start, end;
    int sort_dir = ASC;  // Default to ascending
    char direction;

    switch (choice) {
        case 1:
            printf("Entrez le numero de la colonne a trier (0 a %d) : ", dataframe->num_columns - 1);
            scanf("%d", &index);
            if (index < 0 || index >= dataframe->num_columns) {
                printf("Index invalide.\n");
                return;
            }

            printf("Choisissez la direction du tri (A pour ascendant, D pour descendant): ");
            scanf(" %c", &direction);
            sort_dir = (direction == 'A' || direction == 'a') ? ASC : DESC;

            sort(dataframe->columns[index], sort_dir);
            printf("Colonne %d triee.\n", index);
            break;

        case 2:
            printf("Entrez les indices de debut et de fin pour le tri (ex: 1 3 pour trier de la colonne 1 a 3) : ");
            scanf("%d %d", &start, &end);
            if (start < 0 || end >= dataframe->num_columns || start > end) {
                printf("Indices invalides.\n");
                return;
            }

            printf("Choisissez la direction du tri (A pour ascendant, D pour descendant): ");
            scanf(" %c", &direction);
            sort_dir = (direction == 'A' || direction == 'a') ? ASC : DESC;

            for (int i = start; i <= end; i++) {
                sort(dataframe->columns[i], sort_dir);
                printf("Colonne %d triee.\n", i);
            }
            break;

        case 3:
            printf("Choisissez la direction du tri pour toutes les colonnes (A pour ascendant, D pour descendant): ");
            scanf(" %c", &direction);
            sort_dir = (direction == 'A' || direction == 'a') ? ASC : DESC;

            for (int i = 0; i < dataframe->num_columns; i++) {
                sort(dataframe->columns[i], sort_dir);
                printf("Colonne %d triee.\n", i);
            }
            break;

        case 4:
            printf("Retour au menu precedent.\n");
            return;

        default:
            printf("Choix non valide.\n");
            break;


    }
}

void explain_cdataframe() {
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
}

