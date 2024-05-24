#include "structures.h"
#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void ajouter_donnees_entrainement(Sportif *sportif, Entrainement entrainement) {
    FILE *fichier;
    char nom_fichier[LONGUEUR_NOM_FICHIER];

    // Construire le nom du fichier à partir du prénom et du nom de l'athlète
    snprintf(nom_fichier, sizeof(nom_fichier), "%s%s.txt", sportif->prenom, sportif->nom);

    // Ouvrir le fichier en mode lecture pour vérifier les doublons
    fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        // Afficher un message d'erreur si le fichier ne peut pas être ouvert
        printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
        return;
    }

    // Vérifier si un entraînement de relais pour le même jour existe déjà
    char ligne[LONGUEUR_MAX_LIGNE];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        Entrainement existant;
        sscanf(ligne, "%10s %9s", existant.date, existant.epreuve);
        if (strcmp(existant.date, entrainement.date) == 0 && strcmp(existant.epreuve, "relais") == 0) {
            printf("L'entrainement n'a pas été ajouté, un entraînement de relais pour le même jour existe déjà. \n");
            fclose(fichier);
            return;
        }
    }
    fclose(fichier);

    // Ouvrir le fichier en mode ajout (append)
    fichier = fopen(nom_fichier, "a");
    if (fichier == NULL) {
        // Afficher un message d'erreur si le fichier ne peut pas être ouvert
        printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
        return;
    }

    // Ajouter les données d'entraînement au fichier
    if (strcmp(entrainement.epreuve, "relais") == 0) {
        // Si l'épreuve est un relais, inclure la position dans le relais
        fprintf(fichier, "%s %s %d %d %d %d %d\n", entrainement.date, entrainement.epreuve, 
                entrainement.temps_h, entrainement.temps_m, entrainement.temps_s, 
                entrainement.temps_ms, entrainement.position_relais);
    } else {
        // Sinon, ne pas inclure la position
        fprintf(fichier, "%s %s %d %d %d %d %d\n", entrainement.date, entrainement.epreuve, 
                entrainement.temps_h, entrainement.temps_m, entrainement.temps_s, 
                entrainement.temps_ms, 0);
    }

    // Fermer le fichier
    fclose(fichier);
}


void charger_donnees_entrainement(Sportif *sportif) {
    char nom_fichier[LONGUEUR_NOM_FICHIER];
    snprintf(nom_fichier, sizeof(nom_fichier), "%s%s.txt", sportif->prenom, sportif->nom);

    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s pour lecture.\n", nom_fichier);
        perror("fopen");
        return;
    }

    // Initialiser le nombre d'entraînements à 0
    sportif->nb_entrainements = 0; // Modification : Initialise le nombre d'entraînements à 0

    char ligne[LONGUEUR_MAX_LIGNE];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        // Vérifier si le sportif a déjà atteint la limite de 100 entraînements
        if (sportif->nb_entrainements >= MAX_ENTREES) {
            printf("Le sportif a atteint la limite de 100 entraînements.\n");
            break;
        }

        Entrainement entrainement;
        sscanf(ligne, "%10s %9s %d %d %d %d %d", entrainement.date, entrainement.epreuve, &entrainement.temps_h, &entrainement.temps_m, &entrainement.temps_s, &entrainement.temps_ms, &entrainement.position_relais);

        // Ajouter l'entraînement à la liste des entraînements du sportif
        sportif->entrainement[sportif->nb_entrainements++] = entrainement; // Modification : Stocke l'entraînement dans la structure sportif
    }
    fclose(fichier);
}



void afficher_historique_entrainement(Sportif *sportif, char *epreuve, char *date_debut, char *date_fin) {
    charger_donnees_entrainement(sportif);

    printf("Historique d'entraînement pour %s %s:\n\n", sportif->prenom, sportif->nom);

    for (int i = 0; i < sportif->nb_entrainements; i++) {
        Entrainement *entrainement = &sportif->entrainement[i];

        if ((epreuve == NULL || strcmp(epreuve, entrainement->epreuve) == 0) && 
            (date_debut == NULL || strcmp(entrainement->date, date_debut) >= 0) && 
            (date_fin == NULL || strcmp(entrainement->date, date_fin) <= 0)) {

            printf("%s - %s : ", entrainement->date, entrainement->epreuve);

            // Afficher le temps en fonction des valeurs présentes
            if (entrainement->temps_h > 0) {
                printf("%dh ", entrainement->temps_h);
            }
            if (entrainement->temps_h > 0 || entrainement->temps_m > 0) {
                printf("%dm ", entrainement->temps_m);
            }
            if (entrainement->temps_h > 0 || entrainement->temps_m > 0 || entrainement->temps_s > 0) {
                printf("%ds ", entrainement->temps_s);
            }
            printf("%d", entrainement->temps_ms);

            if (entrainement->temps_h == 0 && entrainement->temps_m == 0 && entrainement->temps_s == 0 && entrainement->temps_ms == 0) {
                printf("0s 0");
            }

            if (entrainement->position_relais != 0) {
                printf(" (position %d)", entrainement->position_relais);
            }
            printf("\n\n"); // Ajout d'un saut de ligne après chaque entraînement
        }
    }
}


