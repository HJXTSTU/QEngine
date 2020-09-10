#include "input.h"
std::vector<int> Input::keyCodeHash;

bool Input::keyStatus[KEY_CODE_TOTAL][KEY_TOTAL_STATUS];

void Input::pushKeyHashRange(int from, int to) {
	int cur = from;
	while (cur <= to) {
		keyCodeHash.push_back(cur);
		cur++;
	}
}

void Input::pushKeyHash(int key) {
	keyCodeHash.push_back(key);
}

void Input::generateKeyCodeHash() {
	pushKeyHash(GLFW_KEY_SPACE);
	pushKeyHash(GLFW_KEY_APOSTROPHE);

	pushKeyHashRange(GLFW_KEY_COMMA, GLFW_KEY_9);


	pushKeyHash(GLFW_KEY_SEMICOLON);
	pushKeyHash(GLFW_KEY_EQUAL);

	pushKeyHashRange(GLFW_KEY_A, GLFW_KEY_RIGHT_BRACKET);


	pushKeyHash(GLFW_KEY_GRAVE_ACCENT);
	pushKeyHash(GLFW_KEY_WORLD_1);
	pushKeyHash(GLFW_KEY_WORLD_2);

	pushKeyHashRange(GLFW_KEY_ESCAPE, GLFW_KEY_END);

	pushKeyHashRange(GLFW_KEY_CAPS_LOCK, GLFW_KEY_PAUSE);

	pushKeyHashRange(GLFW_KEY_F1, GLFW_KEY_F25);

	pushKeyHashRange(GLFW_KEY_KP_0, GLFW_KEY_KP_EQUAL);
	pushKeyHashRange(GLFW_KEY_LEFT_SHIFT, GLFW_KEY_MENU);
}

bool Input::getKeyDown(KeyCode keyCode) {
	return Input::keyStatus[keyCode][KeyStatus::KEY_PRESS];
}

bool Input::getKey(KeyCode keyCode) {
	return Input::keyStatus[keyCode][KeyStatus::KEY_PRESSING];
}

bool Input::getKeyUp(KeyCode keyCode) {
	return Input::keyStatus[keyCode][KeyStatus::KEY_RELEASED];
}

InputUpdator InputUpdator::instance;

InputUpdator::InputUpdator() {
	Input::generateKeyCodeHash();
}

void InputUpdator::refreshKeyStatus(std::shared_ptr<GLWindow> window)
{
	for (int i = 0; i < KEY_CODE_TOTAL; i++) {
		int lastPressingState = Input::keyStatus[i][KEY_PRESSING];
		Input::keyStatus[i][KEY_PRESSING] = window->GetKey(Input::keyCodeHash[i]);
		if (lastPressingState != Input::keyStatus[i][KEY_PRESSING]) {
			Input::keyStatus[i][KEY_PRESS] = Input::keyStatus[i][KEY_PRESSING];
			Input::keyStatus[i][KEY_RELEASED] = !Input::keyStatus[i][KEY_PRESSING];
		}
		else {
			Input::keyStatus[i][KEY_PRESS] = false;
			Input::keyStatus[i][KEY_RELEASED] = false;
		}
	}
}
