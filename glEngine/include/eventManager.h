#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include <glEngine.h>

//This must be set only once
glEngine* glEngineSingleton=0;

class eventManager
{
public:

	static bool registerGameSingleton(glEngine* glEngineInstance)
	{
		//assert(glEngineInstance == 0);
		if (glEngineInstance != 0)
		{
			glEngineSingleton = glEngineInstance;
			initCallbackFunctions();
			return true;
		}
		return false;
	}

	//Check if singleton is already initialised
	static bool isSingletonIntialised()
	{
		return (getGameSingleton() != 0);
	}

	static glEngine* getGameSingleton()
	{
		return glEngineSingleton;
	}

	static void glfwErrorCallback(int error, const char* description)
	{
		puts(description);			//show error on console
	}
private:

	//Init callback functions
	static void initCallbackFunctions()
	{
		glfwSetKeyCallback(glEngineSingleton->getWindow(), &eventManager::key_callback);
		glfwSetCursorPosCallback(glEngineSingleton->getWindow(), &eventManager::mouse_callback);
		glfwSetScrollCallback(glEngineSingleton->getWindow(), &eventManager::scroll_callback);
		glfwSetFramebufferSizeCallback(glEngineSingleton->getWindow(), &eventManager::framebuffer_size_callback);
		glfwSetErrorCallback(&eventManager::glfwErrorCallback);
	}

	//Transfert events to glEngine instance
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		glEngineSingleton->key_callback(window, key, scancode, action, mode);
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		glEngineSingleton->scroll_callback(window, xoffset, yoffset);
	}

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		glEngineSingleton->mouse_callback(window, xpos, ypos);
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glEngineSingleton->framebuffer_size_callback(window, width, height);
	}
};

#endif