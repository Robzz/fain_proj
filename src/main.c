
/*===============================================================*\

    Vendredi 25 septembre 2013

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/

#include <cstdio>
#include <iostream>
#include <cmath>
#include <vector>

#include <GL/glut.h>
#include <GL/gl.h>

#include "Image.h"
#include "shapes.h"
#include "draw.h"
#include "vector.h"

typedef enum { LINES, CIRCLES, POLYGONS, FILL_RECURSIVE, FILL_SWEEP } Mode;
typedef enum { IDLE, DRAWING, EDIT } State;

Mode mode = LINES;
State state = IDLE;
Polygon p;
Line l = Line(Vec2i(), Vec2i());
Circle c = Circle(Vec2i(), 0);
Image *img, *canvas = NULL;

void overwrite_img() {
    delete img;
    img = canvas;
    canvas = NULL;
}

void new_canvas() {
    if(canvas)
        delete canvas;
    canvas = new Image(*img);
}

void change_mode(Mode m) {
    if(state == EDIT) {
        overwrite_img();
    }
    else if(state == DRAWING) {
        delete canvas;
        canvas = NULL;
        glutPostRedisplay();
    }
    state = IDLE;
    switch(m) {
        case LINES:
            mode = LINES;
            printf("Mode set to lines.\n");
            break;
        case CIRCLES:
            mode = CIRCLES;
            printf("Mode set to circles.\n");
            break;
        case POLYGONS:
            mode = POLYGONS;
            p = Polygon();
            printf("Mode set to polygons.\n");
            break;
        case FILL_RECURSIVE:
            mode = FILL_RECURSIVE;
            printf("Mode set to recursive seed-fill.\n");
            break;
        case FILL_SWEEP:
            mode = FILL_SWEEP;
            printf("Mode set to sweep seed-fill.\n");
            break;
    }
}

/* Convert window y coordinates to image y coordinates (basically Y axis inversion) */
int y_win_to_img(const Image* img, int y) {
    return img->height() - y;
}

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(canvas)
        canvas->draw();
    else
        img->draw();

    glutSwapBuffers();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir en fonction de la position de
// la souris (x,y), en fonction du bouton de la souris qui a été
// pressé ou relaché.
//------------------------------------------------------------------

void mouse_CB(int button, int button_state, int x, int y) {
    y = y_win_to_img(img, y);
    if((button == GLUT_LEFT_BUTTON) && (button_state == GLUT_DOWN)) {
        switch(mode) {
            case LINES:
                if(state == IDLE) {
                    state = DRAWING;
                    l.set_p1(Vec2i(x,y));
                    printf("First point for bresenham line (%d, %d)\n", x, y);
                    break;
                }
                else if (state == DRAWING) {
                    state = IDLE;
                    l.set_p2(Vec2i(x,y));
                    printf("Drawing bresenham line from (%d, %d) to (%d, %d)\n", l.p1().x(), l.p1().y(), x, y);
                    draw_line_bresenham(img, l);
                }
                break;
            case CIRCLES:
                if(state == IDLE) {
                    state = DRAWING;
                    c.set_center(Vec2i(x,y));
                    printf("Center for bresenham circle (%d, %d)\n", x, y);
                    break;
                }
                else if (state == DRAWING) {
                    state = IDLE;
                    Vec2i center = c.center();
                    Vec2f R(x - center.x(), y - center.y());
                    int r = sqrt(R*R);
                    c.set_radius(r);
                    printf("Drawing bresenham circle at center (%d, %d) with radius %d\n", center.x(), center.y(), r);
                    draw_circle_bresenham(img, c);
                }
                break;
            case POLYGONS:
                if(state == IDLE || state == EDIT) {
                    if(state == EDIT) {
                        delete img;
                        img = canvas;
                        canvas = NULL;
                    }
                    p = Polygon();
                    state = DRAWING;
                }
                p.add_point(Vec2i(x, y));
                printf("Polygon point n°%lu at (%d, %d)\n", p.n_points(), x, y);
                break;
            case FILL_RECURSIVE:
                printf("Filling recursively area at (%d, %d)\n", x, y);
                seed_fill_recursive(img, x, y, img->color_at(x, y), img->current_color());
                break;
            case FILL_SWEEP:
                printf("Sweep-filling area at (%d, %d)\n", x, y);
                seed_fill_sweep(img, x, y, img->color_at(x, y), img->current_color());
                break;
        }
    }

    glutPostRedisplay();
}

