//
// Created by 20220848 on 21/04/2024.
//

#include "cdataframe.h"

#define MAX_COLUMNS 50  // Définissez une limite sur le nombre de colonnes

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

void clean_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}  // Cette boucle vide le tampon stdin.
}
void create_column_process(CDATAFRAME *dataframe) {
    if (dataframe->num_columns >= MAX_COLUMNS) {
        printf("Nombre maximum de colonnes atteint (%d colonnes).\n", MAX_COLUMNS);
        return;
    }

    printf("Creation d'une colonne...\n");
    char name_col[256];
    printf("Nommez votre colonne : ");

    clean_stdin();  // Nettoyer stdin pour éviter que fgets ne lise une ligne vide

    // Utilisez fgets pour lire la ligne entière, incluant les espaces.
    if (fgets(name_col, sizeof(name_col), stdin) == NULL) {
        printf("Erreur lors de la lecture du nom de la colonne.\n");
        return;  // ou gérez l'erreur comme il se doit
    }

    // Remplacez les newline par des caractères nuls pour éviter des problèmes dans les chaînes
    name_col[strcspn(name_col, "\n")] = 0;

    // Affiche le nom de la colonne entré par l'utilisateur
    printf("Vous avez nomme votre colonne : '%s'\n", name_col);

    ENUM_TYPE col_type;
    printf("Choisissez le type de colonne que vous voulez creer :\n"
           "1 - Colonne d'entier (INT)\n"
           "2 - Colonne de reel (FLOAT)\n"
           "3 - Colonne de caracteres (CHAR)\n"
           "4 - Colonne de chaine de caracteres (STRING)\n"
           "5 - Colonne de reel long (DOUBLE)\n"
           "6 - Colonne de types multiples (STRUCTURE)\n"
           "7 - Retourner au menu precedent\n");
    printf("Entrez votre choix : ");
    char choice;
    scanf(" %c", &choice);
    char typ[10];
    switch (choice) {
        case '1': col_type = INT; strcpy(typ, "INT"); break;
        case '2': col_type = FLOAT; strcpy(typ, "FLOAT"); break;
        case '3': col_type = CHAR; strcpy(typ, "CHAR"); break;
        case '4': col_type = STRING; strcpy(typ, "STRING"); break;
        case '5': col_type = DOUBLE; strcpy(typ, "DOUBLE"); break;
        case '6': col_type = STRUCTURE; printf("Bientot disponible.\n"); return;
        case '7': return; // Retour directement au menu de création
        default: printf("Choix invalide. Retour au menu de creation.\n"); return;
    }

    COLUMN* new_column = create_column(col_type, name_col);
    if (new_column == NULL) {
        printf("Echec de la creation de la colonne '%s'.\n", name_col);
    } else {
        dataframe->columns[dataframe->num_columns++] = new_column;  // Ajoutez la colonne au dataframe
        printf("Colonne '%s' de type '%s' creee avec succes.\n", name_col, typ);
    }
}

const char* getTypeName(ENUM_TYPE type) {
    switch(type) {
        case INT: return "INT";
        case FLOAT: return "FLOAT";
        case CHAR: return "CHAR";
        case STRING: return "STRING";
        case DOUBLE: return "DOUBLE";
        case STRUCTURE: return "STRUCTURE";
        default: return "UNKNOWN";
    }
}

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
        if ( count > 5) {  // Ask if more insertions are expected, unless it's the last one planned
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
        case INT: {
            int val;
            printf("Saisissez une valeur entiere:\n");
            scanf("%d", &val);
            getchar();  // Consume the newline character
            return insert_value(col, &val);  // Assuming insert_value returns 1 on success, 0 on failure
        }
        case FLOAT: {
            float fval;
            printf("Saisissez une valeur reelle:\n");
            scanf("%f", &fval);
            getchar();  // Consume the newline character
            return insert_value(col, &fval);
        }
        case DOUBLE: {
            double dval;
            printf("Saisissez une valeur reelle longue:\n");
            scanf("%lf", &dval);
            getchar();  // Consume the newline character
            return insert_value(col, &dval);
        }
        case CHAR: {
            char cval;
            printf("Saisissez un caractere:\n");
            scanf(" %c", &cval);  // Notice the space before %c to consume any leftover newline characters
            getchar();  // Consume the newline character
            return insert_value(col, &cval);
        }
        case STRING: {
            char sval[256];
            printf("Saisissez une chaine de caracteres:\n");
            scanf(" %255s", sval);  // Use buffer limit to prevent overflow
            getchar();  // Consume the newline character
            return insert_value(col, sval);
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

    printf("Affichage des colonnes...\n");
    printf("Que voulez-vous afficher :\n"
           "1 - Une colonne en particulier\n"
           "2 - Un nombre precis de colonnes\n"
           "3 - Toutes les colonnes\n"
           "4 - Retourner au menu precedent\n");

    char option;
    printf("Entrez votre choix : ");
    scanf(" %c", &option);

    int col_index, start, end, max_rows = 0;
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
            printf("\n");
            if (start < 0 || end >= dataframe->num_columns || start > end) {
                printf("Indices invalides.\n");
            } else {
                print_multiple_cols(dataframe, start, end);
            }
            break;

        case '3':
            printf("\n");
            print_multiple_cols(dataframe, 0, dataframe->num_columns - 1);
            break;

        case '4':
            // Retourner au menu précédent (cette option ne fait rien ici, elle sert juste à sortir de la fonction)
            return;

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
        printf("Colonne %d : %s\t", i, dataframe->columns[i]->title);
    }
    printf("\n");

    // Afficher les données des colonnes
    for (int row = 0; row < max_rows; row++) {
        for (int col = start; col <= end; col++) {
            if (row < dataframe->columns[col]->size) {
                char str[20]; // Taille suffisante pour stocker la conversion de la valeur
                convert_value(dataframe->columns[col], row, str, sizeof(str));
                printf(" [%d] : %s\t", row, str);
            } else {
                printf(" [%d] : NULL\t", row);  // Afficher NULL pour les indices hors limites
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
            printf("Entrez la position de la valeur recherchee : ");
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
