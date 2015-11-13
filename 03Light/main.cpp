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

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;


void main() {
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
}

)";

GLSL_FRAG glslCubeFrag = R"(
#version 330 core
#pragma debug(on)

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 fColor;

void main() {
	fColor = vec4(lightColor * objectColor, 1.0f);
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

vec3 lightPos = vec3(1.2f, 1.0f, -2.0f);

#define RENDER_CUBE_PROG 1
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
		mat4 projection, view, model;

		view = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));
		model = translate(model, lightPos);
		model = scale(model, vec3(0.1f));
		projection = perspective(radians(45.0f), w->GetWidth() * 1.0f / w->GetHeight(), 0.1f, 100.0f);

		GLProg *prog = GLProg::GetFromID(RENDER_LIGHT_PROG);
		prog->Use();
		prog->SetUniform("projection", projection);
		prog->SetUniform("view", view);
		prog->SetUniform("model", model);

		DrawArrays(GL_TRIANGLES);
	}

private:

};

class CubeElement : public GLSimpleElement {
public:

	virtual void Initialize() {
		GLfloatArray vert = cubePosition;
		GLProg *prog = GLProg::GetFromID(RENDER_CUBE_PROG);
		prog->Use();
		LoadVertexArray(vert, 0);
		InitGLResources();
	}

	virtual void Update() {
		GLWindow *w = GetWindow();
		mat4 projection, view, model;

		view = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(1.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));
		projection = perspective(radians(45.0f), w->GetWidth() * 1.0f / w->GetHeight(), 0.1f, 100.0f);
		model = scale(model, vec3(0.3f));
		GLProg *prog = GLProg::GetFromID(RENDER_CUBE_PROG);
		prog->Use();
		prog->SetUniform("projection", projection);
		prog->SetUniform("view", view);
		prog->SetUniform("model", model);

		prog->SetUniform("objectColor", vec3(1.0f, 0.5f, 0.31f));
		prog->SetUniform("lightColor", vec3(1.0f, 1.0f, 1.0f));

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

		lightElt = new LightElement();
		cubeElt = new CubeElement();

		lightElt->Initialize();
		cubeElt->Initialize();

		addElement(lightElt);
		addElement(cubeElt);
	}

private:

	LightElement *lightElt;
	CubeElement *cubeElt;
};

int main(int argc, char *argv[]) {
	MyWindow *w = new MyWindow(800, 600, "Light");
	GLApp::GetInstance()->SetWindow(w);
	GLApp::GetInstance()->SetContext(GLUT_CORE_PROFILE, 3, 3);
	GLApp::GetInstance()->SetFrameInSecond(60);
	GLApp::GetInstance()->Go(argc, argv);

}