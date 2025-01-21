#ifndef RENDERFUNCTIONS_H
#define RENDERFUNCTIONS_H

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

namespace renderFunctions {
	/// Require active context
	void init();
	void *retrieveFunctionPtr(char const *functionName);
	
	namespace uniform {
		extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
		extern PFNGLUNIFORM4FPROC glUniform4f;
		extern PFNGLUNIFORM1IPROC glUniform1i;
	}
	
	namespace window {
		extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
		extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
	}
	
	namespace shader {
		extern PFNGLCREATESHADERPROC glCreateShader;
		extern PFNGLCOMPILESHADERPROC glCompileShader;
		extern PFNGLDELETESHADERPROC glDeleteShader;
		extern PFNGLATTACHSHADERPROC glAttachShader;
		extern PFNGLDETACHSHADERPROC glDetachShader;
		extern PFNGLCREATEPROGRAMPROC glCreateProgram;
		extern PFNGLLINKPROGRAMPROC glLinkProgram;
		extern PFNGLSHADERSOURCEPROC glShaderSource;
		extern PFNGLUSEPROGRAMPROC glUseProgram;
		extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
	}
	
	namespace vertexArray {
		extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
		extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
		extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
		extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
		extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
		extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	}
	
	namespace buffer {
		extern PFNGLGENBUFFERSPROC glGenBuffers;
		extern PFNGLBINDBUFFERPROC glBindBuffer;
		extern PFNGLBUFFERDATAPROC glBufferData;
		extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	}
	
	namespace texture {
		extern PFNGLACTIVETEXTUREPROC glActiveTexture;
		extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
	}
}

#endif // RENDERFUNCTIONS_H
