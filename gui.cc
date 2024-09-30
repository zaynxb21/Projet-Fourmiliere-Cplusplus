// fichier : gui.cc
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB
// contibution au fichier 
// Yasmine : 50% de la mise au point des fonctions / methodes de ce fichier
// Zaynab : 50% de la mise au point des fonctions / methodes de ce fichier

#include "gui.h"
#include "simulation.h"
#include "squarecell.h"
#include "graphic.h"
#include "constantes.h"
#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <thread>

using namespace std;
static Simulation* simul;

void setSimPointer(Simulation* simulation) {
	simul = simulation;
}

static unsigned count1(0);

Gui::Gui():
	started(false),
	mBox(Gtk::ORIENTATION_HORIZONTAL, 10), 
	mBoxGeneral(Gtk::ORIENTATION_VERTICAL, 5), 
    mBoxInfo(Gtk::ORIENTATION_VERTICAL, 5),
    mBoxAnthill(Gtk::ORIENTATION_VERTICAL, 5),
	mBoxRight(Gtk::ORIENTATION_VERTICAL, 5), 
	mBoxLeft(Gtk::ORIENTATION_VERTICAL, 5),
    mFrameGeneral("General"), mFrameInfo("Info"), 
    mFrameAnthill("Anthill Info"), mButtonExit("Exit"), 
    mButtonStart("Start"), mButtonOpen("Open"), mButtonSave("Save"), 
    mButtonStep("Step"), mButtonPrevious("Previous"), 
    mButtonNext("Next")
   
    {
    m_Area.set_size_request(500,500);
    setupGui();
    initGeneral();
    initInfo();
    initAnthill();
    connectButtons();
    mBox.pack_start(mBoxLeft);
	mBox.pack_start(mBoxRight);
    mBoxRight.pack_start(m_Area);
    m_Area.draw();
	Glib::signal_idle().connect( sigc::mem_fun(*this, &Gui::on_idle) );
    add_events(Gdk::KEY_RELEASE_MASK);
    show_all_children();
}




void Gui::simulation(){
	if(count1==1){
		simul->set_true_size();
	}
	simul->generer_food();
	simul->fairebougercoll();
	simul->creation_fourmis_init();
	simul->gerer_vies();
	simul->changer_total_food();
	simul->deplacement_predator();
	simul->deplacement_generator();
	simul->deplacement_defensor();
	mLabelNbFood.set_text(string("Nb food: ")+
							     to_string(simul->getfoodsize()));
	if (numfourmiliere!=-1){
		mLabelTotalFood.set_text(string("Total food: ")+
								 to_string(simul->
								 getsizefood(numfourmiliere)));
		mLabelnbC.set_text(string("nbC: ")+
						   to_string(simul->getnbC(numfourmiliere)));
		mLabelnbD.set_text(string("nbD: ")+
						   to_string(simul->getnbD(numfourmiliere)));
		mLabelnbP.set_text(string("nbP: ")+
						   to_string(simul->getnbP(numfourmiliere)));
	}
	m_Area.draw();
}
	
bool Gui::on_idle(){
	if(started){
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		simulation();
		cout<<"Mise à jour de la simulation numéro : "<<++count1<<endl;
		mButtonStart.set_label("Stop");
		return true;
	}else{
		mButtonStart.set_label("Start");
		return true;
	}  
}

bool Gui::on_idle1(){
	cout<<"Mise à jour de la simulation numéro : "<<++count1<<endl;  
	simulation(); 
	return true;  
}

Gui::~Gui() {}



void Gui::onButtonClickExit() {
    exit(0); 
}

void Gui::onButtonClickStart() {
	started = !started ;
}

void Gui::onButtonClickOpen() {
    Gtk::FileChooserDialog dialog("Please choose a file",
								  Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this); 
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);
    int result = dialog.run();
    switch(result) {
		case(Gtk::RESPONSE_OK):{ 
			simul->reset_entites();
			resetInfos();
            string filename = dialog.get_filename();
            char* filenameChar = const_cast<char*>(filename.c_str());
            if(simul->lecture(filenameChar)){
				mLabelNbFood.set_text(string("Nb food: ")+
									  to_string(simul->getfoodsize()));
				m_Area.draw();
			}else{
				m_Area.clear();
			}
            break;
		}
        case(Gtk::RESPONSE_CANCEL): 
			break;
        default: 
            cout << "Pas ouverture" << endl;
            break;
	}
}

