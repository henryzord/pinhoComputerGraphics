#ifndef TEXTURE_H
#define TEXTURE_H


#include <stdio.h>
#include <stdlib.h>
#include <string>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

extern "C" {
	#include <jpeglib.h>
}

#ifndef M_PI
#define M_PI 3.1415926
#endif

// Define a estrutura de uma imagem
typedef struct
{
    char nome[50];	// nome do arquivo carregado
	int ncomp;		// número de componentes na textura (1-intensidade, 3-RGB)
	GLint dimx;		// largura 
	GLint dimy;		// altura
    GLuint texid;	// identifição da textura em OpenGL
	unsigned char *data;	// apontador para a imagem em si
} TEX;

// Funções para liberação de memória
void LiberaTexturas();

// Funções para manipulação de texturas e materiais
TEX *CarregaTextura(std::string, bool mipmap);
void SetaFiltroTextura(GLint tex, GLint filtromin, GLint filtromag);
TEX *CarregaJPG(const char *filename, bool inverte=true);

#endif
