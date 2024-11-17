
#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <ctype.h>

#include "lecture_caracteres.h"
#include "analyse_lexicale.h"

   /* --------------------------------------------------------------------- */


/* Les variables et fonctions locales au module */

   Lexeme lexeme_en_cours ;	/* le lexeme courant */

   void ajouter_caractere(char **s, char c);
   Nature_Caractere nature_caractere (char c);
   int est_separateur(char c ) ;
   int est_chiffre(char c ) ;
   int est_lettre(char c ) ;
   int est_symbole(char c ) ;
   int est_caractere_autorise_pour_identificateur_variables_types_et_fonctions(char c);
   void reconnaitre_lexeme();

   /* --------------------------------------------------------------------- */

   int demarrer(char *chaineInstruction) {
      int retour = demarrer_car(chaineInstruction);

      //Si le fichier n'existe pas
      if(retour == 0)
      {
         printf("Erreur, le fichier specifie n'existe pas.\n");
      }
      else
      {
         avancer();
      }

      return retour;
   }

   /* --------------------------------------------------------------------- */

   void avancer() {
      reconnaitre_lexeme();
   } 

   /* --------------------------------------------------------------------- */

   Lexeme lexeme_courant() {
      return lexeme_en_cours;
   } 

   /* --------------------------------------------------------------------- */

   int fin_de_sequence() {
      return lexeme_en_cours.nature == FIN_SEQUENCE;
   }

   /* --------------------------------------------------------------------- */

   void arreter() {
      arreter_car() ;
   }

   /* --------------------------------------------------------------------- */


   // reconnaissance d'un nouveau lexeme
   // etat initial : le caractere courant est soit separateur 
   //                soit le 1er caractere d'un lexeme
   // etat final : 
   //       - un nouveau lexeme est reconnu dans lexeme_en_cours
   //       - le caractere courant est soit la fin de fichier, 
   //		soit un separateur,  soit le 1er caractere d'un lexeme

   void reconnaitre_lexeme() {
      typedef enum {E_INIT, E_NUM, E_FIN, E_VAR_FUNCT, E_CHAINE_DE_CARACTERES, E_EGAL_OU_DOUBLE_EGAL, E_DIFFERENT, E_DETECTION_INFERIEUR, E_DETECTION_SUPERIEUR, E_DETECTION_COMMENTAIRES_DIV} Etat_Automate ;
      Etat_Automate etat=E_INIT;

     lexeme_en_cours.chaine = "\0" ;

     // on utilise ensuite un automate pour reconnaitre et construire le prochain lexeme

      while (etat != E_FIN)
      {
         // on commence par lire et ignorer les separateurs
         passerSeperateurs();

         // etat initial
         if(etat == E_INIT)
         {
            if(nature_caractere(caractere_courant()) == C_FIN_SEQUENCE)
            {
               lexeme_en_cours.nature = FIN_SEQUENCE;
               etat = E_FIN;
            }
            else
            {
               if(nature_caractere(caractere_courant()) == CHIFFRE)
               {
                  lexeme_en_cours.valeur = 0;
                  lexeme_en_cours.nature = ENTIER;
                  etat = E_NUM;
               }
               else
               {
                  if(caractere_courant() == '.')
                  {
                     lexeme_en_cours.valeur = 0;
                     lexeme_en_cours.nature = ENTIER;
                     etat = E_NUM;
                  }
                  else
                  {
                     if(nature_caractere(caractere_courant()) == LETTRE)
                     {
                        etat = E_VAR_FUNCT;
                     }
                     else
                     {
                        if(nature_caractere(caractere_courant()) == SYMBOLE)
                        {
                           lexeme_en_cours.ligne = numero_ligne();
                           lexeme_en_cours.colonne = numero_colonne();
                           if(caractere_courant() == '+')
                           {
                              lexeme_en_cours.nature = PLUS;
                              etat = E_FIN;
                           }
                           else
                           {
                              if(caractere_courant() == '-')
                              {
                                 lexeme_en_cours.nature = MOINS;
                                 etat = E_FIN;
                              }
                              else
                              {
                                 if(caractere_courant() == '*')
                                 {
                                    lexeme_en_cours.nature = MUL;
                                    etat = E_FIN;
                                 }
                                 else
                                 {
                                    if(caractere_courant() == '/')
                                    {
                                       etat = E_DETECTION_COMMENTAIRES_DIV;
                                    }
                                    else
                                    {
                                       if(caractere_courant() == '(')
                                       {
                                          lexeme_en_cours.nature = PARO;
                                          etat = E_FIN;
                                       }
                                       else
                                       {
                                          if(caractere_courant() == ')')
                                          {
                                             lexeme_en_cours.nature = PARF;
                                             etat = E_FIN;
                                          }
                                          else
                                          {
                                             if(caractere_courant() == '{')
                                             {
                                                lexeme_en_cours.nature = ACO_OUVRANT;
                                                etat = E_FIN;
                                             }
                                             else
                                             {
                                                if(caractere_courant() == '}')
                                                {
                                                   lexeme_en_cours.nature = ACO_FERMANT;
                                                   etat = E_FIN;
                                                }
                                                else
                                                {
                                                   if(caractere_courant() == '"')
                                                   {
                                                      lexeme_en_cours.nature = CHAINE_DE_CARACTERES;
                                                      etat = E_CHAINE_DE_CARACTERES;
                                                      avancer_car();
                                                   }
                                                   else
                                                   {
                                                      if(caractere_courant() == ';')
                                                      {
                                                         lexeme_en_cours.nature = POINT_VIRGULE;
                                                         etat = E_FIN;
                                                      }
                                                      else
                                                      {
                                                         if(caractere_courant() == ',')
                                                         {
                                                            lexeme_en_cours.nature = VIRGULE;
                                                            etat = E_FIN;
                                                         }
                                                         else
                                                         {
                                                            if(caractere_courant() == '=')
                                                            {
                                                               etat = E_EGAL_OU_DOUBLE_EGAL;
                                                            }
                                                            else
                                                            {
                                                               if(caractere_courant() == '!')
                                                               {
                                                                  etat = E_DIFFERENT;
                                                               }
                                                               else
                                                               {
                                                                  if(caractere_courant() == '<')
                                                                  {
                                                                     etat = E_DETECTION_INFERIEUR;
                                                                  }
                                                                  else
                                                                  {
                                                                     if(caractere_courant() == '>')
                                                                     {
                                                                        etat = E_DETECTION_SUPERIEUR;
                                                                     }
                                                                     else
                                                                     {
                                                                        printf("Erreur lexicale, symbole inconnu.\n");
                                                                        lexeme_en_cours.nature = ERREUR;
                                                                        ajouter_caractere (&lexeme_en_cours.chaine, caractere_courant()) ;
                                                                        etat = E_FIN;
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
                                    }
                                 }
                              }
                           }
                        }
                        else
                        {
                           printf("Erreur lexicale. Caractere inconnu lu: ");
                           printf("%c\n", caractere_courant());
                           lexeme_en_cours.nature = ERREUR;
                           ajouter_caractere(&lexeme_en_cours.chaine, caractere_courant()) ;
                           etat = E_FIN;
                        }
                     }
                     

                     if(etat == E_FIN)
                     {
                        avancer_car();
                     }
                  }
               }
            }
         }
         else
         {
            if(etat == E_NUM)
            {
               if(nature_caractere(caractere_courant()) == CHIFFRE)
               {
                  ajouter_caractere (&lexeme_en_cours.chaine, caractere_courant()) ;
                  lexeme_en_cours.valeur = (lexeme_en_cours.valeur * 10) + (caractere_courant() - '0');
                  avancer_car();
               }
               else
               {
                  //On regarde si l'entier est un reel
                  if(caractere_courant() == '.')
                  {
                     ajouter_caractere (&lexeme_en_cours.chaine, '.');

                     //Permet de calculer les decimales
                     int diviseurVirgule = 10;

                     while(etat == E_NUM)
                     {
                        avancer_car();
                        passerSeperateurs();

                        if(nature_caractere(caractere_courant()) == CHIFFRE)
                        {
                           lexeme_en_cours.valeur = lexeme_en_cours.valeur + (((double)(caractere_courant() - '0')) / diviseurVirgule);
                           diviseurVirgule = diviseurVirgule * 10;
                        }
                        else
                        {
                           etat = E_FIN;
                        }
                     }
                  }
                  else
                  {
                     etat = E_FIN;
                  }
               }
            }
            else
            {
               if(etat == E_VAR_FUNCT)
               {
                  lexeme_en_cours.ligne = numero_ligne();
                  lexeme_en_cours.colonne = numero_colonne();

                  //On sauvegarde la chaine de caracteres dans le lexeme en cours
                  while(est_caractere_autorise_pour_identificateur_variables_types_et_fonctions(caractere_courant()) == 1)
                  {
                     ajouter_caractere(&lexeme_en_cours.chaine, caractere_courant());
                     avancer_car();
                  }

                  //printf("Identificateur ou type lu = \"%s\"\n", lexeme_en_cours.chaine);
                  
                  //On regarde si la chaine de caractere qui a ete lue est un type de variable (int, double ou string), le potentiel nom d'une variable, l'identificateur if, else ou while ou bien la valeur null (que l'on peut affecter Ã  une chaine de caracteres)
                  if(strcmp(lexeme_en_cours.chaine, "int") == 0)
                  {
                     lexeme_en_cours.nature = TYPE_INT;
                  }
                  else
                  {
                     if(strcmp(lexeme_en_cours.chaine, "double") == 0)
                     {
                        lexeme_en_cours.nature = TYPE_DOUBLE;
                     }
                     else
                     {
                        if(strcmp(lexeme_en_cours.chaine, "string") == 0)
                        {
                           lexeme_en_cours.nature = TYPE_STRING;
                        }
                        else
                        {
                           if(strcmp(lexeme_en_cours.chaine, "null") == 0)
                           {
                              lexeme_en_cours.nature = LEXEME_VALEUR_NULL;
                           }
                           else
                           {
                              if(strcmp(lexeme_en_cours.chaine, "if") == 0)
                              {
                                 lexeme_en_cours.nature = IDENTIFICATEUR_IF;
                              }
                              else
                              {
                                 if(strcmp(lexeme_en_cours.chaine, "else") == 0)
                                 {
                                    lexeme_en_cours.nature = IDENTIFICATEUR_ELSE;
                                 }
                                 else
                                 {
                                    if(strcmp(lexeme_en_cours.chaine, "while") == 0)
                                    {
                                       lexeme_en_cours.nature = IDENTIFICATEUR_WHILE;
                                    }
                                    else
                                    {
                                       //Si l'identificateur est utilise comme une fonction il y a une parenthese ouverte derriere, sinon c'est un identificateur de variable
                                       lexeme_en_cours.nature = IDENTIFICATEUR_VARIABLE_OU_FONCTION;
                                    }
                                 }
                              }
                           }
                        }
                     }
                  }
                  
                  etat = E_FIN;
               }
               else
               {
                  if(etat == E_CHAINE_DE_CARACTERES)
                  {
                     lexeme_en_cours.ligne = numero_ligne();
                     lexeme_en_cours.colonne = numero_colonne();
                     lexeme_en_cours.nature = CHAINE_DE_CARACTERES;

                     //On sauvegarde la chaine de caracteres dans le lexeme en cours
                     while(caractere_courant() != '"')
                     {
                        if(caractere_courant() == '\\')
                        {
                           //Liste des caracteres pouvant etre echapes:
                           //'\t', '\n', '\r', '\a', '\b', '\f'
                           avancer_car();
                           if(caractere_courant() == 't')
                           {
                              ajouter_caractere(&lexeme_en_cours.chaine, '\t');
                           }
                           else
                           {
                              if(caractere_courant() == 'n')
                              {
                                 ajouter_caractere(&lexeme_en_cours.chaine, '\n');
                              }
                              else
                              {
                                 if(caractere_courant() == 'r')
                                 {
                                    ajouter_caractere(&lexeme_en_cours.chaine, '\r');
                                 }
                                 else
                                 {
                                    if(caractere_courant() == 'a')
                                    {
                                       ajouter_caractere(&lexeme_en_cours.chaine, '\a');
                                    }
                                    else
                                    {
                                       if(caractere_courant() == 'b')
                                       {
                                          ajouter_caractere(&lexeme_en_cours.chaine, '\b');
                                       }
                                       else
                                       {
                                          if(caractere_courant() == 'f')
                                          {
                                             ajouter_caractere(&lexeme_en_cours.chaine, '\f');
                                          }
                                          else
                                          {
                                             if(caractere_courant() == '"')
                                             {
                                                ajouter_caractere(&lexeme_en_cours.chaine, '\"');
                                             }
                                             else
                                             {
                                                lexeme_en_cours.nature = ERREUR;
                                                ajouter_caractere (&lexeme_en_cours.chaine, '\\');
                                                ajouter_caractere (&lexeme_en_cours.chaine, caractere_courant()) ;
                                                etat = E_FIN;
                                                printf("Erreur de lexeme, un caractere ne pouvant pas etre echappe d'une chaine de caractere s'est vu precede d'un \\.");
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
                           ajouter_caractere(&lexeme_en_cours.chaine, caractere_courant());
                        }

                        avancer_car();
                     }
                     avancer_car();
                     
                     etat = E_FIN;
                  }
                  else
                  {
                     //Etat permettant de controler si un egal ou un double egal est lu
                     if(etat == E_EGAL_OU_DOUBLE_EGAL)
                     {
                        avancer_car();
                        passerSeperateurs();

                        if(caractere_courant() == '=')
                        {
                           lexeme_en_cours.nature = DOUBLE_EGAL;
                           avancer_car();
                        }
                        else
                        {
                           lexeme_en_cours.nature = EGAL;
                        }

                        etat = E_FIN;
                     }
                     else
                     {
                        //Etat permettant de controler si un '!=' est lu
                        if(etat == E_DIFFERENT)
                        {
                           avancer_car();
                           passerSeperateurs();

                           if(caractere_courant() == '=')
                           {
                              lexeme_en_cours.nature = DIFFERENT;
                              avancer_car();
                           }
                           else
                           {
                              lexeme_en_cours.nature = ERREUR;
                           }

                           etat = E_FIN;
                        }
                        else
                        {
                           //Etat permettant de controler si un '<' ou un '<=' est lu
                           if(etat == E_DETECTION_INFERIEUR)
                           {
                              avancer_car();
                              passerSeperateurs();

                              if(caractere_courant() == '=')
                              {
                                 lexeme_en_cours.nature = LEXEME_INFERIEUR_OU_EGAL;
                                 avancer_car();
                              }
                              else
                              {
                                 lexeme_en_cours.nature = LEXEME_INFERIEUR;
                              }

                              etat = E_FIN;
                           }
                           else
                           {
                              //Etat permettant de controler si un '>' ou un '>=' est lu
                              if(etat == E_DETECTION_SUPERIEUR)
                              {
                                 avancer_car();
                                 passerSeperateurs();

                                 if(caractere_courant() == '=')
                                 {
                                    lexeme_en_cours.nature = LEXEME_SUPERIEUR_OU_EGAL;
                                    avancer_car();
                                 }
                                 else
                                 {
                                    lexeme_en_cours.nature = LEXEME_SUPERIEUR;
                                 }

                                 etat = E_FIN;
                              }
                              else
                              {
                                 if(etat == E_DETECTION_COMMENTAIRES_DIV)
                                 {
                                    avancer_car();

                                    //Si le commentaire lu est un commentaire s'arretant Ã  la fin de la ligne
                                    if(caractere_courant() == '/')
                                    {
                                       while(caractere_courant() != '\n')
                                       {
                                          avancer_car();
                                       }

                                       avancer_car();
                                       etat = E_INIT;
                                    }
                                    else
                                    {
                                       //Si le commentaire lu est un commentaire /*Commentaire*/
                                       if(caractere_courant() == '*')
                                       {
                                          avancer_car();

                                          int commentairePasse = 0;
                                          while(commentairePasse == 0)
                                          {
                                             if(caractere_courant() == '*')
                                             {
                                                avancer_car();
                                                if(caractere_courant() == '/')
                                                {
                                                   commentairePasse = 1;
                                                }
                                             }
                                             else
                                             {
                                                avancer_car();
                                             }
                                          }

                                          avancer_car();
                                          etat = E_INIT;
                                       }
                                       else
                                       {
                                          lexeme_en_cours.nature = DIV;
                                          avancer_car();
                                          etat = E_FIN;
                                       }
                                    }
                                 }
                                 //Etat final
                                 else
                                 {

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

   /* --------------------------------------------------------------------- */


   void passerSeperateurs()
   {
      while(est_separateur(caractere_courant()) == 1)
      {
        	avancer_car();
      }
   }

   // cette fonction ajoute le caractere c a la fin de la chaine s dont l'adresse du poiteur est passee en parametre
   // Exemple d'utilisation:
   // char* chaine = "Salut cava";
   // ajouter_caractere(&chaine, 'a');
   // ->Maintenant, chaine pointe vers la chaine de caracteres: "Salut cava"
 
   void ajouter_caractere(char **s, char c)
   {
      if(*s != NULL)
      {
         int longueurString = strlen(*s);
         char *ancienneChaine = *s;
         *s = NULL;
         char *nouvelleChaine = malloc ((longueurString + 2) * sizeof(char));
         strcpy (nouvelleChaine, ancienneChaine);
         nouvelleChaine[longueurString] = c;
         nouvelleChaine[longueurString + 1] = '\0';

         if(longueurString > 0)
         {
            free(ancienneChaine);
         }
         *s = nouvelleChaine;
      }
   }

   /* --------------------------------------------------------------------- */

   Nature_Caractere nature_caractere (char c) {
      Nature_Caractere retour = ERREUR_CAR;

      
      if (est_chiffre(c) == 1)
      {
         retour = CHIFFRE;
      }
      else
      {
         if (est_symbole(c))
         {
            retour = SYMBOLE;
         }
         else
         {
            if (est_lettre(c))
            {
               retour = LETTRE;
            }
            else
            {
               if(fin_de_sequence_car(c))
               {
                  retour = C_FIN_SEQUENCE;
               }
            }
         }
      }

      return retour;
   }
   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere separateur
   int est_separateur(char c)
   {
      int retour = 0;

      if(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\a' || c == '\b' || c == '\f')
      {
         retour = 1;
      }

      return retour;
   }

   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere chiffre
   int est_chiffre(char c) {
      int retour = 0;

      if(c >= '0' && c <= '9')
      {
         retour = 1;
      }

      return  retour;
   }

   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere correspondant a un symbole arithmetique
   int est_symbole(char c)  {
      int retour = 0;

      if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')'
           || c == '{' || c == '}' || c == '"' || c == ';' || c == ',' || c == '=' || c == '!' || c == '<' || c == '>')
      {
         retour = 1;
      }

      return retour;
   }

   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere correspondant a un symbole arithmetique
   int est_caractere_autorise_pour_identificateur_variables_types_et_fonctions(char c)  {
      int retour = 0;

      if (est_lettre(c) == 1 || est_chiffre(c) == 1 || c == '_' || c == '.')
      {
         retour = 1;
      }

      return retour;
   }

   /* --------------------------------------------------------------------- */

   // vaut vrai ssi c designe un caractere correspondant a une lettre ou a  un underscore
   int est_lettre(char c)  {
      int retour = 0;

      if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')
      {
         retour = 1;
      }

      return retour;
   }

   /* --------------------------------------------------------------------- */

   // renvoie la chaine de caracteres correspondant a la nature du lexeme
   char *Nature_vers_Chaine (Nature_Lexeme nature) {
	switch (nature) {
		case ENTIER: return "NOMBRE" ;
		case PLUS: return " + " ;
      case MOINS: return " - " ;            
      case MUL: return " * " ;            
      case DIV: return " / " ;      
      case PARO: return "(" ;    
      case PARF: return ")" ;
      case IDENTIFICATEUR_VARIABLE_OU_FONCTION: return "IDENTIFICATEUR" ;
      case TYPE_INT: return "int " ;
      case TYPE_DOUBLE: return "double " ;
      case TYPE_STRING: return "string " ;
      case POINT_VIRGULE: return ";\n" ;
      case FIN_SEQUENCE: return "FIN_SEQUENCE" ;
      case ERREUR: return "ERREUR" ;
      default: return "ERREUR" ;            
	} ;
   } 

   /* --------------------------------------------------------------------- */

   // affiche a l'ecran le lexeme l
   void afficher(Lexeme l) {

      switch (l.nature) {
         case FIN_SEQUENCE: 
            break;
         default: 
            printf("(ligne %d, ", l.ligne);
            printf("colonne %d) : ",l.colonne);
	    printf("[") ;
            printf("nature = %s", Nature_vers_Chaine(l.nature)) ;
            printf(", chaine = %s, ", l.chaine) ;

            if(l.nature == ENTIER)
            {
               printf("valeur = %f", l.valeur);
            }
	    printf("]") ;
	} ;
   }

   /* --------------------------------------------------------------------- */
