// fichier : projet.cc
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB
// contibution au fichier 
// Yasmine : 50% de la mise au point des fonctions / methodes de ce fichier
// Zaynab : 50% de la mise au point des fonctions / methodes de ce fichier

#include "simulation.h"
#include "gui.h"
#include <gtkmm.h>
#include <iostream>
#include <string>
#include <vector>
    
using namespace std; 
int main(int argc, char *argv[]){
	Simulation simulation;
	setSimPointer(&simulation);
	int fakeArgc = 1;
    auto app = Gtk::Application::create(fakeArgc, argv, 
										"org.gtkmm.example");
    if(argc == 2) simulation.lecture(argv[1]);
    Gui buttons;
    return app->run(buttons);
} 
