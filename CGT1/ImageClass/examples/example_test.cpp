// **********************************************************************
// PUCRS/FACIN
// Programa de testes para manipula��o de Imagens
//
// Marcio Sarroglia Pinho
//
// pinho@pucrs.br
// **********************************************************************

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#include "SOIL/SOIL.h"

#include "ImageClass.h"

ImageClass Image, NewImage;

const int LIMIAR = 100;
#define LARGURA_JAN 1000
#define ALTURA_JAN 500

// **********************************************************************
//
// **********************************************************************
#include <fstream>
float ImageMat[512][512];

class Vetor{
    double x,y;
public:
    Vetor(){ x=0; y=0;} // construtora
    void versor()
    {
        
    }
    void set(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
    void get (double &x, double &y)
    {
        x = this->x;
        y = this->y;
    }
    double getX() { return this->x; }
    double getY() { return this->y; }
    void add(Vetor v)
    {
        this->x += v.getX();
        this->y += v.getY();
    }
    void multiply(double n) { this->x *=n; this->y *=n;}
};

Vetor A,B,DIR;
double Veloc = 1.0;
#ifdef WIN32
#include <windows.h>
#include <glut.h>
static DWORD last_idle_time;
#else
#include <sys/time.h>
static struct timeval last_idle_time;
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif


// **********************************************************************
//  void ConvertBlackAndWhite()
// **********************************************************************
int LoadTXT(const char *Nome)
{
        cout << "Iniciou LoadTXT....";
        ifstream ArqImg;
        
        Image.SetSize(512,512, 3); // define o tamanho da imagem conforme a especifica�ao
        
        ArqImg.open(Nome, ios::in);
        if(!ArqImg)
        {
            cout << "Erro na abertura do arquivo TXT!" << endl;
            return 0;
        }
        for(int x=0; x<Image.SizeX(); x++)
            for(int y=0; y<Image.SizeY(); y++)
            {
                ArqImg >> ImageMat[x][y];
            }
        ArqImg.close();
        
        for(int x=0; x<Image.SizeX(); x++)
        {
            for(int y=0; y<Image.SizeY(); y++)
            {
                unsigned char r,g,b;
                float fator = 255.0/3500.0;  // este c�lculo deve ser refeito para que os ��metros BROLHJo E CONTRASTE seja
                r = g = b = ImageMat [y][x] * fator;
                Image.DrawPixel(x, y, r,g,b);
            }
        }
        cout << "Terminou LoadTXT....";
        
        return 1;
    }
                  // **********************************************************************
                  //  void ConvertBlackAndWhite()
                  // **********************************************************************
                  void ConvertBlackAndWhite()
    {
        unsigned char r,g,b;
        int x,y;
        int i;
        cout << "Iniciou Black & White....";
        
        for(x=0; x<Image.SizeX(); x++)
        {
            for(y=0; y<Image.SizeY(); y++)
            {
                i = Image.GetPointIntensity(x,y); // VERIFICA O TOM DE CINZA DA IMAGEM
                // Image.ReadPixel(x,y,r,g,b);
                
                if (i < LIMIAR)
                {
                    NewImage.DrawPixel(x, y,0,0,0);  // exibe um ponto PRETO na imagem
                }
                else NewImage.DrawPixel(x, y, 255,255,255); // exibe um ponto VERMELHO na imagem
            }
        }
        cout << "Concluiu Black & White.\n";
    }
                  
                  
                  // **********************************************************************
                  // void DetectImageBorders()
                  // **********************************************************************
                  void DetectImageBorders()
    {
        cout << "Iniciou DetectImageBorders....";
#define SENSIBILIDADE 10
        
        
        cout << "Iniciou DetectImageBorders...." << endl;
    }
                  
                  // **********************************************************************
                  // void ConvertToGrayscale()
                  // **********************************************************************
                  void ConvertToGrayscale()
    {
        cout << "Iniciou ConvertToGrayscale..." << endl;
        int x,y;
        int i;
        for(x=0; x<Image.SizeX(); x++)
        {
            for(y=0; y<Image.SizeY(); y++)
            {
                i = Image.GetPointIntensity(x,y); // Le o TOM DE CINZA DA IMAGEM
                NewImage.DrawPixel(x, y,i,i,i);  // exibe um ponto CINZA na imagem da direita
            }
        }
        cout << "Concluiu ConvertToGrayscale." << endl;
    }
                  
