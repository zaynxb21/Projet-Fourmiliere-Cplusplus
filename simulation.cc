// fichier : simulation.cc
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB
// contibution au fichier 
// Yasmine : 60% de la mise au point des fonctions / methodes de ce fichier
// Zaynab : 40% de la mise au point des fonctions / methodes de ce fichier

#include "simulation.h"
#include "constantes.h"
#include "squarecell.h"
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <sstream> 
#include <algorithm>
#include <stdlib.h>
#include <random>
 
using namespace std;

enum Lecture {nbN, coo_food, nbF, NBC, NBD, NBP, Donnees_Fourmilieres, 
			  END};
bool Simulation::lecture(string nomFichier){
    ifstream fichier(nomFichier);
    string line;
    if(!fichier.fail()){
        while(getline(fichier>>ws, line)){
			if(line[0] == '#') continue;
				if(!initialisation_parametres(line)){
					return false;}}}
	else{
		cout << "Le fichier texte est absent" << endl;}
    fichier.close();
    return true;
}

static int countx(0), total(0), compt_c(0), compt_d(0), compt_p(0), 
	       count1(0), count2(0), nbr_F(0);
	
bool Simulation::initialisation_parametres(string line){
	static int etat(nbN); 
	istringstream data(line);
	switch(etat){
		case nbN : 
			data >> total; countx=0;
			if (total==0) etat=nbF; else etat=coo_food;
			break;
		case coo_food:
			if (countx != total){
				Nourriture n(line); food_.push_back(n); ++countx;}
			if(countx==total) etat=nbF;
			break;
		case nbF : 
			data >> total; countx=0;
			if (total==0){
				etat=nbN;
				if(verification_donnees_grille()) return true;
				else return false;}
			etat=Donnees_Fourmilieres;
			break;
		case Donnees_Fourmilieres : 
			if(total!=0){
				Fourmiliere f(line, nbr_F); mais_ant.push_back(f); }
			compt_c=mais_ant[nbr_F].get_nbr_collector(); 
			compt_d=mais_ant[nbr_F].get_nbr_defensor();
			compt_p=mais_ant[nbr_F].get_nbr_predator();++nbr_F;countx=0;
			if (compt_c!=0){etat=NBC;}
			if (compt_c==0 and compt_d!=0){etat= NBD;}
			if (compt_c==0 and compt_d==0 and compt_p!=0){etat= NBP;}
			if (compt_c==0 and compt_d==0 and compt_p==0){
				if (!(lectF(nbr_F, total))){
					resetcompteurs(); etat=nbN;
					if(verification_donnees_grille()) return true;
					else return false;}
				else{ 
					etat=Donnees_Fourmilieres; }}
			break;
		case NBC :
			if(compt_c!=0){
				Collector c(line, nbr_F-1); coo_collectors.push_back(c);
				++countx; }
			if (compt_c==countx and compt_d!=0) etat=NBD;
			if (compt_c==countx and compt_d==0 and compt_p!=0) etat=NBP;
			if (compt_c==countx and compt_d==0 and compt_p==0 
			    and lectF(nbr_F, total)) etat=Donnees_Fourmilieres; 
			if (compt_c==countx and compt_d==0 and compt_p==0 
			    and !lectF(nbr_F, total)){ resetcompteurs(); etat=nbN;
				if(verification_donnees_grille()) return true;
				else return false;}
			break;
		case NBD :
			if (compt_d!=0){
				Defensor d(line, nbr_F-1); coo_defensor.push_back(d);
				++count1;}
			if (compt_d==count1 or compt_d==0) etat= NBP; 
			if (compt_d==count1 and compt_p==0 and lectF(nbr_F, total)) 
				etat=Donnees_Fourmilieres; 
			if (compt_d==count1 and compt_p==0 and !lectF(nbr_F,total)){
				resetcompteurs(); etat=nbN;
				if(verification_donnees_grille()) return true;
				else return false;}
			break;
		case NBP :
			if (compt_p !=0 ){
				Predator p(line, nbr_F-1); coo_predator.push_back(p);
				++count2;}
			if (compt_p==count2){
				if(lectF(nbr_F, total)){
					countx=0; count1=0; count2=0; 
					etat=Donnees_Fourmilieres;}
				if (!(lectF(nbr_F, total))){
					resetcompteurs(); etat=nbN;
					if(verification_donnees_grille()) return true;
					else return false;}}
			break;}
		return true;
}

bool Simulation::lectF(int nombre_fourmiliere_lu, int total){
	if (nombre_fourmiliere_lu!=total){
		return true;}
	else{
		return false;}
}

bool Simulation::verification_donnees_grille(){
	if(coo_fourmis_dans_grille(coo_collectors, coo_defensor, 
							   coo_predator) 
	   and coo_anthill_dans_grille(mais_ant) and
	   food_dans_grille(food_) and 
	   fourmis_dans_grille(coo_collectors, coo_defensor, coo_predator) 
	   and fourmiliere_dans_grille(mais_ant) and 
	   superposition_nourriture_grille(food_, grille) and
	   superposition_fourmilieres(mais_ant, grille) and 
	   generator_dans_fourmiliere(mais_ant) and 
	   defensor_dans_fourmiliere(coo_defensor, mais_ant) and  
	   superposition_fourmis_grille(coo_collectors, coo_defensor, 
									coo_predator, grille)){
		return true;}
	else{
		return false;}						 
}

int Simulation::getsizeant(){
	return mais_ant.size();
}

int Simulation::getfoodsize(){
	return food_.size();
}

double Simulation::getsizefood(int i){
	return mais_ant[i].get_nbr_food();
}

int Simulation::getnbC(int i){
	return mais_ant[i].get_nbr_collector();
}

int Simulation::getnbD(int i){
	return mais_ant[i].get_nbr_defensor();
}

int Simulation::getnbP(int i){
	return mais_ant[i].get_nbr_predator();
}

void Simulation::setnbC(int i, string signe){
	if(signe=="-"){
		mais_ant[i].set_nbc();
	}else{
		mais_ant[i].setnbc();
	}
}

void Simulation::setnbD(int i, string signe){
	if(signe=="-"){
		mais_ant[i].set_nbd();
	}else{
		mais_ant[i].setnbd();
	}
}

void Simulation::setnbP(int i, string signe){
	if(signe=="-"){
		mais_ant[i].set_nbp();
	}else{
		mais_ant[i].setnbp();
	}
}

void Simulation::set_true_size(){
	int nbF=mais_ant.size();
	for(int i(0); i<nbF; ++i){
		int nbc=mais_ant[i].get_nbr_collector();
		int nbd=mais_ant[i].get_nbr_defensor();
		int nbp=mais_ant[i].get_nbr_predator();
		int s=sizeG*sizeG;
		int w=sizeC*sizeC*nbc;
		int o=sizeD*sizeD*nbd;
		int u=sizeP*sizeP*nbp;
		mais_ant[i].set_side(int(sqrt(4*(s+w+o+u))));
	}
}	
	
void Simulation::add_food(Nourriture n){
	food_.push_back(n);
	
}

void Simulation::gerer_vies(){
	int nbc=coo_collectors.size(), nbd=coo_defensor.size(), 
		nbp=coo_predator.size();
	for (int c(0); c<nbc; ++c){
		coo_collectors[c].set_age_simulation();
		if (coo_collectors[c].get_age()>=300){
			changer_etat_vie_collector(c);
			end_of_life_collector(c, nbc);
		}
	}
	for (int d(0); d<nbd; ++d){
		coo_defensor[d].set_age_simulation();
		if (coo_defensor[d].get_age()>=300){
			changer_etat_vie_defensor(d);
			end_of_life_defensor(d, nbd);
		}
	}
	for (int p(0); p<nbp; ++p){
		coo_predator[p].set_age_simulation();
		if (coo_predator[p].get_age()>=300){
			changer_etat_vie_predator(p);
			end_of_life_predator(p, nbp);
		}
	}
	gerer_vies_generator();
	gerer_vies_defensor();
}

void Simulation::gerer_vies_generator(){
	Square F; Square f; 
	int nbF=mais_ant.size(); 
	int nbc=coo_collectors.size(), nbd=coo_defensor.size();
	int nbp=coo_predator.size();
	for(int m(0); m<nbF; ++m){
		f.x=set_indice_i(mais_ant[m].get_g())-2;
		f.y=set_indice_j(mais_ant[m].get_g())-2;
		f.side=mais_ant[m].get_g().side;
		F.x=set_indice_i(mais_ant[m].get_F());
		F.y=set_indice_j(mais_ant[m].get_F());
		F.side=mais_ant[m].get_F().side;
		if(f.x==F.x or f.x+5==F.x+F.side or f.y==F.y or 
		   f.y+5==F.y+F.side){
			kill_all_fourmiliere(m, nbc, nbd, nbp);
			swap(mais_ant[m], mais_ant[mais_ant.size()-1]);
			mais_ant.pop_back(); --nbF;
		}
		if(carre_compris_dans_autre(mais_ant[m].get_g(),
									mais_ant[m].get_F())){
		swap(mais_ant[m], mais_ant[mais_ant.size()-1]);
			mais_ant.pop_back(); --nbF;
			kill_all_fourmiliere(m, nbc, nbd, nbp);
		}
	}
}
	
