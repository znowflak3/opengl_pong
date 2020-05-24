#pragma once

#include <unordered_map>
#include <string>

class InputManager
{
public:
	InputManager();

	bool IsKeyPressed(std::string name);
	void ChangeKeyState(std::string name, bool  value);
private:
	std::unordered_map<std::string, bool> Keys;

	bool KeyExist(std::string name);
};