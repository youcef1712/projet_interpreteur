#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>

#include "evaluation.h"

   // vaut vrai ssi c designe un caractere separateur
   // C'est une amelioration de la fonction qui nous etait fournie dans analyse_lexicale
   int Separateur(char c)
   {
      int retour = 0;

      if(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\a' || c == '\b' || c == '\f')
      {
         retour = 1;
      }

      return retour;
   }

   // cette fonction ajoute le caractere c a la fin de la chaine s dont l'adresse du poiteur est passee en parame¨tre
//amelioration avec free
 
   void AjouterCaractere(char **s, char c)
   {
      if(*s != NULL)
      {
         int longueurString = strlen(*s);


         char *nouvelleChaine = malloc ((longueurString + 2) * sizeof(char));
         strcpy (nouvelleChaine, *s);
         nouvelleChaine[longueurString] = c;
         nouvelleChaine[longueurString + 1] = '\0';

         if(longueurString > 0)
         {
            free(*s);
         }
         *s = nouvelleChaine;
      }
   }

   //Cette fonction supprime les separateurs d'une chaine de caracteres dont le pointeur vers cette chaine est passee en parametre
   void SupprimerSeparateursChaineCaracteres(char **chaine)
{
    if (chaine != NULL)
    {
        if (*chaine != NULL)
        {
            int long_chaine = strlen(*chaine);

            if (long_chaine > 0)
            {
                int i = 0, tailleChaineSansSeparateurs = 0;
                char chaineSansSeparateursPasCompacte[10000];

                while (i < long_chaine)
                {
                    if (Separateur((*chaine)[i]) == 0)
                    {
                        chaineSansSeparateursPasCompacte[tailleChaineSansSeparateurs] = (*chaine)[i];
                        tailleChaineSansSeparateurs = tailleChaineSansSeparateurs + 1;
                    }
                    i = i + 1;
                }
                chaineSansSeparateursPasCompacte[tailleChaineSansSeparateurs] = '\0';

                // On va maintenant libérer la chaîne pointée par le pointeur de chaîne passé en paramètre
                // puis lui allouer la mémoire suffisante pour qu'il accueille la chaîne
                if (*chaine != NULL && strlen(*chaine) > 0)
                {
                    free(*chaine);
                    *chaine = NULL;
                }

                *chaine = malloc((strlen(chaineSansSeparateursPasCompacte) + 1) * sizeof(char));
                strcpy(*chaine, chaineSansSeparateursPasCompacte);
            }
        }
    }
}





   ERREUR_SEMANTIQUE evaluationArbreAbstrait(ArbreAbstrait *arbreAEvaluer, ListeVariables* listeVariablesProgramme, int debug);
   ERREUR_SEMANTIQUE evaluationArbreAbstraitRecurssif(NoeudArbreAbstrait *noeudArbreAEvaluer, ListeVariables* listeVariablesProgramme, int debug);

   ERREUR_SEMANTIQUE evaluationListeArbresAbstraitsListeVariablesExistante(ListeArbresAbstraits *listeArbresAEvaluer, ListeVariables* listeVariablesProgramme, int debug)
   {
      ERREUR_SEMANTIQUE retour = FAUTE_SEMANTIQUE;

      if(listeArbresAEvaluer != NULL)
      {
         if(listeArbresAEvaluer->tete != NULL)
         {
            if(listeVariablesProgramme != NULL)
            {
               ElementListeArbresAbstraits* elementParcoursListe = listeArbresAEvaluer->tete;

               while(elementParcoursListe != NULL && retour == FAUTE_SEMANTIQUE)
               {
                  retour = evaluationArbreAbstrait(elementParcoursListe->arbre, listeVariablesProgramme, debug);

                  elementParcoursListe = elementParcoursListe->suivant;
               }
            }
            else
            {
               retour = FAUTE_SEMANTIQUE_LISTE_VARIABLES_NULLE;
               printf("Erreur, la liste des variables du programme passe en parametre est nulle\n");
            }
         }
         else
         {
            retour = LISTE_ARBRES_A_EVALUER_VIDE;
            printf("Erreur, la liste des arbres a  evaluer est vide\n");
         }
      }
      else
      {
         retour = POINTEUR_LISTE_ARBRES_A_EVALUER_NUL;
         printf("Erreur, le poiteur vers la liste des arbres a  evaluer est nul\n");
      }

      return retour;
   }

   ERREUR_SEMANTIQUE evaluationListeArbresAbstraits(ListeArbresAbstraits *listeArbresAEvaluer, int debug)
   {
      ERREUR_SEMANTIQUE retour = FAUTE_SEMANTIQUE;

      if(listeArbresAEvaluer != NULL)
      {
         if(listeArbresAEvaluer->tete != NULL)
         {
            ElementListeArbresAbstraits* elementParcoursListe = listeArbresAEvaluer->tete;

            //On cree la liste des variables du programme
            ListeVariables* listeVariablesProgramme = NouvelleListeVariables();

            while(elementParcoursListe != NULL && retour == FAUTE_SEMANTIQUE)
            {
               retour = evaluationArbreAbstrait(elementParcoursListe->arbre, listeVariablesProgramme, debug);

               elementParcoursListe = elementParcoursListe->suivant;
            }

            //Une fois que le code a ete interprete, on detruit la liste des variables
            DetruireListeVariables(listeVariablesProgramme);
         }
         else
         {
            retour = LISTE_ARBRES_A_EVALUER_VIDE;
            printf("Erreur, la liste des arbres e  evaluer est vide\n");
         }
      }
      else
      {
         retour = POINTEUR_LISTE_ARBRES_A_EVALUER_NUL;
         printf("Erreur, le poiteur vers la liste des arbres e  evaluer est nul\n");
      }

      return retour;
   }

   ERREUR_SEMANTIQUE evaluationArbreAbstrait(ArbreAbstrait *arbreAEvaluer, ListeVariables* listeVariablesProgramme, int debug)
   {
      ERREUR_SEMANTIQUE retour;
      
      int arbreOK = 0;

      //On contre´le que l'arbre abstrait e  evaluer est correct
      if(arbreAEvaluer != NULL)
      {
         if(arbreAEvaluer->tete != NULL)
         {
            arbreOK = 1;
            retour = evaluationArbreAbstraitRecurssif(arbreAEvaluer->tete, listeVariablesProgramme, debug);
         }
      }

      if(arbreOK != 1)
      {
         printf("L'arbre abstrait devant contenir l'instruction est vide.\n");
         retour = ARBRE_ABSTRAIT_VIDE;
      }

      return retour;
   }

   ERREUR_SEMANTIQUE evaluationArbreAbstraitRecurssif(NoeudArbreAbstrait *noeudArbreAEvaluer, ListeVariables* listeVariablesProgramme, int debug)
   {
      ERREUR_SEMANTIQUE retour = FAUTE_SEMANTIQUE;

      if(noeudArbreAEvaluer == NULL)
      {
         retour = POINTEUR_NOEUD_ARBRE_NUL;
         printf("Erreur, le poiteur vers le noeud de l'arbre e  evaluer est nul.\n");
      }
      else
      {
         //Debut de l'evaluation        

         //Le sommet d'un arbre abstrait correct ne peut etre qu'un appel de fonction, une declaration de variable (auquel cas le fils gauche de la racine est soit une affectation, soit)
         if(noeudArbreAEvaluer->nature == FONCTION_)
         {
            //Si l'arbre instruction est un appel de fonction, la racine doit n'avoir qu'un fils gauche ou n'avoir aucun fils (si la fonction est Console.ReadLine)
            if(noeudArbreAEvaluer->gauche != NULL)
            {
               if(noeudArbreAEvaluer->droite == NULL)
               {
                  //Les seules fonctions disponibles pour le moment sont la fonction Console.WriteLine(), Console.ReadLine() et les fonctions de convertion entre types
                  //Seule la fonction Console.WriteLine a une utilite en debut d'instruction, Console.ReadLine() et les fonctions Convert.To n'etant utiles que de part leur valeur de retour
                  
                  //Si la fonction Console.WriteLine est detectee
                  if(strcmp(noeudArbreAEvaluer->identificateurVariableOuFonction, "Console.WriteLine") == 0)
                  {
                     //Soit le noeud de la fonction a un fils gauche et il doit representer la chaine de caracte¨res e  afficher, soit il n'en a pas et seul un retour e  la ligne est affiche
                     if(noeudArbreAEvaluer->gauche == NULL)
                     {
                        printf("\n");
                     }
                     //Si un parame¨tre est place dans le Console.WriteLine
                     else
                     {
                        //Lors de la construction de l'arbre, on doit avoir place e  gauche du noeud FONCTION ce qu'il y avait entre les parenthe¨ses de celle-ci
                        //La forme correcte d'un Console.WriteLine est: Console.WriteLine(terme1 + ... + termeN)
                        //Les termes doivent eªtre de type STRING ou convertits
                        //Si le parametre est null, une erreur est renvoyee

                        //Pour faire plus simple, j'ai cree une fonction GetStringArbre() qui retourne la chaine de caracte¨res correspondant e  ce qu'il y a d'ecrit dans l'arbre.
                        //Si il y a une erreur avec l'arbre ou que celui-ci ne correspond pas e  une chaine de caracte¨res, une erreur est renvoyee.

                        //On appelle donc notre fonction
                        char* affichConsole = NULL;
                        int estChaineAAfficherConsoleWriteLineNulle;
                        
                        retour = GetStringNoeudArbre(noeudArbreAEvaluer->gauche, listeVariablesProgramme, &affichConsole, &estChaineAAfficherConsoleWriteLineNulle, debug);

                        //Si il n'y a pas d'erreur et que la chaine n'est pas nulle, on l'affiche e  l'ecran et on place derrie¨re un retour e  la ligne
                        //Si la chaine est nulle, c'est une erreur
                        if(retour == FAUTE_SEMANTIQUE)
                        {
                           if(estChaineAAfficherConsoleWriteLineNulle == 0)
                           {
                              printf(affichConsole);
                              printf("\n");
                           }
                           else
                           {
                              retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                              printf("Erreur, la fonction Console.WriteLine est appelee, mais la chaine passee en parame¨tre est nulle.\n");
                           }
                        }

                        //On libe¨re maintenant la memoire de la chaine affichee
                        if(affichConsole != NULL && strlen(affichConsole) > 0)
                        {
                           free(affichConsole);
                           affichConsole = NULL;
                        }
                     }
                  }
                  else
                  {
                     //Si la fonction est une fonction Convert.To ou Console.ReadLine, il ne se passe rien de special pour l'utilisateur mais on effectue tout de meªme un appel de ces fonctions pour voir si il y avait un proble¨me avec les parame¨tres passes

                     if(strcmp(noeudArbreAEvaluer->identificateurVariableOuFonction, "Convert.ToString") == 0
                        || strcmp(noeudArbreAEvaluer->identificateurVariableOuFonction, "Convert.ToInt32") == 0
                        || strcmp(noeudArbreAEvaluer->identificateurVariableOuFonction, "Convert.ToDouble") == 0
                        || strcmp(noeudArbreAEvaluer->identificateurVariableOuFonction, "Console.ReadLine") == 0)
                     {
                        //Je me permet de creer une variable temporaire, et de lui affecter la valeur du Convert.To afin de savoir si les parame¨tres etaient corrects
                        //Je commence par chercher un nom unique e  la variable temporaire
                        char* nomVariableTemporaire = "temp";
                        int nomUniqueVariable = 0;
                        TypesVariables typeVariableTrouveDansListe = TYPE_VARIABLE_NON_DEFINI;

                        ERREUR_LISTE_VARIABLES erreurListe = PAS_D_ERREUR_LISTE_VARIABLES;

                        while(nomUniqueVariable == 0)
                        {
                           erreurListe = GetTypeVariableListeVariables(listeVariablesProgramme, nomVariableTemporaire, &typeVariableTrouveDansListe);

                           if(erreurListe == PAS_D_ERREUR_LISTE_VARIABLES)
                           {
                              //Si ce nom de variable est deje  pris, on ajoute au nom un caracte¨re aleatoire jusqu'e  que le nom soit unique
                              ajouter_caractere(&nomVariableTemporaire, (rand()%26) + 97);
                           }
                           else
                           {
                              if(erreurListe == VARIABLE_PAS_TROUVEE)
                              {
                                 nomUniqueVariable = 1;
                              }
                              else
                              {
                                 retour = FAUTE_SEMANTIQUE_LISTE_VARIABLES;
                              }
                           }
                        }


                        if(retour ==FAUTE_SEMANTIQUE)
                        {
                           //On declare la variable temporaire, son type est different en fonction du Convert.To
                           if(strcmp(noeudArbreAEvaluer->identificateurVariableOuFonction, "Convert.ToString") == 0
                              || strcmp(noeudArbreAEvaluer->identificateurVariableOuFonction, "Console.ReadLine") == 0)
                           {
                              erreurListe = DeclarerVariableSansValeurListeVariables(listeVariablesProgramme, nomVariableTemporaire, STRING);
                           }
                           else
                           {
                              if(strcmp(noeudArbreAEvaluer->identificateurVariableOuFonction, "Convert.ToInt32") == 0)
                              {
                                 erreurListe = DeclarerVariableSansValeurListeVariables(listeVariablesProgramme, nomVariableTemporaire, INT);
                              }
                              else
                              {
                                 if(strcmp(noeudArbreAEvaluer->identificateurVariableOuFonction, "Convert.ToDouble") == 0)
                                 {
                                    erreurListe = DeclarerVariableSansValeurListeVariables(listeVariablesProgramme, nomVariableTemporaire, DOUBLE);
                                 }
                              }
                           }

                           if(erreurListe == PAS_D_ERREUR_LISTE_VARIABLES)
                           {
                              //Je construit un arbre-instruction d'affectation de valeur de la fonction e  la variable temporaire
                              NoeudArbreAbstrait *noeudAffectationTestFonction = nouveauNoeudArbreAbstrait();
                              noeudAffectationTestFonction->nature = AFFECTATION;

                              noeudAffectationTestFonction->gauche = nouveauNoeudArbreAbstrait();
                              noeudAffectationTestFonction->gauche->nature = VARIABLE_;
                              noeudAffectationTestFonction->gauche->identificateurVariableOuFonction = malloc((strlen(nomVariableTemporaire) + 1) * sizeof(char));
                              strcpy(noeudAffectationTestFonction->gauche->identificateurVariableOuFonction, nomVariableTemporaire);

                              noeudAffectationTestFonction->droite = noeudArbreAEvaluer;

                              //On evalue maintenant l'arbre qu'on a construit en appelant notre fonction recursivement
                              retour = evaluationArbreAbstraitRecurssif(noeudAffectationTestFonction, listeVariablesProgramme, debug);


                              //Si il n'y a pas d'erreur, l'appel de la fonction etait correct.
                              if(retour == FAUTE_SEMANTIQUE)
                              {
                                 //On detruit alors l'arbre que l'on vient de creer, la variable temporaire et on passe e  l'arbre-instruction suivante
                                 //Destruction de la variable temporaire
                                 erreurListe = DetruireVariable(listeVariablesProgramme, nomVariableTemporaire);
                                 if(erreurListe != PAS_D_ERREUR_LISTE_VARIABLES)
                                 {
                                    printf("Erreur lors de la destruction de la variable temporaire.\n");
                                    retour = FAUTE_SEMANTIQUE_LISTE_VARIABLES;
                                 }
                                 else
                                 {
                                    //Je detruit la chaine du nom de la variable
                                    if(nomVariableTemporaire != NULL && strlen(nomVariableTemporaire) > 0)
                                    {
                                       free(nomVariableTemporaire);
                                       nomVariableTemporaire = NULL;
                                    }

                                    //Je detruit le noeud representant la variable
                                    int retourDestruction = detruireNoeudArbreAbstrait(noeudAffectationTestFonction->gauche);

                                    if(retourDestruction == 1)
                                    {
                                       //Et pour terminer, je detruit le noeud de l'affectation
                                       retourDestruction = detruireNoeudArbreAbstrait(noeudAffectationTestFonction);
                                    }
                                    
                                    if(retourDestruction == 0)
                                    {
                                       printf("Erreur lors de la destruction de l'arbre abstrait temporaire testant un appel de fonction.\n");
                                       retour = FAUTE_SEMANTIQUE_LISTE_VARIABLES;
                                    }
                                 }
                              }
                           }
                           else
                           {
                              printf("Erreur lors de la declaration de la variable temporaire.\n");
                              retour = FAUTE_SEMANTIQUE_LISTE_VARIABLES;
                           }
                        }
                     }
                     else
                     {
                        retour = FAUTE_SEMANTIQUE_FONCTION_INCONNUE;
                        printf("Erreur lors de l'evaluation de l'arbre, fonction inconnue.\n");
                     }
                  }
               }
               else
               {
                  printf("La structure de l'arbre est mauvaise, un appel de fonction est e  la racine, mais celle-ci a un fils droit, ce qui ne fait aucun sens.\n");
                  retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
               }
            }
            else
            {
               printf("La structure de l'arbre est mauvaise, un appel de fonction est e  la racine, mais celle-ci n'a aucun fils gauche (le fils gauche d'un appel de fonction representant normalement le parame¨tre de cette dernie¨re).\n");
               retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
            }
         }
         else
         {
            if(noeudArbreAEvaluer->nature == TYPE_VARIABLE_)
            {
               if(noeudArbreAEvaluer->gauche == NULL)
               {
                  printf("La structure de l'arbre est mauvaise, un noeud de declaration de variable d'un type est place mais il n'a aucun fils gauche.\n");
                  retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
               }
               else
               {
                  if(noeudArbreAEvaluer->droite != NULL)
                  {
                     printf("La structure de l'arbre est mauvaise, un noeud de declaration de variable d'un type est place mais il a un fils droit, ce qui ne fait aucun sens.\n");
                     retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                  }
                  else
                  {
                     //Si le fils gauche est directement une variable (accompagne de son identificateur), c'est une simple declaration sans affectation
                     if(noeudArbreAEvaluer->gauche->nature == VARIABLE_)
                     {
                        //Si ce noeud a un/des fils, c'est une erreur
                        if(noeudArbreAEvaluer->gauche->gauche != NULL || noeudArbreAEvaluer->gauche->droite != NULL)
                        {
                           printf("La structure de l'arbre est mauvaise, un noeud de declaration de variable d'un type est place mais le noeud representant la variable declaree a un/des fils, ce qui ne fait aucun sens.\n");
                           retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                        }
                        else
                        {
                           if(EstTypeDefini(noeudArbreAEvaluer->typeVariable) == 0)
                           {
                              printf("Erreur, une variable est declaree mais son type n'est pas un type defini.\n");
                              retour = ERREUR_INFORMATION_NOEUD_ARBRE_ABSTRAIT;
                           }
                           ERREUR_LISTE_VARIABLES retourDeclarationVariable = DeclarerVariableSansValeurListeVariables(listeVariablesProgramme, noeudArbreAEvaluer->gauche->identificateurVariableOuFonction, noeudArbreAEvaluer->typeVariable);

                           //Il peut y avoir une erreur car une variable du meeme nom avait ete declaree etc...
                           if(retourDeclarationVariable != PAS_D_ERREUR_LISTE_VARIABLES)
                           {
                              retour = FAUTE_SEMANTIQUE_LISTE_VARIABLES;
                              printf("Variable non-declaree e  cause d'une erreur: %s\n", noeudArbreAEvaluer->gauche->identificateurVariableOuFonction);
                           }
                           else
                           {
                              
                           }
                        
                        }
                     }
                     else
                     {
                        //Si le fils gauche est une affectation de valeur e  une variable, on declare la variable en lui affectant une valeur
                        if(noeudArbreAEvaluer->gauche->nature == AFFECTATION)
                        {
                           //Il faut que l'affectation ait obligatoirement deux fils (la variable et sa valeur), sinon c'est une erreur
                           if(noeudArbreAEvaluer->gauche->gauche == NULL || noeudArbreAEvaluer->gauche->droite == NULL)
                           {
                              printf("La structure de l'arbre est mauvaise, un noeud de declaration de variable d'un type avec affectation est place mais le noeud representant l'affectation n'a pas deux fils, ce qui ne fait aucun sens.\n");
                              retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                           }
                           else
                           {
                              //Si le fils gauche de l'affectation (la variable) a un/plusieurs fils, c'est une erreur
                              if(noeudArbreAEvaluer->gauche->gauche->gauche != NULL || noeudArbreAEvaluer->gauche->gauche->droite != NULL)
                              {
                                 printf("La structure de l'arbre est mauvaise, un noeud de declaration de variable d'un type avec affectation est place mais le noeud representant la variable e  un/deux fils, ce qui ne fait aucun sens.\n");
                                 retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                              }
                              else
                              {
                                 //Il faut que le type de la variable soit defini
                                 if(EstTypeDefini(noeudArbreAEvaluer->typeVariable) == 0)
                                 {
                                    printf("Erreur, une variable est declaree avec affectation de valeur mais son type n'est pas un type defini.\n");
                                    retour = ERREUR_INFORMATION_NOEUD_ARBRE_ABSTRAIT;
                                 }
                                 else
                                 {
                                    //Cas dans lequel le type est OK
                                    
                                    //On commencer par declarer notre variable comme plus haut, puis on va lui affecter une valeur en appelant recursivement notre fonction
                                    ERREUR_LISTE_VARIABLES retourDeclarationVariable = DeclarerVariableSansValeurListeVariables(listeVariablesProgramme, noeudArbreAEvaluer->gauche->gauche->identificateurVariableOuFonction, noeudArbreAEvaluer->typeVariable);
                                    
                                    //Il peut y avoir une erreur car une variable du meªme nom avait ete declaree etc...
                                    if(retourDeclarationVariable != PAS_D_ERREUR_LISTE_VARIABLES)
                                    {
                                       retour = FAUTE_SEMANTIQUE_LISTE_VARIABLES;
                                       printf("Variable non-declaree e  cause d'une erreur: %s\n", noeudArbreAEvaluer->gauche->identificateurVariableOuFonction);
                                    }
                                    else
                                    {
                                      
                                    }
                                    //Sinon, la variable a ete declaree sans erreur


                                    //On affecte maintenant la valeur e  la variable declaree en appelant recursivement notre fonction
                                    if(retour ==FAUTE_SEMANTIQUE)
                                    {
                                       evaluationArbreAbstraitRecurssif(noeudArbreAEvaluer->gauche, listeVariablesProgramme, debug);
                                    }
                                 }
                              }
                           }
                        }
                        //Sinon, la structure de l'arbre ne veut rien dire
                        else
                        {
                           printf("La structure de l'arbre est mauvaise, un noeud de declaration de variable d'un type est place mais le fils gauche n'est ni une variable, ni une affectation de valeur e  une variable.\n");
                           retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                        }
                     }
                  }
               }
            }
            else
            {
               //Si l'instruction est une affectation de valeur
               if(noeudArbreAEvaluer->nature == AFFECTATION)
               {
                  //Il faut que l'affectation ait obligatoirement deux fils (la variable et sa valeur), sinon c'est une erreur
                  if(noeudArbreAEvaluer->gauche == NULL || noeudArbreAEvaluer->droite == NULL)
                  {
                     printf("La structure de l'arbre est mauvaise, un noeud d'affectation est place mais le noeud representant l'affectation n'a pas deux fils, ce qui ne fait aucun sens.\n");
                     retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                  }
                  else
                  {
                     //Il faut bien que le fils gauche soit une variable et que son nom soit defini
                     if(noeudArbreAEvaluer->gauche->nature != VARIABLE_)
                     {
                        printf("La structure de l'arbre est mauvaise, un noeud d'affectation de variable est place mais le fils gauche de la racine n'est pas une variable, ce qui ne fait aucun sens.\n");
                        retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                     }
                     else
                     {
                        if(noeudArbreAEvaluer->gauche->identificateurVariableOuFonction == NULL)
                        {
                           printf("Les informations de l'arbre sont mauvaises, un noeud d'affectation de variable est place et le fils gauche est bien une variable, sauf que la chaine representant son nom est NULL.\n");
                           retour = ERREUR_INFORMATION_NOEUD_ARBRE_ABSTRAIT;
                        }
                        else
                        {
                           //Si le fils gauche de l'affectation (la variable) a un/plusieurs fils, c'est une erreur
                           if(noeudArbreAEvaluer->gauche->gauche != NULL || noeudArbreAEvaluer->gauche->droite != NULL)
                           {
                              printf("La structure de l'arbre est mauvaise, un noeud d'affectation de variable est place mais le noeud representant la variable e  un/deux fils, ce qui ne fait aucun sens.\n");
                              retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                           }
                           else
                           {
                              //Les informations de l'arbre semblent bonnes
                              //On va chercher les informations de la variable (si elle existe et son type)
                              TypesVariables typeVariableRecherchee = TYPE_VARIABLE_NON_DEFINI;
                              
                              ERREUR_LISTE_VARIABLES erreurUtilisationFonctionsListesVariables = GetTypeVariableListeVariables(
                                 listeVariablesProgramme, noeudArbreAEvaluer->gauche->identificateurVariableOuFonction, &typeVariableRecherchee
                              );
                              

                              //Si elle existe et qu'il n'y a pas d'erreur
                              if(erreurUtilisationFonctionsListesVariables == PAS_D_ERREUR_LISTE_VARIABLES)
                              {
                                 //Si son type n'est pas defini, erreur
                                 if(EstTypeDefini(typeVariableRecherchee) == 0)
                                 {
                                    printf("Erreur, une variable est declaree, on veut lui affecter une valeur mais son type n'est pas un type defini.\n");
                                    retour = FAUTE_SEMANTIQUE_TYPE_VARIABLE;
                                 }
                                 else
                                 {
                                    //En fonction du type de variable, la fae§on avec laquelle on ira chercher la valeur affectee sera differente
                                    if(typeVariableRecherchee == INT || typeVariableRecherchee == DOUBLE)
                                    {
                                       double valeurAffectee;
                                       retour = GetValeurDoubleNoeudArbre(noeudArbreAEvaluer->droite, listeVariablesProgramme, &valeurAffectee, debug);

                                       //La valeur dans l'arbre est correcte
                                       if(retour == FAUTE_SEMANTIQUE)
                                       {
                                          //On appelle notre fonction d'affectation de valeur e  un variable
                                          ERREUR_LISTE_VARIABLES retourDeclarationVariable = ChangerValeurVariableListeVariables(
                                             listeVariablesProgramme, noeudArbreAEvaluer->gauche->identificateurVariableOuFonction,
                                             1, 0,
                                             valeurAffectee, NULL
                                          );

                                          //Il peut tout de meªme y avoir une erreur, on ne sait jamais
                                          if(retourDeclarationVariable != PAS_D_ERREUR_LISTE_VARIABLES)
                                          {
                                             retour = FAUTE_SEMANTIQUE_LISTE_VARIABLES;
                                          }
                                          //Sinon, la variable a ete declaree sans erreur
                                       }
                                    }
                                    else
                                    {
                                       if(typeVariableRecherchee == STRING)
                                       {
                                          char* chaineAffectee = NULL;
                                          int estChaineAffecteeNulle;
                                          retour = GetStringNoeudArbre(noeudArbreAEvaluer->droite, listeVariablesProgramme, &chaineAffectee, &estChaineAffecteeNulle, debug);

                                          //La valeur dans l'arbre est correcte
                                          if(retour == FAUTE_SEMANTIQUE)
                                          {
                                             //On appelle notre fonction d'affectation de valeur e  un variable
                                             ERREUR_LISTE_VARIABLES retourDeclarationVariable = ChangerValeurVariableListeVariables(
                                                listeVariablesProgramme, noeudArbreAEvaluer->gauche->identificateurVariableOuFonction,
                                                0, estChaineAffecteeNulle,
                                                0, chaineAffectee
                                             );

                                             //Il peut tout de meªme y avoir une erreur, on ne sait jamais
                                             if(retourDeclarationVariable != PAS_D_ERREUR_LISTE_VARIABLES)
                                             {
                                                retour = FAUTE_SEMANTIQUE_LISTE_VARIABLES;
                                             }
                                             //Sinon, la variable a ete declaree sans erreur
                                          }

                                          if(chaineAffectee != NULL && strlen(chaineAffectee) > 0)
                                          {
                                             free(chaineAffectee);
                                             chaineAffectee = NULL;
                                          }
                                       }
                                    }
                                 }
                              }
                              else
                              {
                                 retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                              }
                           }
                        }
                     }
                  }
               }
               else
               {
                  //Si l'instruction est une boucle (if ou tant que)
                  if(noeudArbreAEvaluer->nature == BOUCLE)
                  {
                     //Le type de boucle doit eªtre defini
                     if(noeudArbreAEvaluer->typeBoucle == IF || noeudArbreAEvaluer->typeBoucle == WHILE)
                     {
                        //Il faut que l'affectation ait obligatoirement une liste d'instructions dans le cas oe¹ la condition est varlidee (que ce soit un if ou un while)
                        if(noeudArbreAEvaluer->listeArbresInstructionsBoucleConditionValide == NULL)
                        {
                           printf("La structure de l'arbre est mauvaise, un noeud de boucle est place mais la liste des instructions e  effectuer si la condiction est validee est nulle.\n");
                           retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                        }
                        else
                        {
                           //La boucle doit avoir un noeud representant une condition d'execution
                           if(noeudArbreAEvaluer->conditionExecutionBoucle == NULL)
                           {
                              printf("La structure de l'arbre est mauvaise, un noeud de boucle est place mais le noeud devant representer la condition d'execution de cette dernie¨re est null.\n");
                              retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                           }
                           else
                           {
                              if(noeudArbreAEvaluer->conditionExecutionBoucle->nature != OPERATION)
                              {
                                 printf("La structure de l'arbre est mauvaise, un noeud de boucle est place mais le noeud devant representer la condition d'execution de cette dernie¨re n'a pas comme racine une operation.\n");
                                 retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                              }
                              else
                              {
                                 if(noeudArbreAEvaluer->conditionExecutionBoucle->gauche == NULL || noeudArbreAEvaluer->conditionExecutionBoucle->droite == NULL)
                                 {
                                    printf("La structure de l'arbre est mauvaise, un noeud de boucle est place mais le noeud devant representer la condition d'execution de cette dernie¨re n'a pas deux fils representant les deux parties de la condition.\n");
                                    retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                                 }
                                 else
                                 {
                                    if(noeudArbreAEvaluer->typeBoucle == IF)
                                    {
                                       //On va regarder si il  la condition est verifiee
                                       int estConditionVerifiee;

                                       retour = getValeurArbreConditionBoucle(noeudArbreAEvaluer->conditionExecutionBoucle, listeVariablesProgramme, &estConditionVerifiee, debug);

                                       if(retour == FAUTE_SEMANTIQUE)
                                       {
                                          //Si la condition d'egalite est respectee, on execute le code du if, sinon on execute le code du else
                                          if(estConditionVerifiee == 1)
                                          {
                                             evaluationListeArbresAbstraitsListeVariablesExistante(noeudArbreAEvaluer->listeArbresInstructionsBoucleConditionValide, listeVariablesProgramme, debug);
                                          }
                                          else
                                          {
                                             if(noeudArbreAEvaluer->listeArbresInstructionsBoucleConditionNonValide != NULL)
                                             {
                                                evaluationListeArbresAbstraitsListeVariablesExistante(noeudArbreAEvaluer->listeArbresInstructionsBoucleConditionNonValide, listeVariablesProgramme, debug);
                                             }
                                          }
                                       }
                                    }
                                    else
                                    {
                                       if(noeudArbreAEvaluer->typeBoucle == WHILE)
                                       {
                                          //Un noeud while ne doit avoir qu'un fils gauche
                                          if(noeudArbreAEvaluer->listeArbresInstructionsBoucleConditionNonValide != NULL)
                                          {
                                             printf("La structure de l'arbre est mauvaise, un noeud de boucle while est place mais la liste des instructions e  effectuer si la condiction n'est pas validee n'est pas nulle, ce qui ne fait aucun sens.\n");
                                             retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                                          }
                                          else
                                          {
                                             int sortBoucleWhile = 0;

                                             while(sortBoucleWhile == 0 && retour == FAUTE_SEMANTIQUE)
                                             {
                                                int estConditionVerifiee;

                                                retour = getValeurArbreConditionBoucle(noeudArbreAEvaluer->conditionExecutionBoucle, listeVariablesProgramme, &estConditionVerifiee, debug);

                                                if(retour == FAUTE_SEMANTIQUE)
                                                {
                                                   //Si la condition du while est respectee, on execute le code puis on re-regarde si la condition est verifiee
                                                   if(estConditionVerifiee == 1)
                                                   {
                                                      evaluationListeArbresAbstraitsListeVariablesExistante(noeudArbreAEvaluer->listeArbresInstructionsBoucleConditionValide, listeVariablesProgramme, debug);
                                                   }
                                                   else
                                                   {
                                                      sortBoucleWhile = 1;
                                                   }
                                                }
                                             }
                                          }
                                       }
                                       else
                                       {
                                          printf("Erreur, un noeud de l'arbre est une boucle mais l'operateur de condition d'execution de la boucle n'est pas valide (seul le double egal est valide). Operateur: %d\n", noeudArbreAEvaluer->conditionExecutionBoucle->operation);
                                          retour = ERREUR_INFORMATION_NOEUD_ARBRE_ABSTRAIT;
                                       }
                                    }
                                 }
                              }
                           }
                        }
                     }
                     else
                     {
                        printf("Erreur, un noeud de l'arbre est une boucle mais le type de la boucle n'est pas correct ou indefini.\n");
                        retour = ERREUR_INFORMATION_NOEUD_ARBRE_ABSTRAIT;
                     }
                  }
                  //Ne doit en theorie jamais arriver
                  else
                  {
                     printf("Nature de noeud abstrait inconnue.\n");
                     retour = NATURE_NOEUD_ARBRE_ABSTRAIT_INCONNUE;
                  }
               }
            }
         }
      }

      return retour;
   }

   //Cette fonction n'est pas e  appeler directement, elle est appelee par GetValeurDoubleNoeudArbre
   ERREUR_SEMANTIQUE GetValeurDoubleNoeudArbreRecurssif(NoeudArbreAbstrait *noeudValeur, ListeVariables* listeVariablesProgramme, double* valeurDoubleRetour, int debug)
   {
      ERREUR_SEMANTIQUE retour = FAUTE_SEMANTIQUE;

      //On regarde ce qu'est la valeur du noeud
      //Le noeud ne peut eªtre qu'une operation (+, -, *, /), une valeur (int ou double), une variable (contenant une valeur int ou double), ou une fonction retournant une valeur (int ou double)
      

      if(noeudValeur->nature == NOMBRE_ || noeudValeur->nature == VARIABLE_)
      {
         if(noeudValeur->gauche != NULL || noeudValeur->droite != NULL)
         {
            printf("Erreur, la structure de l'arbre representant une valeur (int ou double) est mauvaise. La racine est une valeur (int ou double) ou une variable pouvant eªtre une valeur (int ou double) mais elle a un/des fils.\n");
            retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
         }
         else
         {
            //Une variable int ou double peut avoir une valeur ou eªtre nulle
            if(noeudValeur->nature == VARIABLE_)
            {
               if(noeudValeur->identificateurVariableOuFonction != NULL)
               {
                  TypesVariables typeVariable;
                  int valeurVariablePasAffectee;
                  int valeurIntVariable;
                  double valeurDoubleVariable;
                  //Ne sert e  rien dans notre cas (une variable int ou double ne peut pas eªtre nulle, elle peut juste ne pas avoir de valeur affectee), c'est juste pour respecter les parame¨tres de la fonction
                  int valeurNulle;
                  char* valeurChaine = NULL;

                  //Cette fonction retourne la valeur de la variable dont le nom est specifie, si elle existe  
                  ERREUR_LISTE_VARIABLES retourGetVariable = GetTypeEtValeurVariableListeVariables(listeVariablesProgramme, noeudValeur->identificateurVariableOuFonction, &typeVariable, &valeurVariablePasAffectee, &valeurNulle, &valeurIntVariable, &valeurDoubleVariable, &valeurChaine);
                  if(valeurChaine != NULL && strlen(valeurChaine) > 0)
                  {
                     free(valeurChaine);
                     valeurChaine = NULL;
                  }
                  
                  if(retourGetVariable != PAS_D_ERREUR_LISTE_VARIABLES)
                  {
                     if(retourGetVariable == VARIABLE_PAS_TROUVEE)
                     {
                        printf("Erreur, un identificateur de variable est mentionne mais elle n'est jamais declaree.\n");
                        retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                     }
                  }
                  else
                  {
                     if(valeurVariablePasAffectee == 1)
                     {
                        printf("Erreur, instruction tentant d'acceder e  la valeur d'une variable non-assignee.\n");
                        retour = FAUTE_SEMANTIQUE_TENTE_ACCES_VALEUR_VARIABLE_NON_ASSIGNEE;
                     }
                     else
                     {
                        if(typeVariable == INT)
                        {
                           *valeurDoubleRetour = valeurIntVariable;
                        }
                        else
                        {
                           if(typeVariable == DOUBLE)
                           {
                              *valeurDoubleRetour = valeurDoubleVariable;
                           }
                           else
                           {
                              retour = FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE;
                           }
                        }
                     }
                  }
               }
               else
               {
                  printf("Erreur, un noeud dont la nature est une variable voit son identificateur e  NULL.\n");
                  retour = ERREUR_INFORMATION_NOEUD_ARBRE_ABSTRAIT;
               }
            }
            else
            {
               if(noeudValeur->nature == NOMBRE_)
               {
                  *valeurDoubleRetour = noeudValeur->valeurNombre;
               }
            }
         }
      }
      else
      {
         //Si null apparait dans une expression arithmetique, c'est une erreur
         if(noeudValeur->nature == VALEUR_NULLE_)
         {
            //printf("Erreur, apparition de la valeur null dans une expression arithmetique.\n");
            retour = FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE;
         }
         else
         {

            if(noeudValeur->nature == OPERATION)
            {
               if(noeudValeur->operation == ADDITION || noeudValeur->operation == SOUSTRACTION
                  || noeudValeur->operation == MULTIPLICATION || noeudValeur->operation == DIVISION)
               {
                  if(noeudValeur->gauche == NULL)
                  {
                     printf("Erreur, la structure de l'arbre representant une operation est mauvaise. La racine est un operateur mais elle n'a pas de fils gauche.\n");
                     retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                  }
                  else
                  {
                     if(noeudValeur->droite == NULL)
                     {
                        if(noeudValeur->operation != ADDITION && noeudValeur->operation != SOUSTRACTION)
                        {
                           printf("Erreur, la structure de l'arbre representant une operation est mauvaise. La racine est un operateur, elle n'a qu'un unique fils gauche mais l'operateur n'est pas un operateur de signe.\n");
                           retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                        }

                        //Si le noeud n'a qu'un fils gauche et que l'operateur est un operateur de signe (+ ou -)
                        //-Si l'operateur est +: valeurDoubleRetour = valeurDoubleRetour * 1
                        //*Si l'operateur est -: valeurDoubleRetour = valeurDoubleRetour * -1
                        else
                        {
                           retour = GetValeurDoubleNoeudArbreRecurssif(noeudValeur->gauche, listeVariablesProgramme, valeurDoubleRetour, debug);

                           if(retour == FAUTE_SEMANTIQUE)
                           {
                              if(noeudValeur->operation == SOUSTRACTION)
                              {
                                 *valeurDoubleRetour = *valeurDoubleRetour * -1;
                              }
                              //else = if(noeudValeur->operation == ADDITION), on ne fait rien
                           }
                        }
                     }
                     else
                     {
                        double valeurDoubleOperandeGauche;

                        retour = GetValeurDoubleNoeudArbreRecurssif(noeudValeur->gauche, listeVariablesProgramme, &valeurDoubleOperandeGauche, debug);

                        if(retour == FAUTE_SEMANTIQUE)
                        {
                           double valeurDoubleOperandeDroite;

                           retour = GetValeurDoubleNoeudArbreRecurssif(noeudValeur->droite, listeVariablesProgramme, &valeurDoubleOperandeDroite, debug);

                           if(retour == FAUTE_SEMANTIQUE)
                           {
                              if(noeudValeur->operation == ADDITION)
                              {
                                 *valeurDoubleRetour = valeurDoubleOperandeGauche + valeurDoubleOperandeDroite;
                              }
                              else
                              {
                                 if(noeudValeur->operation == SOUSTRACTION)
                                 {
                                    *valeurDoubleRetour = valeurDoubleOperandeGauche - valeurDoubleOperandeDroite;
                                 }
                                 else
                                 {
                                    if(noeudValeur->operation == MULTIPLICATION)
                                    {
                                       *valeurDoubleRetour = valeurDoubleOperandeGauche * (valeurDoubleOperandeDroite);
                                    }
                                    else
                                    {
                                       if(noeudValeur->operation == DIVISION)
                                       {
                                          if(valeurDoubleOperandeDroite == 0)
                                          {
                                             retour = FAUTE_SEMANTIQUE_DIVISION_PAR_ZERO;
                                             printf("Erreur, division avec le second operande egal e  zero.\n");
                                          }
                                          else
                                          {
                                             *valeurDoubleRetour = valeurDoubleOperandeGauche / (valeurDoubleOperandeDroite);
                                          }
                                       }
                                       else
                                       {
                                          printf("Erreur, l'arbre representant une operation est mauvaise. La racine est un operateur, mais ce dernier est un operateur inconnu.\n");
                                          retour = ERREUR_INFORMATION_NOEUD_ARBRE_ABSTRAIT;
                                       }
                                    }
                                 }
                              }
                           }
                        }
                     }
                  }
               }
               else
               {
                  printf("Erreur, la structure de l'arbre representant une operation mathematique est mauvaise. Un operateur est lu mais ce n'est ni le +, ni le -, ni le *, ni le /.\n");
                  retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
               }
            }
            else
            {
               //Si c'est une fonction
               if(noeudValeur->nature == FONCTION_)
               {
                  //Les seules fonctions pouvant apparaitre e  cet endroit du code sont les fonctions retournant une valeur (un nombre double ou int).
                  //Dans notre cas, seule les fonctions Convert.ToInt32() et Convert.ToDouble() sont interpretees
                  if(strcmp(noeudValeur->identificateurVariableOuFonction, "Convert.ToInt32") == 0
                     || strcmp(noeudValeur->identificateurVariableOuFonction, "Convert.ToDouble") == 0)
                  {
                     //Ces deux fonctions prennent un parame¨tre.
                     //Il faut que notre noeud ait obligatoirement un fils gauche (dont le sous-arbre doit representer ce qu'il y a entre les parenthe¨ses de la fonction) et pas de fils droit
                     if(noeudValeur->gauche == NULL)
                     {
                        printf("un noeud representant un appel de la fonction \"%s\" est lu, mais il n'a aucun fils gauche (le fils gauche representant le parame¨tre entre les parenthe¨ses de l'appel de la fonction).", noeudValeur->identificateurVariableOuFonction);
                        retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                     }
                     else
                     {
                        if(noeudValeur->droite != NULL)
                        {
                           printf("un noeud representant un appel de la fonction \"%s\" est lu, mais il a un fils droit, ce qui n'est pas logique car normalement seul un fils gauche doit eªtre present (le fils gauche representant le parame¨tre entre les parenthe¨ses de l'appel de la fonction).", noeudValeur->identificateurVariableOuFonction);
                           retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                        }
                        //Si la structure de l'arbre est correcte
                        else
                        {
                           //Ici, nous ne gerons que les deux cas d'appel de la fonction suivant:
                           //-Le parame¨tre passe est une expression arithmetique
                           //-Le parame¨tre passe est une concatenation de chaines de caracte¨res (dans ce cas le , le retour du Convert.toString est simplement la concatenation sans modifications)

                           //On va donc appeler successivement les fonctions GetStringNoeudArbre et GetValeurDoubleNoeudArbre
                           //sur le fils gauche du noeud (le  ou est range le parame¨tre entre les parenthe¨ses de Convert.ToInt32/ToDouble)
                           //pour savoir si ce qu'il y a entre les parenthe¨se est une chaine de caracte¨res 
                           //ou une valeur (int ou double) correcte

                           
                           //On commence par regarder si le parame¨tre est une une expression arithmetique correcte
                           retour = GetValeurDoubleNoeudArbreRecurssif(noeudValeur->gauche, listeVariablesProgramme, valeurDoubleRetour, debug);
                           //Si il n'y a pas d'erreur on ne regarde pas si le parame¨tre est une chaine de caracte¨res
                           if(retour == FAUTE_SEMANTIQUE)
                           {
                              //Si la fonction appelee est Convert.ToInt32, on va transformer notre resultat en entier
                              if(strcmp(noeudValeur->identificateurVariableOuFonction, "Convert.ToInt32") == 0)
                              {
                                 int resultatEntier = *valeurDoubleRetour;
                                 *valeurDoubleRetour = resultatEntier;
                              }
                           }
                           //Sinon, on regarde si l'erreur est une erreur provoquee par le type du contenu de l'arbre (si c'etait car l'arbre ne representait pas une expression arithmetique).
                           else
                           {
                              //Si c'en etait bien une erreur de ce type, on regarde si le parame¨tre est une expression arithmetique
                              if(retour == FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE)
                              {
                                 char* chaineParametre = NULL;
                                 int estValeurParametreNulle;
                                 retour = GetStringNoeudArbre(noeudValeur->gauche, listeVariablesProgramme, &chaineParametre, &estValeurParametreNulle, debug);
                                 
                                 //Si il n'y a pas d'erreur on ne regarde pas si le parame¨tre est une expression arithmetique
                                 if(retour == FAUTE_SEMANTIQUE)
                                 {
                                    //Si la valeur de la chaine est nulle, la valeur devient zero
                                    if(estValeurParametreNulle == 1)
                                    {
                                       *valeurDoubleRetour = 0;
                                    }
                                    else
                                    {
                                       //Sinon, on va regarder la structure de la chaine de caracte¨res, il faut qu'elle ne soit composee que d'un nombre (les separateurs (espaces, \n...) ne provoquent pas d'erreur, on se contente de les ignorer)
   
                                       
                                       char* chaineSecondParametreCeQuIlYADerriereLeNombreDansLaChaineSource;

                                       //Je supprime les separateurs de la chaine source
                                       SupprimerSeparateursChaineCaracteres(&chaineParametre);

                                       *valeurDoubleRetour = strtod(chaineParametre, &chaineSecondParametreCeQuIlYADerriereLeNombreDansLaChaineSource);
                                       
                                       //taille de la chaine du second parame¨tre
                                       int tailleChaineSecondParametre = strlen(chaineSecondParametreCeQuIlYADerriereLeNombreDansLaChaineSource);

                                       //Si la chaine source etait correcte
                                       if(tailleChaineSecondParametre == 0)
                                       {
                                          //Si la fonction appelee est Convert.ToInt32, on va transformer notre resultat en entier
                                          if(strcmp(noeudValeur->identificateurVariableOuFonction, "Convert.ToInt32") == 0)
                                          {
                                             int resultatEntier = *valeurDoubleRetour;
                                             *valeurDoubleRetour = resultatEntier;
                                          }
                                       }
                                       //Sinon, on renvoie une erreur
                                       else
                                       {
                                          printf("Un noeud representant un appel de la fonction \"%s\" est lu, ce qui est passe en parame¨tre est bien une chaine de caracte¨res mais celle-ci ne represente pas une valeur (int ou double) correcte.\n", noeudValeur->identificateurVariableOuFonction);
                                          retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                                       }
                                    }
                                 }
                                 else
                                 {
                                    printf("un noeud representant un appel de la fonction \"%s\" est lu, mais ce qui est passe en parame¨tre n'est ni une valeur (int ou double), ni une chaine de caracte¨res (pouvant representer une valeur), ni une valeur nulle.", noeudValeur->identificateurVariableOuFonction);
                                    retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                                 }
                              }
                              //Sinon, 
                           }
                        }
                     }
                  }
                  else
                  {
                     //Si le retour de la fonction n'est juste pas de bon type
                     if(strcmp(noeudValeur->identificateurVariableOuFonction, "Console.ReadLine") == 0
                        || strcmp(noeudValeur->identificateurVariableOuFonction, "Convert.ToString") == 0)
                     {
                        retour = FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE;
                     }
                     //Si la fonction n'a vraiment aucune raison d'eªtre appelee ici
                     else
                     {
                        printf("Erreur, une fonction ne revoyant pas de valeur apparait dans un arbre devant representer une valeur (int ou double).\n");
                        retour = FAUTE_SEMANTIQUE_CAS_PAS_GERE;
                     }
                  }
               }
               //Sinon, le type de noeud n'est pas logique
               else
               {
                  if(noeudValeur->nature == CHAINE_CARACTERES_)
                  {
                     retour = FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE;
                  }
                  else
                  {
                     printf("Erreur, un noeud qui n'est ni une valeur (int ou double), ni une variable, ni une fonction apparait dans un arbre devant representer une operation. Type noeud: %d\n", noeudValeur->nature);
                     retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
                  }
               }
            }
         }
      }

      return retour;
   }


   //Cette fonction retourne la valeur en DOUBLE correspondant aux operations ecrites dans l'arbre.

   ERREUR_SEMANTIQUE GetValeurDoubleNoeudArbre(NoeudArbreAbstrait *noeudValeur, ListeVariables* listeVariablesProgramme, double* valeurDoubleRetour, int debug)
   {
      ERREUR_SEMANTIQUE retour = FAUTE_SEMANTIQUE;

      //On controle si le noeud et la liste des variables ne sont pas nuls
      if(noeudValeur == NULL)
      {
         printf("Erreur, le noeud d'arbre abstrait qui devait eªtre interprete pour calculer la valeur qu'il represente est nul.\n");
         retour = POINTEUR_NOEUD_ARBRE_NUL;
      }
      else
      {
         if(listeVariablesProgramme == NULL)
         {
            printf("Erreur, le pointeur de la liste des variables est nul.\n");
            retour = POINTEUR_LISTE_VARIABLES_NUL;
         }
         //Si tout va bien
         else
         {
            //On affecte la valeur 0 au resultat par defaut
            *valeurDoubleRetour = 0;

            //On appelle notre fonction recurssive
            retour = GetValeurDoubleNoeudArbreRecurssif(noeudValeur, listeVariablesProgramme, valeurDoubleRetour, debug);

            if(retour == FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE)
            {
               printf("Erreur, une variable/valeur est placee dans une concatenation de chaine de caracte¨res mais ce n'est pas une valeur int/double.\n");
            }
         }
      }

      return retour;
   }


   //Cette fonction n'est pas e  appeler directement, elle est appelee par GetStringNoeudArbre
   //Le pointeur pointe par pointeurChaineRetour doit forcement eªtre initialise e  NULL sinon erreur: *pointeurChaineRetour = NULL
   ERREUR_SEMANTIQUE GetStringNoeudArbreRecurssif(NoeudArbreAbstrait *noeudString, ListeVariables* listeVariablesProgramme, char** pointeurChaineRetour, int* estChaineNulle, int debug)
   {
      ERREUR_SEMANTIQUE retour = FAUTE_SEMANTIQUE;
      *estChaineNulle = 0;

      //On regarde ce qu'est la valeur du noeud
      //Le noeud ne peut eªtre qu'un '+' (concatenation), une chaine de caracte¨res (dans une variable ou non), ou une fonction retournant une chaine de caracte¨res
      
      //Si c'est une chaine de caracte¨res, on regarde si elle a un fils droit ou gauche
      //->Si elle n'en a aucun, pas d'erreur, le resultat est la valeur de cette chaine
      //->Si elle en a au moins un, erreur
      if(noeudString->nature == CHAINE_CARACTERES_ || noeudString->nature == VARIABLE_ || noeudString->nature == VALEUR_NULLE_)
      {
         if(noeudString->gauche != NULL || noeudString->droite != NULL)
         {
            printf("Erreur, la structure de l'arbre representant une chaine de caracte¨res est mauvaise. La racine est une chaine de caracte¨res ou une variable pouvant eªtre une chaine de caracte¨res mais elle a un/des fils.\n");
            retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
         }
         else
         {
            //Il est e  savoir qu'en C#, une variable chaine de caracte¨res NULL est interpretee comme une chaine de caracte¨res vide
            if(noeudString->nature == VARIABLE_)
            {
               if(noeudString->identificateurVariableOuFonction != NULL)
               {
                  TypesVariables typeVariable;
                  int valeurVariablePasAssignee;

                  //Ne servent e  rien, c'est juste pour respecter les parame¨tres de la fonction
                  int valeurIntVariable;
                  double valeurDoubleVariable;

                  //Cette fonction effectue une copie de la chaine de la variable si elle existe
                  ERREUR_LISTE_VARIABLES retourGetVariable = GetTypeEtValeurVariableListeVariables(listeVariablesProgramme, noeudString->identificateurVariableOuFonction, &typeVariable, &valeurVariablePasAssignee, estChaineNulle, &valeurIntVariable, &valeurDoubleVariable, pointeurChaineRetour);
                  if(retourGetVariable != PAS_D_ERREUR_LISTE_VARIABLES)
                  {
                     if(retourGetVariable == VARIABLE_PAS_TROUVEE)
                     {
                        printf("Erreur, un identificateur de variable est mentionne mais elle n'est jamais declaree.\n");
                        retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                     }
                  }
                  else
                  {
                     if(typeVariable == STRING)
                     {
                        if(valeurVariablePasAssignee == 1)
                        {
                           printf("Erreur, instruction tentant d'acceder e  la valeur d'une variable non-assignee.\n");
                           retour = FAUTE_SEMANTIQUE_TENTE_ACCES_VALEUR_VARIABLE_NON_ASSIGNEE;
                        }
                        else
                        {
                           if(*estChaineNulle == 1)
                           {
                              if(*pointeurChaineRetour != NULL && strlen(*pointeurChaineRetour) > 0)
                              {
                                 free(*pointeurChaineRetour);
                                 *pointeurChaineRetour = NULL;
                              }
                           }
                        }
                     }
                     else
                     {
                        retour = FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE;
                     }
                  }
               }
               else
               {
                  printf("Erreur, un noeud dont la nature est une variable voit son identificateur e  NULL.\n");
                  retour = ERREUR_INFORMATION_NOEUD_ARBRE_ABSTRAIT;
               }
            }
            else
            {
               if(noeudString->nature == CHAINE_CARACTERES_)
               {
                  if(noeudString->chaineDeCaracteres == NULL)
                  {
                     printf("Erreur, un noeud chaine de caracte¨res est placee dans une concatenation de chaine de caracte¨res mais la chaine de caracte¨res est nulle.\n");
                     retour = ERREUR_INFORMATION_NOEUD_ARBRE_ABSTRAIT;
                  }
                  else
                  {
                     *estChaineNulle = 0;
                     if(*pointeurChaineRetour != NULL && strlen(*pointeurChaineRetour) > 0)
                     {
                        free(*pointeurChaineRetour);
                        *pointeurChaineRetour = NULL;
                     }
                     *pointeurChaineRetour = malloc((strlen(noeudString->chaineDeCaracteres) + 1) * sizeof(char));
                     strcpy(*pointeurChaineRetour, noeudString->chaineDeCaracteres);
                  }
               }
               else
               {
                  if(noeudString->nature == VALEUR_NULLE_)
                  {
                     if(*pointeurChaineRetour != NULL && strlen(*pointeurChaineRetour) > 0)
                     {
                        free(*pointeurChaineRetour);
                        *pointeurChaineRetour = NULL;
                     }
                     *estChaineNulle = 1;
                  }
               }
            }
         }
      }
      else
      {
         //Si c'est une concatenation, on regarde si elle a un fils droit ou gauche
         //->Si elle en a deux, pas d'erreur, le resultat est la concatenation des deux fils
         //->Sinon, erreur
         if(noeudString->nature == OPERATION)
         {
            if(noeudString->operation == ADDITION)
            {
               if(noeudString->gauche == NULL || noeudString->droite == NULL)
               {
                  printf("Erreur, la structure de l'arbre representant une chaine de caracte¨res est mauvaise. La racine est l'operateur de concatenation mais elle n'a pas deux fils e  concatener.\n");
                  retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
               }
               else
               {
                  char* premierePartieConcatenation = NULL;
                  int estChainePremierePartieConcatenationNulle;
                  
                  retour = GetStringNoeudArbreRecurssif(noeudString->gauche, listeVariablesProgramme, &premierePartieConcatenation, &estChainePremierePartieConcatenationNulle, debug);
                  if(retour == FAUTE_SEMANTIQUE)
                  {
                     char* secondePartieConcatenation = NULL;
                     int estChaineSecondePartieConcatenationNulle;
                     retour = GetStringNoeudArbreRecurssif(noeudString->droite, listeVariablesProgramme, &secondePartieConcatenation, &estChaineSecondePartieConcatenationNulle, debug);

                     if(retour == FAUTE_SEMANTIQUE)
                     {
                        if((estChainePremierePartieConcatenationNulle == 1) && (estChaineSecondePartieConcatenationNulle == 1))
                        {
                           *estChaineNulle = 1;
                        }
                        else
                        {
                           if(estChainePremierePartieConcatenationNulle == 1)
                           {
                              if(*pointeurChaineRetour != NULL && strlen(*pointeurChaineRetour) > 0)
                              {
                                 free(*pointeurChaineRetour);
                                 *pointeurChaineRetour = NULL;
                              }
                              *pointeurChaineRetour = malloc((strlen(secondePartieConcatenation) + 1) * sizeof(char));
                              
                              strcpy(*pointeurChaineRetour, secondePartieConcatenation);
                           }
                           else
                           {
                              if(estChaineSecondePartieConcatenationNulle == 1)
                              {
                                 if(*pointeurChaineRetour != NULL && strlen(*pointeurChaineRetour) > 0)
                                 {
                                    free(*pointeurChaineRetour);
                                    *pointeurChaineRetour = NULL;
                                 }
                                 *pointeurChaineRetour = malloc((strlen(premierePartieConcatenation) + 1) * sizeof(char));
                                 
                                 strcpy(*pointeurChaineRetour, premierePartieConcatenation);
                              }
                              else
                              {
                                 if(*pointeurChaineRetour != NULL && strlen(*pointeurChaineRetour) > 0)
                                 {
                                    free(*pointeurChaineRetour);
                                    *pointeurChaineRetour = NULL;
                                 }
                                 *pointeurChaineRetour = malloc(((strlen(premierePartieConcatenation) + 1) * sizeof(char)) + ((strlen(secondePartieConcatenation) + 1) * sizeof(char)));
                                 
                                 //On concate¨ne les deux chaines dans chaineRetour
                                 strcpy(*pointeurChaineRetour, premierePartieConcatenation);
                                 strcat(*pointeurChaineRetour, secondePartieConcatenation);
                              }
                           }
                        }
                     }

                     free(secondePartieConcatenation);
                  }

                  free(premierePartieConcatenation);
               }
            }
            else
            {
               printf("Erreur, la structure de l'arbre representant une chaine de caracte¨res est mauvaise. Un operateur est lu mais ce n'est pas l'operateur de la concatenation.\n");
               retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
            }
         }
         else
         {
            //Si c'est une fonction
            if(noeudString->nature == FONCTION_)
            {
               //Les seules fonctions pouvant apparaitre e  cet endroit du code sont les fonctions retournant une chaine de caracte¨res.
               //Dans notre cas, seule la fonction Convert.ToString() est interpretee
               if(strcmp(noeudString->identificateurVariableOuFonction, "Convert.ToString") == 0)
               {
                  //La fonction Convert.ToString() prend un parame¨tre.
                  //Il faut que notre noeud ait obligatoirement un fils gauche (dont le sous-arbre doit representer ce qu'il y a entre les parenthe¨ses de la fonction) et pas de fils droit
                  if(noeudString->gauche == NULL)
                  {
                     printf("un noeud representant un appel de la fonction \"Convert.ToString\" est lu, mais il n'a aucun fils gauche (le fils gauche representant le parame¨tre entre les parenthe¨ses de l'appel de la fonction).");
                     retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                  }
                  else
                  {
                     if(noeudString->droite != NULL)
                     {
                        printf("un noeud representant un appel de la fonction \"Convert.ToString\" est lu, mais il a un fils droit, ce qui n'est pas logique car normalement seul un fils gauche doit eªtre present (le fils gauche representant le parame¨tre entre les parenthe¨ses de l'appel de la fonction).");
                        retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                     }
                     //Si la structure de l'arbre est correcte
                     else
                     {
                        //Ici, nous ne gerons que les deux cas d'appel de la fonction suivant:
                        //-Le parame¨tre passe est une expression arithmetique
                        //-Le parame¨tre passe est une concatenation de chaines de caracte¨res (dans ce cas le , le retour du Convert.toString est simplement la concatenation sans modifications)




                        //On commence par regarder si le parame¨tre est une chaine de caracte¨res correcte
                        retour = GetStringNoeudArbreRecurssif(noeudString->gauche, listeVariablesProgramme, pointeurChaineRetour, estChaineNulle, debug);
                        
                        //Si il n'y a pas d'erreur on ne regarde pas si le parame¨tre est une expression arithmetique
                        if(retour == FAUTE_SEMANTIQUE)
                        {
                           //Si la valeur de la chaine est nulle, on free *pointeurChaineRetour
                           if(*estChaineNulle == 1)
                           {
                              if(*pointeurChaineRetour != NULL && strlen(*pointeurChaineRetour) > 0)
                              {
                                 free(*pointeurChaineRetour);
                                 *pointeurChaineRetour = NULL;
                              }
                           }
                        }
                        //Sinon, on regarde si l'erreur est une erreur provoquee par le type du contenu de l'arbre (si c'etait car l'arbre ne representait pas une chaine de caracte¨res).
                        else
                        {
                           //Si c'en etait bien une erreur de ce type, on regarde si le parame¨tre est une expression arithmetique
                           if(retour == FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE)
                           {
                              double valeurParametre;
                              retour = GetValeurDoubleNoeudArbre(noeudString->gauche, listeVariablesProgramme, &valeurParametre, debug);

                              //Si ce n'est pas non plus une expression arithmetique, on affiche un message d'erreur
                              if(retour != FAUTE_SEMANTIQUE)
                              {
                                 printf("un noeud representant un appel de la fonction \"Convert.ToString\" est lu, mais ce qui est passe en parame¨tre n'est ni une valeur (int ou double), ni une chaine de caracte¨res, ni une valeur nulle.");
                                 retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                              }
                              //Sinon, on converti le resultat en chaine de caracte¨re et on le retourne
                              else
                              {
                                 if(*pointeurChaineRetour != NULL && strlen(*pointeurChaineRetour) > 0)
                                 {
                                    free(*pointeurChaineRetour);
                                    *pointeurChaineRetour = NULL;
                                 }

                                 //On copie le double
                                 //Je fixe une limite de taille au nombre en string e  300 caracte¨res
                                 int long_chaine = 300;
                                 *pointeurChaineRetour = malloc((long_chaine + 1) * sizeof(char));

                                 snprintf(*pointeurChaineRetour, long_chaine, "%f", valeurParametre);
                              }
                           }

                           //Sinon, 
                        }
                     }
                  }
               }
               else
               {
                  //Les seules fonctions pouvant apparaitre e  cet endroit du code sont les fonctions retournant une chaine de caracte¨res.
                  //Dans notre cas, seule la fonction Convert.ToString() est interpretee
                  if(strcmp(noeudString->identificateurVariableOuFonction, "Console.ReadLine") == 0)
                  {
                     //La fonction Convert.ReadLine() ne prend aucun parame¨tre.
                     //Il faut que notre noeud n'ait aucun fils
                     if(noeudString->gauche != NULL || noeudString->droite != NULL)
                     {
                        printf("un noeud representant un appel de la fonction \"Console.ReadLine\" est lu, mais il n'a un fils gauche ou droit alors que la fonction ne prend aucun parame¨tre lors de son appel.");
                        retour = FAUTE_SEMANTIQUE_UTILISATION_FONCTION;
                     }
                     //Si la structure de l'arbre est correcte
                     else
                     {
                        //On va lire la chaine de caracte¨res saisie de l'utilisateur
                        //Je limite la chaine de caracte¨res saisie e  1000 caracte¨res
                        //Je declare une premiere variable tre¨s grande, je ferais une copie du contenu saisi par l'utilisateur dans la variable de retour plus tard
                        char chaineSaisieNonRaccourcie[10000];

                        //On lit la saisie de l'utilisateur
                        scanf("%[a-z,A-Z,0-9,' ']", chaineSaisieNonRaccourcie);
                        char caracterePourViderInputBuffer;
                        while ((caracterePourViderInputBuffer = getchar()) != EOF && caracterePourViderInputBuffer != '\n');

                        //On copie la partie saisie par l'utilisateur dans la variable de retour
                        if(*pointeurChaineRetour != NULL && strlen(*pointeurChaineRetour) > 0)
                        {
                           free(*pointeurChaineRetour);
                           *pointeurChaineRetour = NULL;
                        }
                        
                        *pointeurChaineRetour = malloc((strlen(chaineSaisieNonRaccourcie) + 2) * sizeof(char));
                        strcpy(*pointeurChaineRetour, chaineSaisieNonRaccourcie);

                        *estChaineNulle = 0;

                        //Pour terminer, on vide l'input buffer
                        fflush(stdin);
                     }
                  }
                  else
                  {
                     //Si le retour de la fonction n'est juste pas de bon type
                     if(strcmp(noeudString->identificateurVariableOuFonction, "Convert.ToInt32") == 0
                        || strcmp(noeudString->identificateurVariableOuFonction, "Convert.ToDouble") == 0)
                     {
                        retour = FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE;
                     }
                     //Si la fonction n'a vraiment aucune raison d'etre appelee ici (par exemple si la fonction est Console.WriteLine, son appel ici n'est pas logique)
                     else
                     {
                        printf("Erreur, une fonction ne revoyant pas de valeur apparait dans un arbre devant representer une chaine de caracte¨res.\n");
                        retour = FAUTE_SEMANTIQUE_CAS_PAS_GERE;
                     }
                  }
               }
            }
            else
            {
               if(noeudString->nature == NOMBRE_)
               {
                  retour = FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE;
               }
               //Sinon, le type de noeud n'est pas logique
               else
               {
                  printf("Erreur, un noeud qui n'est ni une chaine de caracte¨res, ni une variable, ni une fonction apparait dans un arbre devant representer une chaine de caracte¨res.\n");
                  retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
               }
            }
         }
      }

      return retour;
   }


   //Cette fonction fait pointer l'argument pointeurChaineRetour vers un pointeur de chaine de caracte¨respointant vers la chaine correspondant e  ce qu'il y a d'ecrit dans l'arbre.
   //Si il y a une erreur avec l'arbre ou que celui-ci ne correspond pas e  une chaine de caracte¨res, une erreur est renvoyee.
   //Cette fonction appelle GetStringNoeudArbreRecurssif
   ERREUR_SEMANTIQUE GetStringNoeudArbre(NoeudArbreAbstrait *noeudString, ListeVariables* listeVariablesProgramme, char** pointeurChaineRetour, int* estChaineNulle, int debug)
   {
      ERREUR_SEMANTIQUE retour = FAUTE_SEMANTIQUE;

      //On controle si le noeud et la liste des variables ne sont pas nuls
      if(noeudString == NULL)
      {
         printf("Erreur, le noeud d'arbre abstrait qui represente la chaine de caractere est nul.\n");
         retour = POINTEUR_NOEUD_ARBRE_NUL;
      }
      else
      {
         if(listeVariablesProgramme == NULL)
         {
            printf("Erreur, le pointeur de la liste des variables est nul.\n");
            retour = POINTEUR_LISTE_VARIABLES_NUL;
         }
         //Si tout va bien
         else
         {
            //On remet e  zero la chaine de caracteres
            if(*pointeurChaineRetour != NULL && strlen(*pointeurChaineRetour) > 0)
            {
               free(*pointeurChaineRetour);
               *pointeurChaineRetour = NULL;
            }

            //On appelle notre fonction recurssive
            retour = GetStringNoeudArbreRecurssif(noeudString, listeVariablesProgramme, pointeurChaineRetour, estChaineNulle, debug);

            if(retour == FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE)
            {
               printf("Erreur, une variable est placee dans une concatenation de chaine de caracte¨res mais son type n'est pas STRING/une valeur int/double est placee dans l'arbre mais elle n'est pas convertie en string.\n");
            }
         }
      }
      
      return retour;
   }


   //Fonction e  ne pas appeler directement, elle est appelee par getValeurArbreConditionBoucle
   ERREUR_SEMANTIQUE getValeurArbreConditionBoucleRecurssif(NoeudArbreAbstrait *noeudCondition, ListeVariables* listeVariablesProgramme, int* estConditionTrue, int debug)
   {
      ERREUR_SEMANTIQUE retour = FAUTE_SEMANTIQUE;

      if(noeudCondition->nature != OPERATION)
      {
         printf("La structure de l'arbre est mauvaise, un noeud de boucle est place mais le noeud devant representer la condition d'execution de cette dernie¨re n'a pas comme racine une operation.\n");
         retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
      }
      else
      {
         if(noeudCondition->operation != EGALITE && noeudCondition->operation != DIFFERENT_DE
            && noeudCondition->operation != SUPERIEUR_A && noeudCondition->operation != SUPERIEUR_OU_EGAL_A
            && noeudCondition->operation != INFERIEUR_A && noeudCondition->operation != INFERIEUR_OU_EGAL_A)
         {
            printf("La structure de l'arbre est mauvaise, un noeud de boucle est place, c'est une operation mais le type d'operation n'est pas un type correct pour une condition (double-egal, inferieur ou egal...).\n");
            retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
         }
         else
         {
            if(noeudCondition->gauche == NULL || noeudCondition->droite == NULL)
            {
               printf("La structure de l'arbre est mauvaise, un noeud de boucle est place mais le noeud devant representer la condition d'execution de cette dernie¨re n'a pas deux fils representant les deux parties de la condition.\n");
               retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT;
            }
            else
            {
               //On va regarde si il  la condition est verifiee
               int estPartie1Chaine = 0, estPartie2Chaine = 0, estPartie1ExpressionArithmetique = 0, estPartie2ExpressionArithmetique = 0;
               char* chainePartie1 = NULL, *chainePartie2 = NULL;
               int estChainePartie1Nulle, estChainePartie2Nulle;
               double valeurDoubleRetourPartie1 = 0, valeurDoubleRetourPartie2 = 0;


               //Pour commencer, ce qui est compare doit eªtre comparable
               //Il est impossible de comparer une chaine de caracte¨res avec un nombre
               //Par contre, un int et un double peuvent eªtre compares entre-eux
               //Si l'une des deux parties comparee n'a pas de valeur assignee, erreur
               //Il est impossible d'appliquer les operateurs de comparaison <, <=, >, >= aux chaines de caracte¨res

               //On commence par regarder si la premie¨re partie de la condition est une une expression arithmetique correcte
               retour = GetValeurDoubleNoeudArbreRecurssif(noeudCondition->gauche, listeVariablesProgramme, &valeurDoubleRetourPartie1, debug);
               
               //Si il y a une erreur, on regarde si l'erreur est une erreur provoquee par le type du contenu de l'arbre (si c'etait car l'arbre ne representait pas une expression arithmetique).
               if(retour != FAUTE_SEMANTIQUE)
               {
                  //Si l'erreur etait bien un proble¨me de type, on regarde si la premie¨re partie de l'expression est une expression arithmetique
                  if(retour == FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE)
                  {
                     retour = GetStringNoeudArbre(noeudCondition->gauche, listeVariablesProgramme, &chainePartie1, &estChainePartie1Nulle, debug);
               
                     //Si il n'y a pas d'erreur on regarde si la seconde partie de la condition est, elle aussi, une chaine de caracte¨res
                     if(retour == FAUTE_SEMANTIQUE)
                     {
                        estPartie1Chaine = 1;

                        retour = GetStringNoeudArbre(noeudCondition->droite, listeVariablesProgramme, &chainePartie2, &estChainePartie2Nulle, debug);
                        
                        //Si il n'y a pas d'erreur semantique, on compare les deux chaines et si ceux sont les deux meªmes, on effectue les instructions du if
                        if(retour == FAUTE_SEMANTIQUE)
                        {
                           estPartie2Chaine = 1;
                        }
                        //Sinon, on regarde si le type de la seconde partie existe afin de pouvoir retourner une erreur correcte
                        else
                        {
                           if(retour == FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE)
                           {
                              retour = GetValeurDoubleNoeudArbreRecurssif(noeudCondition->droite, listeVariablesProgramme, &valeurDoubleRetourPartie2, debug);

                              if(retour == FAUTE_SEMANTIQUE)
                              {
                                 estPartie2ExpressionArithmetique = 1;
                              }
                           }
                        }
                     }
                  }
               }
               //Sinon, il n'y a pas d'erreur et on compare avec l'autre valeur de la condition
               else
               {
                  estPartie1ExpressionArithmetique = 1;

                  retour = GetValeurDoubleNoeudArbreRecurssif(noeudCondition->droite, listeVariablesProgramme, &valeurDoubleRetourPartie2, debug);
                  
                  //Si il n'y a pas d'erreur semantique, on compare les deux chaines et si ceux sont les deux meªmes, on effectue les instructions du if
                  if(retour == FAUTE_SEMANTIQUE)
                  {
                     estPartie2ExpressionArithmetique = 1;
                  }
                  //Sinon, on regarde si le type de la seconde partie existe afin de pouvoir retourner une erreur correcte
                  else
                  {
                     if(retour == FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE)
                     {
                        retour = GetStringNoeudArbre(noeudCondition->droite, listeVariablesProgramme, &chainePartie2, &estChainePartie2Nulle, debug);

                        if(retour == FAUTE_SEMANTIQUE)
                        {
                           estPartie2Chaine = 1;
                        }
                     }
                  }
               }

               //Si il n'y a pas d'erreur, on effectue la comparaison
               if(retour == FAUTE_SEMANTIQUE)
               {
                  //Pour rappel, il est impossible de comparer une chaine de caracte¨res avec un int/double
                  if((estPartie1ExpressionArithmetique == 1 && estPartie2Chaine == 1) || (estPartie2ExpressionArithmetique == 1 && estPartie1Chaine == 1))
                  {
                     printf("Erreur, il est impossible de comparer une chaine de caracte¨res avec une expression arithmetique.\n");
                     retour = ERREUR_INFORMATIONS_NOEUD_CONDITION_ENTREE_BOUCLE;
                  }
                  else
                  {
                     int estPartie1Nulle = 0, estPartie2Nulle = 0;

                     if(estChainePartie1Nulle == 1)
                     {
                        estPartie1Nulle = 1;
                     }
                     if(estChainePartie2Nulle == 1)
                     {
                        estPartie2Nulle = 1;
                     }
                     
                     //Si la comparaison est == ou !=, l'une etant l'oppose de l'autre, on peut les regrouper
                     //Si l'une est vraie->l'autre est fausse
                     if(noeudCondition->operation == EGALITE || noeudCondition->operation == DIFFERENT_DE)
                     {
                        int estPartie1EgalePartie2 = 0;

                        //On commence par regarder si le type des deux parties est le meªme, puis on regarde si la valeur est la meªme
                        if(estPartie1Chaine == 1 && estPartie2Chaine == 1)
                        {
                           //Si les deux chaines sont nulles
                           if(estPartie1Nulle == 1 && estPartie2Nulle == 1)
                           {
                              estPartie1EgalePartie2 = 1;
                           }
                           else
                           {
                              //Si l'une des deux chaines est nulle
                              if(estPartie1Nulle == 1 || estPartie2Nulle == 1)
                              {
                                 estPartie1EgalePartie2 = 0;
                              }
                              else
                              {
                                 //Si aucune des deux chaines n'est nulle
                                 if(chainePartie1 != NULL && chainePartie2 != NULL)
                                 {
                                    if(strcmp(chainePartie1, chainePartie2) == 0)
                                    {
                                       estPartie1EgalePartie2 = 1;
                                    }
                                 }
                              }
                           }
                        }
                        else
                        {
                           if(estPartie1ExpressionArithmetique == 1 && estPartie2ExpressionArithmetique == 1)
                           {
                              if(valeurDoubleRetourPartie1 == valeurDoubleRetourPartie2)
                              {
                                 estPartie1EgalePartie2 = 1;
                              }
                           }
                        }


                        if(noeudCondition->operation == EGALITE)
                        {
                           if(estPartie1EgalePartie2 == 1)
                           {
                              *estConditionTrue = 1;
                           }
                        }
                        else
                        {
                           if(noeudCondition->operation == DIFFERENT_DE)
                           {
                              if(estPartie1EgalePartie2 == 0)
                              {
                                 *estConditionTrue = 1;
                              }
                           }
                        }
                     }
                     else
                     {
                        if(noeudCondition->operation == INFERIEUR_A || noeudCondition->operation == SUPERIEUR_OU_EGAL_A
                           || noeudCondition->operation == SUPERIEUR_A || noeudCondition->operation == INFERIEUR_OU_EGAL_A)
                        {
                           //Rappel, on ne peut pas effectuer les comparaisons <, <=, >, >= avec des chaines de caracte¨res
                           if(estPartie1ExpressionArithmetique == 0 || estPartie1ExpressionArithmetique == 0)
                           {
                              printf("Erreur, un noeud de l'arbre est une boucle, l'operateur de condition d'execution de la boucle est <, <=, >, ou >= mais au moins l'une des deux parties comparees ne represente pas une expression arithmetique.\n");
                              retour = FAUTE_SEMANTIQUE_COMPARAISON_IMPOSSIBLE;
                           }
                           else
                           {
                              //Si la comparaison est < ou >=, l'une etant l'oppose de l'autre, on peut les regrouper
                              //Si l'une est vraie->l'autre est fausse
                              if(noeudCondition->operation == INFERIEUR_A || noeudCondition->operation == SUPERIEUR_OU_EGAL_A)
                              {
                                 int estPartie1SuperieureOuEgalePartie2 = 0;

                                 if(valeurDoubleRetourPartie1 >= valeurDoubleRetourPartie2)
                                 {
                                    estPartie1SuperieureOuEgalePartie2 = 1;
                                 }

                                 if(noeudCondition->operation == SUPERIEUR_OU_EGAL_A)
                                 {
                                    if(estPartie1SuperieureOuEgalePartie2 == 1)
                                    {
                                       *estConditionTrue = 1;
                                    }
                                 }
                                 else
                                 {
                                    if(noeudCondition->operation == INFERIEUR_A)
                                    {
                                       if(estPartie1SuperieureOuEgalePartie2 == 0)
                                       {
                                          *estConditionTrue = 1;
                                       }
                                    }
                                 }
                              }
                              else
                              {
                                 //Si la comparaison est > ou <=, l'une etant l'oppose de l'autre, on peut les regrouper
                                 //Si l'une est vraie->l'autre est fausse
                                 if(noeudCondition->operation == SUPERIEUR_A || noeudCondition->operation == INFERIEUR_OU_EGAL_A)
                                 {
                                    int estPartie1InferieureOuEgalePartie2 = 0;

                                    if(valeurDoubleRetourPartie1 <= valeurDoubleRetourPartie2)
                                    {
                                       estPartie1InferieureOuEgalePartie2 = 1;
                                    }

                                    if(noeudCondition->operation == INFERIEUR_OU_EGAL_A)
                                    {
                                       if(estPartie1InferieureOuEgalePartie2 == 1)
                                       {
                                          *estConditionTrue = 1;
                                       }
                                    }
                                    else
                                    {
                                       if(noeudCondition->operation == SUPERIEUR_A)
                                       {
                                          if(estPartie1InferieureOuEgalePartie2 == 0)
                                          {
                                             *estConditionTrue = 1;
                                          }
                                       }
                                    }
                                 }
                              }
                           }
                        }
                        else
                        {
                           printf("Erreur, un noeud de l'arbre est une boucle mais l'operateur de condition d'execution de la boucle n'est pas valide (seul le double egal, le different de, les superieurs/inferieurs (ou egal) sont des operateurs valides). Operateur: %d\n", noeudCondition->operation);
                           retour = ERREUR_INFORMATION_NOEUD_ARBRE_ABSTRAIT;
                        }
                     }
                  }
               }
            }
         }
      }

      return retour;
   }


   //Fonction permettant de savoir si une condition d'entree dans une boucle est verifiee
   //Si la condition est verifiee, l'attribut estConditionTrue vers une variable de valeur 1, sinon 0
   //Une erreur est retournee si il y a un proble¨me avec l'arbre representant la condition
   ERREUR_SEMANTIQUE getValeurArbreConditionBoucle(NoeudArbreAbstrait *noeudCondition, ListeVariables* listeVariablesProgramme, int* estConditionTrue, int debug)
   {
      ERREUR_SEMANTIQUE retour = FAUTE_SEMANTIQUE;

      //On controle si le noeud et la liste des variables ne sont pas nuls
      if(noeudCondition == NULL)
      {
         printf("Erreur, le noeud d'arbre abstrait qui devait eªtre interprete comme une condition d'entree dans une boucle est nul.\n");
         retour = POINTEUR_NOEUD_ARBRE_NUL;
      }
      else
      {
         if(listeVariablesProgramme == NULL)
         {
            printf("Erreur, le pointeur de la liste des variables est nul.\n");
            retour = POINTEUR_LISTE_VARIABLES_NUL;
         }
         //Si tout va bien
         else
         {
            *estConditionTrue = 0;

            //On appelle notre fonction recurssive
            retour = getValeurArbreConditionBoucleRecurssif(noeudCondition, listeVariablesProgramme, estConditionTrue, debug);

            if(retour == FAUTE_SEMANTIQUE_CONTENU_ARBRE_MAUVAIS_TYPE)
            {
               printf("Erreur, une variable est placee dans une concatenation de chaine de caracte¨res mais son type n'est pas STRING/une valeur int/double est placee dans l'arbre mais elle n'est pas convertie en string.\n");
            }
         }
      }

      return retour;
   }

   