void Simulation::gerer_vies_defensor(){
	Square F; Square de;
	int nbF=mais_ant.size(); 
	int nbc=coo_collectors.size(), nbd=coo_defensor.size();
	int nbp=coo_predator.size();
	for(int d(0); d<nbd; ++d){
		de.x=set_indice_i(coo_defensor[d].get_carre())-1;
		de.y=set_indice_j(coo_defensor[d].get_carre())-1;
		de.side=coo_defensor[d].get_carre().side;
		F.x=set_indice_i(mais_ant[d].get_F());
		F.y=set_indice_j(mais_ant[d].get_F());
		F.side=mais_ant[d].get_F().side;
		if(de.x==F.x or de.x+3==F.x+F.side or de.y==F.y 
		   or de.y+3==F.y+F.side){
			if(nbd==1){
				kill_all_fourmiliere(coo_defensor[d].get_hillid2(), 
				                     nbc, nbd, nbp);
				swap(mais_ant[coo_defensor[d].get_hillid2()], 
				     mais_ant[mais_ant.size()-1]);
				mais_ant.pop_back(); --nbF;
			}else{
				swap(coo_defensor[d],coo_defensor[nbd-1]);
				coo_defensor.pop_back(); --nbd;
			}
		}
		if(carre_compris_dans_autre(coo_defensor[d].get_carre(),
		   mais_ant[coo_defensor[d].get_hillid2()].get_F())){
			if(nbd==1){
				kill_all_fourmiliere(coo_defensor[d].get_hillid2(), 
									 nbc, nbd, nbp);
				swap(mais_ant[coo_defensor[d].get_hillid2()], 
				     mais_ant[mais_ant.size()-1]);
				mais_ant.pop_back(); --nbF;
			}else{
				swap(coo_defensor[d],coo_defensor[nbd-1]);
				coo_defensor.pop_back(); --nbd;
			}
		}
	}
}
	
void Simulation::changer_total_food(){
	int nbc=coo_collectors.size(),nbF=mais_ant.size(), 
		nbd=coo_defensor.size(), nbp=coo_predator.size();
	for (int i(0); i<nbF; ++i){
		int nbT=1+mais_ant[i].get_nbr_collector()
		        +mais_ant[i].get_nbr_defensor()
				+mais_ant[i].get_nbr_predator();
		if (mais_ant[i].get_nbr_food()-nbT*0.1>0){
			mais_ant[i].set_nbr_food(nbT*0.1);
		}else if (mais_ant[i].get_nbr_food()-nbT*0.1<=0){
			mais_ant[i].set_nbr_food(0);
			for (int c(0); c<nbc; ++c){
				if (coo_collectors[c].get_hillid2()==i){
					swap(coo_collectors[c], coo_collectors[nbc-1]);
					coo_collectors.pop_back();
				}
			}
			for (int d(0); d<nbd; ++d){
				if (coo_defensor[d].get_hillid2()==i){
					swap(coo_defensor[d], coo_defensor[nbd-1]);
					coo_defensor.pop_back();
				}
			}
			for (int p(0); p<nbp; ++p){
				if (coo_predator[p].get_hillid2()==i){
					swap(coo_predator[p], coo_predator[nbp-1]);
					coo_predator.pop_back();
				}
			}
			swap(mais_ant[i], mais_ant[nbF-1]);
			mais_ant.pop_back();
		}
	}
}

void Simulation::dessinerentites(){
	dessinernourriture(food_);
	dessinerfourmis(coo_collectors,coo_defensor,coo_predator,mais_ant);
	dessinerfourmilieres(mais_ant);
}

void Simulation::reset_entites(){
	food_.clear();
	mais_ant.clear();
	coo_collectors.clear();
	coo_defensor.clear();
	coo_predator.clear();
}

void Simulation::save_grille(string filename){
	int nbf=food_.size(), nbc=coo_collectors.size(),nbF=mais_ant.size(), 
		nbd=coo_defensor.size(), nbp=coo_predator.size();
	cout<<"\n"<<nbc<<"\n"<<nbd<<"\n"<<nbp<<"\n"<<endl;
	ofstream fichier(filename);
	if(fichier.is_open()){
		fichier<<"# Nom du scenario de test"<<"\n"<<"#"<<"\n"<<"\n";
		fichier<< "# nb food"<<"\n"<<nbf<<"\n"<<"\n";
		if(nbf!=0) fichier<<"# food"<<"\n";
		for (int i(0); i<nbf; ++i){
			fichier<<food_[i].get_r().x<<" "<<food_[i].get_r().y<<"\n";}
		fichier<<"\n"<< nbF << " # nb anthill"<<"\n";
		for (int i(0); i<nbF; ++i){
			fichier<<"\n"<<"	"<<mais_ant[i].get_F().x<<" "
			<<mais_ant[i].get_F().y<<" "<<mais_ant[i].get_F().side
			<<" "<<mais_ant[i].get_g().x<<" "<<mais_ant[i].get_g().y
			<<" "<<mais_ant[i].get_nbr_food()<<" "
			<<mais_ant[i].get_nbr_collector()<<" "
			<<mais_ant[i].get_nbr_defensor()<<" "
			<<mais_ant[i].get_nbr_predator()<<" # nb anthill"<<" #"
			<<i+1<<"\n";
			if (nbc!=0 or nbd!=0 or nbp!=0){
				fichier<<"\n"<<"	# collectors:"<<"\n";
				for (int c(0); c<nbc; ++c){
					if (coo_collectors[c].get_hillid2()==i){
						fichier<<"	"<<coo_collectors[c].get_carre().x
						<<" "<<coo_collectors[c].get_carre().y<<" "
						<<coo_collectors[c].get_age()<<" "
						<<coo_collectors[c].get_has_food()<<"\n";}}
				fichier<<"\n"<<"	# defensor:"<<"\n";
				for (int d(0); d<nbd; ++d){
					if (coo_defensor[d].get_hillid2()==i){
						fichier<<"	"<<coo_defensor[d].get_carre().x
						<<" "<<coo_defensor[d].get_carre().y<<" "
						<<coo_defensor[d].get_age()<<"\n";}}
				fichier<<"\n"<<"	# predator:"<<"\n";
				for (int p(0); p<nbp; ++p){
					if (coo_predator[p].get_hillid2()==i){
						fichier<<"	"<<coo_predator[p].get_carre().x
						<<" "<<coo_predator[p].get_carre().y<<" "
						<<coo_predator[p].get_age()<<"\n";}}}}} 
	fichier.close();
}

void resetcompteurs(){
	compt_c=0; compt_d=0; compt_p=0; countx=0; count1=0; 
	count2=0; total=0; nbr_F=0;
}


	

Square Simulation::get_square_fourmiliere(int i){
	return mais_ant[i].get_F();
}

bool Simulation::obstacle1(Fourmiliere F, int indice){
	Square carre = F.get_F();
	carre.side++;
	bool obs = false;
	for(int i=0; i<getsizeant(); i++){
		if(i==indice){
			continue;
		}
		if(sup_carree(carre,get_square_fourmiliere(i))){
			obs= true;
		}
	}
	return obs;
}

bool Simulation::obstacle2(Fourmiliere F, int indice){
	Square carre = F.get_F();
	carre.side++;
	carre.y--;
	bool obs = false;
	for(int i=0; i<getsizeant(); i++){
		if (i==indice){
			continue;}
		if (sup_carree(carre,get_square_fourmiliere(i))){
			obs= true;
		}
	}
	return obs;
}
	
bool Simulation::obstacle3(Fourmiliere F, int indice){
	Square carre = F.get_F();
	carre.side++;
	carre.x--;
	carre.y--;
	bool obs = false;
	for (int i=0; i<getsizeant(); i++){
		if (i==indice){
			continue;
		}
		if (sup_carree(carre,get_square_fourmiliere(i))){
			obs= true;
		}
	}
	return obs;
}

bool Simulation::obstacle4(Fourmiliere F, int indice){
	Square carre = F.get_F();
	carre.side++;
	carre.x--;
	bool obs = false;
	for (int i=0; i<getsizeant(); i++){
		if (i==indice){
			continue;
		}
		if (sup_carree(carre,get_square_fourmiliere(i))){ 
			obs= true;
		}
	}
	return obs;
}

