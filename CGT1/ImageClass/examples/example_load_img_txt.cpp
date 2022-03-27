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

//#define _USE_SOIL_

#include "ImageClass.h"

ImageClass Image;

#define LARGURA_JAN 500
#define ALTURA_JAN 500

// **********************************************************************
// bool LeImagemTXT(int largura, int altura)
// **********************************************************************
bool LeImagemTXT(int largura, int altura)
{
    cout << "Iniciou Leitura..." << endl;
    int x,y;
    int i;
    for(y=0; y<altura; y++)
    {
        cout << "Lendo linha " << y << endl;
        for(x=0; x<largura; x++)
        {
            cin >> i;
            Image.DrawPixel(x, altura-y-1,i,i,i);  // exibe um ponto CINZA na imagem da direita
        }
    }
    cout << "Concluiu Leitura." << endl;
    return true;
}
// **********************************************************************
//  void LeTXT()
// **********************************************************************
void LeTXT()
{
    int largura, altura;
    system("clear");
    cout << "Digite a largura da imagem(Eixo X): ";
    cin >> largura;
    cout << "Digite a altura da imagem(Eixo Y): ";
    cin >> altura;
    Image.SetSize(largura, altura);
    if (!LeImagemTXT(largura, altura))
    {
        cout << "Dados invalidos";
        exit(1);
    }
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
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

// Ajusta o ZOOM da imagem para que apareca na metade da janela
    float zoomH = (glutGet(GLUT_WINDOW_WIDTH))/(double)Image.SizeX();
    float zoomV = (glutGet(GLUT_WINDOW_HEIGHT))/(double)Image.SizeY();
    Image.SetZoomH(zoomH);
    Image.SetZoomV(zoomV);
// posiciona a imagem no canto inferior esquerdo da janela
    Image.SetPos(0, 0);

// Coloca as imagens na tela
    Image.Display();

// Mostra a tela OpenGL
    glutSwapBuffers();
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
        break;
    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...

        break;
    default:
        break;
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

    if (argc !=1 )
    {
        cout << "Formato: ";
        cout << "LoadTxtImage < TxtFileName" << endl;
        exit(1);
    }

    LeTXT ();

    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( arrow_keys );

    glutMainLoop ( );
    return 0;
}


