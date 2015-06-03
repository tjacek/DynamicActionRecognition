#pragma once
#include "StdAfx.h"
#include "ActionVariance.h"
#include "io.h"
#include "DepthMapBinFileIO.h"

extern double mean(vector<double> data);
extern double standardDeviation(vector<double> sample);
extern float mediana(vector<float> sample);

extern CDepthMap * getMedianaImage(Action * action);
extern CDepthMap * getRandomImage(Action * action);

extern cv::Mat  depthMap2Mat(CDepthMap* cdepth,bool ucharType);
extern vector<cv::Mat> toArray(ActionArray * aa);
extern void showAction(char depthFileName[]);
extern void showDiffAction(char depthFileName[]);
extern void savePointCloud(char depthFileName[]);
extern void saveImg();
extern void showClouds();
extern void showCategory(string category);
extern void showVibe(char depthFileName[]);
