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

ImageClass original, postFilter, blankImage;

#define LARGURA_JAN 800
#define ALTURA_JAN 300

#define FILTER_SIDE 3

/**
 * Troca os valores entre duas variáveis.
 * @param a primeiro valor
 * @param b segundo valor
 */
void swap(unsigned char* a, unsigned char* b) {
    unsigned char t = *a;
    *a = *b;
    *b = t;
}

/**
 * Parte que faz propriamente a ordenação de valores.
 *
 * @param arr Array
 * @param low Menor índice desse array
 * @param high Maior índice desse array
 * @return
 */
unsigned char partition(unsigned char arr[], int low, int high) {
    unsigned char pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/**
 * Algoritmo principal do quicksort. Ordena um array in-place.
 *
 * @param arr Array de valores a serem ordenados.
 * @param low Menor índice desse array
 * @param high Maior índice desse array
 */
void quickSort(unsigned char arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/**
 * Pega (se possível) o valor grayscale de um pixel da imagem.
 *
 * @param i Posição no eixo x do pixel. Se exceder a imagem, o valor da borda
 *          na altura j será retornado no lugar.
 * @param j Posição no eixo y do pixel. Se exceder a imagem, o valor da borda
 *          na largura i será retornado no lugar.
 * @param img Imagem de onde o pixel será retirado
 * @return O valor (grayscale) do pixel na posição (i, j).
 */
unsigned char getGrayscalePixel(int i, int j, ImageClass img) {
    unsigned char r, g, b;
    if(i < 0) {
        i = 0;
    } else if(i >= img.SizeX()) {
        i = img.SizeX() - 1;
    }
    if(j < 0) {
        j = 0;
    } else if(j >= img.SizeY()) {
        j = img.SizeY() - 1;
    }
    img.ReadPixel((GLint)i, (GLint)j, r, g, b);
    unsigned char gray_pixel = (unsigned char)((0.3 * r) + (0.59 * g) + (0.11 * b));
    return gray_pixel;
}

/**
 * Captura uma determinada região da imagem.
 *
 * @param side Lado da máscara.
 * @param centerX Coordanada x do centro da máscara
 * @param centerY Coordanada y do centro da máscara
 * @param img Imagem
 * @return a máscara, com valores atualizados.
 */
unsigned char *getMask(int side, int centerX, int centerY, ImageClass img) {
    int min_x = centerX - (int)(side / 2);
    int max_x = centerX + (int)(side / 2);
    int min_y = centerY - (int)(side / 2);
    int max_y = centerY + (int)(side / 2);

    unsigned char *mask = new unsigned char [side * side];

    int counter = 0;
    for(int i = min_x; i <= max_x; i++) {
        for(int j = min_y; j <= max_y; j++) {
            mask[counter] = getGrayscalePixel(i, j, img);
            counter += 1;
        }
    }
    return mask;
}

/**
 * Pega o valor mediano de uma posição na imagem.
 *
 * @param side Lado da máscara
 * @param i Posição central no eixo x da máscara
 * @param j Posição central no eixo y da máscara
 * @param img Imagem
 * @return O valor mediano, quando aplicada uma máscara (de centro (i, j)) sobre uma imagem.
 */
unsigned char medianLapFilterToRegion(int side, int i, int j, ImageClass img) {
    unsigned char *mask = getMask(side, i, j, img);
    quickSort(mask, 0, side * side);
    if(((side * side) % 2) == 0) {
        return (mask[(side * side) / 2] + mask[((side * side) / 2) + 1]) / 2;
    }
    unsigned char new_pixel = mask[(side * side) / 2];
    delete[] mask;
    return new_pixel;
}

/**
 * Aplica um filtro mediano sobre uma figura inteira. Pula de FILTER_SIDE em FILTER_SIDE
 * Gera uma nova figura.
 *
 * @param img Imagem que será processada.
 * @return A mesma imagem, agora pós passada do filtro mediano.
 */
ImageClass meanFilterLap(ImageClass img) {
    int length = FILTER_SIDE * FILTER_SIDE;
    ImageClass newImg = ImageClass(img.SizeX(), img.SizeY(), img.Channels());
    img.CopyTo(&newImg);

    int half_side = (int)FILTER_SIDE/2;

    for (int i = half_side; i < img.SizeX(); i += FILTER_SIDE) {
        for (int j = half_side; j < img.SizeY(); j += FILTER_SIDE) {
            unsigned char new_pixel = medianLapFilterToRegion(FILTER_SIDE, i, j, img);
            
            for(int k = (i - half_side); k <= (i + half_side); k++) {
                for(int l = (j - half_side); l <= (j + half_side); l++) {
                    if(k < 0) {
                        k = 0;
                    } else if(k >= img.SizeX()) {
                        k = img.SizeX() - 1;
                    }
                    if(l < 0) {
                        l = 0;
                    } else if(l >= img.SizeY()) {
                        l = img.SizeY() - 1;
                    }    
//                    cout << "processed pixel (" << i << "," << j << ")" << endl;
                    newImg.DrawPixel(k, l, new_pixel);
                }
            }
        }
    }
    return newImg;
}


// **********************************************************************
//  void init(void)
// **********************************************************************
void init(int argc, char **argv) {
    int r;
    // Carrega a uma imagem

    if (argc < 2) {
        cout << "Usage:" << endl <<
             "\t./example_image_manipulation <image_path>" << endl;
        throw 6;
    }

    r = original.Load(argv[1]); // Carrega uma imagem

    if (!r) {
        exit(1); // Erro na carga da imagem
    } else {
        cout << ("Imagem carregada!\n");
    }

    postFilter = meanFilterLap(original);

    blankImage.SetSize(LARGURA_JAN, ALTURA_JAN, original.Channels());
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
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    blankImage.SetPos(0, 0);
    original.SetPos(8, 22);
    postFilter.SetPos(original.SizeX() + 16, 22);

    // a ordem aqui importa
    blankImage.Display();
    original.Display();
    postFilter.Display();

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
            original.Delete();
            postFilter.Delete();
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
//            blankImage.CopyTo(&original);
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

    glutCreateWindow("Filtro mediano (pulando)");

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


