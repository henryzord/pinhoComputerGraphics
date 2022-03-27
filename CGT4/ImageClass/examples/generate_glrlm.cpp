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
#include <fstream>

using namespace std;

#define N_CLASSES 4  // número de classes no problema
#define N_SHADES 64  // número de tons de cinza das MCOs
#define ALL_SHADES 256  // valor máximo de tom de cinza em uma imagem

#define FILTER_SIDE 3 // diâmetro do filtro deslizado sobre a imagem

#define N_MATRICES 4
int glrlm_filter[N_MATRICES][2] = {{0, 1} ,{1, 0}, {1, -1}, {-1, 1}};

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
 * Desliza um filtro pela imagem
 */
void passGLRLMs(double matrices[N_CLASSES][N_MATRICES][N_SHADES][FILTER_SIDE], ImageClass img, ImageClass gt, int bottomX, int topX, int bottomY, int topY) {

    int filter[FILTER_SIDE][FILTER_SIDE];

    int pixelClass = getCurrentPixelClass(bottomX + (FILTER_SIDE/2) + 1, bottomY + (FILTER_SIDE/2) + 1, gt);

    // preenche o filtro
    int counterX = 0;
    int counterY = 0;
    for(int k = bottomX; k < topX; k++) {
        for(int l = bottomY; l < topY; l++) {
            int pixelColor = (int)((
                float)(int)getGrayscalePixel(k, l, img)/((float)ALL_SHADES/(float)N_SHADES));
            filter[counterX][counterY] = pixelColor;
            counterY += 1;
        }
        counterY = 0;
        counterX += 1;
    }

    // anota valores para matriz de corrida
    for (int k = 0; k < N_MATRICES; k++) {
        counterX = glrlm_filter[k][0] >= 0? 0 : FILTER_SIDE - 1;
        counterY = glrlm_filter[k][1] >= 0? 0 : FILTER_SIDE - 1;

        int oldPixelColor;
        int stride = 0;
        do {
            oldPixelColor = filter[counterX][counterY];

            counterX += glrlm_filter[k][0];
            counterY += glrlm_filter[k][1];

            if(oldPixelColor != filter[counterX][counterY]) {
                matrices[pixelClass][k][oldPixelColor][stride] += 1;  
                stride = 0;
            } else {
                stride += 1;
            }
        } while(counterX > 0 && counterX < FILTER_SIDE && counterY > 0 && counterY < FILTER_SIDE);
        matrices[pixelClass][k][oldPixelColor][stride] += 1;  
    }
}

int main(int argc, char **argv) {
    // Carrega a uma imagem

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

        double matrices[N_CLASSES][N_MATRICES][N_SHADES][FILTER_SIDE];
        for(int c = 0; c < N_CLASSES; c++) {
            for(int m = 0; m < N_MATRICES; m++) {
                for(int s = 0; s < N_SHADES; s++) {
                    for(int f = 0; f < FILTER_SIDE; f++) {
                        matrices[c][m][s][f] = 0.0;
                    }
                }
            }
        }

        for(int k = 0; k < original.SizeX(); k += FILTER_SIDE) {
            for (int l = 0; l < original.SizeY(); l += FILTER_SIDE) {

                passGLRLMs(matrices, original, annotated,
                    k, k + FILTER_SIDE, l, l + FILTER_SIDE
                );
            }
        }

        for (int c = 0; c < N_CLASSES; c++) {
            for (int m = 0; m < N_MATRICES; m++) {
                std::string thisPath(pathwrite.data());
                thisPath.append("/");
                thisPath.append(filenames[i].substr(0, filenames[i].find(".")));
                thisPath.append(std::to_string(i));
                thisPath.append("_");
                thisPath.append(classnames[c]);
                thisPath.append("_");
                thisPath.append(std::to_string(m));
                thisPath.append(".csv");

                std::ofstream out(thisPath.data());

                out << "n_class,n_matrix,n_shade";
                for(int fs = 1; fs <= FILTER_SIDE; fs++) {
                    out << ',' << fs;
                }
                out << '\n';
                for(int s = 0; s < N_SHADES; s++) {
                    out << c << ',' << m << ',' << s ;
                    for(int fs = 0; fs < FILTER_SIDE; fs++) {
                         out << ',' << matrices[c][m][s][fs];
                    }
                    out << '\n';
                }
            }
        }
        counter_files += 1;
        printf("%d/%d done\n", counter_files, n_files);
    }
    return 0;
}


