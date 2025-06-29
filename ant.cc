// Emilien Coudurier et Aksel Acar

// Emilien Coudurier : 75%
// Aksel Acar : 25%

#include "squarecell.h"
#include "message.h"
#include "ant.h"
#include "constantes.h"

using namespace std;

// Constructeur de la super classe Ant
Ant::Ant(int x, int y, int age, int my_anthill) : x_(x), y_(y) , age_(age), 
						my_anthill_(my_anthill) {		
	goal_x = x_;
	goal_y = y_;
	end_of_life_ = false;
	
}

Ant::~Ant() {
	empty_square(x_, y_, size_, false);
}
	
int Ant::get_my_anthill() { return my_anthill_; }
	
// Renvoie la coordonnée x de la fourmi
int Ant::getx() { return x_; }


// Renvoie la coordonnée y de la fourmi
int Ant::gety() { return y_; }


// Renvoie la taille de la fourmi
int Ant::get_size() { return size_; }


// Renvoie l'age de la fourmi
int Ant::get_age() { return age_; }
	
bool Ant::get_end_of_life() { return end_of_life_; }
	
bool Ant::update_ant(int xg, int yg, int xa, int ya, int side, Ant* prey) {
	age_++; 
	return false;
}

void Ant::end_life() { end_of_life_ = true; }


bool Ant::is_home(int const& xah, int const& yah, int const& side) {
	bool x_cond_min, x_cond_max, y_cond_min, y_cond_max;
	
	if (ant_type_announcement() == "# collectors") 
								return is_touching_home(xah, yah, side);
	
	x_cond_min = x_-(size_/2) > xah;

	x_cond_max = x_+(size_/2) < xah+side-1;

	y_cond_min = y_-(size_/2) > yah;

	y_cond_max = y_+(size_/2) < yah+side-1;

	if (x_cond_min and x_cond_max and y_cond_min and y_cond_max) return true;
	else return false;
}

bool Ant::is_touching_home(int const& xah, int const& yah, int const& side) {
	bool x_cond_min, x_cond_max, y_cond_min, y_cond_max;
	
	x_cond_min = x_+(size_/2) >= xah-1;

	x_cond_max = x_-(size_/2) <= xah+side;

	y_cond_min = y_+(size_/2) >= yah-1;

	y_cond_max = y_-(size_/2) <= yah+side;

	if (x_cond_min and x_cond_max and y_cond_min and y_cond_max) return true;
	else return false;
}

// Constructeur de la sous classe Predator qui teste si celui ci est
// correctement initialisé
Predator::Predator(int x, int y, int age, int my_anthill) :
									Ant(x, y, age, my_anthill) { 
	size_ = sizeP;
}

Predator::~Predator() {}

void Predator::draw_ant() const {
	draw_uniforme(x_, y_, SMALL, my_anthill_);
}

string Predator::ant_type_announcement() const {
	return "# predators";
}

string Predator::ant_save_info() const {
	string total("");
	total += "\t" + to_string(x_) + " " + to_string(y_) + " " + to_string(age_);
	return total;
}

bool Predator::test_ant(int x, int y) const {
	if (!(set_square(x, y, 1, true))) {
		cout << message::predator_overlap(x, y);
		return false;
	}
	return true;
}


void Predator::knight_movement(int const& xb, int const& yb, Ant* prey) {
	int potential_x(x_);
	int potential_y(y_);
	int x_prio(2), y_prio(1);
	
	if (prey->get_size() != 5) empty_square(xb, yb, prey->get_size(), false);
	
	if (abs(x_-xb) < abs(y_-yb)) {
		x_prio = 1;
		y_prio = 2;
	}
	
	if (x_ > xb) potential_x = x_- x_prio;
	else if (x_ < xb) potential_x = x_+ x_prio;
	if (y_ > yb) potential_y = y_- y_prio;
	else if (y_ < yb) potential_y = y_+ y_prio;
	
	if (test_square(potential_x, potential_y, size_, 1)) {
		x_ = potential_x;
		y_ = potential_y;
	} else {
		if (x_ > xb) potential_x = x_- y_prio;
		else if (x_ < xb) potential_x = x_+ y_prio;
		if (y_ > yb) potential_y = y_- x_prio;
		else if (y_ < yb) potential_y = y_+ x_prio;	
		
		if (test_square(potential_x, potential_y, size_, 1)) {
			x_ = potential_x;
			y_ = potential_y;
		}
	}
	
	if (prey->get_size() != 5) set_square(xb, yb, prey->get_size(), false);
	
}

