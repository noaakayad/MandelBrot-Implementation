#include <stdio.h>

int main (){
	int T[42];
	T[0] = 0;
	int i = 1;

	while (i < 42){
		T[i] = 2 * T[i-1] + 2;
		i = i + 1;
	}

	for (int j = 0; j < 42; j++){
		printf("élément à l'indice %d (%p) : %d\n", j, &T[j], T[j]);
	}
	return 0;
}

/* L'écart entre les adresses de deux éléments 
consécutifs du tableau est constant. 

Cet écart est exactement la taille d'un int (4 octets)
comme le montre l'exercice précédent.

Ainsi cet écart s'explique par le fait que pour tout i,
si T[i] est enregistrer à une certaine adresse,
alors T[i+1] sera enregistrer à l'adresse : 
adresse de T[i] + 4 octets.*/


