// fichier : nourriture.h
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB

#ifndef NOURRITURE_H_INCLUDED
#define NOURRITURE_H_INCLUDED

#include "squarecell.h"

#include <string>
#include <iostream>
#include <sstream>

class Nourriture{
	
private :
	
	Square r;
	
public :
	Nourriture(int x1, int y1){r.x=x1, r.y=y1, r.side=1;}
	Nourriture(std::string line);
	Square get_r() const;
			
};

bool food_dans_grille(std::vector<Nourriture> food_);
bool superposition_nourriture_grille(std::vector<Nourriture> food_, 
									 Grille& grille);
bool coo_dans_grille(std::vector<Nourriture> food_); 
void dessinernourriture(std::vector<Nourriture> food_);
	
#endif
