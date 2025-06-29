#ifndef FOOD_H
#define FOOD_H

#include <iostream>
#include <vector>
#include <string>
#include <random>

enum diag_family { EVEN, ODD };

class Food {
private :
    int x_;
    int y_;
    diag_family diagonal_family;

public :

    Food(int x = 0, int y = 0);

    void setx(int x);
    void sety(int y);

    int getx();
    int gety();
	diag_family get_diag_family();

    static void add(Food f);
    
    virtual ~Food();

};

unsigned int get_food_number();

bool test_food(int x, int y);

std::string get_all_food_coordinates();

void reset_current_food();

void draw_food();

diag_family food_diag_family(int index);

int max(int const& vx, int const& vy);

int food_x(int index);
int food_y(int index);

void delete_food(int index);

void food_squarecell_empty(diag_family diag_fam);
void food_squarecell_set(diag_family diag_fam);

#endif
