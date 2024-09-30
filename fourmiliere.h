// fichier : fourmilier.h
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB

#ifndef FOURMILIERE_H_INCLUDED
#define FOURMILIERE_H_INCLUDED



#include "constantes.h"
#include "squarecell.h"

#include <string>
#include <iostream>
#include <sstream>
 
 
class Fourmiliere {

private :
	
	Square F;
	unsigned int nbr_collector;
	unsigned int nbr_defensor;
	unsigned int nbr_predator;
	double nbr_food;
	Square g;
	unsigned int numero_fourmiliere;
	Etat_fourmiliere state = CONSTRAINED;
		
public :
		
	Fourmiliere(std::string line, int nombre_fourmiliere_lu);	
	Square get_g() const;
	int get_nbr_collector() const;
	int get_nbr_predator() const;
	int get_nbr_defensor() const;
	double get_nbr_food() const;
	Square get_F() const;
	int get_anthillid() const;
	void set_nbr_food(double nbT);
	void set_nbc();
	void set_nbd();
	void set_nbp();
	void setnbc();
	void setnbd();
	void setnbp();
	void set_side(int size);
	void set_x(int x);
	void set_y(int y);
	void set_state(Etat_fourmiliere state);
	Etat_fourmiliere get_state();
	void set_gx(std::string signe);
	void set_gy(std::string signe);
		
};


bool generator_dans_fourmiliere(std::vector<Fourmiliere> mais_ant); 
bool carrefourmil_dans_grille(Square dedans);

bool superposition_fourmilieres(std::vector<Fourmiliere> mais_ant,
								Grille& grille);

bool fourmiliere_dans_grille(std::vector<Fourmiliere> mais_ant);

bool coo_anthill_dans_grille(std::vector<Fourmiliere> maison_fourmis);							 

void dessinerfourmilieres(std::vector<Fourmiliere> mais_ant);
	
#endif
