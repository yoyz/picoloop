#include <stdio.h>
#include <math.h>
#include "string.h"





//--------------------------------------------------------------------------------------------------
//functions for conversion from float-vst-parameter to string:
/*
void bpm2String(float value, char *string)
{
 double mappedValue = value*200 + 50; //from 50 to 250 bpm
 if(value==0.0)
  sprintf(string,       //pointer where the string is to be stored
          "%s",         //format string, indicates that a string type parameter follows
          "sync");      //the parameter of type string
 else
  sprintf(string,       //pointer where the string is to be stored 
          "%.2f",       //format string, indicates that a double/float type parameter 
                        //follows and should be orunded to 2 digits
          mappedValue); //the parameter of type double/float
}

void percent2String(float value, char* string)
{
 double mappedValue = 100*value;
 sprintf(string,       //pointer where the string is to be stored 
        "%.1f %s",     //format string, indicates that the first parameter (after the
                       //format parameters) will be a double/float type parameter 
                       //and should be runded to 1 digit and the second one will be a
                       //string type parameter
        mappedValue,   //the parameter of type double/float
        "%");          //the parameter of type string (the "%" character in this case)
}

void scaler2String(float value, char* string)
{
 double mappedValue = pow(4, (2*value - 1) );   //between 4^(-1) = 0.25 and 4^1 = 4
 sprintf(string, "%s %.2f", "*", mappedValue);      
}

void numAndDen2String(float value, char* string)
{
 long mappedValue = (long) floor(127*value+1);  //from 1 to 128
 sprintf(string, "%i", mappedValue);      
}

void Freq2String(float value, char* string)
{
 double mappedValue = pow(1000,value)*20; //from 20 to 20000Hz
 sprintf(string, "%.0f", mappedValue);      
}

void timeFactor2String(float value, char* string)
{
 double mappedValue = pow(2, (2*value - 1) );   //between 2^(-1) = 0.5 and 2^1 = 2
 sprintf(string, "%.4f", mappedValue);      
}
*/

/*
void phaseString2float(char *string, float& output)
{
 output = 45.09;
}
*/


void value2String(float value, char *string)
{
 sprintf(string, "%.2f", value);     
}

void phase2String(float value, char *string)
{
 double mappedValue = 360.0*value;

 if( mappedValue <= 359.99 ) 
  sprintf(string, "%.2f", mappedValue);  
 else
  sprintf(string, "%s", "free");   
}

