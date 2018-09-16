//Name: Praveen Balireddy
//Roll: 2018201052

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <iomanip>
#include <string>
#include "Terminal.h"
#include "page.h"
#include "path.h"

//Draw the current state
void Terminal::DrawView(page_Sptr page)
{
    std::cout << "\e[2J" << std::flush;
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);       //getting terminal dimentions
    int rows = ws.ws_row;
    int col = ws.ws_col;

    int highlight_index = page->highlight_index;
    int begin = (highlight_index > rows - 4) ? (highlight_index - rows + 4) : 0;
    int offset = (rows - 3 > (page->files.size() - begin)) ? page->files.size() - begin : rows - 3;

    std::cout << "\033[1;1H"; //Move cursor to start
    if (this->search_flag == 0)     //when not in search mode
    {
        auto &path = Path::getInstance();
        std::cout << "Current Directory: " << path.getAppAbsPath(page->cwd + "/");
        int cursor_row = 2;
        for (int i = begin; i < begin + offset; i++)
        {
            std::cout << "\033[" << cursor_row << ";0H";
            std::cout << page->files[i]->getPermission() << " ";
            std::cout << std::left << std::setw(12) << page->files[i]->getUserName() << " ";
            std::cout << std::left << std::setw(12) << page->files[i]->getGroupName() << " ";
            std::cout << std::left << std::setw(7) << page->files[i]->getSize() << " ";
            std::cout << page->files[i]->getLastModified();
            if (highlight_index == i)       //highlighting in the selected index in the list
            {
                std::cout << "\033[30;46m " << page->files[i]->getFileName() << " "
                          << "\033[0m ";
            }
            else
            {
                if (page->files[i]->getFileType() == 'd')       //coloring the directories
                    std::cout << "\033[35;10m " << page->files[i]->getFileName() << " \033[0m";
                else
                    std::cout << " " << page->files[i]->getFileName() << " ";
            }
            std::cout << std::flush;
            cursor_row += 1;
        }
    }
    else
    {                                   //when in search mode
        int cursor_row = 2;
        for (int i = begin; i < begin + offset; i++)
        {
            std::cout << "\033[" << cursor_row << ";0H";
            if (highlight_index == i)
            {
                std::cout << "\033[30;46m " << Path::getInstance().getAppAbsPath(page->files[i]->getFileName()) << " "
                          << "\033[0m ";
            }
            else
            {
                std::cout << " " << Path::getInstance().getAppAbsPath(page->files[i]->getFileName()) << " ";
            }
            std::cout << std::flush;
            cursor_row += 1;
        }
    }
}

void Terminal::DrawCommand(std::string cmd)         //drawing the commands to the terminal
{
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    int rows = ws.ws_row;
    int col = ws.ws_col;
    this->eraseStatusBar();
    std::cout << ":" << cmd << std::flush;
}

void Terminal::DrawError(std::string err)         //drawing the errors to the terminal
{
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    int rows = ws.ws_row;
    int col = ws.ws_col;
    this->eraseErrorBar();
    std::cout << "Error: " << err << std::flush;
}

void Terminal::eraseStatusBar()                 //erasing the command bar
{
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    int rows = ws.ws_row;
    int col = ws.ws_col;
    std::cout << "\033[" << rows - 1 << ";0H"
              << "\033[K" << std::flush;
}

void Terminal::eraseErrorBar()                  //erasing the error bar
{
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
    int rows = ws.ws_row;
    int col = ws.ws_col;
    std::cout << "\033[" << rows << ";0H"
              << "\033[K" << std::flush;
}

// set non-canon parameters
int Terminal::switchToNormalMode()          //switching to normal mode
{  
    std::cout << "\033[?1049h";         //New Screen
    std::cout << "\033[0;0H";           //Move cursor to start
    if (tcgetattr(0, &orig_term_state)) //get terminal state
    {
        perror("error getting terminal settings");
        return 3;
    }
    curr_term_state = orig_term_state;
    curr_term_state.c_lflag &= ~ICANON;
    curr_term_state.c_lflag &= ~ECHO;
    curr_term_state.c_cc[VMIN] = 1;
    curr_term_state.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &curr_term_state)) //set terminal state
    {
        perror("error setting terminal settings");
        return 3;
    }
    this->mode = Mode::NORMAL;
    return 0;
}

int Terminal::switchToCommandMode()             //switch to command mode(linux settings by default)
{
    curr_term_state = orig_term_state;

    if (tcsetattr(0, TCSANOW, &curr_term_state)) //set terminal state
    {
        perror("error setting terminal settings");
        return 3;
    }
    this->mode = Mode::COMMAND;
    return 0;
}

void Terminal::disableCursor()
{
    std::cout << "\e[?25l" << std::flush;
}
void Terminal::enableCursor()
{
    std::cout << "\e[?25h" << std::flush;
}