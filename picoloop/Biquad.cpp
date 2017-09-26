//
//  Biquad.cpp
//
//  Created by Nigel Redmon on 11/24/12
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//
//  For a complete explanation of the Biquad code:
//  http://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code
//  for your own purposes, free or commercial.
//

#include <math.h>
#include "Biquad.h"

Biquad::Biquad() {
    type = bq_type_lowpass;
    a0 = 1.0;
    a1 = a2 = b1 = b2 = 0.0;
    Fc = 0.50;
    Q = 0.707;
    peakGain = 0.0;
    z1 = z2 = 0.0;
}



Biquad::Biquad(int type, float Fc, float Q, float peakGainDB) {

    setBiquad(type, Fc, Q, peakGainDB);
    z1 = z2 = 0.0;
}

Biquad::~Biquad() {
}



void Biquad::reset()
{
  //type = bq_type_lowpass;
  type = bq_type_lowpass;
    a0 = 1.0;
    a1 = a2 = b1 = b2 = 0.0;
    Fc = 0.50;
    Q = 0.707;
    peakGain = 0.0;
    z1 = z2 = 0.0;
}


void Biquad::setType(int type) {
    this->type = type;
    calcBiquad();
}



float Biquad::getQ() {
    return this->base_Q;
}


float Biquad::getFc() {
  return this->base_Fc;
}


void Biquad::setQ(float Q) {
    this->Q = Q;
    //calcBiquad();
}


void Biquad::setFc(float Fc) {
    this->Fc = Fc;
    //calcBiquad();
}


void Biquad::setPeakGain(float peakGainDB) {
    this->peakGain = peakGainDB;
    //calcBiquad();
}
    

void Biquad::setBiquad(int type, float Fc, float Q, float peakGainDB) {
  //this->type = type;
  //this->type = bq_type_lowpass;
    this->type = type;
    this->Q = Q;
    this->base_Q=Q;
    this->Fc = Fc;
    this->base_Fc = Fc;
    setPeakGain(peakGainDB);
    //this.calcBiquad();
}

