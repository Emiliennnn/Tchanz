#ifndef ANTHILL_H
#define ANTHILL_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#include "ant.h"
#include "constantes.h"

class Anthill {
private :
	int x_;
	int y_; 
	int side_;
	double total_food_;
	int nbC_;
	int nbD_;
	int nbP_;
	bool end_of_klan;
	Etat_fourmiliere anthill_state;
	std::vector <Ant*> current_ant;
	
public :
	Anthill(int x = 0, int y = 0, int side = 0, int total_food = 0, 
			int nbC = 0, int nbD = 0, int nbP = 0, bool end_of_klan = 0);
			
	int getx();
	int gety();
	int get_side();
	double get_total_food();
	int get_nbc();
	int get_nbd();
	int get_nbp();
	int get_xg();
	int get_yg();
	int get_nbt();
	bool get_end_of_klan();
	void update_my_ants();
	void update_end_of_klan();
	void delete_ant(int index);
	void update_current_food(double new_food);
	void delete_anthill();
	void false_end_of_klan();
	
	bool is_ant_in_touch(Ant* ant);
	bool is_ant_in_anthill(Ant* ant);
	bool anthill_overlap (Anthill anthill_1);
	bool test_anthill();
	bool test_sizeF_in_world (int sizeF ,int x ,int y);
	bool test_sizeF (int sizeF, int x, int y);
	void test_update_sizeF();
	void collector_birth(int anthill_index);
	void defensor_birth(int anthill_index);
	void predator_birth(int anthill_index);
	void random_ant_generation(std::default_random_engine& e, int anthill_index);
	void add_ant(Ant* a);
	void show_current_ant();
	void reset_current_ant();
	std::string anthill_ant_info();
	Ant* my_closest_ant(Ant* ant);
	Ant* get_closest_ant(Ant* ant);
	bool food_outside(int const& x, int const& y);
	void update_ant_death();
	
	void draw_my_ants();
};

void show_current_anthill();

bool create_defensor(int x, int y, int age, int anthill_nb);
bool create_collector(int x, int y, int age, Etat_collector has_food, int anthill_nb);
bool create_predator(int x, int y, int age, int anthill_nb);
bool create_generator(int x, int y, int age, int anthill_nb);

double extract_anthill_food(int index);
int extract_anthill_nbc(int index);
int extract_anthill_nbd(int index);
int extract_anthill_nbp(int index);
bool extract_anthill_life(int index);
int dead_anthills_nb();

int get_anthill_number();

void reset_current_anthill();

void draw_anthill();

std::string anthill_main_info(int index);

bool food_outside_anthills(int const& x, int const& y);
void random_food_generation(std::default_random_engine& e);

void update_anthill(std::default_random_engine& e);
double min (double a);
int distance_between_ants(Ant* ant1, Ant* ant2);

#endif