bool Simulation::sortiedumonde1(Fourmiliere F){
	Square carre = F.get_F();
	carre.side++;
	bool obs = false;
	if (carrefourmil_dans_grille(carre)){
		obs= true;
	}
	return obs;
}

bool Simulation::sortiedumonde2(Fourmiliere F){
	Square carre = F.get_F();
	carre.side++;
	carre.y--;
	bool obs = false;
	if (carrefourmil_dans_grille(carre)){
		obs= true;
	}
	return obs;
}

bool Simulation::sortiedumonde3(Fourmiliere F){
	Square carre = F.get_F();
	carre.side++;
	carre.x--;
	carre.y--;
	bool obs = false;
	if (carrefourmil_dans_grille(carre)){
		obs= true;
	}
	return obs;
}

bool Simulation::sortiedumonde4(Fourmiliere F){
	Square carre = F.get_F();
	carre.side++;
	carre.x--;
	bool obs = false;
	if (carrefourmil_dans_grille(carre)){
		obs= true;
	}
	return obs;
}

int Simulation::agrandissement(Fourmiliere F, int indice){
	int n=0;
	if (!obstacle1(F, indice) and !sortiedumonde1(F)){
		n=1;
	}else if (!obstacle2(F, indice) and !sortiedumonde2(F)){
		n=2;
	}else if (!obstacle3(F, indice) and !sortiedumonde3(F)){
		n=3;
	}else if (!obstacle4(F, indice) and !sortiedumonde4(F)){
		n=4;
	}else if (n==0){
		n=0;
	}
	return n;
}

void Simulation::getbig(int i){
	int agr;
	mais_ant[i].set_state(FREE);
	agr=agrandissement(mais_ant[i], i);
	if (agr==1){
		mais_ant[i].set_side(mais_ant[i].get_F().side+1);
	}else if (agr==2){
		mais_ant[i].set_side(mais_ant[i].get_F().side+1);
		mais_ant[i].set_y(mais_ant[i].get_F().y-1);
	}else if (agr==3){
		mais_ant[i].set_side(mais_ant[i].get_F().side+1);
		mais_ant[i].set_y(mais_ant[i].get_F().y-1);
		mais_ant[i].set_x(mais_ant[i].get_F().x-1);
	}else if (agr==4){
		mais_ant[i].set_side(mais_ant[i].get_F().side+1);
		mais_ant[i].set_x(mais_ant[i].get_F().x-1);
	}else if (agr==0){
		mais_ant[i].set_state(CONSTRAINED);
	}	
 
}

std::vector<Nourriture> Simulation::n_atteignable(Collector& C){
	vector<Nourriture> tab;
	for (unsigned int i=0; i<food_.size(); i++){
		if ((!(food_[i].get_r().x%2) and !(C.get_carre().x%2) 
			and !(food_[i].get_r().y%2) and !(C.get_carre().y%2)) 
			or ((food_[i].get_r().x%2) and (C.get_carre().x%2) 
			and (food_[i].get_r().y%2) and (C.get_carre().y%2))
			or (!(food_[i].get_r().x%2) and (C.get_carre().x%2) 
			and !(food_[i].get_r().y%2) and (C.get_carre().y%2))
			or ((food_[i].get_r().x%2) and !(C.get_carre().x%2) 
			and (food_[i].get_r().y%2) and !(C.get_carre().y%2))
			or ((food_[i].get_r().x%2) and !(C.get_carre().x%2) 
			and !(food_[i].get_r().y%2) and (C.get_carre().y%2))
			or (!(food_[i].get_r().x%2) and (C.get_carre().x%2) 
			and (food_[i].get_r().y%2) and !(C.get_carre().y%2))
			or (!(food_[i].get_r().x%2) and !(C.get_carre().x%2) 
			and (food_[i].get_r().y%2) and (C.get_carre().y%2))
			or (!(food_[i].get_r().x%2) and !(C.get_carre().x%2) 
			and (food_[i].get_r().y%2) and (C.get_carre().y%2))){ 
				tab.push_back(food_[i]);
	}}
	return tab;
}

std::vector<int> Simulation::n_distance(Collector& C, std::vector<Nourriture> v_atteign ){
	vector<int> tab;
	for (unsigned int i=0; i<v_atteign.size(); i++){
		tab.push_back(max(abs(v_atteign[i].get_r().x - C.get_carre().x),abs(v_atteign[i].get_r().y - C.get_carre().y)));
	}
	return tab;
}

int Simulation::get_min(Collector& C)
{	
	vector<int>tab = n_distance(C, n_atteignable(C));
    int min = 0;
    for (unsigned int i=1;i<tab.size();i++){   
		if(tab[i] < tab[min]){
           min = i;
        }
    }
    return min;
}

bool Simulation::choix_chem(Collector C, Nourriture N, int butx, int buty){
	int i=get_min(C);
	if(((n_atteignable(C)[i].get_r().y>=C.get_carre().y) and (n_atteignable(C)[i].get_r().x>=C.get_carre().x)) 
		or ((n_atteignable(C)[i].get_r().y<=C.get_carre().y) and (n_atteignable(C)[i].get_r().x<=C.get_carre().x))){
		return true;
	} else {
		return false;
	}
}
void Simulation::diagonale(Collector& C, int i, int butx, int buty){
	Square Y;
	Square n;
	if (C.get_etat()==EMPTY){
		n.x=n_atteignable(C)[i].get_r().x;
		n.y=n_atteignable(C)[i].get_r().y;
	}
		if (buty>C.get_carre().y and butx>C.get_carre().x){
			if (C.get_etat()==EMPTY){
				grille.liberation(n);}
			Y.x=C.get_carre().x+1;
			Y.y=C.get_carre().y+1;	
			if (!grille.get_case(Y.x+1, Y.y)){
			C.set_coo(C.get_carre().x+1, C.get_carre().y+1);}
		}
		else if (buty<C.get_carre().y and butx>C.get_carre().x){
			C.set_coo(C.get_carre().x+1,C.get_carre().y-1);
		}
		else if (buty<C.get_carre().y and butx<C.get_carre().x){
			C.set_coo(C.get_carre().x-1,C.get_carre().y-1);
		}
		else if (buty>C.get_carre().y and butx<C.get_carre().x){
			C.set_coo(C.get_carre().x-1,C.get_carre().y+1);
		}	
}

void Simulation::set_coordo(Collector& C, Square f, Square n, bool nourri, int indice){
	if(nourri){
			n.side=1;
			n.x=n_atteignable(C)[indice].get_r().x;
			n.y=n_atteignable(C)[indice].get_r().y;
	}else{
			n.side=5;
			n.x=mais_ant[C.get_hillid2()].get_g().x;
			n.y=mais_ant[C.get_hillid2()].get_g().y;
	}
		grille.liberation(f);
		grille.liberation(n);
	
	}
	
void Simulation::set_count(Square f, int count){
	if (grille.sup_grille1(f, grille)){
					count=count+1;}
}
	
void Simulation::setcoosquare( Square f, int i, int j){
	f.x= f.x+i;
	f.y=f.y+j;
	}	

void Simulation::occupation(Square f, Square n){
	grille.occupation (f);
	grille.occupation(n);
	}
	
void Simulation::Bloc1(Collector& C, int sensa, int sensb, int vx,
						int vy, int indice, bool nourri, Square f,
						Square n, int count){
	if (sensa==1 and sensb==1){
		set_coordo( C, f,  n, nourri, indice);
		for ( int i=1; i<=((abs(vx+vy)/2)); ++i){
			if (f.x>=126 and f.y>=126){
				setcoosquare( f, -1, -1);
			}else if (f.x>=126){
				setcoosquare( f, -1, 1);
			}else if (f.y>=126){
				setcoosquare(f, 1, -1);
			}else{
				setcoosquare(f, 1, 1);}	
				set_count(f, count);}
		for (int i=1; i<=((abs(vx-vy)/2)-2); i++){
			if (n.x >C.get_carre().x){
				setcoosquare(f, 1, -1);
				set_count(f, count);
			}else {
				setcoosquare(f, -1, 1);
				set_count(f, count);}}
	if (f.x==127 and f.y==127){
				f.side=2;
				setcoosquare(f, -0.5, -0.5);
				occupation(f, n);
			}else if (f.y==127){
				f.side=2;
				setcoosquare(f, -0.5, -0.5);
				grille.occupation (f);
				f.x=f.x+1;
				occupation(f, n);
			}else if (f.x==127){
				f.side=2;
				setcoosquare(f, -0.5, -0.5);
				grille.occupation (f);
				f.y=f.y+1;
				occupation(f, n);
			}else{
				grille.occupation (f);}}}
				
