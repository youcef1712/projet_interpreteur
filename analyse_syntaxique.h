#ifndef __AN_SYN__
#define __AN_SYN__

#include "lecture_caracteres.h"
#include "analyse_lexicale.h"
#include "arbresAbstraits.h"

typedef enum { 
      PAS_D_ERREUR_SYNTAXIQUE,
      ENTREE_VIDE,
      FICHIER_PAS_PU_ETRE_OUVERT,
      ERREUR_DE_SYNTAXE,
      ERREUR_DE_LEXEME,
      ERREUR_CAS_NON_GERE,
      ERREUR_POINTEUR_ARBRE,
      ERREUR_FONCTION,
      ERREUR_RETOUR_FONCTION_ARBRE_NUL,
      ERREUR_AJOUT_ARBRE_INSTRUCTION_DANS_LISTE,
      ERREUR_STRUCTURE_ARBRE_ABSTRAIT_DURANT_CONSTRUCTION
    } ERREUR_SYNTAXIQUE;

//Fonction construisant l'arbre d'une fichier contenant des instructions C#.
ERREUR_SYNTAXIQUE analyseSyntaxique(char *fichierCodeSource, ListeArbresAbstraits *listeArbresInstructionsPrincipales, int estFonctionAppeleePourListeInstructionsBoucleIfWhile, int debug);


//Fonction construit l arbre de l expression suivante

ERREUR_SYNTAXIQUE GetArbreExpression(NoeudArbreAbstrait *noeudArbreExpression, int estDansParenthese, int estDansDeclarationVariable, int estDansConditionBoucleIfWhile, Nature_Lexeme* natureDernierLexemeLu, int debug);


ERREUR_SYNTAXIQUE GetArbreConditionBoucle(NoeudArbreAbstrait *noeudArbreCondition, Nature_Lexeme* natureDernierLexemeLu, int debug);
#endif