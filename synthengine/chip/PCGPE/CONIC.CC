
           ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
           ³ A General Conics Sections Scan Line Algorithm ³
           ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ


The following code is the complete algorithm for the general conic
drawer as mentioned in Foley/VanDam. It is included here with the
permission of Andrew W. Fitzgibbon, who derived the remaining code
sections not included in the book.


//
// CONIC  2D Bresenham-like conic drawer.
//       CONIC(Sx,Sy, Ex,Ey, A,B,C,D,E,F) draws the conic specified
//       by A x^2 + B x y + C y^2 + D x + E y + F = 0, between the
//       start point (Sx, Sy) and endpoint (Ex,Ey).

// Author: Andrew W. Fitzgibbon (andrewfg@ed.ac.uk),
//         Machine Vision Unit,
//         Dept. of Artificial Intelligence,
//         Edinburgh University,
//         
// Date: 31-Mar-94

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int DIAGx[] = {999, 1,  1, -1, -1, -1, -1,  1,  1};
static int DIAGy[] = {999, 1,  1,  1,  1, -1, -1, -1, -1};
static int SIDEx[] = {999, 1,  0,  0, -1, -1,  0,  0,  1};
static int SIDEy[] = {999, 0,  1,  1,  0,  0, -1, -1,  0};
static int BSIGNS[] = {99, 1,  1, -1, -1,  1,  1, -1, -1};

int   debugging = 1;

struct ConicPlotter {
  virtual void plot(int x, int y);
};

struct DebugPlotter : public ConicPlotter {
  int xs;
  int ys;
  int xe;
  int ye;
  int A;
  int B;
  int C;
  int D;
  int E;
  int F;      

  int octant;
  int d;

  void plot(int x, int y);
};

void DebugPlotter::plot(int x, int y)
{
  printf("%3d %3d\n",x,y);

  if (debugging) {
    // Translate start point to origin...
    float tF = A*xs*xs + B*xs*ys + C*ys*ys + D*xs + E*ys + F;
    float tD = D + 2 * A * xs + B * ys;
    float tE = E + B * xs + 2 * C * ys;
  
    float tx = x - xs + ((float)DIAGx[octant] + SIDEx[octant])/2;
    float ty = y - ys + ((float)DIAGy[octant] + SIDEy[octant])/2;
    // Calculate F
    
    float td = 4*(A*tx*tx + B*tx*ty + C*ty*ty + tD*tx + tE*ty + tF);
    
    fprintf(stderr,"O%d ", octant);
    if (d<0)
      fprintf(stderr," Inside "); 
    else 
      fprintf(stderr,"Outside "); 
    float err = td - d;
    fprintf(stderr,"Real(%5.1f,%5.1f) = %8.2f Recurred = %8.2f err = %g\n", 
            tx, ty, td/4, d/4.0f, err);
    if (fabs(err) > 1e-14)
      abort();
  }
  
}

inline int odd(int n)
{
  return n&1;
}

inline int abs(int a)
{
  if (a > 0)
    return a;
  else
    return -a;
}
    
int getoctant(int gx, int gy)
{
  // Use gradient to identify octant.
  int upper = abs(gx)>abs(gy);
  if (gx>=0)                            // Right-pointing
    if (gy>=0)                          //    Up
      return 4 - upper;
    else                                //    Down
      return 1 + upper;
  else                                  // Left
    if (gy>0)                           //    Up
      return 5 + upper;
    else                                //    Down
      return 8 - upper;
}

