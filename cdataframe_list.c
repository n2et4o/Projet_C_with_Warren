//
// Created by 20220848 on 21/04/2024.
//

#include "cdataframe_list.h"

CDATAFRAME *create_empty_cdataframe() {
    CDATAFRAME *cdf = (CDATAFRAME *)malloc(sizeof(CDATAFRAME));
    if (cdf == NULL) {
        fprintf(stderr, "Erreur: Echec de l'allocation pour le dataframe.\n");
        return NULL;
    }

    cdf->head = NULL;      // Commence avec une liste de colonnes vide
    cdf->num_columns = 0;  // Commence avec zéro colonne

    return cdf;
}


CDATAFRAME *create_cdataframe(ENUM_TYPE *cdftype, int size) {
    CDATAFRAME *cdf = (CDATAFRAME *)malloc(sizeof(CDATAFRAME));
    if (!cdf) {
        fprintf(stderr, "Erreur: Echec de l'allocation pour le dataframe.\n");
        return NULL;
    }

    cdf->head = NULL;      // Commence avec une liste de colonnes vide
    cdf->num_columns = 0;  // Commence avec zéro colonne

    ColumnList **current = &(cdf->head);  // Pointeur pour suivre la fin de la liste

    char col_title[MAX_TITLE_LENGTH];

    for (int i = 0; i < size; i++) {
        snprintf(col_title, MAX_TITLE_LENGTH, "Column %d", i + 1);  // Construction sécurisée du titre

        // Crée un nouveau noeud de colonne
        *current = (ColumnList *)malloc(sizeof(ColumnList));
        if (!*current) {
            fprintf(stderr, "Erreur: Echec de l'allocation pour une colonne.\n");
            // Nettoyage de toutes les colonnes précédemment allouées
            while (cdf->head) {
                ColumnList *temp = cdf->head;
                cdf->head = temp->succ;
                delete_column(&(temp->column));
                free(temp);
            }
            free(cdf);
            return NULL;
        }

        // Crée la colonne et l'attache au noeud
        (*current)->column = create_column(cdftype[i], col_title);
        if (!(*current)->column) {
            fprintf(stderr, "Erreur: Echec de la creation de la colonne %d.\n", i + 1);
            free(*current);
            while (cdf->head) {
                ColumnList *temp = cdf->head;
                cdf->head = temp->succ;
                delete_column(&(temp->column));
                free(temp);
            }
            free(cdf);
            return NULL;
        }
        (*current)->succ = NULL;  // Prépare le prochain noeud
        current = &((*current)->succ);  // Avance le pointeur

        cdf->num_columns++;  // Incrémente le compteur de colonnes
    }

    return cdf;
}


void delete_cdataframe(CDATAFRAME **cdf) {
    if (cdf == NULL || *cdf == NULL) {
        printf("Le dataframe est deja libere ou n'existe pas.\n");
        return;
    }

    // Parcourir la liste chaînée des colonnes et libérer chaque colonne en utilisant la fonction delete_column
    ColumnList *current = (*cdf)->head;   // Pointeur pour parcourir la liste
    while (current != NULL) {
        ColumnList *next = current->succ; // Sauvegarder le pointeur vers le prochain noeud
        if (current->column != NULL) {
            delete_column(&(current->column)); // Libérer la colonne
        }
        free(current); // Libérer le noeud lui-même
        current = next; // Passer au prochain noeud
    }

    // Libérer la structure du dataframe
    free(*cdf);

    // Mettre le pointeur à NULL pour éviter des références à de la mémoire libérée
    *cdf = NULL;
    printf("Dataframe supprime avec succes.\n");
}

/*
void delete_column_by_name(CDATAFRAME *cdf, char *col_name) {
    if (cdf == NULL || cdf->head == NULL) {
        printf("Le dataframe est vide ou n'existe pas.\n");
        return;
    }

    ColumnList *current = cdf->head;
    ColumnList *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->column_list->title, col_name) == 0) {
            if (prev == NULL) { // La colonne à supprimer est la tête de la liste
                cdf->head = current->succ;
            } else {
                prev->succ = current->succ; // Débrancher le nœud de la liste
            }

            // Libérer la colonne
            if (current->column_list->title) free(current->column_list->title);
            if (current->column_list->data) {
                for (unsigned int i = 0; i < current->column_list->size; i++) {
                    if (current->column_list->data[i] != NULL) {
                        free(current->column_list->data[i]);
                    }
                }
                free(current->column_list->data);
            }
            if (current->column_list->index) free(current->column_list->index);
            free(current->column_list);

            // Libérer le nœud de la liste
            free(current);
            printf("Colonne '%s' supprimée avec succès.\n", col_name);
            return;
        }
        prev = current;
        current = current->succ;
    }

    printf("Colonne '%s' non trouvée.\n", col_name);
}
*/

