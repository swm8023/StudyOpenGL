#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <string>
#include <set>
#include <iostream>

#include "GLProg.h"
#include "GLArray.h"
#include "GLWindow.h"
#include "GLElement.h"
#include "GLCamera.h"

using namespace std;

class GLApp {
public:
	static GLApp* GetInstance();

	void SetWindow(GLWindow* pGLWindow);
	GLWindow* GetWindow();

	// just call method 'go' to run GLApp
	// before call this method, must set Window
	void Go(int argc, char **argv);

	void SetContext(int mode, int major, int minor);
	void SetFrameInSecond(int frame);	// frame per second
private:
	// private variable
	GLWindow* m_pGLWindow;

	// private method
	// singleton
	GLApp() : m_pGLWindow(nullptr), m_iMajor(-1), m_iMinor(-1),
		m_iFrameInSecond(-1){
	}

	// callback
	static void CallInitialize();
	static void CallDisplay();
	static void CallReshape(int iWidth, int iHeight);
	static void CallTimerPostRedisplay(int);
	static void CallKeyBoardPress(unsigned char key, int y, int z);
	static void CallKeyBoardUp(unsigned char key, int y, int z);
	static void CallMouse(int key, int state, int x, int y);
	static void CallMotion(int x, int y);
	static void CallWheel(int button, int dir, int x, int y);
	// context mode
	int m_iContextMode;
	int m_iMajor, m_iMinor;

	// frame per second
	int m_iFrameInSecond;
};