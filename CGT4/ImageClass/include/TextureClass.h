// *****************************************************************************************
//  ImageClass.h
// *****************************************************************************************

#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

#ifdef _MSC_VER

#endif

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
#endif

#ifdef __APPLE__
/* Para usar o include da GLUT desta forma, adicione as
seguintes clausulas nas configurac›es do Linker:
-framework CoreFoundation
-framework GLUT
-framework OpenGL
*/
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "SOIL/SOIL.h"
#include "ImageClass.h"


GLuint LoadTexture (const char *nomeTex);

