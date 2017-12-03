#include <string>
#ifndef __SAMPLE__
#define __SAMPLE__

class Sample
{
 public:
  Sample();
  ~Sample();
  void setLength();
  void setBuffer(Uint8* new_buffer);
  Uint32 getLength();
  Uint8* getBuffer();

 private:
  Uint32 length;
  Uint8*  buffer;
};

