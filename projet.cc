#include <iostream>

#include "simulation.h"
#include "gui.h"

int main(int argc, char* argv[]) 
{
	if (argc > 1) {
		if(argc != 2) exit(EXIT_FAILURE);
		read_file(argv[1]);
	}

	int fake_argc(1);

	auto app=Gtk::Application::create(fake_argc, argv, "Window");
	Gui gui;
	
	return app->run(gui);
}
