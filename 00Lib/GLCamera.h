#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtc/type_ptr.hpp>
using namespace glm;

enum GL_CAMERA_EVT {
	CAMERA_GO_FORWARD,
	CAMERA_GO_BACKWARD,
	CAMERA_GO_LEFT,
	CAMERA_GO_RIGHT,
	CAMERA_GO_UP,
	CAMERA_GO_DOWN,
	CAMERA_ZOOM_LESS,
	CAMERA_ZOOM_MORE,
	CAMERA_DIRECT_MOVE
};

class GLCamera {
public:
	GLCamera() {
		Initialize(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	}

	GLCamera(vec3 pos, vec3 up, GLfloat aspect = 45.0f, GLfloat yaw = -90.0f, GLfloat pitch = 0.0f) {
		Initialize(pos, up, aspect, yaw, pitch);
	}

	void Initialize(vec3 pos, vec3 up, GLfloat aspect = 45.0f, GLfloat yaw = -90.0f, GLfloat pitch = 0.0f) {
		this->m_camPos = pos;
		this->m_worldUp = up;
		this->m_aspect = aspect;
		this->m_yaw = yaw;
		this->m_pitch = pitch;
		updateVectors();
	}

	GLfloat GetAspect() {
		return m_aspect;
	}

	mat4 GetViewMatrix() {
		return lookAt(m_camPos, m_camFront, m_camUp);
	} 


	void HandleEvent(GL_CAMERA_EVT evt, GLfloat offset) {
		switch (evt) {
		case CAMERA_GO_FORWARD:
			m_camPos += m_camFront * offset;
			break;
		case CAMERA_GO_BACKWARD:
			m_camPos -= m_camFront * offset;
			break;
		case CAMERA_GO_UP:
			m_camPos += m_camUp * offset;
			break;
		case CAMERA_GO_DOWN:
			m_camPos -= m_camUp * offset;
			break;
		case CAMERA_GO_LEFT:
			m_camPos -= m_camRight * offset;
			break;
		case CAMERA_GO_RIGHT:
			m_camPos += m_camRight * offset;
			break;
		case CAMERA_ZOOM_LESS:
			this->m_aspect -= offset;
			break;
		case CAMERA_ZOOM_MORE:
			this->m_aspect += offset;
			break;
		default:
			return;
		}
		// adjust aspect
		if (m_aspect <= 1.0f) m_aspect = 1.0f;
		if (m_aspect >= 45.0f) m_aspect = 45.0f;
	}
	

	void HandleEvent(GL_CAMERA_EVT evt, GLfloat xOffset, GLfloat yOffset) {
		switch (evt) {
		case CAMERA_DIRECT_MOVE:
			m_yaw += xOffset;
			m_pitch += yOffset;
			break;
		default:
			return;
		}
		// adjust pitch
		if (m_pitch < -89.0f) m_pitch = -89.0f;
		if (m_pitch > 89.0f) m_pitch = 89.0f;
		updateVectors();
	}


private:
	vec3 m_camPos, m_camFront, m_camUp, m_camRight, m_worldUp;
	GLfloat m_aspect, m_yaw, m_pitch;

	void updateVectors() {
		m_camFront.x = cos(radians(m_yaw)) * cos(radians(m_pitch));
		m_camFront.y = sin(radians(m_pitch));
		m_camFront.z = sin(radians(m_yaw)) * cos(radians(m_pitch));
		m_camFront = normalize(m_camFront);
		m_camRight = normalize(cross(m_camFront, m_worldUp));
		m_camUp = normalize(cross(m_camRight, m_camFront));

	}

};
