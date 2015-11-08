#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

#include <Gl/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

#define DEFGLSL(x) "#version 330 core\n#pragma debug(on)\n"#x

typedef const char* GLSL_VERT;
typedef const char* GLSL_FRAG;

#define SHADER_FILES   0
#define SHADER_STRINGS 1

#define DECLARE_SETUNIFORM_V(type, func)		\
	void SetUniform(string name, type &v) {	\
		GLint loc = GetUniformLocation(name);	\
		func(loc, 1, value_ptr(v));				\
	}

#define DECLARE_SETUNIFORM_M(type, func)		\
	void SetUniform(string name, type &v) {	\
		GLint loc = GetUniformLocation(name);	\
		func(loc, 1, GL_FALSE, value_ptr(v));	\
	}

struct GLShader {
	GLenum type;
	const char* context;
	GLuint shader;
};


class GLProg {
public:
	static GLProg* Create(int id = -1);

	static GLProg* GetFromID(int id) {
		return m_progMap[id];
	}
	
	void SetID(int id) {
		this->m_id = id;
	}

	const int GetID() const {
		return m_id;
	}

	bool LoadShaderStrings(GLShader *shaders) {
		return LoadShaders(shaders, SHADER_STRINGS);
	}

	bool LoadShaderFiles(GLShader *shaders) {
		return LoadShaders(shaders, SHADER_FILES);
	}

	// Set uniform variable
	DECLARE_SETUNIFORM_V(vec2, glUniform2fv);
	DECLARE_SETUNIFORM_V(vec3, glUniform3fv);
	DECLARE_SETUNIFORM_V(vec4, glUniform4fv);

	DECLARE_SETUNIFORM_M(mat2, glUniformMatrix2fv);
	DECLARE_SETUNIFORM_M(mat3, glUniformMatrix3fv);
	DECLARE_SETUNIFORM_M(mat4, glUniformMatrix4fv);

	GLint GetUniformLocation(string &name) {
		map<string, GLint>::iterator it = m_uniformMap.find(name);
		if (it != m_uniformMap.end()) {
			return it->second;
		}
		else {
			GLint index = glGetUniformLocation(m_iProg, name.c_str());
			m_uniformMap[name] = index;
			return index;
		}
	}

	bool Link() {
		if (m_bCreated == false) {
			return false;
		}

		glLinkProgram(m_iProg);

		// check link result, output error info when link failed
		GLint linkRet;
		glGetProgramiv(m_iProg, GL_LINK_STATUS, &linkRet);
		if (!linkRet) {
			GLsizei len = 0;
			glGetProgramiv(m_iProg, GL_INFO_LOG_LENGTH, &len);
			GLchar *log = new GLchar[len + 1];
			glGetProgramInfoLog(m_iProg, len, &len, log);
			cerr << "Link Program ERROR: " << log << endl;
			delete[] log;
			return false;
		}
		return true;
	}

	void Use() {
		glUseProgram(m_iProg);
	}

private:
	// for program id
	int m_id;
	static map<int, GLProg*> m_progMap;
	static int m_maxProgID;

	// get prog by create
	GLProg() : m_bCreated(false) {}
	
	bool m_bCreated;
	GLuint m_iProg;
	
	map<string, GLint> m_uniformMap;




	// load from files, end with NULL
	bool LoadShaders(GLShader *shaders, int type);
};