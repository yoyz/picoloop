#include "config.h"

Bit32s CPU_Cycles = 0;
Bit32s CPU_CycleLeft = 3000;
Bit32s CPU_CycleMax = 3000;
Bit32s CPU_OldCycleMax = 3000;
Bit32s CPU_CyclePercUsed = 100;
Bit32s CPU_CycleLimit = -1;
Bit32s CPU_CycleUp = 0;
Bit32s CPU_CycleDown = 0;
Bit64s CPU_IODelayRemoved = 0;
Bitu PIC_IRQCheck;
Bitu PIC_IRQActive;
Bitu PIC_Ticks;
