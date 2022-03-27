#include <math.h>
#include <string.h>
#include <vector>
#include "Texture.h"

#define DEBUG

using namespace std;

// Lista de texturas
vector<TEX*> _texturas(0);

#ifndef __FREEGLUT_EXT_H__
// Função para desenhar um texto na tela com fonte bitmap
void glutBitmapString(void *fonte,char *texto)
{
	// Percorre todos os caracteres
	while (*texto)
    	glutBitmapCharacter(fonte, *texto++);
}

void glutStrokeString(void *fonte,char *texto)
{
	// Percorre todos os caracteres
	while (*texto)
    	glutStrokeCharacter(fonte, *texto++);
}

#endif

// Procura uma textura pelo nome na lista e devolve
// o índice onde está, ou -1 se não achar
int _procuraTextura(char *nome)
{
	unsigned int i;
	for(i=0;i<_texturas.size();++i)
		if(!strcmp(nome,_texturas[i]->nome))
			return i;
	return -1;
}

// Libera memória ocupada pela lista de texturas
void LiberaTexturas()
{
	unsigned int i;
#ifdef DEBUG
	printf("Total de texturas: %lu\n",_texturas.size());
#endif
	// Para cada textura
	for(i=0;i<_texturas.size();++i)
	{
		// Libera textura - não é necessário liberar a imagem, pois esta já
		// foi liberada durante a carga da textura - ver CarregaTextura
#ifdef DEBUG
		printf("%s: %d x %d (id: %d)\n",_texturas[i]->nome,_texturas[i]->dimx,
				_texturas[i]->dimy,_texturas[i]->texid);
#endif
		free(_texturas[i]);
	}
	// Limpa lista
	_texturas.clear();
}

// Função para ler um arquivo JPEG e criar uma
// textura OpenGL
// mipmap = true se deseja-se utilizar mipmaps
TEX *CarregaTextura(string arq, bool mipmap)
{
	GLenum formato;

	if(arq=="")		// retornamos NULL caso nenhum nome de arquivo seja informado
		return NULL;

	char *arquivo = (char *)arq.c_str();
	int indice = _procuraTextura(arquivo);
	// Se textura já foi carregada, retorna
	// apontador para ela
	if(indice!=-1)
		return _texturas[indice];

	TEX *pImage = CarregaJPG(arquivo);	// carrega o arquivo JPEG

	if(pImage == NULL)	// se não foi possível carregar o arquivo, finaliza o programa
		exit(0);

	strcpy(pImage->nome,arquivo);
	// Gera uma identificação para a nova textura
	glGenTextures(1, &pImage->texid);

	// Informa o alinhamento da textura na memória
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Informa que a textura é a corrente
	glBindTexture(GL_TEXTURE_2D, pImage->texid);

	printf("CarregaTextura: %d\n",pImage->texid);
	if(pImage->ncomp==1) formato = GL_LUMINANCE;
	else formato = GL_RGB;

	if(mipmap)
	{
		// Cria mipmaps para obter maior qualidade
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, pImage->dimx, pImage->dimy,
			formato, GL_UNSIGNED_BYTE, pImage->data);
		// Ajusta os filtros iniciais para a textura
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		// Envia a textura para OpenGL, usando o formato RGB
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, pImage->dimx, pImage->dimy,
			0, formato, GL_UNSIGNED_BYTE, pImage->data);
		// Ajusta os filtros iniciais para a textura
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	// Finalmente, libera a memória ocupada pela imagem (já que a textura já foi enviada para OpenGL)

	free(pImage->data); 	// libera a memória ocupada pela imagem

	// Inclui textura na lista
	_texturas.push_back(pImage);
	// E retorna apontador para a nova textura
	return pImage;
}

