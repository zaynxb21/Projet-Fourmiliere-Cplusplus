# fichier : Makefile
# membres du groupe : TLIGUI YASMINE et HAJROUN ZAYNAB
# Yasmine : 00% de la mise au point des fonctions / methodes de ce fichier
# Zaynab : 100% de la mise au point des fonctions / methodes de ce fichier


CXX = g++    
CXXFLAGS = -std=c++11 # C++11, ou autre suivant vos préférences 
OUT = projet #variable qui contient le nom de l'executable
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
OFILES =  projet.o simulation.o nourriture.o fourmiliere.o \
		  squarecell.o message.o error_squarecell.o gui.o \
		  graphic.o fourmis.o

CXXFLAGS += -pedantic -Wall    

 
all : projet

projet.o : projet.cc simulation.h gui.h
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
simulation.o : simulation.cc simulation.h squarecell.h
		$(CXX) $(CXXFLAGS) -c $< -o $@
	
nourriture.o : nourriture.cc nourriture.h squarecell.h message.h \
			   error_squarecell.h
		$(CXX) $(CXXFLAGS) -c $< -o $@
		
fourmiliere.o : fourmiliere.cc fourmiliere.h squarecell.h constantes.h \
				message.h error_squarecell.h
		$(CXX) $(CXXFLAGS) -c $< -o $@
		
fourmis.o: fourmis.cc message.h error_squarecell.h constantes.h \
		   squarecell.h
		$(CXX) $(CXXFLAGS) -c $< -o $@
		
squarecell.o : squarecell.cc squarecell.h graphic.h color.h
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
		
message.o : message.cc message.h
		$(CXX) $(CXXFLAGS) -c $< -o $@
		
error_squarecell.o : error_squarecell.cc error_squarecell.h
		$(CXX) $(CXXFLAGS) -c $< -o $@
		
gui.o : gui.cc gui.h simulation.h \
				  squarecell.h graphic.h color.h
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
		
graphic.o : graphic.cc graphic.h color.h
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

$(OUT) : $(OFILES)
		$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

clean : 
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLES *** "
	@/bin/rm -f *.o *.x *.cc~ *.h~ $(OUT)
