// Emilien Coudurier et Aksel Acar

// Emilien Coudurier : 80%
// Aksel Acar : 20%

#include "simulation.h"
#include "graphic.h"
#include "gui.h"

using namespace std;

static Frame default_frame = {1., 0., 500.,  0., 500.,  500, 500}; 

MyArea::MyArea() {};
MyArea::~MyArea() {};


static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr,
									Frame frame) {	
	// putting the visual reference in the center of the window
	// (making the grid centered with the window everytime we resize it)
	cr->translate(frame.width/2, frame.height/2);
  
	cr->scale(frame.width/(frame.x_max - frame.x_min), 
             -frame.height/(frame.y_max - frame.y_min));

	cr->translate(-(frame.x_min + frame.x_max)/2, -(frame.y_min + frame.y_max)/2);
}

void MyArea::refresh() {
	auto win = get_window();
	if(win) {
		Gdk::Rectangle r(0,0, get_allocation().get_width(), 
	                        get_allocation().get_height());
		win->invalidate_rect(r,false);
	}
}

void MyArea::adjust_frame() {
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
	
	frame.width  = width;
	frame.height = height;

    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > default_frame.ratio) {
	    frame.y_max = default_frame.y_max ;
	    frame.y_min = default_frame.y_min ;	
	  
	    double delta(default_frame.x_max - default_frame.x_min);
	    double mid((default_frame.x_max + default_frame.x_min)/2);
        
	    frame.x_max = mid + 0.5*(new_aspect_ratio/default_frame.ratio)*delta;
	    frame.x_min = mid - 0.5*(new_aspect_ratio/default_frame.ratio)*delta;		  	  
    } else {
	    frame.x_max = default_frame.x_max ;
	    frame.x_min = default_frame.x_min ;
	  	  
 	    double delta(default_frame.y_max - default_frame.y_min);
	    double mid((default_frame.y_max + default_frame.y_min)/2);
       
	    frame.y_max = mid + 0.5*(default_frame.ratio/new_aspect_ratio)*delta;
	    frame.y_min = mid - 0.5*(default_frame.ratio/new_aspect_ratio)*delta;		  	  
    }
}

void Gui::refresh_food_label() {
	string label("Number of Food : " + to_string(from_simulation_get_food_nb()));
	food_number.set_label(label);
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	
	adjust_frame();
	orthographic_projection(cr, frame);
	graphic_set_context(cr);
	draw_grid();
	draw_all();

	return true;
}

void Gui::on_button_clicked_exit() {
	exit(0);
}

bool Gui::on_key_press_event(GdkEventKey * key_pressed) {
	if(key_pressed->type == GDK_KEY_PRESS) {
		switch(gdk_keyval_to_unicode(key_pressed->keyval)) {
			case 's':
				on_button_clicked_start();
			break;
			case '1':
				on_button_clicked_step();
			break;
			case 'n':
				on_button_clicked_next();
			break;
			case 'p' :
				on_button_clicked_previous();
			break;
			case 'q' :
				on_button_clicked_exit();
			break;
		}
	}
	return Gtk::Window::on_key_press_event(key_pressed);
}


void Gui::on_button_clicked_save() {
	cout << "Saved !" << endl;
	
	Gtk::FileChooserDialog save_dialog_window("Choose a file : ", 
										Gtk::FILE_CHOOSER_ACTION_SAVE);

	save_dialog_window.set_transient_for(*this);
	save_dialog_window.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	save_dialog_window.add_button("_Save", Gtk::RESPONSE_OK);
	
	int answer = save_dialog_window.run();
	
	switch(answer) {
		case (Gtk::RESPONSE_OK): {
			ofstream outfile (save_dialog_window.get_filename());
			outfile << "# Save" << endl << "#" << endl << endl 
			<< "# nb food" << endl << from_simulation_get_food_nb() << endl << endl
			<< "# food" << endl << show_food() << endl
			<< anthill_nb() << " # nb_anthill" << endl;
			int anthill_total(anthill_nb());
			for (int i(1) ; i <= anthill_total ; i++) {
				if (anthill_save_info(i) != "dead") {
					outfile << "\t" << anthill_save_info(i) << endl;
				} else anthill_total++;
			}
			outfile.close();
		break;
		}
		
		case (Gtk::RESPONSE_CANCEL):
			cout << "Folder opening has been cancelled. " << endl;
		break;

		default:
			cout << "Unexpected button clicked." << endl;
		break;
    }
}

void Gui::on_button_clicked_start() {
	if (selection != RESET) {
		switch(simulation_state) {
			case (STARTED) :
				start_button.set_label("start");
				simulation_state = STOPPED;
				on_button_delete_timer();
			break;
			
			case (STOPPED) :
				start_button.set_label("stop");
				simulation_state = STARTED;
				on_button_add_timer();
			break;
		}
	} else if (get_from_cmd_opened() == false) {
		cout << "No file currently opened, please open a file" 
		<< " before starting the simulation." << endl;
	} else {
		food_number.set_label("Number of food : None");
		set_anthill_default_label();
		selection = RESET;
		previous_next_action();
		on_timeout();
		drawing_area.refresh();
		selection = NEXT;
		refresh_food_label();
		simulation_state = STOPPED;
		on_button_clicked_start();
	}
}

