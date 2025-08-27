#include "UserControls.hpp"

void CUserControls::ComputeMatricesFromInputs(GLFWwindow* xWindow)
{

	// glfwGetTime is called only once, the first time this function is called
	static double dLastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double dCurrentTime = glfwGetTime();
	float rDeltaTime = float(dCurrentTime - dLastTime);

	// Get mouse position
	dXY xMousePos;
	glfwGetCursorPos(xWindow, &xMousePos.X, &xMousePos.Y);

	// Reset mouse position for next frame
	glfwSetCursorPos(xWindow, 1024/2, 768/2);

	// Compute new orientation
	m_rHorizontalAngle += m_rMouseSpeed * float(1024/2 - xMousePos.X );
	m_rVerticalAngle   += m_rMouseSpeed * float( 768/2 - xMousePos.Y );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	vec3 xDirection(cos(m_rVerticalAngle) * sin(m_rHorizontalAngle), sin(m_rVerticalAngle), cos(m_rVerticalAngle) * cos(m_rHorizontalAngle));
	
	// Right vector
	vec3 xRight = vec3(sin(m_rHorizontalAngle - 3.14f/2.0f), 0, cos(m_rHorizontalAngle - 3.14f/2.0f));
	
	// Up vector
	vec3 xUp = cross(xRight, xDirection);

	// Move forward
	if (glfwGetKey( xWindow, GLFW_KEY_UP ) == GLFW_PRESS){
		m_xPosition += xDirection * rDeltaTime * m_rSpeed;
	}
	// Move backward
	if (glfwGetKey( xWindow, GLFW_KEY_DOWN ) == GLFW_PRESS){
		m_xPosition -= xDirection * rDeltaTime * m_rSpeed;
	}
	// Strafe right
	if (glfwGetKey( xWindow, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		m_xPosition += xRight * rDeltaTime * m_rSpeed;
	}
	// Strafe left
	if (glfwGetKey( xWindow, GLFW_KEY_LEFT ) == GLFW_PRESS){
		m_xPosition -= xRight * rDeltaTime * m_rSpeed;
	}

	float FoV = m_rInitialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	m_xProjectionMatrix = perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	m_xViewMatrix = lookAt(
							m_xPosition,			// Camera is here
							m_xPosition+xDirection, // and looks here : at the same position, plus "direction"
							xUp						// Head is up (set to 0,-1,0 to look upside-down)
							);

	// For the next frame, the "last time" will be "now"
	dLastTime = dCurrentTime;
}