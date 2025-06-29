#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

bool read_file(std::string file_name);

bool use_data_food(std::string line);
bool use_data_anthill(std::string line, int& nbc, int& nbd, int& nbp, int& counter);
bool use_data_ant(std::string line, int& state_, int& counter);
void choose_ant_state(int nbc, int nbd, int nbp, int& state_);
bool case_number(int& state_, std::istringstream& data, int& amount, int& counter);
bool case_ant(int& state_, std::string& line, int& counter,
              int& amount_nbc, int& amount_nbd, int& amount_nbp,
              int& anthill_counter);
bool case_anthill(int& state_, std::string& line, int& counter,
                  int& amount_nbc, int& amount_nbd, int& amount_nbp,
                  int& amount);
bool read_line(std::string line);

int from_simulation_get_food_nb();

bool from_simulation_get_life(int index);
int from_simulation_get_nbc(int index);
int from_simulation_get_nbd(int index);
int from_simulation_get_nbp(int index);
double from_simulation_get_total_food(int index);
int from_simulation_get_dead_anthills();

int anthill_nb();

void reset_simulation();

std::string show_food();
std::string anthill_save_info(int index);

void draw_all();

void simulation_update();
bool get_from_cmd_opened();

#endif