bool Predator::on_prey(int const& x_pred, int const& y_pred, 
											int const& xc, int const& yc) {
	bool x_cond_min, x_cond_max, y_cond_min, y_cond_max;	
												
	x_cond_min = x_pred >= xc - 1;
	x_cond_max = x_pred <= xc + 1;
	y_cond_min = y_pred >= yc - 1;									
	y_cond_max = y_pred <= yc + 1;		

	if (x_cond_min and x_cond_max and y_cond_min and y_cond_max) return true;
	else return false;
}

bool Predator::prey_hunted(Ant* prey) {
	int xp(prey->getx());
	int yp(prey->gety());
	if (prey->get_size() == 1) {
		for (int i(-1) ; i < 2 ; i++) {
			for (int j(-1) ; j < 2 ; j++) {
				if (x_+i == xp and y_ == yp) {
					end_life();
					return true;
				}
			}	
		}
	} else if (prey->get_size() != 5) {
		for (int i(-1) ; i < 2 ; i++) {
			for (int j(-1) ; j < 2 ; j++) {
				if (on_prey(x_+i, y_+j, xp, yp)) return true;
			}	
		}
	}
	return false;
}

bool Predator::update_ant(int xg, int yg, int xa, int ya, int side, Ant* prey) {
	age_++;	
	knight_movement(prey->getx(), prey->gety(), prey);
	if (prey_hunted(prey)) prey->end_life();
	
	if (age_ >= bug_life) end_of_life_ = true; 
	
	return false;
}

// Constructeur de la sous classe Collector qui teste si celui ci est
// correctement initialisé
Collector::Collector(int x, int y, int age, Etat_collector has_food, 
	int my_anthill) : Ant(x, y, age, my_anthill), has_food_(has_food), 
	no_objective(false) { 
	size_ = sizeC;
	if ((x_%2) == (y_%2)) diagonal_family = EVEN;
	else diagonal_family = ODD;
}

Collector::~Collector() {}

void Collector::draw_ant() const {
	draw_diagonale(x_, y_, my_anthill_); 
	if (has_food_) draw_losange(x_, y_);
}

string Collector::ant_type_announcement() const {
	return "# collectors";
}

string Collector::ant_save_info() const {
	string total("");
	string has_food_str("false");
	if (has_food_) has_food_str = "true";	
	total += "\t" + to_string(x_) + " " + to_string(y_) + " "  
										+ to_string(age_) + " " + has_food_str;
	return total;
}

bool Collector::test_ant(int x, int y) const {
	int size_(sizeC);
	if (!(set_square(x, y, size_, false))) {
		int wrong_x(0), wrong_y(0);
		error_coordinates(x, y, size_, false, wrong_x, wrong_y);
		cout << message::collector_overlap(x, y, wrong_x, wrong_y);
		return false;
	}
	return true;
}

void Collector::diagonal_movement(int& xa, int& ya, int const& x2, int const& y2) {
	empty_square(xa, ya, size_, false);
	int potential_x(x_), potential_y(y_);

	if (!has_food_) food_squarecell_empty(diagonal_family);
	
	if (x2 < xa) potential_x = xa - 1;
	else if (x2 > xa) potential_x = xa + 1;
	
	if (y2 < ya) potential_y = ya - 1;
	else if (y2 > ya) potential_y = ya + 1;
	
	if (test_square(potential_x, potential_y, size_, false)) {
		xa = potential_x;
		ya = potential_y;
	}
	
	if (!has_food_) food_squarecell_set(diagonal_family);
	
	set_square(xa, ya, size_, false);
}

