// **********************************************************************
// PUCRS/FACIN
// Programa de testes para manipulação de Imagens
//
// Marcio Sarroglia Pinho
//
// pinho@pucrs.br
// **********************************************************************

#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

#ifdef _MSC_VER


#endif

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
static DWORD start_time, end_time;
#else
#include <sys/time.h>
static struct timeval start_time;
#endif


#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#include "SOIL/SOIL.h"

#include "ImageClass.h"

class Ponto{
public:
    double x,y;
    Ponto(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
};

ImageClass Image, Personagem1;

double PosX = 0.0;
double PosY = 0.0;
double DirX = 1.0;
double DirY = 1.0;

Ponto PosPersonagem1(0,0);
Ponto DirPersonagem1(1,1);


#define LARGURA_JAN 800
#define ALTURA_JAN 800

float dt;

void IniciaTimer()
{

#ifdef _WIN32
    start_time = GetTickCount();
#else
    // Figure out time elapsed since last call to idle function
    gettimeofday(&start_time, NULL);
#endif

}
void EncerraTimer()
{
#ifdef _WIN32
    end_time = GetTickCount();
    dt = (float) (end_time - start_time) / 1000.0;
#else
    // Figure out time elapsed since last call to idle function
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    dt = (float)(end_time.tv_sec  - start_time.tv_sec) + 1.0e-6*(end_time.tv_usec - start_time.tv_usec);
#endif
    cout << "Tempo gasto: " << dt << " s. FPS: " << 1/dt << endl;

}
// **********************************************************************
//  void init(void)
// **********************************************************************
void init()
{
    int r;
    // Carrega a uma imagem
    r = Image.Load("../data/Falcao.jpg"); // Carrega uma imagem

    if (!r) exit(1); // Erro na carga da imagem
    else cout << ("Imagem carregada!\n");

    // r = Personagem1.Load("AguiaPeq.png"); // Carrega uma imagem
    r = Personagem1.Load("../data/Img1.png"); // Carrega uma imagem

    if (!r) exit(1); // Erro na carga da imagem
    else cout << ("Imagem carregada!\n");

}
// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
// **********************************************************************
void reshape( int w, int h )
{

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    gluOrtho2D(0,w,0,h);

    // Set the clipping volume
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}
// **********************************************************************
//  void display( void )
// **********************************************************************
void display( void )
{
    IniciaTimer();
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

// Ajusta o ZOOM da imagem para que apareca ocupando toda a janela
    float zoomH = (glutGet(GLUT_WINDOW_WIDTH))/(double)Image.SizeX();
    float zoomV = (glutGet(GLUT_WINDOW_HEIGHT))/(double)Image.SizeY();
    Image.SetZoomH(zoomH);
    Image.SetZoomV(zoomV);

    Personagem1.Display();

// Posiciona a imagem no canto inferior esquerdo da janela
    Image.SetPos(0, 0);

// Calcula a nova posição do personagem
    double Veloc = ALTURA_JAN/2.0;

    double modulo = sqrt(DirPersonagem1.x*DirPersonagem1.x + DirPersonagem1.y*DirPersonagem1.y);

    // transforma o vetor de direção em unitário
    DirPersonagem1.x = DirPersonagem1.x/(modulo);
    DirPersonagem1.y = DirPersonagem1.y/(modulo);

    // Ajusta a posição do personagem
    PosPersonagem1.x += DirPersonagem1.x * (dt * Veloc);
    PosPersonagem1.y += DirPersonagem1.y * (dt * Veloc);
    Personagem1.SetPos(PosPersonagem1.x, PosPersonagem1.y);

// Coloca as imagens na tela
    Image.Display();
    Personagem1.Display();

// Mostra a tela OpenGL
    glutSwapBuffers();

    EncerraTimer();
}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Termina o programa qdo
        Image.Delete();
        exit ( 0 );   // a tecla ESC for pressionada
        break;
     default:
        break;
    }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    switch ( a_keys )
    {
    case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
            PosY+=3;
        break;
    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
            PosY-=3;
        break;
    default:
        break;
    }
    //glutPostRedisplay();
}

// **********************************************************************
// **********************************************************************
void Mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        cout << "(" << x << "," << y << "): " ;
        cout << Image.GetPointIntensity(x,y) << endl;
        Image.DrawPixel(x,y,255,0,0);
        glutPostRedisplay();
    }
}

// **********************************************************************
//  void main ( int argc, char** argv )
// **********************************************************************

int main ( int argc, char** argv )
{
    glutInit            ( &argc, argv );

    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (10,10);

    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize  ( LARGURA_JAN, ALTURA_JAN);
    glutCreateWindow    ( "Image Loader" );

    init ();

    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( arrow_keys );
    glutMouseFunc(Mouse);

    glutIdleFunc ( display );

    glutMainLoop ( );
    return 0;
}


