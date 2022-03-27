// **********************************************************************
// PUCRS/FACIN
// Programa de testes para manipulação de Imagens
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************
#include <iostream>
#include <cmath>

using namespace std;

#ifdef _MSC_VER


#endif

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#include "SOIL/SOIL.h"

#include "ImageClass.h"


#define FILE "datasets/original/A2 Dentes 01 02 03__rec0684.bmp"
//#define FILE "../01_Train_DataSet.png"
//#define FILE "../Nova.bmp"

#define LARGURA_JAN 1000
#define ALTURA_JAN 500

#define COLORS 255
#define HIST_WIDTH 260
#define HIST_HEIGHT 510

ImageClass Image, HistImage;
int hist[COLORS];

#define MEDIAN 7
#define HALF_MEDIAN MEDIAN/2

void Order(int window[]) {
    int temp, i, j;
    for (i = 0; i < MEDIAN; i++) {
        temp = window[i];
        for (j = i - 1; j >= 0 && temp < window[j]; j--) {
            window[j + 1] = window[j];
        }
        window[j + 1] = temp;
    }
}

void GetNeighbors(int Px, int Vetor[MEDIAN], int val[]) {
    int x, z = 0;
    for (x = Px - HALF_MEDIAN; x <= Px + HALF_MEDIAN; x++) {
        Vetor[z] = val[x];
        z++;
    }
}

void Median(int val[], int s) {
    int median[s];

    for (int i = 0; i < s; i++)
        median[i] = val[i];

    for (int i = HALF_MEDIAN; i < s - HALF_MEDIAN; i++) {
        int Vetor[MEDIAN];
        GetNeighbors(i, Vetor, val);
        Order(Vetor);
        median[i] = Vetor[HALF_MEDIAN + 1];
    }

    for (int i = 0; i < s; i++)
        val[i] = median[i];
}

float Maior() {
    float maior = 0;
    int y = 0;
    for (int i = 0; i < COLORS; i++) {
        if (hist[i] > maior) {
            maior = (float) hist[i];
            y = i;
        }
    }

    hist[y] = 0;
    maior = 0;
    for (int i = 0; i < COLORS; i++) {
        if (hist[i] > maior) {
            maior = (float) hist[i];
        }
    }
    return maior;
}

void Histograma() {
    int x, y;
    int i;

    for (i = 0; i < COLORS; i++) {
        hist[i] = 0;
    }

    for (x = 0; x < Image.SizeX(); x++) {
        for (y = 0; y < Image.SizeY(); y++) {
            i = Image.GetPointIntensity(x, y);
            hist[i] += 1;
        }
    }


    Median(hist, 255);
    HistImage.DrawLineV(0, 0, HIST_HEIGHT, 255, 0, 0);
    HistImage.DrawLineH(0, 0, HIST_WIDTH, 255, 0, 0);

    float normal = Maior();
    int j = 0;
    for (i = 0; i < COLORS; i++) {
        HistImage.DrawLineV(j, 0, HIST_HEIGHT * hist[i] / normal, 0, 0, 0);
        j += 2;
    }
}

// **********************************************************************
//  void init(void)
// **********************************************************************
void init() {
    int r;
    string nome = FILE;

    string path = "";
    //string path = "ArquivosCodeBlocks/Imagens/";

    nome = path + nome;
    cout << "imagem a ser carregada: *" << nome << "*" << endl;
    r = Image.Load(nome.c_str());

    if (!r) exit(1);
    else cout << ("Imagem carregada!\n");

    HistImage.SetSize(HIST_WIDTH, HIST_HEIGHT, Image.Channels());
}

// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
// **********************************************************************
void reshape(int w, int h) {
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    gluOrtho2D(0, w, 0, h);
    // Set the clipping volume
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// **********************************************************************
//  void display( void )
// **********************************************************************
void display(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fundo de tela preto
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    Histograma();

    // Ajusta o ZOOM da imagem para que apareca na metade da janela
    float zoomH = (glutGet(GLUT_WINDOW_WIDTH) / 2.0) / (double) Image.SizeX();
    float zoomV = (glutGet(GLUT_WINDOW_HEIGHT)) / (double) Image.SizeY();
    Image.SetZoomH(zoomH);
    Image.SetZoomV(zoomV);
    // posiciona a imagem no canto inferior esquerdo da janela
    Image.SetPos(0, 0);

    float zoomH2 = (glutGet(GLUT_WINDOW_WIDTH) / 2.0 - 50) / (double) HistImage.SizeX();
    float zoomV2 = (glutGet(GLUT_WINDOW_HEIGHT) - 100) / (double) HistImage.SizeY();
    HistImage.SetZoomH(zoomH2);
    HistImage.SetZoomV(zoomV2);
    // posiciona a imagem nova na metada da direita da janela
    HistImage.SetPos(glutGet(GLUT_WINDOW_WIDTH) / 2 + 25, 50);

    // Coloca as imagens na tela
    Image.Display();
    HistImage.Display();

    // Mostra a tela OpenGL
    glutSwapBuffers();
}

// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
// **********************************************************************
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // Termina o programa qdo
            HistImage.Delete();
            Image.Delete();
            exit(0);   // a tecla ESC for pressionada
            break;
        default:
            break;
    }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
// **********************************************************************
void arrow_keys(int a_keys, int x, int y) {
    switch (a_keys) {
        case GLUT_KEY_UP:
            break;
        case GLUT_KEY_DOWN:
            break;
        default:
            break;
    }
}


// **********************************************************************
//  void main ( int argc, char** argv )
// **********************************************************************

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(LARGURA_JAN, ALTURA_JAN);
    glutCreateWindow("Image Loader");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow_keys);
    //glutIdleFunc ( display );

    glutMainLoop();
    return 0;
}
