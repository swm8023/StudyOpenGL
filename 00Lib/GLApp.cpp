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

void GLApp::SetContext(int mode, int major, int minor) {
	m_iContextMode = mode;
	m_iMajor = major;
	m_iMinor = minor;
}

void GLApp::Go(int argc, char **argv) {
	if (m_pGLWindow == nullptr) {
		cerr << "Window not set!!" << endl;
		exit(-1);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(m_pGLWindow->GetWidth(), m_pGLWindow->GetHeight());
	if (m_iMajor != -1) {
		glutInitContextVersion(m_iMajor, m_iMinor);
		glutInitContextProfile(m_iContextMode);
	}
	glutCreateWindow(m_pGLWindow->GetTitle().c_str());
	
	glewExperimental = GL_TRUE; // for support some functions
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK) {
		cerr << " GlewInit Error: " << glewGetErrorString(glewErr) << endl;
		exit(-1);
	}

	cout << "GL Renderer:  " << glGetString(GL_RENDERER) << endl;
	cout << "GL Version:   " << glGetString(GL_VERSION) << endl;
	cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

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
