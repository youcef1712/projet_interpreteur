#ifndef __ARBRES_ABSTRAITS__
#define __ARBRES_ABSTRAITS__

#include "listeVariables.h"

typedef enum {
  NON_DEFINI,
  OPERATION,
  AFFECTATION,
  VARIABLE_,
  FONCTION_,
  TYPE_VARIABLE_,
  NOMBRE_,
  CHAINE_CARACTERES_,
  BOUCLE,
  VALEUR_NULLE_
} TypeNoeudArbreAbstrait;

typedef enum { 
  ADDITION,
  SOUSTRACTION,
  MULTIPLICATION,
  DIVISION,
  EGALITE,
  DIFFERENT_DE,
  INFERIEUR_A,
  INFERIEUR_OU_EGAL_A,
  SUPERIEUR_A,
  SUPERIEUR_OU_EGAL_A,
  AUTRE
} Operations;

typedef enum {
  IF,
  WHILE,
  TYPE_BOUCLE_NON_DEFINI
} TypesBoucles;




struct Noeud { 
  TypeNoeudArbreAbstrait nature;
  Operations operation;
  TypesVariables typeVariable;
  char *identificateurVariableOuFonction;
  double valeurNombre;
  char *chaineDeCaracteres;
  TypesBoucles typeBoucle;
  struct Noeud *conditionExecutionBoucle;
  struct StructListeArbresAbstraits *listeArbresInstructionsBoucleConditionValide;
  struct StructListeArbresAbstraits *listeArbresInstructionsBoucleConditionNonValide;
  struct Noeud *gauche;
  struct Noeud *droite;
};
typedef struct Noeud NoeudArbreAbstrait;

typedef struct { 
  NoeudArbreAbstrait *tete;
} ArbreAbstrait;



struct ElementListe {
  ArbreAbstrait *arbre;
  struct ElementListe *suivant;
};
typedef struct ElementListe ElementListeArbresAbstraits;

struct StructListeArbresAbstraits { 
  ElementListeArbresAbstraits *tete;
};
typedef struct StructListeArbresAbstraits ListeArbresAbstraits;


NoeudArbreAbstrait* nouveauNoeudArbreAbstrait(void);
ArbreAbstrait* nouvelArbreAbstrait(void);

ElementListeArbresAbstraits* nouvelElementListeArbresAbstraits(void);
ListeArbresAbstraits* nouvelleListeArbresAbstraits(void);

//Copie les valeurs et pointeurs du second noeud d'arbre abstrait dans le premier, retourne 1 si la copie s'est bien passee
int CopieInformationsNoeudArbreAbstrait2DansNoeudArbreAbstrait1(NoeudArbreAbstrait *noeudArbreAbstrait1, NoeudArbreAbstrait *noeudArbreAbstrait2);

//Remet a  zero les informations d'un noeud (comme si il venait d'etre cree), retourne 1 si la remise Ã  zero s'est bien passee
int RemiseAZeroNoeudArbreAbstrait(NoeudArbreAbstrait *noeudArbreAbstraitARemettreAZero);

//Retourne 1 si l'element a bien ete ajoute
int ajouterArbreListeArbresAbstraits(ListeArbresAbstraits *laListe, ArbreAbstrait *arbreAAjouter);

int detruireNoeudArbreAbstrait(NoeudArbreAbstrait* noeud);
int detruireNoeudArbreAbstraitEtSaDescendance(NoeudArbreAbstrait* noeud);
int detruireArbreAbstrait(ArbreAbstrait *ArbreADetruire);

void detruireElementListeArbresAbstraitsEtSuivants(ElementListeArbresAbstraits *ElementListeArbresADetruire);
void detruireListeArbresAbstraits(ListeArbresAbstraits *ListeArbresADetruire);

#endif