void clean_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}  // Cette boucle vide le tampon stdin.
}

void create_column_process(CDATAFRAME *dataframe) {
    if (dataframe == NULL) {
        printf("Dataframe non initialise.\n");
        return;
    }

    int num_columns_to_add = 0;
    printf("Combien de colonnes souhaitez-vous creer : ");
    scanf("%d", &num_columns_to_add);
    clean_stdin();  // Nettoyer le buffer après la lecture d'un entier

    if (num_columns_to_add < 1) {
        printf("Nombre de colonnes non valide. Retour au menu precedent.\n");
        return;
    }

    int nb;
    for (int i = 0; i < num_columns_to_add; ++i) {
        printf("\nCreation de la colonne %d...\n", i + 1);
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
            // Ajout de la colonne à la liste chaînée
            ColumnList *new_node = (ColumnList *)malloc(sizeof(ColumnList));
            if (new_node == NULL) {
                printf("Echec d'allocation pour le nouveau noeud de la colonne.\n");
                delete_column(&new_column);
                continue;
            }
            new_node->column = new_column;
            new_node->succ = NULL;
            if (dataframe->head == NULL) {
                dataframe->head = new_node;  // Première colonne ajoutée
            } else {
                ColumnList *last = dataframe->head;
                while (last->succ != NULL) {
                    last = last->succ;
                }
                last->succ = new_node;  // Ajouter à la fin de la liste
            }
            dataframe->num_columns++;
            printf("Colonne '%s' creee avec succes et ajoutee au dataframe.\n", name_col);
        }
        if (i + 1 > 5 && i + 1 < num_columns_to_add) {  // Ask if more insertions are expected, unless it's the last one planned
            printf("Voulez-vous continuer ? (Oui: o / Non: n): ");
            char decision;
            scanf(" %c", &decision);
            getchar();  // Consume the newline character
            if (decision == 'n' || decision == 'N') {
                break;  // Break the loop if user decides not to continue
            }
        }
        nb = i + 1;
    }
    if (nb > 1){
        printf("Vous avez cree %d colonnes\n",nb);
    }
    else{
        printf("Vous avez cree %d colonne\n",nb);
    }
}


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

