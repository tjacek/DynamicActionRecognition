#pragma once
#include "stdafx.h"
#include "DepthMap.h"
#include "PointCloud.h"

class ActionArray{
  public:
    int rows;
    int cols;
	int frames;
	double *** data;
    void convol(double * kernel,ActionArray * action);
    ActionArray(Action * action);
	~ActionArray();
};

class ActionSummary{
  public:
    CDepthMap mean;
    CDepthMap variance;
    
    ActionSummary(ActionArray * action);
	~ActionSummary();
};

PointCloud * actionVar(Action * action,cv::Mat (*projection)(CDepthMap * dimage));
ActionArray *actionDifference(Action * action,cv::Mat (*projection)(CDepthMap * dimage));
ActionArray * transformAction(Action * action, cv::Mat (*projection)(CDepthMap * dimage));
double applyKernel(int x_0,int y_0,int t_0,int k,double * kernel,ActionArray * orginal);

cv::Mat projectionZX(CDepthMap * mat);
cv::Mat projectionZY(CDepthMap * mat);
cv::Mat projectionXY(CDepthMap * mat);

void  showImage(cv::Mat * m,const char * name);