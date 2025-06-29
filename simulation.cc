// Emilien Coudurier et Aksel Acar

// Emilien Coudurier : 90%
// Aksel Acar : 10%

#include "anthill.h"
#include "food.h"
#include "squarecell.h"
#include "simulation.h"
#include <random>

using namespace std;

enum State {NB_FOOD, FOOD, NB_ANTHILL, ANTHILL, COLLECTOR, DEFENSOR, PREDATOR, END};
static bool reset(false);
static bool from_cmd_opened(false);

// Decrypte les infornations liées à la nourriture

bool use_data_food(string line) {
    istringstream data(line);
    int x(0);
    int y(0);
    if(!(data >> x >> y)) return false;
    else {
        Food food(x, y);
        if (!test_food(x, y)) return false;
    }
    return true;
}

// Decrypte les informations liées aux fourmillière

bool use_data_anthill(string line, int& nbc, int& nbd, int& nbp, int& counter) {
    int x(0), y(0), side(0), xg(0), yg(0);
    double total_food(0);
    istringstream data(line);

    if(!(data >> x >> y >> side >> xg >> yg >>
              total_food >> nbc >> nbd >> nbp)) return false;
    else {
        Anthill anthill(x, y, side, total_food, nbc, nbd, nbp);
        if (!anthill.test_anthill()) return false;
        if (!create_generator(xg, yg, 0, counter)) return false;
        return true;
    }
}

// Décrypte les informations liées aux fourmis

bool use_data_ant(string line, int& state_, int& counter) {
    int x(0), y(0), age(0);
    string has_food_string("false");
    istringstream data(line);
    if (state_ == COLLECTOR) {
        if(!(data >> x >> y >> age >> has_food_string)) return false;
        else {
            Etat_collector has_food(EMPTY);
            if (has_food_string == "true") has_food = LOADED;
            if(!create_collector(x, y, age, has_food, counter)) return false;
            return true;
        }
    }
    if(!(data >> x >> y >> age)) return false;
    else {
        switch(state_) {
            case DEFENSOR :
                if (!create_defensor(x, y, age, counter)) return false;
            break;
            
            case PREDATOR :
                if (!create_predator(x, y, age, counter)) return false;
            break;
        }
        return true;
    }
}



// Choisit quelle fourmi lire par la suite

void choose_ant_state(int nbc, int nbd, int nbp, int& state_) {
    if (!(nbc == 0)) state_ = COLLECTOR;
    else if (!(nbd == 0)) state_ = DEFENSOR;
    else if (!(nbp == 0)) state_ = PREDATOR;
}



// Contenu d'un case dans lequel on lit un nombre

bool case_number(int& state_, istringstream& data, int& amount, int& counter) {
    if(!(data >> amount)) return false;
    else counter = 0;
    if ((amount == 0) and (state_ == NB_FOOD)) state_ = NB_ANTHILL;
    else if (state_ == NB_FOOD) state_ = FOOD;
    else if (state_ == NB_ANTHILL) state_ = ANTHILL;
    return true;
}



// Contenu d'un case dans lequel on decode une fourmi

bool case_ant(int& state_, string& line, int& counter,
              int& amount_nbc, int& amount_nbd, int& amount_nbp,
              int& anthill_counter) {
    if(!(use_data_ant(line, state_, anthill_counter))) return false;
    else {
        counter++;
        switch (state_) {
            case COLLECTOR :
                if (counter == amount_nbc) {
                    counter = 0;
                    if (amount_nbd != 0) state_ = DEFENSOR;
                    else if (amount_nbp != 0) state_ = PREDATOR;
                    else state_ = ANTHILL;
                }
            break;

            case DEFENSOR :
                if (counter == amount_nbd) {
                    counter = 0;
                    if (amount_nbp != 0) state_ = PREDATOR;
                    else state_ = ANTHILL;
                }
            break;

            case PREDATOR :
                if (counter == amount_nbp) {
                    counter = 0;
                    state_ = ANTHILL;
                }
            break;
        }
    }
    return true;
}



// Contenu d'un case dans lequel on decode une fourmilliere

