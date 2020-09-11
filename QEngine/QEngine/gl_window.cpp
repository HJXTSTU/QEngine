#include "gl_window.h"


WindowBuilder& WindowBuilder::AddWindowHint(int hint, int value) {
	windowHints.push_back(WindowHint{ hint,value });
	return *this;
}

WindowBuilder& WindowBuilder::SetSize(int width, int height) {
	this->width = width;
	this->height = height;
	return *this;
}

WindowBuilder& WindowBuilder::SetTitle(const char *title) {
	this->title = title;
	return *this;
}

std::shared_ptr<GLWindow> WindowBuilder::BuildDefaultWindow() {
	if (!glfwInit()) {
		return NULL;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow *window = glfwCreateWindow(1024, 1024, "QEngine", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return std::make_shared<GLWindow>(window);
}

std::shared_ptr<GLWindow> WindowBuilder::BuildWindow() {
	if (!glfwInit()) {
		return NULL;
	}
	std::list<WindowHint>::iterator it = this->windowHints.begin();
	while (it != this->windowHints.end()) {
		glfwWindowHint(it->hint, it->value);
	}
	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	return std::make_shared<GLWindow>(window);
}


GLWindow::GLWindow() {
	this->window = NULL;
}

GLWindow::GLWindow(GLFWwindow *window) {
	this->window = window;
}

GLboolean GLWindow::InitWindow(int WIDTH, int HEIGHT, const char *title) {
	if (!glfwInit()) {
		return GL_FALSE;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		return GL_FALSE;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return GL_TRUE;
}

void GLWindow::SetCursorPosCallback(GLFWcursorposfun func) {
	glfwSetCursorPosCallback(window, func);
}

void GLWindow::SetScrollCallback(GLFWscrollfun func) {
	glfwSetScrollCallback(window, func);
}

void GLWindow::DisableCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GLWindow::EnableCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

int GLWindow::ShouldClose() {
	return glfwWindowShouldClose(window);
}

void GLWindow::CloseWindow() {
	glfwSetWindowShouldClose(this->window, true);
}

void GLWindow::Destory()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

bool GLWindow::GetKey(int key) {
	return glfwGetKey(this->window, key) == GLFW_PRESS;
}




void GLWindow::EndRender() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

GLFWwindow * GLWindow::GetWindow() {
	return this->window;
}