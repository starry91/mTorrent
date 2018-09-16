//Name: Praveen Balireddy
//Roll: 2018201052

#include "page.h"
#include <dirent.h>
#include <pwd.h>
#include "file.h"
#include <unistd.h>
#include <string>
#include <memory>
#include "path.h"
#include <sys/wait.h>
#include "error.h"
#include <syslog.h>
#include <fcntl.h>

Page::Page(std::string path = NULL) //constructor
{
    syslog(0, "Page(): path: %s", path.c_str());
    if (path.empty())
        path = get_current_dir_name();
    DIR *dp;
    dp = opendir(path.c_str());
    if (dp == NULL)
    {
        throw Error("Invalid Args: " + Path::getInstance().getAppAbsPath(path) + " does not exist");
    }
    struct dirent *entry;
    while ((entry = readdir(dp)))
    {
        try
        {
            this->files.push_back(std::make_shared<File>(File(path, std::string(entry->d_name))));
        }
        catch (Error e)
        {
        }
    }
    this->highlight_index = 0;
    this->cwd = std::string(path);
}

Page::Page(std::vector<std::string> files) //constructor for search page
{
    for (auto it = files.begin(); it != files.end(); it++)
    {
        try
        {
            this->files.push_back(std::make_shared<File>(*it, 0));
        }
        catch (Error e)
        {
        }
    }
    this->highlight_index = 0;
    this->cwd = "search";
}

void Page::scrollDown() //scrolling down
{
    if (this->highlight_index < this->files.size() - 1)
        this->highlight_index += 1;
}

void Page::scrollUp() //scrolling up
{
    if (this->highlight_index >= 1)
        this->highlight_index -= 1;
}

page_Sptr Page::enterDir() //entering dir or opening file using xdg-open
{
    auto file = this->files[this->highlight_index];
    auto path_obj = Path::getInstance();
    std::string search_path = path_obj.getSystemAbsPath(file->getFileName());
    // try
    // {
    if (this->cwd == "search") //when in search mode
    {
        if (File(search_path).getEffFileType() == 'd')
        {
            return std::make_shared<Page>(Page((char *)search_path.c_str()));
        }
        else
        {
            int pid = fork();
            if (pid == 0)
            { //child
                int devNull = open("/dev/null", 0);
                dup2(devNull, STDOUT_FILENO);
                dup2(devNull, STDERR_FILENO);
                execl("/usr/bin/xdg-open", "xdg-open", search_path.c_str(), NULL);
            }
            else
            {
                int status;
                int err = waitpid(-1, &status, WUNTRACED);
                //syslog(0, "Exit status: %u", WEXITSTATUS(status));
                if (WEXITSTATUS(status) != 0)
                {
                    throw Error("No application is registered as handling this file");
                }
            }
        }
    }
    else if (file->getEffFileType() == 'd') //when in normal mode
    {
        std::string path;
        if (file->getFileName() == "..")
            path = path_obj.getParentDir(this->cwd);
        else if (file->getFileName() == ".")
            path = this->cwd;
        else
            path = this->cwd + "/" + file->getFileName();
        //syslog(0, "enterDir: path: %s", path.c_str());
        if (path.length() >= path_obj.getHomePath().length())
        {
            return std::make_shared<Page>(Page((char *)path.c_str()));
        }
    }
    else
    {
        int pid = fork();
        if (pid == 0)
        { //child
            int devNull = open("/dev/null", 0);
            dup2(devNull, STDOUT_FILENO);
            dup2(devNull, STDERR_FILENO);
            std::string path = this->cwd + "/" + file->getFileName();
            execl("/usr/bin/xdg-open", "xdg-open", path.c_str(), NULL);
        }
        else
        {
            int status;
            int err = waitpid(-1, &status, WUNTRACED);
            //syslog(0, "Exit status: %u", WEXITSTATUS(status));
            if (WEXITSTATUS(status) != 0)
            {
                throw Error("No application is registered as handling this file");
            }
        }
    }
    return NULL;
}

page_Sptr Page::gotoParent() //goto parent
{
    auto path_obj = Path::getInstance();
    std::string path = path_obj.getParentDir(this->cwd);
    if (path.length() >= path_obj.getHomePath().length())
        return std::make_shared<Page>(Page((char *)path.c_str()));
    return NULL;
}

page_Sptr Page::gotoHome(std::string path) //goto HOME
{
    auto path_obj = Path::getInstance();
    return std::make_shared<Page>(Page((char *)path_obj.getHomePath().c_str()));
}