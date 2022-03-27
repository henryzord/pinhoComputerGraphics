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

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <map>
#include <limits>
#include <dirent.h>

#define X_DIST 22
#define Y_DIST 22
#define Y_PAD 22
#define X_PAD 22
#define ZOOM_LEVEL 0.3

// specific to current work

#define N_SHADES 64  // number of grey shades
#define ALL_SHADES 256  // maximum shade value

#define FILTER_SIDE 7
#define WINDOW_SIZE 16

#define LARGURA_JAN 910
#define ALTURA_JAN 600

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color;

ImageClass original, annotated;
ImageClass cf0, cf1, cf2, cf3, cf4;

#define N_IMAGES_TEST 5
ImageClass *testImages[N_IMAGES_TEST] = {&cf0, &cf1, &cf2, &cf3, &cf4};
int displayIndex = 0;

int cglm_filter[2] = {3, -3};

inline bool file_exists(const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

int countFilesDir(char *path) {
    DIR *dir;
    struct dirent *ent;
    int n_files = 0;
    if ((dir = opendir(path)) != NULL) {
        // print all the files and directories within directory
        while ((ent = readdir (dir)) != NULL) {
            std::string filename(ent->d_name);
            if(filename.size() > 2) {
                n_files += 1;
            }

        }
        closedir(dir);
    } else {
        //could not open directory */
        perror ("");
        exit(EXIT_FAILURE);
    }
    return n_files;
}


std::string *listDir(char *path) {
    DIR *dir;
    struct dirent *ent;

    int n_files = countFilesDir(path);
    std::string *filenames = new std::string[n_files];

    int counter = 0;
    if ((dir = opendir(path)) != NULL) {
        // print all the files and directories within directory
        while ((ent = readdir (dir)) != NULL) {
            std::string filename(ent->d_name);
            if(filename.size() > 2) {
                filenames[counter] = filename;
                counter += 1;
            }
        }
        closedir(dir);
    } else {
        //could not open directory */
        perror ("");
        exit(EXIT_FAILURE);
    }
    return filenames;
}

int getGrayscalePixel(int i, int j, ImageClass img) {
    unsigned char r, g, b;
    img.ReadPixel(i, j, r, g, b);
    int gray_pixel = (int)((0.3 * r) + (0.59 * g) + (0.11 * b));
    return gray_pixel;
}

bool match(unsigned char r, unsigned char g, unsigned char b, color targetColor) {
    return (r == targetColor.r) && (g == targetColor.g) && (b == targetColor.b);
}


/**
 * Normaliza pelo valor máximo.
 * @param matrix
 */
void normalizeMatrix(double *matrix) {
    double max_value = numeric_limits<double>::min();
    for(int i = 0; i < N_SHADES; i++) {
        for(int j = 0; j < N_SHADES; j++) {
            max_value = max(matrix[i * N_SHADES + j], max_value);
        }
    }

    for(int i = 0; i < N_SHADES; i++) {
        for(int j = 0; j < N_SHADES; j++) {
            matrix[i * N_SHADES + j] /= max_value;
        }
    }
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



int getCurrentPixelClass(int i, int j, ImageClass gt) {
    unsigned char r, g, b;
    gt.ReadPixel(i, j, r, g, b);
    // dentina
    if(r == 0 && g == 255 && b == 0) {
        return 0;
    }
    // canal
    if(r == 255 && g == 0 && b == 0) {
        return 1;
    }
    // pinos
    if(r == 0 && g == 0 && b == 255) {
        return 2;
    }
    return 3;  // background
}

void classifyRegion(ImageClass img, ImageClass gt, ImageClass *target, int bottomX, int topX, int bottomY, int topY, int n_windows) {
    int filter[FILTER_SIDE][FILTER_SIDE];

    double cglm[N_SHADES * N_SHADES];
    for(int k = 0; k < N_SHADES; k++) {
        for(int l = 0; l < N_SHADES; l++) {
            cglm[k * N_SHADES + l] = 0.0;
        }
    }
    for(int i = bottomX + (int)(FILTER_SIDE/2); i < topX - (int)(FILTER_SIDE/2); i++) {
        for(int j = bottomY + (int)(FILTER_SIDE/2); j < topY - (int)(FILTER_SIDE/2); j++) {
            int counterX = 0;
            int counterY = 0;

            // fulfills filter
            for(int k = i - (int)(FILTER_SIDE/2); k <= i + (int)(FILTER_SIDE/2); k++) {
                for(int l = j - (int)(FILTER_SIDE/2); l <= j + (int)(FILTER_SIDE/2); l++) {
                    int pixelColor = (int)((float)(int)getGrayscalePixel(k, l, img)/((float)ALL_SHADES/(float)N_SHADES));
                    filter[counterX][counterY] = pixelColor;
                    counterY += 1;
                }
                counterY = 0;
                counterX += 1;
            }

            // computes metric
            int centralPixelRawValue = filter[(int) (FILTER_SIDE / 2) + 1][(int) (FILTER_SIDE / 2) + 1];

            int otherPixelRawValue = filter[
                ((int)(FILTER_SIDE / 2) + 1 + cglm_filter[0])][
                (int)(FILTER_SIDE / 2) + 1 + (-1 * cglm_filter[1])
            ];
            cglm[centralPixelRawValue * N_SHADES + otherPixelRawValue] += 1.0;  //  * (n_windows * n_windows);  // TODO testing!
        }
    }

    normalizeMatrix(&cglm[0]);
    double contrast = 0;
    double homogeneity = 0;
    double _asm = 0;
    double variance = 0;
    double sum = 0;
    for(int i = 0; i < N_SHADES; i++) {
        for(int j = 0; j < N_SHADES; j++) {
            sum += cglm[i * N_SHADES + j];
            homogeneity += cglm[i * N_SHADES + j] * (cglm[i * N_SHADES + j]/(1 + (double)pow(i - j, 2)));
            contrast += cglm[i * N_SHADES + j] * (double)pow(i - j, 2);
            _asm += pow(cglm[i * N_SHADES + j], 2.0);
            variance += cglm[i * N_SHADES + j] * (double)abs(i - j);
        }
    }
    contrast /= sum;
    homogeneity /= sum;
    _asm /= sum;
    variance /= sum;
    double entropy = sqrt(_asm);

    printf("contrast: %f homogeneity: %f _asm: %f entropy: %f variance: %f\n", contrast, homogeneity, _asm, entropy, variance);

    unsigned char r = 0, g = 0, b = 0;
    // kind of works
    if(variance >= 8 && variance <= 9) {
        r = 255;  // canal
    } else if(variance >= 9 && variance <= 19) {  // 43 a 46
        g = 255;  // dentina
    } else if(variance >= 19) {
        b = 255;  // pinos  // 1 a 8
    }


    for(int i = bottomX; i < topX; i++) {
        for (int j = bottomY; j < topY; j++) {
            target->DrawPixel(i, j, r, g, b);
        }
    }
}

void classifyImage(ImageClass img, ImageClass gt, ImageClass *target) {
    int n_windows = img.SizeX() / WINDOW_SIZE;

    int counter_windows = 0;
    for(int i = 0; i < img.SizeX(); i += WINDOW_SIZE) {
        for(int j = 0; j < img.SizeY(); j += WINDOW_SIZE) {
            classifyRegion(img, gt, target, i, i + WINDOW_SIZE, j, j + WINDOW_SIZE, n_windows);
            counter_windows += 1;
            printf("on window %d/%d\n", counter_windows, n_windows * n_windows);
        }
    }
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

    color dentinaColor = {.r = 0, .g = 255, .b = 0};
    color canalColor = {.r = 255, .g = 0, .b = 0};
    color pinosColor = {.r = 0, .g = 0, .b = 255};
    
    color *classColors[] = {&dentinaColor, &canalColor, &pinosColor};

    std::string *filenames = listDir(argv[2]);
    int n_files = countFilesDir(argv[2]);

    for(int i = 0; i < n_files; i++) {
        std::string gtPath = std::string(argv[2]);
        gtPath.append("/");
        gtPath.append(filenames[i]);
        std::string orPath = std::string(argv[1]);
        orPath.append("/");
        orPath.append(filenames[i]);

        int r2 = annotated.Load(gtPath.data());

        if (r2 == 0) {
            exit(1); // Erro na carga da imagem
        }

        int r1 = original.Load(orPath.data());
        if(r1 == 0) {
            exit(1);
        }

        *testImages[i] = ImageClass(640, 640, 3);
        testImages[i]->SetPos(22 + (640 * displayIndex), 22);
        testImages[i]->SetZoomH(0.9);
        testImages[i]->SetZoomV(0.9);

        classifyImage(original, annotated, testImages[i]);
        displayIndex += 1;

        printf("WARNING: breaking after first image!");
        break;
    }
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    // a ordem aqui importa
    for(int i = 0; i < displayIndex; i++) {
        testImages[i]->Display();
    }

//    original.SetZoomH(0.5);
//    original.SetZoomV(0.5);
//    // TODO remove later!
//    original.Display();

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
            for(int i = 0; i < N_IMAGES_TEST; i++) {
                testImages[i]->Display();
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

    glutCreateWindow("Segmentacao de imagens por texturas");

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


