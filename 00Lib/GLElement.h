#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "GLArray.h"

#include <GL/assimp/Importer.hpp>
#include <GL/assimp/scene.h>
#include <GL/assimp/postprocess.h>

using namespace std;

class GLWindow;

class GLElement {
public:
	virtual bool Initialize() { return true; };
	virtual void Update() {};

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
	void LoadTexture(string path, int texid, GLint loc);

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
		GLint loc;
	};
	vector<TextureInfo> m_textureInfo;
};

// GLAssipElement;
class GLAssipElement : public GLElement {
public:
	GLAssipElement(string assipPath, int progid);
	virtual bool Initialize();
	virtual void Update();

private:
	vector<GLSimpleElement*> m_elms;
	string m_assipPath;
	int m_progid;
	
	void ProcessNode(aiNode*, const aiScene*);
	GLSimpleElement* ProcessMesh(aiMesh*, const aiScene*);
};