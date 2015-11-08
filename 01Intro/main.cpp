#include "GLApp.h"
#include <cmath>
//#include <GL/glm/glm.hpp>

#pragma comment (lib, "SOIL.lib")

#ifdef NDEBUG
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "vimergl.lib")
#else
#pragma comment (lib, "glew32d.lib")
#pragma comment (lib, "vimergld.lib")
#endif


GLSL_VERT glslVert = DEFGLSL(
	layout(location = 0) in vec4 vPosition;
	layout(location = 1) in vec3 vColor;
	layout(location = 2) in vec2 vCoord;

	out vec3 oColor;
	out vec2 oCoord;

	void main() {
		gl_Position = vPosition;
		oColor = vColor;
		oCoord = vCoord;
	}
);

GLSL_FRAG glslFrag = DEFGLSL(
	in vec3 oColor;
	in vec2 oCoord;
	
	out vec4 fColor;
	uniform sampler2D oTex1;
	uniform sampler2D oTex2;

	void main() {
		// fColor = vec4(oColor, 1.0);
		fColor = mix(texture(oTex1, vec2(oCoord.x, 1 - oCoord.y)) * vec4(oColor, 1.0), 
			texture(oTex2, vec2(oCoord.x, 1 - oCoord.y)), 0.2);
	}
);

#define RENDER_PROG 1

class DTriangleObject : public GLSimpleElement {
public:
	virtual void Initialize() {
		GLfloatArray vert, color, coord;
		vert.Init(4, 3, {
			 0.50f,  0.50f, 0.0f,
			 0.50f, -0.50f, 0.0f,
			-0.50f, -0.50f, 0.0f,
			-0.50f,  0.50f, 0.0f,
		});
		
		color.Init(4, 3, {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
		});

		coord.Init(4, 2, {
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f,
		});

		vector<GLushort> index{0, 1, 3, 1, 2, 3};


		LoadVertexArray(vert, 0);
		LoadVertexArray(color, 1);
		LoadVertexArray(coord, 2);
		LoadIndex(index);
		LoadTexture("../Debug/5326572.jpg", 0, GLProg::GetFromID(RENDER_PROG)->GetUniformLocation("oTex1"));
		LoadTexture("../Debug/test.jpg", 1, GLProg::GetFromID(RENDER_PROG)->GetUniformLocation("oTex2"));

		InitGLResources();

		
	}

	virtual void Update() {
		DrawElements(GL_TRIANGLES);
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

		GLProg *prog = GLProg::Create(RENDER_PROG);
		prog->LoadShaderStrings(shaders);
		prog->Link();
		prog->Use();


		DTriangleObject *dto = new DTriangleObject();
		dto->Initialize();
		this->addElement(dto);



	}

	void BeforeUpdate() {
		glClear(GL_COLOR_BUFFER_BIT);

		vec4 color{ sin(GetTickCount() / 1000.0f) / 2.0f + 0.5f };
		GLProg *prog = GLProg::GetFromID(RENDER_PROG);
	}

	void AfterUpdate() {
		glFlush();
	}

	void Reshape(int iWidth, int iHeight) {
		cout << "reshape" << endl;
	}

private:

	DTriangleObject *dtobj;

};

int main(int argc, char *argv[]) {
	MyWindow *w = new MyWindow(512, 512, "Hello 2");
	GLApp::GetInstance()->SetWindow(w);
	GLApp::GetInstance()->SetContext(GLUT_CORE_PROFILE, 3, 3);
	GLApp::GetInstance()->SetFrameInSecond(60);
	GLApp::GetInstance()->Go(argc, argv);

}