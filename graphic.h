#ifndef GTKMM_GRAPHIC_H
#define GTKMM_GRAPHIC_H

#include <iostream>

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm.h>

enum uniform_size { BIG, SMALL }; 

void choose_colour(int colour_nb, bool lighter);

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);
 
void draw_grid();

void vide(int x, int y, int size, int family_colour);

void losange(int x, int y);

void uniforme(int x, int y, uniform_size size, int family_colour);
  
#endif // GTKMM_GRAPHIC_H
