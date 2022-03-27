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


#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#include "ImageClass.h"

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <limits>
#include <dirent.h>

using namespace std;

#define N_MCOS 3  // number of MCOs
#define N_SHADES 64  // number of grey shades
#define ALL_SHADES 256  // maximum shade value

#define WINDOW_SIZE 64
#define FILTER_SIDE 7 // side (i.e. diameter) of filter used to parse window

// MCO value should not exceed FILTER_SIDE/2
int MCOS[N_MCOS][2] = {{0, 3}, {3, 0}, {3, -3}};  // grey-level co-occurence matrices

#define LARGURA_JAN 910
#define ALTURA_JAN 600

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color;

ImageClass original, annotated;
ImageClass mco0(N_SHADES, N_SHADES, 3), mco1(N_SHADES, N_SHADES, 3), mco2(N_SHADES, N_SHADES, 3);

#define N_CLASSES 4

ImageClass *mcos_imgs[N_MCOS] = {&mco0, &mco1, &mco2};


/**
 * Conta os arquivos em um diretório.
 * @param path Diretório a ser investigado
 * @return Número de arquivos no diretório
 */
int countFilesDir(char *path) {
    DIR *dir;
    struct dirent *ent;
    int n_files = 0;
    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            std::string filename(ent->d_name);
            if(filename.size() > 2) {
                n_files += 1;
            }

        }
        closedir(dir);
    } else {
        perror ("");
        exit(EXIT_FAILURE);
    }
    return n_files;
}


/**
 * Lista os arquivos de um diretório.
 * @param path Diretório a ser investigado
 * @return Um array de std::string com o nome de cada arquivo no diretório
 */
std::string *listDir(char *path) {
    DIR *dir;
    struct dirent *ent;

    int n_files = countFilesDir(path);
    std::string *filenames = new std::string[n_files];

    int counter = 0;
    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            std::string filename(ent->d_name);
            if(filename.size() > 2) {
                filenames[counter] = filename;
                counter += 1;
            }
        }
        closedir(dir);
    } else {
        perror ("");
        exit(EXIT_FAILURE);
    }
    return filenames;
}

/**
 * Pega o pixel (em tom de cinza) de uma determinada posição na imagem.
 * @param i Eixo x
 * @param j Eixo y
 * @param img Imagem
 * @return o tom de cinza do pixel
 */
