#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "listeVariables.h"

ListeVariables* NouvelleListeVariables(void)
{
   ListeVariables* nouvelleListe = malloc(sizeof(ListeVariables));

   nouvelleListe->tete = NULL;
   
   return nouvelleListe;
}

ElementListeVariables* NouvelElementListeVariables(void)
{
   ElementListeVariables* nouvelElement = malloc(sizeof(ElementListeVariables));

   nouvelElement->nom = NULL;
   nouvelElement->typeVariable = TYPE_VARIABLE_NON_DEFINI;
   nouvelElement->variableInt = NULL;
   nouvelElement->variableDouble = NULL;
   nouvelElement->variableString = NULL;
   nouvelElement->suivant = NULL;
   
   return nouvelElement;
}


ElementListeVariablesInt* NouvelElementListeVariablesInt(void)
{
   ElementListeVariablesInt* nouvelElement = malloc(sizeof(ElementListeVariablesInt));

   nouvelElement->pasDeValeur = 1;
   
   return nouvelElement;
}
ElementListeVariablesDouble* NouvelElementListeVariablesDouble(void)
{
   ElementListeVariablesDouble* nouvelElement = malloc(sizeof(ElementListeVariablesDouble));

   nouvelElement->pasDeValeur = 1;
   
   return nouvelElement;
}
ElementListeVariablesString* NouvelElementListeVariablesString(void)
{
   ElementListeVariablesString* nouvelElement = malloc(sizeof(ElementListeVariablesString));

   nouvelElement->pasDeValeur = 1;
   nouvelElement->adresseChaine = NULL;
   
   return nouvelElement;
}


//Fait pointer le de pointeur pointeur passe en parametre vers un pointeur pointant une variable d'une liste dont le nom est passe en parametre
//Une erreur est retournee si la liste est nulle ou si la variable n'est pas trouvee
ERREUR_LISTE_VARIABLES GetPointeurVariableByNom(ListeVariables* laListeVariables, char* nomVariable, ElementListeVariables** pointeurVariable)
{
   ERREUR_LISTE_VARIABLES retour = VARIABLE_PAS_TROUVEE;

   if(laListeVariables != NULL)
   {
      ElementListeVariables* pointeurParcoursListe = laListeVariables->tete;
      
      while(retour == VARIABLE_PAS_TROUVEE && pointeurParcoursListe != NULL)
      {
         //Si on a trouve notre variable
         if(strcmp(nomVariable, pointeurParcoursListe->nom) == 0)
         {
            *pointeurVariable = pointeurParcoursListe;
            retour = PAS_D_ERREUR_LISTE_VARIABLES;
         }
         else
         {
            pointeurParcoursListe = pointeurParcoursListe->suivant;
         }
      }
   }
   else
   {
      retour = LISTE_VARIABLES_NULLE;
   }

   return retour;
}