int Collector::nb_of_collision(int const& xa, int const& ya, 
												int const& xb, int const& yb) {
	int counter(0);
	int direction_x(1), direction_y(1);

	if (xa > xb) direction_x = -1;
	if (ya > yb) direction_y = -1;
	
	for (int i(1) ; i <= abs(xb-xa) ; i++) {
		if (xa+i*direction_x > 126 or xa+i*direction_x < 1 
						or ya+i*direction_y > 126 or ya+i*direction_y < 1) return 100;
		if (!test_square(xa+i*direction_x, ya+i*direction_y, size_, false)) counter++;
	}
	
	if (xb > final_goal_x) direction_x = -1;
	else direction_x = 1;
	if (yb > final_goal_y) direction_y = -1;
	else direction_y = 1;
	
	for (int i(1) ; i <= abs(xb-final_goal_x) ; i++) {
		if (xb+i*direction_x > 126 or xb+i*direction_x < 1
						or yb+i*direction_y > 126 or yb+i*direction_y < 1) return 100;
		if (!test_square(xb+i*direction_x, yb+i*direction_y, size_, false))	counter++;
	}
	return counter;
}


bool Collector::choose_closest_food(int& vx, int& vy, int& closest_index, 
															int& closest_distance) {
	int potential_food_counter(0);
	for (unsigned int i(0) ; i < get_food_number() ; i++) {
		if (diagonal_family == food_diag_family(i)) {
			potential_food_counter++;
			vx = food_x(i) - x_;
			vy = food_y(i) - y_;
			if (closest_distance > max(abs(vx), abs(vy))) {
				closest_distance = max(abs(vx), abs(vy));
				closest_index = i;
			}
		}
	}
	if (potential_food_counter == 0) return false;
	else return true;
}

void Collector::rectify_trajectory(int& proj_x, int& proj_y) {
	int direction_x(1), direction_y(1), diff(0);
	if (x_ > proj_x) direction_x = -1;
	if (y_ > proj_y) direction_y = -1;
	
	if (x_ == 1 or x_ == 126 or y_ == 1 or y_ == 126) {
		if (proj_x < 1) proj_x = 2 - proj_x;
		if (proj_y < 1) proj_y = 2 - proj_y;
		if (proj_x > 126) proj_x = 252 - proj_x;
		if (proj_y > 126) proj_y = 252 - proj_y;
	} else {
		if (proj_x <= 1) {
			diff = proj_x - 1;
			proj_x += direction_x*diff;
			proj_y += direction_y*diff;
		} else if (proj_x >= 126) {
			diff = proj_x - 126;
			proj_x -= direction_x*diff;
			proj_y -= direction_y*diff;
		}
		
		if (proj_y <= 1) {
			diff = proj_y - 1;
			proj_x += direction_x*diff;
			proj_y += direction_y*diff;
		} else if (proj_y >= 126) {
			diff = proj_y - 126;
			proj_x -= direction_x*diff;
			proj_y -= direction_y*diff;
		}		
	}
}

void Collector::choose_correct_trajectory(int& vx, int& vy, 
											int target_x, int target_y) {
	int proj_x1(0);
	int proj_y1(0);
	int proj_x2(0);
	int proj_y2(0);
	int t1(0);
	int t2(0);
	
	if (abs(vx) == abs(vy)) {
		goal_x = final_goal_x;
		goal_y = final_goal_y;
	} else if (vx != vy) {
		t1 = (vx+vy)/2;
		t2 = (vx-vy)/2;
		proj_x1 = target_x - t1;
		proj_y1 = target_y - t1;
		
		proj_x2 = target_x - t2;
		proj_y2 = target_y + t2;
		
		if ((proj_x1 >= 127 or proj_x1 <= 0) or 
				(proj_y1 >= 127 or proj_y1 <= 0)) rectify_trajectory(proj_x1, proj_y1);
		if ((proj_x2 >= 127 or proj_x2 <= 0) or 
				(proj_y2 >= 127 or proj_y2 <= 0)) rectify_trajectory(proj_x2, proj_y2);
		
		if (nb_of_collision(x_, y_, proj_x1, proj_y1) 
								< nb_of_collision(x_, y_, proj_x2, proj_y2)) {
			goal_x = proj_x1;
			goal_y = proj_y1;
		} else {
			goal_x = proj_x2;
			goal_y = proj_y2;
		}
	}
}

