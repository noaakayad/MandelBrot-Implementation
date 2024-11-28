/* Travail réalisé sur un Mac Intel core i5 avec MacOS Ventura */

/* Auteur : AKAYAD NOA
   Date : 30/11/23


   compilation avec la commande : gcc -W -Wall -ansi -pedantic mandel.c -o mandel
   exécution avec la commande : ./mandel 

   Ce programme permet de dessiner des partie de la fractale de Mandelbrot.
   On apprend à manier l'allocation de mémoire.
   On apprend à modifier ou créer un fichier depuis un code.
   On apprend à dessiner la fractale.*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define STRMAX 256
#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 1000
#define BW 0
#define BW_ALTERN 1 
#define GREY_ST 2

struct camera {
	double x;
	double y;
	double height;
	double width;
};

struct render{
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	int H;
	int W;
	int iter_max;
	double radius;
	int* img;
	char basename[STRMAX];
	struct camera pov;
	int type;
};

double map (int v, int imin, int imax, double omin, double omax){
	double taille_i = imax - imin; /* Je calcule la taille de l'intervalle de départ. */
	double taille_vi = (double)v - imin; /* Je calcule l'écart entre v et imin. */
	double taille_o = omax - omin; /* Je calcule la taille pour l'intervalle d'arrivée. */
	double pourcentage_vi = taille_vi / taille_i; /* Je calcule le pourcentage de v dans l'intervalle de départ. */

	return pourcentage_vi * taille_o + omin; /* Ce pourcentage étant le même pour la valeur proportionnelle dans toute intervalle, on retrouve cette valeur proportionnelle en faisant le chemin inverse au calcul de taille_vi mais appliqué à l'intervalle d'arrivée. */
}

void render_init(struct render *set, int argc, char *argv[]){
	/* Initialisation des termes de set non présents dans les arguments */
	set->radius = 2.0;
	set->type = GREY_ST;
	set->img = malloc((set->W * set->H) * sizeof(int));
    if (set->img == NULL){
        printf("Erreur d'allocation mémoire !\n");
        exit(1);
    }

    /* Application des arguments */
	if (argc > 1){
		if (sscanf(argv[1],"%d", &set->iter_max) != 1){ /* Si sscanf(...) = 0, c'est qu'il y a une erreur de format dans le renseignement de l'interlocuteur. */
			printf("Erreur, format incorrect pour le nb d'iterations. C'est un entier positif.\n");
			exit(1); /* On prévient ainsi l'interlocuteur et le programme s'arrête. */
		}
	}
	else {
		set->iter_max = 100; /* Si l'argument n'est pas fourni par l'interlocuteur, on applique la valeur par défaut. */
	}

	if (argc > 2){
		if (sscanf(argv[2], "%lf,%lf,%lf,%lf", &set->pov.x, &set->pov.y, &set->pov.width, &set->pov.height) != 4){ /* Si sscanf(...) = 0, c'est qu'il y a une erreur de format dans le renseignement de l'interlocuteur. */
			printf("Erreur format incorrect pour les paramètres de la caméra. Vous devez renseigner un réel x représentant la partie réelle de z = x + iy le point au centre dans le plan de l'image, suivit d'une virgule et de y la partie imaginaire de z, suivit d'une virgule et de la largeur du rectangle de visualisation (entier positif), et suivit d'une virgule et de la longueur du rectangle de visualisation (entier positif).\n");
			exit(1); /* On prévient ainsi l'interlocuteur et le programme s'arrête. */
		}
	}
	else {
		set->pov.x = -0.76;
		set->pov.y = 0;
		set->pov.width = 2.48;
		set->pov.height = 2.48; /* Si l'argument n'est pas fourni par l'interlocuteur, on applique la valeur par défaut. */
	}

	if (argc > 3){
		if (sscanf(argv[3], "%dx%d", &set->W, &set->H) != 2){ /* Si sscanf(...) = 0, c'est qu'il y a une erreur de format dans le renseignement de l'interlocuteur. */
			printf("Erreur format incorrect pour la taille de l'image, vous devez renseigner une taille représentant la longueur (un entier strictement positif) suivit de la lettre x et suivit de la hauteur (un entier strictement positif). Exemple : 1000x1000.\n");
			exit(1); /* On prévient ainsi l'interlocuteur et le programme s'arrête. */
		}
		if (set->W <= 0 || set->H <= 0){
			printf("Erreur format incorrect pour la taille de l'image, vous devez renseigner une taille représentant la longueur (un entier strictement positif) suivit de la lettre x et suivit de la hauteur (un entier strictement positif). Exemple : 1000x1000.\n");
			exit(1); /* Autre erreur possible, si les entiers ne sont pas strictement positifs, on fait la même procédure. */
		}
	}
	else {
		set->W = DEFAULT_WIDTH;
		set->H = DEFAULT_HEIGHT; /* Si l'argument n'est pas fourni par l'interlocuteur, on applique la valeur par défaut. */
	}

	if (argc > 4){
		/* Initialisation des variables. */
		int i = 0;
		int j = 0;
		int taille = strlen(argv[4]); 
		char* intermediaire = malloc (taille * sizeof(char)); 

		while (i < taille && argv[4][i] == ' '){
			i++;
		} /* On passe les premiers ' ' précédent la première lettre du mot. */

		for (; i < taille; i++){
			if (argv[4][i] != ' '){
				intermediaire[j] = argv[4][i];
				j++; /* Si la lettre en cours de parcours n'est pas un ' ' alors il faut la copier dans intermédiaire. */
			}
			else if (j == 0 || intermediaire[j-1] != '_'){
				intermediaire[j] = '_';
				j++;	
			} /* Si j = 0, alors on vient de commencer la boucle, autrement dit on parcours le premier caractère du mot différent de ' ' donc on l'ajoute au mot. 
			Si j > 0 et qu'on parcours un ' ' alors si son caractère précédent est différent de '_', c'est qu'on est au premier espace suivant le premier caractère différent de ' ' du mot, ainsi on ajoute '_' dans intermédiaire.
			Pour la suite du parcours, si on tombe sur un caractère étant un ' ' et précédé par '_' c'est qu'on a un 2e espace dans le mot, et alors on l'ignore. */ 
		}		

    	strncpy(set->basename, intermediaire, STRMAX - 5); /* on nomme set->basename par l'intermédiaire calculé. */

    	free(intermediaire); /* on libère la mémoire alloué pour l'intermédiaire. */
	}
	else {
		strncpy(set->basename, "mandel", STRMAX); /* Si l'argument n'est pas fourni par l'interlocuteur, on applique la valeur par défaut. */
	}
}