void Simulation::Bloc2(Collector& C, int sensa, int sensb, int vx, 
						int vy, int indice, bool nourri, Square f, 
						Square n, int count){
	if (sensa==1 and sensb==-1 ){
		set_coordo( C, f,  n, nourri, indice);
		for ( int i=1; i<=((abs(vx+vy)/2)); ++i){
			if (f.x>126 and f.y<0){
					setcoosquare(f, -1, 1);
				}else if (f.x>126){
					setcoosquare(f, -1, -1);
				}else if (f.y<0){
					setcoosquare(f, 1, 1);
				}else{	
					setcoosquare(f, 1, -1);}
				set_count(f, count);}
		for (int i=1; i<=((abs(vx-vy)/2)); i++){
			if (n.x >C.get_carre().x){
				setcoosquare(f, 1, 1);
				set_count(f, count);
			}else{				
				setcoosquare(f, -1, -1);
				set_count(f, count);}}
		if (f.x==127 and f.y==0){
				f.side=2;
				setcoosquare(f, -0.5, -0.5);
				occupation(f, n);
		}else if (f.y==0){
				f.side=2;
				setcoosquare(f, -0.5, 0.5);
				grille.occupation (f);
				f.x=f.x+1;
				occupation(f, n);
		}else if (f.x==127){
				f.side=2;
				setcoosquare(f, -0.5, 0.5);
				grille.occupation (f);
				f.y=f.y-1;
				occupation(f, n);
		}else{
				grille.occupation (f);}}}

void Simulation::Bloc3(Collector& C, int sensa, int sensb, int vx, 
						int vy, int indice, bool nourri, Square f, 
						Square n, int count){
	if (sensa==-1 and sensb==-1 ){	
		set_coordo( C, f,  n, nourri, indice);
		for ( int i=1; i<=((abs(vx+vy)/2)); ++i){
			if (f.x<0 and f.y<0){
				setcoosquare(f, 1, 1);
			}else if (f.x<0){
				setcoosquare(f, 1, -1);
			}else if (f.y<0){
				setcoosquare(f, -1, 1);
			}else{	
				setcoosquare(f, -1, -1);}
			set_count(f, count);}
		for (int i=1; i<=((abs(vx-vy)/2)); i++){
			if (n.x >=C.get_carre().x){
				setcoosquare(f, 1, -1);
				set_count(f, count);
			}else {	
				setcoosquare(f, -1, -1);
				set_count(f, count);}}
	if (f.x==0 and f.y==0){
				f.side=2;
				setcoosquare(f, 0.5, 0.5);
				occupation(f, n);
	}else if (f.y==0){
				f.side=2;
				setcoosquare(f, -0.5, -0.5);
				grille.occupation (f);
				f.x=f.x+1;
				occupation(f, n);
	}else if (f.x==0){
				f.side=2;
				setcoosquare(f, 0.5, 0.5);
				grille.occupation (f);
				f.y=f.y-1;
				occupation(f, n);
	}else{
				grille.occupation (f);}}
}

void Simulation::Bloc4(Collector& C, int sensa, int sensb, int vx, 
						int vy, int indice, bool nourri, Square f, 
						Square n, int count){
	if (sensa==-1 and sensb==1 ){
		set_coordo( C, f,  n, nourri, indice);
		
		for ( int i=1; i<=((abs(vx+vy)/2)); ++i){
			if (f.x<2 and f.y>125){
				setcoosquare(f, 1, -1);
			}else if (f.x<2){
					setcoosquare(f, 1, 1);
			}else if (f.y>125){
					setcoosquare(f, -1, -1);
			}else{	
					setcoosquare(f, -1, 1);	}
			set_count(f, count);
		};	
		for (int i=1; i<=((abs(vx-vy)/2)); i++){
			if (n.x >C.get_carre().x){
				setcoosquare(f, 1, 1);
				set_count(f, count);
			}else {	
				if(C.get_etat()==EMPTY){			
				setcoosquare(f, -1, 1);}
				else{
					setcoosquare(f, 1, -1);}
				set_count(f, count);}}}						
}
						
						
int Simulation::nbr_dobstaclchem(Collector& C, int sensa, int sensb, int vx, int vy, int indice, bool nourri){
	int count(0);
	Square f, n;
	f.side=3;
	f.x=C.get_carre().x;
	f.y=C.get_carre().y;
	
	Bloc1(C, sensa, sensb, vx, vy, indice, nourri, f,  n,  count);
	Bloc2(C, sensa, sensb, vx, vy, indice, nourri, f,  n,  count);
	Bloc3(C, sensa, sensb, vx, vy, indice, nourri, f,  n,  count);
	Bloc4(C, sensa, sensb, vx, vy, indice, nourri, f,  n,  count);

	return count;
}
	
void Simulation::calculdessens(Collector& C, int vx, int vy, int sens1a, int sens1b,int sens2a, int sens2b, int Nx, int Ny,  int Cx,  int Cy){
		if (vx>=0){
		sens1a=1;
	}else{
		sens1a=-1;}
	if (vy>=0){
		sens1b=1;
	}else{
		sens1b=-1;}
	
	if(sens1a==1 and sens1b==1 and Nx>=Cx){
		sens2a=1;
		sens2b=-1;	
	}else if(sens1a==1 and sens1b==1 and Nx<Cx){
		sens2a=-1;
		sens2b=1;
	}else if(sens1a==1 and sens1b==1 and Nx==Cx){
		if(vy>vx){
		sens2a=-1;
		sens2b=1;
	}else{
			sens2a=1;
			sens2b=-1;}	
	}else if(sens1a==1 and sens1b==-1 and Nx>Cx){
			sens2a=1;
			sens2b=1;
	}else if(sens1a==1 and sens1b==-1 and Nx<Cx){
			sens2a=-1;
			sens2b=-1;
	}else if(sens1a==1 and sens1b==-1 and Nx==Cx){
			if(vy>-vx){
				sens2a=1;
				sens2b=1;}
				else{
				sens2a=-1;
				sens2b=-1;}	
	}else if(sens1a==-1 and sens1b==-1 and Nx>Cx){
			sens2a=1;
			sens2b=-1;
	}else if(sens1a==-1 and sens1b==-1 and Nx<Cx){
			if(C.get_etat()==LOADED){
				sens2a=+1;
				sens2b=-1;}
			else{
				sens2a=-1;
				sens2b=1;}
	}else if(sens1a==-1 and sens1b==-1 and Nx==Cx){
			if(vy>vx){
				sens2a=-1;
				sens2b=1;}
			else{
				sens2a=1;
				sens2b=-1;
			}	
	}else if(sens1a==-1 and sens1b==1 and Nx<Cx){
			if (C.get_etat()==EMPTY){
			sens2a=1;
			sens2b=1;
			}else{
				sens2a=-1;
				sens2b=-1;}
	}else if(sens1a==-1 and sens1b==1 and Nx>Cx){
			sens2a=1;
			sens2b=1;
	}else if(sens1a==-1 and sens1b==1 and Nx==Cx){
			if(vy>-vx){
				sens2a=1;
				sens2b=1;}
			else{
				sens2a=-1;
				sens2b=-1;}}
				
}
	
