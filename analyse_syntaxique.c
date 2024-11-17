#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 

#include "analyse_syntaxique.h"

   int fichierCorrectementOuvert = 0;

   /* --------------------------------------------------------------------- */

   //Fonction construisant l'arbre d'une fichier contenant des instructions C#.
   ERREUR_SYNTAXIQUE analyseSyntaxique(char *fichierCodeSource, ListeArbresAbstraits *listeArbresInstructionsPrincipales, int estFonctionAppeleePourListeInstructionsBoucleIfWhile, int debug)
   {
      
      
     demarrer(fichierCodeSource);
      

      ERREUR_SYNTAXIQUE retour = PAS_D_ERREUR_SYNTAXIQUE;

      //Si il y a un probleme qui fait que le fichier n'a pas pu etre ouvert
      /*if(fichierCorrectementOuvert != 1)
      {
         printf("Erreur, le fichier n'a pas pu etre ouvert.\n");
         retour = FICHIER_PAS_PU_ETRE_OUVERT;
      }*/
      //Si l'adresse du fichier est bonne et que le fichier a ete correctement ouvert
     // else
      //{
         //Cas dans lequel le fichier entre est vide
         if(fin_de_sequence() == 1)
         {
            retour = ENTREE_VIDE;
            printf("Erreur, le fichier est vide.\n");
         }
         else
         {
            if(listeArbresInstructionsPrincipales != NULL)
            {
               if(listeArbresInstructionsPrincipales->tete != NULL)
               {
                  detruireElementListeArbresAbstraitsEtSuivants(listeArbresInstructionsPrincipales->tete);
               }

               ListeArbresAbstraits *listeArbresInstructions = listeArbresInstructionsPrincipales;


               //On commence l'analyse du fichier instruction par instruction
               //printf("Debut analyse\n");


               Nature_Lexeme natureLexemeCourant;
               ArbreAbstrait *arbreAbstraitInstruction;
               int finInstructionsBoucleIfWhile = 0;


               //Analyse de la sequence de lexemes
               while(fin_de_sequence() == 0 && finInstructionsBoucleIfWhile == 0 && retour == PAS_D_ERREUR_SYNTAXIQUE)
               {
                  arbreAbstraitInstruction = NULL;
                  natureLexemeCourant = lexeme_courant().nature;

                  //I) Recherche du debut d'une ligne de commande. Une ligne peut etre:
                  //-Une declaration de variable (on commence par declarer son type)
                  //-Une affectation de valeur a  une variable (on commence par donner l'identifiant de la variable)
                  //-Une utilisation d'une fonction (Console.WriteLine)
                  //-Une boucle (if/else ou while)

                  //Si la ligne de commande semble etre une declaration de variable
                  if(natureLexemeCourant == TYPE_INT || natureLexemeCourant == TYPE_DOUBLE || natureLexemeCourant == TYPE_STRING)
                  {
                     TypesVariables typeDeclare = TYPE_VARIABLE_NON_DEFINI;

                     if(natureLexemeCourant == TYPE_INT)
                     {
                        typeDeclare = INT;
                     }
                     if(natureLexemeCourant == TYPE_DOUBLE)
                     {
                        typeDeclare = DOUBLE;
                     }
                     if(natureLexemeCourant == TYPE_STRING)
                     {
                        typeDeclare = STRING;
                     }

                     avancer();

                     int declarationTerminee = 0;

                     //En C#, il est possible de declarer plusieurs variables a  la suite en les separant d'une virgule, ce cas est gere c'est pour a§a qu'un while est present
                     while(retour == PAS_D_ERREUR_SYNTAXIQUE && declarationTerminee == 0)
                     {
                        arbreAbstraitInstruction = nouvelArbreAbstrait();
                        arbreAbstraitInstruction->tete = nouveauNoeudArbreAbstrait();
                        arbreAbstraitInstruction->tete->nature = TYPE_VARIABLE_;
                        arbreAbstraitInstruction->tete->typeVariable = typeDeclare;

                        //Normalement, un identificateur de variable ou de fonction devrait arriver, mais vu que la creation de fonction n'est pas geree, on renvoie une erreur si ce n'est pas une declaration de variable
                        //Si il n'arrive pas, c'est une erreur
                        
                        natureLexemeCourant = lexeme_courant().nature;
                        if(natureLexemeCourant == IDENTIFICATEUR_VARIABLE_OU_FONCTION)
                        {
                           arbreAbstraitInstruction->tete->gauche = nouveauNoeudArbreAbstrait();
                           arbreAbstraitInstruction->tete->gauche->identificateurVariableOuFonction = malloc((strlen(lexeme_courant().chaine) + 1) * sizeof(char));
                           strcpy(arbreAbstraitInstruction->tete->gauche->identificateurVariableOuFonction, lexeme_courant().chaine);

                           avancer();
                           natureLexemeCourant = lexeme_courant().nature;

                           //Un identificateur est un indentificateur de fonction si une parenthese ouverte arrive en suivant
                           if(natureLexemeCourant == PARO)
                           {
                              if(retour == PAS_D_ERREUR_SYNTAXIQUE)
                              {
                                 printf("Erreur, une fonction semble declaree mais ce cas n'est pas gere par l'interpreteur.\n");
                                 retour = ERREUR_CAS_NON_GERE;
                              }
                           }
                           else
                           {
                              //L'identificateur est bien un identificateur de variable
                              arbreAbstraitInstruction->tete->gauche->nature = VARIABLE_;

                              
                              if(natureLexemeCourant == EGAL)
                              {
                                 NoeudArbreAbstrait *noeudAffectationInsere = nouveauNoeudArbreAbstrait();
                                 noeudAffectationInsere->nature = AFFECTATION;
                                 noeudAffectationInsere->gauche = arbreAbstraitInstruction->tete->gauche;
                                 arbreAbstraitInstruction->tete->gauche = noeudAffectationInsere;

                                 //On place a  droite du noeud affectation (du noeud "=") la valeur que doit prendre la variable declaree (ce qu'il y a derriere le egal)
                                 arbreAbstraitInstruction->tete->gauche->droite = nouveauNoeudArbreAbstrait();
                                 avancer();
                                 retour = GetArbreExpression(arbreAbstraitInstruction->tete->gauche->droite, 0, 1, 0, &natureLexemeCourant, debug);

                                 //Cette fonction avance automatiquement au caractere derriere l'expression lue:
                                 //Exemple: exprsssion = (2 + 3) suite. La fonction avancera automatiquement au caractere derriere le ')'

                                 //natureLexemeCourant a maintenant la valeur du denrier lexeme lu dans l'expression (point-virgule, virgule etc...)
                              }
                              else
                              {
                                 if(natureLexemeCourant == POINT_VIRGULE || natureLexemeCourant == VIRGULE)
                                 {
                                    avancer();
                                 }
                                 else
                                 {
                                    printf("Erreur, une variable est declaree make le lexeme suivant cette declaration n'est pas correct. Lexeme lu: %d\n", natureLexemeCourant);
                                    retour = ERREUR_DE_SYNTAXE;
                                 }
                              }


                              if(retour == PAS_D_ERREUR_SYNTAXIQUE)
                              {
                                 //La variable est declaree et on lui a potentiellement assigne une valeur.
                                 //On avance au prochain lexeme, si l'on rencontre une virgule cela veut dire que l'on declare une nouvelle variable du meme type
                                 //et on ne sort pas de la boucle. Sinon on ne peut rencontrer qu'un point-virgule indiquant la fin de la declaration de variables, sinon c'est une erreur.
                                 if(natureLexemeCourant == POINT_VIRGULE)
                                 {
                                    declarationTerminee = 1;
                                 }
                                 else
                                 {
                                    if(natureLexemeCourant == VIRGULE)
                                    {
                                       
                                    }
                                    else
                                    {
                                       printf("Erreur, une variable est declaree et on lui a potentiellement affecte une valeur mais ni une virgule, ni un point virgule indiquant la fin de la declaration n'a ete lu a  la suite de cette declaration.\n");
                                       retour = ERREUR_DE_SYNTAXE;
                                    }
                                 }


                                 //Instructions permettant de ranger l'arbre de la declaration dans la liste des arbres-instructions
                                 if(retour == PAS_D_ERREUR_SYNTAXIQUE)
                                 {
                                    int arbreBienAjoute = ajouterArbreListeArbresAbstraits(listeArbresInstructions, arbreAbstraitInstruction);

                                    if(arbreBienAjoute == 0)
                                    {
                                       printf("Erreur, l'arbre abstrait representant l'instruction n'a pas pu etre ajoute.\n");
                                       retour = ERREUR_AJOUT_ARBRE_INSTRUCTION_DANS_LISTE;
                                    }
                                    else
                                    {
                                       arbreAbstraitInstruction = NULL;
                                    }
                                 }
                              }
                           }
                        }
                        else
                        {
                           if(retour == PAS_D_ERREUR_SYNTAXIQUE)
                           {
                              printf("Erreur, un identificateur de variable etait attendu (un type de variable est place mais ce qui est derriere n'est pas logique).\n");
                              retour = ERREUR_DE_SYNTAXE;
                           }
                        }
                     }
                  }
                  else
                  {
                     //Si la ligne de commande semble etre une affectation de valeur a  une variable deja  declaree ou l'appel d'une fonction
                     if(natureLexemeCourant == IDENTIFICATEUR_VARIABLE_OU_FONCTION)
                     {
                        arbreAbstraitInstruction = nouvelArbreAbstrait();
                        arbreAbstraitInstruction->tete = nouveauNoeudArbreAbstrait();
                        //On copie le nom de la fonction/de la variable
                        arbreAbstraitInstruction->tete->identificateurVariableOuFonction = malloc((strlen(lexeme_courant().chaine) + 1) * sizeof(char));
                        strcpy(arbreAbstraitInstruction->tete->identificateurVariableOuFonction, lexeme_courant().chaine);

                        avancer();
                        natureLexemeCourant = lexeme_courant().nature;

                        //Si la ligne de commmande est l'appel d'une fonction
                        if(natureLexemeCourant == PARO)
                        {
                           arbreAbstraitInstruction->tete->nature = FONCTION_;
                           
               
                           avancer();
                           natureLexemeCourant = lexeme_courant().nature;

                           if(natureLexemeCourant != PARF)
                           {
                              arbreAbstraitInstruction->tete->gauche = nouveauNoeudArbreAbstrait();

                              retour = GetArbreExpression(arbreAbstraitInstruction->tete->gauche, 1, 0, 0, &natureLexemeCourant, debug);
                              //natureLexemeCourant prend la valeur du dernier lexeme lu dans l'expression (point-virgule, virgule etc...)
                              //Cette fonction avance automatiquement au caractere derriere l'expression lue:
                              //Exemple: exprsssion = (2 + 3) suite. La fonction avancera automatiquement au caractere derriere le ')'
                           }
                           else
                           {
                              avancer();
                           }
                        }
                        //Si la ligne de commmande est une affectation de valeur a  une variable
                        else
                        {
                           arbreAbstraitInstruction->tete->nature = VARIABLE_;
                           
                           //On controle qu'il y a bien un egal a  la suite
                           if(natureLexemeCourant == EGAL)
                           {
                              //Si il y a un egal, on construit l'arbre d'affectation de valeur
                              NoeudArbreAbstrait* nouvelleTeteEgal = nouveauNoeudArbreAbstrait();
                              nouvelleTeteEgal->nature = AFFECTATION;
                              nouvelleTeteEgal->gauche = arbreAbstraitInstruction->tete;
                              arbreAbstraitInstruction->tete = nouvelleTeteEgal;

                              avancer();

                              //On place a  droite de la tete (du noeud "=") la valeur que doit prendre la variable declaree
                              arbreAbstraitInstruction->tete->droite = nouveauNoeudArbreAbstrait();
                              retour = GetArbreExpression(arbreAbstraitInstruction->tete->droite, 0, 0, 0, &natureLexemeCourant, debug);
                              //natureLexemeCourant prend la valeur du dernier lexeme lu dans l'expression (point-virgule, virgule etc...)
                              //Cette fonction avance automatiquement au caractere derriere l'expression lue:
                              //Exemple: exprsssion = (2 + 3) suite. La fonction avancera automatiquement au caractere derriere le ')'
                           }
                           //Sinon, c'est une erreur syntaxique
                           else
                           {
                              if(retour == PAS_D_ERREUR_SYNTAXIQUE)
                              {
                                 printf("Erreur, un identificateur de variable ou de fonction est place mais ce qui est place derriere n'est pas logique (une affectation est attendue (egal)).\n");
                                 retour = ERREUR_DE_SYNTAXE;
                              }
                           }
                        }
                        
                        //Si il n'y a pas d'erreur (l'instruction est un appel de fonction/affectation de valeur correct), un point-virgule est attendu, sinon c'est une erreur.
                        if(retour == PAS_D_ERREUR_SYNTAXIQUE)
                        {
                           if(arbreAbstraitInstruction->tete->nature == VARIABLE_)
                           {
                              //Si l'instrcution est une affectation de valeur a  une variable, le dernier lexeme lu par GetArbreExpression doit etre un point-virgule
                              if(natureLexemeCourant == POINT_VIRGULE)
                              {
                                 
                              }
                              else
                              {
                                 printf("Erreur, l'instruction est une affectation de valeur a  une variable, un point-virgule etait attendu pour conclure l'instruction mais un autre lexeme est arrive. Le dernier lexeme lu est: %d.\n", natureLexemeCourant);
                                 retour = ERREUR_DE_SYNTAXE;
                              }
                           }
                           else
                           {
                              if(arbreAbstraitInstruction->tete->nature == FONCTION_)
                              {
                                 //Si l'instrcution est un appel de fonction, le dernier lexeme lu par GetArbreExpression doit etre une parenthese fermante, et nous devons etre positionnes sur un point-virgule
                                 if(natureLexemeCourant == PARF && lexeme_courant().nature == POINT_VIRGULE)
                                 {
                                    avancer();
                                 }
                                 else
                                 {
                                    printf("Erreur, l'instruction est un appel de fonction, une parenthese fermante suivie d'un point-virgule etaient attendus pour conclure l'instruction mais une autre serie de lexemes est arrivee. Les deux derniers lexemes lus sont: %d%d.\n", natureLexemeCourant, lexeme_courant().nature);
                                    retour = ERREUR_DE_SYNTAXE;
                                 }
                              }
                           }
                        }
                     }
                     else
                     {
                        //Si la ligne de commande semble etre une boucle (if/else ou while)
                        if(natureLexemeCourant == IDENTIFICATEUR_IF || natureLexemeCourant == IDENTIFICATEUR_WHILE)
                        {
                           //En fonction du type de boucle, on construit l'arbre differement
                           //Si c'est une boucle if, l'attribut listeArbresInstructionsBoucleConditionValide de la racine de l'arbre de cette derniere sera la liste des instructions de si la condition est satisfaite, listeArbresInstructionsBoucleConditionValide sera la liste des instructions du else (il peut ne pas y en avoir)
                           //Si c'est un while, la racine de l'arbre du WHILE aura listeArbresInstructionsBoucleConditionValide qui pointera vers la liste des conditions a  executer tant que la condition est valable
                           //L'arbre de la condition est construit et pointe par l'attribut
                           arbreAbstraitInstruction = nouvelArbreAbstrait();
                           arbreAbstraitInstruction->tete = nouveauNoeudArbreAbstrait();
                           arbreAbstraitInstruction->tete->nature = BOUCLE;

                           if(natureLexemeCourant == IDENTIFICATEUR_IF)
                           {
                              arbreAbstraitInstruction->tete->typeBoucle = IF;
                           }
                           else
                           {
                              if(natureLexemeCourant == IDENTIFICATEUR_WHILE)
                              {
                                 arbreAbstraitInstruction->tete->typeBoucle = WHILE;
                              }
                           }

                           avancer();
                           natureLexemeCourant = lexeme_courant().nature;

                           //Si la boucle n'a pas une syntaxe correcte
                           if(natureLexemeCourant != PARO)
                           {
                              if(retour == PAS_D_ERREUR_SYNTAXIQUE)
                              {
                                 printf("Erreur, un debut de boucle if ou while a ete lue mais aucune parenthese ouvrante (delimitant une condition) n'a ete lue derriere.\n");
                                 retour = ERREUR_DE_SYNTAXE;
                              }
                           }
                           //Si
                           else
                           {
                              avancer();

                              //On lit la condition et on range l'arbre correspondant dans l'attribut conditionExecutionBoucle
                              arbreAbstraitInstruction->tete->conditionExecutionBoucle = nouveauNoeudArbreAbstrait();
                              retour = GetArbreConditionBoucle(arbreAbstraitInstruction->tete->conditionExecutionBoucle, &natureLexemeCourant, debug);
                              //natureLexemeCourant prend la valeur du dernier lexeme lu dans l'expression (point-virgule, virgule etc...)
                              //Cette fonction avance automatiquement au caractere derriere l'expression lue:
                              //Exemple: condition = (variable == valeur) {. La fonction avancera automatiquement au caractere derriere le ')', au niveau du '{'

                              //Si il n'y a pas d'erreur, on regarde la suite, une accolade ouvrante devrait apparaitre
                              if(retour == PAS_D_ERREUR_SYNTAXIQUE)
                              {
                                 if(natureLexemeCourant == PARF && lexeme_courant().nature == ACO_OUVRANT)
                                 {
                                    avancer();
                                    
                                    //On va alors creer la liste des instructions de la boucle et la ranger dans l'attribut listeArbresInstructionsBoucleConditionValide
                                    arbreAbstraitInstruction->tete->listeArbresInstructionsBoucleConditionValide = nouvelleListeArbresAbstraits();
                                    retour = analyseSyntaxique(fichierCodeSource, arbreAbstraitInstruction->tete->listeArbresInstructionsBoucleConditionValide, 1, debug);

                                    //Normalement, (si il n'y a pas d'erreurs) l'analyseur syntaxique a avance derrriere l'accolade fermante
                                    if(retour == PAS_D_ERREUR_SYNTAXIQUE)
                                    {
                                       //Si la boucle lue etait un while, l'instruction est terminee, sinon on regarde si il y a unelse derriere
                                       if(arbreAbstraitInstruction->tete->typeBoucle == IF)
                                       {
                                          natureLexemeCourant = lexeme_courant().nature;

                                          //Si il y a un else derriere le if, on range ses instructions dans listeArbresInstructionsBoucleConditionNonValide
                                          if(natureLexemeCourant == IDENTIFICATEUR_ELSE)
                                          {
                                             avancer();
                                             natureLexemeCourant = lexeme_courant().nature;
                                             if(natureLexemeCourant == ACO_OUVRANT)
                                             {
                                                avancer();
                                    
                                                //On va alors creer la liste des instructions de la boucle et la ranger dans l'attribut listeArbresInstructionsBoucleConditionValide
                                                arbreAbstraitInstruction->tete->listeArbresInstructionsBoucleConditionNonValide = nouvelleListeArbresAbstraits();
                                                retour = analyseSyntaxique(fichierCodeSource, arbreAbstraitInstruction->tete->listeArbresInstructionsBoucleConditionNonValide, 1, debug);

                                                //L'interpretation de la boucle if/else est terminee
                                             }
                                             else
                                             {
                                                printf("Erreur, l'instruction est la seconde partie d'une boucle if (un else), une accolade ouverte etaient attendue mais un autre lexeme est arrive. Le lexemes lu est: %d.\n", natureLexemeCourant);
                                                retour = ERREUR_DE_SYNTAXE;
                                             }
                                          }
                                          //Sinon on ne fait rien et on passe a  l'instruction suivante
                                       }
                                    }
                                 }
                                 else
                                 {
                                    printf("Erreur, l'instruction est une boucle if ou while, une parenthese fermante suivie d'une accolade ouverte etaient attendues mais une autre serie de lexemes est arrivee. Les deux derniers lexemes lus sont: %d, %d.\n", natureLexemeCourant, lexeme_courant().nature);
                                    retour = ERREUR_DE_SYNTAXE;
                                 }
                              }
                           }
                        }
                        //Il y a une erreur syntaxique, une ligne de commande ne peut pas commencer par autre chose que les cas geres precedement
                        else
                        {
                           if(natureLexemeCourant == ACO_FERMANT)
                           {
                              if(estFonctionAppeleePourListeInstructionsBoucleIfWhile == 1)
                              {
                                 finInstructionsBoucleIfWhile = 1;
                              }
                              else
                              {
                                 printf("Erreur, une accolade fermante a ete lue mais nous l'analyse syntaxique ne lisait pas une boucle if ou while.\n");
                                 retour = ERREUR_DE_SYNTAXE;
                              }
                           }
                           else
                           {
                              printf("Erreur, une ligne de commande ne peut pas commencer par commencer par autre chose qu'un type (debut d'une declaration de variable) ou qu'un identificateur de variable (affectation de valeur a  une variable)/fonction (appel d'une fonction).\n");
                              retour = ERREUR_DE_SYNTAXE;
                           }
                        }
                     }


                     //Instructions permettant de ranger l'arbre de l'instruction dans la liste si il n'y a pas eu d'erreur durant sa creation
                     if(retour == PAS_D_ERREUR_SYNTAXIQUE && finInstructionsBoucleIfWhile == 0)
                     {
                        int arbreBienAjoute = ajouterArbreListeArbresAbstraits(listeArbresInstructions, arbreAbstraitInstruction);
                        
                        if(arbreBienAjoute == 0)
                        {
                           printf("Erreur, l'arbre abstrait representant l'instruction n'a pas pu etre ajoute.\n");
                           retour = ERREUR_AJOUT_ARBRE_INSTRUCTION_DANS_LISTE;
                        }
                     }
                  }
               }
            }
            //En theorie n'arrive jamais
            else
            {
               printf("Erreur avec le pointeur de l'arbre passe en parametre.\n");
               retour = ERREUR_POINTEUR_ARBRE;
            }
         }

         if(estFonctionAppeleePourListeInstructionsBoucleIfWhile == 0)
         {
            arreter();
         }
         else
         {
            if(retour == PAS_D_ERREUR_SYNTAXIQUE)
            {
               avancer();
            }
         }
      

      return retour;}
   

   


   //Fonction appelee par GetArbreExpression
   ERREUR_SYNTAXIQUE GetArbreExpressionRecurssif(NoeudArbreAbstrait *noeudArbreExpression, int estDansParenthese, int estDansDeclarationVariable, int estDansConditionBoucleIfWhile, int debug)
   {
      ERREUR_SYNTAXIQUE retour = PAS_D_ERREUR_SYNTAXIQUE;

      //Cas dans lequel on est a  la fin du fichier avant meme d'avoir commence a  construire l'arbre
      if(fin_de_sequence() == 1)
      {
         retour = ERREUR_DE_LEXEME;
         printf("Erreur, une expression etait attendue mais on est arrive a la fin du fichier.\n");
      }
      else
      {
         if(noeudArbreExpression != NULL)
         {
            noeudArbreExpression->nature = NON_DEFINI;

            Nature_Lexeme natureLexemeCourant;

            //Analyse de la sequence de lexemes (jusqu'a  la premiere parenthese fermante, point-virgule, virgule ou fin de sequence). En fonction de si ces elements sont logiques ou non, on retourne une erreur.
            //I) Analyse du debut de l'expression, seuls certains lexemes peuvent apparaitre:
            //   Un +, -, identificateur de variable/fonction, une parenthese ouvrante, un nombre ou une chaine de caracteres.
            natureLexemeCourant = lexeme_courant().nature;

            int avancerApresLectureLexeme = 1;
            int plusOuMoinsRencontrePremierLexeme = 0;

            //Si le lexeme est celui d'un mot reserve a  une declaration de variable, a§a n'a rien a  faire la 
            if(natureLexemeCourant == TYPE_INT ||natureLexemeCourant == TYPE_DOUBLE ||natureLexemeCourant == TYPE_STRING)
            {
               printf("Erreur, un mot reserve a  la declaration de variable (int, double, string) est apparu dans une expression devant representer une valeur.\n");
               retour = ERREUR_DE_SYNTAXE;
            }
            else
            {
               if(natureLexemeCourant == PARF)
               {
                  if(estDansParenthese == 0)
                  {
                     printf("Erreur, une parenthese fermante a ete lue, mais un debit d'expression devant representer une valeur etait attendu.\n");
                     retour = ERREUR_DE_SYNTAXE;
                  }
                  else
                  {
                     printf("Erreur, une parenthese vide est presente.\n");
                     retour = ERREUR_DE_SYNTAXE;
                  }
               }
               else
               {
                  if(natureLexemeCourant == POINT_VIRGULE)
                  {
                     printf("Erreur, un point-virgule a ete lu, mais une expression etait attendue.\n");
                     retour = ERREUR_DE_SYNTAXE;
                  }
                  else
                  {
                     if(natureLexemeCourant == VIRGULE)
                     {
                        printf("Erreur, une virgule a ete lu, mais une expression etait attendue.\n");
                        retour = ERREUR_DE_SYNTAXE;
                     }
                     else
                     {
                        if(natureLexemeCourant == PARO)
                        {
                           avancer();
                           retour = GetArbreExpression(noeudArbreExpression, 1, 0, 0, &natureLexemeCourant, debug);
                           avancerApresLectureLexeme = 0;
                        }
                        else
                        {
                           //Une expression ne peut commencer que par un entier, une chaine de caractere, une variable, une fonction ou un signe + ou -.
                           //Si l'on rencontre l'un de ces lexemes, on l'indique dans la racine et on stocke leur valeur, sinon c'est une erreur
                           if(natureLexemeCourant == ENTIER)
                           {
                              noeudArbreExpression->nature = NOMBRE_;
                              noeudArbreExpression->valeurNombre = lexeme_courant().valeur;
                           }
                           else
                           {
                              if(natureLexemeCourant == CHAINE_DE_CARACTERES)
                              {
                                 noeudArbreExpression->nature = CHAINE_CARACTERES_;
                                 
                                 if(noeudArbreExpression->chaineDeCaracteres != NULL && strlen(noeudArbreExpression->chaineDeCaracteres) > 0)
                                 {
                                    free(noeudArbreExpression->chaineDeCaracteres);
                                    noeudArbreExpression->chaineDeCaracteres = NULL;
                                 }
                                 
                                 noeudArbreExpression->chaineDeCaracteres = malloc((strlen(lexeme_courant().chaine) + 1) * sizeof(char));
                                 strcpy(noeudArbreExpression->chaineDeCaracteres, lexeme_courant().chaine);
                              }
                              else
                              {
                                 if(natureLexemeCourant == LEXEME_VALEUR_NULL)
                                 {
                                    noeudArbreExpression->nature = VALEUR_NULLE_;
                                 }
                                 else
                                 {
                                    if(natureLexemeCourant == IDENTIFICATEUR_VARIABLE_OU_FONCTION)
                                    {
                                       if(noeudArbreExpression->identificateurVariableOuFonction != NULL && strlen(noeudArbreExpression->identificateurVariableOuFonction) > 0)
                                       {
                                          free(noeudArbreExpression->identificateurVariableOuFonction);
                                          noeudArbreExpression->identificateurVariableOuFonction = NULL;
                                       }
                                       noeudArbreExpression->identificateurVariableOuFonction = malloc((strlen(lexeme_courant().chaine) + 1) * sizeof(char));
                                       strcpy(noeudArbreExpression->identificateurVariableOuFonction, lexeme_courant().chaine);
                                       
                                       avancer();
                                       natureLexemeCourant = lexeme_courant().nature;

                                       //Un identificateur est un indentificateur de fonction si une parenthese ouverte arrive en suivant
                                       if(natureLexemeCourant == PARO)
                                       {
                                          noeudArbreExpression->nature = FONCTION_;

                                          //On place a  gauche de la tete (du noeud FONCTION) ce qu'il y a entre les parentheses (si il y a des choses entre, une fonction peut etre appelee sans parametres, Console.ReadLine et Console.WriteLine le peuvent)
                                          avancer();
                                          natureLexemeCourant = lexeme_courant().nature;

                                          if(natureLexemeCourant != PARF)
                                          {
                                             noeudArbreExpression->gauche = nouveauNoeudArbreAbstrait();
                                             retour = GetArbreExpression(noeudArbreExpression->gauche, 1, 0, 0, &natureLexemeCourant, debug);
                                          }
                                          else
                                          {
                                             avancer();
                                          }
                                       }
                                       else
                                       {
                                          //L'identificateur est un identificateur de variable
                                          noeudArbreExpression->nature = VARIABLE_;
                                       }

                                       avancerApresLectureLexeme = 0;
                                    }
                                    else
                                    {
                                       if(natureLexemeCourant == PLUS)
                                       {
                                          //On ne fait rien mais il n'y a pas de probleme, on va construire l'arbre du plus juste en suivant (et on garde en memoire qu'on a rencontre un PLUS)
                                          avancerApresLectureLexeme = 0;
                                          plusOuMoinsRencontrePremierLexeme = 1;
                                       }
                                       else
                                       {
                                          if(natureLexemeCourant == MOINS)
                                          {
                                             //On ne fait rien mais il n'y a pas de probleme, on va construire l'arbre du plus juste en suivant (et on garde en memoire qu'on a rencontre un MOINS)
                                             avancerApresLectureLexeme = 0;
                                             plusOuMoinsRencontrePremierLexeme = 1;
                                          }
                                          else
                                          {
                                             printf("Erreur, un lexeme de debut d'expression etait attendu (entier, chaine de caractere, variable, fonction, signe + ou -) mais un autre lexeme est apparu. Lexeme lu: %d\n", natureLexemeCourant);
                                             retour = ERREUR_DE_SYNTAXE;
                                          }
                                       }
                                    }
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }

            //Si La premiere partie de l'expression est correcte
            //II) Analyse de la seconde partie de l'expression, a  partir de ce moment seuls des lexemes de "lien" entre deux parties d'une expression (partie1 + partie2 * partie3) ou des lexemes indiquant la fin de l'expression peuvent apparaitre:
            //    Un +, -, *, /, une parenthese fermante (si on etait dans une parenthese) ou un point-virgule (si l'on etait pas dans une parenthese)
            if(retour == PAS_D_ERREUR_SYNTAXIQUE)
            {
               if(avancerApresLectureLexeme == 1)
               {
                  avancer();
               }
               natureLexemeCourant = lexeme_courant().nature;

               //Si le lexeme lu est un lexeme de "lien" entre deux parties d'une expression (+, -, * ou /). Il va falloir gerer les priorites.
               //-Si dans la premiere partie on avait rencontre un + ou un -
               // ->Le noeudArbreExpression devient un + ou un - et on appelle recursivement la fonction dans laquelle on se situe sur notre fils gauche.


               //-Si dans la premiere partie on avait rencontre un lexeme correct autre qu'un + ou un -,
               // ->Si le lexeme rencontre est un + ou un - (pas prioritaires) on va alors copier les informations du noeud au sommet a  son fils gauche.
               //   Transformer noeudArbreExpression en + ou - et appelle recursivement la fonction dans laquelle on se situe sur notre fils droit.

               // ->Sinon, si le lexeme rencontre est un * ou un / (prioritaires) on va commencer par appeler la fonction dans laquelle on se situe sur un nouveau noeud.
               //   En fonction du resultat il y a deux cas possibles:
               //   -Soit l'arbre n'est compose que de sa racine (ou d'une unique negation d'un terme, -5 par exemple). Si c'est le cas on copie les informations du noeud au sommet a  son fils gauche, la racine devenant un *, / et on place le retour de la fonction en fils droit de notre multiplication.
               //   -Soit l'arbre n'est pas compose que de sa racine (il y a des operations dedans). Si c'est le cas on remplace le fils gauche du retour de la fonction par (noeudArbreExpression (*, /) filsGaucheRetourFonction).
               //    Car le fils gauche du retour de la fonction est le premier element entre dans l'arbre.
               //    Une fois le retour de la fonction transforme, on fait noeudArbreExpression = retourTransforme. C'est le nouveau retour de cette fonction
               if(natureLexemeCourant == PLUS || natureLexemeCourant == MOINS
                  || natureLexemeCourant == MUL || natureLexemeCourant == DIV)
               {
                  NoeudArbreAbstrait* noeudInsereDroiteOuGauche = nouveauNoeudArbreAbstrait();

                  int retourFonctionCopie = 1;
                  if(plusOuMoinsRencontrePremierLexeme == 0)
                  {
                     retourFonctionCopie = CopieInformationsNoeudArbreAbstrait2DansNoeudArbreAbstrait1(noeudInsereDroiteOuGauche, noeudArbreExpression);
                  }
                  if(retourFonctionCopie != 1)
                  {
                     retour = ERREUR_FONCTION;
                  }
                  else
                  {
                     RemiseAZeroNoeudArbreAbstrait(noeudArbreExpression);

                     avancer();
                     
                     if(natureLexemeCourant == PLUS || natureLexemeCourant == MOINS)
                     {
                        noeudArbreExpression->nature = OPERATION;
                        if(natureLexemeCourant == PLUS)
                        {
                           noeudArbreExpression->operation = ADDITION;
                        }
                        else
                        {
                           if(natureLexemeCourant == MOINS)
                           {
                              noeudArbreExpression->operation = SOUSTRACTION;
                           }
                        }

                        noeudArbreExpression->gauche = noeudInsereDroiteOuGauche;

                        if(plusOuMoinsRencontrePremierLexeme == 1)
                        {
                           retour = GetArbreExpressionRecurssif(noeudArbreExpression->gauche, estDansParenthese, estDansDeclarationVariable, estDansConditionBoucleIfWhile, debug);
                        }
                        else
                        {
                           noeudArbreExpression->droite = nouveauNoeudArbreAbstrait();
                           retour = GetArbreExpressionRecurssif(noeudArbreExpression->droite, estDansParenthese, estDansDeclarationVariable, estDansConditionBoucleIfWhile, debug);
                        }
                     }
                     else
                     {
                        if(natureLexemeCourant == MUL || natureLexemeCourant == DIV)
                        {
                           retour = GetArbreExpressionRecurssif(noeudArbreExpression, estDansParenthese, estDansDeclarationVariable, estDansConditionBoucleIfWhile, debug);
                           
                           if(retour == PAS_D_ERREUR_SYNTAXIQUE)
                           {
                              if(noeudArbreExpression != NULL)
                              {
                                 //Soit l'arbre de la suite de l'expression n'est compose que de sa racine (ou d'un signe suivi d'un unique terme, -5 ou +-+5 par exemple).
                                 //Si c'est le cas on place le retour de la fonction en fils droit de notre multiplication.
                                 if((noeudArbreExpression->gauche == NULL && noeudArbreExpression->droite ==  NULL)
                                    || (noeudArbreExpression->gauche != NULL && noeudArbreExpression->droite ==  NULL))
                                 {
                                    NoeudArbreAbstrait* noeudArbreSuiteExpressionInsereADroiteMulDiv = nouveauNoeudArbreAbstrait();
                                    int retourCopie = CopieInformationsNoeudArbreAbstrait2DansNoeudArbreAbstrait1(noeudArbreSuiteExpressionInsereADroiteMulDiv, noeudArbreExpression);

                                    if(retourCopie == 1)
                                    {
                                       RemiseAZeroNoeudArbreAbstrait(noeudArbreExpression);

                                       noeudArbreExpression->nature = OPERATION;
                                       if(natureLexemeCourant == MUL)
                                       {
                                          noeudArbreExpression->operation = MULTIPLICATION;
                                       }
                                       else
                                       {
                                          if(natureLexemeCourant == DIV)
                                          {
                                             noeudArbreExpression->operation = DIVISION;
                                          }
                                       }
                                       noeudArbreExpression->gauche = noeudInsereDroiteOuGauche;
                                       noeudArbreExpression->droite = noeudArbreSuiteExpressionInsereADroiteMulDiv;
                                    }
                                    else
                                    {
                                       printf("Erreur lors de la copie des informations d'un noeud vers un autre noeud.\n");
                                       retour = ERREUR_FONCTION;
                                    }
                                 }
                                 else
                                 {
                                    //Soit l'arbre n'est pas compose que de sa racine (il y a des operations dedans). Si c'est le cas on remplace le fils gauche du retour de la fonction par (noeudArbreExpression (*, /) filsGaucheRetourFonction).
                                    //Car le fils gauche du retour de la fonction est le premier element entre dans l'arbre.
                                    //Une fois le retour de la fonction transforme, on fait noeudArbreExpression = retourTransforme. C'est le nouveau retour de cette fonction
                                    if(noeudArbreExpression->nature == OPERATION)
                                    {
                                       if(noeudArbreExpression->gauche != NULL && noeudArbreExpression->droite !=  NULL)
                                       {
                                          NoeudArbreAbstrait* noeudArbreMulDivInsereAGaucheNoeudSuiteExpression = nouveauNoeudArbreAbstrait();
                                          noeudArbreMulDivInsereAGaucheNoeudSuiteExpression->nature = OPERATION;
                                          if(natureLexemeCourant == MUL)
                                          {
                                             noeudArbreMulDivInsereAGaucheNoeudSuiteExpression->operation = MULTIPLICATION;
                                          }
                                          else
                                          {
                                             if(natureLexemeCourant == DIV)
                                             {
                                                noeudArbreMulDivInsereAGaucheNoeudSuiteExpression->operation = DIVISION;
                                             }
                                          }
                                          noeudArbreMulDivInsereAGaucheNoeudSuiteExpression->gauche = noeudInsereDroiteOuGauche;
                                          noeudArbreMulDivInsereAGaucheNoeudSuiteExpression->droite = noeudArbreExpression->gauche;

                                          //On place le nouveau noeud a  la gauche du noeud de la suite de l'expression
                                          noeudArbreExpression->gauche = noeudArbreMulDivInsereAGaucheNoeudSuiteExpression;
                                       }
                                       else
                                       {
                                          retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT_DURANT_CONSTRUCTION;
                                          printf("Erreur, au niveau de la structure de l'arbre suite a  un appel recurssif lors de sa construction. Il a un fil non-nul a  droite mais un fils nul a  gauche.\n");
                                       }
                                    }
                                    else
                                    {
                                       retour = ERREUR_STRUCTURE_ARBRE_ABSTRAIT_DURANT_CONSTRUCTION;
                                       printf("Erreur, au niveau de la structure de l'arbre suite a  un appel recurssif lors de sa construction. Un arbre dont la racine est une operation est attendue.\n");
                                    }
                                 }
                              }
                              else
                              {
                                 retour = ERREUR_RETOUR_FONCTION_ARBRE_NUL;
                                 printf("Erreur, le retour de la fonction est un arbre nul.\n");
                              }
                           }
                        }
                     }
                  }
               }
               //Sinon, le lexeme lu n'est pas un lexeme de "lien" entre deux parties d'une expression
               //Les trois seuls lexemes valides sont alors une parenthese fermante (valide si l'on avait lu une parenthese ouverte), un point-virgule (valide si l'on avait pas lu de parenthese ouverte) ou un double egal (si l'on etait dans une condition).
               //Si le lexeme lu n'est pas l'un de ces deux lexemes, c'est une erreur.
               //Si il n'y a pas d'erreur, on se contente de sortir de la fonction.
               else
               {
                  //Si le lexeme lu est une parenthese fermante
                  if(natureLexemeCourant == PARF)
                  {
                     //Cas dans lequel la parenthese fermante n'est pas logique
                     if(estDansParenthese == 0)
                     {
                        printf("Erreur de parenthesage.\n");
                        retour = ERREUR_DE_SYNTAXE;
                     }
                     //Sinon, la parenthese fermante est logique, on ne renvoie pas d'erreur
                     else
                     {
                        
                     }
                  }
                  else
                  {
                     //Si le lexeme lu est un point-virgule
                     if(natureLexemeCourant == POINT_VIRGULE)
                     {
                        //Cas dans lequel le point-virgule n'est pas logique
                        if(estDansParenthese == 1)
                        {
                           printf("Erreur, un point-virgule indiquant la fin d'une instruction a ete lu mais une parenthese fermante etait attendue.\n");
                           retour = ERREUR_DE_SYNTAXE;
                        }

                        //Sinon, le point-virgule est logique et on ne renvoie pas d'erreur
                     }
                     //Si le lexeme lu n'est pas correct
                     else
                     {
                        //Si le lexeme lu est une virgule
                        if(natureLexemeCourant == VIRGULE)
                        {
                           //Cas dans lequel la virgule n'est pas logique
                           if(estDansParenthese == 1 || estDansDeclarationVariable == 0)
                           {
                              if(estDansParenthese == 1)
                              {
                                 printf("Erreur, une virgule indiquant la fin d'une declaration de variable d'un type avant de passer a  une autre a ete lue mais une parenthese fermante etait attendue.\n");
                              }
                              else
                              {
                                 printf("Erreur, une virgule indiquant la fin d'une declaration de variable d'un type avant de passer a  une autre a ete lue mais nous n'etions pas dans la cas d'une declaration de variable.\n");
                              }
                              
                              retour = ERREUR_DE_SYNTAXE;
                           }

                           //Sinon, la virgule est logique, on ne renvoie pas d'erreur et on avance
                           else
                           {
                              
                           }
                        }
                        else
                        {
                           if(natureLexemeCourant == DOUBLE_EGAL || natureLexemeCourant == DIFFERENT
                              || natureLexemeCourant == LEXEME_INFERIEUR || natureLexemeCourant == LEXEME_INFERIEUR_OU_EGAL
                              || natureLexemeCourant == LEXEME_SUPERIEUR || natureLexemeCourant == LEXEME_SUPERIEUR_OU_EGAL)
                           {
                              //Si le double egal ou le different de n'est pas logique
                              if(estDansConditionBoucleIfWhile == 0)
                              {
                                 printf("Erreur, un double egal a ete lue mais nous n'etions pas dans une condition d'entree dans une boucle.\n");
                                 retour = ERREUR_DE_SYNTAXE;
                              }
                              //Sinon il l'est
                           }
                           else
                           {
                              printf("Erreur, un lexeme de lien (+, -, *, /)");
                              if(estDansParenthese == 0)
                              {
                                 if(estDansDeclarationVariable == 0)
                                 {
                                    printf(" ou");
                                 }
                                 else
                                 {
                                    printf(",");
                                 }

                                 printf(" un point-virgule");

                                 if(estDansDeclarationVariable == 1)
                                 {
                                    printf(" ou une virgule");
                                 }
                              }
                              else
                              {
                                 printf(" ou une parenthese fermante");
                              }

                              printf(" etait attendu, mais un lexeme incorrect a ete lu. Le lexeme lu est: %d\n", natureLexemeCourant);

                              retour = ERREUR_DE_SYNTAXE;
                           }
                        }
                     }
                  }
               }
            }
         }
         //En theorie n'arrive jamais
         else
         {
            printf("Erreur avec le pointeur de l'arbre passe en parametre lors de l'appel de la fonction GetArbreExpression.\n");
            retour = ERREUR_POINTEUR_ARBRE;
         }
      }

      return retour;
   }

   //Fonction construisant l'arbre contenant la prochaine expression rencontree
   //(par exemple une expression decrivant une valeur affectee a  une variable ou l'expression entre les parentheses d'une fonction telle que Console.WriteLine())
   //Cette fonction avance automatiquement au caractere derriere l'expression lue:
   //Exemple: exprsssion = (2 + 3) suite. La fonction avancera automatiquement au caractere derriere le ')'
   //Le dernier lexeme lu est retourne dans natureDernierLexemeLu. Si aucun lexeme n'a ete lu, sa valeur est LEXEME_INDEFINI
   //Cette fonction appelle GetArbreExpressionRecurssif
   ERREUR_SYNTAXIQUE GetArbreExpression(NoeudArbreAbstrait *noeudArbreExpression, int estDansParenthese, int estDansDeclarationVariable, int estDansConditionBoucleIfWhile, Nature_Lexeme* natureDernierLexemeLu, int debug)
   {
      RemiseAZeroNoeudArbreAbstrait(noeudArbreExpression);

      ERREUR_SYNTAXIQUE retour = GetArbreExpressionRecurssif(noeudArbreExpression, estDansParenthese, estDansDeclarationVariable, estDansConditionBoucleIfWhile, debug);

      *natureDernierLexemeLu = lexeme_courant().nature;

      if(retour == PAS_D_ERREUR_SYNTAXIQUE)
      {
         avancer();
      }

      return retour;
   }


   //Fonction appelee par GetArbreConditionBoucle
   ERREUR_SYNTAXIQUE GetArbreConditionBoucleRecurssif(NoeudArbreAbstrait *noeudArbreCondition, int debug)
   {
      ERREUR_SYNTAXIQUE retour = PAS_D_ERREUR_SYNTAXIQUE;

      //Cas dans lequel on est a  la fin du fichier avant meme d'avoir commence a  construire l'arbre
      if(fin_de_sequence() == 1)
      {
         retour = ERREUR_DE_LEXEME;
         printf("Erreur, une expression etait attendue mais on est arrive a  la fin du fichier.\n");
      }
      else
      {
         if(noeudArbreCondition != NULL)
         {
            noeudArbreCondition->nature = NON_DEFINI;

            Nature_Lexeme natureLexemeCourant;

            //Analyse de la sequence de lexemes (jusqu'a  la premiere parenthese fermante, point-virgule, virgule ou fin de sequence). En fonction de si ces elements sont logiques ou non, on retourne une erreur.
            //I) Analyse du debut de la condition, la premiere partie de la condition est une expression
            NoeudArbreAbstrait* noeudInsereGauche = nouveauNoeudArbreAbstrait();
            retour = GetArbreExpressionRecurssif(noeudInsereGauche, 0, 0, 1, debug);

            //Si La premiere partie de l'expression est correcte, un lexeme d'egalite doit apparaitre
            if(retour == PAS_D_ERREUR_SYNTAXIQUE)
            {
               natureLexemeCourant = lexeme_courant().nature;

               if(natureLexemeCourant == DOUBLE_EGAL || natureLexemeCourant == DIFFERENT
                  || natureLexemeCourant == LEXEME_INFERIEUR || natureLexemeCourant == LEXEME_INFERIEUR_OU_EGAL
                  || natureLexemeCourant == LEXEME_SUPERIEUR || natureLexemeCourant == LEXEME_SUPERIEUR_OU_EGAL)
               {
                  RemiseAZeroNoeudArbreAbstrait(noeudArbreCondition);
                  noeudArbreCondition->nature = OPERATION;
                  if(natureLexemeCourant == DOUBLE_EGAL)
                  {
                     noeudArbreCondition->operation = EGALITE;
                  }
                  else
                  {
                     if(natureLexemeCourant == DIFFERENT)
                     {
                        noeudArbreCondition->operation = DIFFERENT_DE;
                     }
                     else
                     {
                        if(natureLexemeCourant == LEXEME_INFERIEUR)
                        {
                           noeudArbreCondition->operation = INFERIEUR_A;
                        }
                        else
                        {
                           if(natureLexemeCourant == LEXEME_INFERIEUR_OU_EGAL)
                           {
                              noeudArbreCondition->operation = INFERIEUR_OU_EGAL_A;
                           }
                           else
                           {
                              if(natureLexemeCourant == LEXEME_SUPERIEUR)
                              {
                                 noeudArbreCondition->operation = SUPERIEUR_A;
                              }
                              else
                              {
                                 if(natureLexemeCourant == LEXEME_SUPERIEUR_OU_EGAL)
                                 {
                                    noeudArbreCondition->operation = SUPERIEUR_OU_EGAL_A;
                                 }
                              }
                           }
                        }
                     }
                  }
                  
                  noeudArbreCondition->gauche = noeudInsereGauche;
                  noeudArbreCondition->droite = nouveauNoeudArbreAbstrait();

                  avancer();
                  
                  retour = GetArbreExpressionRecurssif(noeudArbreCondition->droite, 1, 0, 0, debug);

                  if(retour == PAS_D_ERREUR_SYNTAXIQUE)
                  {
                     natureLexemeCourant = lexeme_courant().nature;

                     if(natureLexemeCourant != PARF)
                     {
                        retour = ERREUR_DE_SYNTAXE;
                        printf("Erreur, une parenthese pour delimiter la fin d'une condition d'entree dans une boucle etait attendue mais un autre lexeme est arrive. Lexeme lu: %d\n", natureLexemeCourant);
                     }
                  }
               }
               else
               {
                  retour = ERREUR_DE_SYNTAXE;
                  printf("Erreur, un lexeme d'egalite etait attendu (condition), mais un autre lexeme a ete lu. Lexeme lu: %d\n", natureLexemeCourant);
               }
            }
         }
         //En theorie n'arrive jamais
         else
         {
            printf("Erreur avec le pointeur de l'arbre passe en parametre lors de l'appel de la fonction GetArbreExpression.\n");
            retour = ERREUR_POINTEUR_ARBRE;
         }
      }

      return retour;
   }

   //Fonction construisant l'arbre contenant la prochaine expression rencontree
   //(par exemple une expression decrivant une valeur affectee a  une variable ou l'expression entre les parentheses d'une fonction telle que Console.WriteLine())
   //Cette fonction avance automatiquement au caractere derriere l'expression lue:
   //Exemple: exprsssion = (2 + 3) suite. La fonction avancera automatiquement au caractere derriere le ')'
   //Le dernier lexeme lu est retourne dans natureDernierLexemeLu. Si aucun lexeme n'a ete lu, sa valeur est LEXEME_INDEFINI
   //Cette fonction appelle GetArbreExpressionRecurssif
   ERREUR_SYNTAXIQUE GetArbreConditionBoucle(NoeudArbreAbstrait *noeudArbreCondition, Nature_Lexeme* natureDernierLexemeLu, int debug)
   {
      RemiseAZeroNoeudArbreAbstrait(noeudArbreCondition);

      ERREUR_SYNTAXIQUE retour = GetArbreConditionBoucleRecurssif(noeudArbreCondition, debug);

      *natureDernierLexemeLu = lexeme_courant().nature;

      if(retour == PAS_D_ERREUR_SYNTAXIQUE)
      {
         avancer();
      }

      return retour;
   }
   