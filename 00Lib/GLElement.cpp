#include "GLElement.h"
#include "GLApp.h"
#include <GL/SOIL.h>

GLWindow* GLElement::GetWindow() const {
	return GLApp::GetInstance()->GetWindow();
}


// Load Methods
void GLSimpleElement::LoadVertexArray(GLfloatArray &arr, GLint layoutIndex, GLenum drawMethod) {
	m_vertexArrayInfo.push_back({layoutIndex, arr, 0, drawMethod});
}

void GLSimpleElement::LoadIndex(vector<GLushort> &indexVec) {
	this->m_indexArr.Init(1, indexVec.size(), indexVec);
}

void GLSimpleElement::LoadTexture(string path, int texid, GLint loc) {
	this->m_textureInfo.push_back({ texid, path, 0, loc});
}

// init opengl vao, vbo and ebo
void GLSimpleElement::InitGLResources() {
	// vao
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// vbos
	for (size_t i = 0; i < m_vertexArrayInfo.size(); i++) {
		VertexArrayInfo *vinfo = &m_vertexArrayInfo[i];
		GLfloatArray *arr = &(vinfo->paramArr);

		glGenBuffers(1, &(vinfo->bo));
		glBindBuffer(GL_ARRAY_BUFFER, vinfo->bo);
		glBufferData(GL_ARRAY_BUFFER, vinfo->paramArr.Size(), arr->Data(), vinfo->drawMethod);
		glVertexAttribPointer(vinfo->layoutIndex, arr->GetCol(), GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(vinfo->layoutIndex);
	}

	// ebo, only init when index array is set
	if (m_indexArr.Size() != 0) {
		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexArr.Size(), m_indexArr.Data(), GL_STATIC_DRAW);
	}

	// texuture
	for (size_t i = 0; i < m_textureInfo.size(); i++) {
		TextureInfo *tinfo = &m_textureInfo[i];
		int width, height;

		unsigned char* image = SOIL_load_image(tinfo->path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

		glGenTextures(1, &(tinfo->to));
		glBindTexture(GL_TEXTURE_2D, tinfo->to);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); // Linear Min Filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		SOIL_free_image_data(image);
	}

	// reset binding vao
	glBindVertexArray(0);
}

// Draw Methods
void GLSimpleElement::DrawArrays(GLenum type, int first, int nums) {
	for (size_t i = 0; i < m_textureInfo.size(); i++) {
		TextureInfo *tinfo = &m_textureInfo[i];
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, tinfo->to);
		glUniform1i(tinfo->loc, i);
	}
	glBindVertexArray(m_vao);
	glDrawArrays(type, first, nums);
	glBindVertexArray(0);
}

void GLSimpleElement::DrawArrays(GLenum type) {
	DrawArrays(type, 0, m_vertexArrayInfo[0].paramArr.GetRow());
}

void GLSimpleElement::DrawElements(GLenum type) {
	for (size_t i = 0; i < m_textureInfo.size(); i++) {
		TextureInfo *tinfo = &m_textureInfo[i];
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, tinfo->to);
		glUniform1i(tinfo->loc, i);
	}
	glBindVertexArray(m_vao);
	glDrawElements(type, m_indexArr.GetCol(), GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
}
