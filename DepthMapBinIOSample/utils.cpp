#include "StdAfx.h"
#include "utils.h"
#include "Vibe.h"

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
  //ActionArray * aa=actionDifference(&action,projectionZX);
  //vector<cv::Mat> frames=toArray(  aa);
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  for(int i = 0;i<action.size();i++){
     cv::imshow("Display window",depthMap2Mat(action.at(i),true));
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

void showVibe(char depthFileName[]){
  Action action=readAction( depthFileName);
  //cout << action.size() <<"\n";
  VibeParams * params=new VibeParams();
  vibeAction(&action, params);
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  for(int i = 0;i<action.size();i++){
     cv::imshow("Display window",depthMap2Mat(action.at(i),true));
       //Sleep(300);
	 cv::waitKey(0);
  }
}

double mean(vector<double> data){
  double avg=0.0;
  vector<double>::iterator it ;
  double n= (data.size()-1);
  for (it = data.begin(); it != data.end(); it++){
      float x= *it;
	  avg+=x;
  }
  avg /=n;
  return avg;
}

double standardDeviation(vector<double> data){
  vector<double>::iterator it ;
  double avg=mean(data);
  double sd=0.0;
  double n= (data.size()-1);
  for(it = data.begin(); it != data.end(); it++){
     double error=avg - *it;
	 error=(error*error)/n;
	 sd+=error;
  }
  return sqrt( sd);
}

//bool compare (int i,int j) { return (i<j); }


float mediana(vector<float> sample){
  std::sort(sample.begin(),sample.end());
  int n=sample.size();
  if(n % 2 ==0){
	n/=2;
	return sample.at(n);
  }
  n/=2;
  return sample.at(n+1);
}

CDepthMap * getMedianaImage(Action * action){
  CDepthMap * depth=  action->at(0);
  CDepthMap * median=new CDepthMap();
  median->SetSize(depth->GetNCols(),depth->GetNRows());
  for(int i=1;i<depth->GetNCols()-1;i++){
	for(int j=1;j<depth->GetNRows()-1;j++){
      median->SetItem(j,i, 0.0);
      vector<float> points;
	  for(int t=0;t<action->size();t++){
		  points.push_back(depth->GetItem(j,i));
		  //value+=depth->GetItem(j,i);
	  }
	  //value/=(float) action->size();
	  median->SetItem(j,i, mediana( points));
	}
  }
  return median;
}

CDepthMap * getRandomImage(Action * action){
  CDepthMap * depth=  action->at(0);
  CDepthMap * median=new CDepthMap();
  median->SetSize(depth->GetNCols(),depth->GetNRows());
  for(int i=1;i<depth->GetNCols()-1;i++){
	for(int j=1;j<depth->GetNRows()-1;j++){
     
	  int n=rand() % action->size();	  
	  float value=action->at(n)->GetItem(j,i);
	  median->SetItem(j,i, value);
	}
  }
  return median;
}