bool Collector::test_capture(int const& xb, int const& yb) {
	bool xcond, ycond;
	xcond = (xb >= x_ - size_/2) and (xb <= x_ + size_/2);
	ycond = (yb >= y_ - size_/2) and (yb <= y_ + size_/2);
	if (xcond and ycond) return true;
	else return false;
}

void Collector::set_goal(int& closest_index, int& closest_distance,
										int& xg, int& yg, bool& at_home) {
	int vx(0);
	int vy(0);
	if (has_food_ == EMPTY) {
		if (choose_closest_food(vx, vy, closest_index, closest_distance)) {
			vx = food_x(closest_index) - x_;
			vy = food_y(closest_index) - y_;
			final_goal_x = food_x(closest_index);
			final_goal_y = food_y(closest_index);
			no_objective = false;
			choose_correct_trajectory(vx, vy, food_x(closest_index), 
													food_y(closest_index));
		} else {
			if ((xg%2 == yg%2) != (x_%2 == y_%2)) xg--;
			final_goal_x = xg;
			final_goal_y = yg;
			vx = xg - x_;
			vy = yg - y_;
			choose_correct_trajectory(vx, vy, xg, yg);
			no_objective = true;
		}
	} else if (!at_home) {
		if ((xg%2 == yg%2) != (x_%2 == y_%2)) xg--;
		final_goal_x = xg;
		final_goal_y = yg;
		vx = xg - x_;
		vy = yg - y_;
		choose_correct_trajectory(vx, vy, xg, yg);
	}
}

bool Collector::update_ant(int xg, int yg, int xa, int ya, int side, Ant* prey) {
	age_++;		
	int closest_index(0);
	int closest_distance(128); 		// initialized with 128, which
			// is the furthest distance betwteen a food and a collector possible 
	int vx(0);
	int vy(0);
	bool food_success(false);
	bool at_home(is_home(xa, ya, side));
	food_success = choose_closest_food(vx, vy, closest_index, closest_distance);
	if (food_success or has_food_) {
		if ((x_ == goal_x and y_ == goal_y and !(at_home and has_food_)) 
				or test_capture(food_x(closest_index), food_y(closest_index))
				or no_objective) {
			set_goal(closest_index, closest_distance, xg, yg, at_home);
		}
	}
	
	if (age_ >= bug_life) {
		end_of_life_ = true;
		if (has_food_) Food food(x_, y_);
	}
	
	diagonal_movement(x_, y_, goal_x, goal_y);
	
	at_home = is_home(xa, ya, side);
	
	if (food_success) {
		if (test_capture(food_x(closest_index), food_y(closest_index))) {
			delete_food(closest_index);
			has_food_ = LOADED;
			set_goal(closest_index, closest_distance, xg, yg, at_home);
		}
	}
	if (at_home and has_food_) {
		has_food_ = EMPTY;
		set_goal(closest_index, closest_distance, xg, yg, at_home);
		return true;
	}
	return false;
}

// Constructeur de la sous classe Generator qui teste si celui ci est
// correctement initialisé
Generator::Generator(int x, int y, int age, int my_anthill) : 
								Ant(x, y, age, my_anthill) { 
	size_ = sizeG;
}

Generator::~Generator() {}

void Generator::draw_ant() const {
	draw_uniforme(x_, y_, BIG, my_anthill_+1);
}

string Generator::ant_save_info() const {
	//return "Wrong ant selected : 'Generator' doesn't need info for save.";
	return "";
}

string Generator::ant_type_announcement() const {
	//return "Wrong ant selected : 'Generator' shouldn't be announced.";
	return "";
}

bool Generator::test_ant(int x, int y) const {
	int size_(sizeG);
	if (!(set_square(x, y, size_, false))) {
		int wrong_x(0), wrong_y(0);
		error_coordinates(x, y, size_, false, wrong_x, wrong_y);
		cout << message::generator_overlap(x, y, wrong_x , wrong_y);
		return false;
	}
	return true;
}