void Gui::on_button_add_timer() {
  if(!timer_added) {
	  Glib::signal_timeout().connect( sigc::mem_fun(*this, &Gui::on_timeout),
									  timeout_value );
	  timer_added = true;
	}
}


void Gui::on_button_delete_timer() {
	if (timer_added) {
		disconnect  = true;   
		timer_added = false;
	}
}

bool Gui::on_timeout() {
	static unsigned int val(1);
  
	if (selection == RESET) {
		val = 1;
		return false;
	}
  
  	drawing_area.refresh();

	if(disconnect) {
		disconnect = false;
		return false;
	}
   
	if (step) {
		cout << "This is simulation update number : " << val << endl;
		++val;
		simulation_update();
		refresh_food_label();
		bool_refresh = true;
		previous_next_action();
		step = false;
	} else {  
		cout << "This is simulation update number : " << val << endl;
		++val; 
		simulation_update();
		refresh_food_label();
		bool_refresh = true;
		previous_next_action();
	} 
  return true; 
}

void Gui::on_button_clicked_step() {
	if (selection == RESET and get_from_cmd_opened()) {
		food_number.set_label("Number of food : None");
		set_anthill_default_label();
		selection = RESET;
		previous_next_action();
		on_timeout();
		drawing_area.refresh();
		selection = NEXT;
		refresh_food_label();
		simulation_state = STOPPED;
		on_button_clicked_step();
	}
	if (simulation_state == STOPPED) {
		step = true;
		on_timeout();
	}
}

void Gui::set_anthill_default_label() {
	nbc_nb.set_label("");
	nbd_nb.set_label("");
	nbp_nb.set_label("");
	anthill_food_nb.set_label("");
	selected_anthill.set_label("None selected");
}

void Gui::refresh_anthill_label(int& id) {
				
	string selected_anthill_label("ID ");
	string nbc_label("nbC : ");
	string nbd_label("nbD : ");
	string nbp_label("nbP : ");
	string anthill_food_label("Total Food : ");
	
	selected_anthill_label += to_string(id-1) + " : ";
	nbc_label += to_string(from_simulation_get_nbc(id));
	nbd_label += to_string(from_simulation_get_nbd(id));
	nbp_label += to_string(from_simulation_get_nbp(id));
	anthill_food_label += to_string(from_simulation_get_total_food(id));
	
	nbc_nb.set_label(nbc_label);
	nbd_nb.set_label(nbd_label);
	nbp_nb.set_label(nbp_label);
	anthill_food_nb.set_label(anthill_food_label);
	selected_anthill.set_label(selected_anthill_label);
}

void Gui::previous_next_action() {
	if (anthill_nb() > 0) { 
		static int selected_anthill_index(-1);
		if (bool_refresh) {
			int id(selected_anthill_index+1);
			if (selected_anthill_index != -1) refresh_anthill_label(id);
			bool_refresh = false;
		} else {
			int dead_anthills(from_simulation_get_dead_anthills());
			if (selection == PREVIOUS) {
				if (selected_anthill_index >= 0) {
					do selected_anthill_index --;
					while (from_simulation_get_life(selected_anthill_index+1) 
										== true and selected_anthill_index >= 0);
					if (selected_anthill_index < 0) selected_anthill_index = -1;
				} else {
					selected_anthill_index = anthill_nb()+dead_anthills;
					do selected_anthill_index --;
					while (from_simulation_get_life(selected_anthill_index+1) 
										== true and selected_anthill_index >= 0);
					if (selected_anthill_index < 0) selected_anthill_index = -1;
				}	
			} else if (selection == NEXT) {
				if (selected_anthill_index < anthill_nb()+dead_anthills-1) {
					do selected_anthill_index ++;
					while (from_simulation_get_life(selected_anthill_index+1) == true
								and selected_anthill_index <= anthill_nb()+dead_anthills-1);
					if (selected_anthill_index > anthill_nb()+dead_anthills-1) 
														selected_anthill_index = -1;
				} else {
					selected_anthill_index = anthill_nb()+dead_anthills;
					do selected_anthill_index ++;
					while (from_simulation_get_life(selected_anthill_index+1) == true 
								and selected_anthill_index <= anthill_nb()+dead_anthills-1);
					if (selected_anthill_index > anthill_nb()+dead_anthills-1) 
														selected_anthill_index = -1;
				}
			} else selected_anthill_index = -1;
			if (selection == NEXT or selection == PREVIOUS) {
				if (selected_anthill_index < 0) set_anthill_default_label();
				else {
					int id(selected_anthill_index+1);
					refresh_anthill_label(id);
				}
			}
		}
	}
}


void Gui::on_button_clicked_previous() {
	if (anthill_nb() != 0) {
		selection = PREVIOUS;
		previous_next_action();
	}
}


