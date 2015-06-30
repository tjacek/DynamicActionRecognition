#pragma once
#include "stdafx.h"
#include "DepthMap.h"
#include "io.h"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/nonfree/features2d.hpp"

using namespace cv; 
typedef map<string,Mat> * Desc;

class BOWExtractor{
  public:
	Ptr<DescriptorMatcher> matcher;
    Ptr<DescriptorExtractor> extractor ;
    SurfFeatureDetector * detector;
	BOWKMeansTrainer * bowTrainer;
	BOWImgDescriptorExtractor * bowDE;
	Mat dictionary;
	int dictionarySize;
	
	BOWExtractor();
	void createDictionary(string dirName);
	Desc getDescriptor(string dirName);
};


void saveDesc(string input,string output,Desc desc);
void getBowDesc(string input,string output);
