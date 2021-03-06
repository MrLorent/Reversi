#include <iostream>
#include <cstring>
#include "modele.h"
#include "vue.h"
#include "controleur.h"

using namespace std;


// MODÈLE

/**
 * Initialise le jeu passé en paramètre:
 * - Initialise le joueur 1
 * - Initialise le joueur 2 (Humain ou ordinateur)
 * - Implémente les jetons de base à chaque joueur
 * - Initialise le joueur courant
 * - Initialise le joueur en attente
 * - Initialise le mode de la partie
*/
void initJeu(Jeu * unJeu){
	int coor1[2]={3,3}, coor2[2]={4,4}, coor3[2]={3,4}, coor4[2]={4,3};

	// Initialisation du premier joueur et de ses deux premiers jetons
	cout << endl;
	cout << CYAN << "ENREGISTREMENT DU PREMIER JOUEUR :" << ANNULE_COULEUR <<endl;
	initJoueur(&unJeu->joueur1, 'b');
	ajouteJetonJoueur(&unJeu->joueur1, coor1);
	ajouteJetonJoueur(&unJeu->joueur1, coor2);

    // Initialisation du second joueur et de ses deux premiers jetons
    if(unJeu->mode == HUMAIN){
        // Dans le cas d'une partie face à un autre joueur:
        cout << endl;
        cout << CYAN << "ENREGISTREMENT DU SECOND JOUEUR :" << ANNULE_COULEUR << endl;
        initJoueur(&unJeu->joueur2, 'n');
    }else{
        // Dans le cas d'une partie face à l'ordinateur:
        initOrdinateur(&unJeu->joueur2);
    }
    ajouteJetonJoueur(&unJeu->joueur2, coor3);
    ajouteJetonJoueur(&unJeu->joueur2, coor4);

	// Initialisation du joueur courant
	unJeu->joueurCourant = &unJeu->joueur1;
	unJeu->joueurEnAttente = &unJeu->joueur2;

	// Initialisation de la liste coups jouables à NULL pour le début
	unJeu->coupsJouables = NULL;

	// Initialisation du plateau de jeu
	initPlateau(unJeu->plateau);
}

// CONTROLEURS

/**
 * Change le joueur courant du jeu passé en paramètre
*/
void changeJoueurCourant(Jeu * unJeu){
	if(strcmp(unJeu->joueurCourant->nom, (*unJeu).joueur1.nom) == 0 ){
		unJeu->joueurCourant = &unJeu->joueur2;
		unJeu->joueurEnAttente = &unJeu->joueur1;
	}else{
		unJeu->joueurCourant = &unJeu->joueur1;
		unJeu->joueurEnAttente = &unJeu->joueur2;
	}
}

/**
 * Enregistre les coups jouables d'un joueur sur un tour, et le nombre de jetons compturés par coup dans la liste de coups jouables fournie en paramètre
*/
bool analyseCoupsJouables(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], Joueur * joueurCourant, Joueur * adversaire, ListeCoupsJouables * coupsJouables){
    Jeton * tmp = joueurCourant->listeJetons;

    // Pour chaque jetons de la liste du joueur courant
    while(tmp != NULL){
        Jeton * leCoupJouable = NULL;

        // On parcourt les 8 cases adjacentes
        for(int direction=0; direction<8;direction++){
            // On regarde si l'une d'entre elle contient un jeton adverse
            if(caseExiste(tmp->coordonnees[0]+VECTEURS[direction][0], tmp->coordonnees[1]+VECTEURS[direction][1]) && plateau[tmp->coordonnees[1]+VECTEURS[direction][1]][tmp->coordonnees[0]+VECTEURS[direction][0]]->couleur == adversaire->couleur){
                int count = 0;

                // Si l'on détecte un jeton adverse, on verifie si cette direction aboutie à une case vide
                if(directionJouable(plateau, tmp, direction, &count, adversaire->couleur)){
                    // Si l'on aboutie à une case vide, on récupère sa position et on l'enregistre comme coups jouable avec le nombre de jetons qu'elle permet de capturer
                    leCoupJouable = plateau[tmp->coordonnees[1]+VECTEURS[direction][1]*(count+1)][tmp->coordonnees[0]+VECTEURS[direction][0]*(count+1)];
                    enregistreCoupJouable(coupsJouables, leCoupJouable, count);
                }
            }
        }
        tmp = tmp->suivant;
    }

    // Si il y a au moins un coups jouable, on confirme que le joueur courant peux jouer
    if(*coupsJouables != NULL){
        return true;
    }else{
        return false;
    }
    
}

