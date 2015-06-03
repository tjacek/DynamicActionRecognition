// DepthMapBinIOSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "dataset.h"
#include "pointCloud.h"
#include "Difference.h"
#include "utils.h"
#include "ActionVariance.h"



void createArffDataset(DatasetParametrs params, char depthFileName[]){
  Categories categories=readFulldataset( depthFileName);
  buildFullDataset(params,categories);
}

double getDepth(int x,int y,int t,Action action){
  CDepthMap* cdepth= action.at(t);
  return cdepth->GetItem(x,y);
}

int main(int argc, char * argv[])
{   
	cout << "a";
	//char depthFileName[]="C:/Users/TP/Desktop/kinect/dataset/julka/c5_02.bin";
	//showVibe(depthFileName);
	//saveImg( );
	//showCategory("a18");
	//showClouds();
	//showTransform();
	//showClouds();
	//char depthFileName[]="C:/Users/TP/Desktop/doktoranckie/Dataset/Full/a17/a17_s01_e01_sdepth.bin";
//	char testFileName[]="C:/Users/user/Desktop/kwolek/vibe/test.bin";
	//showDiffAction(depthFileName);
	//showAction(depthFileName);
	//savePointCloud(depthFileName);
	//showAction(depthFileName);
	//showHistogram(depthFileName);
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
	if(argc==1){*/

	   //DatasetParametrs params;
	   //params.thetaBins=8;
	   //params.betaBins=4;
	   //char depthFileName[] = "C:/Users/TP/Desktop/doktoranckie/Dataset/Full";
	   //params.output="C:/Users/TP/Desktop/doktoranckie/final.arff";
	   //createArffDataset( params,depthFileName);
	/*}*/
	
	system("pause");

	return 0;
}

