/*---------------------------------------------------------------------------------

	Example of interpolation schemes on Nintendo DS
	-- Esse

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
        float t = i*one_step;
        int x = q0_x*(((t - td1)*(t - td2))/((td0 - td1)*(td0 - td2))) + 
         q1_x*(((t - td0)*(t - td2))/((td1 - td0)*(td1 - td2))) +
         q2_x*(((t - td0)*(t - td1))/((td2 - td1)*(td2 - td0)));

        int y = q0_y*(((t - td1)*(t - td2))/((td0 - td1)*(td0 - td2))) + 
          q1_y*(((t - td0)*(t - td2))/((td1 - td0)*(td1 - td2))) +
          q2_y*(((t - td0)*(t - td1))/((td2 - td1)*(td2 - td0)));
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
        float t = i*one_step;
        int x = q0_x*(((t - td1)*(t - td2)*(t - td3))/((td0 - td1)*(td0 - td2)*(td0 - td3))) + 
          q1_x*(((t - td0)*(t - td2)*(t - td3))/((td1 - td0)*(td1 - td2)*(td1 - td3))) +
          q2_x*(((t - td0)*(t - td1)*(t - td3))/((td2 - td1)*(td2 - td0)*(td2 - td3))) +
          q3_x*(((t - td0)*(t - td1)*(t - td2))/((td3 - td1)*(td3 - td0)*(td3 - td2)));

        int y = q0_y*(((t - td1)*(t - td2)*(t - td3))/((td0 - td1)*(td0 - td2)*(td0 - td3))) + 
          q1_y*(((t - td0)*(t - td2)*(t - td3))/((td1 - td0)*(td1 - td2)*(td1 - td3))) +
          q2_y*(((t - td0)*(t - td1)*(t - td3))/((td2 - td1)*(td2 - td0)*(td2 - td3))) +
          q3_y*(((t - td0)*(t - td1)*(t - td2))/((td3 - td1)*(td3 - td0)*(td3 - td2)));
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
    }
    if (keysHeld() & KEY_DOWN) {
      lambda = lambda - 0.01;
      if (lambda < 0)
        lambda = 0;
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