void Simulation::chem_alternatif(Collector& C, int i, int butx, int buty, bool nourri, int coll){
	int Cx=C.get_carre().x;
	int Cy=C.get_carre().y;
	int Nx= butx;
	int Ny= buty;
	int vx=Nx-Cx;
	int vy=Ny-Cy;
	int sens1a;
	int sens1b;
	
		if (vx>=0){
			sens1a=1;
		}else{
			sens1a=-1;
		}
	if (vy>=0){
			sens1b=1;
		}else{
			sens1b=-1;
		}
	int sens2a;
	int sens2b;
	
	
	if(sens1a==1 and sens1b==1 and Nx>=Cx){
			
			sens2a=1;
			sens2b=-1;
			
				
		
	}else if(sens1a==1 and sens1b==1 and Nx<Cx){
			sens2a=-1;
			sens2b=1;
	}else if(sens1a==1 and sens1b==1 and Nx==Cx){
			if(vy>vx){
				sens2a=-1;
				sens2b=1;}
				else{
				sens2a=1;
				sens2b=-1;
			}	
	}else if(sens1a==1 and sens1b==-1 and Nx>Cx){
			sens2a=1;
			sens2b=1;
	}else if(sens1a==1 and sens1b==-1 and Nx<Cx){
			sens2a=-1;
			sens2b=-1;
	}else if(sens1a==1 and sens1b==-1 and Nx==Cx){
			if(vy>-vx){
				sens2a=1;
				sens2b=1;}
				else{
				sens2a=-1;
				sens2b=-1;
			}	
	}else if(sens1a==-1 and sens1b==-1 and Nx>Cx){
			sens2a=1;
			sens2b=-1;
	}else if(sens1a==-1 and sens1b==-1 and Nx<Cx){
			if(C.get_etat()==LOADED){
				sens2a=+1;
			sens2b=-1;}
			else{
			sens2a=-1;
			sens2b=1;
		}
	}else if(sens1a==-1 and sens1b==-1 and Nx==Cx){
			if(vy>vx){
				sens2a=-1;
				sens2b=1;}
				else{
				sens2a=1;
				sens2b=-1;
			}	
	}else if(sens1a==-1 and sens1b==1 and Nx<Cx){
			if (C.get_etat()==EMPTY){
			sens2a=1;
			sens2b=1;
			}else{
				sens2a=-1;
				sens2b=-1;
			}
	}else if(sens1a==-1 and sens1b==1 and Nx>Cx){
			sens2a=1;
			sens2b=1;
			
	}else if(sens1a==-1 and sens1b==1 and Nx==Cx){
			if(vy>-vx){
				sens2a=1;
				sens2b=1;}
				else{
				sens2a=-1;
				sens2b=-1;
			}	
		}
	

		
	if (nbr_dobstaclchem(C,sens2a,sens2b,vx,vy,i,nourri)>=nbr_dobstaclchem(C,sens1a,sens1b,vx,vy,i, nourri)){
			
		if(sens1a ==1 and sens1b==1){
			if (Cx+1>124 and Cy+1>124){
				
				C.set_coo(C.get_carre().x-1,C.get_carre().y-1);
			}
			else if (Cx+1>124){
				
				C.set_coo(C.get_carre().x-1,C.get_carre().y+1);
			}else if (Cy>125){
				
				C.set_coo(C.get_carre().x+1,C.get_carre().y-1);
			}else{	
			
			C.set_coo(C.get_carre().x+1,C.get_carre().y+1);
			}
		}
		else if (sens1a ==-1 and sens1b==-1){
			// si le point de coordonnees (C.get_carre().x-1,C.get_carre().y-1) va sortir du monde alors deplacer selon le vecteur miroir selon si il touche en haut/bas ou a droit/gauche
			//sinon depelacer comme tel
		
			if (Cx-1<1 and Cy-1<1){
				C.set_coo(C.get_carre().x+1,C.get_carre().y+1);
			}
			else if (Cx-1<1){
				C.set_coo(C.get_carre().x+1,C.get_carre().y-1);
			}else if (Cy-1<1){
				C.set_coo(C.get_carre().x-1,C.get_carre().y+1);
			}else{	
			C.set_coo(C.get_carre().x-1,C.get_carre().y-1);
			}
		}
		else if (sens1a ==-1 and sens1b==1){
			if (Cx-1<1 and Cy+1>124){
				C.set_coo(C.get_carre().x+1,C.get_carre().y-1);
			}else if (Cx-1<1){
				C.set_coo(C.get_carre().x+1,C.get_carre().y+1);
			}else if (Cy+1>124){
				C.set_coo(C.get_carre().x-1,C.get_carre().y-1);
			}else{	
			C.set_coo(C.get_carre().x-1,C.get_carre().y+1);
			}
		}
		else if(sens1a ==1 and sens1b==-1){
			if (Cx+1>124 and Cy-1<1){
				C.set_coo(C.get_carre().x-1,C.get_carre().y+1);
			}else if (Cx+1>124){
				C.set_coo(C.get_carre().x-1,C.get_carre().y-1);
			}else if (Cy-1<1){
				C.set_coo(C.get_carre().x+1,C.get_carre().y+1);
			}else{	
			C.set_coo(C.get_carre().x+1,C.get_carre().y-1);
			}
		}
	}else {
	
		 
		if(sens2a ==-1 and sens2b==-1){
			if (Cx-1<1 and Cy-1<1){
				C.set_coo(C.get_carre().x+1,C.get_carre().y+1);
			}
			else if (Cx-1<1){
				C.set_coo(C.get_carre().x+1,C.get_carre().y-1);
			}else if (Cy-1<1){
				C.set_coo(C.get_carre().x-1,C.get_carre().y+1);
			}else{	
			C.set_coo(C.get_carre().x-1,C.get_carre().y-1);
			}
		}
		else if (sens2a ==1 and sens2b==1){
			if (Cx>126 and Cy>126){
				
				C.set_coo(C.get_carre().x-1,C.get_carre().y-1);
			}
			else if (Cx>126){
				
				C.set_coo(C.get_carre().x-1,C.get_carre().y+1);
			}else if (Cy>126){
				
				C.set_coo(C.get_carre().x+1,C.get_carre().y-1);
			}else{	
				
			C.set_coo(C.get_carre().x+1,C.get_carre().y+1);
			}
		}
		else if (sens2a ==1 and sens2b==-1){
			if (Cx+1>126 and Cy-1<1){
				C.set_coo(C.get_carre().x-1,C.get_carre().y+1);
			}else if (Cx+1>126){
				C.set_coo(C.get_carre().x-1,C.get_carre().y-1);
			}else if (Cy-1<1){
				C.set_coo(C.get_carre().x+1,C.get_carre().y+1);
			}else{	
			C.set_coo(C.get_carre().x+1,C.get_carre().y-1);
			}
		}
		else if(sens2a ==-1 and sens2b==1){
			if (Cx-1<1 and Cy+1>126){
				C.set_coo(C.get_carre().x+1,C.get_carre().y-1);
			}else if (Cx-1<1){
				C.set_coo(C.get_carre().x+1,C.get_carre().y+1);
			}else if (Cy+1>126){
				C.set_coo(C.get_carre().x-1,C.get_carre().y-1);
			}else{	
			C.set_coo(C.get_carre().x-1,C.get_carre().y+1);
			}
		}
	}
	
	
}	

void Simulation::manger_nourriture(Collector& C, int indice_nourri){
	
	if ((food_[indice_nourri].get_r().x>C.get_carre().x-1.5 and food_[indice_nourri].get_r().x<C.get_carre().x+1.5) 
		and (food_[indice_nourri].get_r().y>C.get_carre().y-1.5 and food_[indice_nourri].get_r().y<C.get_carre().y+1.5)){
		food_.erase(food_.begin()+indice_nourri);
		C.set_etat(LOADED);
	}
}



void Simulation::go_to(Collector& C, int where, int butx, int buty, int i_){
	if (where==0){
		int i(get_min(C));
		if (abs(butx - C.get_carre().x)==abs(buty - C.get_carre().y)){
			
			diagonale(C, i, butx, buty);
		}else{
			chem_alternatif(C, i, butx, buty, C.get_etat()==EMPTY, i_ );
		}
	}else if (where==1){
		go_back(C, i_);
	}
	
}

void Simulation::go_back(Collector& C, int i){
	
	int gen_x=mais_ant[C.get_hillid2()].get_g().x;
	int gen_y=mais_ant[C.get_hillid2()].get_g().y;
	bool k(false);
	if (abs(gen_x - C.get_carre().x)==abs(gen_y - C.get_carre().y)){
			diagonale(C, i, gen_x, gen_y);
		}else{
			chem_alternatif(C, i, gen_x, gen_y, k, i);
			
		}
}

void Simulation::ramener_nourriture(Collector& C){
	Square im;
	im.x=C.get_carre().x-1;
	im.y=C.get_carre().y-1;
	im.side=C.get_carre().side;
	if(im.x<mais_ant[C.get_hillid2()].get_F().x+mais_ant[C.get_hillid2()].get_F().side and im.x+im.side>mais_ant[C.get_hillid2()].get_F().x 
	   and im.y<mais_ant[C.get_hillid2()].get_F().y+mais_ant[C.get_hillid2()].get_F().side and im.y+im.side>mais_ant[C.get_hillid2()].get_F().y and C.get_etat()==LOADED){
		C.set_etat(EMPTY);
		mais_ant[C.get_hillid2()].set_nbr_food(-50);
	}
}


void Simulation::fairebougercoll(){
	for (unsigned int i=0; i<coo_collectors.size(); i++){
		if(coo_collectors[i].get_etat()==LOADED){
				go_to(coo_collectors[i], coo_collectors[i].get_etat()==LOADED, mais_ant[coo_collectors[i].get_hillid2()].get_g().x, mais_ant[coo_collectors[i].get_hillid2()].get_g().y, i);}
		if(coo_collectors[i].get_etat()==EMPTY and (n_atteignable(coo_collectors[i]).size()!=0)){
		go_to(coo_collectors[i], coo_collectors[i].get_etat(), n_atteignable(coo_collectors[i])[i].get_r().x, n_atteignable(coo_collectors[i])[i].get_r().y, i);
		manger_nourriture(coo_collectors[i], get_min(coo_collectors[i]));
		}else {
			if(coo_collectors[i].get_etat()==EMPTY and (n_atteignable(coo_collectors[i]).size()==0) and carre_centre_dans_non_centre(coo_collectors[i].get_carre(), mais_ant[coo_collectors[i].get_hillid2()].get_F())){
			if (coo_collectors[i].get_carre().x>=mais_ant[coo_collectors[i].get_hillid2()].get_g().x and coo_collectors[i].get_carre().y>=mais_ant[coo_collectors[i].get_hillid2()].get_g().y){
					coo_collectors[i].set_coo(coo_collectors[i].get_carre().x+1, coo_collectors[i].get_carre().y+1);
			}
			}else if(coo_collectors[i].get_etat()==EMPTY and (n_atteignable(coo_collectors[i]).size()==0) ){
				coo_collectors[i].set_coo(coo_collectors[i].get_carre().x, coo_collectors[i].get_carre().y);
			}else{
		go_to(coo_collectors[i], coo_collectors[i].get_etat()==LOADED, mais_ant[coo_collectors[i].get_hillid2()].get_g().x, mais_ant[coo_collectors[i].get_hillid2()].get_g().y, i);
		
		ramener_nourriture(coo_collectors[i]);
	}}}
}

