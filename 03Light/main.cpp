#include "GLApp.h"
#include <cmath>
#include <GL/glm/glm.hpp>
#include <GL/assimp/Importer.hpp>
#include <GL/assimp/scene.h>
#include <GL/assimp/postprocess.h>

using namespace glm;

#pragma comment (lib, "SOIL.lib")

#ifdef NDEBUG
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "vimergl.lib")
#pragma comment (lib, "assimp.lib")
#else
#pragma comment (lib, "glew32d.lib")
#pragma comment (lib, "vimergld.lib")
#pragma comment (lib, "assimpd.lib")
#endif

// shaders for light point
GLSL_VERT glslVert = DEFGLSL330(
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexcoords;

out vec2 texCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	texCoords = vTexcoords;
}
);

GLSL_FRAG glslFrag = DEFGLSL330(
in vec2 texCoords;
out vec4 fColor;

uniform sampler2D texDiffuse1;

void main() {
	fColor = vec4(texture(texDiffuse1, texCoords));
}
);

#define GLSL_PROG 1
class MyWindow : public GLWindow {
public:
	MyWindow(int iWidth, int iHeight, string strTitle)
		: GLWindow(iWidth, iHeight, strTitle) {
	}

	void Initialize() {
		GLShader shaders[] = {
			{ GL_VERTEX_SHADER, glslVert },
			{ GL_FRAGMENT_SHADER, glslFrag },
			{ GL_NONE, nullptr },
		};

		GLProg *prog = GLProg::Create(GLSL_PROG);
		prog->LoadShaderStrings(shaders);
		prog->Link();
		prog->Use();

		auto glElm = new GLAssipElement("../nano/nanosuit.obj", GLSL_PROG);
		glElm->Initialize();

		addElement(glElm);

		this->GetCamera()->Initialize(vec3(0.0f, 1.0f, 3.0f), vec3(0.0f, 1.0f, 0.0f));
	}

	void BeforeUpdate() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// set camera here
		GLCamera *camera = this->GetCamera();
		GLfloat speed = 0.05f, deltaTm = 1.0f;

		if (keyst['w']) {
			camera->HandleEvent(CAMERA_GO_UP, deltaTm * speed);
		}
		if (keyst['s']) {
			camera->HandleEvent(CAMERA_GO_DOWN, deltaTm * speed);
		}
		if (keyst['a']) {
			camera->HandleEvent(CAMERA_GO_LEFT, deltaTm * speed);
		}
		if (keyst['d']) {
			camera->HandleEvent(CAMERA_GO_RIGHT, deltaTm * speed);
		}
		if (keyst[GLUT_LEFT_BUTTON]) {
			if (preX == -1) {
				preX = nowX, preY = nowY;
			} else {
				GLfloat yawDt = (nowX - preX) * speed * deltaTm;
				GLfloat pitchDt = (preY - nowY) * speed * deltaTm;
				camera->HandleEvent(CAMERA_DIRECT_MOVE, yawDt, pitchDt);
				preX = nowX, preY = nowY;
			}
		} else {
			preX = preY = -1;
		}
		GLfloat wheelSpeed = 3.0f;
		if (wheelDir < 0) {
			camera->HandleEvent(CAMERA_ZOOM_LESS, -wheelDir * wheelSpeed);
			wheelDir = 0;
		}
		if (wheelDir > 0) {
			camera->HandleEvent(CAMERA_ZOOM_MORE, wheelDir * wheelSpeed);
		}

		// update uniform matrix
		glm::mat4 projection = perspective(camera->GetAspect(), GetWidth() * 1.0f / GetHeight(), 0.1f, 100.0f);
		glm::mat4 view = camera->GetViewMatrix();
		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.50f, 0.0f));
		model = glm::scale(model, vec3(0.2f, 0.2f, 0.2f));

		GLProg::GetFromID(GLSL_PROG)->SetUniform("projection", projection);
		GLProg::GetFromID(GLSL_PROG)->SetUniform("view", view);
		GLProg::GetFromID(GLSL_PROG)->SetUniform("model", model);
	}



	void OnMouse(int key, int state, int x, int y) {
		// dtobj->SetKeyState(key, state, x, y);
		keyst[key] = !state;
		nowX = x, nowY = y;
	}

	void OnKey(unsigned char key, int state, int x, int y) {
		// dtobj->SetKeyState(key, state, x, y);
		keyst[key] = !state;
	}

	void OnMotion(int x, int y) {
		// dtobj->SetKeyState(-1, 0, x, y);
		nowX = x, nowY = y;
	}

	void OnWheel(int key, int dir, int x, int y) {
		// dtobj->SetKeyState(key, dir, x, y);
		wheelDir = dir;
	}

private:


	bool keyst[300] = {0};
	GLfloat preX = -1, preY, nowX, nowY;
	int wheelDir = 0;
};

int main(int argc, char *argv[]) {
	MyWindow *w = new MyWindow(800, 600, "Light");
	GLApp::GetInstance()->SetWindow(w);
	GLApp::GetInstance()->SetContext(GLUT_CORE_PROFILE, 3, 3);
	GLApp::GetInstance()->SetFrameInSecond(60);
	GLApp::GetInstance()->Go(argc, argv);

}