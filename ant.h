#ifndef ANT_H
#define ANT_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "constantes.h"
#include "food.h"

class Ant {
	
protected :
	int x_;
	int y_;
	int age_;
	int my_anthill_;
	int size_;
	
	int goal_x;
	int goal_y;
	
	int final_goal_x;
	int final_goal_y;
	
	bool end_of_life_;
	
public :
	Ant(int x, int y, int age = 0, int my_anthill = 0);
	
	int getx();
	int gety();
	int get_size();
	int get_age();
	int get_my_anthill();
	bool get_end_of_life();
	
	virtual bool update_ant(int xg, int yg, int xa, int ya, int side, Ant* prey);
	virtual bool test_ant(int x, int y) const = 0;
	virtual void draw_ant() const = 0;
	virtual std::string ant_save_info() const = 0;
	virtual std::string ant_type_announcement() const = 0;
	
	bool is_home(int const& xah, int const& yah, int const& side);
	bool is_touching_home(int const& xah, int const& yah, int const& side);
	bool is_out_range(int const& xa, int const& ya, int const& side);
	bool death_cond(int const& xa, int const& ya, int const& side);
	
	void end_life();
	
	virtual ~Ant();
};


class Predator : public Ant {

public :
	Predator(int x, int y, int age, int my_anthill);
	
	virtual bool test_ant(int x, int y) const override;
	virtual void draw_ant() const override;
	virtual std::string ant_save_info() const override;
	virtual std::string ant_type_announcement() const override;
	virtual bool update_ant(int xg, int yg, int xa, int ya, int side, Ant* prey) override;
	
	void knight_movement(int const& xb, int const& yb, Ant* prey);
	bool on_prey(int const& x_pred, int const& y_pred, int const& xc, int const& yc);
	bool prey_hunted(Ant* prey);
		
	virtual ~Predator();
};


class Collector : public Ant {

private :
	Etat_collector has_food_;
	diag_family diagonal_family;
	bool no_objective;
		
public :
	Collector(int x, int y, int age, Etat_collector has_food, int my_anthill);

	virtual bool update_ant(int xg, int yg, int xa, int ya, int side, Ant* prey) override;
	virtual bool test_ant(int x, int y) const override;
	virtual void draw_ant() const override;
	virtual std::string ant_save_info() const override;
	virtual std::string ant_type_announcement() const override;
	
	int nb_of_collision(int const& xa, int const& ya, int const& xb, int const& yb);	
	void diagonal_movement(int& xa, int& ya, int const& x2, int const& y2);
	bool choose_closest_food(int& vx, int& vy, int& closest_index, int& closest_distance);
	void choose_correct_trajectory(int& vx, int& vy, int target_x, int target_y);
	void set_goal(int& closest_index, int& closest_distance, int& xg, int& yg, bool& at_home);
	void rectify_trajectory(int& proj_x, int& proj_y);
	bool test_capture(int const& xb, int const& yb);
	
	virtual ~Collector();
};


class Generator : public Ant {
	
public :
	Generator(int x, int y, int age, int my_anthill);
	virtual bool test_ant(int x, int y) const override;
	virtual std::string ant_save_info() const override;
	virtual std::string ant_type_announcement() const override;
	virtual bool update_ant(int xg, int yg, int xa, int ya, int side, Ant* prey) override;
	virtual void draw_ant() const override;
	void queen_movement();
	virtual ~Generator();
};




class Defensor : public Ant {
	
public :
	Defensor(int x, int y, int age, int my_anthill);
	
	virtual bool test_ant(int x, int y) const override;
	virtual void draw_ant() const override;
	virtual std::string ant_save_info() const override;
	virtual std::string ant_type_announcement() const override;
	virtual bool update_ant(int xg, int yg, int xa, int ya, int side, Ant* prey) override;
	void to_corner_movement(int const& xa, int const& ya, int const& side);
	bool in_corner(int const& xa, int const& ya, int const& side);
	bool intruder_in_contact(Ant* prey);
	
	void straight_movement(int const& xa, int const& ya, int const& side);
	bool in_position(int const& xa, int const& ya, int const& side);
	
	virtual ~Defensor();
};


#endif
