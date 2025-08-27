#ifndef __3D_USERCONTROL_HPP
#define __3D_USERCONTROL_HPP

#include "General.hpp"

class CUserControls
{

public:

	CUserControls() 
	{
		m_xPosition = vec3( 0, 0, 5 );
		m_rHorizontalAngle = 3.14f;
		m_rVerticalAngle = 0.0f;
		m_rInitialFoV = 45.0f;
		m_rSpeed = 3.0f;
		m_rMouseSpeed = 0.005f;
	}

	~CUserControls() {}

	void ComputeMatricesFromInputs(GLFWwindow* xWindow);
	
	mat4 GetViewMatrix() { return m_xViewMatrix; }
	mat4 GetProjectionMatrix() { return m_xProjectionMatrix; }

private:

	mat4 m_xViewMatrix; 
	mat4 m_xProjectionMatrix;

	// Initial position : on +Z
	vec3 m_xPosition;

	// Initial horizontal angle : toward -Z
	float m_rHorizontalAngle;

	// Initial vertical angle : none
	float m_rVerticalAngle;

	// Initial Field of View
	float m_rInitialFoV;
	
	// Initial speeds
	float m_rSpeed; // 3 units / second
	float m_rMouseSpeed;

};

#endif __3D_USERCONTROL_HPP