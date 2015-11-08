#include "GLProg.h"

int GLProg::m_maxProgID = 0;
map<int,GLProg*> GLProg::m_progMap = map<int, GLProg*>();


// use this method to get a new program
GLProg* GLProg::Create(int id) {
	// id already exsist
	if (id != -1 && m_progMap.find(id) != m_progMap.end()) {
		return nullptr;
	}

	// new prog and insert into map
	GLProg *prog = new GLProg();
	if (id == -1) {
		while (m_progMap.find(m_maxProgID) != m_progMap.end()) {
			m_maxProgID++;
		}
		m_progMap[m_maxProgID] = prog;
		prog->SetID(m_maxProgID);
		m_maxProgID++;
	}
	else {
		m_progMap[id] = prog;
		prog->SetID(id);
	}

	return prog;
}

// private method


bool GLProg::LoadShaders(GLShader *shaders, int type) {
	// create progrom if not created
	if (m_bCreated == false) {
		m_bCreated = true;
		m_iProg = glCreateProgram();
	}
	else {
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
		}
		else if (type == SHADER_STRINGS) {
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