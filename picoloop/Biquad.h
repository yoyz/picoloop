//
//  Biquad.h
//
//  Created by Nigel Redmon on 11/24/12
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//
//  For a complete explanation of the Biquad code:
//  http://www.earlevel.com/main/2012/11/25/biquad-c-source-code/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code
//  for your own purposes, free or commercial.
//


#include "Oscillator.h"

#ifdef   __SDL12__               
#include <SDL/SDL.h>             
#endif // __SDL12__              
#ifdef  __SDL20__                
#include <SDL2/SDL.h>            
#endif // __SDL20__              
//#include "SDL/SDL.h"

#include <stdint.h>
#include <stdio.h>
#ifndef   M_PI
#define   M_PI      3.1415926535897932384626433832795
#endif
//#define    DECAL 16777216
//#define      DECAL 2147483648
/* #define      SHIFT 15 */
/* #define      DECAL 32768 */

               

#ifndef Biquad_h
#define Biquad_h

#define      BQ_SHIFT 16
#define      BQ_DECAL 65536


enum {
    bq_type_lowpass = 0,
    bq_type_highpass,
    bq_type_bandpass,
    bq_type_notch,
    bq_type_peak,
    bq_type_lowshelf,
    bq_type_highshelf
};

class Biquad  {
public:
    Biquad();

    Biquad(int type, float Fc, float Q, float peakGainDB);
    ~Biquad();
    void reset();
    void setType(int type);
    float getFc();
    float getQ();
    void setQ(float Q);
    void setFc(float Fc);
    void setPeakGain(float peakGainDB);
    void setBiquad(int type, float Fc, float Q, float peakGain);

    float   process_one_sample(float in);
    int32_t process_one_sample(int32_t in);

    float   process_one_sample_f(float in);
    int32_t process_one_sample_i(int32_t in);


    void   process_samples(float   * in, int nbsamples);
    void   process_samples(int32_t * in, int nbsamples);

    Sint16  tick();
    void    setInput(Oscillator * vcoosc);
    void calcBiquad(void);
    
protected:


    int type;

    float a0, a1, a2, b1, b2;
    float Fc, Q, peakGain;
    float base_Fc, base_Q, base_peakGain;
    float z1, z2;

    int64_t   i_a0, i_a1, i_a2, i_b1, i_b2;
    int64_t   i_Fc, i_Q,  i_peakGain;
    int64_t   i_z1, i_z2;

};



#endif // Biquad_h
