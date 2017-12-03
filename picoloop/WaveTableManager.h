#include <vector>
#include "WaveTable.h"
#include "Master.h"

#ifndef __WAVETABLEMANAGER__
#define __WAVETABLEMANAGER__

class WaveTableManager 
{
private:

  //Constructor Destructor
  WaveTableManager();
  ~WaveTableManager();

  //WaveTableManager & operator= (const WaveTableManager&) {}
  //WaveTableManager(const WaveTableManager&)              {}

  std::vector<WaveTable*> wtvector;  

  int    size;


public:
  static WaveTableManager& getInstance();



  //void addWaveTable(WaveTable * WT);


  int         getSize();
  void        insert(WaveTable * WT,int position);
  WaveTable * get(int position);


};
#endif
