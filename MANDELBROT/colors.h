#ifndef COLORS_H
#define COLORS_H

/* Une structure pour représenter une couleur, quel que soit
 * le modèle choisi (tant qu'il utilise 3 composantes).
 */
struct color {
	double c1, c2, c3 ;	/* Composantes de la couleur rgb, hsv, hsl, ... */
} ;


/* Convertit une couleur exprimée en HSV en une couleur
 * exprimée en RGB.
 *
 * Voir https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
 */
void hsv2rgb(struct color *rgb, struct color *hsv) ;

#endif
