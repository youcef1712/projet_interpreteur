

/* Programme principal pour la calculette */

#include <stdio.h>

#include "analyse_syntaxique.h"

int main (int argc, char *argv[]) {
   int resultat ;

   if (argc < 2) {
	printf("Donner le nom de fichier !\n") ;
	return 1 ; 
   } ;

   analyser(argv[1], &resultat) ;
   printf("le resultat est : %d\n", resultat) ; 
   return 0 ;
}