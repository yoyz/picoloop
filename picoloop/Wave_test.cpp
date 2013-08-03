#include "Wave.h"
#include "AudioEngine.h"
#include "SineOscillator.h"
#include <stdio.h>
#include <math.h>
Wave cowbell;
AudioEngine ae;
SineOscillator s1;
SineOscillator s2;
SineOscillator s3;




void sine_1(int sec)
{
  int i,size;
  size = 44100*sec;
  Sint16 buffer [size]; //buffer array
  s1.setFreq(110);
  s2.setFreq(440);
  s3.setFreq(1760);

  printf("Generate sine 1\n");

  for(i = 0; i < size; i++)
    {
      s1.setAmplitude(127);
      s2.setAmplitude(127);
      s3.setAmplitude(64);
      buffer[i]=s1.tick()-s2.tick()-s3.tick();
      buffer[i]=s1.tick()+s2.tick();
      //      buffer[i]=s1.tick();
    }

  FILE* fd=fopen("sine2","w+");
  
  for (int i=0; i< size ; i++)
    {      
      Sint16 d=buffer[i];
      fwrite(&d,sizeof(unsigned Sint16),1,fd);
    }

  fclose(fd);
  printf("closing sine 1\n");


}


void sine_2(int sec)
{
  int number = 0;
  int i, size;
  double const Pi=4*atan(1); 
  printf("Pi:%f\n",Pi);
  //cout << "Enter number of seconds:" << endl;
  //scanf("%d", &number);
  size = 44100*sec;
  Sint16 buffer [size]; //buffer array
  
  printf("Generate sine 2\n");
  for(i = 0; i < size; i++)
    {
      //buffer[i] = (Sint16)(float)sin((2*Pi*440)/(44100*i))*4096;
      float  f;
      Uint16 d;
      //d=sin((2*Pi*440)/(44100*i))*20000;
      //d=sin((2*Pi*440)/(44100*i))*4096;
      //d=sin(1);
      //f=(sin((i/3.14)/50)*30000);
      //f=sin(2*3.14*440)/(44000*ei)*127; # Don't work

      /*


gnuplot> plot [0:880] sin((x/3.14159)/22)*10000


  10000 ++-----***-----+-------+------+-------+-****--+------+-------+----++
        +    ** + **   +       +      +   sin((x/3.14159)/22)*10000 ****** |
   8000 ++   *      *                         *      *                    ++
        |   *       **                       *       *                     |
   6000 ++ *          *                      *        *                   ++
        |  *          *                     *          *                   |
   4000 ++*            *                   *           *                  ++
        |*             *                   *            *                  |
   2000 +*              *                 *              *                +*
      0 *+              *                *               *                *+
        |                *               *                *               *|
  -2000 ++                *             *                  *              *+
        |                 *             *                  *             * |
  -4000 ++                *            *                   *            * ++
        |                  *          *                     *           *  |
  -6000 ++                  *         *                      *         *  ++
        |                   *        *                       *        *    |
  -8000 ++                   **     *                         **     **   ++
        +       +      +      **   ** +       +       +      + *    *+     |
 -10000 ++------+------+-------+***---+-------+-------+------+--*****+----++
        0      100    200     300    400     500     600    700     800

       */


      //f=(sin(i/3.14/8)*127000);
      //      f=(sin((i/3.14)/16)*100*127));
      f=(sin((i/3.14159)/22)*10000);

      d=(Uint16)f;
      //printf("%f\t%d\n",f,d);
      buffer[i]=d;
      //printf("%d\n",buffer[i]);
    }

  printf("write sine file\n");
  FILE* fd=fopen("sine","w+");
  
  for (int i=0; i< size ; i++)
    {      
      Sint16 d=buffer[i];
      fwrite(&d,sizeof(unsigned Sint16),1,fd);
    }

  fclose(fd);
  printf("closing sine 2\n");



}

void cow()
{
  Uint32 length;
  Uint8* buf;


  cowbell.loadWave("DT-42-0.1.0/808-cowbell.wav");

  length=cowbell.getLength();
  printf("length:%d\n",length);


  FILE* fd=fopen("toto","w+");
  
  for (int i=0; i< length ; i++)
    {
      
      fwrite(cowbell.getBuffer(i),sizeof(Uint8),1,fd);
    }

  fclose(fd);


}


int main()
{
  ae.openAudio();
  cow();
  sine_2(4);

  sine_1(4);

}
