#include <stdio.h>

void passe_valeur(int a){
	printf("l'adresse est %p, et la valeur est %d\n", &a, a);
	a = a + 5;
}

int retourne_valeur(int a){
	printf("l'adresse est %p, et la valeur est %d\n", &a, a);
	
	a = 7*a;
	
	return a;
}

void passe_reference(int* a){
	printf("l'adresse est %p, et la valeur est %d\n", a, *a);

	*a = *a + 11;

}

int main (){
	int j = 1 ;
	printf("\n\nLa variable stockée à l'adresse %p contient la valeur %d\n", (void*)&j, j) ;
	printf("\n") ;

	printf("Appel à passe_valeur() ...\n") ;
	passe_valeur(j) ;
	printf("nouvelle valeur de j : %d\n", j) ;
	printf("\n") ;

	printf("Appel à retourne_valeur() ...\n") ;
	j = retourne_valeur(j) ;
	printf("nouvelle valeur de j : %d\n", j) ;
	printf("\n") ;

	printf("Appel à passe_reference() ...\n") ;
	passe_reference(&j) ;
	printf("nouvelle valeur de j : %d\n", j) ;
	printf("\n") ;
	return 0;
}

/* 
5) On voit que passe_valeur n'a pas modifié j à 
l'inverse de retourne_valeur ou de passe_référence, 
tout simplement car dans passe_valeur, on ne modifie pas
j, mais une "copie locale" de j qu'on prend en entrée,
toute modification apportée à cette copie n'affecte
pas j; 
dans retourne_valeur,on ne modifie pas directement
j pour les mêmes raisons qu'avec passe_valeur,
or on retourne une valeur, ainsi en appliquant :
j = retourne_valeur(), on a bien modification de j,
mais seulement grâce à cette opération.
Pour modifier directement j, il faut passer par le
pointeur de j, comme on le fait dans passe_référence,
et c'est pourquoi passe_référence modifie bien j. 

6) Tableau_init_alea() modifie bien le tableau car
un tableau fonctionne comme un pointeur, en effet 
un tableau est une combinaison, un enchaînement 
d’adresse d’élément. Lorsqu'une fonction prend en entré un
tableau, on passe en réalité en entrée
l'adresse de sa première case, donc un pointeur vers
le tableau. D'où le fait que le contenu du tableau est 
bien modifié par la fonction (en passant par les pointeurs 
on a bien modification du paramètre pris en entré plutôt 
qu'une modification d'une copie faite de ce dernier).
*/