void Gui::on_button_clicked_next() {
	if (anthill_nb() != 0) {
		selection = NEXT;
		previous_next_action();
	}
}



void Gui::on_button_clicked_open() {
	
	Gtk::FileChooserDialog open_dialog_window("Choose a file : ", 
										Gtk::FILE_CHOOSER_ACTION_OPEN);
	open_dialog_window.set_transient_for(*this);
	open_dialog_window.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	open_dialog_window.add_button("_Open", Gtk::RESPONSE_OK);
	int answer = open_dialog_window.run();
	
	switch(answer) 
	{
		case(Gtk::RESPONSE_OK):
			food_number.set_label("Number of food : None");
			set_anthill_default_label();
			if (timer_added) on_button_clicked_start();
			selection = RESET;
			previous_next_action();
			on_timeout();
			reset_simulation();
			drawing_area.refresh();
			if(!read_file(open_dialog_window.get_filename())) {
				reset_simulation();
				selection = RESET;
				previous_next_action();
				drawing_area.refresh();
			} else {
				selection = NEXT;
				refresh_food_label();
			}
		break;
			
		case(Gtk::RESPONSE_CANCEL):
			cout << "Folder opening has been cancelled. " << endl;
		break;
			
		default:
			cout << "Unexpected button clicked." << endl;
		break;
    }
}

// ------------------------------------------------	Constructuer de Gui
Gui::Gui() : simulation_state(STOPPED), step(false), timer_added(false),
			disconnect(false),  bool_refresh(false), timeout_value(100), 
			selection(RESET), main_box(Gtk::ORIENTATION_HORIZONTAL, 10), 
			left_box(Gtk::ORIENTATION_VERTICAL, 10),
			drawing_box(Gtk::ORIENTATION_VERTICAL, 0), 
			general_box(Gtk::ORIENTATION_VERTICAL, 0), general_title("General"),
			exit_button("exit"), open_button("open"), save_button("save"), 
			start_button("start"), step_button("step"), info_title("Info"), 
			food_number("Number of food : None"), anthill_title("Anthill"), 
			previous_button("Previous"), next_button("Next"), 
			selected_anthill("None selected"), nbc_nb(""), nbd_nb(""),
			nbp_nb(""), anthill_food_nb("") {	
	// --------------------------------------------	caracteristiques de la fenetre 
	set_title("Tchanz Simulation");
	set_border_width(10);

	// --------------------------------------------	Ajout des boites dans la boite
	// --------------------------------------------	principale
	add(main_box);
	main_box.pack_start(left_box, false, false);
	main_box.pack_start(drawing_box);
	
	// --------------------------------------------	Ajout de toutes les boites de 
	// --------------------------------------------	widgets dans la boite de gauche
	left_box.pack_start(general_box, false, false);
	left_box.pack_start(info_box, false, false);
	left_box.pack_start(anthill_box, false, false);
	
	// -------------------------------------------	Elements de la boite general
	general_box.pack_start(general_title, false,false);
	general_box.pack_start(exit_button,false,false);
	general_box.pack_start(open_button,false,false);
	general_box.pack_start(save_button,false,false);
	general_box.pack_start(start_button,false,false);
	general_box.pack_start(step_button,false,false);
	
	// -------------------------------------------	Elements de la boite info
	info_box.pack_start(info_title, false,false);
	info_box.pack_start(food_number, false,false);
	
	// -------------------------------------------	Elements de la boite fourmilliere
	anthill_box.pack_start(anthill_title, false,false);
	anthill_box.pack_start(previous_button, false,false);
	anthill_box.pack_start(next_button, false,false);
	anthill_box.pack_start(selected_anthill, false, false);
	anthill_box.pack_start(anthill_food_nb, false, false);
	anthill_box.pack_start(nbc_nb, false, false);
	anthill_box.pack_start(nbd_nb, false, false);
	anthill_box.pack_start(nbp_nb, false, false);
	
	// --------------------------------------------	Elements de la boite de dessin
	drawing_box.pack_start(drawing_area);
	drawing_area.set_size_request(500, 500);
	
	// -------------------------------------------	Connexion des boutons
	exit_button.signal_clicked().connect(sigc::mem_fun(*this, 
										&Gui::on_button_clicked_exit));
	open_button.signal_clicked().connect(sigc::mem_fun(*this, 
										&Gui::on_button_clicked_open));
	save_button.signal_clicked().connect(sigc::mem_fun(*this, 
										&Gui::on_button_clicked_save));
	start_button.signal_clicked().connect(sigc::mem_fun(*this, 
										&Gui::on_button_clicked_start));
	step_button.signal_clicked().connect(sigc::mem_fun(*this, 
										&Gui::on_button_clicked_step));
	previous_button.signal_clicked().connect(sigc::mem_fun(*this, 
										&Gui::on_button_clicked_previous));
	next_button.signal_clicked().connect(sigc::mem_fun(*this, 
										&Gui::on_button_clicked_next));
	
	show_all_children();
}
