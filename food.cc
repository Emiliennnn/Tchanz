//Emilien Coudurier et Aksel Acar

//Emilien Coudurier : 50 %
//Aksel Acar : 50 %

#include "food.h"
#include "squarecell.h"
#include "message.h"
#include "constantes.h"
#include <cmath>
#include <algorithm>

using namespace std ;

static vector<Food> current_food;

Food::Food(int x, int y) : x_(x), y_(y) {
	if ((x_%2) == (y_%2)) diagonal_family = EVEN;
	else diagonal_family = ODD;
	current_food.push_back(*this);
}

Food::~Food() {
	//empty_square(x_, y_, 1, true);
}

bool test_food(int x, int y) {
	if (!(set_square(x, y, 1, true))) {
		cout << message::food_overlap(x, y);
		return false;
	} else {
		return true;
	}
}

void Food::add(Food f) {current_food.push_back(f);}

void Food::setx(int x) { x_ = x ;}
void Food::sety(int y) { y_ = y ;}

int Food::getx() { return x_; }
int Food::gety() { return y_; }

diag_family Food::get_diag_family() { return diagonal_family; }

unsigned int get_food_number() { return current_food.size(); }

string get_all_food_coordinates() {
	string total("");
	for (unsigned int i(0) ; i < current_food.size() ; i++) {
		total += to_string(current_food[i].getx()) + " " 
									+ to_string(current_food[i].gety()) + "\n";
	}
	return total;
}

void reset_current_food() {
	current_food.clear();
}

void draw_food() {
	for (unsigned int i(0) ; i < current_food.size() ; i++) {
		draw_losange(current_food[i].getx(), current_food[i].gety());
	}
}

diag_family food_diag_family(int index) {
	return current_food[index].get_diag_family();
}

int max(int const& a, int const& b) {
	if (a > b) return a;
	else return b;
}

int food_x(int index) { return current_food[index].getx(); }
int food_y(int index) { return current_food[index].gety(); }

void food_squarecell_empty(diag_family diag_fam) {
	for (unsigned int i(0) ; i < current_food.size() ; i++) {
		if (food_diag_family(i) == diag_fam) {
			empty_square(food_x(i), food_y(i), 1, true);
		}
	}
}

void food_squarecell_set(diag_family diag_fam) {
	for (unsigned int i(0) ; i < current_food.size() ; i++) {
		if (food_diag_family(i) == diag_fam) {
			set_square(food_x(i), food_y(i), 1, true);
		}
	}
}

void delete_food(int index) {
	empty_square(food_x(index), food_y(index), 1, true);
    swap(current_food[index], current_food[current_food.size()-1]);
    current_food.pop_back();
}
