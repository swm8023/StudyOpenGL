#include "GLElement.h"
#include "GLApp.h"


// Load Methods
void GLElement::LoadVertexes(GLfloatArray *vertexes, int layoutIndex) {
	// create vertex object if not created
	if (m_vao == 0) {
		glGenVertexArrays(1, &m_vao);
	}
	glBindVertexArray(m_vao);
	m_vboNum = vertexes->GetRow();

	// create vertex buffer and enable vertex
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexes->Size(), vertexes->Data(), GL_STATIC_DRAW);
	glVertexAttribPointer(layoutIndex, vertexes->GetCol(), GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(layoutIndex);
}

void GLElement::LoadColors(GLfloatArray *colors, int layoutIndex) {
	// create vertex object if not created
	if (m_vao == 0) {
		glGenVertexArrays(1, &m_vao);
	}

	glBindVertexArray(m_vao);
	m_vboNum = colors->GetRow();

	// create color buffer and enable vertex
	glGenBuffers(1, &m_cbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
	glBufferData(GL_ARRAY_BUFFER, colors->Size(), colors->Data(), GL_STATIC_DRAW);
	glVertexAttribPointer(layoutIndex, colors->GetCol(), GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(layoutIndex);
}

void GLElement::LoadIndexes(GLushortArray *indexes) {
	m_eboNum = indexes->GetRow();

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes->Size(), indexes->Data(), GL_STATIC_DRAW);
}

void GLElement::LoadCoords(GLfloatArray &coords, string path) {
	;
}

// Draw Methods
void GLElement::DrawArrays(GLenum type, int first, int nums) {
	glBindVertexArray(m_vao);
	glDrawArrays(type, first, nums);
}

void GLElement::DrawArrays(GLenum type) {
	DrawArrays(type, 0, m_vboNum);
}

void GLElement::DrawElements(GLenum type) {
	glDrawElements(type, m_eboNum, GL_UNSIGNED_SHORT, NULL);
}

// Getter and Setter
GLWindow* GLElement::GetWindow() const {
	return GLApp::GetInstance()->GetWindow();
}

