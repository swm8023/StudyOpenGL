#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class GLWindow;

template<typename T>
class GLArray {
public:
	GLArray() : m_iRow(0), m_iCol(0) {}

	void Init(int iRow, int iCol, vector<T> data) {
		m_data = data;
		m_iRow = iRow;
		m_iCol = iCol;
	}

	const T* Data() const {
		return Size() > 0 ? &m_data[0] : nullptr;
	}
	const int Size() const {
		return m_iRow * m_iCol * sizeof(T);
	}

	int GetRow() const { return m_iRow; }
	int GetCol() const { return m_iCol; }
	void SetRow(int iRow) { m_iRow = iRow; }
	void SetCol(int iCol) { m_iCol = iCol; }

private:
	vector<T> m_data;
	int m_iRow, m_iCol;
};

typedef GLArray<GLfloat> GLfloatArray;
typedef GLArray<GLushort> GLushortArray;
typedef GLArray<GLuint> GLuintArray;
typedef GLArray<GLint> GLintArray;

class GLObject {
public:
	virtual void Init() = 0;
	virtual void Draw() = 0;


	// Load Methods
	bool LoadResources(GLfloatArray *verts, GLfloatArray *colors, GLushortArray *indexes) {
		// vertex array
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		int numLocation = 0;
		// init vertex buffer 
		if (verts != nullptr) {
			m_vboNum = verts->GetRow();

			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, verts->Size(), verts->Data(), GL_STATIC_DRAW);
			glVertexAttribPointer(numLocation, verts->GetCol(), GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(numLocation);
			numLocation++;
		}

		// init color buffer
		if (colors != nullptr) {
			m_cboNum = colors->GetRow();

			glGenBuffers(1, &m_cbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
			glBufferData(GL_ARRAY_BUFFER, colors->Size(), colors->Data(), GL_STATIC_DRAW);
			glVertexAttribPointer(numLocation, colors->GetCol(), GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(numLocation);
			numLocation++;
		}

		// init index buffer
		if (indexes != nullptr) {
			m_eboNum = indexes->GetRow();

			glGenBuffers(1, &m_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes->Size(), indexes->Data(), GL_STATIC_DRAW);
		}

		return true;
	}

	bool LoadCoords(GLfloatArray &coords, string path) {
		return false;
	}

	// Draw Methods
	void DrawArrays(GLenum type, int first, int nums) {
		glBindVertexArray(m_vao);
		glDrawArrays(type, first, nums);
	}

	void DrawArrays(GLenum type) {
		return glDrawArrays(type, 0, m_vboNum);
	}

	void DrawElements() {

	}
	
	// Getter and Setter
	void SetWindow(GLWindow* window) {
		this->m_window = window;
	}

	GLWindow* GetWindow() const{
		return m_window;
	}

private:
	GLuint m_vao, m_vbo, m_cbo, m_ebo;
	GLint m_vaoNum, m_vboNum, m_cboNum, m_eboNum;

	GLWindow* m_window;
};