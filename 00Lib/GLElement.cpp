#include "GLElement.h"
#include "GLApp.h"

// Load Methods
bool GLElement::LoadResources(GLfloatArray *verts, GLfloatArray *colors, GLushortArray *indexes) {
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

bool GLElement::LoadCoords(GLfloatArray &coords, string path) {
	return false;
}

// Draw Methods
void GLElement::DrawArrays(GLenum type, int first, int nums) {
	glBindVertexArray(m_vao);
	glDrawArrays(type, first, nums);
}

void GLElement::DrawArrays(GLenum type) {
	return glDrawArrays(type, 0, m_vboNum);
}

void GLElement::DrawElements() {

}

// Getter and Setter
GLWindow* GLElement::GetWindow() const {
	return GLApp::GetInstance()->GetWindow();
}

