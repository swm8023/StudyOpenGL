#include "GLApp.h"

#include <GL/glm/glm.hpp>

#ifdef NDEBUG
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "vimergl.lib")
#else
#pragma comment (lib, "glew32d.lib")
#pragma comment (lib, "vimergld.lib")
#endif


GLSL_VERT glslVert = DEFGLSL(
	layout(location = 0) in vec4 vPosition;
	layout(location = 1) in vec4 vColor;

	out vec4 fs_Color;
	void main() {
		gl_Position = vPosition;
		fs_Color = vColor;
	}
);

GLSL_FRAG glslFrag = DEFGLSL(
	in vec4 fs_Color;
	out vec4 fColor;

	void main() {
		fColor = fs_Color;
	}
);

class DTriangleObject : public GLElement {
public:
	virtual void Initialize() {
		GLfloatArray vert, color;
		GLushortArray index;
		vert.Init(6, 2, {
			-0.90f, -0.90f,
			 0.85f, -0.90f,
			-0.90f,  0.85f,
			 0.90f, -0.85f,
			 0.90f,  0.90f,
			-0.85f,  0.90f });
		
		color.Init(6, 3, {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
		});
		
		index.Init(3, 1, {
			0, 1, 2
		});

		LoadVertexes(&vert);
		LoadColors(&color);
		LoadIndexes(&index);
	}

	virtual void Update() {
		DrawArrays(GL_TRIANGLES);
		// DrawElements(GL_TRIANGLES);
	}
};


class MyWindow : public GLWindow {
public:
	MyWindow(int iWidth, int iHeight, string strTitle) 
		: GLWindow(iWidth, iHeight, strTitle) {
	}

	void Initialize() {
		GLShader shaders[] = {
			{GL_VERTEX_SHADER, glslVert},
			{GL_FRAGMENT_SHADER, glslFrag},
			{GL_NONE, nullptr},
		};
		prog.LoadShaderStrings(shaders);
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