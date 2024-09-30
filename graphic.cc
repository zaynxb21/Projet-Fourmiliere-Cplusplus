// fichier : graphic.cc
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB
// contibution au fichier 
// Yasmine : 30% de la mise au point des fonctions / methodes de ce fichier
// Zaynab : 70% de la mise au point des fonctions / methodes de ce fichier

#include "graphic.h"

#include <string>

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

double convertir_x(double x){
	return x+0.5;
}

double  convertir_y(double y){
	return y+0.5;
}

void SetContext(const Cairo::RefPtr<Cairo::Context>& cr){
    static bool situation(false);
    if(!situation) {
		ptcr = &cr;
        situation = true;
    }
}

void drawSegment(double x1, double y1, double x2, double y2, 
				 double epaisseur){
	(*ptcr)->set_line_width(epaisseur);
    (*ptcr)->save();
    (*ptcr)->move_to(x1, y1);
    (*ptcr)->line_to(x2, y2);
    (*ptcr)->restore();
    (*ptcr)->stroke();
}

void remplirforme(){
	(*ptcr)->fill_preserve();
	(*ptcr)->stroke();
}

void carrecomplet(int x1, int y1, int side, int a, char nuance){
	double  x=convertir_x(x1);
	double  y=convertir_y(y1);
	(*ptcr)->set_line_width(0.01);
	setColor(a, nuance);
	(*ptcr)->rectangle(x-(side-1)/2-0.5,y-(side-1)/2-0.5,side,side);
	remplirforme();	
}

void losange(int x1, int y1, int side, Color couleur){
	double  x=convertir_x(x1);
	double  y=convertir_y(y1); 
	(*ptcr)->set_line_width(0.01);
	setColorGrille(couleur);
	(*ptcr)->move_to(x-(side-1)/2-0.5, y);
	(*ptcr)->line_to(x, y-(side-1)/2-0.5);
	(*ptcr)->line_to(x+(side-1)/2+0.5, y);
	(*ptcr)->line_to(x, y+(side-1)/2+0.5);
	(*ptcr)->line_to(x-(side-1)/2-0.5, y);
	(*ptcr)->line_to(x, y-(side-1)/2-0.5);
	remplirforme();
}

void carrevide(double x1, double y1, double side, int a, char nuance){
	double x=convertir_x(x1);
	double y=convertir_y(y1);
	setColor(a, nuance);
	drawSegment(x,y,x+side-1, y,0.2);
	drawSegment(x+side-1, y,x+side-1, y+side-1,0.2);
	drawSegment(x+side-1, y+side-1,x,y+side-1, 0.2);
	drawSegment(x, y+side-1,x,y, 0.2);
	drawSegment(x,y,x+side-1, y,0.2);
	drawSegment(x+side-1, y,x+side-1, y+side-1,0.2);
}

void tracerlignesgrille(unsigned int a, unsigned int b, Color couleur){
	for (unsigned int i(0); i<=gmax; i+=1){
		setColorGrille(couleur);
		drawSegment(i, 0, i, a, 0.05);
		(*ptcr)->save();
		(*ptcr)->restore();
		(*ptcr)->stroke();
	}
	for (unsigned int j(0); j<=gmax; j+=1){
		setColorGrille(couleur);
		drawSegment(0, j, b, j, 0.05);
		(*ptcr)->save();
		(*ptcr)->restore();
		(*ptcr)->stroke();
	}
}

void tracerbords(){
	(*ptcr)->set_line_width(0.01);
	setColorGrille(WHITE);
	for (int h(0) ; h<gmax ; h++){
		(*ptcr)->rectangle(0,h,1,1);
		remplirforme();
	}
	for (int k(0) ; k<gmax ; k++){
		(*ptcr)->rectangle(k,0,1,1);
		remplirforme();
	}
	for (int m(0) ; m<gmax ; m++){
		(*ptcr)->rectangle(m,gmax-1,1,1);
		remplirforme();
	}
	for (int n(0) ; n<gmax ; n++){
		(*ptcr)->rectangle(gmax-1,n,1,1);
		remplirforme();
	}
}

int taille_dessin1(500);

void tracer_carre_croix(int x1, int y1, int side, int a, char nuance){
	double  x=convertir_x(x1);
	double  y=convertir_y(y1);
	setColor(a, nuance);
	drawSegment(x, y+(side-1)/2+0.5, x, y-(side-1)/2-0.5, 
			    taille_dessin1/(gmax*2));
	drawSegment(x+(side-1)/2+0.5, y, x-(side-1)/2-0.5, y, 
			    taille_dessin1/(gmax*2));
}
	
void tracer_carre_damier(int x1, int y1, int side, int a, char nuance1, 
						 char nuance2){
	tracer_carre_croix(x1, y1, side, a, nuance2);
	double  x=convertir_x(x1);
	double  y=convertir_y(y1);
	setColor(a, nuance1);
	drawSegment(x-0.5 , y, x+0.5, y, taille_dessin1  / (gmax*2));
}
	

void setColorGrille(Color couleur){
	switch(couleur){
		case WHITE:
			(*ptcr)->set_source_rgb(WHITE_RGB);
			break;
		case GREY:
			(*ptcr)->set_source_rgb(GREY_RGB);
			break;
		case BLACK:
			(*ptcr)->set_source_rgb(BLACK_RGB);
			break;
	}
}

void setColor(int a, char nuance){
	if (a%6==0 and nuance=='C')
		(*ptcr)->set_source_rgb(RED_C_RGB);
	if (a%6==0 and nuance=='F')
		(*ptcr)->set_source_rgb(RED_F_RGB);
	if (a%6==1 and nuance=='C')
		(*ptcr)->set_source_rgb(GREEN_C_RGB);
	if (a%6==1 and nuance=='F')
		(*ptcr)->set_source_rgb(GREEN_F_RGB);
	if (a%6==2 and nuance=='C')
		(*ptcr)->set_source_rgb(BLUE_C_RGB);
	if (a%6==2 and nuance=='F')
		(*ptcr)->set_source_rgb(BLUE_F_RGB);
	if (a%6==3 and nuance=='C')
		(*ptcr)->set_source_rgb(YELLOW_C_RGB);
	if (a%6==3 and nuance=='F')
		(*ptcr)->set_source_rgb(YELLOW_F_RGB);
	if (a%6==4 and nuance=='C')
		(*ptcr)->set_source_rgb(MAGENTA_C_RGB);
	if (a%6==4 and nuance=='F')
		(*ptcr)->set_source_rgb(MAGENTA_F_RGB);
	if (a%6==5 and nuance=='C')
		(*ptcr)->set_source_rgb(CYAN_C_RGB);
	if (a%6==5 and nuance=='F')
		(*ptcr)->set_source_rgb(CYAN_F_RGB);
}
