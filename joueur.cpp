#include <iostream>
#include <cstring>
#include "modele.h"
#include "vue.h"
#include "controleur.h"

using namespace std;

void initJoueur(Joueur * unJoueur, char uneCouleur){
	cout << "Veuillez entrer le nom du joueur:" << endl;
	cin >> unJoueur->nom;
	unJoueur->couleur = uneCouleur;
	unJoueur->nbJeton = 0;
	unJoueur->listeJetons = NULL;
}

// void ajouteJetonJoueur(Joueur * unJoueur, Jeton unJeton){
// 	unJeton.couleur = unJoueur->couleur;
// 	unJoueur->listeJetons[unJoueur->nbJeton] = unJeton;
// 	(unJoueur->nbJeton)++;
// }

void ajouteJetonJoueur(Joueur * unJoueur, int coordonnees[2]){
	Jeton * nouveauJeton = new Jeton;
	initJeton(nouveauJeton, unJoueur->couleur, coordonnees);
	
	if(unJoueur->listeJetons != NULL){
		nouveauJeton->suivant = unJoueur->listeJetons;
	}else{
		nouveauJeton->suivant = NULL;
	}
	unJoueur->listeJetons = nouveauJeton;
	(unJoueur->nbJeton)++;
}

void supprimeJetonJoueur(Joueur * unJoueur, int coordonnees[2]){
	Jeton * tmp = unJoueur->listeJetons;
	Jeton * precedent = NULL;
	bool supprime = false;

	while(tmp != NULL && !supprime){
		if(tmp->coordonnees[0] == coordonnees[0] && tmp->coordonnees[1] == coordonnees[1]){
			if(precedent == NULL){
				unJoueur->listeJetons = unJoueur->listeJetons->suivant;
				free(tmp);
			}else{
				precedent->suivant = tmp->suivant;
				free(tmp);
			}
			unJoueur->nbJeton = unJoueur->nbJeton -1;

			supprime = true;
		}
		precedent = tmp;
		tmp = tmp->suivant;
	}
}

void afficheJoueur(Joueur joueur){
	cout << "AFFICHAGE DES INFORMATIONS D'UN JOUEUR" << endl;
	cout << "Nom: " << joueur.nom << endl;
	cout << "Nombre de jetons: " << joueur.nbJeton << endl;
}