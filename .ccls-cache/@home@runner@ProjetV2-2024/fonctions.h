#ifndef FONCTIONS_H
#define FONCTIONS_H
#include "structures.h"

void ajouter_donnees_entrainement(Sportif *sportif, Entrainement entrainement);
void afficher_historique_entrainement(Sportif *sportif, char *epreuve, char *date_debut, char *date_fin);
void afficher_statistiques_performance(Sportif *sportif);
void choisir_athletes_pour_JO(char *epreuve);
void charger_donnees_entrainement(Sportif *sportif);

#endif

