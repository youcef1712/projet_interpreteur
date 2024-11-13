

   typedef enum {
	  ERREUR_CAR,     // caractere incorrect
	  CHIFFRE,			// chiffre 
	  SYMBOLE,			// symbole,
	  LETTRE,			// lettre (premiere lettre d'un identificateur de variable),
     SEPARATEUR,
     C_FIN_SEQUENCE   // caractere de fin de sequence     
   } Nature_Caractere ;

   typedef enum
   {
      ENTIER,                              // sequence de chiffres avec ou sans virgule
      CHAINE_DE_CARACTERES,                // chaine de caracteres
      LEXEME_VALEUR_NULL,                  // valeur null
      PLUS,                                // +
      MOINS,                               // -
      MUL,                                 // *
      DIV,                                 // /
      PARO,                                // (
      PARF,                                // )
      IDENTIFICATEUR_VARIABLE_OU_FONCTION, // identificateur d'une variable (tout depend si il y a une parenthese derriere)
      TYPE_INT,                            // type de variable int
      TYPE_DOUBLE,                         // type de variable double
      TYPE_STRING,                         // type de variable string
      IDENTIFICATEUR_IF,                   // identificateur d'une boucle if
      IDENTIFICATEUR_ELSE,                 // identificateur de la seconde partie d'une boucle if
      IDENTIFICATEUR_WHILE,                // identificateur d'une boucle while
      EGAL,                                // Egal
      DOUBLE_EGAL,              // Double Egal
      DIFFERENT,   // !=
      LEXEME_SUPERIEUR,         // >
      LEXEME_SUPERIEUR_OU_EGAL, // >=
      LEXEME_INFERIEUR,         // <
      LEXEME_INFERIEUR_OU_EGAL, // <=
      ACO_OUVRANT,              // Accolade ouvrante
      ACO_FERMANT,              // Accolade fermante
      VIRGULE,                  // virgule
      POINT_VIRGULE,            // point-virgule
      FIN_SEQUENCE,             // pseudo lexeme ajoute en fin de sequence
      LEXEME_INDEFINI,
      ERREUR
   }
   Nature_Lexeme;

   typedef struct { 
      Nature_Lexeme nature;    // nature du lexeme
      unsigned int ligne;           // numero de ligne
      unsigned int colonne;         // numero de colonne
      char *chaine;           // chaine de caracteres
      double valeur;          // valeur d'un entier

   } Lexeme ; 

   void afficher(Lexeme l);

   void passerSeperateurs();

   //Retourne 1 si le fichier a ete correctement ouvert, sinon 0
 int demarrer(char *nom_fichier);
   // e.i. : indifferent
   // e.f. : la sequence de lexemes est lue dans le fichier designe par
   //        nom_fichier
   //        fin_de_sequence <=> lexeme_courant.nature = FIN_SEQUENCE
   //        (non fin_de_sequence) => lexeme courant est le premier
   //        lexeme de la sequence
   //        l'exception Erreur_Lexicale est levee en cas d'erreur

   void avancer();
   // pre-condition : la machine sequentielle est demarree
   // e.i. : on pose i = rang du lexeme_courant :
   //        (non fin_de_sequence)
   //        et (non lexeme_courant.nature = FIN_SEQUENCE)
   // e.f. : fin_de_sequence <=> lexeme_courant.nature = FIN_SEQUENCE
   //        (non fin_de_sequence) => lexeme_courant est le lexeme i+1
   //        l'exception Erreur_Lexicale est levee en cas d'erreur

   Lexeme lexeme_courant();
   // pre-condition : la machine sequentielle est demarree
   // lexeme_courant est :
   // . le pseudo lexeme FIN_SEQUENCE si fin_de_sequence
   // . le pseudo lexeme ERREUR si une erreur a ete detectee
   // . le lexeme de rang i sinon

   int fin_de_sequence();
   // pre-condition : la machine sequentielle est demarree
   // fin_de_sequence vaut :
   // . vrai si tous les lexemes de la sequence ont ete reconnus
   // . faux sinon

   void arreter();



   // cette fonction ajoute le caractere c a la fin de la chaine s dont l'adresse du poiteur est passee en parametre

void ajouter_caractere(char **s, char c);