#include "structures.h"
#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    while (1) {
        printf("\nMenu Principal:\n");
        printf("1. Ajouter de nouvelles données d'entraînement\n");
        printf("2. Afficher l'historique d'entraînement\n");
        printf("3. Afficher les statistiques de performance\n");
        printf("4. Choisir les athlètes pour les JO\n");
        printf("5. Quitter\n");

        int choix;
        printf("Entrez votre choix: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                char prenom[LONGUEUR_MAX_NOM], 
                nom[LONGUEUR_MAX_NOM], 
                date[LONGUEUR_MAX_DATE], 
                epreuve[LONGUEUR_MAX_EPREUVE];
                int temps_h, temps_m, temps_s, temps_ms, position;

                printf("Entrez le prénom de l'athlète: ");
                scanf("%s", prenom);
                printf("Entrez le nom de l'athlète: ");
                scanf("%s", nom);
                printf("Entrez la date (JJ/MM/AAAA): ");
                scanf("%s", date);
                printf("Entrez l'épreuve: ");
                scanf("%s", epreuve);
                printf("Entrez le temps (h m s ms): ");
                scanf("%d %d %d %d", &temps_h, &temps_m, &temps_s, &temps_ms);

                Sportif sportif;
                strcpy(sportif.prenom, prenom);
                strcpy(sportif.nom, nom);
                sportif.nb_entrainements = 0;

                if (strcmp(epreuve, "relais") == 0) {
                    printf("Entrez la position dans le relais (1-4): ");
                    scanf("%d", &position);
                } else {
                    position = 0;
                }

                Entrainement entrainement;
                strcpy(entrainement.date, date);
                strcpy(entrainement.epreuve, epreuve);
                entrainement.temps_h = temps_h;
                entrainement.temps_m = temps_m;
                entrainement.temps_s = temps_s;
                entrainement.temps_ms = temps_ms;
                entrainement.position_relais = position;

                ajouter_donnees_entrainement(&sportif, entrainement);
                break;
            }
            case 2: {
                char prenom[LONGUEUR_MAX_NOM], nom[LONGUEUR_MAX_NOM], epreuve[LONGUEUR_MAX_EPREUVE], date_debut[LONGUEUR_MAX_DATE], date_fin[LONGUEUR_MAX_DATE];

                printf("Entrez le prénom de l'athlète: ");
                scanf("%s", prenom);
                printf("Entrez le nom de l'athlète: ");
                scanf("%s", nom);
                printf("Entrez l'épreuve (facultatif, appuyez sur Entrée pour ignorer) : ");
                getchar(); // to consume the newline left by previous scanf
                fgets(epreuve, sizeof(epreuve), stdin);
                epreuve[strcspn(epreuve, "\n")] = '\0';
                printf("Entrez la date de début (facultatif, appuyez sur Entrée pour ignorer) : ");
                fgets(date_debut, sizeof(date_debut), stdin);
                date_debut[strcspn(date_debut, "\n")] = '\0';
                printf("Entrez la date de fin (facultatif, appuyez sur Entrée pour ignorer) : ");
                fgets(date_fin, sizeof(date_fin), stdin);
                date_fin[strcspn(date_fin, "\n")] = '\0';

                Sportif sportif;
                strcpy(sportif.prenom, prenom);
                strcpy(sportif.nom, nom);
                sportif.nb_entrainements = 0;

                // Charge les données d'entraînement de l'athlète à partir du fichier dans la structure sportif
                charger_donnees_entrainement(&sportif);

                afficher_historique_entrainement(&sportif, epreuve[0] != '\0' ? epreuve : NULL, date_debut[0] != '\0' ? date_debut : NULL, date_fin[0] != '\0' ? date_fin : NULL);
                break;
            }
            case 3: {
                char prenom[LONGUEUR_MAX_NOM], nom[LONGUEUR_MAX_NOM];

                printf("Entrez le prénom de l'athlète: ");
                scanf("%s", prenom);
                printf("Entrez le nom de l'athlète: ");
                scanf("%s", nom);

                Sportif sportif;
                strcpy(sportif.prenom, prenom);
                strcpy(sportif.nom, nom);
                sportif.nb_entrainements = 0;

                afficher_statistiques_performance(&sportif);
                break;
            }
            case 4: {
                char epreuve[LONGUEUR_MAX_EPREUVE];
                printf("Entrez l'épreuve: ");
                scanf("%s", epreuve);

                choisir_athletes_pour_JO(epreuve);
                break;
            }
            case 5: {
                printf("Au revoir !\n");
                return 0;
            }
            default: {
                printf("Choix invalide. Veuillez entrer un nombre entre 1 et 5.\n");
                break;
            }
        }
    }
    return 0;
}
