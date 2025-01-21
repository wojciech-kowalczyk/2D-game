#include "renderFunctions.h"

using namespace renderFunctions;

// @format:off

PFNGLGETUNIFORMLOCATIONPROC uniform::glGetUniformLocation;
PFNGLUNIFORM4FPROC          uniform::glUniform4f;
PFNGLUNIFORM1IPROC          uniform::glUniform1i;

PFNWGLCHOOSEPIXELFORMATARBPROC      window::wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC   window::wglCreateContextAttribsARB;

PFNGLCREATESHADERPROC   shader::glCreateShader;
PFNGLCOMPILESHADERPROC  shader::glCompileShader;
PFNGLDELETESHADERPROC   shader::glDeleteShader;
PFNGLATTACHSHADERPROC   shader::glAttachShader;
PFNGLDETACHSHADERPROC   shader::glDetachShader;
PFNGLCREATEPROGRAMPROC  shader::glCreateProgram;
PFNGLLINKPROGRAMPROC    shader::glLinkProgram;
PFNGLSHADERSOURCEPROC   shader::glShaderSource;
PFNGLUSEPROGRAMPROC     shader::glUseProgram;
PFNGLDELETEPROGRAMPROC  shader::glDeleteProgram;

PFNGLGENVERTEXARRAYSPROC            vertexArray::glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC            vertexArray::glBindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC         vertexArray::glDeleteVertexArrays;
PFNGLENABLEVERTEXATTRIBARRAYPROC    vertexArray::glEnableVertexAttribArray;
PFNGLDISABLEVERTEXATTRIBARRAYPROC   vertexArray::glDisableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC        vertexArray::glVertexAttribPointer;

PFNGLGENBUFFERSPROC     buffer::glGenBuffers;
PFNGLBINDBUFFERPROC     buffer::glBindBuffer;
PFNGLBUFFERDATAPROC     buffer::glBufferData;
PFNGLDELETEBUFFERSPROC  buffer::glDeleteBuffers;

PFNGLACTIVETEXTUREPROC  texture::glActiveTexture;
PFNGLGENERATEMIPMAPPROC texture::glGenerateMipmap;

static bool initDone {false};

void renderFunctions::init() {
	if (!initDone) {
		// uniform
		{
			using namespace uniform;
			glGetUniformLocation =  reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(retrieveFunctionPtr("glGetUniformLocation"));
			glUniform4f =           reinterpret_cast<PFNGLUNIFORM4FPROC>(retrieveFunctionPtr("glUniform4f"));
			glUniform1i =           reinterpret_cast<PFNGLUNIFORM1IPROC>(retrieveFunctionPtr("glUniform1i"));
		}
		
		// window
		{
			using namespace window;
			wglChoosePixelFormatARB =    reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(renderFunctions::retrieveFunctionPtr("wglChoosePixelFormatARB"));
			wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(renderFunctions::retrieveFunctionPtr("wglCreateContextAttribsARB"));
		}
		
		// shader
		{
			using namespace shader;
			glCreateShader =    reinterpret_cast<PFNGLCREATESHADERPROC>(retrieveFunctionPtr("glCreateShader"));
			glCompileShader =   reinterpret_cast<PFNGLCOMPILESHADERPROC>(retrieveFunctionPtr("glCompileShader"));
			glDeleteShader =    reinterpret_cast<PFNGLDELETESHADERPROC>(retrieveFunctionPtr("glDeleteShader"));
			glAttachShader =    reinterpret_cast<PFNGLATTACHSHADERPROC>(retrieveFunctionPtr("glAttachShader"));
			glDetachShader =    reinterpret_cast<PFNGLDETACHSHADERPROC>(retrieveFunctionPtr("glDetachShader"));
			glCreateProgram =   reinterpret_cast<PFNGLCREATEPROGRAMPROC>(retrieveFunctionPtr("glCreateProgram"));
			glLinkProgram =     reinterpret_cast<PFNGLLINKPROGRAMPROC>(retrieveFunctionPtr("glLinkProgram"));
			glShaderSource =    reinterpret_cast<PFNGLSHADERSOURCEPROC>(retrieveFunctionPtr("glShaderSource"));
			glUseProgram =      reinterpret_cast<PFNGLUSEPROGRAMPROC>(retrieveFunctionPtr("glUseProgram"));
			glDeleteProgram =   reinterpret_cast<PFNGLDELETEPROGRAMPROC>(retrieveFunctionPtr("glDeleteProgram"));
		}
		
		// vertexArray
		{
			using namespace vertexArray;
			glGenVertexArrays =             reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>(retrieveFunctionPtr("glGenVertexArrays"));
			glBindVertexArray =             reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>(retrieveFunctionPtr("glBindVertexArray"));
			glDeleteVertexArrays =          reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>(retrieveFunctionPtr("glDeleteVertexArrays"));
			glEnableVertexAttribArray =     reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(retrieveFunctionPtr("glEnableVertexAttribArray"));
			glDisableVertexAttribArray =    reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYPROC>(retrieveFunctionPtr("glDisableVertexAttribArray"));
			glVertexAttribPointer =         reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>(retrieveFunctionPtr("glVertexAttribPointer"));
		}
		
		// buffer
		{
			using namespace buffer;
			glGenBuffers =      reinterpret_cast<PFNGLGENBUFFERSPROC>(retrieveFunctionPtr("glGenBuffers"));
			glBindBuffer =      reinterpret_cast<PFNGLBINDBUFFERPROC>(retrieveFunctionPtr("glBindBuffer"));
			glBufferData =      reinterpret_cast<PFNGLBUFFERDATAPROC>(retrieveFunctionPtr("glBufferData"));
			glDeleteBuffers =   reinterpret_cast<PFNGLDELETEBUFFERSPROC>(retrieveFunctionPtr("glDeleteBuffers"));
		}
		
		// texture
		{
			using namespace texture;
			glActiveTexture =   reinterpret_cast<PFNGLACTIVETEXTUREPROC>(retrieveFunctionPtr("glActiveTexture"));
			glGenerateMipmap =  reinterpret_cast<PFNGLGENERATEMIPMAPPROC>(retrieveFunctionPtr("glGenerateMipmap"));
		}
		
		initDone = true;
	}
}

// @format:on

void *renderFunctions::retrieveFunctionPtr(char const *const functionName) {
	void *functionPtr = reinterpret_cast<void *>(wglGetProcAddress(functionName));
	
	// Different implementations can return different failure codes (-1, 0, 1, 2, 3)
	if (functionPtr == reinterpret_cast<void *>(-1) || functionPtr == nullptr || functionPtr == reinterpret_cast<void *>(1) ||
	    functionPtr == reinterpret_cast<void *>(2) || functionPtr == reinterpret_cast<void *>(3)) {
		HINSTANCE__ *const module = LoadLibraryA("opengl32.dll");
		
		// Functions from openGl 1.1 can only be obtained by GetProcAddress()
		functionPtr = reinterpret_cast<void *>(GetProcAddress(module, functionName));
	}
	
	return functionPtr;
}