void Simulation::liberation_generator(int i, string signe, string coo){
	grille.liberation(mais_ant[i].get_g());
	if(coo=="x"){
		mais_ant[i].set_gx(signe);
	}else{
		mais_ant[i].set_gy(signe);
	}	
}
		
void Simulation::deplacement_generator(){
	int nbF=mais_ant.size();
	for (int i; i<nbF; ++i){
		int x=mais_ant[i].get_F().x+mais_ant[i].get_F().side/2;
		int y=mais_ant[i].get_F().y+mais_ant[i].get_F().side/2;
		if(mais_ant[i].get_g().x<x){
			liberation_generator(i, "+", "x");
			if(grille.sup_grille1(mais_ant[i].get_g(), grille))
				mais_ant[i].set_gx("-");
		}else if(mais_ant[i].get_g().x>x){
			liberation_generator(i, "-", "x");
			if(grille.sup_grille1(mais_ant[i].get_g(), grille))
				mais_ant[i].set_gx("+");
		}else if(mais_ant[i].get_g().x==x and mais_ant[i].get_g().y<y){
			liberation_generator(i, "+", "y");
			if(grille.sup_grille1(mais_ant[i].get_g(), grille))
				mais_ant[i].set_gy("-");
		}else if(mais_ant[i].get_g().x==x and mais_ant[i].get_g().y>y){
			liberation_generator(i, "-", "y");
			if(grille.sup_grille1(mais_ant[i].get_g(), grille))
				mais_ant[i].set_gy("+");
		}else if(mais_ant[i].get_g().y<y){
			liberation_generator(i, "+", "y");
			if(grille.sup_grille1(mais_ant[i].get_g(), grille))
				mais_ant[i].set_gy("-");
		}else if(mais_ant[i].get_g().y>y){
			liberation_generator(i, "-", "y");
			if(grille.sup_grille1(mais_ant[i].get_g(), grille))
				mais_ant[i].set_gy("+");
		}
		grille.occupation(mais_ant[i].get_g());
	}
}	

void Simulation::deplacement_defensor(){
	int d1=0, d2=0, d3=0, d4=0, nbd=coo_defensor.size();
	int distance_minimale(0);
	for (int d(0); d<nbd; ++d){
		d1=coo_defensor[d].get_carre().x
		   -mais_ant[coo_defensor[d].get_hillid2()].get_F().x; 
		d2=mais_ant[coo_defensor[d].get_hillid2()].get_F().x
		   +mais_ant[coo_defensor[d].get_hillid2()].get_F().side-1
		   -coo_defensor[d].get_carre().x; //cote droit
		d3=coo_defensor[d].get_carre().y
		   -mais_ant[coo_defensor[d].get_hillid2()].get_F().y; //cote bas
		d4=mais_ant[coo_defensor[d].get_hillid2()].get_F().y
		   +mais_ant[coo_defensor[d].get_hillid2()].get_F().side-1
		   -coo_defensor[d].get_carre().y; //cote haut
		distance_minimale=min(min(min(d1, d2), d3), d4);
		if(not(d1==2) and not(d2==2) and not(d3==2) and not(d4==2)){
			if(distance_minimale==d1){
				grille.liberation(coo_defensor[d].get_carre());
				coo_defensor[d].set_x("-");
				if(grille.sup_grille1(coo_defensor[d].get_carre(),grille))
					coo_defensor[d].set_x("+"); 
			}else if(distance_minimale==d2){
				grille.liberation(coo_defensor[d].get_carre());
				coo_defensor[d].set_x("+");
				if(grille.sup_grille1(coo_defensor[d].get_carre(),grille))
					coo_defensor[d].set_x("-"); 	
		    }else if(distance_minimale==d3){
				grille.liberation(coo_defensor[d].get_carre());
				coo_defensor[d].set_y("-");
				if(grille.sup_grille1(coo_defensor[d].get_carre(),grille))
					coo_defensor[d].set_y("+"); 
		    }else if(distance_minimale==d4){
				grille.liberation(coo_defensor[d].get_carre());
				coo_defensor[d].set_y("+");
				if(grille.sup_grille1(coo_defensor[d].get_carre(),grille))
					  coo_defensor[d].set_y("-"); 
		   }grille.occupation(coo_defensor[d].get_carre());
		}
    }
    grille.nettoyergrille();
}

void Simulation::generer_food(){
	int nbF=mais_ant.size();
	int nbf=food_.size();
	int F(0);
	bernoulli_distribution b(food_rate);
	static default_random_engine e;
	uniform_int_distribution<unsigned> u(1,126);
	static default_random_engine r;
	if (b(e)){
		Nourriture n(u(r), u(r));
			for(int i(0); i<nbF; ++i){
				if(n.get_r().x>=mais_ant[i].get_F().x and n.get_r().x<=mais_ant[i].get_F().x+mais_ant[i].get_F().side-1 and n.get_r().y>=mais_ant[i].get_F().y and n.get_r().y<=mais_ant[i].get_F().y+mais_ant[i].get_F().side-1){
					++F;
				}
			}
			if(F==0 and not(grille.sup_grille1(n.get_r(), grille))){
				food_.push_back(n);
				grille.occupation(n.get_r());
	        }
    }
    for(int n(0); n<nbf; ++n){
		for(int t(0); t<nbF; ++t){
			if(food_[n].get_r().x>=mais_ant[t].get_F().x 
			   and food_[n].get_r().x<=mais_ant[t].get_F().x
			   +mais_ant[t].get_F().side-1 and food_[n].get_r().y>=
			   mais_ant[t].get_F().y and food_[n].get_r().y<=
			   mais_ant[t].get_F().y+mais_ant[t].get_F().side-1){
				swap(food_[n], food_[nbf-1]);
				food_.pop_back();
				grille.liberation(food_[nbf-1].get_r());
				--nbf;
			}
		}
	}
}

void Simulation::creation_fourmis_init(){
	int nbF=mais_ant.size();
	for(int i(0); i<nbF; ++i){
		creation_fourmis(i);
	}
}
	

void Simulation::creation_fourmis(int i){
	double nbf_fourmiliere(0);
	double prop_coll_fourmiliere(0), prop_def_fourmiliere(0);
	nbf_fourmiliere=getnbC(i)+getnbD(i)+getnbP(i);
	if(not(nbf_fourmiliere==0)){
		prop_coll_fourmiliere=getnbC(i)/nbf_fourmiliere;
		prop_def_fourmiliere=getnbD(i)/nbf_fourmiliere;
	}else{
		prop_coll_fourmiliere=0;
		prop_def_fourmiliere=0;
	}
	double a=mais_ant[i].get_nbr_food()*birth_rate;
	bernoulli_distribution b(min(1.,a));
	static default_random_engine w;
	if(b(w)){
		if(mais_ant[i].get_state()==FREE){
			if(prop_coll_fourmiliere<prop_free_collector){
				creer_collector(i);
				return;
			}else if(prop_def_fourmiliere<prop_free_defensor){
				creer_defensor(i);
				return;
			}else{
				creer_predator(i);
				return;
		    }
	    }
		if(mais_ant[i].get_state()==CONSTRAINED){
			if(prop_coll_fourmiliere<prop_constrained_collector){
				creer_collector(i);
				return;
			}else if(prop_def_fourmiliere<prop_constrained_defensor){
				creer_defensor(i);
				return;
			}else{
				creer_predator(i);
				return;
			}
		}
	}else{
		mais_ant[i].set_state(CONSTRAINED);
		return;
	}
	
}