/**
 * Vérifie si la direction fournie à partir d'une position donnée aboutie bien à une case vide en comptant le nombre de case parcourue 
*/
bool directionJouable(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], Jeton * position, int uneDirection, int * count, char couleurAdversaire){
    Jeton * caseSuivante;
    
    // On vérifie que la case suivante existe bien dans le tableau
    if(caseExiste(position->coordonnees[0] + VECTEURS[uneDirection][0], position->coordonnees[1] + VECTEURS[uneDirection][1])){
        caseSuivante = plateau[position->coordonnees[1] + VECTEURS[uneDirection][1]][position->coordonnees[0] + VECTEURS[uneDirection][0]];

        // Si la case suivante contient encore un jeton adverse, alors on continue d'avancer
        if(caseSuivante->couleur == couleurAdversaire){
            // On met à jour le nombre de jetons parcourus (jetons captures éventuels)
            (*count)++;
            return directionJouable(plateau, caseSuivante, uneDirection, count, couleurAdversaire);
        }else{
            // Si la case suivante est vide, alors le coup est jouable (=true), sinon il n'est pas jouable (=false)
            if(caseSuivante->couleur == 'v'){
                return true;
            }else{
                return false;
            }
        }
    }else{
        return false;
    }
}

/**
 * Ajoute un nouveau coup jouable à la liste de coups jouables passés en paramètre
 * Si le coup existe déjà, seul le nombre capture du coup déjà enregistré est mis à jour
*/
void enregistreCoupJouable(ListeCoupsJouables * laListe, Marqueur * emplacement, int nbCaptures){
    // On vérifie si le coup jouable est déjà enregistré
	int rang = estEnregistre(laListe, emplacement->coordonnees);

	if(rang >= 0){
        // Si il est déjà enregistré, alors on le récupère et on met à jour son nombre de jetons capturés
		CoupJouable * leCoupJouable = *laListe;

        for(int i=0; i<rang-1; i++){
            leCoupJouable = leCoupJouable->suivant;
        }

		leCoupJouable->nbCaptures = leCoupJouable->nbCaptures + nbCaptures;

	}else{
        // Si le coup n'existe pas déjà, on en créer un autre, et on l'ajoute à la liste
		CoupJouable * nouveauCoup = new CoupJouable;
		nouveauCoup->emplacement = emplacement;
		nouveauCoup->nbCaptures = nbCaptures;
		nouveauCoup->suivant = *laListe;
		*laListe = nouveauCoup;
	}
}

/**
 * Vérifie si le coup jouable fournit en paramètre existe déjà dans la liste fournie 
*/
int estEnregistre(ListeCoupsJouables * coupsJouables, int coorEmplacement[2]){
	int rang = 0;
	bool present = false;
	CoupJouable * tmp = *coupsJouables;

	while(tmp != NULL && !present){
		if((tmp)->emplacement->coordonnees[0] == coorEmplacement[0] && (tmp)->emplacement->coordonnees[1] == coorEmplacement[1]){
			present = true;
		}
		rang++;
		tmp = tmp->suivant;
	}
	if(present){
		return rang;
	}else{
		return -1;
	}
}

/**
 * Vérifie si la case saisie par l'utilisateur fait bien partie de la liste de coups jouable fournie
*/
bool coupJouable(ListeCoupsJouables * coupsJouables, int caseSouhaitee[2]){
    if(estEnregistre(coupsJouables, caseSouhaitee) >= 0){
        return true;
    }else{
        cout << "Erreur: vous devez au moins capturer un jeton adverse." << endl;
        cout << "Veuillez saisir une nouvelle case :" << endl;
        return false;
    }
}

