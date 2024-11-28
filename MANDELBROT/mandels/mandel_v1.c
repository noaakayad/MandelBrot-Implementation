#include <stdio.h>
#include <math.h>

#define W 160
#define H 50
#define DEFAULT_WIDTH 1000
#define DEFAULT_HEIGHT 1000

double map (int v, int imin, int imax, double omin, double omax){
	double taille_i = imax - imin;
	double taille_vi = (double)v - imin;
	double taille_o = omax - omin;
	double pourcentage_vi = taille_vi / taille_i;

	return pourcentage_vi * taille_o + omin;
}

int main(void){
	double xmin = -2;
	double xmax = 0.48;
	double ymin = -1.24;
	double ymax = 1.24;
	int max_iter = 100;
	int radius = 2;
	int Py;
	int Px;
	FILE* fout;

	int* img[DEFAULT_WIDTH];
	int i;
	for (i = 0; i < DEFAULT_WIDTH; i++){
		int tab_i[DEFAULT_HEIGHT];
		img[i] = tab_i;
	}


	for (Py = 0; Py < H; Py++){
		for (Px = 0; Px < W; Px++){
			double x = map(Px, 0, W, xmin, xmax);
			double y = map(Py, 0, H, ymin, ymax);
			int iteration = 0;

			double xn0 = 0;
			double yn0 = 0;

			while(sqrt(xn0*xn0 + yn0*yn0) <= radius && iteration < max_iter){
				double save_x = xn0;
				double save_y = yn0; 
				xn0 = save_x*save_x - save_y*save_y + x;
				yn0 = 2*save_x*save_y + y;
				iteration ++;
			}

			img[Px][Py] = iteration;
		}
		printf("\n");
	}

	fout = fopen("mandel.ppm", "w");
	if (fout == NULL){
		printf("Impossible d'ouvrir le fichier\n");
		return 1;
	}

	fprintf(fout, "P3\n");
	fprintf(fout, "%d %d\n", DEFAULT_WIDTH, DEFAULT_HEIGHT);
	fprintf(fout, "255\n");
	fprintf(fout, "#Ici le nb max d'itération est max_iter autrement dit 100.#\n");
	fprintf(fout, "#La zone dans le plan complexe explorée est l'ensemble des z = x + iy tel que 𝑥∈[−2;0,48] et 𝑦∈[−1,24;1,24].#\n");

	for (Py = 0; Py < DEFAULT_HEIGHT; Py++){
		for (Px = 0; Px < DEFAULT_WIDTH; Px++){
			double x = map(Px, 0, DEFAULT_WIDTH, xmin, xmax);
			double y = map(Py, 0, DEFAULT_HEIGHT, ymin, ymax);
			int iteration = 0;

			double xn0 = 0;
			double yn0 = 0;

			while(sqrt(xn0*xn0 + yn0*yn0) <= radius && iteration < max_iter){
				double save_x = xn0;
				double save_y = yn0; 
				xn0 = save_x*save_x - save_y*save_y + x;
				yn0 = 2*save_x*save_y + y;
				iteration ++;
			}

			if (sqrt(xn0*xn0 + yn0*yn0) > radius){
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