bool case_anthill(int& state_, string& line, int& counter,
                  int& amount_nbc, int& amount_nbd, int& amount_nbp,
                  int& amount) {
    if (!(use_data_anthill(line, amount_nbc, amount_nbd, amount_nbp,
                                                counter))) return false;
    else {
        counter++;
        choose_ant_state(amount_nbc, amount_nbd, amount_nbp, state_);
    }
    if (counter-1 == amount) state_ = END;
	return true;
}

// Decode une ligne entiere et relegue aux fonctions qui correspondent à la
// maniere dont il faut les decoder

void reset_read_line(int& state, int& counter1, int& amount1, int& counter2,
						int& amount_nbc, int& amount_nbd, int& amount_nbp) {
	state = NB_FOOD;
	counter1 = 0;
	amount1 = 0;
	counter2 = 0;
	amount_nbc = 0;
	amount_nbd = 0;
	amount_nbp = 0;
	reset = false;
}

bool read_line(string line) {
    istringstream data(line);
    static int state_(NB_FOOD), counter1(0), amount1(0), counter2(0);
    static int amount_nbc(0), amount_nbd(0), amount_nbp(0);
    if (reset) reset_read_line(state_, counter1, amount1, counter2, amount_nbc,
													amount_nbd, amount_nbp);
    switch (state_) {
        case NB_FOOD :
            if(!case_number(state_, data, amount1, counter1)) return false;
        break;
        case FOOD :
            if (!(use_data_food(line))) return false;
            else counter1++;
            if (counter1 == amount1) state_ = NB_ANTHILL;
        break;
        case NB_ANTHILL :
            if (!case_number(state_, data, amount1, counter1)) return false;
        break;
        case ANTHILL :
            if(!case_anthill(state_, line, counter1, amount_nbc,
                                amount_nbd, amount_nbp, amount1)) return false;
        break;
        case COLLECTOR :
            if(!case_ant(state_, line, counter2, amount_nbc,
                                amount_nbd, amount_nbp, counter1)) return false;
        break;
        case DEFENSOR :
            if(!case_ant(state_, line, counter2, amount_nbc,
                                amount_nbd, amount_nbp, counter1)) return false;
        break;
        case PREDATOR :
            if(!case_ant(state_, line, counter2, amount_nbc,
                                amount_nbd, amount_nbp, counter1)) return false;
        break;
        case END :
        break;
    }
    return true;
}

// Fonction principale qui lit un fichier et renvoie la lecture de
// chaque ligne a une autre fonction

bool read_file(string file_name) {
    string line;
    ifstream file(file_name);
    if (!file.fail()) {
        while(getline(file >> ws,line)) {
            string new_line = "";
            for (unsigned int i(0) ; i < line.size() ; i++) {
                if(line[i]=='#') i = line.size();
                else {
                    new_line += line[i];
                }
            }
            if (new_line != "") {
				if (!read_line(new_line)) {
					return false;
				}
			}
        }
    }
    from_cmd_opened = true;
    return true;
}

void reset_simulation() {
	reset_current_anthill();
	reset_current_food();
	empty_square(0, 0, 128, true);
	reset = true;
}

int from_simulation_get_food_nb() {
	return get_food_number();
}

bool from_simulation_get_life(int index) { return extract_anthill_life(index); }
int from_simulation_get_nbc(int index) { return extract_anthill_nbc(index); }
int from_simulation_get_nbd(int index) { return extract_anthill_nbd(index); }
int from_simulation_get_nbp(int index) { return extract_anthill_nbp(index); }
double from_simulation_get_total_food(int index) {return extract_anthill_food(index);}
int from_simulation_get_dead_anthills() { return dead_anthills_nb(); }

// Fonction qui va récupérer sous forme de string les informations d'une
// fourmilliere necessaires pour la sauvegarder
string anthill_save_info(int index) {
	if (extract_anthill_life(index) == false) return anthill_main_info(index);
	else return "dead";
}



int anthill_nb() { return get_anthill_number(); }



string show_food() {
	return get_all_food_coordinates();
}


void draw_all() {
	draw_food();
	draw_anthill();	
}

bool get_from_cmd_opened() { return from_cmd_opened; }

void simulation_update() {

	static default_random_engine e;
	
	random_food_generation(e);
		
	update_anthill(e);
}
