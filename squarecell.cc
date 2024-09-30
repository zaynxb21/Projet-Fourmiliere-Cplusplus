// fichier :squarecell.cc
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB
// contibution au fichier 
// Yasmine : 00% de la mise au point des fonctions / methodes de ce fichier
// Zaynab : 100% de la mise au point des fonctions / methodes de ce fichier

#include "squarecell.h"
#include "error_squarecell.h"
#include "graphic.h"

#include <iostream>

using namespace std;



int set_indice_j(Square b){
		return b.x;
}

int set_indice_i(Square b){
		return g_max-1-b.y;
}

bool carre_compris_dans_autre(Square num1, Square num2){
	if(!(num1.x-((num1.side-1)/2)>num2.x 
		 and num1.x+((num1.side-1)/2)<=	(num2.x+num2.side-2) 
		 and num1.y-((num1.side-1)/2)>num2.y 
		 and num1.y+((num1.side-1)/2)<=(num2.side-2+num2.y))){
		return true;
	}else{
		return false;
	}
}

bool sup_carree(Square F1,Square F2){
	if(F1.x<F2.x+F2.side and F1.x+F1.side>F2.x 
	   and F1.y<F2.y+F2.side and F1.y+F1.side> F2.y){
		return true;
	}else{
		return false;
	} 
}

bool coordonee_dans_grille(Square c){
	if(!(c.x>=0 and c.x<=g_max-1 and c.y>=0 and c.y<=g_max-1)){
		cout<<error_squarecell::print_index(c.x, g_max-1);
		return false;
	}
	return true;
}




bool carre_type_centre_dans_grille(Square c){
     if(!(c.x+((c.side-1)/2)<=g_max-1 and c.x-((c.side-1)/2)>=0 
		  and c.y+((c.side-1)/2)<=g_max-1 and c.y-((c.side-1)/2)>=0)){
		return true;
	}else{
		return false;
	}
}

bool carre_type_non_centre_dans_grille(Square dedans){
	if(!(dedans.x>=0 and dedans.y>=0 
		 and dedans.x+dedans.side<=g_max-1 
		 and dedans.y+dedans.side<=g_max-1)){
		return true;
	}else{
		return false;
	}
}

int Grille::get_size(){
	return grid.size();
}
		
void Grille::set_cell(int a, int b, bool valeur){
	grid[a][b]=valeur;
}
		
void Grille::initialization(bool in){
	for (int n=0; n<g_max; n++){
		for (int m(0); m<g_max; m++){
			grid[n][m]=in;
		}
	}
}
	
bool Grille::get_case(int lgn, int cln){
	return grid[lgn][cln];
}
		
void Grille::set_cell4square(int a_c, int b_c, bool valeur_c){
	set_cell(a_c, b_c, valeur_c);
}
		
void Grille::occupation (Square c){ 
	int i= set_indice_i(c);
	int j= set_indice_j(c);
	for (int t=0; t<c.side; t++){
		for (int r=0; r<c.side; r++){
			set_cell4square(i-(c.side-1)/2+r,j-(c.side-1)/2+t,true);
		}
	}
}

void Grille::liberation (Square c){
	int i= set_indice_i(c);
	int j= set_indice_j(c);
	for (int t=0; t<c.side; t++){
		for (int r=0; r<c.side; r++){
			set_cell4square(i-(c.side-1)/2+r,j-(c.side-1)/2+t,false);
		} 
	}
}

int Grille::get_value(int value){
	return value;
}
		
bool Grille::sup_grille (Square c, Grille tab, int& x_cellule, 
						 int& y_cellule){
	int i= set_indice_i(c);
	int j= set_indice_j(c);
	for (int r=0; r<c.side; r++){
		for (int t=0; t<c.side; t++){
			if ((tab.get_case((i-(c.side-1)/2)+t,
				 (j-(c.side-1)/2)+r))==true){
				x_cellule=(j-(c.side-1)/2)+r;
				y_cellule=g_max-1-(i-(c.side-1)/2)-t;
				return true;
			}
		}
	}
	return false;
}

bool Grille::sup_grille1 (Square c, Grille tab){
	int i= set_indice_i(c);
	int j= set_indice_j(c);
	for (int r=0; r<c.side; r++){
		for (int t=0; t<c.side; t++){
			if ((tab.get_case((i-(c.side-1)/2)+t,
				 (j-(c.side-1)/2)+r))==true){
				return true;
			}
		}
	}
	return false;
}

void carrecompletsquare(Square c, int a, char nuance){
	carrecomplet(c.x, c.y, c.side, a, nuance);
}

void losangesquare(Square c, Color couleur){
	losange(c.x, c.y, c.side, couleur);
}


void carre_vide(Square c, int a, char nuance){
	carrevide(c.x, c.y, c.side, a, nuance);
}

void carre_croix(Square c, int a, char nuance1, char nuance2){
	carrecompletsquare(c, a, nuance2);
	tracer_carre_croix(c.x, c.y, c.side, a, nuance1);	
}

void carre_damier(Square c, int a, char nuance1, char nuance2){
	carrecompletsquare(c, a, nuance1);
	tracer_carre_damier(c.x, c.y, c.side, a, nuance1, nuance2);
}

Grille::Grille(){
	for (int n=0; n<g_max; n++){
		for (int m(0); m<g_max; m++){
			grid[n][m]=false;
		}
	}
}

void Grille::nettoyergrille(){
	for (int n=0; n<g_max; n++){
		for (int m(0); m<g_max; m++){
			grid[n][m]=false;
		}
	}
}

bool carre_centre_dans_non_centre(Square F1, Square C){
	if(F1.x<(C.x-C.side/2)+C.side and F1.x+F1.side>(C.x-C.side/2)
	   and F1.y<(C.y-C.side/2)+C.side and F1.y+F1.side> (C.y-C.side/2)){
		return true;
	}else{
		return false;
	} 
}
