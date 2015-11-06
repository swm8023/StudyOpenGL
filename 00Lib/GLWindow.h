#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <set>
#include <iostream>
using namespace std;

class GLElement;

class GLWindow {
public:
	GLWindow(int iWidth, int iHeight, string strTitle);

	// interface
	virtual void Initialize() = 0;
	virtual void BeforeUpdate() = 0;
	virtual void AfterUpdate() = 0;
	virtual void Reshape(int, int) = 0;

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