//Fonction retournant le type et la valeur d'une variable dont le nom est passe en parametre
//Une erreur est retournee si la variable n'est pas trouvee
//valeurNulle == 1 si la variable est nulle (seulement pour les string)
//Si l'on a pas encore affecte de valeur Ã  la variable (si elle est juste declaree), pasDeValeurAffectee == 1
ERREUR_LISTE_VARIABLES GetTypeEtValeurVariableListeVariables(
  ListeVariables* laListeVariables, char* nomVariable, TypesVariables* typeVariable, int* pasDeValeurAffectee, int* valeurNulle,
  int* valeurInt, double* valeurDouble, char** pointeurChaine
)
{
   //On contrele si la variable existe, si elle n'existe pas on retourne une erreur
   ElementListeVariables *pointeurVariableRecherchee;
   
   ERREUR_LISTE_VARIABLES retour = GetPointeurVariableByNom(laListeVariables, nomVariable, &pointeurVariableRecherchee);
   
   //Si la variable existe, c'est ok
   if(retour == PAS_D_ERREUR_LISTE_VARIABLES)
   {
      *typeVariable = pointeurVariableRecherchee->typeVariable;
      *pasDeValeurAffectee = 0;
      *valeurNulle = 0;

      if(*typeVariable == INT)
      {
         if(pointeurVariableRecherchee->variableInt != NULL)
         {
            *pasDeValeurAffectee = pointeurVariableRecherchee->variableInt->pasDeValeur;
            *valeurInt = pointeurVariableRecherchee->variableInt->valeur;
         }
         else
         {
            retour = POINTEUR_VALEUR_VARIABLE_NULL;
            printf("Erreur, le pointeur vers la valeur de la variable (ElementListeVariablesInt* variableInt) est nul.\n");
         }
      }
      else
      {
         if(*typeVariable == DOUBLE)
         {
            if(pointeurVariableRecherchee->variableDouble != NULL)
            {
               *pasDeValeurAffectee = pointeurVariableRecherchee->variableDouble->pasDeValeur;
               *valeurDouble = pointeurVariableRecherchee->variableDouble->valeur;
            }
            else
            {
               retour = POINTEUR_VALEUR_VARIABLE_NULL;
               printf("Erreur, le pointeur vers la valeur de la variable (ElementListeVariablesDouble* variableDouble) est nul.\n");
            }
         }
         else
         {
            if(*typeVariable == STRING)
            {
               if(pointeurVariableRecherchee->variableString != NULL)
               {
                  if(*pointeurChaine != NULL && strlen(*pointeurChaine) > 0)
                  {
                     free(*pointeurChaine);
                     *pointeurChaine = NULL;
                  }

                  *pasDeValeurAffectee = pointeurVariableRecherchee->variableString->pasDeValeur;
                  
                  if(pointeurVariableRecherchee->variableString->pasDeValeur == 0)
                  {
                     if(pointeurVariableRecherchee->variableString->adresseChaine == NULL)
                     {
                        *valeurNulle = 1;
                     }
                     else
                     {
                        *valeurNulle = 0;
                        *pointeurChaine = malloc((strlen(pointeurVariableRecherchee->variableString->adresseChaine) + 1) * sizeof(char));
                        strcpy(*pointeurChaine, pointeurVariableRecherchee->variableString->adresseChaine);
                     }
                  }
               }
               else
               {
                  retour = POINTEUR_VALEUR_VARIABLE_NULL;
                  printf("Erreur, le pointeur vers la valeur de la variable (ElementListeVariablesString* variableString) est nul.\n");
               }
            }
            else
            {
               if(*typeVariable == TYPE_VARIABLE_NON_DEFINI)
               {
                  retour = TYPE_VARIABLE_NON_DEFINI;
                  printf("Erreur, le type de la variable dont on veut les informations n'est pas defini dans la liste");
               }
               else
               {
                  retour = TYPE_VARIABLE_INCONNU;
                  printf("Erreur, le type de la variable est inconnu.\n");
               }
            }
         }
      }
   }

   return retour;
}

//Fonction retournant le type d'une variable dont le nom est passe en parametre
//Une erreur est retournee si la variable n'est pas trouvee
ERREUR_LISTE_VARIABLES GetTypeVariableListeVariables(
  ListeVariables* laListeVariables, char* nomVariable, TypesVariables* typeVariable
)
{
   int valeurNulle, pasDevaleur, valeurInt;
   double valeurDouble;
   char* chaine = NULL;
   
   ERREUR_LISTE_VARIABLES retour = GetTypeEtValeurVariableListeVariables(
      laListeVariables, nomVariable, typeVariable,
      &pasDevaleur, &valeurNulle, &valeurInt, &valeurDouble, &chaine
   );
   

   if(chaine != NULL && strlen(chaine) > 0)
   {
      free(chaine);
      chaine = NULL;
   }
   

   return retour;
}


