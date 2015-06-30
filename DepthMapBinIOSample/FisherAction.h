#pragma once
#include "stdafx.h"
#include "DepthMap.h"
#include "opencv2/contrib/contrib.hpp"
#include "io.h"
using namespace cv;
using namespace std;

pair<vector<Mat>,vector<int>> readInput(string dirname,Categories categ);

void fisherAction(string imagedir,string labelsdir);