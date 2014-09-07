/*---------------------------------------------------------------------------------

	$Id: my_first.cpp,v 1.13 2008-12-02 20:21:20 dovoto Exp $

	Simple console print demo
	-- dovoto


---------------------------------------------------------------------------------*/
#include <nds.h>

#include <stdio.h>

#include <vector>

#include <iostream>

#include <math.h>

#include "nds/ndstypes.h"

// #include "knee.h"

using namespace std;

volatile int frame = 0;

void Vblank() {
//---------------------------------------------------------------------------------
	frame++;
}

void setPixel( int x, int y, int color ) {
    VRAM_A[x + y * 256] = color;
}

/*
void drawInterpolation4pq(vector<touchPosition> const &touchPositions) {
  int p1_x = touchPositions[1].px - touchPositions[0].px;
  int p1_y = touchPositions[1].py - touchPositions[0].py;
  int palpha_x = touchPositions[2].px - touchPositions[0].px;
  int palpha_y = touchPositions[2].py - touchPositions[0].py;
  int pbeta_x = touchPositions[3].px - touchPositions[0].px;
  int pbeta_y = touchPositions[3].py - touchPositions[0].py;
  int c = palpha_y * pbeta_x - palpha_x * pbeta_y;
  if (c==0)
    return;
  int d = (pbeta_y * p1_x - pbeta_x * p1_y)/c;
  if (d==0)
    return;
  int e = (palpha_y * p1_x - palpha_x * p1_y)/c;
  if (e==0)
    return;
  float ro1 = sqrt(e*(1+d-e)/d);
  float ro2 = sqrt(d*(1+d-e)/e);
  if ((e-d)==0)
    return;
  float alpha_plus = (1+ro1) / (e-d);
  float beta_plus = (1+ro2) / (e-d);
  float a1_x = (palpha_x - (pow(alpha_plus,2))*p1_x) / (alpha_plus - pow(alpha_plus,2));
  float a1_y = (palpha_y - (pow(alpha_plus,2))*p1_y) / (alpha_plus - pow(alpha_plus,2));
  float a2_x = (alpha_plus * p1_x - palpha_x) / (alpha_plus - pow(alpha_plus,2));
  float a2_y = (alpha_plus * p1_y - palpha_y) / (alpha_plus - pow(alpha_plus,2));
//  iprintf("\x1b[16;0H x = %f", beta_plus);
  // for(int i=0;(i*0.1)<beta_plus;i++) {
  for(int i=0;i<(beta_plus*100);i++) {
//    list4points[[1]] + a1 * s + a2 * s^2
    float t = i*0.01;
    int x = touchPositions[0].px + a1_x * t + a2_x * pow(t,2);
    int y = touchPositions[0].py + a1_y * t + a2_y * pow(t,2);
    iprintf("\x1b[16;0H x = %d, y = %d\n", x,y);
    setPixel(x,y,255);
  }
}
*/

void drawInterpolationLagrange(vector<touchPosition> const &touchPositions, float lambda) {
  int k = touchPositions.size()-1;
  if (touchPositions.size() % 2 == 0)
  {
    k = k-1;
  }
  for(int j=0;j<k;j = j+2) {    
    int td0 = 0;
    int n_x = touchPositions[j+1].px - touchPositions[j+0].px;
    int n_y = touchPositions[j+1].py - touchPositions[j+0].py;
    float td1 = pow(sqrt(pow(n_x,2) + pow(n_y,2)),lambda);
    n_x = touchPositions[j+2].px - touchPositions[j+1].px;
    n_y = touchPositions[j+2].py - touchPositions[j+1].py;
    float td2 = pow(sqrt(pow(n_x,2) + pow(n_y,2)),lambda) + td1;
    int q0_x = touchPositions[j+0].px;
    int q0_y = touchPositions[j+0].py;
    int q1_x = touchPositions[j+1].px;
    int q1_y = touchPositions[j+1].py;
    int q2_x = touchPositions[j+2].px;
    int q2_y = touchPositions[j+2].py;
    
    float range = td2 - td0;
    float one_step = range/1000;
      for(int i=0;i<(td2*(1/one_step));i++) {
    //    list4points[[1]] + a1 * s + a2 * s^2
        float t = i*one_step;
        int x = q0_x*(((t - td1)*(t - td2))/((td0 - td1)*(td0 - td2))) + 
         q1_x*(((t - td0)*(t - td2))/((td1 - td0)*(td1 - td2))) +
         q2_x*(((t - td0)*(t - td1))/((td2 - td1)*(td2 - td0)));

        int y = q0_y*(((t - td1)*(t - td2))/((td0 - td1)*(td0 - td2))) + 
          q1_y*(((t - td0)*(t - td2))/((td1 - td0)*(td1 - td2))) +
          q2_y*(((t - td0)*(t - td1))/((td2 - td1)*(td2 - td0)));
        // iprintf("\x1b[16;0H x = %d, y = %d\n", x,y);
        setPixel(x,y,255);
      }

  }
}

