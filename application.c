
/* Exemple d'utilisation de l'analyse lexicale : 
    Affichage d'une sequence de lexemes            */

#include <stdio.h>
#include <stdlib.h>

#include "analyse_syntaxique.h"
#include "arbresAbstraits.h"
#include "evaluation.h"

int main (int argc, char *argv[])
{
    // on demarre l'analyse lexicale sur le fichier transmis en argument
    if(argc == 2)
    {
        //L'application affiche les erreurs de debug si debug = 1;
        int debug = 1;

        char* nomFichierCodeSource = argv[1];

        //On a besoin d'une variable qui pointera vers les arbres abstraits de chaque instruction de code interpretee (une instruction = un element de la liste)
        ListeArbresAbstraits *listeArbresAbstraitsCodeSource = NULL;
        listeArbresAbstraitsCodeSource = nouvelleListeArbresAbstraits();

        ERREUR_SYNTAXIQUE erreurAnalyseSyntaxique = analyseSyntaxique(nomFichierCodeSource, listeArbresAbstraitsCodeSource, 0, debug);

        if(erreurAnalyseSyntaxique == PAS_D_ERREUR_SYNTAXIQUE)
        {
            //On evalue les instructions de notre liste d'arbres abstraits.
            ERREUR_SEMANTIQUE erreurEvaluation = evaluationListeArbresAbstraits(listeArbresAbstraitsCodeSource, debug);

            if(erreurEvaluation != PAS_D_ERREUR_SYNTAXIQUE)
                
            {
                printf("Il y a eu une erreur lors de l'interpretation des instructions de la liste d'arbres abstraits generee.\n");
            }
        }
        else
        {
            printf("Il y a eu une erreur lors de l'analyse syntaxique du fichier (construction de l'arbre).\n");
        }
        
        //On détruit l'arbre représentant la ligne de code
        detruireListeArbresAbstraits(listeArbresAbstraitsCodeSource);
        listeArbresAbstraitsCodeSource = NULL;
    }
    else
    {
        printf("Le nombre d'arguments est incorrect, il faut spécifier un et un seul nom de fichier a interpréter.\n");
    }

    return 0 ;
}