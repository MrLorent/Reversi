#include <iostream>
#include <cstring>
#include "modele.h"
#include "vue.h"
#include "controleur.h"

using namespace std;

// MODÈLE
void initPlateau(Jeton * plateau[MAXLARGEUR][MAXLARGEUR]){
    // Chaque case est remplie d'un marqueur indiquant que la case est "vide"
    for(int l=0;l<MAXLARGEUR;l++){
        for(int c=0;c<MAXLARGEUR;c++){
            plateau[l][c]= new Marqueur;
            int coor[2] = {c , l};
            initMarqueur(plateau[l][c], coor);
        }
    }
}

// VUES
void affichePlateau(Jeton * plateau[MAXLARGEUR][MAXLARGEUR]){
    const char tabLettres[MAXLARGEUR] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
    const char tabChiffres[MAXLARGEUR] = { '1', '2', '3', '4', '5', '6', '7', '8' };

    // Espacement
    cout << endl;
    // Affichage de la première ligne de lettres
    cout << "  ";
    for(int i=0; i<MAXLARGEUR;i++){
        if(i == MAXLARGEUR-1){
            cout << MAGENTA << "   " << tabLettres[MAXLARGEUR-1] << "  " << ANNULE_COULEUR << endl;
        }else
        {
            cout << MAGENTA << "   " << tabLettres[i] << ANNULE_COULEUR;
        }
    }
    afficheLigneTransition();

    // Affichage du plateau
    for(int l=0; l < MAXLARGEUR; l++){
        // On affiche le numéro de ligne
        cout << MAGENTA << " " << tabChiffres[l] << ANNULE_COULEUR << " |";

        // On affiche une ligne du plateau
        for(int c=0; c < MAXLARGEUR; c++){
            switch (plateau[l][c]->couleur)
            {
            case 'b':
                cout << " X " << "|";
                break;
            case 'n':
                cout << " O " << "|";
                break;
            case 'j':
                cout << JAUNE << " * " << ANNULE_COULEUR << "|";
                break;
            default:
                cout << "   |";
                break;
            }
        }
        cout << endl;
        afficheLigneTransition();
    }
    // Espacement
    cout << endl;
}

void afficheLigneTransition(){
    // Lignes horizontales du plateau
    cout << "   ";
    for(int l=0; l < MAXLARGEUR-1; l++){
        cout << "+---";
    }
    cout << "+---+" << endl;
}

// CONTROLEURS
void ajouteJetonPlateau(Joueur * unJoueur, Jeton * lePlateau[MAXLARGEUR][MAXLARGEUR]){
    Jeton * tmp = unJoueur->listeJetons;
    
    while(tmp != NULL){
        // ATTENTION: Ici pour des questions d'affichage du plateau, les ordonnées sont données en PREMIER, et les abscisses en SECOND!
        lePlateau[tmp->coordonnees[1]][tmp->coordonnees[0]] = tmp;
        tmp = tmp->suivant;
    }
}

void ajouteCoupsJouablesPlateau(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], ListeCoupsJouables * coupsJouables){
    CoupJouable * tmp = *coupsJouables;

    // Ici on parcours la liste de coups jouables, et on change la couleur des marqueurs situé aux emplacements jouable en 'j' pour 'jouable'
    while(tmp != NULL){
        tmp->emplacement->couleur = 'j';
        tmp = tmp->suivant;
    }
}

void retireCoupsJouablesPlateau(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], ListeCoupsJouables * coupsJouables){
    CoupJouable * tmp = *coupsJouables;

    // On retire les coups jouables qui n'ont pas été joués
    while(tmp != NULL){
        if(tmp->emplacement->couleur == 'j'){
            tmp->emplacement->couleur = 'v';
        }
        tmp = tmp->suivant;
    }

    // On vide la liste pour le prochain tour
    videListeCoupsJouables(coupsJouables);
}

bool caseExiste(int x, int y){
    // On verifie que les coordonnées passés en paramètres existe bien dans le plateau
    return (x >= 0 && x <= 7) && (y >= 0 && y <= 7);
}