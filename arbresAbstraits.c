#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "arbresAbstraits.h"

NoeudArbreAbstrait* nouveauNoeudArbreAbstrait(void)
{
   NoeudArbreAbstrait *nouveauNoeud = malloc(sizeof(NoeudArbreAbstrait));

   nouveauNoeud->nature = NON_DEFINI;

   nouveauNoeud->operation = AUTRE;
   
   nouveauNoeud->typeVariable = TYPE_VARIABLE_NON_DEFINI;

   nouveauNoeud->identificateurVariableOuFonction = NULL;

   nouveauNoeud->valeurNombre = 0;

   nouveauNoeud->chaineDeCaracteres = NULL;

   nouveauNoeud->typeBoucle = TYPE_BOUCLE_NON_DEFINI;

   nouveauNoeud->conditionExecutionBoucle = NULL;

   nouveauNoeud->listeArbresInstructionsBoucleConditionValide = NULL;
   nouveauNoeud->listeArbresInstructionsBoucleConditionNonValide = NULL;

   nouveauNoeud->gauche = NULL;
   nouveauNoeud->droite = NULL;
   
   return nouveauNoeud;
}

ArbreAbstrait* nouvelArbreAbstrait(void)
{
   ArbreAbstrait *nouveauArbre = malloc(sizeof(ArbreAbstrait));

   nouveauArbre->tete = NULL;
   
   return nouveauArbre;
}

ElementListeArbresAbstraits* nouvelElementListeArbresAbstraits(void)
{
   ElementListeArbresAbstraits *nouvelElement = malloc(sizeof(ElementListeArbresAbstraits));

   nouvelElement->arbre = NULL;
   nouvelElement->suivant = NULL;
   
   return nouvelElement;
}

ListeArbresAbstraits* nouvelleListeArbresAbstraits(void)
{
   ListeArbresAbstraits *nouvelleListe = malloc(sizeof(ListeArbresAbstraits));

   nouvelleListe->tete = NULL;
   
   return nouvelleListe;
}

//Copie les valeurs et pointeurs du second noeud d'arbre abstrait dans le premier, val_retourne 1 si la copie s'est bien passee
int CopieInformationsNoeudArbreAbstrait2DansNoeudArbreAbstrait1(NoeudArbreAbstrait *noeudArbreAbstrait1, NoeudArbreAbstrait *noeudArbreAbstrait2)
{
   int val_retour = 0;
   if(noeudArbreAbstrait1 != NULL)
   {
      if(noeudArbreAbstrait2 != NULL)
      {
         noeudArbreAbstrait1->nature = noeudArbreAbstrait2->nature;

         noeudArbreAbstrait1->operation = noeudArbreAbstrait2->operation;

         noeudArbreAbstrait1->typeVariable = noeudArbreAbstrait2->typeVariable;

         if(noeudArbreAbstrait1->identificateurVariableOuFonction != NULL && strlen(noeudArbreAbstrait1->identificateurVariableOuFonction) > 0)
         {
            free(noeudArbreAbstrait1->identificateurVariableOuFonction);
            noeudArbreAbstrait1->identificateurVariableOuFonction = NULL;
         }
         if(noeudArbreAbstrait2->identificateurVariableOuFonction != NULL)
         {
            if(strlen(noeudArbreAbstrait1->identificateurVariableOuFonction) > 0)
            {

            }
            else
            {

            }
            noeudArbreAbstrait1->identificateurVariableOuFonction = malloc((strlen(noeudArbreAbstrait2->identificateurVariableOuFonction) + 1) * sizeof(char));
            strcpy(noeudArbreAbstrait1->identificateurVariableOuFonction, noeudArbreAbstrait2->identificateurVariableOuFonction);
         }
         
         noeudArbreAbstrait1->valeurNombre = noeudArbreAbstrait2->valeurNombre;

         if(noeudArbreAbstrait1->chaineDeCaracteres != NULL && strlen(noeudArbreAbstrait1->chaineDeCaracteres) > 0)
         {
            free(noeudArbreAbstrait1->chaineDeCaracteres);
            noeudArbreAbstrait1->chaineDeCaracteres = NULL;
         }
         if(noeudArbreAbstrait2->chaineDeCaracteres != NULL)
         {
            noeudArbreAbstrait1->chaineDeCaracteres = malloc((strlen(noeudArbreAbstrait2->chaineDeCaracteres) + 1) * sizeof(char));
            strcpy(noeudArbreAbstrait1->chaineDeCaracteres, noeudArbreAbstrait2->chaineDeCaracteres);
         }

         noeudArbreAbstrait1->gauche = noeudArbreAbstrait2->gauche;

         noeudArbreAbstrait1->droite = noeudArbreAbstrait2->droite;

         val_retour = 1;
      }
   }

   return val_retour;
}


