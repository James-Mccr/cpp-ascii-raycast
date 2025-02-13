#include "input.h"
#include <curses.h>
#include <ncurses.h>

Input::Input()
{
    mouseinterval(0);
    mousemask(mouseMask, NULL);
}

UserInput Input::Read()
{
    int i = getch();
    MEVENT mouseEvent;
    switch (i)
    {
        case 'q': return UserInput::Quit;
        case KEY_LEFT: return UserInput::Left;
        case KEY_RIGHT: return UserInput::Right;
        case KEY_UP: return UserInput::Up;
        case KEY_DOWN: return UserInput::Down;
        case KEY_SPACE: return UserInput::Space;
        case KEY_MOUSE:
        {
            if (getmouse(&mouseEvent) != OK || !(mouseEvent.bstate & mouseMask))
                return UserInput::None;
            mouseInput.x = mouseEvent.x;
            mouseInput.y = mouseEvent.y;
            mousemask(mouseMask, NULL); // stops calls being synchronous for some reason
            return UserInput::Mouse1;
        }
        default: return UserInput::None;
    }
}

MouseInput mouseInput{};
UserInput userInput{};