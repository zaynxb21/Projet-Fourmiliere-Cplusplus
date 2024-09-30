// fichier : gui.h
// membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB

#ifndef LAYOUTBUTTONS_HEADER_H
#define LAYOUTBUTTONS_HEADER_H

#include "simulation.h"

#include <gtkmm.h>


struct Frame 
{
	double xMin;
	double xMax;
	double yMin;
	double yMax;
	double asp;  
	int height;  
	int width;  
};

class MyArea : public Gtk::DrawingArea
{
	
private:

	Frame frame;
	bool empty;
	void refresh();
	
protected:

	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
	void draw_frame(const Cairo::RefPtr<Cairo::Context>& cr);
	
public:

	MyArea();
	virtual ~MyArea();
	void clear();
	void draw();
	void setFrame(Frame x); 
	void resetFrame();
	void adjustFrame();
	
};

class Gui: public Gtk::Window {
	
private:
	bool started;
	int numfourmiliere=-1;
	
protected:

	Gtk::Box mBox, mBoxGeneral, mBoxInfo, mBoxAnthill,
			 mBoxRight, mBoxLeft;

    Gtk::Frame mFrameGeneral, mFrameInfo, mFrameAnthill;
	MyArea  m_Area;
    Gtk::Button mButtonExit, mButtonStart, mButtonOpen, mButtonSave, 
				mButtonStep, mButtonPrevious, mButtonNext;

    Gtk::Label mLabelNbFood, mLabelId, mLabelTotalFood, 
		       mLabelnbC, mLabelnbD, mLabelnbP;
    
    void onButtonClickExit();
    void onButtonClickStart();
    void onButtonClickSave();
    void onButtonClickOpen();
    void onButtonClickPrevious();
    void onButtonClickNext();
    void onButtonClickStep();
    bool on_idle();
    bool on_idle1();
    void setupGui();
    void initGeneral();
    void initInfo();
    void initAnthill();
    void connectButtons();
    void buttonsPreviousNext(std::string type);
	void resetInfos();
	void simulation();
    bool on_key_press_event(GdkEventKey* keyEvent) override;
    
	
	
public:

	Gui();
    virtual ~Gui();
    void afficherdessin();
    
};

void setSimPointer(Simulation* sim);





#endif  
