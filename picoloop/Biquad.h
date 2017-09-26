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

#define      SHIFT 16
#define      DECAL 65536
               

#ifndef Biquad_h
#define Biquad_h

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
    int16_t process_one_sample(int16_t in);
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


inline float Biquad::process_one_sample(float in) {
    float out = in * a0 + z1;
    z1 = in * a1 + z2 - b1 * out;
    z2 = in * a2 - b2 * out;
    return out;
}


inline int16_t Biquad::process_one_sample(int16_t in) 
//Sint16 Biquad::tick(Sint16 in) 
{
  int64_t i_in=0;
  int64_t i_out=0;
  int64_t i_out_tmp=0;
  int16_t out=0;
  

  //if (in==0) return 0;

  i_in=in;
  i_in=i_in<<SHIFT;
  
  /*
  Biquads come in several forms. 
  The most obvious, a direct implementation of the 
  second order difference equation 
  (y[n] = a0*x[n] + a1*x[n-1] + a2*x[n-2] - b1*y[n-1] - b2*y[n-2]) 
  called direct form I:

  from http://www.earlevel.com/main/2003/02/28/biquads/
   */

  // SIMPLE FORM
  /*
  i_out=(( i_a0*i_in )>>SHIFT ) + (( i_a1*i_z1 ) >>SHIFT)+ (( i_a2*i_z2 )>>SHIFT)- (( i_b1*i_z1) >> SHIFT) -(( i_b2*i_z2) >> SHIFT);
  i_z1=i_z2;
  i_in=i_z1;
  */

  // SECOND FORM
  i_out = (( i_in * i_a0 ) >> SHIFT ) + i_z1;

  i_z1  = (( i_in * i_a1 ) >> SHIFT ) + i_z2 - (( i_b1 * i_out ) >> SHIFT );
  i_z2  = (( i_in * i_a2 ) >> SHIFT ) -        (( i_b2 * i_out ) >> SHIFT );


  //  i_z1  = i_in / i_a1   + i_z2    - i_b1 / i_out;
  //  i_z2  = i_in / i_a2             - i_b2 / i_out;

  //  i_z1 = ( ( i_in / i_a1 ) << SHIFT ) + i_z2 - ( ( i_b1 / i_out_tmp ) << SHIFT);
  //  i_z2 = ( ( i_in / i_a2 ) << SHIFT )        - ( ( i_b2 / i_out_tmp ) << SHIFT); 
  

  
  //  printf("i_in:%d i_out:%d\n",i_in,i_out);
  //i_out=i_out;
  i_out=i_out >> SHIFT;
  out=i_out;


  return out;
}


#endif // Biquad_h
