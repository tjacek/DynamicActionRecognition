#pragma once
#include "stdafx.h"
#include "DepthMap.h"
#include "opencv2/contrib/contrib.hpp"
#include "io.h"
using namespace cv;
using namespace std;

pair<vector<Mat>,vector<int>> readInput(string dirnam,Categories categ);

void fisherAction(string imagedir,string labelsdir);
Mat project(Mat src,Ptr<cv::FaceRecognizer> model );
pair<vector<Mat>,vector<int>> getTrainingData(vector<Mat> data,vector<int> labels);