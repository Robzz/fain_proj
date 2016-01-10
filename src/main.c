
/*===============================================================*\

    Vendredi 25 septembre 2013

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/

#include <cstdio>
#include <cmath>
#include <vector>

#include <GL/glut.h>
#include <GL/gl.h>

#include "Image.h"
#include "draw.h"
#include "vector.h"

typedef enum { LINES, CIRCLES, POLYGONS, FILL_RECURSIVE } Mode;
typedef enum { IDLE, DRAWING } State;

Mode mode = LINES;
State state = IDLE;
std::vector<Vec2f> polygon_pts;
Image *img;

void change_mode(Mode m) {
    switch(m) {
        case LINES:
            mode = LINES;
            state = IDLE;
            printf("Mode set to lines.\n");
            break;
        case CIRCLES:
            mode = CIRCLES;
            state = IDLE;
            printf("Mode set to circles.\n");
            break;
        case POLYGONS:
            mode = POLYGONS;
            state = IDLE;
            polygon_pts.clear();
            printf("Mode set to polygons.\n");
            break;
        case FILL_RECURSIVE:
            mode = FILL_RECURSIVE;
            printf("Mode set to recursive seed-fill.\n");
            break;
    }
}

/* Convert window y coordinates to image y coordinates (basically Y axis inversion) */
int y_win_to_img(const Image* img, int y) {
    return img->_height - y;
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

    I_draw(img);

    glutSwapBuffers();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir en fonction de la position de
// la souris (x,y), en fonction du bouton de la souris qui a été
// pressé ou relaché.
//------------------------------------------------------------------

void mouse_CB(int button, int button_state, int x, int y) {
    static int x1 = 0, y1 = 0;
    y = y_win_to_img(img, y);
    if((button == GLUT_LEFT_BUTTON) && (button_state == GLUT_DOWN)) {
        switch(mode) {
            case LINES:
                if(state == IDLE) {
                    state = DRAWING;
                    x1 = x;
                    y1 = y;
                    printf("First point for bresenham line (%d, %d)\n", x1, y1);
                    break;
                }
                else if (state == DRAWING) {
                    state = IDLE;
                    printf("Drawing bresenham line from (%d, %d) to (%d, %d)\n", x1, y1, x, y);
                    draw_line_bresenham(img, x1, y1, x, y);
                }
                break;
            case CIRCLES:
                if(state == IDLE) {
                    state = DRAWING;
                    x1 = x;
                    y1 = y;
                    printf("Center for bresenham circle (%d, %d)\n", x1, y1);
                    break;
                }
                else if (state == DRAWING) {
                    state = IDLE;
                    Vec2f R(x - x1, y - y1);
                    int r = sqrt(R*R);
                    printf("Drawing bresenham circle at center (%d, %d) with radius %d\n", x1, y1, r);
                    draw_circle_bresenham(img, x1, y1, r);
                }
                break;
            case POLYGONS:
                if(state == IDLE)
                    state = DRAWING;
                polygon_pts.push_back(Vec2f(x, y));
                printf("Polygon point n°%lu at (%d, %d)\n", polygon_pts.size(), x, y);
                break;
            case FILL_RECURSIVE:
                printf("Filling recursively area at (%d, %d)\n", x, y);
                seed_fill_recursive(img, x, y, img->_buffer[x][y], img->_current_color);
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
        case 27 : exit(1); break;
        case 'S':
        case 's': change_mode(LINES); break;
        case 'C':
        case 'c': change_mode(CIRCLES); break;
        case 'P':
        case 'p': change_mode(POLYGONS); break;
        case 'R':
        case 'r': change_mode(FILL_RECURSIVE); break;
        case 13: // ENTER key
            if(mode == POLYGONS && state != IDLE) {
                auto s = polygon_pts.size();
                if(s >= 2) {
                    printf("Drawing polygon (%lu points)\n", polygon_pts.size());
                    draw_polygon(img, polygon_pts);
                    polygon_pts.clear();
                }
                else
                    printf("Error : polygon must have at least 2 points\n");
            }
            break;
        case 'z' : I_zoom(img,2.0); break;
        case 'Z' : I_zoom(img,0.5); break;
        case 'i' : I_zoomInit(img); break;
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
	case GLUT_KEY_UP    : I_move(img,0,d); break;
	case GLUT_KEY_DOWN  : I_move(img,0,-d); break;
	case GLUT_KEY_LEFT  : I_move(img,d,0); break;
	case GLUT_KEY_RIGHT : I_move(img,-d,0); break;
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
			img = I_read(argv[1]);
			largeur = img->_width;
			hauteur = img->_height;
		}
		else
		{
			largeur = atoi(argv[1]);
			hauteur = atoi(argv[2]);
			img = I_new(largeur,hauteur);
			Color rouge(100,0,0);
			Color blanc(200,200,255);
			I_checker(img,rouge,blanc,50);
                        I_changeColor(img, Color(0, 0, 0));
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
		// glutMotionFunc(mouse_move_CB);
		// glutPassiveMotionFunc(passive_mouse_move_CB);

		glutMainLoop();

		return 0;
	}
}
