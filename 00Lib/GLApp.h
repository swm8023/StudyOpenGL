#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <set>
#include <iostream>
using namespace std;

#include "GLProg.h"
#include "GLArray.h"
#include "GLWindow.h"
#include "GLElement.h"

class GLApp {
public:
	static GLApp* GetInstance();

	void SetWindow(GLWindow* pGLWindow);
	GLWindow* GetWindow();

	// just call method 'go' to run GLApp
	// before call this method, must set Window
	void Go(int argc, char **argv);

private:
	// private variable
	GLWindow* m_pGLWindow;

	// private method
	GLApp() { m_pGLWindow = nullptr; }		// singleton

	// callback
	static void CallInitialize();
	static void CallDisplay();
	static void CallReshape(int iWidth, int iHeight);
};