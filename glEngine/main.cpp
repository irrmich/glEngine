#include <dependencies.h>
// GL includes

int main()
{
	//Create new glEngine instance
	glEngine* engine = new glEngine();

	//show windowing version (GLFW)
	version::printGlfwVersion();

	//Init graphics and scene
	engine->initGraphics(800, 600, true, true, true, true, false, 4);
	engine->createScene();

	//Save engine instance into eventManager
	if (eventManager::registerGameSingleton(engine))
	{
		engine->startRendering();
		return 0;
	}
	return 1;
}