/**
 * Identifie les jetons capturer par un coup joué, et effectue le transfert d'un joueur à l'autre
*/
void joueLeCoup(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], int coorJetonPlace[2], Joueur * joueurCourant, Joueur * adversaire){
    // On parcourt les 8 case adjacentes à la case du jeton qui vient d'être placé
    for(int direction=0;direction<8;direction++){
        ListeJetons jetonsCaptures = NULL;

        // On vérifie si une des case adjacente contient un jeton adverse
        if(caseExiste(coorJetonPlace[0]+VECTEURS[direction][0], coorJetonPlace[1]+VECTEURS[direction][1]) && plateau[coorJetonPlace[1]+VECTEURS[direction][1]][coorJetonPlace[0]+VECTEURS[direction][0]]->couleur == adversaire->couleur){

            // Si un jeton adverse est détecté, on verifie qu'un autre jeton du joueur courant se trouve derrière tout en enregistrant les jetons adverse que l'on parcourt dans la liste de jetonsCapture
            if(directionCapture(plateau, &jetonsCaptures, coorJetonPlace, direction, adversaire->couleur, joueurCourant->couleur)){
                // Si la direction aboutie bien sur un jeton du joueur courant, alors pour chaque jeton de la liste de jetonsCaptures, on ajoute un jeton correspondant au joueur courant, et on supprime le jeton correspondant de la liste de jeton du joueur en attente 
                Jeton * tmp = jetonsCaptures;
                while(tmp != NULL){
                    ajouteJetonJoueur(joueurCourant, tmp->coordonnees);
                    supprimeJetonJoueur(adversaire, tmp->coordonnees);
                    tmp = tmp->suivant;
                }
            }
        }
        // On vide la liste pour la prochaine direction à vérifier
        videListeJetons(&jetonsCaptures);
    }
    // En plus des jetons capturés, on ajoute à la liste de jeton du joueur courant le jeton qu'il vient de placer, et on supprime le marqueur de la position correspondante contenu dans le plateau
    ajouteJetonJoueur(joueurCourant, coorJetonPlace);
    free(plateau[coorJetonPlace[1]][coorJetonPlace[0]]);
}

/**
 * Vérifie si la direction fournie à partir de la position jouée aboutie bien à une case contenant un des jetons du joueur courant en listant les jetons adverses parcourus entre temps
*/
bool directionCapture(Jeton * plateau[MAXLARGEUR][MAXLARGEUR], ListeJetons* jetonsCaptures, int position[2], int uneDirection, char couleurAdversaire, char couleurJoueurCourant){
    Jeton* caseSuivante;
    
    // On vérifie que la case suivante existe
    if(caseExiste(position[0] + VECTEURS[uneDirection][0], position[1] + VECTEURS[uneDirection][1])){
        caseSuivante = plateau[position[1] + VECTEURS[uneDirection][1]][position[0] + VECTEURS[uneDirection][0]];

        if(caseSuivante->couleur == couleurAdversaire){
            // Si la case suivante contient un jeton adverse, on l'ajoute à la liste de captures potentielles, et on continue d'avancer
            ajouteJetonCapture(jetonsCaptures, caseSuivante->coordonnees);
            return directionCapture(plateau, jetonsCaptures, caseSuivante->coordonnees, uneDirection, couleurAdversaire, couleurJoueurCourant);
        }else{
            // Si la case suivante contient un des jetons du joueur courant, alors les jetons parcourus jusque là sont bien captures
            if(caseSuivante->couleur == couleurJoueurCourant){
                return true;
            }else{
                return false;
            }
        }
    }else{
        return false;
    }
}

/**
 * Vide la liste de coups jouable fournie en paramètre
*/
void videListeCoupsJouables(ListeCoupsJouables * uneListe){
	CoupJouable * tmp = *uneListe;

	while(tmp != NULL){
		*uneListe = (*uneListe)->suivant;
		free(tmp);
		tmp = *uneListe;
	}
}