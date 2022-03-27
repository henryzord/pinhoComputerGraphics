// **********************************************************************
// PUCRS/FACIN
// Programa de testes para manipulação de Imagens
//
// Marcio Sarroglia Pinho
//
// pinho@pucrs.br
// **********************************************************************
//
//  Para rodar no XCODE, veja o arquivo "InstrucoesXCODE.rtf"
//


#include <iostream>
#include <cmath>

using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#include "ImageClass.h"

ImageClass image, blankImage;
// número de segundos que a imagem tem para atravessar a tela
int n_seconds = 0;

#include "Temporizador.h"

Temporizador T;


const int LIMIAR = 100;
#define LARGURA_JAN 1000
#define ALTURA_JAN 500


// **********************************************************************
//  void ConvertBlackAndWhite()
// **********************************************************************
void ConvertBlackAndWhite() {
    unsigned char r, g, b;
    int x, y;
    int i;

    cout << "Iniciou Black & White....";

    for (x = 0; x < image.SizeX(); x++) {
        for (y = 0; y < image.SizeY(); y++) {
            i = image.GetPointIntensity(x, y); // VERIFICA O TOM DE CINZA DA IMAGEM
            // image.ReadPixel(x,y,r,g,b);

            if (i < LIMIAR) {
                blankImage.DrawPixel(x, y, 0, 0, 0);  // exibe um ponto PRETO na imagem
            } else blankImage.DrawPixel(x, y, 255, 255, 255); // exibe um ponto VERMELHO na imagem
        }
    }
    cout << "Concluiu Black & White.\n";
}

// **********************************************************************
// void DetectImageBorders()
// **********************************************************************
void ConvertBlackAndWhite(int Imin, int Imax) {
    unsigned char r, g, b;
    int x, y;
    int i;
    cout << "Iniciou Black & White....";
    for (x = 0; x < image.SizeX(); x++) {
        for (y = 0; y < image.SizeY(); y++) {
            i = image.GetPointIntensity(x, y); // VERIFICA O TOM DE CINZA DA IMAGEM
            image.ReadPixel(x, y, r, g, b);

            if (i >= Imin && i <= Imax) {
                blankImage.DrawPixel(x, y, 0, 0, 0);  // exibe um ponto PRETO na imagem
            } else blankImage.DrawPixel(x, y, 255, 255, 255); // exibe um ponto VERMELHO na imagem

        }
    }
    cout << "Concluiu Black & White." << endl;
}

// **********************************************************************
// void DetectImageBorders()
// **********************************************************************
void DetectImageBorders() {
    cout << "Iniciou DetectImageBorders....";
#define SENSIBILIDADE 10
    // varredura vertical

    int x, y;
    int i1, i2;
/*
    for(x=0; x<image.SizeX(); x++)
    {
        for(y=0; y<image.SizeY()-1; y++)
        {
            i1 = image.GetPointIntensity(x,y); // Le o TOM DE CINZA DA IMAGEM
            i2 = image.GetPointIntensity(x,y+1); // Le o TOM DE CINZA DA IMAGEM
            if (fabs(i1-i2) > SENSIBILIDADE)
                blankImage.DrawPixel(x, y,255,255,255);  // exibe um ponto BRANCO na imagem da direita
            else blankImage.DrawPixel(x, y,0,0,0);  // exibe um ponto BRANCO na imagem da direita

        }
    }
*/
    // varredura horizontal
    for (y = 0; y < image.SizeY(); y++) {
        for (x = 0; x < image.SizeX() - 1; x++) {
            i1 = image.GetPointIntensity(x, y); // Le o TOM DE CINZA DA IMAGEM
            i2 = image.GetPointIntensity(x + 1, y); // Le o TOM DE CINZA DA IMAGEM
            if (fabs(i1 - i2) > SENSIBILIDADE)
                blankImage.DrawPixel(x, y, 255, 255, 255);  // exibe um ponto BRANCO na imagem da direita
            else blankImage.DrawPixel(x, y, 0, 0, 0);  // exibe um ponto BRANCO na imagem da direita

        }
    }

    cout << "Iniciou DetectImageBorders...." << endl;
}

// **********************************************************************
// void ConvertToGrayscale()
// **********************************************************************
void ConvertToGrayscale() {
    cout << "Iniciou ConvertToGrayscale..." << endl;
    int x, y;
    int i;
    for (x = 0; x < image.SizeX(); x++) {
        for (y = 0; y < image.SizeY(); y++) {
            i = image.GetPointIntensity(x, y); // Le o TOM DE CINZA DA IMAGEM
            blankImage.DrawPixel(x, y, i, i, i);  // exibe um ponto CINZA na imagem da direita
        }
    }
    cout << "Concluiu ConvertToGrayscale." << endl;
}

