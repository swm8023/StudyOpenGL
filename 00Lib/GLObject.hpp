#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class GLArray {
public:
	GLArray() : m_iRow(0), m_iCol(0) {}

	const T* Data() const {
		return Size() > 0 ? &data[0] : nullptr;
	}
	const int Size() const {
		return m_iRow * m_iCol * sizeof(T);
	}

	int GetRow() { return m_iRow; }
	int GetCol() { return m_iCol; }
	void SetRow(int iRow) { m_iRow = iRow; }
	void SetCol(int iCol) { m_iCol = iCol; }

	void Init(vector<T> &data, int iRow, int iCol) {
		m_data = data;
		m_iRow = iRow;
		m_iCol = iCol;
	}
private:
	vector<T> m_data;
	int m_iRow, m_iCol;
};

typedef GLArray<GLfloat> GLfloatArray;
typedef GLArray<GLushort> GLushortArray;

class GLObject {
public:
	bool LoadResources(GLfloatArray *verts, GLfloatArray *colors, GLushortArray *indexes) {
		// vertex array
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		int numLocation = 0;
		// verts 
		if (verts != nullptr) {
			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, verts->Size(), verts->Data(), GL_STATIC_DRAW);
			glVertexAttribPointer(numLocation, verts->GetCol(), GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(numLocation);
			numLocation++;
		}

		// init colors
		if (colors != nullptr) {
			glGenBuffers(1, &m_cbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
			glBufferData(GL_ARRAY_BUFFER, colors->Size(), colors->Data(), GL_STATIC_DRAW);
			glVertexAttribPointer(numLocation, colors->GetCol(), GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(numLocation);
			numLocation++;
		}

		// init indexes
		if (indexes != nullptr) {
			glGenBuffers(1, &m_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes->Size(), indexes->Data(), GL_STATIC_DRAW);
		}


		return true;
	}

	bool LoadCoords(GLfloatArray &coords, string path) {
		return false;
	}

	void DrawArrays(GLenum type, int first, int nums) {
		glBindVertexArray(m_vao);
		glDrawArrays(type, first, nums);
	}

	void DrawElements() {
	}

private:
	GLuint m_vao, m_vbo, m_cbo, m_ebo;
};