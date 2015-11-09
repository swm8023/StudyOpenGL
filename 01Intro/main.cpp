#include "GLApp.h"
#include <cmath>
#include <GL/glm/glm.hpp>

using namespace glm;

#pragma comment (lib, "SOIL.lib")

#ifdef NDEBUG
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "vimergl.lib")
#else
#pragma comment (lib, "glew32d.lib")
#pragma comment (lib, "vimergld.lib")
#endif




GLSL_VERT glslVert = R"(
#version 330 core
#pragma debug(on)

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 oColor;
out vec2 oCoord;

void main() {
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	oCoord = vCoord;
}

)";

GLSL_FRAG glslFrag = R"(
#version 330 core
#pragma debug(on)

in vec2 oCoord;

	
out vec4 fColor;
uniform sampler2D oTex1;
uniform sampler2D oTex2;

void main() {
	fColor = mix(texture(oTex1, vec2(oCoord.x, 1 - oCoord.y)), 
		texture(oTex2, vec2(oCoord.x, 1 - oCoord.y)), 0.2);
}

)";

#define RENDER_PROG 1

class DTriangleObject : public GLSimpleElement {
public:
	virtual void Initialize() {
		GLfloatArray vert, coord;
		vert.Init(36, 3, {
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		});
		


		coord.Init(36, 2, {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,

			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f,

			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f
		});



		LoadVertexArray(vert, 0);
		LoadVertexArray(coord, 1);

		LoadTexture("../Debug/2.jpg", 0, GLProg::GetFromID(RENDER_PROG)->GetUniformLocation("oTex1"));
		LoadTexture("../Debug/1.jpg", 1, GLProg::GetFromID(RENDER_PROG)->GetUniformLocation("oTex2"));

		InitGLResources();

		
	}

	virtual void Update() {
		GLWindow *w = GetWindow();
		GLProg *prog = GLProg::GetFromID(RENDER_PROG);

		mat4 model, view, projection;
		
		// same projection matrix
		projection = perspective(radians(45.0f), w->GetWidth() * 1.0f / w->GetHeight(), 0.1f, 100.0f);

		prog->SetUniform("projection", projection);

		// view matrix on a circle, changed width time
		float radius = 10;
		float tm = GetTickCount() / 30.0f;
		float camX = sin(radians(tm)) * radius;
		float camZ = cos(radians(tm)) * radius;

		//lookAt(eyePosition, lookPosition, upAxis)
		view = lookAt(vec3(camX, 0.0f, camZ), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		prog->SetUniform("view", view);

		// more element, use different model matrix
		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		for (int i = 0; i < 10; i++) {
			model = mat4(1.0f);
			model = translate(model, cubePositions[i]);
			model = rotate(model, radians(GetTickCount() / 30.0f * (i + 1)), vec3(0.5f, 1.0f, 0.0f));
			prog->SetUniform("model", model);

			DrawArrays(GL_TRIANGLES);
		}
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


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