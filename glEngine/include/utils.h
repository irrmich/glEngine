#include <glEngine.h>

class version
{
public:
	static void printGlfwVersion()
	{
		int major, minor, revision;
		glfwGetVersion(&major, &minor, &revision);
		printf("Running against GLFW %i.%i.%i\n", major, minor, revision);
	}
};