void insert_value_process(CDATAFRAME *dataframe) {
    if (dataframe == NULL || dataframe->head == NULL) {
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

    ColumnList *node = dataframe->head;
    for (int i = 0; i < col_index && node != NULL; i++) {
        node = node->succ;
    }

    if (node == NULL || node->column == NULL) {
        printf("La colonne %d n'existe pas.\n", col_index);
        return;
    }

    COLUMN *col = node->column;

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
            return insert_value(col, &val);  // Assuming insert_value returns 1 on success, 0 on failure
        }
        case MY_FLOAT: {
            float fval;
            printf("Saisissez une valeur reelle:\n");
            scanf("%f", &fval);
            getchar();  // Consume the newline character
            return insert_value(col, &fval);
        }
        case MY_DOUBLE: {
            double dval;
            printf("Saisissez une valeur reelle longue:\n");
            scanf("%lf", &dval);
            getchar();  // Consume the newline character
            return insert_value(col, &dval);
        }
        case MY_CHAR: {
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
    if (dataframe == NULL || dataframe->head == NULL) {
        printf("Aucune colonne n'a encore été créée.\n");
        return;
    }

    printf("Affichage des colonnes...\n");
    printf("Que voulez-vous afficher :\n"
           "1 - Une colonne en particulier\n"
           "2 - Un nombre précis de colonnes\n"
           "3 - Toutes les colonnes\n"
           "4 - Retourner au menu précédent\n");

    int option;
    printf("Entrez votre choix (1-4) : ");
    if (scanf("%d", &option) != 1) {
        printf("Entrée invalide.\n");
        return;
    }
    getchar(); // Consommer le retour chariot restant

    int col_index, start, end;
    COLUMN *current;

    switch (option) {
        case 1:
            printf("Entrez le numéro de la colonne que vous voulez afficher : ");
            if (scanf("%d", &col_index) != 1) {
                printf("Entrée invalide.\n");
                return;
            }
            getchar(); // Consommer le retour chariot restant
            current = dataframe->head;
            for (int i = 1; i < col_index && current != NULL; i++) {
                current = current->next;
            }
            if (current == NULL) {
                printf("Index invalide.\n");
            } else {
                printf("\nColonne %d : %s\n", col_index, current->title);
                print_col(current);
            }
            break;

        case 2:
            printf("Entrez l'indice de début et de fin pour l'affichage des colonnes (ex: 1 3 pour afficher les colonnes 1 à 3) : ");
            if (scanf("%d %d", &start, &end) != 2) {
                printf("Entrée invalide.\n");
                return;
            }
            getchar(); // Consommer le retour chariot restant
            if (start < 1 || end < start || end > dataframe->num_columns) {
                printf("Indices invalides.\n");
                break;
            }
            current = dataframe->head;
            for (int i = 1; i < start && current != NULL; i++) {
                current = current->next;
            }
            for (int i = start; current != NULL && i <= end; i++) {
                printf("\nColonne %d : %s\n", i, current->title);
                print_col(current);
                current = current->next;
            }
            break;

        case 3:
            current = dataframe->head;
            int i = 1;
            while (current != NULL) {
                printf("\nColonne %d : %s\n", i++, current->title);
                print_col(current);
                current = current->next;
            }
            break;

        case 4:
            return;

        default:
            printf("Choix non valide.\n");
            break;
    }
}





void search_columns_process(CDATAFRAME *dataframe) {
    if (dataframe == NULL || dataframe->head == NULL) {
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

    // Parcourir la liste chaînée pour trouver la colonne
    COLUMN *col = dataframe->head;
    for (int i = 0; i < col_index && col != NULL; i++) {
        col = col->next;
    }

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

    int start, end;
    int sort_dir = ASC;  // Default to ascending
    char direction;

    switch (choice) {
        case 1: {
            printf("Entrez le numero de la colonne a trier (0 a %d) : ", dataframe->num_columns - 1);
            scanf("%d", &start);
            sort_single_column(dataframe, start);
            break;
        }

        case 2: {
            printf("Entrez les indices de debut et de fin pour le tri (ex: 1 3 pour trier de la colonne 1 a 3) : ");
            scanf("%d %d", &start, &end);
            sort_multiple_columns(dataframe, start, end);
            break;
        }

        case 3: {
            printf("Choisissez la direction du tri pour toutes les colonnes (A pour ascendant, D pour descendant): ");
            scanf(" %c", &direction);
            sort_dir = (direction == 'A' || direction == 'a') ? ASC : DESC;

            for (int i = 0; i < dataframe->num_columns; i++) {
                sort_single_column(dataframe, i);
            }
            break;
        }

        case 4:
            printf("Retour au menu precedent.\n");
            return;

        default:
            printf("Choix non valide.\n");
            break;
    }
}

// Fonction pour trier une seule colonne par son index dans le CDATAFRAME
void sort_single_column(CDATAFRAME *dataframe, int index) {
    if (index < 0 || index >= dataframe->num_columns) {
        printf("Index invalide.\n");
        return;
    }

    COLUMN *col = dataframe->head;
    for (int i = 0; i < index && col != NULL; i++) {
        col = col->next;
    }

    if (col == NULL) {
        printf("Colonne a l'index %d non trouvee.\n", index);
        return;
    }

    printf("Choisissez la direction du tri (A pour ascendant, D pour descendant): ");
    char direction;
    scanf(" %c", &direction);
    int sort_dir = (direction == 'A' || direction == 'a') ? 0 : 1; // 0: ASC, 1: DESC

    sort(col, sort_dir);
    printf("Colonne %d triee.\n", index);
}


// Fonction pour trier plusieurs colonnes dans l'intervalle spécifié
void sort_multiple_columns(CDATAFRAME *dataframe, int start, int end) {
    if (start < 0 || end >= dataframe->num_columns || start > end) {
        printf("Indices invalides.\n");
        return;
    }

    printf("Choisissez la direction du tri (A pour ascendant, D pour descendant): ");
    char direction;
    scanf(" %c", &direction);
    int sort_dir = (direction == 'A' || direction == 'a') ? 0 : 1; // 0: ASC, 1: DESC

    COLUMN *col = dataframe->head;
    for (int i = 0; i < start && col != NULL; i++) {
        col = col->next;
    }

    for (int i = start; i <= end && col != NULL; i++) {
        sort(col, sort_dir);
        printf("Colonne %d triee.\n", i);
        col = col->next;
    }
}


void delete_column_by_name(CDATAFRAME *dataframe, char *name) {
    COLUMN *current = dataframe->head;
    COLUMN *previous = NULL;
    while (current != NULL) {
        if (strcmp(current->title, name) == 0) {
            if (previous == NULL) { // Suppression de la tête
                dataframe->head = current->next;
            } else { // Suppression dans le milieu ou la fin
                previous->next = current->next;
            }
            free(current); // Supposer que cela gère aussi la libération de `current->data`
            dataframe->num_columns--;
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Colonne non trouvee.\n");
}

void delete_column_by_index(CDATAFRAME *dataframe, int index) {
    if (index < 0 || index >= dataframe->num_columns) {
        printf("Indice invalide.\n");
        return;
    }
    COLUMN *current = dataframe->head;
    COLUMN *previous = NULL;
    for (int i = 0; i < index; ++i) {
        previous = current;
        current = current->next;
    }
    if (previous == NULL) { // Suppression de la tête
        dataframe->head = current->next;
    } else { // Suppression dans le milieu ou la fin
        previous->next = current->next;
    }
    free(current); // Supposer que cela gère aussi la libération de `current->data`
    dataframe->num_columns--;
    printf("Colonne a l'indice %d supprimee avec succes.\n", index);
}

void delete_all_columns(CDATAFRAME *dataframe) {
    COLUMN *current = dataframe->head;
    while (current != NULL) {
        COLUMN *next = current->next;
        free(current); // Supposer que cela gère aussi la libération de `current->data`
        current = next;
    }
    dataframe->head = NULL;
    dataframe->num_columns = 0;
    printf("Toutes les colonnes ont ete supprimees.\n");
}

void delete_columns_range(CDATAFRAME *dataframe, int start, int end) {
    if (start < 0 || end >= dataframe->num_columns || start > end) {
        printf("Indices invalides.\n");
        return;
    }

    COLUMN *current = dataframe->head;
    COLUMN *previous = NULL;
    COLUMN *next;
    int index = 0;

    // Avancer jusqu'à l'indice de début
    while (index < start && current != NULL) {
        previous = current;
        current = current->next;
        index++;
    }

    // Commencer la suppression jusqu'à l'indice de fin
    while (index <= end && current != NULL) {
        next = current->next;
        free(current); // Supposer que cela gère aussi la libération de `current->data`
        current = next;
        index++;
        dataframe->num_columns--;
    }

    // Relier le dernier nœud non supprimé avec le premier nœud après l'intervalle supprimé
    if (previous == NULL) {
        // L'intervalle inclut la première colonne
        dataframe->head = next;
    } else {
        previous->next = next;
    }

    printf("Colonnes de %d a %d supprimees avec succes.\n", start, end);
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
    getchar(); // Consomme le caractère de nouvelle ligne laissé dans le tampon d'entrée

    char col_name[256];
    int start, end;

    switch (option) {
        case '1':
            printf("Entrez le nom de la colonne que vous voulez supprimer: ");
            fgets(col_name, sizeof(col_name), stdin);
            col_name[strcspn(col_name, "\n")] = 0; // Remove the newline character at the end
            delete_column_by_name(dataframe, col_name);
            break;

        case '2':
            printf("Entrez l'indice de la colonne que vous voulez supprimer (0 a %d) : ", dataframe->num_columns - 1);
            scanf("%d", &start);
            delete_column_by_index(dataframe, start);
            break;

        case '3':
            printf("Entrez l'indice de debut et de fin pour la suppression (ex: 1 3 pour supprimer les colonnes 1 a 3 inclus) : ");
            scanf("%d %d", &start, &end);
            delete_columns_range(dataframe, start, end);
            break;

        case '4':
            delete_all_columns(dataframe);
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