int conic(int xs, int ys, int xe, int ye,
          int A, int B, int C, int D, int E, int F,
          ConicPlotter * plotterdata)
{
  A *= 4;
  B *= 4;
  C *= 4;
  D *= 4;
  E *= 4;
  F *= 4;
  
  // Translate start point to origin...
  F = A*xs*xs + B*xs*ys + C*ys*ys + D*xs + E*ys + F;
  D = D + 2 * A * xs + B * ys;
  E = E + B * xs + 2 * C * ys;
  
  // Work out starting octant
  int octant = getoctant(D,E);
  
  int dxS = SIDEx[octant]; 
  int dyS = SIDEy[octant]; 
  int dxD = DIAGx[octant];
  int dyD = DIAGy[octant];

  int bsign = BSIGNS[octant];
  int d,u,v;
  switch (octant) {
  case 1:
    d = A + B/2 + C/4 + D + E/2 + F;
    u = A + B/2 + D;
    v = u + E;
    break;
  case 2:
    d = A/4 + B/2 + C + D/2 + E + F;
    u = B/2 + C + E;
    v = u + D;
    break;
  case 3:
    d = A/4 - B/2 + C - D/2 + E + F;
    u = -B/2 + C + E;
    v = u - D;
    break;
  case 4:
    d = A - B/2 + C/4 - D + E/2 + F;
    u = A - B/2 - D;
    v = u + E;
    break;
  case 5:
    d = A + B/2 + C/4 - D - E/2 + F;
    u = A + B/2 - D;
    v = u - E;
    break;
  case 6:
    d = A/4 + B/2 + C - D/2 - E + F;
    u = B/2 + C - E;
    v = u - D;
    break;
  case 7:
    d = A/4 - B/2 + C + D/2 - E + F;
    u =  -B/2 + C - E;
    v = u + D;
    break;
  case 8:
    d = A - B/2 + C/4 + D - E/2 + F;
    u = A - B/2 + D;
    v = u - E;
    break;
  default:
    fprintf(stderr,"FUNNY OCTANT\n");
    abort();
  }
  
  int k1sign = dyS*dyD;
  int k1 = 2 * (A + k1sign * (C - A));
  int Bsign = dxD*dyD;
  int k2 = k1 + Bsign * B;
  int k3 = 2 * (A + C + Bsign * B);

  // Work out gradient at endpoint
  int gxe = xe - xs;
  int gye = ye - ys;
  int gx = 2*A*gxe +   B*gye + D;
  int gy =   B*gxe + 2*C*gye + E;
  
  int octantcount = getoctant(gx,gy) - octant;
  if (octantcount <= 0)
    octantcount = octantcount + 8;
  fprintf(stderr,"octantcount = %d\n", octantcount);
  
  int x = xs;
  int y = ys;
  
  while (octantcount > 0) {
    if (debugging)
      fprintf(stderr,"-- %d -------------------------\n", octant); 
    
    if (odd(octant)) {
      while (2*v <= k2) {
        // Plot this point
        ((DebugPlotter*)plotterdata)->octant = octant;
        ((DebugPlotter*)plotterdata)->d = d;
        plotterdata->plot(x,y);
        
        // Are we inside or outside?
        if (d < 0) {                    // Inside
          x = x + dxS;
          y = y + dyS;
          u = u + k1;
          v = v + k2;
          d = d + u;
        }
        else {                          // outside
          x = x + dxD;
          y = y + dyD;
          u = u + k2;
          v = v + k3;
          d = d + v;
        }
      }
    
      d = d - u + v/2 - k2/2 + 3*k3/8; 
      // error (^) in Foley and van Dam p 959, "2nd ed, revised 5th printing"
      u = -u + v - k2/2 + k3/2;
      v = v - k2 + k3/2;
      k1 = k1 - 2*k2 + k3;
      k2 = k3 - k2;
      int tmp = dxS; dxS = -dyS; dyS = tmp;
    }
    else {                              // Octant is even
      while (2*u < k2) {
        // Plot this point
        ((DebugPlotter*)plotterdata)->octant = octant;
        ((DebugPlotter*)plotterdata)->d = d;
        plotterdata->plot(x,y);
        
        // Are we inside or outside?
        if (d > 0) {                    // Outside
          x = x + dxS;
          y = y + dyS;
          u = u + k1;
          v = v + k2;
          d = d + u;
        }
        else {                          // Inside
          x = x + dxD;
          y = y + dyD;
          u = u + k2;
          v = v + k3;
          d = d + v;
        }
      }
      int tmpdk = k1 - k2;
      d = d + u - v + tmpdk;
      v = 2*u - v + tmpdk;
      u = u + tmpdk;
      k3 = k3 + 4*tmpdk;
      k2 = k1 + tmpdk;
      
      int tmp = dxD; dxD = -dyD; dyD = tmp;
    }
    
    octant = (octant&7)+1;
    octantcount--;
  }

  // Draw final octant until we reach the endpoint
  if (debugging)
    fprintf(stderr,"-- %d (final) -----------------\n", octant); 
    
  if (odd(octant)) {
    while (2*v <= k2 && x != xe && y != ye) {
      // Plot this point
      ((DebugPlotter*)plotterdata)->octant = octant;
      ((DebugPlotter*)plotterdata)->d = d;
      plotterdata->plot(x,y);
      
      // Are we inside or outside?
      if (d < 0) {                      // Inside
        x = x + dxS;
        y = y + dyS;
        u = u + k1;
        v = v + k2;
        d = d + u;
      }
      else {                            // outside
        x = x + dxD;
        y = y + dyD;
        u = u + k2;
        v = v + k3;
        d = d + v;
      }
    }
  }
  else {                        // Octant is even
    while ((2*u < k2) && (x != xe) && (y != ye)) {
      // Plot this point
      ((DebugPlotter*)plotterdata)->octant = octant;
      ((DebugPlotter*)plotterdata)->d = d;
      plotterdata->plot(x,y);
      
      // Are we inside or outside?
      if (d > 0) {                      // Outside
        x = x + dxS;
        y = y + dyS;
        u = u + k1;
        v = v + k2;
        d = d + u;
      }
      else {                            // Inside
        x = x + dxD;
        y = y + dyD;
        u = u + k2;
        v = v + k3;
        d = d + v;
      }
    }
  }



  return 1;
}

main(int argc, char ** argv)
{
  DebugPlotter db;
  db.xs = -7;
  db.ys = -19;
  db.xe = -8;
  db.ye = -8;
  db.A = 1424;
  db.B = -964;
  db.C = 276;
  db.D = 0;
  db.E = 0;
  db.F = -40000;
  conic(db.xs,db.ys,db.xe,db.ye,db.A,db.B,db.C,db.D,db.E,db.F, &db);
}
