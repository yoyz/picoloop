using namespace std;

#include <string>
#include <iostream>
#include <sstream>
#include "PatternElement.h"

PatternElement::PatternElement()
{
  Note=0;
  Trig=false;
  Channel=0;
}

int PatternElement::getNote()
{
  return PatternElement::Note;
}
/*

float PatternElement::getNoteFreq()
{
  return note_hertz[PatternElement::Note];
}
*/
bool PatternElement::setNote(int N)
{
  Note=N;
}

bool PatternElement::setTrig(bool T)
{
  return Trig=T;
}

bool PatternElement::getTrig()
{
  return Trig;
}


string PatternElement::getStr()
{
  stringstream ss;
  string s;
  ss << "Note:" << Note << " Trig:" << Trig << " Channel:" << Channel ;
  s=ss.str();
  return s;
    //cout << "Note:" << Note << " Trig:" << Trig << " Channel:" << Channel << "\n";
}

void PatternElement::print()
{
  cout << getStr() << "\n";
}