// Seta o filtro de uma textura específica
// ou de todas na lista (se for passado o argumento -1)
void SetaFiltroTextura(GLint tex, GLint filtromin, GLint filtromag)
{
	glEnable(GL_TEXTURE_2D);
	if(tex!=-1)
	{
		glBindTexture(GL_TEXTURE_2D,tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtromin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtromag);
	}
	else
	for(unsigned int i=0;i<_texturas.size();++i)
	{
		glBindTexture(GL_TEXTURE_2D,_texturas[i]->texid);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtromin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtromag);
	}
	glDisable(GL_TEXTURE_2D);
}

// Decodifica uma imagem JPG e armazena em uma estrutura TEX.
void DecodificaJPG(jpeg_decompress_struct* cinfo, TEX *pImageData, bool inverte)
{
	// Lê o cabeçalho de um arquivo jpeg
	jpeg_read_header(cinfo, TRUE);
	
	// Começa a descompactar um arquivo jpeg com a informação 
	// obtida do cabeçalho
	jpeg_start_decompress(cinfo);

	// Pega as dimensões da imagem e varre as linhas para ler os dados do pixel
	pImageData->ncomp = cinfo->num_components;
	pImageData->dimx  = cinfo->image_width;
	pImageData->dimy  = cinfo->image_height;

	int rowSpan = pImageData->ncomp * pImageData->dimx;
	// Aloca memória para o buffer do pixel
	pImageData->data = new unsigned char[rowSpan * pImageData->dimy];
		
	// Aqui se usa a variável de estado da biblioteca cinfo.output_scanline 
	// como o contador de loop
	
	// Cria um array de apontadores para linhas
	int height = pImageData->dimy - 1;
	unsigned char** rowPtr = new unsigned char*[pImageData->dimy];
	if(inverte)
		for (int i = 0; i <= height; i++)
			rowPtr[height - i] = &(pImageData->data[i*rowSpan]);
	else
		for (int i = 0; i <= height; i++)
			rowPtr[i] = &(pImageData->data[i*rowSpan]);

	// Aqui se extrai todos os dados de todos os pixels
	int rowsRead = 0;
	while (cinfo->output_scanline < cinfo->output_height) 
	{
		// Lê a linha corrente de pixels e incrementa o contador de linhas lidas
		rowsRead += jpeg_read_scanlines(cinfo, &rowPtr[rowsRead], cinfo->output_height - rowsRead);
	}
	
	// Libera a memória
	delete [] rowPtr;

	// Termina a decompactação dos dados 
	jpeg_finish_decompress(cinfo);
}

// Carrega o arquivo JPG e retorna seus dados em uma estrutura tImageJPG.
TEX *CarregaJPG(const char *filename, bool inverte)
{
	struct jpeg_decompress_struct cinfo;
	TEX *pImageData = NULL;
	FILE *pFile;
	
	// Abre um arquivo JPG e verifica se não ocorreu algum problema na abertura
	if((pFile = fopen(filename, "rb")) == NULL) 
	{
		// Exibe uma mensagem de erro avisando que o arquivo não foi encontrado
		// e retorna NULL
		printf("Impossível carregar arquivo JPG: %s\n",filename);
		return NULL;
	}
	
	// Cria um gerenciado de erro
	jpeg_error_mgr jerr;

	// Objeto com informações de compactação para o endereço do gerenciador de erro
	cinfo.err = jpeg_std_error(&jerr);
	
	// Inicializa o objeto de decompactação
	jpeg_create_decompress(&cinfo);
	
	// Especifica a origem dos dados (apontador para o arquivo)	
	jpeg_stdio_src(&cinfo, pFile);
	
	// Aloca a estrutura que conterá os dados jpeg
	pImageData = (TEX*)malloc(sizeof(TEX));

	// Decodifica o arquivo JPG e preenche a estrutura de dados da imagem
	DecodificaJPG(&cinfo, pImageData, inverte);
	
	// Libera a memória alocada para leitura e decodificação do arquivo JPG
	jpeg_destroy_decompress(&cinfo);
	
	// Fecha o arquivo 
	fclose(pFile);

	// Retorna os dados JPG (esta memória deve ser liberada depois de usada)
	return pImageData;
}