int m2v (struct render *set, int x, int y){
	return y*set->W + x; /* En considérant l'axe x comme les colonnes et l'axe y comme les lignes, alors on déduit directement la formule précédente. */
}


void render_image(struct render* set){
	/* Initialisation des variables. */
	int Py;
	int Px;

	for (Py = 0; Py < set->H; Py++){
		for (Px = 0; Px < set->W; Px++){
			/* Application de l'algorithme */
			double x = map(Px, 0, set->W, set->xmin, set->xmax);
			double y = map(Py, 0, set->H, set->ymin, set->ymax);
			int iteration = 0;

			double xn0 = x;
			double yn0 = y;

			int indice = m2v(set, Px, Py); /* indice du pixel dans set->img. */

			while(sqrt(xn0*xn0 + yn0*yn0) < set->radius && iteration < set->iter_max){
				double save_x = xn0;
				double save_y = yn0; 
				xn0 = save_x*save_x - save_y*save_y + x;
				yn0 = 2*save_x*save_y + y;
				iteration ++;
			}

			set->img[indice] = iteration;
		}
		printf("%d \r", Py);
	}
}

void get_pixel_color(struct render *set, int Px, int Py, int* r, int* g, int* b){
	int indice = m2v(set, Px, Py);
	int iteration = set->img[indice];

	if (iteration == set->iter_max){ /* En effet cette condition suffit à savoir si le point est dans l'ensemble car si set->img[indice] = set->iter_max, c'est que la boucle while de l'agorithme a pu aller jusqu'à l'itération maximale, et que donc le module de zn est resté inférieur à set->radius, ce qui signifie qu'il appartient à l'ensemble. */
			*r = 0;
			*g = 0;
			*b = 0;
	}

	else if (set->type == BW){
           *r=255;
           *g=255;
           *b=255;
		}

	else if (set->type == BW_ALTERN){
		if (iteration%2 == (set->iter_max)%2){
			*r = 0;
			*g = 0;
			*b = 0;
		}
		else {
			*r = 255;
			*g = 255;
			*b = 255;				
		}
	}

	else if (set->type == GREY_ST) { 
		if (iteration < 256){
			*r = iteration;
			*g = iteration;
			*b = iteration;
		}
		else {
			*r = iteration % 255;
			*g = iteration % 255;
			*b = iteration % 255;
		}
	}

}

void save_image(struct render *set){
	/* Initialisation des variables */
	FILE* fout;
	int Py;
	int Px;

	strcat(set->basename, ".ppm"); /* ajout de l'extension. */

	fout = fopen(set->basename, "w"); /* ouverture du fichier en mode écriture. */
	if (fout == NULL){
		printf("Impossible d'ouvrir le fichier\n");
		return ;
	}

	/* ajout des en-têtes et des commentaires. */
	fprintf(fout, "P3\n");
	fprintf(fout, "%d %d\n", set->W, set->H);
	fprintf(fout, "255\n");
	fprintf(fout, "#Ici le nb max d'itération est max_iter autrement dit %d.#\n", set->iter_max);
	fprintf(fout, "#La zone dans le plan complexe explorée est l'ensemble des z = x + iy tel que 𝑥∈[%lf;%lf] et 𝑦∈[%lf;%lf].#\n", set->xmin, set->xmax, set->ymin, set->ymax);


	for (Py = 0; Py < set->H; Py++){
		for (Px = 0; Px < set->W; Px++){
			int r;
			int g;
			int b;

			get_pixel_color(set, Px, Py, &r, &g, &b); /* Récupération de la couleur du pixel. */
			fprintf(fout, "%d %d %d ", r, g, b); /* ajout du pixel colorié dans l'image. */
		}
		fprintf(fout, "\n");
	}

	fclose(fout); /* fermeture du fichier. */
}

void cam2rect (struct render *set, struct camera *pov){
	set->xmin = pov->x - (pov->width)/2;
	set->xmax = pov->x + (pov->width)/2;
	set->ymin = pov->y - (pov->height)/2;
	set->ymax = pov->y + (pov->height)/2;
}

int main(int argc, char *argv[]){

	struct render set;

	render_init(&set, argc, argv);

	cam2rect(&set, &set.pov);
 
	render_image(&set);

	save_image(&set);

	free(set.img);

	return 0;
}











