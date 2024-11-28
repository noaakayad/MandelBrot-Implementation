#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double alea(double a, double b){
	double taille;
	double random_hors_intervalle;
	if (a > b){
		double save = a;
		a = b;
		b = save;
	}

	taille = b - a;
	random_hors_intervalle = (double) random() / (double) RAND_MAX;
	return a + random_hors_intervalle * taille;
}
/* 
Étant donné que random() ne peut que renvoyer un nombre aléatoire entre 0 et RAND_MAX, ainsi on va utiliser ce nombre 
aléatoire et le diviser par RAND_MAX pour qu'il devienne un nb aléatoire entre 0 et 1. Par la suite on le multiplie
par la taille de l'intervalle [a;b] pour qu'il puisse être entre 0 et taille. Enfin, on l'additionne à a afin que le nb
aléatoire soit un nb aléatoire entre a et b.

En d'autre termes c'est comme si initiallement notre nb aléatoire r était tel que 0 <= r <= RAND_MAX,
Puis r := r/RAND_MAX, et alors : 0 <= r <= 1.
Puis r := r * taille, et alors : 0 <= r <= taille.
Puis r := a + r * taille, et alors : a <= r <= taille + a, c'est à dire : a <= r <= b. 
D'où l'obtention d'un nb aléatoire entre a et b
*/

/* De plus dans la fonction alea, on vérifie si a<b pour
s'assurer que la taille qu'on calcule ne soit pas négatif,
ce qui changerait l'entièreté du raisonnement et 
du résultat */

void affiche_tab (int n, double* tab){
	int i = 0;

	while (i < n){
		if (i%5 != 0){
			printf("%.2f  ", tab[i]);
			i = i+1;
		}
		else {
			printf("\n");
			printf("%.2f  ", tab[i]);
			i = i+1;
		}
	}
}
/* Pour afficher les cases du tableau 5 à 5, on utilise un compteur i initialement à 0, de sorte que tant que i 
ne soit pas plus grand que la taille du tableau, si i n'est pas multiple de, on affiche la valeur du tableau à la case i
tout en mettant un espace pour que les résultat ne soit pas collés entre eux, et on incrémente i de 1. Et si i est 
un multiple de 5, cela signifie qu'on a affiché 5 cases du tableau, et ainsi, on fait un retour à la ligne et on affiche 
la case i du tableau, et enfin on incrémente i de 1.
*/

void make_tab_alea_100(void){
	double* tab = malloc (100*sizeof(double));
	
	int i = 0;
	while (i < 100){

		tab[i] = alea(-1.0, 1.0);
		i = i + 1;
	}

	affiche_tab(100, tab);
	free(tab);
}
/* ici, on va créer un tableau de 100 doubles, puis chaque case du tableau prendra une valeur aléatoire entre -1 et 1 
grâce à la fonction alea qui renvoit ce nb aléatoire. Et enfin, on affiche 5 à 5 les valeurs de ce tableau grâce à la
fonction affiche_tab.
*/

int main (void){
	srandom((unsigned)time(NULL));
	/*cette commande permet que le résultat de random() soit différent à chaque appel, grâce à l'utilisation du temps.*/

	make_tab_alea_100();

	return 0;
}