void Simulation::deplacement_predator(){
	int nbP(coo_predator.size());
	for(int i(0); i<nbP; ++i){
		Square coo_cible;
		coo_cible=entiteplusproche(i,coo_predator[i].get_hillid2());
		int dx(coo_predator[i].get_carre().x-coo_cible.x);
		int dy(coo_predator[i].get_carre().y-coo_cible.y);
		grille.occupation(coo_predator[i].get_carre());
		if(dx>0 and dy>=0){
			if(dx>=dy and pas_collision_def(i, coo_cible, 1)){
				coo_predator[i].set_x("--"); 
				coo_predator[i].set_y("-");
			}else if(pas_collision_def(i, coo_cible, 2)){
				coo_predator[i].set_y("--");
				coo_predator[i].set_x("-"); 
			}
			grille.occupation(coo_predator[i].get_carre());}
		if(dx<0 and dy<=0){
			if(abs(dx)>=abs(dy) and pas_collision_def(i, coo_cible, 3)){
				coo_predator[i].set_x("++");
				coo_predator[i].set_y("+"); 
			}else if(pas_collision_def(i, coo_cible, 4)){
				coo_predator[i].set_y("++");
				coo_predator[i].set_x("+"); 
			}
			grille.occupation(coo_predator[i].get_carre());}
		if(dx<=0 and dy>0){
			if(abs(dx)>=dy and pas_collision_def(i, coo_cible, 5)){
				coo_predator[i].set_x("++");
				coo_predator[i].set_y("-"); 
			}else if(pas_collision_def(i, coo_cible, 6)){
				coo_predator[i].set_y("--");
				coo_predator[i].set_x("+");
			}
			grille.occupation(coo_predator[i].get_carre());}
		if(dx>=0 and dy<0){
			if(dx>=abs(dy) and pas_collision_def(i, coo_cible, 7)){
				coo_predator[i].set_x("--");
				coo_predator[i].set_y("+"); 
			}else if(pas_collision_def(i, coo_cible, 8)){
				coo_predator[i].set_y("++");
				coo_predator[i].set_x("-"); 
			}
	    grille.occupation(coo_predator[i].get_carre());}
		mort_cause_predator(i, coo_predator[i].get_hillid2());}	
}


Square Simulation::entiteplusproche(int i, int num){
	int nbP(coo_predator.size()), nbC(coo_collectors.size());
	vector<int> distancep; vector<int> distancec;
	int nbr_collector_cible(0), nbr_predator_cible(0);
	if(mais_ant[num].get_state()==FREE
	   and (coo_predator[i].get_carre().x<=mais_ant[num].get_F().x
	   or coo_predator[i].get_carre().y<=mais_ant[num].get_F().y
	   or coo_predator[i].get_carre().x>=mais_ant[num].get_F().x
	   +mais_ant[coo_predator[i].get_hillid2()].get_F().side-1
	   or coo_predator[i].get_carre().y>=mais_ant[num].get_F().y
	   +mais_ant[coo_predator[i].get_hillid2()].get_F().side-1)){   
			Square f;
			f.x=mais_ant[num].get_F().x+mais_ant[num].get_F().side/2;
			f.y=mais_ant[num].get_F().y+mais_ant[num].get_F().side/2;
			f.side=mais_ant[num].get_F().x;
            return f;
	}
	for(int p(0); p<nbP; ++p){
		if(coo_predator[p].get_hillid2()!=num){
			int distance(sqrt(pow(coo_predator[p].get_carre().x
			+coo_predator[i].get_carre().x,2)
			-pow(coo_predator[p].get_carre().y
			+coo_predator[i].get_carre().y,2)));
			distancep.push_back(distance);
			++nbr_predator_cible;
			if(mais_ant[num].get_state()==FREE and 
			((coo_predator[p].get_carre().x<=mais_ant[num].get_F().x
			or coo_predator[p].get_carre().y<=mais_ant[num].get_F().y
			or coo_predator[p].get_carre().x>=mais_ant[num].get_F().x
			+mais_ant[num].get_F().side-1
			or coo_predator[p].get_carre().y>=mais_ant[num].get_F().y
			+mais_ant[num].get_F().side-1))){   
			distancep[distancep.size()-1]=30000000;}
		}else{
			distancep.push_back(200000);
		}
	}
	for(int c(0); c<nbC; ++c){
		if(coo_collectors[c].get_hillid2()!=num){
			int distance(sqrt(pow(coo_collectors[c].get_carre().x
			-coo_predator[i].get_carre().x,2)
			+pow(coo_collectors[c].get_carre().y
			-coo_predator[i].get_carre().y,2)));
			distancec.push_back(distance);
			++nbr_collector_cible;
			if(mais_ant[num].get_state()==FREE and 
			((coo_collectors[c].get_carre().x+1<=mais_ant[num].get_F().x
			or coo_collectors[c].get_carre().y+1<=mais_ant[num].get_F().y
			or coo_collectors[c].get_carre().x-1>=mais_ant[num].get_F().x
			+mais_ant[num].get_F().side-1
			or coo_collectors[c].get_carre().y-1>=mais_ant[num].get_F().y
			+mais_ant[num].get_F().side-1))){  
					distancec[distancec.size()-1]=30000000;
		    }
		}else distancec.push_back(200000);}
	if(nbr_collector_cible+nbr_predator_cible==0
	and (coo_predator[i].get_carre().x<=mais_ant[num].get_F().x
	    or coo_predator[i].get_carre().y<=mais_ant[num].get_F().y
	    or coo_predator[i].get_carre().x>=mais_ant[num].get_F().x
	    +mais_ant[coo_predator[i].get_hillid2()].get_F().side-1
	    or coo_predator[i].get_carre().y>=mais_ant[num].get_F().y
	    +mais_ant[coo_predator[i].get_hillid2()].get_F().side-1)){   
			Square f;
			f.x=mais_ant[num].get_F().x+mais_ant[num].get_F().side/2;
			f.y=mais_ant[num].get_F().y+mais_ant[num].get_F().side/2;
			f.side=mais_ant[num].get_F().x;
			return f;
	}else if(nbr_collector_cible+nbr_predator_cible==0){
		return coo_predator[i].get_carre();
	}
	int mindistancec(getmin(distancec)),mindistancep(getmin(distancep));
	if(distancec[mindistancec]<distancep[mindistancep]){
		return coo_collectors[mindistancec].get_carre();
    }else{
		return coo_predator[mindistancep].get_carre();
    }
}

    
int Simulation::getmin(vector<int> tab){
	int min = 0;
    for (unsigned int i=1;i<tab.size();i++)
    {
        if(tab[i] < tab[min])
        {
           min = i;
        }
    }
    return min;
}


void Simulation::mort_cause_predator(int i, int num){
	int nbC=coo_collectors.size(), nbP=coo_predator.size();
	int xp=coo_predator[i].get_carre().x;
	int yp=coo_predator[i].get_carre().y;
	for(int c(0); c<nbC; ++c){
		if(coo_collectors[c].get_hillid2()!=num){
			int cx=coo_collectors[c].get_carre().x;
			int cy=coo_collectors[c].get_carre().y;
			if(xp>=cx-2 and yp>=cy-2 and xp<=cx+2 and yp<=cy+2){
				swap(coo_collectors[c], coo_collectors[nbC-1]);
				grille.liberation(coo_collectors[nbC-1].get_carre());
				coo_collectors.pop_back();
				setnbC(coo_collectors[c].get_hillid2(), "-");
				--nbC;
				if(coo_collectors[c].get_etat()==LOADED){
					creation_nourriture(coo_collectors[c].get_carre());
				}
				break;
			}
		}
	}
	for(int p(0); p<nbP; ++p){
		if(coo_predator[p].get_hillid2()!=num){
			int px=coo_predator[p].get_carre().x;
			int py=coo_predator[p].get_carre().y;
			if(xp>=px-1 and yp>=py-1 and xp<=px+1 and yp<=py+1){
				swap(coo_predator[p], coo_predator[nbP-1]);
				grille.liberation(coo_predator[nbP-1].get_carre());
				coo_predator.pop_back();
				setnbP(coo_predator[p].get_hillid2(), "-");
				swap(coo_predator[i], coo_predator[nbP-1]);
				grille.liberation(coo_predator[nbP-1].get_carre());
				coo_predator.pop_back();
				setnbP(coo_predator[p].get_hillid2(), "-");
				--nbP;
				break;
			}
		}
	}
}
	
