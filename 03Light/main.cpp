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

// shaders for light point
GLSL_VERT glslLightVert = R"(
#version 330 core
layout(location = 0) in vec3 vPosition;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
}

)";

GLSL_FRAG glslLightFrag = R"(
#version 330 core
	
out vec4 fColor;

void main() {
	fColor = vec4(1.0f);
}

)";

// shader for cube
GLSL_VERT glslCubeVert = R"(
#version 330 core
#pragma debug(on)

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 normal;
out vec3 fragPos;

void main() {
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	normal = mat3(transpose(inverse(model))) * normal;
	fragPos = vec3(model * vec4(vPosition, 1.0f));
}

)";

GLSL_FRAG glslCubeFrag = R"(
#version 330 core
#pragma debug(on)

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 normal;
in vec3 fragPos;
out vec4 fColor;

void main() {
	// ambient lighting
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

		// diffuse lighting
	float v = dot(normalize(normal), normalize(lightPos - fragPos));
	float diff = max(v, 0.0);
	vec3 diffuse = diff * lightColor;
	
	// specular lighting

	vec3 result = (ambient + diffuse) * objectColor;
    fColor = vec4(result, 1.0f);
}

)";

GLfloatArray cubePosition(36, 3, {
	-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,-1.0f,  1.0f, -1.0f,-1.0f, -1.0f, -1.0f,-1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
});


GLfloatArray cubeNormal(36, 3, {
	 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	 0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	-1.0f,  0.0f,  0.0f,-1.0f,  0.0f,  0.0f,-1.0f,  0.0f,  0.0f,-1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
	 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f
});

vec3 lightPos = vec3(1.2f, 1.0f, 0.0f);

#define RENDER_CUBE_PROG   1
#define RENDER_LIGHT_PROG  2



class LightElement : public GLSimpleElement {
public:

	virtual void Initialize() {
		GLfloatArray vert = cubePosition;
		GLProg *prog = GLProg::GetFromID(RENDER_LIGHT_PROG);
		prog->Use();
		LoadVertexArray(vert, 0);
		InitGLResources();
	}

	virtual void Update() {
		GLWindow *w = GetWindow();
		mat4 projection, model;

		model = translate(model, lightPos);
		model = scale(model, vec3(0.1f));
		projection = perspective(radians(w->GetCamera()->GetAspect()), w->GetWidth() * 1.0f / w->GetHeight(), 0.1f, 100.0f);

		GLProg *prog = GLProg::GetFromID(RENDER_LIGHT_PROG);
		prog->Use();
		prog->SetUniform("projection", projection);
		prog->SetUniform("view", w->GetCamera()->GetViewMatrix());
		prog->SetUniform("model", model);

		DrawArrays(GL_TRIANGLES);
	}

private:

};

class CubeElement : public GLSimpleElement {
public:

	virtual void Initialize() {
		GLfloatArray vert = cubePosition, normal = cubeNormal;
		GLProg *prog = GLProg::GetFromID(RENDER_CUBE_PROG);
		prog->Use();
		LoadVertexArray(vert, 0);
		LoadVertexArray(normal, 1);

		InitGLResources();
	}

	virtual void Update() {
		GLWindow *w = GetWindow();
		mat4 projection, view, model;

		projection = perspective(radians(w->GetCamera()->GetAspect()), w->GetWidth() * 1.0f / w->GetHeight(), 0.1f, 100.0f);
		model = scale(model, vec3(0.5f));
		GLProg *prog = GLProg::GetFromID(RENDER_CUBE_PROG);
		prog->Use();
		prog->SetUniform("projection", projection);
		prog->SetUniform("view", w->GetCamera()->GetViewMatrix());
		prog->SetUniform("model", model);

		prog->SetUniform("objectColor", vec3(1.0f, 0.5f, 0.31f));
		prog->SetUniform("lightColor", vec3(1.0f, 1.0f, 1.0f));
		prog->SetUniform("lightPos", lightPos);

		DrawArrays(GL_TRIANGLES);
	}

private:



};


class MyWindow : public GLWindow {
public:
	MyWindow(int iWidth, int iHeight, string strTitle)
		: GLWindow(iWidth, iHeight, strTitle) {
	}

	void Initialize() {
		GLShader shadersLight[] = {
			{ GL_VERTEX_SHADER, glslLightVert },
			{ GL_FRAGMENT_SHADER, glslLightFrag },
			{ GL_NONE, nullptr },
		};

		GLShader shadersCube[] = {
			{ GL_VERTEX_SHADER, glslCubeVert },
			{ GL_FRAGMENT_SHADER, glslCubeFrag },
			{ GL_NONE, nullptr },
		};

		GLProg *lightProg = GLProg::Create(RENDER_LIGHT_PROG);
		lightProg->LoadShaderStrings(shadersLight);
		lightProg->Link();

		GLProg *cubeProg = GLProg::Create(RENDER_CUBE_PROG);
		cubeProg->LoadShaderStrings(shadersCube);
		cubeProg->Link();

		// lightElt = new LightElement();
		cubeElt = new CubeElement();

		// lightElt->Initialize();
		cubeElt->Initialize();

		// addElement(lightElt);
		addElement(cubeElt);

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

	LightElement *lightElt;
	CubeElement *cubeElt;

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