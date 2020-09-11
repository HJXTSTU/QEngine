#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <list>

class GLWindow;


struct WindowHint {
	int hint;
	int value;
};

class WindowBuilder {
private:
	std::list<WindowHint> windowHints;

	int width;
	int height;

	const char *title;
public:
	WindowBuilder &AddWindowHint(int hint, int value);

	WindowBuilder &SetSize(int width, int height);

	WindowBuilder &SetTitle(const char *title);

	std::shared_ptr<GLWindow>  BuildDefaultWindow();

	std::shared_ptr<GLWindow> BuildWindow();
};

class GLWindow {
private:
	GLFWwindow *window;
public:
	GLWindow();

	GLWindow(GLFWwindow *window);

	GLboolean InitWindow(int WIDTH, int HEIGHT, const char *title);

	void SetCursorPosCallback(GLFWcursorposfun func);

	void SetScrollCallback(GLFWscrollfun func);

	void DisableCursor();

	void EnableCursor();

	int ShouldClose();

	void CloseWindow();

	void Destory();

	void EndRender();

	bool GetKey(int key);

	GLFWwindow *GetWindow();
};