void passive_mouse_move_CB(int x, int y) {
    Vec2i R;
    if(state == DRAWING) {
        switch(mode) {
            case LINES:
                new_canvas();
                l.set_p2(Vec2i(x, canvas->height()-y));
                draw_line_bresenham(canvas, l);
                break;
            case CIRCLES:
                new_canvas();
                R = Vec2i(x, y) - c.center();
                c.set_radius(sqrt(R*R));
                draw_circle_bresenham(canvas, c);
                break;
            case POLYGONS:
                new_canvas();
                if(p.n_points() == 1) {
                    l.set_p1(p.first());
                    l.set_p2(Vec2i(x, canvas->height()-y));
                    draw_line_bresenham(canvas, l);
                }
                else {
                    Polygon pp(p);
                    pp.add_point(Vec2i(x, canvas->height() - y));
                    draw_polygon(canvas, pp);
                }
                break;
            default:
                break;
        }
    }
    glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
    // fprintf(stderr,"key=%d\n",key);
    switch(key) {
        case 27 : 
            if(canvas)
                delete canvas;
            delete img;
            exit(1);
            break;
        case 'G':
        case 'g': img->greyscale(); break;
        case 'O':
        case 'o': printf("Number of connected components : %d\n", img->n_connected_components()); break;
        case 'W':
        case 'w':
            double d;
            std::cout << "Threshold value (0-1, negative = auto) : ";
            std::cin >> d;
            img->threshold(d);
            break;
        case 'S':
        case 's': change_mode(LINES); break;
        case 'C':
        case 'c': change_mode(CIRCLES); break;
        case 'P':
        case 'p': change_mode(POLYGONS); break;
        case 'R':
        case 'r': change_mode(FILL_RECURSIVE); break;
        case 'B':
        case 'b': change_mode(FILL_SWEEP); break;
        case 13: // ENTER key
            if(mode == POLYGONS && state != IDLE) {
                auto s = p.n_points();
                if(s >= 2) {
                    printf("Drawing polygon (%lu points)\n", s);
                    if(canvas)
                        delete canvas;
                    canvas = new Image(*img);
                    draw_polygon(canvas, p);
                    printf("Polygon is %s and %s-oriented\n", p.is_convex() ? "convex" : "concave",
                                                              (p.orientation() == Polygon::Left) ? "left" : "right");
                }
                else
                    printf("Error : polygon must have at least 2 points\n");
                state = EDIT;
            }
            break;
        case '+':
            if(state == EDIT) {
                printf("Upscaling polygon\n");
                p.scale(2);
                if(canvas)
                    delete canvas;
                canvas = new Image(*img);
                draw_polygon(canvas, p);
            }
            break;
        case '-':
            if(state == EDIT) {
                printf("Downscaling polygon\n");
                p.scale(0.5);
                if(canvas)
                    delete canvas;
                canvas = new Image(*img);
                draw_polygon(canvas, p);
            }
            break;
        case ' ':
            if(state == EDIT) {
                printf("Rotating polygon\n");
                p.rotate(Polygon::Right);
                if(canvas)
                    delete canvas;
                canvas = new Image(*img);
                draw_polygon(canvas, p);
            }
            break;
        case 'z' : img->zoom(2.0); break;
        case 'Z' : img->zoom(0.5); break;
        case 'i' : img->zoomInit(); break;
        default : fprintf(stderr,"keyboard_CB : %d : unknown key.\n",key);
    }
    glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche spéciale (F1, F2 ... F12, home, end, insert,
// haut, bas, droite, gauche etc).
//------------------------------------------------------------------

void special_CB(int key, int x, int y)
{
	// int mod = glutGetModifiers();

	int d = 10;

	switch(key)
	{
	case GLUT_KEY_UP    : img->move(0,d); break;
	case GLUT_KEY_DOWN  : img->move(0,-d); break;
	case GLUT_KEY_LEFT  : img->move(d,0); break;
	case GLUT_KEY_RIGHT : img->move(-d,0); break;
	default : fprintf(stderr,"special_CB : %d : unknown key.\n",key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if((argc!=3)&&(argc!=2))
	{
		fprintf(stderr,"\n\nUsage \t: %s <width> <height>\nou",argv[0]);
		fprintf(stderr,"\t: %s <ppmfilename> \n\n",argv[0]);
		exit(1);
	}
	else
	{
		int largeur, hauteur;
		if(argc==2)
		{
			img = Image::read(argv[1]);
			largeur = img->width();
			hauteur = img->height();
		}
		else
		{
			largeur = atoi(argv[1]);
			hauteur = atoi(argv[2]);
			img = new Image(largeur,hauteur);
			Color rouge(0.5,0,0);
			Color blanc(0.8,0.8,1);
			img->checker(rouge,blanc,50);
                        img->changeColor(Color(0, 0, 0));
		}
		int windowPosX = 100, windowPosY = 100;

		glutInitWindowSize(largeur,hauteur);
		glutInitWindowPosition(windowPosX,windowPosY);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
		glutInit(&argc, argv);
		glutCreateWindow(argv[0]);

		glViewport(0, 0, largeur, hauteur);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glOrtho(0,largeur,0,hauteur,-1,1);

		glutDisplayFunc(display_CB);
		glutKeyboardFunc(keyboard_CB);
		glutSpecialFunc(special_CB);
		glutMouseFunc(mouse_CB);
		//glutMotionFunc(mouse_move_CB);
		glutPassiveMotionFunc(passive_mouse_move_CB);

		glutMainLoop();

		return 0;
	}
}
