#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <iostream>
using namespace std;


class GLWindow {
public:
	GLWindow(int iWidth, int iHeight, string strTitle) {
		this->m_iWidth = iWidth;
		this->m_iHeight = iHeight;
		this->m_strTitle = strTitle;
	}
	
	// interface
	virtual void Initialize() = 0;
	virtual void Display() = 0;
	virtual void Reshape(int, int) = 0;

	// setter and getter	
	int GetWidth() { return m_iWidth; }
	int GetHeight() { return m_iHeight; }
	string GetTitle() { return m_strTitle; }

	void SetWidth(int iWidth) { this->m_iWidth = iWidth; }
	void SetHeight(int iHeight) { this->m_iHeight = iHeight; }
	void SetTitle(string strTitle) { this->m_strTitle = strTitle; }


private:
	int m_iWidth;
	int m_iHeight;
	string m_strTitle;
	
	GL
};


class GLApp {
public:
	static GLApp* GetInstance() {
		static GLApp glApp;
		return &glApp;
	}

	void SetWindow(GLWindow* pGLWindow) {
		m_pGLWindow = pGLWindow;
	}

	GLWindow* GetWindow() {
		return m_pGLWindow;
	}

	// just call method 'go' to run GLApp
	// before call this method, must set Window
	void Go(int argc, char **argv) {
		if (m_pGLWindow == nullptr) {
			cerr << "Window not set!!" << endl;
			exit(-1);
		}

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA);
		glutInitWindowSize(m_pGLWindow->GetWidth(), m_pGLWindow->GetHeight());
		glutCreateWindow(m_pGLWindow->GetTitle().c_str());

		if (glewInit()) {
			cerr << " Unable to initialize glew!!" << endl;
			exit(-1);
		}

		GLApp::CallInitialize();
		
		glutDisplayFunc(GLApp::CallDisplay);
		glutReshapeFunc(GLApp::CallReshape);
		glutMainLoop();
	}

private:
	// private variable
	GLWindow* m_pGLWindow;
	
	// private method
	GLApp() { m_pGLWindow = nullptr; }

	static void CallInitialize() {
		GLApp *gl = GLApp::GetInstance();
		if (gl->m_pGLWindow)
			gl->m_pGLWindow->Initialize();
	}

	static void CallDisplay() {
		GLApp *gl = GLApp::GetInstance();
		if (gl->m_pGLWindow)
			gl->m_pGLWindow->Display();
	}

	static void CallReshape(int iWidth, int iHeight) {
		GLApp *gl = GLApp::GetInstance();
		if (gl->m_pGLWindow)
			gl->m_pGLWindow->Reshape(iWidth, iHeight);
	}
};