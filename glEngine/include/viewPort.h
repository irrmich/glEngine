#ifndef VIEWPORT_H_INCLUDED
#define VIEWPORT_H_INCLUDED
//include GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <learnopengl/camera.h>

class Viewport
{
public:
	//Enumerates predefined viewports
    typedef enum
    {
		VT_NONE=0,			//Not predefined
        VT_NORMAL,			//Draw on entire framebuffer surface
        VT_BOTTOM,			//Draw on bottom, at half screen
        VT_BOTTOM_LEFT,
        VT_BOTTOM_RIGHT,
        VT_TOP,
        VT_TOP_LEFT,
        VT_TOP_RIGHT,
        VT_LEFT,
        VT_RIGHT
    }VIEWPORT_TYPE;

    //Create Viewport
    Viewport(GLFWwindow* window,int lowerLeftCornerX,int lowerLeftCornerY,int width,int height,Camera* camera):
        m_window(window),m_lowerLeftCornerX(lowerLeftCornerX),
        m_lowerLeftCornerY(lowerLeftCornerY),m_width(width),
		m_height(height),m_camera(camera)
    {

    }
	  
    Viewport(GLFWwindow* window,VIEWPORT_TYPE type,int wndWidth,int wndHeight,Camera* camera):m_wndWidth(wndWidth),m_wndHeight(wndHeight)
    {
		m_window = window;

		//Only one camera per viewport
		this->setCamera(camera);

		//Set predefined viewport
		this->setType(type);
    }
	
	Viewport(GLFWwindow* window, VIEWPORT_TYPE type, Camera* camera)
	{
		m_window = window;

		//Get entire drawable surface
		this->RetrieveFrameBufferSize();
		 
		//Only one camera per viewport
		this->setCamera(camera);

		//Set predefined viewport
		this->setType(type);
	}

	void setType(VIEWPORT_TYPE type)
	{
		m_type = type;

		switch (m_type)
		{
		case VT_NORMAL:
		{
			m_lowerLeftCornerX = 0;
			m_lowerLeftCornerY = 0;
			m_width = m_wndWidth;
			m_height = m_wndHeight;

			//Viewport(window, 0, 0, wndWidth, wndHeight);
			break;
		}
		case VT_RIGHT:
		{
			m_lowerLeftCornerX = m_wndWidth / 2;
			m_lowerLeftCornerY = 0;
			m_width = m_wndWidth / 2;
			m_height = m_wndHeight;
			break;
			//Viewport(window,wndWidth/2,0,wndWidth/2,wndHeight);
			//break;
		}
		case VT_LEFT:
		{
			m_lowerLeftCornerX = 0;
			m_lowerLeftCornerY = 0;
			m_width = m_wndWidth / 2;
			m_height = m_wndHeight;
			break;
			/*Viewport(window,0,0,wndWidth/2,wndHeight);
			break;*/
		}
		case VT_TOP:
		{
			m_lowerLeftCornerX = 0;
			m_lowerLeftCornerY = m_wndHeight / 2;
			m_width = m_wndWidth;
			m_height = m_wndHeight;
			break;
			/*Viewport(window,0,wndHeight/2,wndWidth,wndHeight);
			break;*/
		}
		case VT_TOP_LEFT:
		{
			m_lowerLeftCornerX = 0;
			m_lowerLeftCornerY = m_wndHeight / 2;
			m_width = m_wndWidth / 2;
			m_height = m_wndHeight / 2;
			break;
			/*Viewport(window,0,wndHeight/2,wndWidth/2,wndHeight/2);
			break;*/
		}
		case VT_TOP_RIGHT:
		{
			m_lowerLeftCornerX = m_wndWidth / 2;
			m_lowerLeftCornerY = m_wndHeight / 2;
			m_width = m_wndWidth / 2;
			m_height = m_wndHeight / 2;
			break;
			/*Viewport(window,wndWidth/2,wndHeight/2,wndWidth/2,wndHeight/2);
			break;*/
		}
		case VT_BOTTOM:
		{
			m_lowerLeftCornerX = 0;
			m_lowerLeftCornerY = 0;
			m_width = m_wndWidth;
			m_height = m_wndHeight / 2;
			break;
			/*Viewport(window,0,0,wndWidth,wndHeight/2);
			break;*/
		}
		case VT_BOTTOM_LEFT:
		{
			m_lowerLeftCornerX = 0;
			m_lowerLeftCornerY = 0;
			m_width = m_wndWidth / 2;
			m_height = m_wndHeight / 2;
			break;
			/*Viewport(window,0,0,wndWidth/2,wndHeight/2);
			break;*/
		}
		case VT_BOTTOM_RIGHT:
		{
			m_lowerLeftCornerX = m_wndWidth / 2;
			m_lowerLeftCornerY = 0;
			m_width = m_wndWidth / 2;
			m_height = m_wndHeight / 2;
			break;
			/*Viewport(window,wndWidth/2,0,wndWidth/2,wndHeight/2);
			break;*/
		}
		case VT_NONE:
		{
			m_lowerLeftCornerX = 0;
			m_lowerLeftCornerY = 0;
			m_width = 0;
			m_height = 0;
			break;
		}
		default:
		{
			m_lowerLeftCornerX = 0;
			m_lowerLeftCornerY = 0;
			m_width = m_wndWidth;
			m_height = m_wndHeight;
			/*Viewport(window,0,0,wndWidth,wndHeight);*/
			break;
		}
		}
	}

