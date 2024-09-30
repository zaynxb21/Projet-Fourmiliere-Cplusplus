// fichier : fourmis.h
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB

#ifndef FOURMIS_H_INCLUDED
#define FOURMIS_H_INCLUDED


#include "fourmiliere.h"
#include "constantes.h"
#include "squarecell.h"

#include <string>
#include <iostream>
#include <sstream>

class Fourmis {
	
protected :
		
	Square  carre;
	int age;
	int numero_fourmiliere;
	bool end_of_life=false;
		
public :
		
	Square get_carre() const;
	int get_age() const;
	int get_hillid2() const;
	void set_age_simulation();
	void set_coo(int x, int y);
	Fourmis(){};
	Fourmis(std::string line, int numero_fourmiliere1, int side1);
	Fourmis(int x1, int y1, int age1,int numero_fourmiliere1,int side1){
		carre.x=x1; carre.y=y1; age=age1; 
		numero_fourmiliere=numero_fourmiliere1; carre.side=side1;
	}
	void set_y(std::string signe);
	void set_x(std::string signe);	
	bool get_end_of_life(){
		return end_of_life;
	}
	void set_end_of_life(bool vie){
		end_of_life=vie;
	}
};

class Collector : public Fourmis {
	
private :

	Etat_collector etat=EMPTY;	
	
protected :
	
	std::string has_food;
		    
public :
	
	void set_etat(Etat_collector etat_);	
	Etat_collector get_etat();
	Collector(){};	
	Collector(std::string line, int numero_fourmiliere_);
	Collector(int x1, int y1, int age1, std::string etat, 
			  int numero_fourmiliere1): Fourmis(x1, y1, age1, 
			  numero_fourmiliere1,sizeC){
				  has_food=etat;
	};
	std::string get_has_food()const ;
			
};
	
class Defensor : public Fourmis {
	
public :
		
	Defensor(std::string line, int numero_fourmiliere1)
		: Fourmis(line, numero_fourmiliere1,sizeD ){}
	Defensor(int x1, int y1, int age1, int numero_fourmiliere1)
		: Fourmis(x1, y1, age1, numero_fourmiliere1,sizeD ){}
};
	
class Predator : public Fourmis {
	
public : 
		
	Predator(std::string line, int numero_fourmiliere1)
		: Fourmis(line, numero_fourmiliere1, sizeP){}
	Predator(int x1, int y1, int age1, int numero_fourmiliere1)
		: Fourmis(x1, y1, age1, numero_fourmiliere1, sizeP){}
};

bool fourmis_dans_grille(std::vector<Collector> coo_collectors, 
						 std::vector<Defensor>coo_defensor, 
						 std::vector<Predator>coo_predator);
bool defensor_dans_fourmiliere(std::vector<Defensor>coo_defensor, 
							   std::vector<Fourmiliere> mais_ant);
bool coo_fourmis_dans_grille(std::vector<Collector> coo_collectors, 
							 std::vector<Defensor>coo_defensor, 
							 std::vector<Predator>coo_predator);
bool superposition_fourmis_grille(std::vector<Collector> coo_collectors, 
								  std::vector<Defensor>coo_defensor, 
								  std::vector<Predator>coo_predator,
								  Grille& grille);

void dessinerfourmis(std::vector<Collector> coo_collectors, 
					 std::vector<Defensor>coo_defensor, 
					 std::vector<Predator>coo_predator, 
					 std::vector<Fourmiliere> mais_ant);
#endif