//Remet Ã  zero les informations d'un noeud (comme si il venait d'Ãªtre cree), val_retourne 1 si la remise Ã  zero s'est bien passee
int RemiseAZeroNoeudArbreAbstrait(NoeudArbreAbstrait *noeudArbreAbstraitARemettreAZero)
{
   int val_retour = 0;
   if(noeudArbreAbstraitARemettreAZero != NULL)
   {
      noeudArbreAbstraitARemettreAZero->nature = NON_DEFINI;

      noeudArbreAbstraitARemettreAZero->operation = AUTRE;
      
      noeudArbreAbstraitARemettreAZero->typeVariable = TYPE_VARIABLE_NON_DEFINI;

      if(noeudArbreAbstraitARemettreAZero->identificateurVariableOuFonction != NULL && strlen(noeudArbreAbstraitARemettreAZero->identificateurVariableOuFonction) > 0)
      {
         free(noeudArbreAbstraitARemettreAZero->identificateurVariableOuFonction);
         noeudArbreAbstraitARemettreAZero->identificateurVariableOuFonction = NULL;
      }

      noeudArbreAbstraitARemettreAZero->valeurNombre = 0;

      if(noeudArbreAbstraitARemettreAZero->chaineDeCaracteres != NULL && strlen(noeudArbreAbstraitARemettreAZero->chaineDeCaracteres) > 0)
      {
         free(noeudArbreAbstraitARemettreAZero->chaineDeCaracteres);
         noeudArbreAbstraitARemettreAZero->chaineDeCaracteres = NULL;
      }

      noeudArbreAbstraitARemettreAZero->typeBoucle = TYPE_BOUCLE_NON_DEFINI;

      noeudArbreAbstraitARemettreAZero->conditionExecutionBoucle = NULL;

      noeudArbreAbstraitARemettreAZero->listeArbresInstructionsBoucleConditionValide = NULL;
      noeudArbreAbstraitARemettreAZero->listeArbresInstructionsBoucleConditionNonValide = NULL;

      noeudArbreAbstraitARemettreAZero->gauche = NULL;
      noeudArbreAbstraitARemettreAZero->droite = NULL;

      val_retour = 1;
   }

   return val_retour;
}


//Renvoie 1 si l'arbre semble avoir ete ajoute correctement
int ajouterArbreListeArbresAbstraits(ListeArbresAbstraits *laListe, ArbreAbstrait *arbreAAjouter)
{
   int val_retour = 0;

   if(laListe != NULL)
   {
      if(laListe->tete != NULL)
      {
         ElementListeArbresAbstraits *elementParcoursListe = laListe->tete;

         while(elementParcoursListe->suivant != NULL)
         {
            elementParcoursListe = elementParcoursListe->suivant;
         }

         elementParcoursListe->suivant = nouvelElementListeArbresAbstraits();
         elementParcoursListe->suivant->arbre = arbreAAjouter;
         val_retour = 1;
      }
      else
      {
         laListe->tete = nouvelElementListeArbresAbstraits();
         laListe->tete->arbre = arbreAAjouter;
         val_retour = 1;
      }
   }

   return val_retour;
}

int detruireNoeudArbreAbstrait(NoeudArbreAbstrait* noeud)
{
   int val_retour = 1;
   if(noeud != NULL)
   {
      if(noeud->identificateurVariableOuFonction != NULL && strlen(noeud->identificateurVariableOuFonction) > 0)
      {
         free(noeud->identificateurVariableOuFonction);
      }
      if(noeud->chaineDeCaracteres != NULL && strlen(noeud->chaineDeCaracteres) > 0)
      {
         free(noeud->chaineDeCaracteres);
      }

      free(noeud);
      noeud = NULL;
   }
   else
   {
      val_retour = 0;
   }

   return val_retour;
}

int detruireNoeudArbreAbstraitEtSaDescendance(NoeudArbreAbstrait* noeud)
{
   int val_retour = 1;
   if(noeud != NULL)
   {
      if(noeud->gauche != NULL)
      {
         detruireNoeudArbreAbstraitEtSaDescendance(noeud->gauche);
      }
      if(noeud->droite != NULL)
      {
         detruireNoeudArbreAbstraitEtSaDescendance(noeud->droite);
      }
      if(noeud->listeArbresInstructionsBoucleConditionValide != NULL)
      {
         detruireListeArbresAbstraits(noeud->listeArbresInstructionsBoucleConditionValide);
      }
      if(noeud->listeArbresInstructionsBoucleConditionNonValide != NULL)
      {
         detruireListeArbresAbstraits(noeud->listeArbresInstructionsBoucleConditionNonValide);
      }

      val_retour = detruireNoeudArbreAbstrait(noeud);

      noeud = NULL;
   }
   else
   {
      val_retour = 0;
   }

   return val_retour;
}


int detruireArbreAbstrait(ArbreAbstrait *ArbreADetruire)
{
   int val_retour = 1;
   if(ArbreADetruire != NULL)
   {
      if(ArbreADetruire->tete != NULL)
      {
         val_retour = detruireNoeudArbreAbstraitEtSaDescendance(ArbreADetruire->tete);
      }

      free(ArbreADetruire);
      ArbreADetruire = NULL;
   }
   else
   {
   val_retour = 0;
   }

   return val_retour;
}



void detruireElementListeArbresAbstraitsEtSuivants(ElementListeArbresAbstraits *ElementListeArbresADetruire)
{
   if(ElementListeArbresADetruire != NULL)
   {
      if(ElementListeArbresADetruire->arbre != NULL)
      {
         detruireArbreAbstrait(ElementListeArbresADetruire->arbre);
      }
      if(ElementListeArbresADetruire->suivant != NULL)
      {
         detruireElementListeArbresAbstraitsEtSuivants(ElementListeArbresADetruire->suivant);
      }

      free(ElementListeArbresADetruire);
      ElementListeArbresADetruire = NULL;
   }
}

void detruireListeArbresAbstraits(ListeArbresAbstraits *ListeArbresADetruire)
{
   if(ListeArbresADetruire != NULL)
    {
         if(ListeArbresADetruire->tete != NULL)
         {
            detruireElementListeArbresAbstraitsEtSuivants(ListeArbresADetruire->tete);
         }

         free(ListeArbresADetruire);
         ListeArbresADetruire = NULL;
    }
}