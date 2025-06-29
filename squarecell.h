#ifndef MAIN_CPP_SQUARECELL_H
#define MAIN_CPP_SQUARECELL_H

#include <iostream>
#include <vector>

#include "graphic.h"

struct Square {

    int x_;
    int y_;
    int size_ ;
    bool creation_point;
};

bool test_square(int x, int y, int size, bool centre);

bool test_square(Square A);

void error_coordinates(int x_, int y_, int size, bool centre, 
										int& wrong_x, int& wrong_y);

bool test_index(int x, int y , int size, bool centre);
bool test_outside(int x, int y , int size, bool centre);

bool test_index(Square square);
bool test_outside(Square square);

double get_case(int x, int y);
void set_case(int x, int y, bool val);

bool set_square (int x, int y , int size, bool centre);

bool set_square(Square A);

void empty_square(int x, int y , int size, bool centre);

void empty_square(Square C);

void draw_losange (int x, int y);

void draw_vide (int x, int y, int size, int family_colour);

void draw_uniforme (int x, int y, uniform_size ant, int family_colour);

void draw_diagonale (int xc, int yc, int family_colour);

void draw_grille (int xc, int yc, int family_colour);

#endif //MAIN_CPP_SQUARECELL_H