void Generator::queen_movement() {
	int potential_x(x_), potential_y(y_);
	empty_square(x_, y_, size_, false);
	
	if (((x_%2) == (y_%2)) != ((goal_x%2) == (goal_y%2))) goal_x--;
	
	if (goal_x == x_ ) {
		if (goal_y < y_) potential_y = y_ - 1;
		else if (goal_y > y_) potential_y = y_ + 1;
	}
	
	if (goal_y == y_ ) {
		if (goal_x < x_) potential_x = x_ - 1;
		else if (goal_x > x_) potential_x = x_ + 1;
	}
	
	if (goal_x < x_) potential_x = x_ - 1;
	else if (goal_x > x_) potential_x = x_ + 1;
	
	if (goal_y < y_) potential_y = y_ - 1;
	else if (goal_y > y_)potential_y = y_ + 1;
	
	
	if (test_square(potential_x, potential_y, size_, false)) {
		x_ = potential_x;
		y_ = potential_y;
	}	
	
	set_square(x_, y_ , size_, false);
}

bool Generator::update_ant(int xg, int yg, int xa, int ya, int side, Ant* prey) {
	age_++ ; 
	int potential_x(x_), potential_y(y_);
	goal_x = xa + 3;
	goal_y = ya + 3;
	empty_square(x_, y_, size_, false);
	
	if (is_out_range(xa, ya, side))	end_of_life_ = true;
	
	if (!is_home(xa, ya, side) and !end_of_life_) {
		if (x_-2==xa) potential_x += 1;
		if (x_+2==xa+side-1) potential_x -= 1;
		if (y_-2==ya) potential_y += 1;
		if (y_+2==ya+side-1) potential_y -= 1;
		if (test_square(potential_x, potential_y, size_, false)) {
			x_ = potential_x;
			y_ = potential_y;
		} else end_of_life_ = true;
	} else if (end_of_life_) return false;
	
	set_square(x_, y_ , size_, false);
	
	queen_movement();
	
	return false ;
}
// Constructeur de la sous classe Defensor qui teste si celui ci est
// correctement initialisé
Defensor::Defensor(int x, int y, int age, int my_anthill) : 
								Ant(x, y, age, my_anthill) { 
	size_ = sizeD;
}

Defensor::~Defensor() {}

void Defensor::draw_ant() const {
	draw_grille(x_, y_, my_anthill_);
}

string Defensor::ant_type_announcement() const {
	return "# defensors";
}

string Defensor::ant_save_info() const {
	string total("");
	total += "\t" + to_string(x_) + " " + to_string(y_) + " " + to_string(age_);
	return total;
}

bool Defensor::test_ant(int x, int y) const {
	int size_(sizeD);
	if (!(set_square(x, y, size_, false))) {
		int wrong_x(0), wrong_y(0);
		error_coordinates(x, y, size_, false, wrong_x, wrong_y);
		cout << message::defensor_overlap(x, y, wrong_x, wrong_y);
		return false;
	}
	return true;
}

bool Defensor::in_corner(int const& xa, int const& ya, int const& side) {
	bool x_cond, y_cond;
	
	x_cond = x_-size_/2 == xa+1 or x_+size_/2 == xa+side-2;
	y_cond = y_-size_/2 == ya+1 or y_+size_/2 == ya+side-2;
	
	if(x_cond and y_cond) return true;
	else return false;
}

bool Defensor::in_position(int const& xa, int const& ya, int const& side) {
	bool x_cond, y_cond;
	
	x_cond = x_-size_/2 == xa+1 or x_+size_/2 == xa+side-2;
	y_cond = y_-size_/2 == ya+1 or y_+size_/2 == ya+side-2;
	
	if(x_cond or y_cond) return true;
	else return false;
}

bool Ant::death_cond(int const& xa, int const& ya, int const& side) {
	bool bot_left, bot_right, top_left, top_right;
	int potential_x(x_), potential_y(y_);
	
	bot_left = (x_-size_/2 == xa) and (y_-size_/2 == ya);
	bot_right = (x_+size_/2 == xa+side-1) and (y_-size_/2 == ya);
	top_left = (x_-size_/2 == xa) and (y_+1 == ya+side-1);
	top_right = (x_+size_/2 == xa+side-1) and (y_+size_/2 == ya+side-1);
	
	if (bot_left or bot_right or top_left or top_right) return true;
	else if (!is_home(xa, ya, side)) {
		empty_square(x_, y_, size_, false);
		if (x_-size_/2 == xa) potential_x += 1;
		else if (x_+size_/2 == xa+side-1) potential_x -= 1;
		else if (y_-size_/2 == ya) potential_y += 1;
		else if (y_+size_/2 == ya+side-1) potential_y -= 1;
		if (test_square(potential_x, potential_y, size_, false)) {
			x_ = potential_x;
			y_ = potential_y;
			return false;
		} else {
			set_square(x_, y_, size_, false);
			return true;
		}
	}
	return false;
}