//Fonction d'ajout d'une variable Ã  la liste/de changement de valeur d'une variable de la liste
//La valeur de la variable ne peut Ãªtre nulle que si cette derniere est de type string
ERREUR_LISTE_VARIABLES DeclarerOuChangerValeurVariableListeVariables(
   ListeVariables* laListeVariables,
   char* nomVariable, TypesVariables type, int declaration, int valeurNulle, int pasDeValeur,
   int valeurInt, double valeurDouble, char* chaine
)
{
   ERREUR_LISTE_VARIABLES retour = PAS_D_ERREUR_LISTE_VARIABLES;

   //Il faut que le type soit valide
   if(type != INT && type != DOUBLE && type != STRING)
   {
      printf("Probleme avec le type de variable indique.\n");
      retour = PROBLEME_PARAMETRES;
   }
   else
   {
      //Si la fonction est apellee pour une declaration de variable (avec assignation de valeur ou non), on
      //part du postulat qu'un controle a dejÃ  ete fait (qu'aucune variable du mÃªme non n'a ete declaree) et
      //on ajoute un element en tÃªte de liste
      if(declaration == 1)
      {
         ElementListeVariables* nouvelleVariable = NouvelElementListeVariables();


         //On copie la chaine du nom de la variable depuis le pointeur passe en parametre (pour ne pas avoir de
         //probleme si la chaine Ã  l'adresse du parametre nom est modifiee/supprimee)
         nouvelleVariable->nom = malloc((strlen(nomVariable) + 1) * sizeof(char));
         strcpy (nouvelleVariable->nom, nomVariable);

         //On donne un type Ã  notre variable
         nouvelleVariable->typeVariable = type;

         //On associe Ã  l'element cree le pointeur vers la valeur
         if(type == INT)
         {
            nouvelleVariable->variableInt = NouvelElementListeVariablesInt();
         }
         else
         {
            if(type == DOUBLE)
            {
               nouvelleVariable->variableDouble = NouvelElementListeVariablesDouble();
            }
            else
            {
               if(type == STRING)
               {
                  nouvelleVariable->variableString = NouvelElementListeVariablesString();
               }
            }
         }

         nouvelleVariable->suivant = laListeVariables->tete;
         laListeVariables->tete = nouvelleVariable;
      }



      //Le code ci-dessous indique comment on associe une valeur a  notre variable
      //On commence par rechercher notre variable dans la liste
      ElementListeVariables *variableAModifier;

      retour = GetPointeurVariableByNom(laListeVariables, nomVariable, &variableAModifier);

      //Si il n'y a pas d'erreur, on a trouve la variable. On modifie alors sa valeur
      if(retour == PAS_D_ERREUR_LISTE_VARIABLES)
      {
         //Si une valeur est affectee
         if(pasDeValeur == 0)
         {
            //Si la valeur affectee est nulle est que ne type n'est pas string, une erreur est renvoyee
            if(valeurNulle == 1)
            {
               if(type != STRING)
               {
                  printf("Probleme, la valeur nulle est affectee Ã  une variable dont le type n'est pas string.\n");
                  retour = PROBLEME_PARAMETRES;
               }
               else
               {
                  variableAModifier->variableString->pasDeValeur = 0;

                  if(variableAModifier->variableString->adresseChaine != NULL && strlen(variableAModifier->variableString->adresseChaine) > 0)
                  {
                     free(variableAModifier->variableString->adresseChaine);
                  }
                  
                  variableAModifier->variableString->adresseChaine = NULL;
               }
            }
            //Si la valeur affectee n'est pas nulle
            else
            {
               if(type == INT)
               {
                  variableAModifier->variableInt->pasDeValeur = 0;
                  variableAModifier->variableInt->valeur = valeurInt;
               }
               else
               {
                  if(type == DOUBLE)
                  {
                     variableAModifier->variableDouble->pasDeValeur = 0;
                     variableAModifier->variableDouble->valeur = valeurDouble;
                  }
                  else
                  {
                     if(type == STRING)
                     {
                        if(chaine != NULL)
                        {
                           variableAModifier->variableString->pasDeValeur = 0;

                           if(variableAModifier->variableString->adresseChaine != NULL && strlen(variableAModifier->variableString->adresseChaine) > 0)
                           {
                              free(variableAModifier->variableString->adresseChaine);
                              variableAModifier->variableString->adresseChaine = NULL;
                           }
                           variableAModifier->variableString->adresseChaine = malloc ((strlen(chaine) + 1) * sizeof(char));
                           strcpy (variableAModifier->variableString->adresseChaine, chaine);
                        }
                        else
                        {
                           printf("L'adresse entree dans le parametre \"chaine\" est nulle alors que les parametres de la fonction indiquent qu'il faut faire une assignation de valeur Ã  une variable chaine de caracteres non-nulle.\n");
                           retour = PROBLEME_PARAMETRES;
                        }
                     }
                     else
                     {
                        printf("Probleme avec le type de variable indique.\n");
                        retour = PROBLEME_PARAMETRES;
                     }
                  }
               }
            }
         }
      }
   }

   return retour;
}


