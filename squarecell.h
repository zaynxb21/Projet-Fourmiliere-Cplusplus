// fichier :squarecell.h
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB

#ifndef SQUARECELL_H_INCLUDED
#define SQUARECELL_H_INCLUDED

#include "color.h"

#include <vector>
#include <string>
#include <cmath>

struct Square {
	int x;
	int y;
	int side; 
};

int set_indice_j(Square b);
int set_indice_i(Square b);
 

bool carre_compris_dans_autre(Square num1, Square num2);
bool sup_carree(Square F1,Square F2);
bool coordonee_dans_grille(Square c);
bool carre_type_centre_dans_grille(Square c);
bool carre_type_non_centre_dans_grille(Square dedans);
bool carre_centre_dans_non_centre(Square F1, Square C);

constexpr short int g_dim(7);
const short int g_max(pow(2,g_dim));

class Grille {
	
private :
	
	std::vector <std::vector<bool>> grid = 
		 std::vector<std::vector<bool>>(g_max,std::vector<bool>(g_max));
		                  
public :

	Grille ();
	void nettoyergrille();
	int get_size();
	void set_cell(int a, int b, bool valeur);
	void initialization(bool in);
	bool get_case(int lgn, int cln);
	void set_cell4square(int a_c, int b_c, bool valeur_c);
	void occupation (Square c);
	void liberation (Square c);
	int get_value(int value);
	bool sup_grille (Square c, Grille tab, int& x_cellule, 
					 int& y_cellule);
	bool sup_grille1 (Square c, Grille tab);
		
};

void carrecompletsquare(Square c, int a, char nuance);
void losangesquare(Square c, Color couleur);
void carre_croix(Square c, int a, char nuance1, char nuance2);
void carre_damier(Square c, int a, char nuance1, char nuance2);
void carre_vide(Square c, int a, char nuance);


#endif