void Defensor::to_corner_movement(int const& xa, int const& ya, int const& side) {
	bool touching_side(true);
	int potential_x(x_), potential_y(y_);
	empty_square(x_, y_, size_, false);
		
	if (!end_of_life_) {
		if (x_-size_/2 == xa+1) touching_side = false;
		else if (x_+size_/2 == xa+side-2) touching_side = false;
		
		switch (touching_side) {
			case (false) :
				if (abs(y_-ya) < abs(y_-ya+side-1)) potential_y -= 1;
				else potential_y += 1;
			break;
			case (true) :
				if (abs(x_-xa) < abs(x_-xa+side-1)) potential_x += 1;
				else potential_x -= 1;
			break;
		}
		
		if(test_square(potential_x, potential_y, size_, false)) {
			x_ = potential_x;
			y_ = potential_y;
		} 
		
		set_square(x_, y_, size_, false);
	}
}

void Defensor::straight_movement(int const& xa, int const& ya, int const& side) {
	int potential_x(x_), potential_y(y_);
	bool prior_direction(true);
	int x_dist, y_dist;
	empty_square(x_, y_, size_, false);
	
	if (!end_of_life_) {
		if (abs(x_-xa) < abs(x_-xa+side-1)) x_dist = abs(x_-xa);
		else x_dist = abs(x_-xa+side-1);
		if (abs(y_-ya) < abs(y_-ya+side-1)) y_dist = abs(y_-ya);
		else y_dist = abs(y_-ya+side-1);
		
		if (x_dist > y_dist) prior_direction = false;
		
		if(prior_direction and abs(xa-x_) < abs(xa+side-1-x_)) potential_x = x_ - 1;
		else if(prior_direction) potential_y = y_ + 1;
		else if (abs(ya-y_) < abs(ya+side-1-y_)) potential_y = y_ - 1;
		else potential_x = x_ + 1;
		
		if (test_square(potential_x, potential_y, size_, false)) {
			x_ = potential_x;
			y_ = potential_y;
		}
		
		set_square(x_, y_, size_, false);
	}
}


bool Defensor::intruder_in_contact(Ant* prey) {
	
	if (prey->get_size() == 1) return false;
	
	int xp(prey->getx()), yp(prey->gety());
	bool x_cond_min, x_cond_max, y_cond_min, y_cond_max;	
												
	x_cond_min = x_-2 <= xp + 1;
	x_cond_max = x_+2 >= xp - 1;
	y_cond_min = y_-2 <= yp + 1;							
	y_cond_max = y_+2 >= yp - 1;

	if (x_cond_min and x_cond_max and y_cond_min and y_cond_max) return true;
	else return false;
}

bool Ant::is_out_range(int const& xa, int const& ya, int const& side) {
	bool x_cond_min, x_cond_max, y_cond_min, y_cond_max;	
							
	int rectification(0);
	if (size_ == 5) rectification = 1;
												
	x_cond_min = x_+rectification >= xa + side - 1;
	x_cond_max = x_-rectification <= xa;
	y_cond_min = y_+rectification >= ya + side - 1;
	y_cond_max = y_-rectification <= ya;

	if (x_cond_min or x_cond_max or y_cond_min or y_cond_max) return true;
	else return false;
}


bool Defensor::update_ant(int xg, int yg, int xa, int ya, int side, Ant* prey) {
	age_++;
	if (is_out_range(xa, ya, side) or death_cond(xa, ya, side)) end_of_life_ = true;
	
	if (end_of_life_ == false) {
		if(!in_position(xa, ya, side)) straight_movement(xa, ya, side);
		else if(!in_corner(xa, ya, side)) to_corner_movement(xa, ya, side);
		if (age_ >= bug_life) end_of_life_ = true;
	}
	
	if (intruder_in_contact(prey)) prey->end_life();

	return false;
}