//Fonction d'ajout d'une variable a  la liste (declaration) avec affectation de valeur

ERREUR_LISTE_VARIABLES DeclarerVariableAvecValeurListeVariables(
   ListeVariables* laListeVariables, char* nomVariable, TypesVariables type,
   int valeurInt, double valeurDouble, int valeurNulle, char* chaine
)
{
   //On contrele si la variable existe, si elle existe on retourne une erreur
   ElementListeVariables *pointeurTestExistenceVariable;
   ERREUR_LISTE_VARIABLES retour = GetPointeurVariableByNom(laListeVariables, nomVariable, &pointeurTestExistenceVariable);
   
   //Si la variable n'existe pas, c'est ok
   if(retour == VARIABLE_PAS_TROUVEE)
   {
      retour = DeclarerOuChangerValeurVariableListeVariables(laListeVariables, nomVariable, type, 1, valeurNulle, 0, valeurInt, valeurDouble, chaine);
   }

   return retour;
}
//Fonction d'ajout d'une variable a  la liste (declaration) sans affectation de valeur
//Retourne une erreur si la variable n'est pas trouvee dans la liste
//Retourne une erreur si une variable du mÃªme nom est dejÃ  dans la liste
ERREUR_LISTE_VARIABLES DeclarerVariableSansValeurListeVariables(
   ListeVariables* laListeVariables, char* nomVariable, TypesVariables type
)
{
   //On contrele si la variable existe, si elle existe on retourne une erreur
   ElementListeVariables *pointeurTestExistenceVariable;
   ERREUR_LISTE_VARIABLES retour = GetPointeurVariableByNom(laListeVariables, nomVariable, &pointeurTestExistenceVariable);
   
   //Si la variable n'existe pas, c'est ok
   if(retour == VARIABLE_PAS_TROUVEE)
   {
      retour = DeclarerOuChangerValeurVariableListeVariables(laListeVariables, nomVariable, type, 1, 0, 1, 0, 0, NULL);
   }

   return retour;
}

//Fonction de modification de valeur d'une variable existante dans la liste
//Retourne une erreur si la variable n'est pas trouvee dans la liste
//Retourne une erreur si le type de la variable ne coincide pas avec celui de la valeur devant Ãªtre affectee
//Si valeurNulle == 1, on rend la valeur de notre variable NULLE (il faut que ce soit une chaine de caracteres, sinon une erreur est renvoyee)
//Si estValeurAffecteeNombre == 1, la valeur affectee Ã  la variable sera valeurNombre, sinon ce sera chaine
ERREUR_LISTE_VARIABLES ChangerValeurVariableListeVariables(
   ListeVariables* laListeVariables, char* nomVariable, int estValeurAffecteeNombre, int valeurNulle,
   double valeurNombre, char* chaine
)
{
   //On contrele si la variable existe, si elle n'existe pas on retourne une erreur
   ElementListeVariables *pointeurVariableAModifier;
   ERREUR_LISTE_VARIABLES retour = GetPointeurVariableByNom(laListeVariables, nomVariable, &pointeurVariableAModifier);
   
   //Si la variable existe, c'est ok
   if(retour == PAS_D_ERREUR_LISTE_VARIABLES)
   {
      //On contrele maintenant si les informations passees dans les parametres sont correctes
      //->Il faut que le type de la valeur a  affecter concorde avec le type de la variable (nombre pour int/double, chaine pour string)
      
      //Cas dans lesquels c'est correct
      if(((pointeurVariableAModifier->typeVariable == INT || pointeurVariableAModifier->typeVariable == DOUBLE) && estValeurAffecteeNombre == 1)
         || (pointeurVariableAModifier->typeVariable == STRING && estValeurAffecteeNombre == 0))
      {
         retour = DeclarerOuChangerValeurVariableListeVariables(laListeVariables, nomVariable, pointeurVariableAModifier->typeVariable, 0, valeurNulle, 0, valeurNombre, valeurNombre, chaine);
      }
      //Sinon c'est incorrect
      else
      {
         printf("Probleme la valeur Ã  affecter n'est pas coherent avec le type de la variable cible.\n");
         retour = PROBLEME_PARAMETRES;
      }
   }

   return retour;
}



