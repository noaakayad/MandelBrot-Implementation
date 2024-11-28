#include <stdio.h>
#include <math.h>

double map (int v, int imin, int imax, double omin, double omax){
	double taille_i = imax - imin;
	double taille_vi = (double)v - imin;
	double taille_o = omax - omin;
	double pourcentage_vi = taille_vi / taille_i;

	return pourcentage_vi * taille_o + omin;
}

int main(void){
	int W = 160;
	int H = 50;
	double xmin = -2;
	double xmax = 0.48;
	double ymin = -1.24;
	double ymax = 1.24;
	int max_iter = 100;
	int radius = 2;

	int Py;
	int Px;

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

			if (sqrt(xn0*xn0 + yn0*yn0) > radius){
				printf(" ");
			}
			else {
				printf("*");
			}
		}
		printf("\n");
	}

	return 0;
}











