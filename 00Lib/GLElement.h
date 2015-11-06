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


	// Load Methods
	bool LoadResources(GLfloatArray *verts, GLfloatArray *colors, GLushortArray *indexes);

	bool LoadCoords(GLfloatArray &coords, string path);

	// Draw Methods
	void DrawArrays(GLenum type, int first, int nums);
	void DrawArrays(GLenum type);

	void DrawElements();

	// Getter and Setter
	GLWindow* GetWindow() const;

private:
	GLuint m_vao, m_vbo, m_cbo, m_ebo;
	GLint m_vaoNum, m_vboNum, m_cboNum, m_eboNum;
};