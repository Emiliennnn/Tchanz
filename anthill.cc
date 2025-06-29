// Emilien Coudurier et Aksel Acar

// Emilien Coudurier : 50%
// Aksel Acar : 50%

#include "anthill.h"
#include "squarecell.h"
#include "message.h"

using namespace std;

// Liste des fourmillieres actuellement existentes
static vector<Anthill> current_anthill;


// Teste si une fourmi est contenue dans la fourmilliere en question
bool Anthill::is_ant_in_anthill(Ant* ant) {
	bool x_cond_min, x_cond_max, y_cond_min, y_cond_max;
	
	x_cond_min = ant->getx()-(ant->get_size()/2) > x_;

	x_cond_max = ant->getx()+(ant->get_size()/2) < x_+side_-1;

	y_cond_min = ant->gety()-(ant->get_size()/2) > y_;

	y_cond_max = ant->gety()+(ant->get_size()/2) < y_+side_-1;

	if (x_cond_min and x_cond_max and y_cond_min and y_cond_max) return true;
	else return false;
}

// Constructuer de fourmilliere qui teste si la fourmilliere peut etre créée
// sans problemes, et l'ajoute à la liste des fourmillieres courantes

Anthill::Anthill(int x, int y, int side, int total_food, 
		int nbC, int nbD, int nbP, bool end_of_klan) :
		x_(x), y_(y), side_(side), total_food_(total_food),
		nbC_(nbC), nbD_(nbD), nbP_(nbP), end_of_klan(end_of_klan) {
	current_anthill.push_back(*this);
	end_of_klan = false;
}
		

bool Anthill::test_anthill() {
	if (!test_index(x_, y_, side_, true)) return false;
	if (!test_outside(x_, y_, side_, true)) return false;
	if (current_anthill.size() != 0) {
		if (!anthill_overlap(*this)) return false;
	}
	return true;
}

// Renvoie la coordonnée x de la fourmilliere
int Anthill::getx() { return x_; }

// Renvoie la coordonnée y de la fourmilliere
int Anthill::gety() { return y_; }

// Renvoie la taille de la fourmilliere
int Anthill::get_side() { return side_; }

double Anthill::get_total_food() { return total_food_; }
int Anthill::get_nbc() { return nbC_; }
int Anthill::get_nbd() { return nbD_; }
int Anthill::get_nbp() { return nbP_; }
int Anthill::get_nbt() { return current_ant.size(); }


// Ajoute a la fourmilliere une fourmi
void Anthill::add_ant(Ant* a) {
	current_ant.push_back(a);
}
	
	
// Teste si une nouvelle fourmilliere est superposée sur une précédente	
bool Anthill :: anthill_overlap (Anthill anthill_1) {
	int x = anthill_1.x_;
	int y = anthill_1.y_;
	int side = anthill_1.side_;
	for ( unsigned int i(0); i < current_anthill.size(); i ++ ) {
		if (current_anthill[i].x_ == x_ and current_anthill[i].y_ == y_) i++;
		if (i<current_anthill.size()){
			if ((current_anthill[i].x_ <= x +  side -1) 
				and (x <= current_anthill[i].x_+current_anthill[i].side_ -1)
				and (current_anthill[i].y_ <= y + side -1) 
				and (y <= current_anthill[i].y_+ current_anthill[i].side_ -1)) {
				cout  << message::homes_overlap(current_anthill.size()-1, i); 	
				return false;
				}
			}
		}
	return true;
}


// Crée un defensor et l'ajoute à sa fourmilliere
bool create_defensor(int x, int y, int age, int anthill_nb) {
	Ant* defensor = new Defensor(x, y, age, anthill_nb);
	if (!(*defensor).test_ant(x, y)) return false;
	if (current_anthill[anthill_nb-1].is_ant_in_anthill(defensor)) {
		current_anthill[anthill_nb-1].add_ant(defensor);
	} else { 
		cout << message::defensor_not_within_home(x, y, anthill_nb-1);
		return false;
	}
	return true;
}



// Crée un predator et l'ajoute à sa fourmilliere
bool create_predator(int x, int y, int age, int anthill_nb) {
	Ant* predator = new Predator(x, y, age, anthill_nb);
	if (!(*predator).test_ant(x, y)) return false;
	current_anthill[anthill_nb-1].add_ant(predator);
	return true;
}



// Crée un collector et l'ajoute à sa fourmilliere
bool create_collector(int x, int y, int age, Etat_collector has_food, int anthill_nb) {
	Ant* collector = new Collector(x, y, age, has_food, anthill_nb);
	if (!(*collector).test_ant(x, y)) return false;
	current_anthill[anthill_nb-1].add_ant(collector);
	return true;
}



