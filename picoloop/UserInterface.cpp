#include "UserInterface.h"

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
}


void UserInterface::handle_key(int menu)
{
  DPRINTF("UserInterface::handle_key(%d,)\n",menu);
}

void UserInterface::display_board(int menu)
{
  DPRINTF("UserInterface::display_board(%d)\n",menu);
}

void UserInterface::display_board_text()
{
  int a=0;
  DPRINTF("UserInterface::display_board_text() %d\n",a);
}