void clearFullScreen(vector<touchPosition> &touchPositions) {
  vramDefault();
  
  videoSetMode(MODE_FB0);
  vramSetBankA(VRAM_A_LCD);
  
  lcdMainOnBottom();
  
	consoleDemoInit();
	
  touchPositions.clear();
}

void clearScreenKeepingPoints(vector<touchPosition> const &touchPositions) {
  vramDefault();
  
  videoSetMode(MODE_FB0);
  vramSetBankA(VRAM_A_LCD);
  
  lcdMainOnBottom();
  
	consoleDemoInit();
  for(vector<touchPosition>::const_iterator it = touchPositions.begin(); it != touchPositions.end(); ++it) {
    setPixel(it->px, it->py, 255);
  }
}

int main(void) {
  
  touchPosition touchXY;
  touchPosition touchOld;
  
  
  float lambda = 1;
  
  videoSetMode(MODE_FB0);
  vramSetBankA(VRAM_A_LCD);
  
  lcdMainOnBottom();
  
  vector<touchPosition> touchArray;
  
  irqSet(IRQ_VBLANK, Vblank);

  consoleDemoInit();
	
	touchRead(&touchOld);
	
  // iprintf("\x1b[10;0H(debug) Frame = %d",frame);
  printf("\x1b[16;0HLambda = %0.2f",lambda);
	
  while(1) {
	
		swiWaitForVBlank();
		touchRead(&touchXY);
		
    // std::cout << touchXY.px;

    if(touchXY.px != 0 || touchXY.py != 0) {
      if(abs(touchXY.px - touchOld.px) > 3 && abs(touchXY.py - touchOld.py) > 3) {
        touchArray.push_back(touchXY);
        touchOld.px = touchXY.px;
        touchOld.py = touchXY.py;
        setPixel(touchXY.px,touchXY.py,255);
      }
    }
    
    // iprintf("\x1b[10;0H(debug) Frame = %d",frame);
    printf("\x1b[16;0HLambda = %0.2f",lambda);
    // iprintf("\x1b %d", int(touchArray.size()));
    
		// print at using ansi escape sequence \x1b[line;columnH 
		// iprintf("\x1b[10;0HFrame = %d",frame);
       // iprintf("\x1b[16;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
      // iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);    
	
	
       // for(vector<touchPosition>::iterator it = touchArray.begin();it != touchArray.end(); ++it) {
       //          iprintf("\x1b %d, %d", it->px, it->py);
       //          
       //        }
       
       scanKeys();
       if (keysHeld() & KEY_A) {
         drawInterpolationLagrange(touchArray, lambda);
       }
       if (keysHeld() & KEY_UP) {
         lambda = lambda + 0.01;
         if (lambda > 1)
           lambda = 1;
         printf("\x1b[16;0HLambda = %0.2f",lambda);
       }
       if (keysHeld() & KEY_DOWN) {
         lambda = lambda - 0.01;
         if (lambda < 0)
           lambda = 0;
         printf("\x1b[16;0HLambda = %0.2f",lambda);
       }
       if (keysHeld() & KEY_X) {
         clearFullScreen(touchArray);
       }
       if (keysHeld() & KEY_Y) {
         clearScreenKeepingPoints(touchArray);
       }
       if (keysHeld() & KEY_LEFT) {
         touchArray.pop_back();
         clearScreenKeepingPoints(touchArray);
       }
       
       // if (touchArray.size() == 3 && !drawed) {
       //   drawInterpolationLagrange(touchArray, 1);
       //   drawed = true;
       //  }
	}
  
  return 0;
}