#pragma once

#include "curses.h"

static constexpr int KEY_SPACE {' '};
const static mmask_t mouseMask{BUTTON1_PRESSED};

enum class UserInput { None, Left, Right, Up, Down, Quit, Space, Mouse1 };

struct MouseInput
{
public:
    int x{}; 
    int y{};
};

class Input
{
public:
    Input();

    UserInput Read();
};

extern MouseInput mouseInput;
extern UserInput userInput;