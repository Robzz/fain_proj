#ifndef DRAW_H
#define DRAW_H

#include "Image.h"
#include "vector.h"
#include <vector>

void draw_line_bresenham(Image* img, int Ax, int Ay, int Bx, int By);
void draw_circle_bresenham(Image* img, int Ox, int Oy, int radius);
void draw_polygon(Image* img, std::vector<Vec2f> const& pts);

void seed_fill_recursive(Image* img, int x, int y, Color old, Color _new);

#endif
