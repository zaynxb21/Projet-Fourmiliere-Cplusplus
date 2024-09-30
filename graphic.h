// fichier : graphic.h
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB

#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include "color.h"

#include <gtkmm.h>

void SetContext(const Cairo::RefPtr<Cairo::Context>& cr);
void drawSegment(double x1, double y1, double x2, double y2, 
				 double epaisseur);
void remplirforme();
void losange(int x, int y, int side, Color couleur);
void carrecomplet(int x, int y, int side, int a, char nuance);
double convertir_x(double x);
double convertir_y(double y);
void carrevide(double x, double y, double side, int a, char nuance);
void tracerlignesgrille(unsigned int a, unsigned int b, Color couleur);
void tracer_carre_damier(int x, int y, int side, int a, char nuance1, 
				         char nuance2);
void tracer_carre_croix(int x, int y, int side, int a, char nuance);
void setColorGrille(Color couleur);
void setColor(int a, char nuance);
void tracerbords();
const short int gmax(128);
#endif
