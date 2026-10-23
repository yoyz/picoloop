#include "../../UserInterface.h"

#ifndef __ER1USERINTERFACE____
#define __ER1USERINTERFACE____

// Defined in PatternPlayer.cpp, next to the other menu globals.
extern int menu;
extern int menu_sub;

class ER1UserInterface : public UserInterface
{
 public:
  ER1UserInterface();
  ~ER1UserInterface();

  void handle_key(int menu);
  void display_board(int menu);
  void display_board_text();

 private:
  void handle_key_env();
  void handle_key_note();
  void handle_key_osc();
  void handle_key_vco();
  void handle_key_lfo();
  void handle_key_fltr();

  void display_board_env();
  void display_board_note();
  void display_board_osc();
  void display_board_vco();
  void display_board_lfo();
  void display_board_fltr();
};

#endif
