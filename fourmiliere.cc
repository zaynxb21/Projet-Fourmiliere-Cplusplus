// fichier : fourmiliere.cc
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB
// contibution au fichier 
// Yasmine : 100% de la mise au point des fonctions / methodes de ce fichier
// Zaynab : 00% de la mise au point des fonctions / methodes de ce fichier

#include "fourmiliere.h"
#include "message.h"
#include "error_squarecell.h"

using namespace std; 

Fourmiliere::Fourmiliere(std::string line, int nombre_fourmiliere_lu){
	std::istringstream data(line);
	unsigned int x1, y1, side1, xg1, yg1, nbC, nbD, nbP;
	double total_food;
	if (!(data >> x1 >> y1 >> side1 >> xg1 >> yg1 >> total_food >> 
		nbC >> nbD >> nbP)){
	}else{
		F.x=x1;
		F.y=y1;
		F.side=side1;
		g.x=xg1;
		g.y=yg1;
		g.side=sizeG;
		nbr_food=total_food;
		nbr_collector=nbC;
		nbr_defensor=nbD;
		nbr_predator=nbP;
		numero_fourmiliere=nombre_fourmiliere_lu;
	}
}

Square Fourmiliere::get_g() const{
	return g;
}

int Fourmiliere::get_nbr_collector() const{
	return nbr_collector;
}

int Fourmiliere::get_nbr_predator() const{
	return nbr_predator;
}

int Fourmiliere::get_nbr_defensor() const{
	return nbr_defensor;
}

double Fourmiliere::get_nbr_food() const{
	return nbr_food;
}

void Fourmiliere::set_nbr_food(double nbT){
	nbr_food-=nbT;
}

void Fourmiliere::set_nbc(){
	--nbr_collector;
}

void Fourmiliere::set_nbd(){
	--nbr_defensor;
}

void Fourmiliere::set_nbp(){
	--nbr_predator;
}

void Fourmiliere::set_side(int size){
	F.side=size;
}

void Fourmiliere::setnbc(){
	++nbr_collector;
}

void Fourmiliere::setnbd(){
	++nbr_defensor;
}

void Fourmiliere::setnbp(){
	++nbr_predator;
}


void Fourmiliere::set_x(int x){
	F.x=x;
}
void Fourmiliere::set_y(int y){
	F.y=y;
}

Square Fourmiliere::get_F() const{
	return F;
}


int Fourmiliere::get_anthillid() const{
	return numero_fourmiliere;
}

void Fourmiliere::set_state(Etat_fourmiliere etat){
	state = etat;
}

Etat_fourmiliere Fourmiliere::get_state(){
	return state;
}


bool generator_dans_fourmiliere(std::vector<Fourmiliere> mais_ant){
	for (unsigned int i(0); i<mais_ant.size(); ++i){
		if(carre_compris_dans_autre(mais_ant[i].get_g(),
									mais_ant[i].get_F())){				
			std::cout<<message::generator_not_within_home
					   (mais_ant[i].get_g().x, mais_ant[i].get_g().y, 
						mais_ant[i].get_anthillid());
			return false; 
		}
	}
	return true;
}


bool superposition_fourmilieres(std::vector<Fourmiliere> mais_ant,
								Grille& grille){
	for(unsigned int i(0); i<mais_ant.size(); ++i){
		for( unsigned int y(i+1); y<mais_ant.size(); ++y){
			if(sup_carree(mais_ant[i].get_F(),mais_ant[y].get_F())){
				cout<<message::homes_overlap
					  (mais_ant[i].get_anthillid(),
					   mais_ant[y].get_anthillid());		
				return false; 
			}
		}
	}
	int x_cellule=0, y_cellule=0;
	for(unsigned int y(0); y<mais_ant.size(); ++y){
		if(grille.sup_grille(mais_ant[y].get_g(), grille, x_cellule, 
							 y_cellule)){				   
			cout<<message::generator_overlap(mais_ant[y].get_g().x,
											 mais_ant[y].get_g().y,
											 x_cellule, y_cellule);
			return false; 
			}
	grille.occupation(mais_ant[y].get_g());
	} 
	return true;
}


bool fourmiliere_dans_grille(std::vector<Fourmiliere> mais_ant){
	for (unsigned int i(0); i<mais_ant.size(); ++i){
		if(carre_type_non_centre_dans_grille(mais_ant[i].get_F())){
			cout<<error_squarecell::print_outside
				  (mais_ant[i].get_F().x, mais_ant[i].get_F().side, 
				   g_max-1);
			return false;
		}
	}
	for (unsigned int i(0); i<mais_ant.size(); ++i){
		if(carre_type_centre_dans_grille(mais_ant[i].get_g())){
			cout<<error_squarecell::print_outside
				  (mais_ant[i].get_g().x, mais_ant[i].get_g().side, 
				   g_max-1);
			return false;
		}
	}
	return true;
}	



bool coo_anthill_dans_grille(std::vector<Fourmiliere> maison_fourmis){
	for(unsigned int i(0); i<maison_fourmis.size(); ++i){
		if(!coordonee_dans_grille(maison_fourmis[i].get_F())){
			return false;
		}
	}
	for (unsigned int i(0); i<maison_fourmis.size(); ++i){
		if(!coordonee_dans_grille(maison_fourmis[i].get_g())){
			return false;
		}
	}
	return true;
}



void dessinerfourmilieres(std::vector<Fourmiliere> mais_ant){
	for(unsigned int i(0); i<mais_ant.size(); ++i){
		carre_vide(mais_ant[i].get_F(), 
				   mais_ant[i].get_anthillid(), 'F');
	}
}

bool carrefourmil_dans_grille(Square dedans){
	if(!(dedans.x>=0 and dedans.y>=0 
		 and dedans.x+dedans.side<=g_max 
		 and dedans.y+dedans.side<=g_max)){
		return true;
	}else{
		return false;
	}
}

void Fourmiliere::set_gx(string signe){
	if(signe=="-"){
		--g.x;
	}
	if(signe=="+"){
		++g.x;
	}
}

void Fourmiliere::set_gy(string signe){
	if(signe=="-"){
		--g.y;
	}
	if(signe=="+"){
		++g.y;
	}
}

