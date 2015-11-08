#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

#include <Gl/glm/glm.hpp>

using namespace std;
using namespace glm;

#define DEFGLSL(x) "#version 330 core\n#pragma debug(on)\n"#x

typedef const char* GLSL_VERT;
typedef const char* GLSL_FRAG;

#define SHADER_FILES   0
#define SHADER_STRINGS 1

#define DECLARE_SETUNIFORM_V(type, func)		\
	void SetUniform(string &name, type &v) {	\
		GLint loc = GetUniformLocation(name);	\
		func(loc, 1, &v[0]);					\
	}

#define DECLARE_SETUNIFORM_M(type, func)		\
	void SetUniform(string &name, type &v) {	\
		GLint loc = GetUniformLocation(name);	\
		func(loc, 1, GL_FALSE, &v[0][0]);		\
	}

struct GLShader {
	GLenum type;
	const char* context;
	GLuint shader;
};


class GLProg {
public:
	GLProg() : m_bCreated(false) {}
	
	operator int() {
		return m_iProg;
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
	bool m_bCreated;
	GLuint m_iProg;

	map<string, GLint> m_uniformMap;

	GLint GetUniformLocation(string &name) {
		map<string, GLint>::iterator it = m_uniformMap.find(name);
		if (it != m_uniformMap.end()) {
			return it->second;
		} else {
			GLint index = glGetUniformLocation(m_iProg, name.c_str());
			m_uniformMap[name] = index;
			return index;
		}
	}

	// load from files, end with NULL
	bool LoadShaders(GLShader *shaders, int type) {
		// create progrom if not created
		if (m_bCreated == false) {
			m_bCreated = true;
			m_iProg = glCreateProgram();
		} else {
			return false;
		}

		// load shaders
		GLShader *entry = shaders;
		for (; entry->type != GL_NONE && entry->context != nullptr; entry++) {
			entry->shader = glCreateShader(entry->type);
			// read source code from file
			if (type == SHADER_FILES) {
				ifstream inf(entry->context);
				if (!inf.good()) {
					cerr << "open file ERROR(" << entry->context << ")" << endl;
					return false;
				}
				istreambuf_iterator<char> beg(inf), end;
				string source(beg, end);

				const char *sourceCstr = source.c_str();
				glShaderSource(entry->shader, 1, &sourceCstr, NULL);
			// read souce from string
			} else if (type == SHADER_STRINGS){
				glShaderSource(entry->shader, 1, &entry->context, NULL); 
			}
			// compile shader
			glCompileShader(entry->shader);

			// check compile result, output error info when compile failed
			GLint compileRet;
			glGetShaderiv(entry->shader, GL_COMPILE_STATUS, &compileRet);
			if (!compileRet) {
				GLsizei len = 0;
				glGetShaderiv(entry->shader, GL_INFO_LOG_LENGTH, &len);
				GLchar *log = new GLchar[len + 1];
				glGetShaderInfoLog(entry->shader, len, &len, log);
				cerr << "Compile Shader ERROR(" << entry->context << "): " << log << endl;
				delete[] log;
				return false;
			}

			// attach to program
			glAttachShader(m_iProg, entry->shader);
		}
		return true;
	}

};