void Gui::resetInfos(){
	mLabelNbFood.set_text(string("Nb food: 0"));
    mLabelId.set_text(string("None selected"));
	mLabelTotalFood.set_text(string(""));
	mLabelnbC.set_text(string(""));
	mLabelnbD.set_text(string(""));
	mLabelnbP.set_text(string(""));
	numfourmiliere=-1;
	count1=0;
}


void Gui::onButtonClickSave(){
    Gtk::FileChooserDialog dialog("Please choose a file",
								  Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Save", Gtk::RESPONSE_OK);
    int result = dialog.run();
    switch(result) {
        case(Gtk::RESPONSE_OK):{ 
			string filename = dialog.get_filename();
            simul->save_grille(filename);
            break;
        }
        case(Gtk::RESPONSE_CANCEL): 
            break;
        default: 
            cout << "Pas de save" << endl;
            break;
    }
}

void Gui::onButtonClickStep(){
	on_idle1();
}

void Gui::buttonsPreviousNext(string type) {
	if(type=="previous") {
		--numfourmiliere;
	}else{
		++numfourmiliere;
	}
	if (numfourmiliere==-2){
		numfourmiliere=simul->getsizeant()-1;
	}
	if(numfourmiliere==simul->getsizeant()){
		numfourmiliere=-1;
	}	
    if (numfourmiliere==-1){
		mLabelId.set_text(string("None selected"));
		mLabelTotalFood.set_text(string(""));
		mLabelnbC.set_text(string(""));
		mLabelnbD.set_text(string(""));
		mLabelnbP.set_text(string(""));
	}
	if (numfourmiliere!=-1 and numfourmiliere<simul->getsizeant()){
		mLabelId.set_text(string("id: ")+ to_string(numfourmiliere));
		mLabelTotalFood.set_text(string("Total food: ")+
								 to_string(simul->
								 getsizefood(numfourmiliere)));
		mLabelnbC.set_text(string("nbC: ")+
						   to_string(simul->getnbC(numfourmiliere)));
		mLabelnbD.set_text(string("nbD: ")+
						   to_string(simul->getnbD(numfourmiliere)));
		mLabelnbP.set_text(string("nbP: ")+
						   to_string(simul->getnbP(numfourmiliere)));
	}
}
	
void Gui::onButtonClickPrevious(){
	buttonsPreviousNext("previous");
}

void Gui::onButtonClickNext(){
	buttonsPreviousNext("next");
}

void Gui::setupGui(){
	set_title("Tchanz");
    set_border_width(10);
    set_position(Gtk::WIN_POS_CENTER);
    add(mBox);
}

void Gui::initGeneral(){
    mBoxLeft.pack_start(mFrameGeneral, false, false);
    mFrameGeneral.add(mBoxGeneral);
    mBoxGeneral.pack_start(mButtonExit, false, false);
    mBoxGeneral.pack_start(mButtonOpen, false, false);
    mBoxGeneral.pack_start(mButtonSave, false, false);
    mBoxGeneral.pack_start(mButtonStart, false, false);
    mBoxGeneral.pack_start(mButtonStep, false, false);
}

void Gui::initInfo() {
	mBoxLeft.pack_start(mFrameInfo, false, false);
    mFrameInfo.add(mBoxInfo);
    mLabelNbFood.set_text(string("Nb food: 0"));
    mBoxInfo.pack_start(mLabelNbFood);
} 

void Gui::initAnthill() {
    mBoxLeft.pack_start(mFrameAnthill, false, false);
    mFrameAnthill.add(mBoxAnthill);
    mBoxAnthill.pack_start(mButtonPrevious, false, false);
    mButtonPrevious.set_size_request(220, 30);
    mBoxAnthill.pack_start(mButtonNext, false, false);
    mLabelId.set_text(string("None selected"));
    mBoxAnthill.pack_start(mLabelId);
    mLabelTotalFood.set_text(string(""));
    mBoxAnthill.pack_start(mLabelTotalFood);
    mLabelnbC.set_text(string(""));
    mBoxAnthill.pack_start(mLabelnbC);
    mLabelnbD.set_text(string(""));
    mBoxAnthill.pack_start(mLabelnbD);
    mLabelnbP.set_text(string(""));
    mBoxAnthill.pack_start(mLabelnbP);
	
} 

void Gui::connectButtons() {
	mButtonExit.signal_clicked().connect(sigc::mem_fun(*this,
                                         &Gui::onButtonClickExit));
    mButtonStart.signal_clicked().connect(sigc::mem_fun(*this,
                                          &Gui::onButtonClickStart));
    mButtonOpen.signal_clicked().connect(sigc::mem_fun(*this,
                                         &Gui::onButtonClickOpen));
    mButtonSave.signal_clicked().connect(sigc::mem_fun(*this,
                                         &Gui::onButtonClickSave));
    mButtonStep.signal_clicked().connect(sigc::mem_fun(*this,
                                         &Gui::onButtonClickStep));
    mButtonPrevious.signal_clicked().connect(sigc::mem_fun
					(*this, &Gui::onButtonClickPrevious));
    mButtonNext.signal_clicked().connect(sigc::mem_fun(*this,
                                         &Gui::onButtonClickNext));

    
} 

bool Gui::on_key_press_event(GdkEventKey * key_event){
	if(key_event->type == GDK_KEY_PRESS){
		switch(gdk_keyval_to_unicode(key_event->keyval)){
			case 'q':
				exit(0);
				return true;
			case 'p':
				buttonsPreviousNext("previous");
				return true;
			case 'n':
				buttonsPreviousNext("next");
				return true;
			case '1':
				on_idle1();
				return true;
			case 's':
				started = !started ;
				return true;
		}
	}
	return Gtk::Window::on_key_press_event(key_event);
}

static Frame default_frame = {0., g_max, 0., g_max, 1., 500, 
							  500}; 

static void ortho_proj(const Cairo::RefPtr<Cairo::Context>& cr,  
					   Frame frame);
									
MyArea::MyArea(): empty(false){
}

MyArea::~MyArea(){
}

void MyArea::clear(){
	empty = true; 
	refresh();
}

void MyArea::draw(){
	empty = false;
	refresh();
}

void MyArea::refresh(){
	auto win = get_window();
	if(win){
		Gdk::Rectangle r(0,0, get_allocation().get_width(), 
						 get_allocation().get_height());
		win->invalidate_rect(r,false);
	}
}


void MyArea::setFrame(Frame f){
	if((f.xMin <= f.xMax) and (f.yMin <= f.yMax) and (f.height > 0)){
		f.asp = f.width/f.height;
		frame = f;
	}else{
		cout << "Mauvais modele" << endl;
	}
} 

void MyArea::resetFrame() {
	frame.xMin = 0;
    frame.xMax = gmax;
    frame.yMin = 0;
    frame.yMax = gmax;
}

void MyArea::adjustFrame(){
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
	frame.width  = width;
	frame.height = height;
    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > default_frame.asp){
		frame.yMax = default_frame.yMax;
	    frame.yMin = default_frame.yMin;	
	    double delta(default_frame.xMax - default_frame.xMin);
	    double mid((default_frame.xMax + default_frame.xMin)/2);
	    frame.xMax = mid+0.5*(new_aspect_ratio/default_frame.asp)*delta;
	    frame.xMin = mid-0.5*(new_aspect_ratio/default_frame.asp)*delta;		  	  
    }else{ 
	    frame.xMax = default_frame.xMax ;
	    frame.xMin = default_frame.xMin ;
 	    double delta(default_frame.yMax - default_frame.yMin);
	    double mid((default_frame.yMax + default_frame.yMin)/2);
	    frame.yMax = mid+0.5*(default_frame.asp/new_aspect_ratio)*delta;
	    frame.yMin = mid-0.5*(default_frame.asp/new_aspect_ratio)*delta;		  	  
    }
}

