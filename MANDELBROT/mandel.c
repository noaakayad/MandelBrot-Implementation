/* Travail réalisé sur un Mac Intel core i5 avec MacOS Ventura 

 	Auteur : AKAYAD NOA
   Date : 30/11/23


   compilation avec la commande : gcc -W -Wall -ansi -pedantic mandel.c -o mandel
   exécution avec la commande : ./mandel 

   Ce programme permet de dessiner des partie de la fractale de Mandelbrot.
   On apprend à manier l'allocation de mémoire.
   On apprend à modifier ou créer un fichier depuis un code.
   On apprend à dessiner la fractale.


	
	Réponses aux questions de l'énoncé : 
*/




#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "colors.h"

#define STRMAX 256
#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 1000
#define BW 0
#define BW_ALTERN 1
#define GREY_ST 2 
#define GREY_SM 3
#define RGB 4


/* Types : */

struct pixdiv {
	int iter;
	double x;
	double y;
};

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
	struct pixdiv* img;
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


void set_defaults (struct render *set){
	/* Application des variables de bases */
	set->radius = 2.0;
	set->W = DEFAULT_WIDTH;
	set->H = DEFAULT_HEIGHT;
	set->type = RGB;
	set->img = malloc((set->W * set->H) * sizeof(struct pixdiv));
   if (set->img == NULL){
       printf("Erreur d'allocation mémoire !\n");
       exit(1);
   }
   set->iter_max = 100;
	set->pov.x = -0.76;
	set->pov.y = 0;
	set->pov.width = 2.48;
	set->pov.height = 2.48;
	strncpy(set->basename, "mandel", STRMAX);
}


/* Cette fonction permet de savoir si une chaine de caractère est un nombre ou un string. */
int int_or_str(char* str) {
	int taille = strlen(str);
	int i;
    
   for (i = 0; i < taille; i++) {
   	if (str[i] < '0' || str[i] > '9') {
      	return 0;
		}
    }

   return 1;
}



int load_config(struct render *set, char *config_file){
	/* Décalration des variables */
	FILE* fin;
	char ligne[STRMAX];
	int nb_ligne = 0;


	/* Lecture du fichier */
	fin = fopen(config_file, "r");
	if (fin == NULL){
		printf("Probleme d'ouverture\n");
		return 0;
	}


	/* Dans cette boucle while, j'utilise le compteur nb_ligne que j'incrémente dès qu'on a fini la lecture d'une ligne du fichier, pour ainsi savoir quel action effectuer sur chaque ligne. */
	while (fgets(ligne, sizeof(ligne), fin) != NULL && nb_ligne < 6){

		if (nb_ligne == 0) {
		/* On est sur la premiere ligne, il n'y a que le nom du fichier. */
		   char intermediaire[STRMAX]; /* comme dans render init, j'utilise un intermediaire pour copier le nom du fichier */
		   if (sscanf(ligne, "%s", intermediaire) != 1) {
		       fclose(fin);
		       return 0;
		   }
		   strncpy(set->basename, intermediaire, STRMAX);
		}


		if (nb_ligne == 1){
		/* on est sur la 2e ligne, il n'y a que les dimensions de l'image */
			if (sscanf(ligne, "%dx%d", &set->W, &set->H) != 2){
				fclose(fin);
				return 0;
			}
			free(set->img); /* Adaptation de set->img grâce à l'obtention des nouvelles dimensions. */
			set->img = malloc((set->W * set->H) * sizeof(struct pixdiv));
   		if (set->img == NULL){
       		printf("Erreur d'allocation mémoire !\n");
       		exit(1);
   		}
		}


		if (nb_ligne == 2){
		/* on est sur la 3e ligne, c'est la ligne du type de set. */
    		ligne[strcspn(ligne, "\r\n")] = '\0';  
    		
			if (strcasecmp(ligne, "rgb") == 0){
				set->type = RGB;
			}
			else if (strcasecmp(ligne, "b&w") == 0){
				set->type = BW;
			}
			else if (strcasecmp(ligne, "b&w_alt") == 0){
				set->type = BW_ALTERN;
			}
			else if (strcasecmp(ligne, "grey_stepped") == 0){
				set->type = GREY_ST;
			}
			else if (strcasecmp(ligne, "grey_smoothed") == 0){
				set->type = GREY_SM;
			}

			else {
				fclose(fin);
				printf("Erreur de lecture du type : '%s'\n", ligne);
				return 0;
			}
		}


		if (nb_ligne == 3){
		/* 3e ligne, c'est la ligne des paramètres de la caméra. */
			if (sscanf(ligne, "%lf,%lf,%lf,%lf", &set->pov.x, &set->pov.y, &set->pov.width, &set->pov.height) != 4){
				fclose(fin);
				return 0;
			}
		}


		if (nb_ligne == 4){
		/* 4e ligne, cest la ligne de l'iteration maximale. */
			if (sscanf(ligne, "%d", &set->iter_max) != 1){
				fclose(fin);
				return 0;
			}
		}


		if (nb_ligne == 5){
		/* 5e ligne, c'est la ligne du rayon. */
			if (sscanf(ligne, "%lf", &set->radius) != 1){
				fclose(fin);
				return 0;
			}
		}

		/* Maintenant qu'on a lu une ligne, on incrémente le compteur puis on passe à la prochaine ligne grâce au while. */
		nb_ligne++;
	}

	/* fermeture du fichier. */
	fclose (fin);
	return 1;
}



