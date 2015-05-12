// DepthMapBinIOSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DepthMapBinFileIO.h"
#include "io.h"
#include "dataset.h"
#include "pointCloud.h"
#include "Difference.h"
#include "ShapeContext3D.h"
#include "ActionVariance.h"

Action readAction(char depthFileName[]){
  	FILE * fp = fopen(depthFileName, "rb");
	Action action;
	if(fp == NULL)
		return action;

	
	int nofs = 0; //number of frames conatined in the file (each file is a video sequence of depth maps)
	int ncols = 0;
	int nrows = 0;
	ReadDepthMapBinFileHeader(fp, nofs, ncols, nrows);

	cout << nofs << " " << ncols << " " << nrows <<" " <<"\n";
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

cv::Mat  depthMap2Mat(CDepthMap* cdepth,bool ucharType){
 cv::Mat mat=cv::Mat::zeros(cdepth->GetNRows(),cdepth->GetNCols(),CV_8UC1 );
  for(int i=0;i<cdepth->GetNRows(); i++){
    for(int j=0;j<cdepth->GetNCols(); j++){
	 float n=0;
	 if(ucharType){
	   n=abs(cdepth->GetItem(i,j)); 
	 }else{
	   n=abs(cdepth->GetItem(i,j)/4.0);
	 }
	  mat.at<uchar>(i,j)= (uchar) n;//cdepth->GetItem(i,j);
    }
  }
  return mat;
}

vector<cv::Mat> toArray(ActionArray * aa){
  vector<cv::Mat> action;
  for(int t=0;t<aa->frames;t++){
	cv::Mat mat=cv::Mat::zeros(aa->rows,aa->cols,CV_8UC1);
	for(int i=0;i<aa->rows;i++){
      for(int j=0;j<aa->rows;j++){
         float n=abs(aa->data[t][i][j]); 
		 mat.at<uchar>(i,j)= (uchar) n;
	  }
	}
	action.push_back(mat);
  }
  return action;
}

void showAction(char depthFileName[]){
  Action action=readAction( depthFileName);
  cout << action.size();
//  differenceOfGaussian3D(&action,false);
  ActionArray * aa=actionDifference(&action,projectionZX);
  vector<cv::Mat> frames=toArray(  aa);
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  for(int i = 0;i<frames.size();i++){
     cv::imshow("Display window",frames.at(i));
       //Sleep(300);
	 cv::waitKey(0);
  }
} 

void showDiffAction(char depthFileName[]){
  Action action=readAction( depthFileName);
  ActionArray * aa=actionDifference(&action,projectionZX);
  ActionSummary * summary=new ActionSummary(aa);
  vector<cv::Mat*> frames;
  cv::Mat  mat=depthMap2Mat(&summary->mean,false);
  frames.push_back(&mat);
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  for(int i = 0;i<frames.size();i++){
     cv::imshow("Display window",*frames.at(i));
       //Sleep(300);
	 cv::waitKey(0);
  }
}

void savePointCloud(char depthFileName[]){
  Action action=readAction( depthFileName);
  //differenceOfGaussian3D(&action,true);
  DynamicPointCloud pointCloud;
  pointCloud.addTimeAction(&action);
  pointCloud.save("test.txt");
}
void showHistogram(char depthFileName[]){
  Action action=readAction( depthFileName);
  //Dy
//  differenceOfGaussian3D(&action,true);
  //getSimpeShapeContext(params,  action);
}

void saveImg(){
  char depthFileName[]="C:/Users/TP/Desktop/doktoranckie/Dataset/Full";
  Categories categories=readFulldataset( depthFileName);
  map<string,int>::iterator it;
  int k=0;
  int i=0;
  for(it=categories.begin();it!=categories.end();it++){
	pair<string,int> p_i=*it;
	cout << p_i.first<<"\n";
	if(k!=p_i.second){
		k=p_i.second;
		i=0;
	}
	char * c=(char*)p_i.first.c_str();
	Action action=readAction(c);
	ActionArray * diff= actionDifference(&action, projectionZY);
    ActionSummary * summary=new ActionSummary(diff);
	cv::Mat mat=depthMap2Mat(&summary->mean,false);
	cv::imwrite("cZY"+intToString(p_i.second)+"_" +intToString(i) +".jpg", mat);
	delete diff;
	delete summary;
	i++;
  }
}


void showClouds(){
  string prefix="C:/Users/TP/Desktop/doktoranckie/Dataset/Full/";
  string cats[]= 
  {"a1/a01_s01_e01_sdepth.bin","a2/a02_s01_e01_sdepth.bin","a3/a03_s01_e01_sdepth.bin","a4/a04_s01_e01_sdepth.bin",
   "a5/a05_s01_e01_sdepth.bin","a6/a06_s01_e01_sdepth.bin","a7/a07_s01_e01_sdepth.bin","a8/a08_s01_e01_sdepth.bin",
   "a9/a09_s01_e01_sdepth.bin","a10/a10_s01_e01_sdepth.bin","a11/a11_s01_e01_sdepth.bin","a12/a12_s01_e01_sdepth.bin",
   "a13/a13_s01_e01_sdepth.bin","a14/a14_s01_e01_sdepth.bin","a15/a15_s01_e01_sdepth.bin","a16/a16_s01_e01_sdepth.bin",
   "a17/a17_s01_e01_sdepth.bin","a18/a18_s01_e01_sdepth.bin","a19/a19_s01_e01_sdepth.bin","a20/a20_s05_e01_sdepth.bin"};
  for(int i=0;i<20;i++){
    string category=cats[i];
	cout << category  <<"\n";
	string fullPath=prefix+category;
	 char *cstr = (char*)fullPath.c_str();
	Action action=readAction(cstr);
//    differenceOfGaussian3D(&action,false);
	//ActionSummarry summary( &action);

	PointCloud * pointCloud=actionVar( &action,projectionZX);

    pointCloud->save("cloud"+intToString(i)+".txt");
  }

}

void showCategory(string category){
  string prefix="C:/Users/TP/Desktop/doktoranckie/Dataset/Full";
  string dirName=prefix+category;
  ImageList images=getImageList(dirName);
  for(int i=0;i<images->size();i++){
	cout << images->at(i) << "\n";
	char *cstr = (char*)images->at(i).c_str();
	Action action=readAction(cstr);
	PointCloud * pointCloud=actionVar( &action,projectionXY);
    pointCloud->save(category+intToString(i)+".txt");
  }
}


int main(int argc, char * argv[])
{   
	saveImg( );
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

