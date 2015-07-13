#include "EffectDisabled.h"
#include "Master.h"

#define FX_SIZE DEFAULTFREQ*8


EffectDisabled::EffectDisabled()
{
  DPRINTF("EffectDisabled::EffectDisabled()\n");
}

EffectDisabled::~EffectDisabled()
{
}

void EffectDisabled::init()
{
  DPRINTF("EffectDisabled::init()\n");
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
