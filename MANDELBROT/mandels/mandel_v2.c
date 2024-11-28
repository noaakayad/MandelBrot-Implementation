#include <stdio.h>
#include <math.h>
#include <string.h>

#define STRMAX 256
#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 1000

struct render{
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	int longueur;
	int largeur;
	int iter_max;
	double radius;
	int img[DEFAULT_WIDTH][DEFAULT_HEIGHT];
	char basename[STRMAX];
};

double map (int v, int imin, int imax, double omin, double omax){
	double taille_i = imax - imin;
	double taille_vi = (double)v - imin;
	double taille_o = omax - omin;
	double pourcentage_vi = taille_vi / taille_i;

	return pourcentage_vi * taille_o + omin;
}

int main(void){
	int Py;
	int Px;
	FILE* fout;

	char intermediaire[STRMAX];
	struct render set;
	set.xmin = -2;
	set.xmax = 0.48;
	set.ymin = -1.24;
	set.ymax = 1.24;
	set.longueur = DEFAULT_HEIGHT;
	set.largeur = DEFAULT_WIDTH;
	set.iter_max = 100;
	set.radius = 2;

	strncpy(set.basename, "mandel", STRMAX);


	for (Py = 0; Py < set.longueur; Py++){
		for (Px = 0; Px < set.largeur; Px++){
			double x = map(Px, 0, set.largeur, set.xmin, set.xmax);
			double y = map(Py, 0, set.longueur, set.ymin, set.ymax);
			int iteration = 0;

			double xn0 = 0;
			double yn0 = 0;

			while(sqrt(xn0*xn0 + yn0*yn0) <= set.radius && iteration < set.iter_max){
				double save_x = xn0;
				double save_y = yn0; 
				xn0 = save_x*save_x - save_y*save_y + x;
				yn0 = 2*save_x*save_y + y;
				iteration ++;
			}

			set.img[Px][Py] = iteration;
		}
		printf("\n");
	}

	strncpy(intermediaire, set.basename, STRMAX);
	strcat(intermediaire, ".ppm");

	fout = fopen(intermediaire, "w");
	if (fout == NULL){
		printf("Impossible d'ouvrir le fichier\n");
		return 1;
	}

	fprintf(fout, "P3\n");
	fprintf(fout, "%d %d\n", set.largeur, set.longueur);
	fprintf(fout, "255\n");
	fprintf(fout, "#Ici le nb max d'itération est max_iter autrement dit 100.#\n");
	fprintf(fout, "#La zone dans le plan complexe explorée est l'ensemble des z = x + iy tel que 𝑥∈[−2;0,48] et 𝑦∈[−1,24;1,24].#\n");

	for (Py = 0; Py < set.longueur; Py++){
		for (Px = 0; Px < set.largeur; Px++){
			double x = map(Px, 0, set.largeur, set.xmin, set.xmax);
			double y = map(Py, 0, set.longueur, set.ymin, set.ymax);
			int iteration = 0;

			double xn0 = 0;
			double yn0 = 0;

			while(sqrt(xn0*xn0 + yn0*yn0) <= set.radius && iteration < set.iter_max){
				double save_x = xn0;
				double save_y = yn0; 
				xn0 = save_x*save_x - save_y*save_y + x;
				yn0 = 2*save_x*save_y + y;
				iteration ++;
			}

			if (sqrt(xn0*xn0 + yn0*yn0) > set.radius){
				fprintf(fout, "255 255 255\n");
			}
			else {
				fprintf(fout, "0 0 0\n");
			}
		}
		printf("\n");
	}

	fclose(fout);

	return 0;
}