// Crée un generator et l'ajoute à sa fourmilliere
bool create_generator(int x, int y, int age, int anthill_nb) {
	Ant* generator = new Generator(x, y, age, anthill_nb);
	if (!(*generator).test_ant(x, y)) return false;
	if (current_anthill[anthill_nb].is_ant_in_anthill(generator)) {
		current_anthill[anthill_nb].add_ant(generator);
	} else {
		cout << message::generator_not_within_home(x, y, anthill_nb);
		return false;
	}
	return true;
}


// Affiche la liste des fourmillieres courantes (à titre de test)
void show_current_anthill() {
	for (unsigned int i(0) ; i < current_anthill.size() ; i++) {
		cout << endl << "Anthill number " << i+1 << " : " << endl << 
		"x = " << current_anthill[i].getx() << endl << "y = " << 
		current_anthill[i].gety() << endl;
		cout << "Ants related : " << endl; 
		current_anthill[i].show_current_ant();
		cout << endl;
	}
}


// Affiche la liste des fourmis d'une fourmilliere (à titre de test)
void Anthill::show_current_ant() {
	cout << current_ant.size() << endl;
	for (unsigned int i(0) ; i < current_ant.size() ; i++) {
		cout << "Ant number " << i+1 << " : " << endl << "x = " 
		<< (*(current_ant[i])).getx() << endl << "y = " << 
		(*(current_ant[i])).gety() << endl;
	}
}

int get_anthill_number() {
	int total(0);
	for (unsigned int i(0) ; i < current_anthill.size() ; i++) {
		if (current_anthill[i].get_end_of_klan() == false) total++;
	}
	return total;
}

double extract_anthill_food(int index) { 
	return current_anthill[index-1].get_total_food(); 
}

int dead_anthills_nb() {
	int total(0);
	for (unsigned int i(0) ; i < current_anthill.size() ; i++) {	
		if (current_anthill[i].get_end_of_klan() == true) total++;
	}
	return total;
}
int extract_anthill_nbc(int index) { return current_anthill[index-1].get_nbc(); }
int extract_anthill_nbd(int index) { return current_anthill[index-1].get_nbd(); }
int extract_anthill_nbp(int index) { return current_anthill[index-1].get_nbp(); }
bool extract_anthill_life(int index) { 
	return current_anthill[index-1].get_end_of_klan(); 
}

void reset_current_anthill() {
	for (unsigned int i(0) ; i < current_anthill.size() ; i++) {
		current_anthill[i].reset_current_ant();
	}
	current_anthill.clear();
}

void Anthill::reset_current_ant() {
	for (unsigned int i(0) ; i < current_ant.size() ; i++) {
		delete current_ant[i];
		current_ant[i] = NULL;
	}
	current_ant.clear();
}

void Anthill::draw_my_ants() {
	for (unsigned i(0) ; i < current_ant.size() ; i++) {
		current_ant[i]->draw_ant();
	}
}


void draw_anthill() {
	for (unsigned i(0) ; i < current_anthill.size() ; i++) {
		if (!current_anthill[i].get_end_of_klan()) {
			current_anthill[i].draw_my_ants();
			draw_vide(current_anthill[i].getx(), current_anthill[i].gety(),
									current_anthill[i].get_side(), i+1);
		}
	}
}

int Anthill::get_xg() {
	return current_ant[0]->getx();
}

int Anthill::get_yg() {
	return current_ant[0]->gety();
}

string anthill_main_info(int index) {
	int dead_anthills(0);
	for (int i(0) ; i < index ; i++) {
		if (current_anthill[i].get_end_of_klan() == true) dead_anthills++;
	}
	string total("");
	total += to_string(current_anthill[index-1].getx()) + " ";
	total += to_string(current_anthill[index-1].gety()) + " ";
	total += to_string(current_anthill[index-1].get_side()) + " ";
	total += to_string(current_anthill[index-1].get_xg()) + " ";
	total += to_string(current_anthill[index-1].get_yg()) + " ";
	total += to_string(current_anthill[index-1].get_total_food()) + " ";
	total += to_string(current_anthill[index-1].get_nbc()) + " ";
	total += to_string(current_anthill[index-1].get_nbd()) + " ";
	total += to_string(current_anthill[index-1].get_nbp()) + " " 
	+ " # Anthill #" + to_string(index-dead_anthills) + "\n";
	
	total += current_anthill[index-1].anthill_ant_info();

	return total;
}

