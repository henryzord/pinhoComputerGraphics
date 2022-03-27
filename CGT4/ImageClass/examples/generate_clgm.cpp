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

#define N_CLASSES 4  // número de classes no problema
#define N_MCOS 3  // número de filtros de matrizes de co-ocorrência de cinza (MCO)
#define N_SHADES 64  // número de tons de cinza das MCOs
#define ALL_SHADES 256  // valor máximo de tom de cinza em uma imagem

#define FILTER_SIDE 7 // diâmetro do filtro deslizado sobre a imagem

// o tamanho do filtro MCO não deve exceder FILTER_SIDE/2 + 1
int MCOS[N_MCOS][2] = {{0, 3}, {3, 0}, {3, -3}};  // filtro MCO

ImageClass dmco0(N_SHADES, N_SHADES, 3), dmco1(N_SHADES, N_SHADES, 3), dmco2(N_SHADES, N_SHADES, 3);
ImageClass cmco0(N_SHADES, N_SHADES, 3), cmco1(N_SHADES, N_SHADES, 3), cmco2(N_SHADES, N_SHADES, 3);
ImageClass pmco0(N_SHADES, N_SHADES, 3), pmco1(N_SHADES, N_SHADES, 3), pmco2(N_SHADES, N_SHADES, 3);
ImageClass bmco0(N_SHADES, N_SHADES, 3), bmco1(N_SHADES, N_SHADES, 3), bmco2(N_SHADES, N_SHADES, 3);

ImageClass *mcos_imgs[N_CLASSES][N_MCOS] = {{&dmco0, &dmco1, &dmco2}, {&cmco0, &cmco1, &cmco2}, {&pmco0, &pmco1, &pmco2}, {&bmco0, &bmco1, &bmco2}};

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color;

ImageClass original, annotated;

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

/**
 * Desliza os filtros MCO sobre uma imagem
 * @param mcos Filtros mco
 * @param img Figura original
 * @param gt Ground truth
 */
void passCGLMs(int mcos[][2], ImageClass img, ImageClass gt) {

    double matrices[N_CLASSES][N_MCOS][N_SHADES * N_SHADES];
    for(int c = 0; c < N_CLASSES; c++) {
        for(int k = 0; k < N_MCOS; k++) {
            for (int m = 0; m < N_SHADES * N_SHADES; m++) {
                matrices[c][k][m] = 0.0;
            }
        }
    }

    int filter[FILTER_SIDE][FILTER_SIDE];

    for(int i = (int)(FILTER_SIDE/2); i <= img.SizeX() - ((int)(FILTER_SIDE/2) + 1); i++) {
        for(int j = (int)(FILTER_SIDE/2); j <= img.SizeY() - ((int)(FILTER_SIDE/2) + 1); j++) {
            int counterX = 0;
            int counterY = 0;

            int pixelClass = getCurrentPixelClass(i, j, gt);

            for(int k = i - (int)(FILTER_SIDE/2); k <= i + (int)(FILTER_SIDE/2); k++) {
                for(int l = j - (int)(FILTER_SIDE/2); l <= j + (int)(FILTER_SIDE/2); l++) {
                    int pixelColor = (int)(
                    	(float)getGrayscalePixel(k, l, img)/((float)ALL_SHADES/(float)N_SHADES));
                    filter[counterX][counterY] = pixelColor;
                    counterY += 1;
                }
                counterY = 0;
                counterX += 1;
            }

            int centralPixelRawValue = filter[(int) (FILTER_SIDE / 2) + 1][(int) (FILTER_SIDE / 2) + 1];

            for (int k = 0; k < N_MCOS; k++) {
                int otherPixelRawValue = filter[
                    ((int)(FILTER_SIDE / 2) + 1 + mcos[k][0])][
                    (int)(FILTER_SIDE / 2) + 1 + (-1 * mcos[k][1])
                ];
                matrices[pixelClass][k][centralPixelRawValue * N_SHADES + otherPixelRawValue] += 1.0;
            }
        }
    }

    for(int c = 0; c < N_CLASSES; c++) {
        for(int k = 0; k < N_MCOS; k++) {
            normalizeMatrix(&matrices[c][k][0]);

            *mcos_imgs[c][k] = ImageClass(N_SHADES, N_SHADES, img.Channels());

            for(int i = 0; i < N_SHADES; i++) {
                for(int j = 0; j < N_SHADES; j++) {
                    mcos_imgs[c][k]->DrawPixel(
                    	i, j,
                        (unsigned char)(matrices[c][k][i * N_SHADES + j] * 255),
                        (unsigned char)(matrices[c][k][i * N_SHADES + j] * 255),
                        (unsigned char)(matrices[c][k][i * N_SHADES + j] * 255)
                    );
                }
            }
        }
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
    std::string classnames[] = {std::string("dentina"), std::string("canal"), std::string("pinos"), std::string("background")};

    int counter_files = 0;
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

        passCGLMs(MCOS, original, annotated);

        for(int c = 0; c < N_CLASSES; c++) {
            for(int m = 0; m < N_MCOS; m++) {
                std::string thisPath(pathwrite.data());
                thisPath.append("/");
                thisPath.append(filenames[i].substr(0, filenames[i].find(".")));
                thisPath.append(std::to_string(i));
                thisPath.append("_");
                thisPath.append(classnames[c]);
                thisPath.append("_");
                thisPath.append(std::to_string(m));
                thisPath.append(".bmp");

                mcos_imgs[c][m]->Save(thisPath.data());
            }
        }
        counter_files += 1;
        printf("%d/%d done\n", counter_files, n_files);
    }
    return 0;
}


