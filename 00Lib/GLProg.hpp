#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;


struct ShaderFile {
	GLenum type;
	const char* fname;
	GLuint shader;
};

class GLProg {
public:
	GLProg() : m_bCreated(false) {}
	
	operator int() {
		return m_iProg;
	}

	// load from files, end with NULL
	bool LoadFromFiles(ShaderFile *shaders) {
		// create progrom if not created
		if (m_bCreated == false) {
			m_bCreated = true;
			m_iProg = glCreateProgram();
		}

		// load shaders
		ShaderFile *entry = shaders;
		for (; entry->type != GL_NONE && entry->fname != nullptr; entry++) {
			entry->shader = glCreateShader(entry->type);
			// read source code from file
			ifstream inf(entry->fname);
			if (!inf.good()) {
				cerr << "open file ERROR(" << entry->fname << ")" << endl;
				return false;
			}
			istreambuf_iterator<char> beg(inf), end;
			string source(beg, end);

			const char *sourceCstr = source.c_str();
			glShaderSource(entry->shader, 1, &sourceCstr, NULL);
			
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
				cerr << "Compile Shader ERROR(" << entry->fname << "): " << log << endl;
				delete[] log;
				return false;
			}

			// attach to program
			glAttachShader(m_iProg, entry->shader);
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

	bool Use() {
		glUseProgram(m_iProg);
	}

private:
	bool m_bCreated;
	GLuint m_iProg;
};