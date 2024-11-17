#ifndef __EVALUATION__
#define __EVALUATION__

#include "analyse_syntaxique.h"
#include "arbresAbstraits.h"
#include "listeVariables.h"

typedef enum { 
    FAUTE_SEMANTIQUE,
    FAUTE_SEMANTIQUE_CAS_PAS_GERE,
    FAUTE_SEMANTIQUE_DIVISION_PAR_ZERO,
    FAUTE_SEMANTIQUE_DIVISION_AVEC_SECOND_OPERANDE_NUL,
    ARBRE_ABSTRAIT_VIDE,
    POINTEUR_LISTE_ARBRES_A_EVALUER_NUL,
    POINTEUR_NOEUD_ARBRE_NUL,
    LISTE_ARBRES_A_EVALUER_VIDE,
    NATURE_NOEUD_ARBRE_ABSTRAIT_INCONNUE,
    ERREUR_STRUCTURE_ARBRE_ABSTRAIT,
    ERREUR_INFORMATION_NOEUD_ARBRE_ABSTRAIT,
    ERREUR_INFORMATIONS_NOEUD_CONDITION_ENTREE_BOUCLE,
    POINTEUR_LISTE_VARIABLES_NUL,
    FAUTE_SEMANTIQUE_LISTE_VARIABLES,
    FAUTE_SEMANTIQUE_LISTE_VARIABLES_NULLE,
    FAUTE_SEMANTIQUE_FONCTION_INCONNUE,
    FAUTE_SEMANTIQUE_UTILISATION_FONCTION,
    FAUTE_SEMANTIQUE_SAISIE_UTILISATEUR,
    FAUTE_SEMANTIQUE_TYPE_VARIABLE,
    FAUTE_SEMANTIQUE_TENTE_ACCES_VALEUR_VARIABLE_NON_ASSIGNEE,
    FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE,
    FAUTE_SEMANTIQUE_COMPARAISON_IMPOSSIBLE
} ERREUR_SEMANTIQUE;



ERREUR_SEMANTIQUE evaluationListeArbresAbstraits(ListeArbresAbstraits *listeArbresAEvaluer, int debug);


//Cette fonction retourne la valeur en DOUBLE correspondant aux opÃ©rations Ã©crites dans l'arbre.
//Si il y a une erreur avec l'arbre ou que celui-ci ne correspond pas Ã  une opÃ©ration, une erreur est renvoyÃ©e.
//Cette fonction appelle GetValeurDoubleNoeudArbreRecurssif
ERREUR_SEMANTIQUE GetValeurDoubleNoeudArbre(NoeudArbreAbstrait *noeudValeur, ListeVariables* listeVariablesProgramme, double* valeurDoubleRetour, int debug);
//Cette fonction n'est pas Ã  appeler directement, elle est appelÃ©e par GetValeurDoubleNoeudArbre
ERREUR_SEMANTIQUE GetValeurDoubleNoeudArbreRecurssif(NoeudArbreAbstrait *noeudValeur, ListeVariables* listeVariablesProgramme, double* valeurDoubleRetour, int debug);

//Cette fonction fait pointer l'argument pointeurChaineRetour vers un pointeur de chaine de caractÃ¨respointant vers la chaine correspondant Ã  ce qu'il y a d'Ã©crit dans l'arbre.
//Si il y a une erreur avec l'arbre ou que celui-ci ne correspond pas Ã  une chaine de caractÃ¨res, une erreur est renvoyÃ©e.
//Cette fonction appelle GetStringNoeudArbreRecurssif
//ATTENTION ! Le pointeur pointÃ© par pointeurChaineRetour doit forcÃ©ment Ãªtre initialisÃ© Ã  NULL sinon erreur: *pointeurChaineRetour = NULL
ERREUR_SEMANTIQUE GetStringNoeudArbre(NoeudArbreAbstrait *noeudString, ListeVariables* listeVariablesProgramme, char** pointeurChaineRetour, int* estChaineNulle, int debug);
//Cette fonction n'est pas Ã  appeler directement, elle est appelÃ©e par GetStringNoeudArbre
//ATTENTION ! Le pointeur pointÃ© par pointeurChaineRetour doit forcÃ©ment Ãªtre initialisÃ© Ã  NULL sinon erreur: *pointeurChaineRetour = NULL
ERREUR_SEMANTIQUE GetStringNoeudArbreRecurssif(NoeudArbreAbstrait *noeudString, ListeVariables* listeVariablesProgramme, char** pointeurChaineRetour, int* estChaineNulle, int debug);

//Fonction permettant de savoir si une condition d'entrÃ©e dans une boucle est vÃ©rifiÃ©e
//Si la condition est vÃ©rifiÃ©e, l'attribut estConditionTrue vers une variable de valeur 1, sinon 0
//Une erreur est retournÃ©e si il y a un problÃ¨me avec l'arbre reprÃ©sentant la condition
ERREUR_SEMANTIQUE getValeurArbreConditionBoucle(NoeudArbreAbstrait *noeudCondition, ListeVariables* listeVariablesProgramme, int* estConditionTrue, int debug);


#endif