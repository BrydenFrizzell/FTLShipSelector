#include "FTLDraw.h"
#include "DirtyHooker.h"
#include <windows.h>
#include <gl\GL.h>
#include <vector>

//vector of functions that take no arguments and return nothing
std::vector<void (*) (void)> hooks;

DWORD glFinishPointer;

void drawTriangle (float x1,float y1,float x2,float y2,float x3,float y3) {
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_TRIANGLES);
	 glVertex3f(x1, y1, 0.0);
	 glVertex3f(x2, y2, 0.0);
	 glVertex3f(x3, y3, 0.0);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
}

void drawRect (float x,float y,float w,float h) {
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_QUADS);
	 glVertex3f(x, y, 0.0); 
	 glVertex3f(x+w, y, 0.0); 
	 glVertex3f(x+w, y+h, 0.0); 
	 glVertex3f(x, y+h, 0.0); 
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
}

//Don't inline or it'll mess up the hook's stack frame!
__declspec(noinline) void callHooks(void) {
	// for some reason, the compiler gets confused.
	// It won't let me make hooks be std::vector<void (void)>, but when using void (*) (void)
	// it derefferences the function pointer before it's called
	void* test = NULL;
	for(int i=0;i<hooks.size();i++) {
		void (*hook) (void) = hooks[i];
		// I've tried several different methods, and the easiest way to get around this is using
		// inline ASM...
		__asm {
			call hook;
		}
	}
}

void openGLFinishHook(void) {
	// EBP, EBX and EDI are pushed by VC++ automatically before this ASM block
	// code here is safe, just don't use any variables, cause they'll prolly screw up the registers
	// if you need variables, create a function and call it here, that will preserve the registers

	callHooks();

	// reset registers and jump to opengl code
	__asm
	{
		pop edi;
		pop ebx;
		pop ebp;
		jmp glFinishPointer;
	}
};

void addDrawHook(void function(void)) {
	hooks.push_back((void (*) (void))function);
}

void hookGLFinish(void) {
	HANDLE FTLProcess = GetCurrentProcess();
	glFinishPointer = (DWORD)GetProcAddress(GetModuleHandle("OPENGL32.dll"), "glFinish");
	RETHook6Byte((0x0025DBB8+0x00400000),openGLFinishHook,FTLProcess);
}