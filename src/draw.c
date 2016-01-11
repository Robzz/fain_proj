#include "draw.h"
#include <cmath>
#include <functional>

int bresenham_criteria(int x, int y, int dx, int dy) {
    return 2*(x*dy + y*dx) + 2*dy - dx;
}

void draw_line_bresenham(Image* img, int Ax, int Ay, int Bx, int By) {
    int dx = abs(Bx - Ax), dx2 = 2*dx, dy = abs(By - Ay), dy2 = 2*dy;
    int incx = (Bx > Ax) ? 1 : -1,
        incy = (By > Ay) ? 1 : -1;
    int x = Ax, y = Ay;
    img->plot(x, y);
    if(dx > dy) {
        // Pente inférieure à 1 
        int c = dy2 - dx;
        for(int i = 1 ; i <= dx ; ++i) {
            if(c < 0)
                c += dy2;
            else {
                c += dy2 - dx2;
                y += incy;
            }
            x += incx;
            img->plot(x, y);
        }
    }
    else {
        // Pente supérieure à 1 
        int c = dx2 - dy;
        for(int i = 1 ; i <= dy ; ++i) {
            if(c < 0)
                c += dx2;
            else {
                c += dx2 - dy2;
                x += incx;
            }
            y += incy;
            img->plot(x, y);
        }
    }
}

void draw_line_bresenham(Image* img, Line const& l) {
    draw_line_bresenham(img, l.p1().x(), l.p1().y(), l.p2().x(), l.p2().y());
}

void draw_circle_bresenham(Image* img, int Ox, int Oy, int radius) {
    int xc = Ox, yc = Oy, x = 0, y = radius, c = 3 - 2*radius;
    std::function<void(int, int)> plot = [&] (int x, int y) { img->plot(x + xc, y + yc); };
    plot(x, y); plot(x, -y);
    plot(y, x); plot(y, -x);
    while(x < y) {
        if(c > 0) {
            c += 4*x - 4*y + 10;
            --y;
        }
        else {
            c += 4*x + 6;   
        }
        ++x;
        plot( x, y); plot( x, -y);
        plot(-x, y); plot(-x, -y);
        plot( y, x); plot( y, -x);
        plot(-y, x); plot(-y, -x);
    }
}

void draw_circle_bresenham(Image* img, Circle const& c) {
    draw_circle_bresenham(img, c.center().x(), c.center().y(), c.radius());    
}

void draw_polygon(Image* img, Polygon const& p) {
    for(auto it = p.begin() ; it+1 != p.end() ; ++it) {
        draw_line_bresenham(img, (*it).x(), (*it).y(), (*(it+1)).x(), (*(it+1)).y());
    }
    draw_line_bresenham(img, p.first().x(), p.first().y(), p.last().x(), p.last().y());
}

void seed_fill_recursive(Image* img, int x, int y, Color old, Color _new) {
    if(img->color_at(x, y) == old) {
        img->plot(x, y, _new);
        seed_fill_recursive(img, x+1, y    , old, _new);
        seed_fill_recursive(img, x-1, y    , old, _new);
        seed_fill_recursive(img, x  , y + 1, old, _new);
        seed_fill_recursive(img, x  , y - 1, old, _new);
    }
}

