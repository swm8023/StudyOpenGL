#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "GLArray.h"

using namespace std;

class GLWindow;

class GLElement {
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;

// Getter and Setter
	GLWindow* GetWindow() const;
};


class GLSimpleElement : public GLElement {
public:
	GLSimpleElement() {
	}

	// Load Vertex Params, maybe vertex position, color or texture coords
	void LoadVertexArray(GLfloatArray &arr, GLint layoutIndex,
		GLenum drawMethod = GL_STATIC_DRAW);
	
	// Load Indexs, for glDrawElements
	void LoadIndex(vector<GLushort> &indexVec);

	// Load texture img
	void LoadTexture(string path, int texid);

	// init resources
	void InitGLResources();

	// Draw Methods
	void DrawArrays(GLenum type, int first, int nums);
	void DrawArrays(GLenum type);

	void DrawElements(GLenum type);

private:
	GLuint m_vao, m_ebo;

	// indexes
	GLushortArray m_indexArr;
	
	// vertex info 
	struct VertexArrayInfo {
		GLint layoutIndex;
		GLfloatArray paramArr;
		GLuint bo;	// buffer object
		GLenum drawMethod;
	};
	vector<VertexArrayInfo> m_vertexArrayInfo;

	// texture info
	struct TextureInfo {
		GLint texid;
		string path;
		GLuint to;	// texture object
	};
	vector<TextureInfo> m_textureInfo;
};