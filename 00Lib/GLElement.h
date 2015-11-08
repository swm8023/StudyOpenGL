#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <iostream>
#include <vector>

#include "GLArray.h"

using namespace std;

class GLWindow;

class GLElement {
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;

	GLElement() {
		m_vao = m_vbo = m_cbo = m_ebo = 0;
		m_vaoNum = m_vboNum = m_cboNum = m_eboNum = 0;
		m_vboLayoutIndex = m_cboLayoutIndex = -1;
	}

	// Load Methods
	void LoadVertexes(GLfloatArray *verts, int layoutIndex = 0);
	void LoadColors(GLfloatArray *colors, int layoutIndex = 1);
	void LoadIndexes(GLushortArray *indexes);
	void LoadCoords(GLfloatArray &coords, string path);
 
	// Draw Methods
	void DrawArrays(GLenum type, int first, int nums);
	void DrawArrays(GLenum type);

	void DrawElements(GLenum type);

	// Getter and Setter
	GLWindow* GetWindow() const;

private:
	GLuint m_vao, m_vbo, m_cbo, m_ebo;
	GLint m_vaoNum, m_vboNum, m_cboNum, m_eboNum;
	int m_vboLayoutIndex, m_cboLayoutIndex;
};