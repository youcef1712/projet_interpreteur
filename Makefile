all: application

analyse_lexicale.o: analyse_lexicale.c analyse_lexicale.h lecture_caracteres.h
	gcc -g -Wall -c analyse_lexicale.c

lecture_caracteres.o: lecture_caracteres.h lecture_caracteres.c
	gcc -g -Wall -c lecture_caracteres.c

listeVariables.o: listeVariables.c listeVariables.h
	gcc -g -Wall -c listeVariables.c

arbresAbstraits.o: arbresAbstraits.c arbresAbstraits.h listeVariables.c listeVariables.h
	gcc -g -Wall -c arbresAbstraits.c

analyse_syntaxique.o: analyse_syntaxique.c analyse_syntaxique.h analyse_lexicale.c analyse_lexicale.h lecture_caracteres.h arbresAbstraits.c arbresAbstraits.h
	gcc -g -Wall -c analyse_syntaxique.c

evaluation.o: evaluation.c evaluation.h analyse_syntaxique.c analyse_syntaxique.h analyse_lexicale.c analyse_lexicale.h lecture_caracteres.h arbresAbstraits.c arbresAbstraits.h listeVariables.c listeVariables.h
	gcc -g -Wall -c evaluation.c

application: application.o analyse_syntaxique.o analyse_lexicale.o lecture_caracteres.o evaluation.o arbresAbstraits.o listeVariables.o
	gcc -g -Wall -o application application.o evaluation.o analyse_syntaxique.o arbresAbstraits.o analyse_lexicale.o lecture_caracteres.o listeVariables.o

clean:
	rm -f application *.o
