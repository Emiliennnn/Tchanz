//Emilien Coudurier et Aksel Acar

//Emilien Coudurier : 10 %
//Aksel Acar : 90 %

#include "squarecell.h"
#include "error_squarecell.h" 

constexpr short unsigned g_max(128);

using namespace std;

using Grid = vector <vector <bool>> ;
static Grid grid(g_max, vector<bool>(g_max,false) ) ;


//Surcharge de la fonction test_index.
//Le bool centre permet de déterminer si le carré est centré ou non.
//Bool false = centré
//Bool true = non centré

bool test_index(int x, int y , int size, bool centre) {
    Square temp = {x, y, size, centre};
    return test_index(temp);
}

bool test_index(Square square) {
    if (square.x_< 0 or square.x_>g_max-1) {
        cout << error_squarecell::print_index( square.x_, g_max-1);
        return false;
    }
    if (square.y_<0 or square.y_>g_max-1) {
        cout << error_squarecell::print_index( square.y_ , g_max-1);
        return false;
    }
    return true;
}

//Surcharge de la fonction test_outside.

bool test_outside(int x, int y , int size, bool centre) {
    Square temp = {x, y, size, centre};
    return test_outside(temp);
}

bool test_outside(Square square) {
    if ((square.creation_point) and (square.x_+square.size_ > g_max)) {
        cout << error_squarecell::print_outside(square.x_ , square.size_, g_max-1);
        return false;
    }
    if ((square.creation_point) and (square.y_+square.size_ > g_max)) {
        cout << error_squarecell::print_outside(square.y_ , square.size_, g_max-1);
        return false;
    }
    if ((!square.creation_point) and ((square.x_+(square.size_/2) > g_max-1)
                                    or (square.x_-(square.size_/2) < 0))) {
        cout << error_squarecell::print_outside(square.x_ , square.size_, g_max-1);
        return false;
    }
    if ((!square.creation_point) and ((square.y_+(square.size_/2) > g_max-1)
                                    or (square.y_-(square.size_/2) < 0))) {
        cout << error_squarecell::print_outside(square.y_ , square.size_, g_max-1);
        return false;
    }
    return true;
}

//Vérifie que l'espace de la grille est libre et qu'il n'y a pas de superposition.

bool test_square(Square square) {

    int k(0);
    int x(0);
    int y(0);

    test_index(square);
    test_outside(square);

    switch (square.creation_point) {
		case true :
            x = square.x_;
            y = square.y_;
        break;

        case false :
            k = square.size_ / 2;
            x = square.x_ - k;
            y = square.y_ - k;
        break;
    }

    if (square.size_ == 1) {
        if (grid[g_max -1 -square.y_][square.x_]) return false;
        else return true;
    } else {
        for (int i(g_max  - (y + square.size_)); i < g_max - y; i++) {
            for (int j(x); j < x + square.size_; j++) {
                if (grid[i][j]) {
                    return false;
                }
            }
        }
    }
    return true;
}

//Surcharge de la fonction test_square.

bool test_square(int x, int y, int size, bool centre) {
	Square temp = {x, y, size, centre};
    return test_square(temp);
}

//Fonction qui donne les coordonnées des endroits où il y a une superposition.

void error_coordinates(int x_, int y_, int size, bool centre,
                       int& wrong_x, int& wrong_y) {

    int k(0);
    int x(0);
    int y(0);

    switch (centre) {
		case true :
            x = x_;
            y = y_;
		break;

        case false :
            k = size / 2;
            x = x_ - k;
            y = y_ - k;
		break;
    }

    if (size == 1) {
        if (grid[g_max -1 -y][x]) {
            wrong_x = x;
            wrong_y = y;
        }
    }
    else {
        for (int i(g_max - (y + size)); i < g_max - y; i++) {
            for (int j(x); j < x + size; j++) {
                if (grid[i][j]) {
                    wrong_x = j;
                    wrong_y = g_max-i-size+(size-1);
                    break;
                }
            }
        }
    }
}

double get_case(int x, int y) { return grid[x][y]; }
void set_case(int x, int y, bool val) { grid[x][y] = val; }

//Fonction qui déclare l’occupation des cellules correspondant
// à un carré passé en paramètre.

bool set_square(Square square) {
    if (test_square(square)) {
		//if (square.size_ == 1) set_case(g_max-square.y_ -1, square.x_, true);
        if (square.creation_point) {
            for (int i(g_max - (square.y_ + square.size_));
										i < g_max - square.y_; i++) {
                for (int j(square.x_); j < square.x_ + square.size_; j++) {
                    set_case(i, j, true);
                }
            }
        } else {
            int k = square.size_ / 2;
            int x = square.x_ - k;
            int y = square.y_ - k;

            for (int i(g_max - (y + square.size_)); i < g_max - y; i++) {
                for (int j(x); j < x + square.size_; j++) {
                    set_case(i, j, true);
                }
            }
        }
        return true;
    }
    return false;
}

//Surcharge de set_square.

bool set_square (int x, int y , int size, bool centre) {
    Square temp = {x, y, size, centre};
    return set_square(temp);
}

//Fonction qui libère les cellules de la grille correspondant à un carré passé
// en paramètre en les faisant passer à false.

void empty_square(Square square) {

    if (square.creation_point) {
        for (int i(g_max - 1 - square.y_) ; 
							i > g_max - 1 - (square.y_ + square.size_) ; i--) {
            for (int j(square.x_)  ; j < square.x_ + square.size_ ; j++){
                set_case(i, j, false);
            }
        }
    }
    else
    {
        int k = square.size_ / 2 ;
        int x = square.x_ - k;
        int y = square.y_ - k;

        for (int i(g_max-1 - y) ; i > g_max-1 - (y + square.size_) ; i--) {
            for (int j(x)  ; j < x + square.size_ ; j++){
                set_case(i, j, false);
            }
        }
    }
}

//Surcharge de empty_square.

void empty_square(int x, int y , int size, bool centre) {
    Square temp = {x, y, size, centre};
    empty_square(temp);
}

//Fonctions qui appellent graphic pour dessiner.

void draw_losange (int x, int y) {
	losange (x, y);
}

void draw_vide (int x, int y, int size, int colour_family) {
	vide (x, y, size, colour_family);
}

void draw_uniforme (int x, int y, uniform_size ant, int colour_family) {
	uniforme (x, y, ant, colour_family);
}

void draw_diagonale (int xc, int yc, int colour_family){
    for (int x(xc-1) ; x <= xc+1; x += 1) {    
        for (int y(yc-1) ; y <= yc+1 ; y += 1) {
            if ( ((x != xc) and (y != yc)) or (y==yc and x==xc) ) {
            choose_colour(colour_family, false);
			}
            else choose_colour(colour_family, true);
            uniforme(x, y, SMALL, -1);
        }
    }
}

void draw_grille (int xc, int yc, int colour_family) {
    for (int x(xc-1) ; x <= xc + 1 ; x += 1) {    
        for (int y(yc-1) ; y <= yc + 1 ; y += 1) {
            if ( ((x != xc) and (y != yc))) {
				choose_colour(colour_family, true); 
			}
            else choose_colour(colour_family, false);
            uniforme(x, y, SMALL, -1);
        }
    }
}
