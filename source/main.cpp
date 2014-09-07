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

#include "knee.h"

using namespace std;

int switch_val = 0;
int interp_type = 0;

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

void drawInterpolationQuadraticLagrange(vector<touchPosition> const &touchPositions, float lambda) {
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

void drawInterpolationCubicLagrange(vector<touchPosition> const &touchPositions, float lambda) {
  int k = touchPositions.size()-1;
  if (k % 3 == 1)
  {
    k = k-1;
  }
  if (k % 3 == 2)
  {
    k = k-2;
  }
  for(int j=0;j<k;j = j+3) {    
    int td0 = 0;
    int n_x = touchPositions[j+1].px - touchPositions[j+0].px;
    int n_y = touchPositions[j+1].py - touchPositions[j+0].py;
    float td1 = pow(sqrt(pow(n_x,2) + pow(n_y,2)),lambda);
    n_x = touchPositions[j+2].px - touchPositions[j+1].px;
    n_y = touchPositions[j+2].py - touchPositions[j+1].py;
    float td2 = pow(sqrt(pow(n_x,2) + pow(n_y,2)),lambda) + td1;
    n_x = touchPositions[j+3].px - touchPositions[j+2].px;
    n_y = touchPositions[j+3].py - touchPositions[j+2].py;
    float td3 = pow(sqrt(pow(n_x,2) + pow(n_y,2)),lambda) + td2;
    int q0_x = touchPositions[j+0].px;
    int q0_y = touchPositions[j+0].py;
    int q1_x = touchPositions[j+1].px;
    int q1_y = touchPositions[j+1].py;
    int q2_x = touchPositions[j+2].px;
    int q2_y = touchPositions[j+2].py;
    int q3_x = touchPositions[j+3].px;
    int q3_y = touchPositions[j+3].py;
    float range = td3 - td0;
    float one_step = range/1000;
      for(int i=0;i<(td3*(1/one_step));i++) {
    //    list4points[[1]] + a1 * s + a2 * s^2
        float t = i*one_step;
        int x = q0_x*(((t - td1)*(t - td2)*(t - td3))/((td0 - td1)*(td0 - td2)*(td0 - td3))) + 
          q1_x*(((t - td0)*(t - td2)*(t - td3))/((td1 - td0)*(td1 - td2)*(td1 - td3))) +
          q2_x*(((t - td0)*(t - td1)*(t - td3))/((td2 - td1)*(td2 - td0)*(td2 - td3))) +
          q3_x*(((t - td0)*(t - td1)*(t - td2))/((td3 - td1)*(td3 - td0)*(td3 - td2)));

        int y = q0_y*(((t - td1)*(t - td2)*(t - td3))/((td0 - td1)*(td0 - td2)*(td0 - td3))) + 
          q1_y*(((t - td0)*(t - td2)*(t - td3))/((td1 - td0)*(td1 - td2)*(td1 - td3))) +
          q2_y*(((t - td0)*(t - td1)*(t - td3))/((td2 - td1)*(td2 - td0)*(td2 - td3))) +
          q3_y*(((t - td0)*(t - td1)*(t - td2))/((td3 - td1)*(td3 - td0)*(td3 - td2)));
        // iprintf("\x1b[16;0H x = %d, y = %d\n", x,y);
        setPixel(x,y,255);
      }

  }
}

void drawInterpolation(vector<touchPosition> const &touchPositions, float lambda) {
  if (interp_type == 0)
    drawInterpolationQuadraticLagrange(touchPositions, lambda);
  if (interp_type == 1)
    drawInterpolationCubicLagrange(touchPositions, lambda);
}


void printInterp() {
  if (interp_type == 0) 
    iprintf("\x1b[17;0HLagrange Quadratic");
  if (interp_type == 1) 
    iprintf("\x1b[17;0HLagrange Cubic       ");
}

void drawBg() {
  if (switch_val == 0) {
    vramDefault();
    videoSetMode(MODE_FB0);
    vramSetBankA(VRAM_A_LCD);
    consoleDemoInit();
  }
  if (switch_val == 1) {
    decompress(kneeBitmap, VRAM_A,  LZ77Vram);
  }
  printInterp();
}

void clearFullScreen(vector<touchPosition> &touchPositions) {
  drawBg();
  touchPositions.clear();
}

void clearScreenKeepingPoints(vector<touchPosition> const &touchPositions) {
  drawBg();
  for(vector<touchPosition>::const_iterator it = touchPositions.begin(); it != touchPositions.end(); ++it) {
    setPixel(it->px, it->py, 255);
  }
}


int main(void) {
  
  // powerOn(POWER_ALL_2D);
  
  // videoSetMode(MODE_5_2D);
  // vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
  
  // int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

  // dmaCopy(kneeBitmap, bgGetGfxPtr(bg3), 256*256);
  // dmaCopy(kneePal, BG_PALETTE, 256*2);
  
  bool drawed = false;
  
  touchPosition touchXY;
  touchPosition touchOld;

  float lambda = 1;
  
  videoSetMode(MODE_FB0);
  vramSetBankA(VRAM_A_LCD);
  
  lcdMainOnBottom();
  
  vector<touchPosition> touchArray;
	
	touchRead(&touchOld);
	
  //  section background
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
	
  consoleDemoInit();

	
  while(1) {
	
		swiWaitForVBlank();
		touchRead(&touchXY);

    if(touchXY.px != 0 || touchXY.py != 0) {
      if(abs(touchXY.px - touchOld.px) > 3 && abs(touchXY.py - touchOld.py) > 3) {
        touchArray.push_back(touchXY);
        touchOld.px = touchXY.px;
        touchOld.py = touchXY.py;
        setPixel(touchXY.px,touchXY.py,255);
      }
    }
    
    printf("\x1b[16;0HLambda = %0.2f",lambda);
    printInterp();
       
    scanKeys();
    if (keysHeld() & KEY_A) {
      drawInterpolation(touchArray, lambda);
      drawed = true;
    }
    if (keysHeld() & KEY_UP) {
      lambda = lambda + 0.01;
      if (lambda > 1)
        lambda = 1;
      // printf("\x1b[16;0HLambda = %0.2f",lambda);
    }
    if (keysHeld() & KEY_DOWN) {
      lambda = lambda - 0.01;
      if (lambda < 0)
        lambda = 0;
      // printf("\x1b[16;0HLambda = %0.2f",lambda);
    }
    if (keysHeld() & KEY_X) {
      clearFullScreen(touchArray);
      drawed = false;
    }
    if (keysHeld() & KEY_Y) {
     clearScreenKeepingPoints(touchArray);
     drawed = false;
    }
    if (keysHeld() & KEY_LEFT) {
      switch_val = switch_val - 1;
      if (switch_val < 0) {
        switch_val = 0;
        } else {
          clearScreenKeepingPoints(touchArray);
          if (drawed)
            drawInterpolation(touchArray, lambda);
        }
    }
    if (keysHeld() & KEY_RIGHT) {
      switch_val = switch_val + 1;
      if (switch_val > 1) {
        switch_val = 1;
      } else {
        clearScreenKeepingPoints(touchArray);
        if (drawed)
          drawInterpolation(touchArray, lambda);
      }
    }
    if (keysHeld() & KEY_L) {
       interp_type = interp_type - 1;
       if (interp_type < 1) {
         interp_type = 0;
         printInterp();
       } 
     }
    if (keysHeld() & KEY_R) {
      interp_type = interp_type + 1;
      if (interp_type > 1) {
        interp_type = 1;
        printInterp();
      } 
    }
  }  
  return 0;
}