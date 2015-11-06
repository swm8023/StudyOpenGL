#include "GLApp.h"
#include "GLWindow.h"

GLApp* GLApp::GetInstance() {
	static GLApp glApp;
	return &glApp;
}

void GLApp::SetWindow(GLWindow* pGLWindow) {
	m_pGLWindow = pGLWindow;
}

GLWindow* GLApp::GetWindow() {
	return m_pGLWindow;
}


void GLApp::Go(int argc, char **argv) {
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

// private methods

void GLApp::CallInitialize() {
	GLApp *gl = GLApp::GetInstance();
	if (gl->m_pGLWindow)
		gl->m_pGLWindow->Initialize();
}

void GLApp::CallDisplay() {
	GLApp *gl = GLApp::GetInstance();
	if (gl->m_pGLWindow)
		gl->m_pGLWindow->BeforeUpdate();
	gl->m_pGLWindow->Update();
	gl->m_pGLWindow->AfterUpdate();
}

void GLApp::CallReshape(int iWidth, int iHeight) {
	GLApp *gl = GLApp::GetInstance();
	if (gl->m_pGLWindow)
		gl->m_pGLWindow->Reshape(iWidth, iHeight);
}
