#ifndef __LISTE_VARIABLES__
#define __LISTE_VARIABLES__

//Differentes erreurs pouvant se produire en utilisant les fonctions
typedef enum { 
    PAS_D_ERREUR_LISTE_VARIABLES,
    LISTE_VARIABLES_NULLE,
    VARIABLE_PAS_TROUVEE,
    VARIABLE_DU_MEME_NOM_DEJA_DECLAREE,
    PROBLEME_PARAMETRES,
    ERREUR_TYPE_VARIABLE_NON_DEFINI,
    TYPE_VARIABLE_INCONNU,
    POINTEUR_VALEUR_VARIABLE_NULL
} ERREUR_LISTE_VARIABLES;



typedef enum {
  INT,
  DOUBLE,
  STRING,
  TYPE_VARIABLE_NON_DEFINI
} TypesVariables;

//Definition des types d'elements de la liste
struct StructElementListeVariablesInt {
  int valeur;
  int pasDeValeur;
};
typedef struct StructElementListeVariablesInt ElementListeVariablesInt;


struct StructElementListeVariablesDouble {
  double valeur;
  int pasDeValeur;
};
typedef struct StructElementListeVariablesDouble ElementListeVariablesDouble;


struct StructElementListeVariablesString {
  char* adresseChaine;
  int pasDeValeur;
};
typedef struct StructElementListeVariablesString ElementListeVariablesString;




struct StructElementListeVariables {
  char* nom;
  TypesVariables typeVariable;
  ElementListeVariablesInt* variableInt;
  ElementListeVariablesDouble* variableDouble;
  ElementListeVariablesString* variableString;
  struct StructElementListeVariables *suivant;
};
typedef struct StructElementListeVariables ElementListeVariables;

typedef struct { 
  ElementListeVariables *tete;
} ListeVariables;


//Fonctions de construction
ListeVariables* NouvelleListeVariables(void);
ElementListeVariables* NouvelElementListeVariables(void);

ElementListeVariablesInt* NouvelElementListeVariablesInt(void);
ElementListeVariablesDouble* NouvelElementListeVariablesDouble(void);
ElementListeVariablesString* NouvelElementListeVariablesString(void);


//Fonction d'ajout d'une variable Ã  la liste/de changement de valeur d'une variable de la liste
//La valeur de la variable ne peut Ãªtre nulle que si cette derniere est de type string
ERREUR_LISTE_VARIABLES DeclarerOuChangerValeurVariableListeVariables(
   ListeVariables* laListeVariables,
   char* nomVariable, TypesVariables type, int declaration, int valeurNulle, int pasDeValeur,
   int valeurInt, double valeurDouble, char* chaine
);

//Les fonctions suivantes sont des specialisations de DeclarerOuChangerValeurVariableListeVariables
//Elles retournent toutes une erreur si la variable n'est pas trouvee dans la liste
//Fonction d'ajout d'une variable Ã  la liste (declaration) avec affectation de valeur
//Retourne une erreur si la variable n'est pas trouvee dans la liste
//Retourne une erreur si une variable du mÃªme nom est dejÃ  dans la liste
//La valeur de la variable ne peut Ãªtre nulle que si cette derniere est de type string
ERREUR_LISTE_VARIABLES DeclarerVariableAvecValeurListeVariables(
   ListeVariables* laListeVariables, char* nomVariable, TypesVariables type,
   int valeurInt, double valeurDouble, int valeurNulle, char* chaine
);
//Fonction d'ajout d'une variable Ã  la liste (declaration) sans affectation de valeur
//Retourne une erreur si la variable n'est pas trouvee dans la liste
//Retourne une erreur si une variable du mÃªme nom est dejÃ  dans la liste
ERREUR_LISTE_VARIABLES DeclarerVariableSansValeurListeVariables(
   ListeVariables* laListeVariables, char* nomVariable, TypesVariables type
);

//Fonction de modification de valeur d'une variable existante dans la liste
//Retourne une erreur si la variable n'est pas trouvee dans la liste
//Retourne une erreur si le type de la variable ne coincide pas avec celui de la valeur devant Ãªtre affectee
//Si valeurNulle == 1, on rend la valeur de notre variable NULLE (il faut que ce soit une chaine de caracteres, sinon une erreur est renvoyee)
//Si estValeurAffecteeNombre == 1, la valeur affectee Ã  la variable sera valeurNombre, sinon ce sera chaine
ERREUR_LISTE_VARIABLES ChangerValeurVariableListeVariables(
   ListeVariables* laListeVariables, char* nomVariable, int estValeurAffecteeNombre, int valeurNulle,
   double valeurNombre, char* chaine
);


//Fonction retournant le type et la valeur d'une variable dont le nom est passe en parametre
//Une erreur est retournee si la variable n'est pas trouvee
//valeurNulle == 1 si la variable est nulle (seulement pour les string)
//Si l'on a pas encore affecte de valeur Ã  la variable (si elle est juste declaree), pasDeValeurAffectee == 1
ERREUR_LISTE_VARIABLES GetTypeEtValeurVariableListeVariables(
  ListeVariables* laListeVariables, char* nomVariable, TypesVariables* typeVariable, int* pasDeValeurAffectee, int* valeurNulle,
  int* valeurInt, double* valeurDouble, char** pointeurChaine
);
//Fonction retournant le type d'une variable dont le nom est passe en parametre
//Une erreur est retournee si la variable n'est pas trouvee
ERREUR_LISTE_VARIABLES GetTypeVariableListeVariables(
  ListeVariables* laListeVariables, char* nomVariable, TypesVariables* typeVariable
);



//Fonctions de destruction
//La premiere fonction utilise la seconde, et la seconde la troisieme
void DetruireListeVariables(ListeVariables* ListeVariablesADetruire);
void DetruireElementListeVariablesEtSaDescendance(ElementListeVariables* ElementListeVariablesADetruire);
void DetruireElementListeVariables(ElementListeVariables* ElementListeVariablesADetruire);
//Si aucune erreur est renvoyee, la variable dont le nom est passe en parametre est dEtruite correctement
ERREUR_LISTE_VARIABLES DetruireVariable(ListeVariables* laListeVariables, char* nomVariable);



//Fonction retournant 1 si le type est un type defini
int EstTypeDefini(TypesVariables leType);

#endif