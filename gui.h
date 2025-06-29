#ifndef GUI_H
#define GUI_H
#include <iostream>
#include <fstream> 

#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>

#include <cairomm/context.h>

enum Simulation_state {STARTED, STOPPED};
enum Following_selection {NEXT, PREVIOUS, RESET};

struct Frame {	
	double ratio;
	double x_min;
	double x_max;
	double y_min;
	double y_max;

	int height;
	int width;
};

class MyArea : public Gtk::DrawingArea {
protected :
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	void adjust_frame();

private :
	Frame frame;

public :
	void refresh();
	MyArea();
	~MyArea();
};

class Gui : public Gtk::Window {
private :
	MyArea drawing_area;
	Simulation_state simulation_state;
	bool step;
	bool timer_added;
	bool disconnect; 
	bool bool_refresh;
	const int timeout_value;
	Following_selection selection;
	
protected :	
	
// --------------------	Boites principales
	Gtk::Box main_box;
	Gtk::Box left_box;
	Gtk::Box drawing_box;
	
// --------------------	Elements de la boite general
	Gtk::Box general_box;
	Gtk::Label general_title;
	Gtk::Button exit_button;
	Gtk::Button open_button;
	Gtk::Button save_button;
	Gtk::Button start_button;
	Gtk::Button step_button;
	
// --------------------	Elements de la boite info
	Gtk::VBox info_box;
	Gtk::Label info_title; 
	Gtk::Label food_number;
	
// --------------------	Elements de la boite fourmilliere
	Gtk::VBox anthill_box;
	Gtk::Label anthill_title; 
	Gtk::Button previous_button;
	Gtk::Button next_button;
	Gtk::Label selected_anthill;
	Gtk::Label nbc_nb;
	Gtk::Label nbd_nb;
	Gtk::Label nbp_nb;
	Gtk::Label anthill_food_nb;
	
public :	
	// ----------------------------------------------------	Constructeur
	Gui();
	
	// ----------------------------------------	méthodes on_button_cliked
	void on_button_clicked_exit();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_button_clicked_start();
	void on_button_clicked_step();
	void on_button_add_timer();
	void on_button_delete_timer();
	void on_button_clicked_previous();
	void on_button_clicked_next();
	void previous_next_action();
	void refresh_food_label();
	void refresh_anthill_label(int& id);
	void set_anthill_default_label();
	
	bool on_key_press_event(GdkEventKey * key_event);
	bool on_timeout();
};

#endif