int getGrayscalePixel(int i, int j, ImageClass img) {
    unsigned char r, g, b;
    img.ReadPixel(i, j, r, g, b);
    int gray_pixel = (int)((0.3 * r) + (0.59 * g) + (0.11 * b));
    return gray_pixel;
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

/**
 * Pega a classe do pixel atual.
 *
 * @param i Eixo x
 * @param j Eixo y
 * @param gt Imagem ground truth
 * @return A classe do pixel: 0 para dentina; 1 para canal; 2 para pino; 3 para background
 */
int getCurrentPixelClass(int i, int j, ImageClass gt) {
    unsigned char r, g, b;
    gt.ReadPixel(i, j, r, g, b);

    if(g > 250 && g <= 255) {
        return 0;  // dentina
    }
    if(r > 250 && r <= 255) {
        return 1;  // canal
    }
    if(b > 250 && b <= 255) {
        return 2;  // pino
    }
    return 3;  // background
}


void passCGLMWindow(int mcos[][2], ImageClass img, ImageClass gt, int bottomX, int topX, int bottomY, int topY, const std::string intro, int counterRegion) {

    double matrices[N_MCOS][N_SHADES * N_SHADES];

    for(int k = 0; k < N_MCOS; k++) {
        for (int m = 0; m < N_SHADES * N_SHADES; m++) {
            matrices[k][m] = 0.0;
        }
    }
    int filter[FILTER_SIDE][FILTER_SIDE];
    std::string classnames[] = {std::string("dentina"), std::string("canal"), std::string("pinos"), std::string("background")};
    unsigned int count_classes[N_CLASSES];
    for(int i = 0; i < N_CLASSES; i++) {
        count_classes[i] = 0;
    }

    for(int i = bottomX + (int)(FILTER_SIDE/2); i <= topX - ((int)(FILTER_SIDE/2) + 1); i++) {
        for(int j = bottomY + (int)(FILTER_SIDE/2); j <= topY - ((int)(FILTER_SIDE/2) + 1); j++) {
            int counterX = 0;
            int counterY = 0;

            for(int k = i - (int)(FILTER_SIDE/2); k <= i + (int)(FILTER_SIDE/2); k++) {
                for(int l = j - (int)(FILTER_SIDE/2); l <= j + (int)(FILTER_SIDE/2); l++) {
                    int pixelColor = (int)((float)(int)getGrayscalePixel(k, l, img)/((float)ALL_SHADES/(float)N_SHADES));
                    count_classes[getCurrentPixelClass(k, l, gt)] += 1;
                    filter[counterX][counterY] = pixelColor;
                    counterY += 1;
                }
                counterY = 0;
                counterX += 1;
            }

            int centralPixelRawValue = filter[(int) (FILTER_SIDE / 2) + 1][(int) (FILTER_SIDE / 2) + 1];

            for (int k = 0; k < N_MCOS; k++) {
                int otherPixelRawValue = filter[
                    ((int)(FILTER_SIDE / 2) + 1 + mcos[k][0])][  // sets at center pixel of filter + moves mcos[k][0] positions
                    (int)(FILTER_SIDE / 2) + 1 + (-1 * mcos[k][1])  //
                ];
                matrices[k][centralPixelRawValue * N_SHADES + otherPixelRawValue] += 1.0;
            }
        }
    }

    int index_class = -1;
    unsigned int max_count = numeric_limits<unsigned int>::min();
    for(int i = 0; i < N_CLASSES; i++) {
        if(count_classes[i] > max_count) {
            max_count = count_classes[i];
            index_class = i;
        }
    }

    for(int k = 0; k < N_MCOS; k++) {
        normalizeMatrix(&matrices[k][0]);

        *mcos_imgs[k] = ImageClass(N_SHADES, N_SHADES, img.Channels());

        for(int i = 0; i < N_SHADES; i++) {
            for(int j = 0; j < N_SHADES; j++) {
                mcos_imgs[k]->DrawPixel(i, j,
                                        (unsigned char)(matrices[k][i * N_SHADES + j] * 255),
                                        (unsigned char)(matrices[k][i * N_SHADES + j] * 255),
                                        (unsigned char)(matrices[k][i * N_SHADES + j] * 255)
                );
            }
        }
        std::string pathwrite(intro.data());

        pathwrite.append(std::to_string(counterRegion));
        pathwrite.append("_");
        pathwrite.append(classnames[index_class]);
        pathwrite.append("_");
        pathwrite.append(std::to_string(k));
        pathwrite.append(".bmp");

        mcos_imgs[k]->Save(pathwrite.data());

    }
}

int main(int argc, char **argv) {
    cout << "NOTA: esse programa apresenta problemas carregando uma imagem .bmp com o alpha channel." << endl;

    if (argc < 4) {
        cout << "Usage:" << endl <<
             "\t./threshold_segumentation <original_image_path> <annotated_image_path> <write_path>" << endl;
        throw 6;
    }

    std::string *filenames = listDir(argv[2]);
    int n_files = countFilesDir(argv[2]);

    std::string pathwrite(argv[3]);

    int counter_files = 0;
    for(int f = 0; f < n_files; f++) {
        std::string gtPath = std::string(argv[2]);
        gtPath.append("/");
        gtPath.append(filenames[f]);
        std::string orPath = std::string(argv[1]);
        orPath.append("/");
        orPath.append(filenames[f]);

        int r2 = annotated.Load(gtPath.data());

        if (r2 == 0) {
            exit(1); // Erro na carga da imagem
        }

        int r1 = original.Load(orPath.data());
        if(r1 == 0) {
            exit(1);
        }
        int counterRegion = 0;

        std::string thisPath(pathwrite.data());
        thisPath.append("/");
        thisPath.append(filenames[f].substr(0, filenames[f].find(".")));
        thisPath.append("_region_");

        for(int i = 0; i < original.SizeX(); i += WINDOW_SIZE) {
            for(int j = 0; j < original.SizeY(); j += WINDOW_SIZE) {
                passCGLMWindow(MCOS, original, annotated, i, i + WINDOW_SIZE, j, j + WINDOW_SIZE, thisPath, counterRegion);
                counterRegion += 1;
            }
        }

        counter_files += 1;
        printf("%d/%d done\n", counter_files, n_files);
    }
    return 0;
}