string Anthill::anthill_ant_info() {
	string total("");
	string announcement("");
	for (unsigned int i(1) ; i < current_ant.size() ; i++) {		
		if (current_ant[i]->ant_type_announcement() != announcement) {
			total += "\n\t" + current_ant[i]->ant_type_announcement() + "\n";
			announcement = current_ant[i]->ant_type_announcement();
		}
		total += current_ant[i]->ant_save_info() + "\n";
	}
	return total;
}


void Anthill::update_end_of_klan() {
	end_of_klan = true;
}

bool Anthill::get_end_of_klan() { return end_of_klan; }

void Anthill::update_my_ants() {
	for (unsigned int i(0) ; i < current_ant.size() ; i++) {
		if (!end_of_klan) {
			Ant* prey = NULL;
			if (current_ant[i]->ant_type_announcement() == "# predators" or
							current_ant[i]->ant_type_announcement() == "# defensors") {
				prey = get_closest_ant(current_ant[i]);
				if (prey->get_size() == 5) prey = current_ant[0];
			}
			if (current_ant[i]->update_ant(get_xg(), get_yg(), x_, y_, side_, prey)) {
				total_food_+=val_food;
			}
			prey = NULL;
			if (current_ant[0]->get_end_of_life()) end_of_klan = true;
		}
	}
}

void Anthill::delete_ant(int index) {
	if (current_ant[index]->ant_type_announcement() == "# defensors") nbD_--;
	if (current_ant[index]->ant_type_announcement() == "# predators") nbP_--;
	if (current_ant[index]->ant_type_announcement() == "# collectors") nbC_--;
	delete current_ant[index];
	current_ant[index] = NULL;
	current_ant.erase(current_ant.begin() + index);
}

void Anthill::delete_anthill() {
	unsigned int ant_nb(current_ant.size());
	for (unsigned int i(0) ; i < ant_nb ; i++) delete_ant(0);
}

int distance_between_ants(Ant* ant1, Ant* ant2) {
	static int distance(0), xdist(0), ydist(0);
	xdist = abs(ant1->getx() - ant2->getx());
	ydist = abs(ant1->gety() - ant2->gety());
	distance = max(xdist, ydist);
	return distance;
}

Ant* Anthill::get_closest_ant(Ant* ant) {
	int distance(128);
	static int temp(0);
	Ant* closest_ant = current_ant[0];
	static Ant* potential_ant(NULL);
	for (unsigned int i(0) ; i < current_anthill.size() ; i++) {
		if (i != unsigned(ant->get_my_anthill()-1) and !current_anthill[i].get_end_of_klan()) {
			potential_ant = current_anthill[i].my_closest_ant(ant);
			temp = distance_between_ants(potential_ant, ant);
			if (ant->ant_type_announcement() != "# defensors") {
				switch (anthill_state) {
					case FREE :
						if (temp < distance and is_ant_in_anthill(potential_ant)) {
							distance = temp;
							closest_ant = current_anthill[i].my_closest_ant(ant);
						}
					break;
					case CONSTRAINED : 
						if (temp < distance) {
							distance = temp;
							closest_ant = current_anthill[i].my_closest_ant(ant);
						}
					break;
				}
			} else {
				if (temp < distance) {
					distance = temp;
					closest_ant = current_anthill[i].my_closest_ant(ant);
				}
			}
		}
	}
	return closest_ant;
}

Ant* Anthill::my_closest_ant(Ant* ant) {
	int distance(128);
	static int temp(0);
	int ant_index(0);
	static bool is_collector(false);
	static bool is_predator(false);
	for (unsigned int i(1) ; i < current_ant.size() ; i++) {
		temp = distance_between_ants(current_ant[i], ant);
		is_collector = current_ant[i]->ant_type_announcement() == "# collectors";
		is_predator = current_ant[i]->ant_type_announcement() == "# predators";
		if (temp < distance and (is_collector or is_predator)) {
			distance = temp;
			ant_index = i;
		}
	}
	return current_ant[ant_index];
}

bool Anthill::food_outside(int const& x, int const& y) {
	bool x_cond_min, x_cond_max, y_cond_min, y_cond_max;

	x_cond_min = x < x_;

	x_cond_max = x > x_+side_-1;

	y_cond_min = y < y_;

	y_cond_max = y > y_+side_-1;

	if (x_cond_min or x_cond_max or y_cond_min or y_cond_max) return true;
	else return false;
}