void DetruireListeVariables(ListeVariables* ListeVariablesADetruire)
{
    if(ListeVariablesADetruire != NULL)
    {
         if(ListeVariablesADetruire->tete != NULL)
         {
            DetruireElementListeVariablesEtSaDescendance(ListeVariablesADetruire->tete);
         }

         free(ListeVariablesADetruire);
         ListeVariablesADetruire = NULL;
    }
}

void DetruireElementListeVariablesEtSaDescendance(ElementListeVariables* ElementListeVariablesADetruire)
{
   if(ElementListeVariablesADetruire != NULL)
   {
      if(ElementListeVariablesADetruire->suivant != NULL)
      {
         DetruireElementListeVariablesEtSaDescendance(ElementListeVariablesADetruire->suivant);
      }

      DetruireElementListeVariables(ElementListeVariablesADetruire);
      ElementListeVariablesADetruire = NULL;
   }
}

void DetruireElementListeVariables(ElementListeVariables* ElementListeVariablesADetruire)
{
   if(ElementListeVariablesADetruire != NULL)
   {
      //On libere l'espace des valeurs prises par la variable
      if(ElementListeVariablesADetruire->variableInt != NULL)
      {
         free(ElementListeVariablesADetruire->variableInt);
      }
      if(ElementListeVariablesADetruire->variableDouble != NULL)
      {
         free(ElementListeVariablesADetruire->variableDouble);
      }
      if(ElementListeVariablesADetruire->variableString != NULL)
      {
         if(ElementListeVariablesADetruire->variableString->adresseChaine != NULL && strlen(ElementListeVariablesADetruire->variableString->adresseChaine) > 0)
         {
            free(ElementListeVariablesADetruire->variableString->adresseChaine);
         }

         free(ElementListeVariablesADetruire->variableString);
      }

      free(ElementListeVariablesADetruire);
      ElementListeVariablesADetruire = NULL;
   }
}


ERREUR_LISTE_VARIABLES DetruireVariable(ListeVariables* laListeVariables, char* nomVariable)
{
   ERREUR_LISTE_VARIABLES retour = VARIABLE_PAS_TROUVEE;

   if(laListeVariables != NULL)
   {
      ElementListeVariables* variableADetruire = NULL;

      if(strcmp(nomVariable, laListeVariables->tete->nom) == 0)
      {
         variableADetruire = laListeVariables->tete;
         laListeVariables->tete = laListeVariables->tete->suivant;
         DetruireElementListeVariables(variableADetruire);
         retour = PAS_D_ERREUR_LISTE_VARIABLES;
      }
      else
      {
         ElementListeVariables* pointeurParcoursListe = laListeVariables->tete;
         
         while(retour == VARIABLE_PAS_TROUVEE && pointeurParcoursListe->suivant != NULL)
         {
            //Si on a trouve notre variable
            if(strcmp(nomVariable, pointeurParcoursListe->suivant->nom) == 0)
            {
               variableADetruire = pointeurParcoursListe->suivant;
               pointeurParcoursListe->suivant = pointeurParcoursListe->suivant->suivant;
               DetruireElementListeVariables(variableADetruire);
               retour = PAS_D_ERREUR_LISTE_VARIABLES;
            }
            else
            {
               pointeurParcoursListe = pointeurParcoursListe->suivant;
            }
         }
      }
   }
   else
   {
      retour = LISTE_VARIABLES_NULLE;
   }

   return retour;
}



int EstTypeDefini(TypesVariables leType)
{
   int retour = 0;

   if(leType == INT || leType == DOUBLE || leType == STRING)
   {
      retour = 1;
   }

   return retour;
}