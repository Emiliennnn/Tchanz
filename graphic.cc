// Emilien Coudurier et Aksel Acar

// Emilien Coudurier : 60%
// Aksel Acar : 40%

#include "graphic.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

using namespace std;

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr) {
	ptcr = &cr;
}


void choose_colour(int colour_nb, bool lighter) {
	double main_colour(0.9);
	double secondary_colour(0.1);
	if (lighter) secondary_colour = 0.5;

	if (colour_nb%6 == 0) colour_nb = 6;
	if (colour_nb > 6) colour_nb = colour_nb%6;
	
	switch (colour_nb) {
		case 1 :
			(*ptcr)->set_source_rgb(main_colour, secondary_colour, secondary_colour);
		break;
		
		case 2 :
			(*ptcr)->set_source_rgb(secondary_colour, main_colour, secondary_colour);
		break;
		
		case 3 :
			(*ptcr)->set_source_rgb(secondary_colour, secondary_colour, main_colour);
		break;
		
		case 4 :
			(*ptcr)->set_source_rgb(main_colour-0.2 + secondary_colour, 
							main_colour-0.2 + secondary_colour, secondary_colour-0.1);
		break;
		
		case 5 :
			(*ptcr)->set_source_rgb(main_colour, secondary_colour, main_colour);
		break;
		
		case 6 :
			(*ptcr)->set_source_rgb(secondary_colour-0.1, main_colour-0.2 + 
								secondary_colour, main_colour-0.2 + secondary_colour);
		break;
	}
}

void draw_grid() {
	const double case_size = 500./128.;
	
	// changement de la couleur du fond en noir 
	(*ptcr)->set_source_rgb(0., 0., 0.);
	(*ptcr)->paint();

	// contour en blanc 
	(*ptcr)->set_source_rgb(1., 1., 1.);
	(*ptcr)->set_line_width(case_size);
	(*ptcr)->rectangle(case_size/2, case_size/2, 500-case_size, 500-case_size);
	(*ptcr)->stroke();
  
	// grille en gris
	(*ptcr)->set_source_rgb(0.5, 0.5, 0.5);
	(*ptcr)->set_line_width(.5);
  
	for (int x(0) ; x < 129 ; x += 1) {
		(*ptcr)->move_to(x*case_size, 0);
		(*ptcr)->line_to(x*case_size, 500); 
		(*ptcr)->stroke();
	}

	for (int y(0) ; y < 129 ; y += 1) {
		(*ptcr)->move_to(0, y*case_size);
		(*ptcr)->line_to(500, y*case_size); 
		(*ptcr)->stroke();
	}
}


void vide(int x, int y, int size, int colour_family) {

	choose_colour(colour_family, false);
	
    const double case_size = 500./128.;
    const double line_thickness = case_size/3;

    (*ptcr)->set_line_width(line_thickness);
    
    int x_rect = x*case_size + case_size/2;
    int y_rect = y*case_size + case_size/2;
    int x_rect_size = size*case_size - case_size;
    int y_rect_size = size*case_size - case_size;
    
	(*ptcr)->rectangle(x_rect, y_rect, x_rect_size, y_rect_size);
	(*ptcr)->stroke();
}


void losange(int x, int y) {
	const double case_size = 500./128.;

	(*ptcr)-> set_source_rgb(1., 1., 1.);
	
    double cote;
    cote = sqrt((case_size*case_size)/2);
    (*ptcr)->set_line_width(cote); 
    
    (*ptcr)-> move_to(x*case_size + case_size/4, y*case_size + case_size/4);
    (*ptcr)-> line_to(x*case_size + 3*case_size/4, y*case_size + 3*case_size/4);

    (*ptcr)->stroke();
}


void uniforme(int x, int y, uniform_size size, int colour_family) {
	
	if (colour_family > 0) choose_colour(colour_family, false);
	const double case_size = 500./128.;
    
    switch(size) {
		case SMALL : 
			(*ptcr)->set_line_width(case_size);

			(*ptcr)-> move_to(x*case_size+case_size/2, y*case_size);
			(*ptcr)-> line_to(x*case_size+case_size/2, y*case_size+case_size);
    
			(*ptcr)->stroke();
		break; 
			
		case BIG : 
			(*ptcr)->set_line_width(5*case_size);

			(*ptcr)-> move_to(x*case_size+case_size/2, (y-2)*case_size);
			(*ptcr)-> line_to(x*case_size+case_size/2, (y+3)*case_size);
		
			(*ptcr)->stroke();
		break;
	}
}
