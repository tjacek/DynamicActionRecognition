// DepthMapBinIOSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DepthMapBinFileIO.h"
#include "io.h"
#include "dataset.h"

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

void test(){
 string dirName ="C:/Users/user/Desktop/kwolek/LargeDataset"; 
 string labelsFile ="C:/Users/user/Desktop/kwolek/labels2.txt";
 Categories cat=readCategories(labelsFile);
 ImageList imageList = getImageList( dirName);
 Labels labels= getLabels(imageList, cat );
 vector<Action> actions= readAllActions(imageList);
 buildDataset(actions,labels);
}

void test2(){
  char depthFileName[] = "C:\\Users\\user\\Desktop\\kwolek\\LargeDataset\\a01_s01_e01_sdepth.bin";
  Action action=readAction(depthFileName);
  FeatureExtractorImpl feature;
  feature.getFeatures(action);
}

void test3(){
  char depthFileName[] = "C:/Users/user/Desktop/kwolek/LargeDataset";
  Categories categories=readFulldataset( depthFileName);
  buildFullDataset(categories);
}

void test4(){
  char depthFileName[] = "C:/Users/user/Desktop/kwolek/LargeDataset";
  Categories categories=readFulldataset( depthFileName);
  vector<string> names=getClassNames();
  map<string,int>::iterator it;
    string str[20];
	string nm[20];

  for(it=categories.begin();it!=categories.end();it++){
     pair<string,int> p_i=*it;
	 string cls=names.at(p_i.second);
	 nm[p_i.second]=p_i.first;
	 str[p_i.second]=cls;
	// cout << p_i.first << " " << cls <<"\n";
  }
  for(int i=0;i<20;i++){
	  cout << str[i] <<" " << nm[i] <<"\n";
  }
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*char depthFileName[] = "C:\\Users\\user\\Desktop\\kwolek\\LargeDataset\\a01_s01_e01_sdepth.bin";
	Action action=readAction(depthFileName);
	cout << action.size();*/
	test3();
	system("pause");

	return 0;
}

