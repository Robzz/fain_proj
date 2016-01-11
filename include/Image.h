/*====================================================*\
  Vendredi 8 novembre 2013
  Arash HABIBI
  Image.h
\*====================================================*/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "Ppm.h"

//-----------------------------------------------------

class Color {
    public:
        Color(float r=0, float g=0, float b=0);
        float red() const;
        void set_red(float r);
        float green() const;
        void set_green(float g);
        float blue() const;
        void set_blue(float b);

        bool operator==(Color const& other);

    private:
        float _red, _green, _blue;
}; 

void C_check(Color c, char *message);

//-----------------------------------------------------

class Image {
    public:
        Image(int _width, int _height);
        Image(Image const& other);
        ~Image();
        static Image* read(char *ppmfilename);

        void fill       (Color c);
        void checker    (Color c1, Color c2, int step);

        void changeColor(Color c);
        void plot       (int x, int y);
        void plot       (int x, int y, Color c);

        void focusPoint (int x, int y);
        void zoomInit   ();
        void zoom       (double zoom_coef);
        void move       (int x, int y);

        void draw       ();

        int width() const;
        int height() const;
        int x_zoom() const;
        int y_zoom() const;
        int x_offset() const;
        int y_offset() const;
        int zoom() const;
        Color current_color() const;

        Color color_at(int x, int y) const;

    private:
	int m_width, m_height;
	int m_xzoom, m_yzoom;
	int m_xoffset, m_yoffset;
	double m_zoom;
	Color m_current_color;
	Color** m_buffer;

        Image& operator=(Image const& other);
};

#endif