                  // **********************************************************************
                  // void InvertImage()
                  // **********************************************************************
                  void InvertImage()
    {
        cout << "Iniciou InvertImage..." << endl;
        
        
        cout << "Concluiu InvertImage." << endl;
    }
                  
                  
                  void OrdenaVetor(int window[])
    {
        int temp, i, j;
        for(i = 0; i < 9; i++)
        {
            temp = window[i];
            for(j = i-1; j >= 0 && temp < window[j]; j--)
            {
                window[j+1] = window[j];
            }
            window[j+1] = temp;
        }
    }
                  void MontaVetor(int Px, int Py, int Vetor[9])
    {
        
    }
                  // **********************************************************************
                  // void Mediana()
                  // **********************************************************************
                  void Mediana()
    {
        
        
    }
                  
                  // **********************************************************************
                  //  void init(void)
                  // **********************************************************************
                  void init()
    {
        int r;
        // Carrega a uma imagem
        string nome;
        
        //nome = "exame_sem_cranio.txt";
        //nome = "Falcao.jpg";
        nome = "../data/Colegio.png";
        //nome += "Ruido2.bmp";
        r = Image.Load(nome.c_str()); // Carrega uma imagem
        //r = LoadTXT (nome.c_str());
        
        if (!r) exit(1); // Erro na carga da imagem
        else cout << ("Imagem carregada!\n");
        
        // Ajusta o tamanho da imagem da direita, para que ela
        // passe a ter o mesmo tamnho da imagem recem carregada
        // Caso precise alterar o tamanho da nova imagem, mude os par�metros
        // da na chamada abaixo
        NewImage.SetSize(Image.SizeX(), Image.SizeY(), Image.Channels());
        cout << "Nova Imagem Criada" << endl;
        
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
        float zoomH = (glutGet(GLUT_WINDOW_WIDTH)/2.0)/(double)Image.SizeX();
        float zoomV = (glutGet(GLUT_WINDOW_HEIGHT))/(double)Image.SizeY();
        Image.SetZoomH(0.1);
        Image.SetZoomV(0.1);
        
        // posiciona a imagem no canto inferior esquerdo da janela
        
        Image.SetPos(B.getX(), B.getY());
        
        // posiciona a imagem nova na metada da direita da janela
        NewImage.SetPos(glutGet(GLUT_WINDOW_WIDTH)/2, 0);
        
        // Ajusta o ZOOM da imagem para que apareca na metade da janela
        NewImage.SetZoomH(zoomH);
        NewImage.SetZoomV(zoomV);
        
        // Coloca as imagens na tela
        Image.Display();
        NewImage.Display();
        
        // Mostra a tela OpenGL
        glutSwapBuffers();
        cout << "Terminou a exibicao!" << endl;
    }
                  // **********************************************************************
                  //  void keyboard ( unsigned char key, int x, int y )
                  // **********************************************************************
                  void keyboard ( unsigned char key, int x, int y )
    {
        switch ( key )
        {
            case ' ':
            case 27: // Termina o programa qdo
                cout << "FIM!" << endl;
                //NewImage.Delete();
                //Image.Delete();
                cout << "FIM!" << endl;
                exit ( 1 );   // a tecla ESC for pressionada
                break;
            case '2':
                //        ConvertBlackAndWhite(220, 255);
                ConvertBlackAndWhite();
                glutPostRedisplay();    // obrigat�rio para redesenhar a tela
                break;
            case 'g':
                ConvertToGrayscale();
                glutPostRedisplay();    // obrigat�rio para redesenhar a tela
                break;
                
            case 'b':
                DetectImageBorders();
                glutPostRedisplay();    // obrigat�rio para redesenhar a tela
                break;
            case 'i':
                InvertImage();
                glutPostRedisplay();    // obrigat�rio para redesenhar a tela
                break;
            case 'm':
                Mediana();
                glutPostRedisplay();    // obrigat�rio para redesenhar a tela
                break;
            case 'c':
                NewImage.CopyTo(&Image);
                glutPostRedisplay();    // obrigat�rio para redesenhar a tela
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
        
        // Define o tamanho da janela gr�fica do programa
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
                  

