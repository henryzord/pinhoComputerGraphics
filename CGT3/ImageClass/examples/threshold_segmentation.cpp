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

ImageClass original, annotated;
ImageClass dentina, canal, pinos;
ImageClass dentinaHit, canalHit, pinosHit;

int n_images = 8;
ImageClass *allImages[8] = {&original, &annotated, &dentina, &canal, &pinos, &dentinaHit, &canalHit, &pinosHit};

#define LARGURA_JAN 910
#define ALTURA_JAN 466

#define FILTER_SIDE 3

#define X_DIST 22
#define Y_DIST 22
#define Y_PAD 22
#define X_PAD 22
#define ZOOM_LEVEL 0.1

#define N_SHADES 255

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color;


ImageClass postThreshold(ImageClass img, unsigned char lowerThreshold, unsigned char upperThreshold) {
    ImageClass newImg = ImageClass(img.SizeX(), img.SizeY(), img.Channels());
    img.CopyTo(&newImg);
    newImg.SetZoomH(img.GetZoomH());
    newImg.SetZoomV(img.GetZoomV());

    unsigned char r, g, b;
    for(int i = 0; i < newImg.SizeX(); i++) {
        for(int j = 0; j < newImg.SizeY(); j++) {
            newImg.ReadPixel(i, j, r, g, b);
            unsigned char gray_pixel = (unsigned char)((0.3 * r) + (0.59 * g) + (0.11 * b));
            unsigned char new_value;
            if((lowerThreshold <= gray_pixel) && (gray_pixel < upperThreshold)) {
                new_value = N_SHADES;
            } else {
                new_value = 0;
            }
            newImg.DrawPixel(i, j, new_value, new_value, new_value);
        }
    }
    return newImg;
}

bool match(unsigned char r, unsigned char g, unsigned char b, color targetColor) {
    return (r == targetColor.r) && (g == targetColor.g) && (b == targetColor.b);
}

ImageClass diffMatrix(ImageClass proposed, ImageClass groundTruth, int *confMatrix, color targetColor) {
    ImageClass output(original.SizeX(), original.SizeY(), original.Channels());
    proposed.CopyTo(&output);
    output.SetZoomH(proposed.GetZoomH());
    output.SetZoomV(proposed.GetZoomV());

    unsigned char r0, g0, b0;
    unsigned char r1, g1, b1;

    // TP, FP, FN, TN
    for(int i = 0; i < 4; i++) {
        confMatrix[i] = 0;
    }

    for(int i = 0; i < groundTruth.SizeX(); i++) {
        for(int j = 0; j < groundTruth.SizeY(); j++) {
            proposed.ReadPixel(i, j, r0, g0, b0);
            groundTruth.ReadPixel(i, j, r1, g1, b1);

            bool hit = (r0 == 255) && (g0 == 255) && (b0 == 255);

            // se estamos em um pixel da classe positiva
            if(match(r1, g1, b1, targetColor)) {
                if(hit) {  // se acertou
                    output.DrawPixel(i, j, targetColor.r, targetColor.g, targetColor.b);
                    confMatrix[0] += 1; // true positive
                } else {  // se errou
                    output.DrawPixel(i, j, 0, 0, 0);
                    confMatrix[2] += 1;  // false negative
                }
            } else {  // estamos em um pixel da classe negativa
                if(!hit) {  // se classificou como sendo negativo
                    output.DrawPixel(i, j, 255, 255, 255);
                    confMatrix[3] += 1;  // true negative
                } else {  // se errou
                    output.DrawPixel(i, j, 128, 128, 128);
                    confMatrix[1] += 1;  // false positive
                }
            }
        }
    }
    return output;
}

void printConfusionMatrix(int *confusionMatrix) {
    cout << "\t\t\t\t\tActual Class" << endl;
    cout << "\t\t\t\t\t\t1\t\t0" << endl;
    cout << "Predicted class" << "\t" << "1 | " << confusionMatrix[0] << " | " << confusionMatrix[1] << endl;
    cout << "\t\t\t\t" << "0 | " << confusionMatrix[2] << " | " << confusionMatrix[3] << endl;
}


