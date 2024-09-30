// fichier : fourmis.cc
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB
// contibution au fichier 
// Yasmine : 100% de la mise au point des fonctions / methodes de ce fichier
// Zaynab : 00% de la mise au point des fonctions / methodes de ce fichier

#include "fourmis.h"
#include "message.h"
#include "error_squarecell.h"

using namespace std;

bool defensor_dans_fourmiliere(std::vector<Defensor>coo_defensor, 
							   std::vector<Fourmiliere> mais_ant){
	for (unsigned int i(0); i<coo_defensor.size();++i){
		if(carre_compris_dans_autre
		   (coo_defensor[i].get_carre(),
			mais_ant[coo_defensor[i].get_hillid2()].get_F())){		
			cout<<message::defensor_not_within_home
				  (coo_defensor[i].get_carre().x, 
				   coo_defensor[i].get_carre().y, 
				   coo_defensor[i].get_hillid2());
			return false;
		}
	}
	return true;
}

bool fourmis_dans_grille(std::vector<Collector> coo_collectors, 
						 std::vector<Defensor>coo_defensor, 
						 std::vector<Predator>coo_predator){
	for(unsigned int i(0); i<coo_collectors.size(); ++i){
		if(carre_type_centre_dans_grille(coo_collectors[i].get_carre()))
		{
			cout<<error_squarecell::print_outside
				  (coo_collectors[i].get_carre().x,
				   coo_collectors[i].get_carre().side, g_max-1);
			return false;
		}
	}
	for(unsigned int i(0); i<coo_defensor.size(); ++i){
		if(carre_type_centre_dans_grille(coo_defensor[i].get_carre())){
			cout<<error_squarecell::print_outside
				  (coo_defensor[i].get_carre().x,
				   coo_defensor[i].get_carre().side, g_max-1);
			return false; 
		}
	}
	for(unsigned int i(0); i<coo_predator.size(); ++i){
		if(carre_type_centre_dans_grille(coo_predator[i].get_carre())){
			cout<<error_squarecell::print_outside
			      (coo_predator[i].get_carre().x,
				   coo_predator[i].get_carre().side, g_max-1);
			return false;
		}
	}
	return true;
}

bool superposition_fourmis_grille(std::vector<Collector> coo_collectors, 
								  std::vector<Defensor>coo_defensor, 
								  std::vector<Predator>coo_predator, 
								  Grille& grille){
	int x_cellule=0, y_cellule=0;
	for(unsigned int y(0); y<coo_collectors.size(); ++y){
		if(grille.sup_grille(coo_collectors[y].get_carre(),
						     grille, x_cellule, y_cellule)){				   
			cout<<message::collector_overlap
			      (coo_collectors[y].get_carre().x,
				   coo_collectors[y].get_carre().y, x_cellule, 
				   y_cellule);
			return false; 
		}
	grille.occupation(coo_collectors[y].get_carre());
	} 
	for(unsigned int y(0); y<coo_defensor.size(); ++y){
		if(grille.sup_grille(coo_defensor[y].get_carre(),
						   grille, x_cellule, y_cellule)){
			cout<<message::defensor_overlap
				  (coo_defensor[y].get_carre().x,
				   coo_defensor[y].get_carre().y, x_cellule, 
				   y_cellule);
		return false; 
		}
	grille.occupation(coo_defensor[y].get_carre());
	} 
	for(unsigned int y(0); y<coo_predator.size(); ++y){
		if(grille.sup_grille(coo_predator[y].get_carre(), grille, 
						     x_cellule, y_cellule)){
			cout<<message::predator_overlap
				  (coo_predator[y].get_carre().x, 
				   coo_predator[y].get_carre().y);											
			return false; 
		}
	grille.occupation(coo_predator[y].get_carre());
	} 	
	return true;
}	

