#include <stdio.h>

/* Cette fonction permet d'afficher les valeurs et 
les adresses présentent dans un tableau d'entier */
void tableau_affiche(int* T, int sz){
	for (int i = 0; i < sz; i++){
		printf("élément à l'indice %d (%p) : %d\n", i, &T[i], T[i]);
	}
}

/* Si la fonction tableau_affiche est défini après le 
main, on ne peut pas l'appeler dans le main car elle 
est définie après le main, ce qui crée un problème de 
compilation. */

/* Cette fonction permet de modifier un tableau prit 
entré en fonction de la formule mathématique donné par 
l'énoncé */
void tableau_init(int* T, int sz, int t0){
	T[0] = t0;
	int i = 1;

	while (i < 42){
		T[i] = 2 * T[i-1] + 2;
		i = i + 1;
	}
}

int main (){
	int T[42];
	tableau_init(T, 42, 0);
	tableau_affiche(T, 42);
	return 0;
}
