using namespace std;
#include <stdio.h>


#ifndef __USERINTERFACE____
#define __USERINTERFACE____

class UserInterface
{
  UserInterface();
  virtual void handle_key(int menu,int submenu);
  virtual void display_board(int menu,int submenu);  
};

#endif
