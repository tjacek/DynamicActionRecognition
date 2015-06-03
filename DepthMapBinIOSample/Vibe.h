#pragma once
#include "StdAfx.h"

#include <math.h>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "DepthMap.h"
#include "utils.h"
#include "Connected.h"

using namespace cv;

class VibeParams{
  public:
   int nbSamples;                   
   int reqMatches;                   
   int radius;                      
   int subsamplingFactor; 

   VibeParams();
   VibeParams(int nbSamples,int reqMatches,int radius,int subsamplingFactor);

   bool decideUpdate();
};

extern void vibeAction(Action * action,VibeParams * params);
extern float inline metric(float  x,float y);
