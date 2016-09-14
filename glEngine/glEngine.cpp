#include <glEngine.h>
#include <viewPort.h>

glEngine::glEngine(){}

bool glEngine::initGraphics(int screenWidth, int screenHeight, bool enableVSync,
	bool enableShadows, bool resizable, bool enableDepthTest, bool fullScreen, short antiAliasing)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_enableShadows = enableShadows;
	m_resizable = resizable;
	m_backgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Init GLFW
	glewExperimental = GL_TRUE;
	if (glfwInit())
	{
		//Use only OPENGL 3.3 Core
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		this->setAntialiasing(antiAliasing);
		this->setWindowResizable(resizable);
		
		m_window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", fullScreen ? glfwGetPrimaryMonitor() : nullptr, nullptr); // Windowed
		glfwMakeContextCurrent(m_window);

		// Options
		this->enableCursor(false);
		this->enableDepthTest(enableDepthTest);

		// Initialize GLEW to setup the OpenGL Function pointers
		glewExperimental = GL_TRUE;
	
		if (glewInit()==GLEW_OK)
		{
			//We don't use wndWidth because it corresponds to the main window size which doesn't represent the real drawable surface
			int drawableWidth, drawableHeight;
			glfwGetFramebufferSize(m_window, &drawableWidth, &drawableHeight);
			this->setViewport(drawableWidth, drawableHeight);
			this->setVSync(enableVSync);
			//this->enableGammaCorrection(true);
			return true;
		}else {return false;}
	}
	return false;
}

void glEngine::setAntialiasing(short antiAliasing)
{
	m_sampleCount = antiAliasing;
	glfwWindowHint(GLFW_SAMPLES, antiAliasing);
	if(m_sampleCount>0)glEnable(GL_MULTISAMPLE);
}

/*
Camera* glEngine::createCamera()
{
	return Camera(glm::vec3(0.0f, 0.0f, 3.0f));
}
*/

void glEngine::enableGammaCorrection(bool enable)
{
	//Should only be enabled if the fragment shader aren't already doing so.
	if (enable) {glEnable(GL_FRAMEBUFFER_SRGB);}
	else{glDisable(GL_FRAMEBUFFER_SRGB);}
}

void glEngine::enableDepthTest(bool enable)
{
	// Test depth (Test de profondeur) force openGL draw farthest faces first
	if (enable) {glEnable(GL_DEPTH_TEST);}
	else {glDisable(GL_DEPTH_TEST);}
}

void glEngine::setWindowResizable(bool resizable)
{
	if (!m_resizable){glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);}
	else{glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);}
}

void glEngine::setCursorVisible(bool visibility)
{
	if (!visibility){glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);}
	else{glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);}
}

void glEngine::enableCursor(bool enable)
{
	if (!enable){glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);}
	else{this->setCursorVisible(true);}
}

bool glEngine::createScene()
{
	//Create scene here :)
	// Setup and compile our shaders
	//shader = new Shader("resources/shaders/multiple_lights.vs", "resources/shaders/multiple_lights.frag");
	shader = new Shader("resources/shaders/standard.vs", "resources/shaders/standard.frag");

	// Load models
	ourModel = new Model("resources/objects/nanosuit/nanosuit.obj");
	return true;
}

bool glEngine::frameRenderingQueued()	//Called after graphics card received data for rendering
{
	return true;
}

bool glEngine::startRendering()
{
	// Rendering loop
	while (!glfwWindowShouldClose(m_window))
	{
		//Calculate deltaTime
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();

		//Check camera move
		this->doMovement();

		// Clear buffers
		this->clearColorBuffers();

		shader->Use();   // <-- Don't forget this one!
						// Transformation matrices
		glm::mat4 projection = viewports[0]->getCamera()->GetPerspectiveMatrix();//glm::perspective(camera.Zoom, (float)m_screenWidth / (float)m_screenHeight, 0.1f, 100.0f);
		glm::mat4 view = viewports[0]->getCamera()->GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		//Set lighthing uniforms
		glm::vec3 camPos = viewports[0]->getCamera()->Position;
		glUniform3f(glGetUniformLocation(shader->Program, "viewPos"), camPos.x, camPos.y, camPos.z);

		//Set shader shininess
		glUniform1f(glGetUniformLocation(shader->Program, "material.shininess"), 32.f);

		// Point light 
		glm::vec3 pointLight(1, 0.5, 1);
		glUniform3f(glGetUniformLocation(shader->Program, "pointLight.position"), pointLight.x, pointLight.y, pointLight.z);
		glUniform3f(glGetUniformLocation(shader->Program, "pointLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader->Program, "pointLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shader->Program, "pointLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader->Program, "pointLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(shader->Program, "pointLight.linear"), 0.009);
		glUniform1f(glGetUniformLocation(shader->Program, "pointLight.quadratic"), 0.0032);

		// Direction light 2
		glm::vec3 directionalLight(0, 0.f, 0);
		//directionalLight = normalize(directionalLight);
		glUniform3f(glGetUniformLocation(shader->Program, "directionalLight.direction"), directionalLight.x, directionalLight.y, directionalLight.z);
		glUniform3f(glGetUniformLocation(shader->Program, "directionalLight.ambient"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(shader->Program, "directionalLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shader->Program, "directionalLight.specular"), 1.0f, 1.0f, 1.0f);
		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ourModel->Draw(*shader);


		// Swap the buffers
		glfwSwapBuffers(m_window);
	}

	return this->closeWindow();
}

void glEngine::clearColorBuffers()
{
	//Clear background color
	glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w);
	
	//If DEPTH test is enabled, then clear both front buffer and depth buffer
	if(glIsEnabled(GL_DEPTH_TEST)==GL_TRUE) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

bool glEngine::closeWindow()
{
	glfwTerminate();
	return true;
}

void glEngine::setViewport(int wndWidth,int wndHeight)
{
	// Define the viewport dimensions
	Camera* cam = new Camera(wndWidth, wndHeight, glm::vec3(0.0f, 0.0f, 3.0f));
	Viewport* vp = new Viewport(m_window, Viewport::VIEWPORT_TYPE::VT_NORMAL, wndWidth, wndHeight, cam);
	viewports.push_back(vp);
	vp->update();
}

void glEngine::setVSync(bool enableVSync)
{
	//Enable vsync
	glfwSwapInterval((int)enableVSync);
}

void glEngine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void glEngine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	viewports[0]->getCamera()->ProcessMouseScroll(yoffset);
}

void glEngine::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	viewports[0]->getCamera()->ProcessMouseMovement(xoffset, yoffset);
}

void glEngine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	for (auto vp = viewports.begin(); vp != viewports.end(); vp++)
	{
		(*vp)->update(true);
	}
	
	//glViewport(0, 0, width, height);
}

void glEngine::doMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		viewports[0]->getCamera()->ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		viewports[0]->getCamera()->ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		viewports[0]->getCamera()->ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		viewports[0]->getCamera()->ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_T])
		viewports[0]->getCamera()->ProcessKeyboard(TOP, deltaTime);
	if (keys[GLFW_KEY_B])
		viewports[0]->getCamera()->ProcessKeyboard(BOTTOM, deltaTime);
}

GLFWwindow* glEngine::getWindow() const
{
	return m_window;
}