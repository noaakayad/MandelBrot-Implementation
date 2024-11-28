#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define STRMAX 256
#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 1000

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
};

double map (int v, int imin, int imax, double omin, double omax){
	double taille_i = imax - imin;
	double taille_vi = (double)v - imin;
	double taille_o = omax - omin;
	double pourcentage_vi = taille_vi / taille_i;

	return pourcentage_vi * taille_o + omin;
}

void render_init(struct render *set, int argc, char *argv[]){
	set->xmin = -2.0;
	set->xmax = 0.48;
	set->ymin = -1.24;
	set->ymax = 1.24;
	set->H = DEFAULT_HEIGHT;
	set->W = DEFAULT_WIDTH;
	set->iter_max = 100;
	set->radius = 2.0;

	if (argc > 1){
		if (sscanf(argv[1],"%d", &set->iter_max) != 1){
			printf("Erreur, format incorrect pour le nb d'iterations.");
			exit(1);
		}
	}

	if (argc > 2){
		if (sscanf(argv[2], "%lf,%lf,%lf,%lf", &set->pov.x, &set->pov.y, &set->pov.width, &set->pov.height) != 4){
			printf("Erreur format incorrect pour les paramètres de la caméra.");
			exit(1);
		}
	}

	if (argc > 3){
		if (sscanf(argv[3], "%dx%d", &set->W, &set->H) != 2){
			printf("Erreur format incorrect pour la taille de l'image");
			exit(1);
		}
	}

	if (argc > 4){
		int i = 0;
		int j = 0;
		int taille = strlen(argv[4]); 
		char* intermediaire = malloc (taille * sizeof(char));

		while (i < taille && argv[4][i] == ' '){
			i++;
		}

		for (; i < taille; i++){
			if (argv[4][i] != ' '){
				intermediaire[j] = argv[4][i];
				j++;
			}
			else if (j == 0 || intermediaire[j-1] != '_'){
				intermediaire[j] = '_';
				j++;	
			}
		}

	    if (j > 0 && intermediaire[j - 1] == '_') {
	        intermediaire[j - 1] = '\0';
	    }		

    	strncpy(set->basename, intermediaire, STRMAX - 5);

    	free(intermediaire);
	}

	set->img = malloc((set->W * set->H) * sizeof(int));
    if (set->img == NULL){
        printf("Erreur d'allocation mémoire !\n");
        exit(1);
    }
}

int m2v (struct render *set, int x, int y){
	return y*set->W + x;
}

void render_image(struct render* set){
	int Py;
	int Px;

	for (Py = 0; Py < set->H; Py++){
		for (Px = 0; Px < set->W; Px++){
			double x = map(Px, 0, set->W, set->xmin, set->xmax);
			double y = map(Py, 0, set->H, set->ymin, set->ymax);
			int iteration = 0;

			double xn0 = 0;
			double yn0 = 0;

			int indice = m2v(set, Px, Py);

			while(sqrt(xn0*xn0 + yn0*yn0) <= set->radius && iteration < set->iter_max){
				double save_x = xn0;
				double save_y = yn0; 
				xn0 = save_x*save_x - save_y*save_y + x;
				yn0 = 2*save_x*save_y + y;
				iteration ++;
			}

			set->img[indice] = iteration;
		}
		printf("%d", Px);
		printf("\r");
	}
}

void get_pixel_color(struct render *set, int Px, int Py, int* r, int* g, int* b){
	double x = map(Px, 0, set->W, set->xmin, set->xmax);
	double y = map(Py, 0, set->H, set->ymin, set->ymax);
	int iteration = 0;

	double xn0 = 0;
	double yn0 = 0;

	while(sqrt(xn0*xn0 + yn0*yn0) <= set->radius && iteration < set->iter_max){
		double save_x = xn0;
		double save_y = yn0; 
		xn0 = save_x*save_x - save_y*save_y + x;
		yn0 = 2*save_x*save_y + y;
		iteration ++;
	}

	if (sqrt(xn0*xn0 + yn0*yn0) > set->radius){
		*r = 255;
		*g = 255;
		*b = 255;
	}
	else {
		*r = 0;
		*g = 0;
		*b = 0;
	}
}

void save_image(struct render *set){
	FILE* fout;
	int Py;
	int Px;

	strcat(set->basename, ".ppm");

	fout = fopen(set->basename, "w");
	if (fout == NULL){
		printf("Impossible d'ouvrir le fichier\n");
		return ;
	}

	fprintf(fout, "P3\n");
	fprintf(fout, "%d %d\n", set->W, set->H);
	fprintf(fout, "255\n");
	fprintf(fout, "#Ici le nb max d'itération est max_iter autrement dit 100.#\n");
	fprintf(fout, "#La zone dans le plan complexe explorée est l'ensemble des z = x + iy tel que 𝑥∈[−2;0,48] et 𝑦∈[−1,24;1,24].#\n");


	for (Py = 0; Py < set->H; Py++){
		for (Px = 0; Px < set->W; Px++){
			double x = map(Px, 0, set->W, set->xmin, set->xmax);
			double y = map(Py, 0, set->H, set->ymin, set->ymax);
			int iteration = 0;

			double xn0 = 0;
			double yn0 = 0;

			while(sqrt(xn0*xn0 + yn0*yn0) <= set->radius && iteration < set->iter_max){
				double save_x = xn0;
				double save_y = yn0; 
				xn0 = save_x*save_x - save_y*save_y + x;
				yn0 = 2*save_x*save_y + y;
				iteration ++;
			}

			if (sqrt(xn0*xn0 + yn0*yn0) > set->radius){
				int r;
				int g;
				int b;

				fprintf(fout, "255 255 255\n");

				get_pixel_color(set, Px, Py, &r, &g, &b);
			}
			else {
				int r;
				int g;
				int b;

				fprintf(fout, "0 0 0\n");

				get_pixel_color(set, Px, Py, &r, &g, &b);
			}
		}
		printf("\n");
	}

	fclose(fout);
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











