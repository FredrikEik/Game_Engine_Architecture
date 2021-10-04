#include "Input.h"
#include "GLFW/glfw3.h"
#include <iostream>

Input::Input()
{
}

Input::~Input()
{
    instance = nullptr;
}

Input* Input::instance = nullptr;

Input* Input::getInstance()
{
	if (!instance)
		instance = new Input;
	return instance;
}

const KeyState& Input::getKeyState(short key)
{
	return m_keyStateCurrent[key];
}

const MousePosition& Input::getMousePosition() const
{
	return m_mousePosition;
}

const MouseKeyState& Input::getMouseKeyState(uint8 key) const
{
    return m_mouseKeyStateCurrent[key];
}

void Input::setMousePosition(GLFWwindow* window, double xpos, double ypos)
{
    m_mousePosition.x = xpos;
    m_mousePosition.y = ypos;
}

void Input::updateKeyState(GLFWwindow* window)
{
    updateMousePosition();
    updateMouseKeyState(window);
    updateScrollState();
	bool keyStateNew[m_keysToCapture];
    for(int i{}; i < m_keysToCapture; ++i)
    {
        keyStateNew[i] = glfwGetKey(window, i);

        m_keyStateCurrent[i].bPressed = false;
        m_keyStateCurrent[i].bReleased = false;

        if(keyStateNew[i] != this->m_keyStateCurrent[i].state)
        {
            if(keyStateNew[i]) //0x8000 means high bet is set. Key is currently held down
            {
                m_keyStateCurrent[i].bPressed = !m_keyStateCurrent[i].bHeld;
                m_keyStateCurrent[i].bHeld = true;
            }
            else
            {
                m_keyStateCurrent[i].bReleased = true;
                m_keyStateCurrent[i].bHeld = false;
            }
        }
        m_keyStateCurrent[i].state = keyStateNew[i];
    }
}

void Input::updateMousePosition()
{
    m_mousePosition.deltaX = static_cast<float>(m_mousePosition.x - m_mousePosition.lastX);
    m_mousePosition.deltaY = static_cast<float>(m_mousePosition.y - m_mousePosition.lastY);

    m_mousePosition.lastX = m_mousePosition.x;
    m_mousePosition.lastY = m_mousePosition.y;
}

void Input::updateMouseKeyState(GLFWwindow* window)
{
    bool keyStateNew[m_mouseKeysToCapture];
    for (int i{}; i < m_mouseKeysToCapture; ++i)
    {
        keyStateNew[i] = glfwGetMouseButton(window, i);

        m_mouseKeyStateCurrent[i].bPressed = false;
        m_mouseKeyStateCurrent[i].bReleased = false;

        if (keyStateNew[i] != this->m_mouseKeyStateCurrent[i].state)
        {
            if (keyStateNew[i]) //0x8000 means high bet is set. Key is currently held down
            {
                m_mouseKeyStateCurrent[i].bPressed = !m_mouseKeyStateCurrent[i].bHeld;
                m_mouseKeyStateCurrent[i].bHeld = true;
            }
            else
            {
                m_mouseKeyStateCurrent[i].bReleased = true;
                m_mouseKeyStateCurrent[i].bHeld = false;
            }
        }
        m_mouseKeyStateCurrent[i].state = keyStateNew[i];
    }
}

void Input::updateScrollState()
{
    m_scrollState.deltaX = m_scrollState.x;
    m_scrollState.deltaY = m_scrollState.y;

    m_scrollState.x = 0;
    m_scrollState.y = 0;
}

void Input::setScrollState(double xoffset, double yoffset)
{
    m_scrollState.x = xoffset;
    m_scrollState.y = yoffset;
}
