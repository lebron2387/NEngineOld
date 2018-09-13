#pragma once


#ifdef _WIN32
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <setjmp.h>
//D3D
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#include "../include/DX/d3d9.h"
#include "../include/DX/d3dx9.h"
#include "../include/DX/d3dx9shader.h"
#endif
//OpenGL
#pragma comment(lib,"opengl32.lib")
#include "../include/GL/gl.h"
#include "../include/GL/glext.h"

//Cg
#pragma comment(lib,"cg.lib")
#pragma comment(lib,"cgD3D9.lib")
#pragma comment(lib,"cgGL.lib")
#include "../include/Cg/cg.h"
//#include "../include/Cg/cgD3D8.h"
#include "../include/Cg/cgD3D9.h"
//#include "../include/Cg/cgD3D10.h"
#include "../include/Cg/cgGL.h"

#include "MemoryManager.h"

#pragma warning (disable: 4996)
#pragma warning (disable: 4067)//The compiler found, and ignored, extra characters following a preprocessor directive. This warning appears only under ANSI compatibility
#pragma warning (disable: 4291)//no matching operator delete found; memory will not be freed if initialization throws an exception
#pragma warning (disable: 4482)//To fix by using the following possible solutionsCompile with /Ze. Use comment delimiters:

__forceinline void Trace(const char *format,...)
{
#ifdef _DEBUG
	char buf[4096];
	va_list	va;								
	va_start(va,format);
	vsprintf(buf, format, va);
	va_end(va);
	OutputDebugString(buf);
#endif //_DEBUG
}

__forceinline void * operator new(unsigned int size,char *file,int line) 
{
	return CMemoryManager::MemoryAllocate(size,file,line);
};

__forceinline void * operator new [] (unsigned int size,char *file,int line) 
{
	return CMemoryManager::MemoryAllocate(size,file,line);
};

__forceinline void operator delete [] (void *p) 
{
	CMemoryManager::MemoryDeallocate(p);
};

__forceinline void operator delete(void *p) 
{
	CMemoryManager::MemoryDeallocate(p);
};

__forceinline void * ReAlloc(void *p,unsigned int size,char *file,int line)
{
	return CMemoryManager::MemoryReAllocate(p,size,file,line);
}
#define ReAlloc(p,s) ReAlloc(p,s,__FILE__, __LINE__)
#define new new(__FILE__, __LINE__)


#define SAFE_DELETE(p)       { if(p) { delete   (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p); (p) = NULL; } }
#define SAFE_RELEASE(p)		 { if(p) { p->Release(); (p) = NULL; } }


