//Name: Praveen Balireddy
//Roll: 2018201052

#ifndef PAGE_H
#define PAGE_H

#include <vector>
#include <string>
#include "file.h"
#include <memory>

//Page class to store a single page(View) related data

class Page
{
public:
  std::vector<file_Sptr> files;
  int highlight_index;
  std::string cwd;

  Page(std::string);
  Page(std::vector<std::string>);
  void scrollDown();
  void scrollUp();
  std::shared_ptr<Page> gotoParent();
  std::shared_ptr<Page> enterDir();
  std::shared_ptr<Page> gotoHome(std::string);
  };

typedef std::shared_ptr<Page> page_Sptr;    //shared pointer to Page class

#endif