static void ortho_proj(const Cairo::RefPtr<Cairo::Context>& cr, 
					   Frame frame){
	cr->translate(frame.width/2, frame.height/2);
	cr->scale(frame.width/(frame.xMax - frame.xMin), 
              -frame.height/(frame.yMax - frame.yMin));
	cr->translate(-(frame.xMin + frame.xMax)/2, 
				  -(frame.yMin + frame.yMax)/2);
}


void MyArea::draw_frame(const Cairo::RefPtr<Cairo::Context>& cr){
	cr->set_line_width(0.01);
	cr->set_source_rgb(0, 0, 0);
	cr->rectangle(0.5,0.5,get_allocation().get_width()-0.5,
				  get_allocation().get_height()-0.5);
	cr->fill_preserve();
	cr->save();
	cr->stroke();
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
	draw_frame(cr); 
	if(not empty){
		adjustFrame();
		ortho_proj(cr, frame);
		SetContext(cr);
		tracerlignesgrille(frame.xMax, frame.yMax, GREY);
		tracerbords();
		simul->dessinerentites();
	}else{
		adjustFrame();
		ortho_proj(cr, frame);
		SetContext(cr);
		tracerlignesgrille(frame.xMax, frame.yMax, GREY);
		tracerbords();
	}
	return true;
}