void Biquad::calcBiquad(void) {

    float norm;
    float V = pow(10, fabs(peakGain) / 20.0);
    float K = tan(M_PI * Fc);
    switch (this->type) {
        case bq_type_lowpass:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K * K * norm;
            a1 = 2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case bq_type_highpass:
            norm = 1 / (1 + K / Q + K * K);
            a0 = 1 * norm;
            a1 = -2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case bq_type_bandpass:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K / Q * norm;
            a1 = 0;
            a2 = -a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case bq_type_notch:
            norm = 1 / (1 + K / Q + K * K);
            a0 = (1 + K * K) * norm;
            a1 = 2 * (K * K - 1) * norm;
            a2 = a0;
            b1 = a1;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case bq_type_peak:
            if (peakGain >= 0) {    // boost
                norm = 1 / (1 + 1/Q * K + K * K);
                a0 = (1 + V/Q * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - V/Q * K + K * K) * norm;
                b1 = a1;
                b2 = (1 - 1/Q * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (1 + V/Q * K + K * K);
                a0 = (1 + 1/Q * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - 1/Q * K + K * K) * norm;
                b1 = a1;
                b2 = (1 - V/Q * K + K * K) * norm;
            }
            break;
        case bq_type_lowshelf:
            if (peakGain >= 0) {    // boost
                norm = 1 / (1 + sqrt(2) * K + K * K);
                a0 = (1 + sqrt(2*V) * K + V * K * K) * norm;
                a1 = 2 * (V * K * K - 1) * norm;
                a2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
                b1 = 2 * (K * K - 1) * norm;
                b2 = (1 - sqrt(2) * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (1 + sqrt(2*V) * K + V * K * K);
                a0 = (1 + sqrt(2) * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - sqrt(2) * K + K * K) * norm;
                b1 = 2 * (V * K * K - 1) * norm;
                b2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
            }
            break;
        case bq_type_highshelf:
            if (peakGain >= 0) {    // boost
                norm = 1 / (1 + sqrt(2) * K + K * K);
                a0 = (V + sqrt(2*V) * K + K * K) * norm;
                a1 = 2 * (K * K - V) * norm;
                a2 = (V - sqrt(2*V) * K + K * K) * norm;
                b1 = 2 * (K * K - 1) * norm;
                b2 = (1 - sqrt(2) * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (V + sqrt(2*V) * K + K * K);
                a0 = (1 + sqrt(2) * K + K * K) * norm;
                a1 = 2 * (K * K - 1) * norm;
                a2 = (1 - sqrt(2) * K + K * K) * norm;
                b1 = 2 * (K * K - V) * norm;
                b2 = (V - sqrt(2*V) * K + K * K) * norm;
            }
            break;
    }
    

    i_a0=a0             * BQ_DECAL;
    i_a1=a1             * BQ_DECAL;
    i_a2=a2             * BQ_DECAL;

    i_b1=b1             * BQ_DECAL;
    i_b2=b2             * BQ_DECAL;

    i_z1=z1             * BQ_DECAL;
    i_z2=z2             * BQ_DECAL;

    i_Fc=Fc             * BQ_DECAL;
    i_Q=Q               * BQ_DECAL;
    i_peakGain=peakGain * BQ_DECAL;



    //printf("<<< %f %f %f    %f %f    %f %f  %f %f %f>>>\n",a0,a1,a2,b1,b2,z1,z2,Fc,Q,peakGain);
    //    printf("[[[ %d %d %d    %d %d    %d %d  %d %d %d]]]\n",i_a0,i_a1,i_a2,i_b1,i_b2,i_z1,i_z2,i_Fc,i_Q,i_peakGain);


    return;
}

float Biquad::process_one_sample(float in) {
    float out = in * a0 + z1;
    z1 = in * a1 + z2 - b1 * out;
    z2 = in * a2 - b2 * out;
    return out;
}


int32_t Biquad::process_one_sample(int32_t in) 
//Sint16 Biquad::tick(Sint16 in) 
{

  int64_t i_in=0;
  int64_t i_out=0;
  int64_t i_out_tmp=0;
  //int16_t out=0;
  int32_t out;
  

  //if (in==0) return 0;

  i_in=in;
  i_in=i_in<<BQ_SHIFT;
  
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
  i_out = (( i_in * i_a0 ) >> BQ_SHIFT ) + i_z1;

  i_z1  = (( i_in * i_a1 ) >> BQ_SHIFT ) + i_z2 - (( i_b1 * i_out ) >> BQ_SHIFT );
  i_z2  = (( i_in * i_a2 ) >> BQ_SHIFT ) -        (( i_b2 * i_out ) >> BQ_SHIFT );


  //  i_z1  = i_in / i_a1   + i_z2    - i_b1 / i_out;
  //  i_z2  = i_in / i_a2             - i_b2 / i_out;

  //  i_z1 = ( ( i_in / i_a1 ) << SHIFT ) + i_z2 - ( ( i_b1 / i_out_tmp ) << SHIFT);
  //  i_z2 = ( ( i_in / i_a2 ) << SHIFT )        - ( ( i_b2 / i_out_tmp ) << SHIFT); 
  

  
  //  printf("i_in:%d i_out:%d\n",i_in,i_out);
  //i_out=i_out;
  i_out=i_out >> BQ_SHIFT;
  out=i_out;

  //printf("X(%d)=%d ",in,out);
  return out;
}

float Biquad::process_one_sample_f(float in)
{
  return this->process_one_sample(in);
}

int32_t Biquad::process_one_sample_i(int32_t in)
{
  return this->process_one_sample(in);
}

void Biquad::process_samples(float   * in, int nbsamples)
{
  //printf("blo");
  int i;
  for (i=0;i<nbsamples;i++)
    in[i]=this->process_one_sample_f(in[i]);
}

void Biquad::process_samples(int32_t   * in, int nbsamples)
{
  //printf("bla:%d",nbsamples);
  //printf("i:%d",in[0]);
  int i;
  for (i=0;i<nbsamples;i++)
    in[i]=process_one_sample_i(in[i]);
  //printf("o:%d",in[0]);
}

