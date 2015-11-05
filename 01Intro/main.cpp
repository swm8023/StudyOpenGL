#include <GL/glew.h>
#include <GL/glut.h>
#include "GLApp.hpp"
#include "GLProg.hpp"
#include "GLObject.hpp"

class DTriangleObject : public GLObject {

};


class MyWindow : public GLWindow {
public:
	MyWindow(int iWidth, int iHeight, string strTitle) 
		: GLWindow(iWidth, iHeight, strTitle) {
	}

	void Initialize() {
		ShaderFile shaders[] = {
			{GL_VERTEX_SHADER, "../01Intro/triangles.vert"},
			{GL_FRAGMENT_SHADER, "../01Intro/triangles.frag"},
			{GL_NONE, nullptr},
		};
		prog.LoadFromFiles(shaders);
		prog.Link();
		prog.Use();

		
		GLfloatArray varr;
		varr.

		


	}

	void Display() {
		glClear(GL_COLOR_BUFFER_BIT);
		
		glFlush();
		
	}

	void Reshape(int iWidth, int iHeight) {
		cout << "reshape" << endl;
	}

private:
	GLProg prog;

	DTriangleObject dtobj;

};

int main(int argc, char *argv[]) {
	GLMyWindow *w = new GLMyWindow(512, 512, "Hello");
	GLApp::GetInstance()->SetWindow(w);
	GLApp::GetInstance()->Go(argc, argv);

	


}