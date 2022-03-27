// **********************************************************************
// PUCRS/FACIN
// Programa de testes para manipulação de Imagens
//
// Marcio Sarroglia Pinho
//
// pinho@pucrs.br
// **********************************************************************

#include <iostream>
#include <iomanip>
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

#include "ImageClass.h"

ImageClass Image;

#define LARGURA_JAN 1000
#define ALTURA_JAN 500

// **********************************************************************
// void ConvertToGrayscale()
// **********************************************************************
void ConvertToGrayscale()
{
    //cout << "Iniciou ConvertToGrayscale..." << endl;
    int x,y;
    int i;
    cout << Image.SizeX() << " " << Image.SizeY() << endl;
    for(y=Image.SizeY()-1; y>=0; y--)
    {
        for(x=0; x<Image.SizeX(); x++)
        {
            i = Image.GetPointIntensity(x,y); // Le o TOM DE CINZA DA IMAGEM
            Image.DrawPixel(x, y,i,i,i);  // exibe um ponto CINZA
            cout << setw(4) << i ;
        }
        cout << endl;
    }
    //cout << "Concluiu ConvertToGrayscale." << endl;
}

// **********************************************************************
//  void CarregaImagem(char *nome)
// **********************************************************************
void CarregaImagem(char *nome)
{
    int r;
    // Carrega a uma image
//    r = Image.Load("ArquivosCodeBlocks/Imagens/1.png"); // Carrega uma imagem
//    r = Image.Load("ArquivosCodeBlocks/Imagens/Falcao.jpg"); // Carrega uma imagem
//    r = Image.Load("ArquivosCodeBlocks/Imagens/Ruido2.bmp"); // Carrega uma imagem

    r = Image.Load(nome);
    if (!r)
    {
        cout << ("Imagem nao encontrada!\n");
        exit(1); // Erro na carga da imagem
    }
    //else cout << ("Imagem carregada!\n");

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

// Ajusta o ZOOM da imagem para que apareca em toda a janela
    float zoomH = (glutGet(GLUT_WINDOW_WIDTH))/(double)Image.SizeX();
    float zoomV = (glutGet(GLUT_WINDOW_HEIGHT))/(double)Image.SizeY();
    Image.SetZoomH(zoomH);
    Image.SetZoomV(zoomV);

// posiciona a imagem no canto inferior esquerdo da janela
    Image.SetPos(0, 0);

// Coloca a imagem na tela
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
    case 'g':
        ConvertToGrayscale();
        glutPostRedisplay();    // obrigatório para redesenhar a tela
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
    glutCreateWindow    ( "Image Converter" );
    if (argc !=2 )
    {
        cout << "Formato: ";
        cout << "Img2Txt NomeDaImage > NomeDoTxt" << endl;
        exit(1);
    }
    CarregaImagem (argv[1]);

    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( arrow_keys );

    glutMainLoop ( );
    return 0;
}


