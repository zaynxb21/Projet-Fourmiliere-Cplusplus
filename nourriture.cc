// fichier : nourriture.cc
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB
// contibution au fichier 
// Yasmine : 100% de la mise au point des fonctions / methodes de ce fichier
// Zaynab : 00% de la mise au point des fonctions / methodes de ce fichier

#include "nourriture.h"
#include "message.h"
#include "error_squarecell.h"

#include <string>
#include <iostream>
#include <sstream>

Nourriture::Nourriture(std::string line){
	std::istringstream data(line);
	unsigned int x1;
	unsigned int y1;
	if(!(data>> x1 >> y1)){
		exit(EXIT_FAILURE);
	}else{
		r.x=x1;
		r.y=y1;
		r.side=1;
	}
}
		
Square Nourriture::get_r()const{
	return r; 
} 

bool food_dans_grille(std::vector<Nourriture> food_){
	for(unsigned int i(0); i<food_.size(); ++i){
		if(carre_type_centre_dans_grille(food_[i].get_r())){
			std::cout<<error_squarecell::print_outside
					   (food_[i].get_r().x, food_[i].get_r().side, 
					    g_max-1);	
			return false;							
		}
	}
	return true;
}

bool superposition_nourriture_grille(std::vector<Nourriture> food_,
									 Grille& grille){
	int x_cellule=0;
	int y_cellule=0;
	for(unsigned int y(0); y<food_.size(); ++y){
		if(grille.sup_grille(food_[y].get_r(),grille, 
							 x_cellule, y_cellule)){
			std::cout<<message::food_overlap(food_[y].get_r().x, 
											 food_[y].get_r().y);
			return false;
			}
		grille.occupation(food_[y].get_r());
		}
		return true;
}

bool coo_dans_grille(std::vector<Nourriture> food_){
	for(unsigned int i(0); i<food_.size(); ++i){
		if(coordonee_dans_grille(food_[i].get_r())){
			return true;
		}
	}
	return false;
}

void dessinernourriture(std::vector<Nourriture> food_){
	for(unsigned int i(0); i<food_.size(); ++i){
		losangesquare(food_[i].get_r(), WHITE);
	}
}
