# Definitions de macros

OUT = projet
CXX     = g++
CXXFLAGS = -Wall -std=c++11
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
CXXFILES = projet.cc squarecell.cc anthill.cc food.cc error_squarecell.cc message.cc ant.cc simulation.cc gui.cc graphic.cc
OFILES = $(CXXFILES:.cc=.o)

# Definition de la premiere regle

all: $(OUT)

projet.o: projet.cc simulation.h message.h squarecell.h gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

squarecell.o: squarecell.cc squarecell.h error_squarecell.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

anthill.o: anthill.cc anthill.h ant.h constantes.h squarecell.h message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

food.o: food.cc food.h squarecell.h message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

error_squarecell.o: error_squarecell.cc error_squarecell.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

ant.o: ant.cc squarecell.h message.h ant.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

simulation.o: simulation.cc anthill.h ant.h constantes.h food.h \
 squarecell.h message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
gui.o: gui.cc gui.h simulation.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

graphic.o: graphic.cc graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

# Definitions de cibles particulieres
clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ prog
