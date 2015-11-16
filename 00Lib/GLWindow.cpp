#include "GLWindow.h"
#include "GLElement.h"
#include "GLCamera.h"

GLWindow::GLWindow(int iWidth, int iHeight, string strTitle) {
	this->m_iWidth = iWidth;
	this->m_iHeight = iHeight;
	this->m_strTitle = strTitle;

	this->m_camera = new GLCamera();
}


void GLWindow::Update() {
	set<GLElement*>::iterator it = m_elemSet.begin();
	while (it != m_elemSet.end()) {
		GLElement *elem = *it;
		elem->Update();
		it++;
	}
}

const int GLWindow::GetWidth() const {
	return m_iWidth;
}

const int GLWindow::GetHeight() const {
	return m_iHeight;
}

const string GLWindow::GetTitle() const {
	return m_strTitle;
}

void GLWindow::SetWidth(int iWidth) {
	this->m_iWidth = iWidth;
}

void GLWindow::SetHeight(int iHeight) {
	this->m_iHeight = iHeight;
}

void GLWindow::SetTitle(string strTitle) {
	this->m_strTitle = strTitle;
}

void GLWindow::addElement(GLElement* pElem) {
	if (m_elemSet.find(pElem) == m_elemSet.end()) {
		m_elemSet.insert(pElem);
	}
}