	//Get viewport type
	VIEWPORT_TYPE getType() const
	{
		return m_type;
	}

	//Set Camera
	void setCamera(Camera* camera)
	{
		m_camera = camera;
	}
	
	//Returns Camera
	Camera* getCamera() const
	{
		return m_camera;
	}

    //Return height of the current window in pixel (can directly be used by glViewport() function)
    int getFrameBufferHeight()
    {
		return m_wndHeight;
    }

    //Return width of the current window in pixel (can directly be used by glViewport() function)
    int getFrameBufferWidth()
    {
		return m_wndWidth;
    }

	//Recalculates framebuffersize (entire drawable surface on the window
	void RetrieveFrameBufferSize()
	{
		glfwGetFramebufferSize(m_window, &m_wndWidth, &m_wndHeight);
	}

    //To be called every frame loop if there is more than one registered viewport
    void update(bool updateFrameBufferSize=false)
    {
		if (updateFrameBufferSize)
		{
			//Get entire drawable surface
			this->RetrieveFrameBufferSize();

			//Recalculates viewport data accordind to new buffersize
			this->setType(m_type);
		}

		//Apply changes
        glViewport(m_lowerLeftCornerX,m_lowerLeftCornerY,m_width,m_height);

		//Update camera aspect ratio according to the new resolution
		m_camera->updateAspectRatio(m_width, m_height);
    }

    //Reset Viewport data
    void setWindow(GLFWwindow* window)
    {
        m_window = window;
    }

    void setLowerLeftCorner(int x, int y)
    {
        m_lowerLeftCornerX=x;
        m_lowerLeftCornerY=y;
    }

	//Set viewport dimensions (not entire drawable surface which can contain more than one viewports)
    void setDimensions(int width,int height)
    {
        m_width=width;
        m_height=height;
    }

    int getLowerLeftCornerX()
    {
        return m_lowerLeftCornerX;
    }

    int getLowerLeftCornerY()
    {
        return m_lowerLeftCornerY;
    }

    //Returns the current Viewport width
    int getWidth()
    {
        return m_width;
    }

    //Get current height of the Viewport
    int getHeight()
    {
        return m_height;
    }
protected:

    GLFWwindow* m_window;
    int m_lowerLeftCornerX;
    int m_lowerLeftCornerY;
    int m_width;
    int m_height;

	//Entire surface drawable
	int m_wndWidth;
	int m_wndHeight;

	//Camera
	Camera* m_camera;

	//viewport type
	VIEWPORT_TYPE m_type;
};

#endif // VIEWPORT_H_INCLUDED
