#include "EffectDisabled.h"
#include "Master.h"

#define FX_SIZE DEFAULTFREQ*8


EffectDisabled::EffectDisabled()
{
  printf("EffectDisabled::EffectDisabled()\n");
}

EffectDisabled::~EffectDisabled()
{
}

void EffectDisabled::init()
{
  printf("EffectDisabled::init()\n");
}

void EffectDisabled::reset()
{
}

void EffectDisabled::setDepth(int val)
{

}

void EffectDisabled::setSpeed(int val)
{
}

int16_t EffectDisabled::process(int16_t in)
{
  return in;
}
