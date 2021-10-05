#pragma once
#include<array>
#include "../CoreMinimal.h"
enum KEY_CODES
{
    // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    KEY_LMB = 0x00,
    KEY_RMB = 0x01,
    KEY_MMB = 0x02,
    KEY_MBACK = 0x03,
    KEY_MFORWARD = 0x04,
    KEY_A = 0x41,
    KEY_B = 0x42,
    KEY_C = 0x43,
    KEY_D = 0x44,
    KEY_E = 0x45,
    KEY_F = 0x46,
    KEY_G = 0x47,
    KEY_H = 0x48,
    KEY_I = 0x49,
    KEY_J = 0x4A,
    KEY_K = 0x4B,
    KEY_L = 0x4C,
    KEY_M = 0x4D,
    KEY_N = 0x4E,
    KEY_O = 0x4F,
    KEY_P = 0x50,
    KEY_Q = 0x51,
    KEY_R = 0x52,
    KEY_S = 0x53,
    KEY_T = 0x54,
    KEY_U = 0x55,
    KEY_V = 0x56,
    KEY_W = 0x57,
    KEY_X = 0x58,
    KEY_Y = 0x59,
    KEY_Z = 0x5A,
    KEY_ESCAPE = 0x1B
};
struct KeyState
{
    bool bPressed;
    bool bReleased;
    bool bHeld;
    bool state{};
    KeyState() : bPressed{ false }, bReleased{ false }, bHeld{ false }{}
};

struct MouseKeyState
{
    bool bPressed;
    bool bReleased;
    bool bHeld;
    bool state{};
    MouseKeyState() : bPressed{ false }, bReleased{ false }, bHeld{ false }{}
};

struct MousePosition
{
    /// The change in X since the last frame only
    float deltaX{};
    /// The change in Y since the last frame only
    float deltaY{};
    long lastX{};
    long lastY{};
    long x{};
    long y{};
};

struct ScrollState
{
    /// The change in X since the last frame only
    float deltaX{};
    /// The change in Y since the last frame only
    float deltaY{};

    float x{};
    float y{};
};

/// Singleton 
/// Only getters are publically exposed, thus the input cannot be changed outside of the Engine class, reducing coupling.
class Input
{
    friend class Engine;
public:

    static Input* getInstance();
    const KeyState& getKeyState(short key);
    const MousePosition& getMousePosition() const;
    const MouseKeyState& getMouseKeyState(uint8 key) const;

private:
    ~Input();
    Input();

    void setMousePosition(struct GLFWwindow* window, double xpos, double ypos);
    void setScrollState(double xoffset, double yoffset);
    void updateKeyState(struct GLFWwindow* window);
    void updateMousePosition();
    void updateMouseKeyState(struct GLFWwindow* window);
    void updateScrollState();

    static constexpr int m_keysToCapture{ 257 }; // 257 because escape is number 256
    static constexpr int m_mouseKeysToCapture{ 8 };
    static Input* instance;
    std::array<KeyState, m_keysToCapture> m_keyStateCurrent;
    std::array<MouseKeyState, m_mouseKeysToCapture> m_mouseKeyStateCurrent;
    MousePosition m_mousePosition;
    ScrollState m_scrollState{};
};