bool food_outside_anthills(int const& x, int const& y) {
	for (unsigned int i(0) ; i < current_anthill.size() ; i++) {
		if(!current_anthill[i].food_outside(x, y)) return false;
	}
	return true;
}

void random_food_generation(std::default_random_engine& e) {
	double p(food_rate); // probabilité ; voir section 2.3.2.3 et Annexe A
	bernoulli_distribution b(p); //booléen true avec une probabilité p
	if (b(e)) {
		for (unsigned int i(0) ; i < max_food_trial ; i++) {
			uniform_int_distribution<unsigned> u(0, 127);
			int random_x(u(e));
			int random_y(u(e));
			
			if (test_square(random_x, random_y, 1, true) 
								and food_outside_anthills(random_x, random_y)) {
				Food food(random_x, random_y);
				set_square(random_x, random_y, 1, true);
				i = max_food_trial;
			}
		}
	}
}


void Anthill::update_ant_death() {
	int death_counter(0);
	if (current_ant[0]->get_end_of_life()) end_of_klan = true;
	else {
		unsigned int ant_nb(current_ant.size());
		for (unsigned int i(1) ; i < ant_nb ; i++) {
			if (current_ant[i-death_counter]->get_end_of_life()) {
				delete_ant(i-death_counter);
				death_counter++;
			}
		}
	}
}


void Anthill::collector_birth(int anthill_index) {
	
	vector<Ant*> :: iterator it; 
	it = current_ant.begin();
	
	for (int j(y_+side_-3); j>= y_+2; j--) {
		for (int i(x_+2);i<= x_+side_-3; i++) {
			if (test_square(i, j, 3, false)) {
				Ant* collector = new Collector(i,j,0,EMPTY, anthill_index+1);
				if (is_ant_in_anthill(collector)) {
					current_ant.insert(it + 1 + nbC_  , collector);
					nbC_++;
					collector->test_ant(i, j);
					j = y_;
					i = x_ + side_ -2;
				} else {
					delete collector;
					collector = NULL;
				}
			}
		}
	}
}

void Anthill::defensor_birth(int anthill_index) {
	
	vector<Ant*> :: iterator it; 
	it = current_ant.begin();
	 
	for (int j(y_+side_-3); j>= y_+2; j--) {
		for (int i(x_+2);i<= x_+side_-3; i++) {
			if (test_square(i,j,3,false)) {
				Ant* defensor = new Defensor(i,j,0, anthill_index+1); 
				if (is_ant_in_anthill(defensor)) {
					current_ant.insert(it + 1+ nbC_ +nbD_, defensor );
					nbD_++;
					defensor->test_ant(i, j);
					j = y_;		
					i = x_ + side_ -2;
				} else {
					delete defensor;
					defensor = NULL;
				}
			}
		}
	}
}

void Anthill::predator_birth(int anthill_index) {
	
	vector<Ant*> :: iterator it; 
	it = current_ant.begin();
		
	for (int j(y_+side_-2); j>= y_+1; j--) {
		for (int i(x_+1);i<= x_+side_-2; i++) {
			if (test_square(i,j,1,false)) {
				Ant* predator = new Predator(i,j,0, anthill_index+1); 
				if (is_ant_in_anthill(predator)) {
					current_ant.insert(it + 1+ nbC_ +nbD_, predator );
					nbP_++;
					predator->test_ant(i, j);
					j = y_;
					i = x_ + side_-1;
				} else {
					delete predator;
					predator = NULL;
				}
			}	
		}
	}
}

	
double min (double a) {
	if (a > 1) return 1;
	else  return a;
} 


void Anthill::random_ant_generation(std::default_random_engine& e,int anthill_index) {
	double birth (birth_rate * total_food_); 
	double ant_rate (min(birth));
	double nbT = 1 + nbC_ + nbD_ + nbP_;
	bool birth_given = false; 
	double prop_c, prop_d;
	
	double p(ant_rate);  
	bernoulli_distribution b(p); //booléen true avec une probabilité p
	if (b(e)) {
		switch (anthill_state){
			case (FREE) : 
				prop_c = prop_free_collector; 
				prop_d = prop_free_defensor;  
				
			break; 
			case (CONSTRAINED) : 
				prop_c = prop_constrained_collector; 
				prop_d = prop_constrained_defensor; 
			break;
		}
		
		if (!birth_given){
			if((nbC_ == 0) or (nbC_/(nbT-1) < prop_c)) {
				collector_birth(anthill_index); 
				birth_given = true;
			}
		}
		if (!birth_given){	
			if ((nbD_ == 0) or (nbD_/(nbT-1) < prop_d)) {
				defensor_birth(anthill_index);
				birth_given = true;
			}
		}	
		if (!birth_given) {
			predator_birth(anthill_index);
			birth_given = true;
		}
	}
}


