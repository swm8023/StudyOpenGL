#include "GLApp.h"

#ifdef NDEBUG
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "vimergl.lib")
#else
#pragma comment (lib, "glew32d.lib")
#pragma comment (lib, "vimergld.lib")
#endif

#ifdef NDEBUG
#else)
#endif

class DTriangleObject : public GLElement {
public:
	virtual void Initialize() {
		GLfloatArray vert;
		vert.Init(6, 2, {
			-0.90f, -0.90f,
			 0.85f, -0.90f,
			-0.90f,  0.85f,
			 0.90f, -0.85f,
			 0.90f,  0.90f,
			-0.85f,  0.90f });
		LoadResources(&vert, nullptr, nullptr);
	}

	virtual void Update() {
		DrawArrays(GL_TRIANGLES);
	}
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

		DTriangleObject *dto = new DTriangleObject();
		dto->Initialize();
		this->addElement(dto);

	}

	void BeforeUpdate() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void AfterUpdate() {
		glFlush();
	}

	void Reshape(int iWidth, int iHeight) {
		cout << "reshape" << endl;
	}

private:
	GLProg prog;

	DTriangleObject *dtobj;

};

int main(int argc, char *argv[]) {
	MyWindow *w = new MyWindow(512, 512, "Hello");
	GLApp::GetInstance()->SetWindow(w);
	GLApp::GetInstance()->SetContext(GLUT_CORE_PROFILE, 3, 3);
	GLApp::GetInstance()->Go(argc, argv);

}