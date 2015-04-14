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

void test3(DatasetParametrs params, char depthFileName[]){
  Categories categories=readFulldataset( depthFileName);
  buildFullDataset(params,categories);
}

void test5(){
  char depthFileName[] = "C:/Users/user/Desktop/kwolek/LargeDataset";
  Categories categories=readFulldataset( depthFileName);
  //pair<Dataset,Labels> pair=buildFullDataset(categories);
  //saveDataset(pair.first, pair.second);
}

void test6(){
 char depthFileName[] = "C:/Users/user/Desktop/kwolek/LargeDataset/a3/a03_s01_e01_sdepth.bin";
 char depthFileName2[] = "C:/Users/user/Desktop/kwolek/LargeDataset/a4/a04_s01_e01_sdepth.bin";
 char depthFileName3[] = "C:/Users/user/Desktop/kwolek/LargeDataset/a7/a07_s01_e01_sdepth.bin";
 char depthFileName4[] = "C:/Users/user/Desktop/kwolek/LargeDataset/a6/a06_s01_e01_sdepth.bin";
  char depthFileName5[] = "C:/Users/user/Desktop/kwolek/LargeDataset/a17/a17_s01_e01_sdepth.bin";

 Action action= readAction(depthFileName5);
 PointCloud cloud;
 cloud.addAction(action);
 cloud.normalize();
 cloud.save("pointCloudA17.xyz");
}

int main(int argc, char * argv[])
{   cout << argc <<"\n";
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
	}
	//while(true){
	//}
	system("pause");

	return 0;
}