bool Anthill :: test_sizeF_in_world (int sizeF ,int x ,int y) {
	
	int origin_x = x ;
	int origin_y = y ;  
	int side = sizeF + 2;
	
	if ((origin_x < 0) or (origin_y < 0) 
			or (origin_x + side > 127) or (origin_y + side > 127) ) {  
				
	return false;
	}
	return true; 	
}


bool Anthill :: test_sizeF (int sizeF, int x, int y) {
	
	int origin_x = x ;
	int origin_y = y ;  
	int side = sizeF;
	
	for ( unsigned int i(0); i < current_anthill.size(); i ++ ) {
		if (current_anthill[i].get_end_of_klan() == false) {
			if (current_anthill[i].x_ == x_ and current_anthill[i].y_ == y_) i++;
				
			if (i<current_anthill.size()) {
				if ((current_anthill[i].x_ <= origin_x +  side -1) 
					and (origin_x <= current_anthill[i].x_+current_anthill[i].side_ -1)
					and (current_anthill[i].y_ <= origin_y + side -1) 
					and (origin_y 
					<= current_anthill[i].y_+ current_anthill[i].side_ -1)) {
					return false;
				}
			}
		}	
	}
	return true;
}


void Anthill :: test_update_sizeF() {
	int sizeF = sqrt(4*(pow(sizeG, 2) + pow(sizeC,2)* nbC_ 
					+ pow(sizeD,2)* nbD_ + pow(sizeP,2)*nbP_)); 
	bool anthill_upgrade(false);  
	int old_side = side_; 
	side_ = sizeF + 2 ;
	if (!anthill_upgrade) {
		if (test_sizeF(side_, x_, y_) and test_sizeF_in_world(side_, x_, y_)){
		anthill_upgrade = true; 
		anthill_state = FREE; 
		}
	}
	if (!anthill_upgrade) {
		if (test_sizeF(side_, x_, y_- (side_- old_side)) 
		and test_sizeF_in_world(side_, x_, y_- (side_- old_side))){
		x_ = x_;
		y_ = y_- (side_-old_side);
		anthill_upgrade = true;
		anthill_state = FREE; 
		}
	}
	if (!anthill_upgrade) {
		if (test_sizeF(side_, x_-(side_- old_side), y_- (side_- old_side)) 
		and test_sizeF_in_world(side_, x_-(side_- old_side), y_- (side_- old_side))){
		x_ = x_-(side_- old_side); 
		y_ = y_-(side_- old_side);
		anthill_upgrade = true;
		anthill_state = FREE; 
		}
	}
	if (!anthill_upgrade) {
		if (test_sizeF(side_, x_-(side_- old_side), y_) 
		and test_sizeF_in_world(side_, x_-(side_- old_side), y_)){
		x_ = x_-(side_- old_side);
		y_ = y_;
		anthill_upgrade = true;
		anthill_state = FREE; 
		}
	}
	if (!anthill_upgrade) {
		side_ = old_side;	
		anthill_state = CONSTRAINED;
	}
}

void Anthill::false_end_of_klan() {
	end_of_klan = false;
}

void Anthill::update_current_food(double new_food) {
	total_food_ = new_food; 
}


void update_anthill(std::default_random_engine& e) {
	for (unsigned int i(0) ; i < current_anthill.size() ; i++) {
		if (current_anthill[i].get_end_of_klan() == false) {	
			double nbT = 1 + current_anthill[i].get_nbc() 
				+ current_anthill[i].get_nbd() + current_anthill[i].get_nbp();
			double new_food_value(current_anthill[i].get_total_food()-(nbT*food_rate));
			current_anthill[i].test_update_sizeF();
		
			if (new_food_value > 0.) {
				current_anthill[i].update_current_food(new_food_value);
			}
			if (new_food_value <= 0.0001) {
				current_anthill[i].update_end_of_klan(); 
			}
			current_anthill[i].random_ant_generation(e,i);
			current_anthill[i].update_my_ants();
		}
	}
	for (unsigned int i(0) ; i < current_anthill.size() ; i++) {
		if (!(current_anthill[i].get_end_of_klan())) {
			current_anthill[i].update_ant_death();
		}
		if (current_anthill[i].get_nbt() != 0 
							and current_anthill[i].get_end_of_klan() == true) {
			current_anthill[i].delete_anthill();
		}
	}
}
