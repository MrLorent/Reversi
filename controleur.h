#ifndef CONTROLEUR_H
#define CONTROLEUR_H

// JEU
bool analyseCoupsJouables(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], Joueur * joueurCourant, Joueur * adversaire, ListeCoupsJouables * coupsJouables);
bool directionJouable(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], Jeton * position, int uneDirection, int * count, char couleurAdversaire);
void enregistreCoupJouable(ListeCoupsJouables * liste, Marqueur * emplacement, int nbCaptures);
int estEnregistre(ListeCoupsJouables * coupsJouables, int coorEmplacement[2]);
void recupereCoupJouable(ListeCoupsJouables uneListe, int rang, CoupJouable * leCoupCherche);
bool coupJouable(ListeCoupsJouables * coupsJouables, int caseSouhaitee[2]);
void joueLeCoup(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], int coorJetonPlace[2], Joueur * joueurCourant, Joueur * adversaire);
bool directionCapture(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], ListeJetons* jetonsCaptures, int position[2], int uneDirection, char couleurAdversaire, char objectif);
void videListeCoupsJouables(ListeCoupsJouables * uneListe);
void changeJoueurCourant(Jeu * unJeu);

// PLATEAU
void ajouteJetonPlateau(Joueur * unJoueur, Jeton * lePlateau[MAXLARGEUR][MAXLARGEUR]);
void ajouteCoupsJouablesPlateau(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], ListeCoupsJouables * coupsJouables);
void retireCoupsJouablesPlateau(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], ListeCoupsJouables * coupsJouables);
bool caseExiste(int x, int y);

// JETON
void ajouteJetonCapture(ListeJetons * liste, int coorJetonCapture[2]);
void videListeJetons(ListeJetons * uneListe);

// JOUEUR
void ajouteJetonJoueur(Joueur * unJoueur, int coordonnees[2]);
void supprimeJetonJoueur(Joueur * unJoueur, int coordonnees[2]);
int ordinateurJoue(Jeu * leJeu);

// MAIN
bool saisieMenuCorrecte(int saisieUt);
bool saisieCorrecte(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], char saisieUt[2], int coorCase[2]);
void convertCoordonnees(char saisieUt[2], int saisieTraduite[2]);
bool quitterPartie(Jeu * leJeu);

// SAUVEGARDE
void sauvegardePartie(Jeu leJeu);
void chargementPartie(Jeu *leJeu);

#endif