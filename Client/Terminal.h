//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <utility>
#include <termios.h>
#include <vector>
#include "page.h"
#include <memory>

//Terminal class to handle the drawing part of the application to the console

enum class Mode
{
	NORMAL = 0,
  COMMAND = 1,
};

class Terminal
{
  public:
    struct termios curr_term_state;
    struct termios orig_term_state;
    Mode mode;
    int search_flag = 0;

    void DrawView(page_Sptr page);
    void DrawCommand(std::string);
    void DrawError(std::string err);
    void scrollDown();
    void scrollUp();
    void enterDir();
    void eraseStatusBar();
    void eraseErrorBar();
    int switchToNormalMode();
    int switchToCommandMode();
    void disableCursor();
    void enableCursor();
};

#endif