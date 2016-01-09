#include "draw.h"
#include <math.h>


int bresenham_criteria(int x, int y, int dx, int dy) {
    return 2*(x*dy + y*dx) + 2*dy - dx;
}

void draw_line_bresenham(Image* img, int Ax, int Ay, int Bx, int By) {
    int dx = abs(Bx - Ax), dx2 = 2*dx, dy = abs(By - Ay), dy2 = 2*dy;
    int incx = (Bx > Ax) ? 1 : -1,
        incy = (By > Ay) ? 1 : -1;
    int x = Ax, y = Ay;
    I_plot(img, x, y);
    if(dx > dy) {
        // Pente inférieure à 1 
        int c = dy2 - dx;
        for(int i = 1 ; i <= dx ; ++i) {
            if(c < 0)
                c += dy2;
            else {
                c += dy2 - dx;
                y += incy;
            }
            x += incx;
            I_plot(img, x, y);
        }
    }
    else {
        // Pente supérieure à 1 
        int c = dx2 - dy;
        for(int i = 1 ; i <= dy ; ++i) {
            if(c < 0)
                c += dx2;
            else {
                c += dx2 - dy;
                y += incx;
            }
            y += incy;
            I_plot(img, x, y);
        }
    }
}
