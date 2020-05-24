#include "InputManager.h"

InputManager::InputManager()
{
	Keys.insert(std::make_pair("W", false));
	Keys.insert(std::make_pair("S", false));
}

bool InputManager::IsKeyPressed(std::string name)
{
	return Keys[name];
}

void InputManager::ChangeKeyState(std::string name, bool value)
{
	Keys[name] = value;
}

bool InputManager::KeyExist(std::string name)
{
	if (Keys.find(name) == Keys.end())
		return false;
	else
		return true;
}
