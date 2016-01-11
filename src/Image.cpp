/*====================================================*\
  Vendredi 8 novembre 2013
  Arash HABIBI
  Image.c
\*====================================================*/

#include "Image.h"

Color::Color(float r, float g, float b) :
    _red(r),
    _green(g),
    _blue(b)
{ }

float Color::red() const { return _red; }
float Color::green() const { return _green; }
float Color::blue() const { return _blue; }

bool Color::operator==(Color const& other) {
    return (_red == other._red && _green == other._green && _blue == other._blue);
}

//------------------------------------------------------------------------

void C_check(Color c, char *message)
{
        fprintf(stderr,"%s : %f %f %f\n",message,c.red(),c.green(),c.blue());
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

Image::Image(int width, int height) :
    m_width(width),
    m_height(height),
    m_xzoom(0),
    m_yzoom(0),
    m_xoffset(0),
    m_yoffset(0),
    m_zoom(1),
    m_current_color(Color(255,255,255)),
    m_buffer(new Color*[width])
{
    for(int x=0;x<width;x++)
    m_buffer[x] = new Color[height];
}

//------------------------------------------------------------------------

//-----

static int _isPpm(char *imagefilename)
{
        FILE *imagefile;
        imagefile = fopen(imagefilename,"r");
        if(imagefile==NULL) {perror(imagefilename); exit(1); }

        else
        {
                int c1 = fgetc(imagefile);
                int c2 = fgetc(imagefile);
                fclose(imagefile);

                if((c1=='P')&&(c2=='6'))        return 1;
                else                                            return 0;
        }
}

//-----

Image* Image::read(char *imagefilename) {
    Image *img;
    char command[100];

    if(_isPpm(imagefilename)) 
        sprintf(command,"cp %s input.ppm",imagefilename);
    else
        sprintf(command,"convert %s input.ppm",imagefilename);

    int stat = system(command);
    if(stat!=0) {
        fprintf(stderr,"Convert : %s -> input.ppm impossible conversion.\n", imagefilename);
        exit(1);
    }
    else {
        Ppm ppm = PPM_nouv("input.ppm", PPM_LECTURE);
        system("rm input.ppm");

        fprintf(stderr,"%d x %d\n",PPM_largeur(ppm),PPM_hauteur(ppm));

        if(ppm!=NULL) {
            img = new Image(PPM_largeur(ppm),PPM_hauteur(ppm));
            int nb_bits=ppm->_nb_bits;
            int valmax = ppm->_valmax;

            int nb_pixels = img->width() * img->height();

            if(nb_bits <= 8) {
                unsigned char *donnees = (unsigned char*)calloc(3*nb_pixels,sizeof(unsigned char));
                PPM_lectureDonneesChar(ppm, donnees);

                for(int y=0 ; y < img->height() ; ++y)
                for(int x=0 ; x < img->width() ; ++x) {
                    int indice = (img->height() - y) * img->width() + x;
                    Color c((1.0*donnees[3*indice  ])/valmax,
                            (1.0*donnees[3*indice+1])/valmax,
                            (1.0*donnees[3*indice+2])/valmax);
                    
                    img->plot(x, y, c);
                    // _plot(img,x,y,c);
                }
            }
            else {
                unsigned short *donnees = (unsigned short*)calloc(3*nb_pixels,sizeof(unsigned short));
                PPM_lectureDonneesShort(ppm, donnees);
                for(int y = 0 ; img->height() ; ++y)
                for(int x = 0 ; img->width() ; ++x) {
                    int indice = (img->height() - y) * img->width() + x;
                    Color c((1.0*donnees[3*indice  ])/valmax,
                            (1.0*donnees[3*indice+1])/valmax,
                            (1.0*donnees[3*indice+2])/valmax);
                    img->m_buffer[x][y] = c;
                }
            }
            PPM_fermeture(ppm);
            return(img);
        }
        else
            return(NULL);
    }
}

//------------------------------------------------------------------------

void Image::fill(Color c) {
    int x,y;
    for(x=0 ; x < m_width ; ++x)
    for(y=0 ; y < m_height; ++y)
        m_buffer[x][y]=c;
}

//------------------------------------------------------------------------

void Image::checker(Color c1, Color c2, int step) {
    for(int x=0 ; x < m_width ; ++x)
    for(int y=0 ; y < m_height ; ++y) {
        int n_x = x/step;
        int n_y = y/step;
        if(( n_x +n_y) % 2 == 0)
            plot(x,y,c1);
        else 
            plot(x,y,c2);
    }
}

//------------------------------------------------------------------------

void Image::changeColor(Color c) {
    m_current_color = c;
}

//------------------------------------------------------------------------

void Image::plot(int x, int y) {
    if((x >= 0) && (x < m_width) &&
       (y >= 0) && (y < m_height))
        m_buffer[x][y] = m_current_color;
    else {
        fprintf(stderr,"I_plot : ERROR !!!\n");
        fprintf(stderr,"x (=%d) must be in the [%d,%d] range and\n", x, 0, m_width);
        fprintf(stderr,"y (=%d) must be in the [%d,%d] range\n", y, 0, m_height);
    }
}

//------------------------------------------------------------------------

void Image::plot(int x, int y, Color c)
{
    if((x >= 0) && (x < m_width) &&
       (y >= 0) && (y < m_height))
        m_buffer[x][y] = c;
    else {
        fprintf(stderr,"I_plotColor : ERROR !!!\n");
        fprintf(stderr,"x (=%d) must be in the [%d,%d] range and\n", x, 0, m_width);
        fprintf(stderr,"y (=%d) must be in the [%d,%d] range\n", y, 0, m_height);
    }
}

int Image::width() const { return m_width; }
int Image::height() const { return m_height; }
int Image::Image::x_zoom() const { return m_xzoom; }
int Image::y_zoom() const { return m_yzoom; }
int Image::x_offset() const { return m_xoffset; }
int Image::y_offset() const { return m_yoffset; }
int Image::zoom() const { return m_zoom; }
Color Image::current_color() const { return m_current_color; }

Color Image::color_at(int x, int y) const { return m_buffer[x][y]; }

//------------------------------------------------------------------------
// Changement de repère

static void _windowToImage(Image *img, int xwin, int ywin, int *ximg, int *yimg) {
    *ximg = img->x_offset() + img->x_zoom() + (xwin - img->x_zoom()) / img->zoom();
    *yimg = img->y_offset() + img->y_zoom() + (ywin - img->y_zoom()) / img->zoom();
}

//-----
// Changement de repère inverse
/*
static void _imageToWindow(Image *img, int ximg, int yimg, int *xwin, int *ywin)
{

        *xwin = img->_xoffset + img->x_zoom() + (ximg-img->x_zoom()-img->_xoffset) * img->zoom();
        *ywin = img->_yoffset + img->y_zoom() + (yimg-img->y_zoom()-img->_yoffset) * img->zoom();
}
*/
//-----

void Image::focusPoint(int xwin, int ywin) {
        int dx = xwin - m_xzoom;
        int dy = ywin - m_yzoom;
        m_xoffset -= dx*(1-1.0/m_zoom);
        m_yoffset -= dy*(1-1.0/m_zoom);
        m_xzoom = xwin;
        m_yzoom = ywin;
}

//------------------------------------------------------------------------

void Image::zoomInit() {
    m_xoffset = 0;
    m_yoffset = 0;
    m_zoom = 1.0;
}

//------------------------------------------------------------------------

void Image::zoom(double zoom_coef) {
    m_zoom *= zoom_coef;
}

//------------------------------------------------------------------------

void Image::move(int x, int y) {
    m_xoffset += x;
    m_yoffset += y;
}

//------------------------------------------------------------------------

void Image::draw() {
    glBegin(GL_POINTS);
    int ximg, yimg;
    for(int xwin=0 ; xwin < m_width ; ++xwin)
    for(int ywin=0 ; ywin < m_height; ++ywin) {
        _windowToImage(this, xwin, ywin, &ximg, &yimg);
        Color c;
        if((ximg >= 0) && (ximg < m_width) &&
           (yimg >= 0) && (yimg < m_height))
            c = m_buffer[ximg][yimg];
        else
            c = Color(0,0,0);

        glColor3f(c.red(),c.green(),c.blue());
        glVertex2i(xwin,ywin);
    }
    glEnd();
}

//------------------------------------------------------------------------
