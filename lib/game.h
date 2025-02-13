#pragma once

#include "console.h"
#include "frame.h"
#include "grid.h"
#include "input.h"
#include "render.h"

class Game
{
public:
    Game(int fps);
    ~Game();

    void Start();
    void End(const string&& _endText = "");

protected:
    Console console{};
    Input input{};
    Render render{console};
    bool gameOver{};

    virtual void Update() = 0;

private:
    string endText{};
};