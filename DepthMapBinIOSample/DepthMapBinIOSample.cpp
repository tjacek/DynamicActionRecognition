// DepthMapBinIOSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DepthMapBinFileIO.h"
#include "io.h"
#include "dataset.h"
#include "pointCloud.h"

Action readAction(char depthFileName[]){
  	FILE * fp = fopen(depthFileName, "rb");
	Action action;
	if(fp == NULL)
		return action;

	
	int nofs = 0; //number of frames conatined in the file (each file is a video sequence of depth maps)
	int ncols = 0;
	int nrows = 0;
	ReadDepthMapBinFileHeader(fp, nofs, ncols, nrows);

	//printf("number of frames=%i\n", nofs);

	//read each frame
	int f; //frame index
	for(f=0; f<nofs; f++)
	{
		CDepthMap * depthMap=new CDepthMap();
		depthMap->SetSize(ncols, nrows); //it allocates space
		//the data will be stored in <depthMap>
		ReadDepthMapBinFileNextFrame(fp, ncols, nrows, *depthMap);

		action.push_back(depthMap);
		//check to see what has been loaded for DEBUG purpose:
//		int nNonZeroPoints = depthMap->NumberOfNonZeroPoints();
//		float avg = depthMap->AvgNonZeroDepth();
//		printf("frame[%i], ncols=%i, nrows=%i, count=%i, avg=%f\n", f, depthMap->GetNCols(), depthMap->GetNRows(), nNonZeroPoints, avg );
	}

	fclose(fp);
	fp=NULL;
	return action;
}

vector<Action> readAllActions(ImageList images){
  vector<Action> actions;
  for(int i=0;i<images->size();i++){
	  string str=images->at(i);
	  char * c=(char *) str.c_str();
	  actions.push_back(readAction(c));
  }
  return actions;
}

void createArffDataset(DatasetParametrs params, char depthFileName[]){
  Categories categories=readFulldataset( depthFileName);
  buildFullDataset(params,categories);
}

double getDepth(int x,int y,int t,Action action){
  CDepthMap* cdepth= action.at(t);
  return cdepth->GetItem(x,y);
}

cv::Mat * depthMap2Mat(CDepthMap* cdepth){
  cv::Mat*  mat =new cv::Mat(cdepth->GetNRows(),cdepth->GetNCols(),CV_8UC1 );
  for(int i=0;i<cdepth->GetNRows(); i++){
    for(int j=0;j<cdepth->GetNCols(); j++){
	  //float n= cdepth->GetItem(i,j)/65535.0;
	  mat->at<uchar>(i,j)= (uchar) cdepth->GetItem(i,j);
    }
  }
  return mat;
}

void showAction(char depthFileName[]){
  Action action=readAction( depthFileName);
  cout << action.size();
  vector<cv::Mat*> frames;
  for(int i=0;i<action.size();i++){
    cv::Mat * mat=depthMap2Mat(action.at(i));
	frames.push_back(mat);
  }
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  for(int i = 0;i<frames.size();i++){
     cv::imshow("Display window",*frames.at(i));
       // Sleep(100);
	 cv::waitKey(0);
  }
}

int main(int argc, char * argv[])
{   
	showAction("C:/Users/user/Desktop/kwolek/LargeDataset/full/a1/a01_s01_e01_sdepth.bin");
	/*cout << argc <<"\n";
	if(argc==6){
	  DatasetParametrs params;
	  params.rBins=atoi(argv[1]) ;
	  params.thetaBins=atoi(argv[2]) ;
	  params.betaBins=atoi(argv[3]) ;
	  params.reductedDim=atoi(argv[4]);
	  params.output=argv[5];
	  //test3( params);
    }
	if(argc==1){
       DatasetParametrs params;
	   char depthFileName[] = "C:/Users/user/Desktop/kwolek/LargeDataset/train";
	   params.output="C:/Users/user/Desktop/kwolek/LargeDataset/train.arff";
	   test3( params,depthFileName);
	}*/
	
	system("pause");

	return 0;
}

