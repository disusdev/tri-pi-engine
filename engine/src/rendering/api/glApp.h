#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "math/mathm.h"
#include "core/logger.h"
#include "core/asserts.h"

class GLApp
{
public:
	GLApp()
	{
		glfwSetErrorCallback(
			[](int error, const char* description)
			{
				LOG_ERROR("Error: %s", description);
			}
		);

		if (!glfwInit())
		{
			ASSERT(false);
			exit(EXIT_FAILURE);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		const GLFWvidmode* info = glfwGetVideoMode(glfwGetPrimaryMonitor());

		// window_ = glfwCreateWindow(info->width, info->height, "Simple example", nullptr, nullptr);
		window_ = glfwCreateWindow(800, 600, "tri-pi-engine", nullptr, nullptr);

		if (!window_)
		{
			glfwTerminate();
			ASSERT(false);
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window_);
		gladLoadGL(glfwGetProcAddress);
		glfwSwapInterval(0);

		// gl debug logs
		// initDebug();
	}
	~GLApp()
	{
		glfwDestroyWindow(window_);
		glfwTerminate();
	}
	GLFWwindow* getWindow() const { return window_; }
	float getDeltaSeconds() const { return deltaSeconds_; }
	void swapBuffers()
	{
		glfwSwapBuffers(window_);
		glfwPollEvents();
		ASSERT(glGetError() == GL_NO_ERROR);

		const double newTimeStamp = glfwGetTime();
		deltaSeconds_ = static_cast<float>(newTimeStamp - timeStamp_);
		timeStamp_ = newTimeStamp;
	}

private:
	GLFWwindow* window_ = nullptr;
	double timeStamp_ = glfwGetTime();
	float deltaSeconds_ = 0;
};
