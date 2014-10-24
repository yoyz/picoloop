#include "WaveTableManager.h"
#include "WaveTable.h"
#include "Generator.h"
#include "Master.h"
#include <stdio.h>


void t1()
{
  WaveTableManager & WTM = WaveTableManager::getInstance();
  WaveTable* WT = new WaveTable();
  printf("WT:0x%08.8X\n",WT);
  printf("%d\n",WTM.getSize());
  //WTM.addWaveTable(WT);
  WTM.insert(WT,2);
  printf("WT:0x%08.8X\n",WTM.get(0));
  printf("WT:0x%08.8X\n",WTM.get(1));
  printf("WT:0x%08.8X\n",WTM.get(2));
  //WTM.setSize(10);

}

void t2()
{
  WaveTableManager & WTM = WaveTableManager::getInstance();
  printf("%d\n",WTM.getSize());
  //WTM.setSize(20);
}

void wt_test()
{
  Sint16 * t;
  WaveTable WT;

  WT.setSize(128);
  WT.setSize(0);
  WT.setSize(128);
  WT.setSize(0);
  WT.setSize(128);

  t=WT.getBuffer();
  t[64]=1;
  WT.setSize(0);
  WT.setSize(128);
  t[64]=1;
  printf("t[64]=%d\n",t[64]);
}


void wtg()
{
  Generator G;
  WaveTableManager & WTM = WaveTableManager::getInstance();
  WaveTable* WT;


  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.noise();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  WTM.insert(WT,PICO_WAVETABLE_NOISE);

  

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.sine();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  WTM.insert(WT,PICO_WAVETABLE_SINE);

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.saw();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  WTM.insert(WT,PICO_WAVETABLE_SAW);

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.pulse();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  WTM.insert(WT,PICO_WAVETABLE_PULSE);

  WT = new WaveTable();
  WT->setSize(WAVETABLE_SIZE);
  G.triangle();
  memcpy(WT->getBuffer(),G.getBuffer(),WAVETABLE_SIZE);
  WTM.insert(WT,PICO_WAVETABLE_TRGL);



  printf("wavetablemanager.getSize : %d\n",WTM.getSize());

}

int main()
{
  //t1();
  //t2();
  //wt_test();
  wtg();
  return(0);
}
