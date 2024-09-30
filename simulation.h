// fichier : simulation.h
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB

#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include "fourmiliere.h"
#include "fourmis.h"
#include "nourriture.h"
#include "squarecell.h"

#include <vector>
#include <string>

struct Chemin{
	double xA;
	double xB;
	double xC;
	double yA;
	double yB;
	double yC;
	double xI;
	double yI;
	double nbr_dobs;
};

class Simulation { 
	
private : 
		
	std::vector<Nourriture> food_;
	std::vector<Fourmiliere> mais_ant;
	std::vector<Collector> coo_collectors;
	std::vector<Defensor>coo_defensor;
	std::vector<Predator>coo_predator;
	std::vector<int> k;
	
public :
	
	bool lecture(std::string nomFichier);
	bool initialisation_parametres(std::string line);
	bool lectF(int nombre_fourmiliere_lu, int total);	
	bool verification_donnees_grille();
	int getsizeant();
	int getnbC(int i);
	int getnbD(int i);
	int getnbP(int i);
	void setnbC(int i, std::string signe);
	void setnbD(int i, std::string signe);
	void setnbP(int i, std::string signe);
	double getsizefood(int i);
	int getfoodsize();
	void dessinerentites();
	void reset_entites();
	void save_grille(std::string filename);
	void reset_compteurs();
	void add_food(Nourriture n);
	void changer_total_food();
	void gerer_vies();
	void changer_etat_vie_collector(int i);
	void changer_etat_vie_defensor(int i);
	void changer_etat_vie_predator(int i);
	Square get_square_fourmiliere(int i);
	void getbig(int i);
	bool obstacle1(Fourmiliere F, int indice);
	bool obstacle2(Fourmiliere F, int indice);
	bool obstacle3(Fourmiliere F, int indice);
	bool obstacle4(Fourmiliere F, int indice);
	int agrandissement(Fourmiliere F, int indice);
	bool sortiedumonde1(Fourmiliere F);
	bool sortiedumonde2(Fourmiliere F);
	bool sortiedumonde3(Fourmiliere F);
	bool sortiedumonde4(Fourmiliere F);
	std::vector<Nourriture> n_atteignable(Collector& C);
	std::vector<int> n_distance(Collector& C, std::vector<Nourriture> v_atteign );
    void go_to(Collector& C, int where, int butx, int buty, int i_);

    void fairebougercoll();
	void generer_food();
    void deplacement_defensor();
    void deplacement_generator(); 
    void deplacement_predator();
    void creation_fourmis(int i);
    void creation_fourmis_init();
    int getmin(std::vector<int> tab);
    Square entiteplusproche(int i, int num);
	int get_min(Collector& C);
	void mort_cause_predator(int i, int num);
	void set_true_size();
	bool pas_collision_def(int i,  Square cible, int a);
	void kill_all_fourmiliere(int m, int& nbc, int& nbd, int& nbp);
	void creer_collector(int i);
	void creer_defensor(int i);
	void creer_predator(int i);
	void end_of_life_collector(int c, int &nbc);
	void end_of_life_defensor(int d, int &nbd);
	void end_of_life_predator(int p, int &nbp);
	void gerer_vies_generator();
	void gerer_vies_defensor();
	void liberation_generator(int i, std::string signe,
							  std::string coo);
	bool choix_chem(Collector C, Nourriture N, int butx, int buty);
    void diagonale(Collector& C, int i, int butx, int buty);
    void chem_alternatif(Collector& C, int i, int butx, int buty, bool nourri, int coll);
    void diagonalenourri(Collector& C, int i, int x, int y, int butx, int buty);
    int nbr_dobstaclchem(Collector& C, int sensa, int sensb, int vx, int vy, int indice, bool nourri);
    void manger_nourriture(Collector& C, int indice_nourri);
    void go_back(Collector& C, int i);
    void ramener_nourriture(Collector& C);
    void set_coordo(Collector& C, Square f, Square n, bool nourri, 
					int indice);
	void Bloc1(Collector& C, int sensa, int sensb, int vx, int vy, 
			   int indice, bool nourri, Square f, Square n, int count);
	void Bloc2(Collector& C, int sensa, int sensb, int vx, int vy, 
			   int indice, bool nourri, Square f, Square n, int count);
	void Bloc3(Collector& C, int sensa, int sensb, int vx, int vy, 
	           int indice, bool nourri, Square f, Square n, int count);
	void Bloc4(Collector& C, int sensa, int sensb, int vx, int vy, 
	           int indice, bool nourri, Square f, Square n, int count);
    void set_count(Square f, int count);
    void setcoosquare( Square f, int i, int j);
    void occupation(Square f, Square n);
    void calculdessens(Collector& C, int vx, int vy, int sens1a, 
                       int sens1b,int sens2a, int sens2b, int Nx, 
                       int Ny,  int Cx,  int Cy);
    void contact_fourmi();
    void creation_nourriture(Square c);
};

int nbr_dobstacle(Chemin chem);
void resetcompteurs();
static Grille grille;


#endif
