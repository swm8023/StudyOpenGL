#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <cstdio>
#include <set>
#include <iostream>
using namespace std;

class GLElement;

class GLWindow {
public:
	GLWindow(int iWidth, int iHeight, string strTitle);

	// interface
	virtual void Initialize() {};
	virtual void BeforeUpdate() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	};

	virtual void AfterUpdate() {
		glFlush();
	};
	virtual void Reshape(int, int) {};

	virtual void OnMouse(int key, int state, int x, int y) {}
	virtual void OnKey(unsigned char key, int state, int x, int y) {}
	virtual void OnWheel(int key, int state, int x, int y) {}
	virtual void OnMotion(int x, int y) {}
	virtual void OnPassiveMotion(int x, int y) {}


	void Update();

	// Getter and Setter	
	const int GetWidth() const;
	const int GetHeight() const ;
	const string GetTitle() const;

	void SetWidth(int iWidth);
	void SetHeight(int iHeight);
	void SetTitle(string strTitle);

	void addElement(GLElement* pElem);

private:
	int m_iWidth;
	int m_iHeight;
	string m_strTitle;

	set<GLElement*> m_elemSet;
};
