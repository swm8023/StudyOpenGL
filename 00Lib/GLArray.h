#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
using namespace std;

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
