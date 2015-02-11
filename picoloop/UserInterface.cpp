#include "UserInterface.h"

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
}


void UserInterface::handle_key(int menu)
{
  printf("UserInterface::handle_key(%d,)\n",menu);
}

void UserInterface::display_board(int menu)
{
  printf("UserInterface::display_board(%d)\n",menu);
}