Fourmis::Fourmis(std::string line, int numero_fourmiliere1, int side1){
	std::istringstream data(line);
	unsigned int x1;
	unsigned int y1;
	unsigned int age1;
	if(!(data>> x1>> y1>>age1)){
		exit(EXIT_FAILURE);
	}else{
		carre.x=x1;
		carre.y=y1;
		carre.side=side1;
		age=age1;
		numero_fourmiliere=numero_fourmiliere1;
	}
}
		
Square Fourmis::get_carre() const{
	return carre;
}

int Fourmis::get_age() const{
	return age;
}
void Fourmis::set_age_simulation(){
	age=age+1;
}
	
int Fourmis::get_hillid2() const{
	return numero_fourmiliere; 
}

void Fourmis::set_coo(int x_, int y_){
	std::cout<<carre.x<<endl;
	std::cout<<carre.y<<endl;
	carre.x=x_;
	carre.y=y_;
	std::cout<<carre.x<<endl;
	std::cout<<carre.y<<endl;
}

Collector::Collector(std::string line, int numero_fourmiliere_):
	Fourmis(line, numero_fourmiliere_, sizeC){
		int a,b,c;
		std::istringstream data(line);
		std::string food;
		if(!(data>>a>>b>>c>>food)){
			exit(EXIT_FAILURE);				 
		}else{
			has_food=food;
		}
}


std::string Collector::get_has_food() const{
	return has_food;
}

bool coo_fourmis_dans_grille(std::vector<Collector> coo_collectors, 
							 std::vector<Defensor>coo_defensor, 
							 std::vector<Predator>coo_predator){
	for(unsigned int i(0); i<coo_collectors.size(); ++i){
		if(!coordonee_dans_grille(coo_collectors[i].get_carre())){
			return false;
		}
	}
	for(unsigned int i(0); i<coo_defensor.size(); ++i){
		if(!coordonee_dans_grille(coo_defensor[i].get_carre())){
			return false;
		}
	}
	for(unsigned int i(0); i<coo_predator.size(); ++i){
		if(!coordonee_dans_grille(coo_predator[i].get_carre())){
			return false;
		}
	}
	return true;
}

void dessinerfourmis(std::vector<Collector> coo_collectors, 
					 std::vector<Defensor>coo_defensor, 
					 std::vector<Predator>coo_predator, 
					 std::vector<Fourmiliere> mais_ant){
	for(unsigned int i(0); i<coo_collectors.size(); ++i){
		carre_damier(coo_collectors[i].get_carre(), 
					 coo_collectors[i].get_hillid2(),'F', 'C');
		if(coo_collectors[i].get_etat()==LOADED){
			Square l;
			l.x=coo_collectors[i].get_carre().x;
			l.y=coo_collectors[i].get_carre().y;
			l.side=1;
		losangesquare(l, WHITE);
		}
	}
	for(unsigned int i(0); i<coo_defensor.size(); ++i){
		carre_croix(coo_defensor[i].get_carre(), 
					coo_defensor[i].get_hillid2(), 'F', 'C');
	}
	for(unsigned int i(0); i<coo_predator.size(); ++i){
		carrecompletsquare(coo_predator[i].get_carre(), 
						   coo_predator[i].get_hillid2(), 'F');
	}
	for(unsigned int i(0); i<mais_ant.size(); ++i){
		carrecompletsquare(mais_ant[i].get_g(), 
						   mais_ant[i].get_anthillid(), 'F');
	}
}

void Collector::set_etat(Etat_collector etat_){
	etat=etat_;
}

Etat_collector Collector::get_etat(){
	return etat;
}

void Fourmis::set_x(string signe){
	if(signe=="-"){
		--carre.x;
	}
	if(signe=="+"){
		++carre.x;
	}
	if(signe=="--"){
		carre.x=carre.x-2;
	}
	if(signe=="++"){
		carre.x=carre.x+2;
	}
}
void Fourmis::set_y(string signe){
	if(signe=="-"){
		--carre.y;
	}
	if(signe=="+"){
		++carre.y;
	}
	if(signe=="--"){
		carre.y=carre.y-2;
	}
	if(signe=="++"){
		carre.y=carre.y+2;
	}
}
