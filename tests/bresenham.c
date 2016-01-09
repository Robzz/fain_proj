#include "draw.h"
#include <stdio.h>

#include <assert.h>


int main(int argc, char** argv) {
    Image* img = I_new(640, 480);
    I_fill(img, C_new(0, 0, 0));
    I_changeColor(img, C_new(255, 255, 255));
    // Try horizontal lines, from left to right and right to left
    draw_line_bresenham(img, 100, 100, 200, 100);
    draw_line_bresenham(img, 200, 200, 100, 200);
    for(int i = 100 ; i <= 200 ; ++i) {
        assert(img->_buffer[i][100]._blue == 255);
        assert(img->_buffer[i][100]._red == 255);
        assert(img->_buffer[i][100]._green == 255);
        assert(img->_buffer[i][200]._blue == 255);
        assert(img->_buffer[i][200]._red == 255);
        assert(img->_buffer[i][200]._green == 255);
    }
    // Try vertical lines, from top to bottom and bottom to top 
    draw_line_bresenham(img, 100, 100, 100, 200);
    draw_line_bresenham(img, 200, 200, 200, 100);
    for(int i = 100 ; i <= 200 ; ++i) {
        assert(img->_buffer[100][i]._blue == 255);
        assert(img->_buffer[100][i]._red == 255);
        assert(img->_buffer[100][i]._green == 255);
        assert(img->_buffer[200][i]._blue == 255);
        assert(img->_buffer[200][i]._red == 255);
        assert(img->_buffer[200][i]._green == 255);
    }
}
