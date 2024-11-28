#include <stdio.h>



int main (){
	char lettre = 'a';
	int x1 = 1;
	short x2 = 2;
	long x3 = 3;
	float x4 = 4.0;
	double x5 = 5.0;
	char x6[11] = "20/20 svp!";
	/* On met 11 et non 10 afin de réserver un espace
	pour le caractère de terminaison '\0' (la sentinel)
	qui va marquer la fin de la chaîne.*/
	
	printf("le caractère stocké à l'adresse %p, vaut %c, et occupe %lu octet(s) en mémoire\n", (void *)&lettre, lettre, sizeof(lettre));
	printf("l'entier court stocké à l'adresse %p, vaut %d, et occupe %lu octet(s) en mémoire\n", (void *)&x1, x1, sizeof(x1));
	printf("l'entier stocké à l'adresse %p, vaut %d, et occupe %lu octet(s) en mémoire\n", (void *)&x2, x2, sizeof(x2));
	printf("l'entier long stocké à l'adresse %p, vaut %ld, et occupe %lu octet(s)en mémoire\n", (void *)&x3, x3, sizeof(x3));
	printf("le nombre floattant simple précision stocké à l'adresse %p, vaut %f, et occupe %lu octet(s) en mémoire\n", (void *)&x4, x4, sizeof(x4));
	printf("le nombre floattant double précision stocké à l'adresse %p, vaut %lf, et occupe %lu octet(s) en mémoire\n", (void *)&x5, x5, sizeof(x5));
	printf("la chaine de caractères stockée à l'adresse %p, contient \"%s\", et occupe %lu octet(s) en mémoire\n", (void *)x6, x6, sizeof(x6));
	/* On ne met pas de & devant x6 pour afficher
	l'adresse car les crochets devant x6 dans sa 
	déclaration montrent que x6 est un tableau de 
	caractères, et donc est déjà un pointeur, d'où la 
	raison pour laquelle on ne met pas de & pour 
	afficher l'adresse.*/
}

/*Les chiffres ne sont pas les mêmes car nous n'avons pas 
donné les mêmes valeurs à nos variables, et parce que la 
mémoire n'est pas forcément stockée au même endroit dans
chaque machines.*/

/*Les octets sont les mêmes car ils dépendent uniquement 
des types utilisés.*/

/*Pour remedier aux changements de valeurs, on peut
utiliser des allocations statiques plutôt que dynamiques,
cela permet de rendre la mémoire plus prévisible.*/