// **********************************************************************
// void InvertImage()
// **********************************************************************
void InvertImage() {
    cout << "Iniciou InvertImage..." << endl;
    unsigned char r, g, b;
    int x, y;
    for (x = 0; x < image.SizeX(); x++) {
        for (y = 0; y < image.SizeY(); y++) {
            image.ReadPixel(x, image.SizeY() - y, r, g, b);
            blankImage.DrawPixel(x, y, r, g, b);  // exibe um ponto CINZA na imagem da direita
        }
    }

    cout << "Concluiu InvertImage." << endl;
}

// **********************************************************************
// void OrdenaVetor(int window[])
// **********************************************************************
void OrdenaVetor(int window[]) {
    int temp, i, j;
    for (i = 0; i < 9; i++) {
        temp = window[i];
        for (j = i - 1; j >= 0 && temp < window[j]; j--) {
            window[j + 1] = window[j];
        }
        window[j + 1] = temp;
    }
}

// **********************************************************************
// void MontaVetor(int Px, int Py, int Vetor[9])
// **********************************************************************
void MontaVetor(int Px, int Py, int Vetor[9]) {

}

// **********************************************************************
// void Mediana()
// **********************************************************************
void Mediana() {

}

// **********************************************************************
//  void init(void)
// **********************************************************************
void init(int argc, char **argv) {
    int r;
    // Carrega a uma imagem

    if (argc < 3) {
        cout << "Usage:" << endl <<
             "\t./example_image_manipulation <image_path> <n_seconds>" << endl;
        throw 6;
    }

    r = image.Load(argv[1]); // Carrega uma imagem
    n_seconds = atoi(argv[2]);

    if (!r) exit(1); // Erro na carga da imagem
    else cout << ("Imagem carregada!\n");

    // blankImage.SetSize(image.SizeX(), image.SizeY(), image.Channels());
    blankImage.SetSize(LARGURA_JAN, ALTURA_JAN, image.Channels());
    cout << "Nova Imagem Criada" << endl;

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

double AccumDeltaT = 0;

// **********************************************************************
//  void display( void )
// **********************************************************************
void display(void) {
    double dt = T.getDeltaT();
    AccumDeltaT += dt;

    double fps = 1.0 / dt;

    // Fundo de tela preto
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    // aqui a magia acontece.
    int SfX = LARGURA_JAN - image.SizeX();
    int SiX = 0;
    int SfY = ALTURA_JAN - image.SizeY();
    int SiY = 0;
    // dt é o tempo atual; n_seconds é o tempo que a figura tem pra atravessar a tela
    double xSpeed = ((SfX - SiX) / n_seconds);
    double ySpeed = ((SfY - SiY) / n_seconds);

    int newX = min(SfX, max(0, (int)(xSpeed * AccumDeltaT)));
    int newY = min(SfY, max(0, (int)(ySpeed * AccumDeltaT)));

    // posiciona a imagem nova na metada da direita da janela
    image.SetPos(newX, newY);

    // Coloca as imagens na tela. a ordem importa
    blankImage.SetPos(0, 0);

    blankImage.Display();
    image.Display();

    // Mostra a tela OpenGL
    glutSwapBuffers();

}

// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
// **********************************************************************
void keyboard(unsigned char key, int x, int y) {
    cout << "Passou pela Keyboard." << endl;
    switch (key) {
        case 27: // Termina o programa qdo
            blankImage.Delete();
            image.Delete();
            exit(0);   // a tecla ESC for pressionada
            break;
        case '2':
//        ConvertBlackAndWhite(220, 255);
            ConvertBlackAndWhite();
            glutPostRedisplay();    // obrigatório para redesenhar a tela
            break;
        case 'g':
            ConvertToGrayscale();
            glutPostRedisplay();    // obrigatório para redesenhar a tela
            break;

        case 'b':
            DetectImageBorders();
            glutPostRedisplay();    // obrigatório para redesenhar a tela
            break;
        case 'i':
            InvertImage();
            glutPostRedisplay();    // obrigatório para redesenhar a tela
            break;
        case 'm':
            Mediana();
            glutPostRedisplay();    // obrigatório para redesenhar a tela
            break;
        case 'c':
            blankImage.CopyTo(&image);
            glutPostRedisplay();    // obrigatório para redesenhar a tela
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
void Mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        cout << "(" << x << "," << y << "): ";
        cout << image.GetPointIntensity(x, y) << endl;
        image.DrawPixel(x, y, 255, 0, 0);
        glutPostRedisplay();
    }


}

// **********************************************************************
//  void main ( int argc, char** argv )
// **********************************************************************

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(10, 10);

    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize(LARGURA_JAN, ALTURA_JAN);

    glutCreateWindow("Manipulacao de imagens");

    init(argc, argv);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow_keys);
    glutMouseFunc(Mouse);

    glutIdleFunc(display);

    glutMainLoop();
    return 0;
}


