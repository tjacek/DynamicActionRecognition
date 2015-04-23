#pragma once
#include "stdafx.h"
#include "DepthMap.h"

class ActionBackground{
  public:
    int rows;
    int cols;
	int frames;
	double *** data;
  public:
    ActionBackground(int frames,int rows,int cols);
};

class Kernel{
  public:
	virtual double K(int x,int y,int z);
};

extern double weightedSum(Action action,Kernel kernel);