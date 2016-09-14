#ifndef glEngine_H
#define glEngine_H

//Some standard libraries required
#include <vector>

//include GLEW STATIC
#define GLEW_STATIC
#include <GL\glew.h>

//include GLFW
#include <GLFW\glfw3.h>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL/SOIL.h>
#include <learnopengl/filesystem.h>
#include <viewPort.h>

class glEngine
{
public:
	glEngine();
	bool initGraphics(int screenWidth=800,int screenHeight=600,bool enableVSync=true,
		bool enableShadows=true,bool resizable=false, bool enableDepthTest=true, bool fullScreen=false,short antiAliasing=0);
	void setViewport(int wndWidth, int wndHeight);
	void setVSync(bool enableVSync);
	void setAntialiasing(short antiAliasing);
	void setCursorVisible(bool visibility);
	void enableCursor(bool enable);
	void enableGammaCorrection(bool enable);
	void clearColorBuffers();
	bool createScene();
	//Camera* createCamera();
	bool frameRenderingQueued();
	bool startRendering();
	GLFWwindow* getWindow() const;
	//Static function (callbacks)
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
protected:
	bool closeWindow();
	void setWindowResizable(bool resizable);
	void enableDepthTest(bool enable);
	//void setCallbackFunctions();
	void doMovement();
private: 
	GLFWwindow* m_window;
	glm::vec4 m_backgroundColor;
	int m_screenWidth, m_screenHeight;
	short m_sampleCount;
	bool m_enableVSync, m_enableShadows,m_resizable;

	//USER DATA
	Shader* shader;
	Model* ourModel;

	// Camera
	std::vector<Viewport*> viewports; //viewPort
	bool keys[1024];
	GLfloat lastX = 400, lastY = 300;
	bool firstMouse = true;

	//Lights
	glm::vec3 pointLightPositions[2] = {
		glm::vec3(-100.f, 10.f, -100.f),
		glm::vec3(100,0,100)
	};

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
};

#endif //glEngine