bool Simulation::pas_collision_def(int i, Square cible, int a){
	grille.liberation(cible);
	Square cible_avant; cible_avant.x=cible.x; cible_avant.y=cible.y;
	if(a==1){
		cible.x=cible.x-2; cible.x=cible.y-1;
		if(grille.sup_grille1(cible, grille)){
			grille.occupation(cible_avant);
			return false;
		}
	}
	if(a==2){
		cible.x=cible.x-1; cible.x=cible.y-2;
		if(grille.sup_grille1(cible, grille)){
			grille.occupation(cible_avant);
			return false;
		}
	}
	if(a==3){
		cible.x=cible.x+2; cible.x=cible.y+1;
		if(grille.sup_grille1(cible, grille)){
			grille.occupation(cible_avant);
			return false;
		}
	}
	if(a==4){
		cible.x=cible.x+1; cible.x=cible.y+2;
		if(grille.sup_grille1(cible, grille)){
			grille.occupation(cible_avant);
			return false;
		}
	}
	if(a==5){
		cible.x=cible.x+2; cible.x=cible.y-1;
		if(grille.sup_grille1(cible, grille)){
			grille.occupation(cible_avant);
			return false;
		}
	}
	if(a==6){
		cible.x=cible.x+1; cible.x=cible.y-2;
		if(grille.sup_grille1(cible, grille)){
			grille.occupation(cible_avant);
			return false;
		}
	}
	if(a==7){
		cible.x=cible.x-2; cible.x=cible.y+1;
		if(grille.sup_grille1(cible, grille)){
			grille.occupation(cible_avant);
			return false;
		}
	}
	if(a==8){
		cible.x=cible.x-1; cible.x=cible.y+2;
		if(grille.sup_grille1(cible, grille)){
			grille.occupation(cible_avant);
			return false;
		}
	}
	return true;
}
	
//etat free si devient petite

void Simulation::kill_all_fourmiliere(int m,int& nbc,int& nbd,int& nbp){
	for (int d(0); d<nbd; ++d){
		if(coo_defensor[d].get_hillid2()==m){
			swap(coo_defensor[d], coo_defensor[nbc-1]);
			coo_defensor.pop_back();
			}
		}
		for (int c(0); c<nbc; ++c){
			if(coo_collectors[c].get_hillid2()==m){
				swap(coo_collectors[c], coo_collectors[nbc-1]);
				coo_collectors.pop_back();
			}
		}
		for (int p(0); p<nbp; ++p){
			if(coo_predator[p].get_hillid2()==m){
				swap(coo_predator[p], coo_predator[nbp-1]);
				coo_predator.pop_back();
			}
		}
}
	
void Simulation::creer_collector(int i){
	int size_avant(0);
	int size_apres(0);
	int xF=mais_ant[i].get_F().x;
	int yF=mais_ant[i].get_F().y;
	for(int r(xF+mais_ant[i].get_F().side-3); r>xF+1; --r){
		for(int j(yF+2); j<yF+mais_ant[i].get_F().side-2; ++j){
			Collector c(r,j, 0, "false", i);
			if(not(grille.sup_grille1(c.get_carre(), grille))){	
				size_avant=mais_ant[i].get_F().side;	
				setnbC(i,"+");
				size_apres=int(sqrt(4*(sizeG*sizeG + sizeC*sizeC*getnbC(i) + sizeD*sizeD*getnbD(i) + sizeP*sizeP*getnbP(i))));
				for(int s(1); s<=size_apres-size_avant; ++s){
					getbig(i);
				}
				coo_collectors.push_back(c);
				grille.occupation(c.get_carre());
				return;
			}
		}
	} 
}
	   


void Simulation::creer_defensor(int i){
	int size_avant(0);
	int size_apres(0);
	int xF=mais_ant[i].get_F().x;
	int yF=mais_ant[i].get_F().y;
	for(int r(xF+mais_ant[i].get_F().side-3); r>xF+1; --r){
		for(int j(yF+2); j<yF+mais_ant[i].get_F().side-2; ++j){
			Defensor d(r,j, 0, i);
			if(not(grille.sup_grille1(d.get_carre(), grille))){				   
				size_avant=mais_ant[i].get_F().side;			   
				setnbD(i,"+");
				size_apres=int(sqrt(4*(sizeG*sizeG + sizeC*sizeC*getnbC(i) + sizeD*sizeD*getnbD(i) + sizeP*sizeP*getnbP(i))));
				for(int s(1); s<=size_apres-size_avant; ++s){
					getbig(i);
				}
				coo_defensor.push_back(d);
				grille.occupation(d.get_carre());
				return;
			}
		}
	} 
}

void Simulation::creer_predator(int i){
	int size_avant(0);
	int size_apres(0);
	int xF=mais_ant[i].get_F().x;
	int yF=mais_ant[i].get_F().y;
	for(int r(xF+mais_ant[i].get_F().side-3); r>xF+1; --r){
		for(int j(yF+2); j<yF+mais_ant[i].get_F().side-2; ++j){
			Predator p(r,j, 0, i);
			if(not(grille.sup_grille1(p.get_carre(), grille))){				   
				size_avant=mais_ant[i].get_F().side;			   
				setnbP(i,"+");
				size_apres=int(sqrt(4*(sizeG*sizeG + sizeC*sizeC*getnbC(i) + sizeD*sizeD*getnbD(i) + sizeP*sizeP*getnbP(i))));
				for(int s(1); s<=size_apres-size_avant; ++s){
					getbig(i);
				}
				coo_predator.push_back(p);
				grille.occupation(p.get_carre());
				return;
			}
		}
	} 
} 
	  
void Simulation::changer_etat_vie_collector(int i){
	coo_collectors[i].set_end_of_life(true);
}

void Simulation::changer_etat_vie_defensor(int i){
	coo_defensor[i].set_end_of_life(true);
}

void Simulation::changer_etat_vie_predator(int i){
	coo_predator[i].set_end_of_life(true);
}
	
void Simulation::end_of_life_collector(int c, int &nbc){
	if (coo_collectors[c].get_end_of_life()==true){
		int t=coo_collectors[c].get_hillid2();
		int nbC=mais_ant[t].get_nbr_collector();
		int nbD=mais_ant[t].get_nbr_defensor();
		int nbP=mais_ant[t].get_nbr_predator();
		if(coo_collectors[c].get_etat()==LOADED){
			creation_nourriture(coo_collectors[c].get_carre());
		}
		grille.liberation(coo_collectors[c].get_carre());
		swap(coo_collectors[c], coo_collectors[nbc-1]);
		coo_collectors.pop_back();
		setnbC(t, "-");
		--nbc;--nbC;
		int s=sizeG*sizeG;
		int w=sizeC*sizeC*nbC;
		int o=sizeD*sizeD*nbD;
		int u=sizeP*sizeP*nbP;
		mais_ant[t].set_side(int(sqrt(4*(s+w+o+u))));
		mais_ant[t].set_state(FREE);
	}
}


void Simulation::end_of_life_defensor(int d, int &nbd){
	if (coo_defensor[d].get_end_of_life()==true){
		int t=coo_defensor[d].get_hillid2();
		int nbC=mais_ant[t].get_nbr_collector();
		int nbD=mais_ant[t].get_nbr_defensor();
		int nbP=mais_ant[t].get_nbr_predator();
		grille.liberation(coo_defensor[d].get_carre());
		swap(coo_defensor[d], coo_defensor[nbd-1]);
		coo_defensor.pop_back();
		setnbD(t, "-");
		--nbd;--nbD;
		int s=sizeG*sizeG;
		int w=sizeC*sizeC*nbC;
		int o=sizeD*sizeD*nbD;
		int u=sizeP*sizeP*nbP;
		mais_ant[t].set_side(int(sqrt(4*(s+w+o+u))));
		mais_ant[t].set_state(FREE);
	}
}


void Simulation::end_of_life_predator(int p, int &nbp){
	if (coo_predator[p].get_end_of_life()==true){
		int t=coo_predator[p].get_hillid2();
		int nbC=mais_ant[t].get_nbr_collector();
		int nbD=mais_ant[t].get_nbr_defensor();
		int nbP=mais_ant[t].get_nbr_predator();
		grille.liberation(coo_predator[p].get_carre());
		swap(coo_predator[p], coo_predator[nbp-1]);
		coo_predator.pop_back();
		setnbP(t, "-");
		--nbp;--nbP;
		int s=sizeG*sizeG;
		int w=sizeC*sizeC*nbC;
		int o=sizeD*sizeD*nbD;
		int u=sizeP*sizeP*nbP;
		mais_ant[t].set_side(int(sqrt(4*(s+w+o+u))));
		mais_ant[t].set_state(FREE);
	}
}

void Simulation::contact_fourmi(){
	int nbd(coo_defensor.size());
	int nbc(coo_collectors.size()); 
	for(int c(0); c<nbc; ++c){
		for(int d(0); c<nbd; ++d){
			Square coll;
			Square def;
			coll.x=coo_collectors[c].get_carre().x-1.5;
			coll.y=coo_collectors[c].get_carre().y-1.5;
			coll.side=coo_collectors[c].get_carre().side;
			def.x=coo_defensor[d].get_carre().x-1.5;
			def.y=coo_defensor[d].get_carre().y-1.5;
			def.side=coo_defensor[d].get_carre().side;
			if(sup_carree(coll, def)){
	            coo_defensor[d].set_end_of_life(true);
	            coo_collectors[c].set_end_of_life(true);
				end_of_life_collector(c, nbc);
				end_of_life_defensor(d, nbd);
			}
		}
	}
}

void Simulation::creation_nourriture(Square c){
	food_.push_back(Nourriture(c.x, c.y));
}
				

