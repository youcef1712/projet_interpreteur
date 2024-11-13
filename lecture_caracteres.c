
/* ------------------------------------------------------------------------
-- module letcure_caracteres
--
-- sequence de caracteres
--
-- P. Habraken : 12 novembre 2002
-- L. Mounier : Aout 2016 (version en C)
 ------------------------------------------------------------------------ */


#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "lecture_caracteres.h"

   /* --------------------------------------------------------------------- */

   // Les variables locales au module 

    int CARAC_COURANT;  	
    unsigned int LigneCour, ColonneCour;  // numeros de ligne et colonne
    FILE *File;		// le fichier d'entree

   /* --------------------------------------------------------------------- */

   int demarrer_car(char *nom_fichier) {
      int retour = 0;
      if (strlen(nom_fichier) == 0)
      {
         File = stdin;
      }
      else
      {
         if(fileExists(nom_fichier) == 1)
         {
            File = fopen(nom_fichier, "r");

            LigneCour = 1;
            ColonneCour = 0;
            avancer_car ();

            retour = 1;
         }
      }

      return retour;
   }

   /* --------------------------------------------------------------------- */

   void avancer_car() {
      if (!feof(File)) { 
	CARAC_COURANT = fgetc(File) ;
	if (CARAC_COURANT != '\n') {  
		ColonneCour = ColonneCour + 1 ;
	} else { 
		ColonneCour = 1 ;
		LigneCour = LigneCour + 1 ;
	} ;
      } else { 
	CARAC_COURANT = EOF ;
      } ;
   }

   /* --------------------------------------------------------------------- */

   char caractere_courant() {
      return CARAC_COURANT;
   }

   /* --------------------------------------------------------------------- */

   int fin_de_sequence_car()  {
      return CARAC_COURANT == EOF ;
   } 

   /* --------------------------------------------------------------------- */

   unsigned int numero_ligne() {
      return LigneCour;
   }

   /* --------------------------------------------------------------------- */

   unsigned int numero_colonne() {
      return ColonneCour ;
   }

   /* --------------------------------------------------------------------- */

  void arreter_car() {
     fclose(File) ;
  } 

   /* --------------------------------------------------------------------- */

// begin
   // put("Analyseur lexical : Machine_Caracteres. ");
   // put_line("Copyright UJF - UFR IMAG.");
// end lecture_caracteres;


   //Retourne 1 si le fichier existe
   int fileExists(char *fileName)
   {
      int retour = 0;

      if(access(fileName, F_OK) == 0)
      {
         //Le fichier existe
         retour = 1;
      }
      return retour;
   }