void render_init(struct render *set, int argc, char *argv[]){
	set_defaults(set);

/* Application des arguments */
	if (argc > 1){
		if (int_or_str(argv[1]) == 0){
			if (load_config(set, argv[1]) == 0){
				printf("0\n");
				set_defaults(set);
			}
		} 
		else {
			if (sscanf(argv[1],"%d", &set->iter_max) != 1){ /* Si sscanf(...) = 0, c'est qu'il y a une erreur de format dans le renseignement de l'interlocuteur. */
				printf("Erreur, format incorrect pour le nb d'iterations. C'est un entier positif.\n");
				exit(1); /* On prévient ainsi l'interlocuteur et le programme s'arrête. */
			}
		}
	}

	if (argc > 2){
		if (sscanf(argv[2], "%lf,%lf,%lf,%lf", &set->pov.x, &set->pov.y, &set->pov.width, &set->pov.height) != 4){ /* Si sscanf(...) = 0, c'est qu'il y a une erreur de format dans le renseignement de l'interlocuteur. */
			printf("Erreur format incorrect pour les paramètres de la caméra. Vous devez renseigner un réel x représentant la partie réelle de z = x + iy le point au centre dans le plan de l'image, suivit d'une virgule et de y la partie imaginaire de z, suivit d'une virgule et de la largeur du rectangle de visualisation (entier positif), et suivit d'une virgule et de la longueur du rectangle de visualisation (entier positif).\n");
			exit(1); /* On prévient ainsi l'interlocuteur et le programme s'arrête. */
		}
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
		free(set->img);
		set->img = malloc((set->W * set->H) * sizeof(struct pixdiv));
   	if (set->img == NULL){
    		printf("Erreur d'allocation mémoire !\n");
 	   	exit(1);
  		}
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
			int i;

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


			/* Application des 4 modifications supplémentaires */
			for(i = 0; i <= 3; i++){
				double save_x = xn0;
				double save_y = yn0; 
				xn0 = save_x*save_x - save_y*save_y + x;
				yn0 = 2*save_x*save_y + y;
			}

			set->img[indice].iter = iteration;
			set->img[indice].x = xn0;
			set->img[indice].y = yn0;
		}
		printf("%d\r", Py);
	}
}



void get_pixel_color(struct render *set, int Px, int Py, int* r, int* g, int* b){

	int indice = m2v(set, Px, Py);
	int iteration = set->img[indice].iter;

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

	else if (set->type == GREY_SM) { 
		double grey;
		double x;
		double y;
		int n_it;

		x = set->img[indice].x;
		y = set->img[indice].y;
		n_it = set->img[indice].iter;

		grey = 5 + n_it - log(log(x*x + y*y)/log(2))/log(2);
		grey = floor(512 * grey/set->iter_max);

		if (grey > 255){
			grey = 255;
		}

		*r = grey;
		*g = grey;
		*b = grey;
	}

	else if (set->type == RGB){
		struct color hsv;
		struct color rgb;
		double grey;
		double x;
		double y;
		int n_it;

		x = set->img[indice].x;
		y = set->img[indice].y;
		n_it = set->img[indice].iter;

		grey = 5 + n_it - log(log(x*x + y*y)/log(2))/log(2);

		hsv.c1 = 360 * grey / set->iter_max;
		hsv.c2 = 1.0;
		hsv.c3 = 1.0;

		hsv2rgb(&rgb, &hsv);

		*r = rgb.c1;
		*g = rgb.c2;
		*b = rgb.c3;
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


/* 

	Estimation de ma note : 20/20 

*/