// **********************************************************************
//  void init(void)
// **********************************************************************
void init(int argc, char **argv) {
    // Carrega a uma imagem

    cout << "NOTA: esse programa apresenta problemas carregando uma imagem .bmp com o alpha channel." << endl;

    if (argc < 3) {
        cout << "Usage:" << endl <<
             "\t./threshold_segumentation <original_image_path> <annotated_image_path>" << endl;
        throw 6;
    }

    int r1 = original.Load(argv[1]);
    int r2 = annotated.Load(argv[2]);

    if ((r1 && r2) == 0) {
        exit(1); // Erro na carga da imagem
    } else {
        cout << ("Imagem carregada!\n");
    }

    color dentinaColor = {.r = 0, .g = 255, .b = 0};
    color canalColor = {.r = 255, .g = 0, .b = 0};
    color pinosColor = {.r = 0, .g = 0, .b = 255};

    original.SetZoomH(ZOOM_LEVEL);
    original.SetZoomV(ZOOM_LEVEL);
    original.SetPos(X_PAD, Y_PAD + (original.SizeY() * ZOOM_LEVEL + Y_DIST));

    annotated.SetZoomH(ZOOM_LEVEL);
    annotated.SetZoomV(ZOOM_LEVEL);
    annotated.SetPos(X_PAD, Y_PAD);

    dentina = postThreshold(original, 45, 119);
    dentina.SetPos(X_PAD + (((original.SizeX() * ZOOM_LEVEL) + X_DIST) * 1), Y_PAD + (original.SizeY() * ZOOM_LEVEL + Y_DIST));

    int confMatrix[4];
    dentinaHit = diffMatrix(dentina, annotated, &confMatrix[0], dentinaColor);
    dentinaHit.SetPos(X_PAD + (((original.SizeX() * ZOOM_LEVEL) + X_DIST) * 1), Y_PAD);
    cout << "confusion Matrix for class dentina:" << endl << endl;
    printConfusionMatrix(confMatrix);

    canal = postThreshold(original, 0, 4);
    canal.SetPos(X_PAD + (((original.SizeX() * ZOOM_LEVEL) + X_DIST) * 2), Y_PAD + (original.SizeY() * ZOOM_LEVEL + Y_DIST));
    canalHit = diffMatrix(canal, annotated, &confMatrix[0], canalColor);
    canalHit.SetPos(X_PAD + (((original.SizeX() * ZOOM_LEVEL) + X_DIST) * 2), Y_PAD);
    cout << "confusion Matrix for class canal:" << endl << endl;
    printConfusionMatrix(confMatrix);
    
    pinos = postThreshold(original, 144, 255);
    pinos.SetPos(X_PAD + (((original.SizeX() * ZOOM_LEVEL) + X_DIST) * 3), Y_PAD + (original.SizeY() * ZOOM_LEVEL + Y_DIST));
    pinosHit = diffMatrix(pinos, annotated, &confMatrix[0], pinosColor);
    pinosHit.SetPos(X_PAD + (((original.SizeX() * ZOOM_LEVEL) + X_DIST) * 3), Y_PAD);
    cout << "confusion Matrix for class pinos:" << endl << endl;
    printConfusionMatrix(confMatrix);
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
    // Fundo de tela preto
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    // a ordem aqui importa
    for(int i = 0; i < n_images; i++) {
        allImages[i]->Display();
    }
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
            for(int i = 0; i < n_images; i++) {
                allImages[i]->Display();
            }
            exit(0);   // a tecla ESC for pressionada
            break;
        case '2':
//        ConvertBlackAndWhite(220, 255);
            glutPostRedisplay();    // obrigatório para redesenhar a tela
            break;
        case 'g':
            glutPostRedisplay();    // obrigatório para redesenhar a tela
            break;

        case 'b':
            glutPostRedisplay();    // obrigatório para redesenhar a tela
            break;
        case 'i':
            glutPostRedisplay();    // obrigatório para redesenhar a tela
            break;
        case 'm':
//            Mediana();
            glutPostRedisplay();    // obrigatório para redesenhar a tela
            break;
        case 'c':
//            background.CopyTo(&original);
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
        cout << original.GetPointIntensity(x, y) << endl;
        original.DrawPixel(x, y, 255, 0, 0);
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

    glutCreateWindow("Segmentacao de imagens por limiar");

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


