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
		texture(oTex2, vec2(oCoord.x, 1 - oCoord.y)), 0.4);
}

)";

#define RENDER_PROG 1

class DTriangleObject : public GLSimpleElement {
public:
	void SetKeyState(int key, int state, int x, int y) {
		// it's a move action
		if (key == -1 || key == GLUT_LEFT_BUTTON) {
			nowX = x, nowY = y;
		}
		if (key >= 0 && key < 300) {
			this->keyst[key] = !state;
		}
		if (key == 0) {
			wheelDir = state;
		}
	}

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

		camPos = vec3(0.0f, 0.0f, -3.0f);
		camFront = normalize(vec3(0.0f, 0.0f, 1.0f));
		camUp = normalize(vec3(0.0f, 1.0f, 0.0f));

	}

	virtual void Update() {
		GLWindow *w = GetWindow();
		GLProg *prog = GLProg::GetFromID(RENDER_PROG);

		// change some parameters with user input
		DoActions();

		mat4 model, view, projection;
		
		// same projection matrix
		projection = perspective(radians(aspect), w->GetWidth() * 1.0f / w->GetHeight(), 0.1f, 100.0f);
		prog->SetUniform("projection", projection);

		// view matrix

		//lookAt(eyePosition, lookPosition, upAxis)
		view = lookAt(camPos, camPos + camFront, camUp);
		prog->SetUniform("view", view);

		// more element, use different model matrix
		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -3.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -5.3f),
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

private:
	int keyst[300];

	vec3 camPos, camFront, camUp;
	GLfloat preX = -1, preY = -1, nowX, nowY;
	GLfloat pitch = 0.0f, yaw = 90.0f;

	int wheelDir = 0;
	GLfloat aspect = 40.0f;

	void DoActions() {
		// four direction
		GLfloat speed = 0.05f;
		if (keyst['w']) {
			camPos += camFront * speed;
		}
		if (keyst['s']) {
			camPos -= camFront * speed;
		}
		if (keyst['a']) {
			camPos -= normalize(cross(camFront, camUp)) * speed;
		}
		if (keyst['d']) {
			camPos += normalize(cross(camFront, camUp)) * speed;
		}
		// focus camera position, change direction user look at 
		if (keyst[GLUT_LEFT_BUTTON]) {
			// first press, don't care, move next frame
			if (preX == -1) {
				preX = nowX, preY = nowY;
			} else {
				GLfloat detX = nowX - preX, detY = preY - nowY;
				preX = nowX, preY = nowY;
				detX *= speed, detY *= speed;
				yaw += detX, pitch += detY;

				camFront = vec3(cos(radians(yaw)) * cos(radians(pitch)),
					sin(radians(pitch)),
					sin(radians(yaw)) * cos(radians(pitch)));

				camFront = normalize(camFront);
			}
		// reset pre position
		} else {
			preX = preY = -1;
		}

		// mouse wheel, won't auto up
		if (wheelDir != 0) {
			aspect += wheelDir * 3;
			if (aspect > 89) aspect = 89;
			if (aspect < 1) aspect = 1;

			wheelDir = 0;
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


		dtobj = new DTriangleObject();
		dtobj->Initialize();
		this->addElement(dtobj);

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

	void OnMouse(int key, int state, int x, int y) {
		dtobj->SetKeyState(key, state, x, y);
	}

	void OnKey(unsigned char key, int state, int x, int y) {
		dtobj->SetKeyState(key, state, x, y);
	}

	void OnMotion(int x, int y) {
		dtobj->SetKeyState(-1, 0, x, y);
	}

	void OnWheel(int key, int dir, int x, int y) {
		dtobj->SetKeyState(key, dir, x, y);
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