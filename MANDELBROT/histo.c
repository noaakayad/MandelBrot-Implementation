/* Travail réalisé sur un Mac Intel core i5 avec MacOS Ventura */

/* Auteur : AKAYAD NOA
   Date : <date du rende>

   compilation avec la commande : gcc -W -Wall -ansi -pedantic histo.c -o histo
   exécution avec la commande : ./histo

    Ce programme permet de générer un histogramme de valeurs aléatoires comprises entre 0 et 80, de voir le min, le max, et la moyenne de cette histogramme, et de voir si l'histogramme a passé le teste du khi2.
    ---------------------------------------------------------------------------
    On apprend comment créer et afficher un histogramme pour évaluer l'uniformité d'un générateur aléatoire en langage C.
    On apprend l'importance des grandeurs statistiques, comme la moyenne, l'écart-type et le test du khi2 , pour vérifier la qualité d'une séquence de nombres aléatoires.
    On apprend les bases de la génération de nombres aléatoires en informatique et comment les fonctions telles que random() en C fonctionnent.

    On constate que l'affichage de l'histogramme se fait bien, que la moyenne est toujours 12,5. Mais on constate aussi que l'histogramme ne passe pas tout le temps le test du khi2, ce qui permet de critiquer la qualité d'une séquence de nombres aléatoires.

    Estimation de ma note : 20/20
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define ALEA_MAX 80
#define T_MAX 1000

void remplir_histo(int* tab, int compteur){
	while (compteur < T_MAX){
		double r = (double) random() / RAND_MAX;
		int random_79;
		r = r*79;
		random_79 = (int) r;
		tab[random_79] ++;
		compteur++;
	}
}
/* pour remplir l'histogramme, à chaque itération, j'utilise le même raisonnement que dans alea.c pour avoir mon nb aléatoire r compris
entre 0 et 79, et je rajoute 1 dans la case de l'histogramme correspondant à ce nb aléatoire jusqu'à arriver à la 
compteur-ième itération.*/

double khi2 (int* T, int n, int v){
	int s = 0;
	int i;

	for (i = 0; i < v; i++){
		s = s + T[i]*T[i];
	}

	return ((double) v / n)*s - n;
}
/* ici, je ne fais qu'appliquer l'expression donnée par l'énoncé, sans oublier de rendre v/n en type double car comme v et 
n sont des entiers et que v < n, on a forcémment que v/n = 0 car l'opération "/" sur des entiers renvoit la partie 
entière du résultat de l'opération.*/
 
double variance (int* T, int sz){
	double m = 0.0;
	double s = 0.0;
	int j = 0;

	while (j < sz){
		double v = T[j];
		double m_prev = m;
		j++;
		m = m + (v - m)/j;
		s = s + (v - m)*(v - m_prev);
	}

	return s/(j-1);
}
/* encore une fois, je ne fais qu'implémenter l'algorithme présent dans l'énoncé.*/

void pass_fail(int* T, int n, int v){
	if (n > 10*v){
		if (fabs(khi2(T, T_MAX, ALEA_MAX) - v) <= 2*sqrt(v)){
			printf("le générateur passe le test du khi2\n");
		}
		else {
			printf("le générateur ne passe pas le test du khi2\n");
		}
	}
}
/* Ici, j'interprète la distance comme la valeur absolue de la différence.*/


int main (void){
/* Déclaration des variables :
*/
	int histo[ALEA_MAX];
	int min = RAND_MAX;
	int max = 0;
	double moyenne = 0.0;
	int hmax = 20;

/* Déclaration des variables pour les boucles for :
*/
	int k;
	int l;
	int i;
	int j;


	srandom((unsigned)time(NULL));

	for (j = 0; j < ALEA_MAX; j++){
		histo[j] = 0;
	}

	remplir_histo(histo, 0);
/* Cette version n'utilise pas v car j'utilise une fonction en dehors du main. On peut également utiliser
v mais il ne faut plus de fonction et écrire ce qu'il y a dans ma fonction en remplaçant r par v.*/


	for (i = 0; i < ALEA_MAX; i++){
		if (histo[i] <= min){
			min = histo[i];
		}
		if (histo[i] >= max){
			max = histo[i];
		}

		moyenne = moyenne + histo[i];
		if (i == ALEA_MAX - 1){
			moyenne = moyenne / ALEA_MAX;
		}
	}
/* Pour le calcul du minimum, je déclare un entier nommé min qui vaut initialement +inf. Ainsi, si je trouve une valeur 
plus petite dans l'histogramme (ce qui arrivera forcément puisque min = +inf en début d'exécution), alors min prendra 
cette valeur. Ce processus se répète pour les autres valeurs de l'histogramme grâce à la boucle for.
De la même manière, pour le max, je le définis initialement à 0, car c'est la plus petite valeur possible de 
l'histogramme. Cela garantit que max sera bien modifié en début d'exécution.
Pour la moyenne, je calcule la somme des éléments du tableau. Une fois que l'on arrive à la dernière itération 
(i = ALEA_MAX-1), cela signifie que nous avons terminé le calcul de la somme des éléments du tableau. Il suffit alors 
de diviser par le nombre d'éléments dans le tableau (ALEA_MAX) pour obtenir la moyenne.*/

	for(k = hmax; k > 0; k--){
		for (l = 0; l < ALEA_MAX; l++){
			if (histo[l] >= k){
				printf("#");
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	}
/* Pour afficher l'histogramme, je raisonne de la façon suivante : D'abord, l'histogramme aura un maximum de hmax lignes. 
Ensuite, on commence par le haut de l'affichage : la ligne hmax. Pour chaque case de l'histogramme, si sa valeur 
est >= hmax , alors sa colonne dans l'affichage ira au minimum jusqu'à la valeur hmax, donc on affiche un "#". Si la case 
a une valeur < hmax, on n'affiche pas de "#", car sa colonne n'ira pas jusqu'à hmax. Ce processus est répété pour toutes
les cases du tableau grâce à la seconde boucle for. 
Après avoir identifié les colonnes qui atteignent hmax , on examine celles qui vont jusqu'à hmax −1, et ainsi de suite,
jusqu'à la valeur 0. Ce décompte est réalisé grâce à la première boucle for.
*/

	printf("min : %d, max : %d, moyenne : %lf\n", min, max, moyenne);
	printf("khi2 : %lf, variance : %lf, écart-type : %lf\n", khi2(histo, T_MAX, ALEA_MAX), variance(histo, ALEA_MAX), sqrt(variance(histo, ALEA_MAX)));
	pass_fail(histo, T_MAX, ALEA_MAX);

	return 0;
}