void afficher_statistiques_performance(Sportif *sportif) {
    charger_donnees_entrainement(sportif);

    Statistiques stats[LONGUEUR_MAX_EPREUVE] = {0};
    int nombre_epreuves = 0;

    for (int i = 0; i < sportif->nb_entrainements; i++) {
        Entrainement *entrainement = &sportif->entrainement[i];
        double temps_total = entrainement->temps_h * 3600 + entrainement->temps_m * 60 + entrainement->temps_s + entrainement->temps_ms / 100.0;

        int j;
        for (j = 0; j < nombre_epreuves; j++) {
            if (strcmp(stats[j].epreuve, entrainement->epreuve) == 0) {
                break;
            }
        }

        if (j == nombre_epreuves) {
            strcpy(stats[nombre_epreuves].epreuve, entrainement->epreuve);
            stats[nombre_epreuves].meilleur_temps = temps_total;
            stats[nombre_epreuves].pire_temps = temps_total;
            stats[nombre_epreuves].total_temps = temps_total;
            stats[nombre_epreuves].nombre_courses = 1;
            nombre_epreuves++;
        } else {
            if (temps_total < stats[j].meilleur_temps) {
                stats[j].meilleur_temps = temps_total;
            }
            if (temps_total > stats[j].pire_temps) {
                stats[j].pire_temps = temps_total;
            }
            stats[j].total_temps += temps_total;
            stats[j].nombre_courses++;
        }
    }

    printf("Statistiques de performance pour %s %s:\n", sportif->prenom, sportif->nom);
    for (int i = 0; i < nombre_epreuves; i++) {
        printf("\n%s:\n", stats[i].epreuve);
        printf("\tMeilleur temps: %.2f\n", stats[i].meilleur_temps);
        printf("\tPire temps: %.2f\n", stats[i].pire_temps);
        printf("\tTemps moyen: %.2f\n", stats[i].total_temps / stats[i].nombre_courses);
    }
}

#include <dirent.h>

// Structure definitions and constants would go here (omitted for brevity)

void choisir_athletes_pour_JO(char *epreuve) {
    Sportif athletes[MAX_ATHLETES]; // Utilisation de la structure Sportif pour stocker les athlètes
    int nombre_athletes = 0;

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt") != NULL) {
                FILE *fichier = fopen(ent->d_name, "r");
                if (fichier == NULL) {
                    printf("Erreur lors de l'ouverture du fichier %s pour lecture.\n", ent->d_name);
                    perror("fopen");
                    continue;
                }

                double total_temps = 0;
                int nombre_courses = 0;

                char nom_athlete[LONGUEUR_MAX_NOM];
                strncpy(nom_athlete, ent->d_name, strlen(ent->d_name) - 4);
                nom_athlete[strlen(ent->d_name) - 4] = '\0';

                char ligne[LONGUEUR_MAX_LIGNE];
                while (fgets(ligne, sizeof(ligne), fichier)) {
                    char date[LONGUEUR_MAX_DATE], event[LONGUEUR_MAX_EPREUVE];
                    int temps_h, temps_m, temps_s, temps_ms, position;
                    sscanf(ligne, "%10s %9s %d %d %d %d %d", date, event, &temps_h, &temps_m, &temps_s, &temps_ms, &position);

                    if (strcmp(event, epreuve) == 0) {
                        total_temps += temps_h * 3600 + temps_m * 60 + temps_s + temps_ms / 100.0;
                        nombre_courses++;
                    }
                }
                fclose(fichier);

                if (nombre_courses > 0) {
                    // Ajoute les données de l'athlète à la structure Sportif
                    strcpy(athletes[nombre_athletes].prenom, ""); // Remplir avec les informations de l'athlète si nécessaire
                    strcpy(athletes[nombre_athletes].nom, nom_athlete);
                    athletes[nombre_athletes].temps_moyen = total_temps / nombre_courses;
                    nombre_athletes++;
                }
            }
        }
        closedir(dir);
    } else {
        printf("Erreur lors de l'ouverture du répertoire courant.\n");
        perror("opendir");
    }

    for (int i = 0; i < nombre_athletes - 1; i++) {
        for (int j = 0; j < nombre_athletes - i - 1; j++) {
            if (athletes[j].temps_moyen > athletes[j + 1].temps_moyen) {
                Sportif temp = athletes[j];
                athletes[j] = athletes[j + 1];
                athletes[j + 1] = temp;
            }
        }
    }

    printf("\nLes trois meilleurs athlètes pour l'épreuve %s sont:\n", epreuve);
    for (int i = 0; i < 3 && i < nombre_athletes; i++) {
        printf("\n%s %s avec un temps moyen de %.2f\n", athletes[i].prenom, athletes[i].nom, athletes[i].temps_moyen);

    }
}

