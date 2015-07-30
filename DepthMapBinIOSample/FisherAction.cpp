#include "StdAfx.h"
#include "FisherAction.h"

pair<vector<Mat>,vector<int>> readInput(string dirname,Categories categ){
  ImageList imageNames=getImageList(dirname);
  vector<Mat> images;
  vector<int> labels;

  for(int i=0;i<imageNames->size();i++){
	string name=imageNames->at(i);
	cout << name <<" ";
	Mat img = imread(name.c_str(), CV_LOAD_IMAGE_GRAYSCALE );
	images.push_back(img);
	int cat=categ[name];
	cout << cat <<"\n";
	labels.push_back(cat);
  }
  pair<vector<Mat>,vector<int>> pair(images,labels);
  return pair;
}

void clean(Mat dist,uchar k){
  for(int i=0;i<dist.rows;i++){
	for(int j=0;j<dist.cols;j++){
      if(dist.at<uchar>(i,j)==k){
		  //cout << dist.at<uchar>(i,j);
		  dist.at<uchar>(i,j)=0;
	  }
    }
  }
}

static Mat norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
	if(src.channels()==1){
      cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
      uchar mode=dst.at<uchar>(0,0);
	  clean(dst,mode);	
	}
    return dst;
}

void showEigenvectors(  Ptr<cv::FaceRecognizer> model){
  Mat eigen = model->getMat("eigenvectors");

  for (int i = 0; i < eigen.cols; i++) {          
	
    Mat eigenaction = eigen.col(i).clone();
	cout <<"\n" << eigenaction.cols << " " << eigenaction.rows<<" \n";
	eigenaction=norm_0_255(eigenaction.reshape(0,320));
	cout <<"\n" << eigenaction.cols << " " << eigenaction.rows<<" \n";
	imshow(format("fisheraction_%d", i), eigenaction );
	waitKey(0); 
  }
}

Mat project(Mat src,Ptr<cv::FaceRecognizer> model ){
 Mat eigenvectors = model->getMat("eigenvectors");
 Mat mean = model->getMat("mean");
 Mat q = subspaceProject(eigenvectors, mean, src.reshape(1,1));
 return q;
}

vector<Mat> projectAll(vector<Mat> src,Ptr<cv::FaceRecognizer> model){
  vector<Mat> projections;
  for(int i=0;i<src.size();i++){
	  Mat newVector=project(src.at(i),model);
	  projections.push_back(newVector);
  }
  return projections;
}

pair<vector<Mat>,vector<int>> getTrainingData(vector<Mat> data,vector<int> labels){
  vector<Mat> train;
  vector<int> trainY;
  for(int i=0;i<data.size();i++){
	  if(i % 2 ==0){
		train.push_back(data.at(i));
		trainY.push_back(labels.at(i));
	  }
  }
  pair<vector<Mat>,vector<int>> pair(train,trainY);
  return pair;
}

void saveFisher(string output,vector<Mat> data,vector<int> labels){
  ofstream myfile;
  myfile.open (output);
  for(int i=0;i<data.size();i++){
	Mat mat=data.at(i);
	cout << i <<"";
	myfile << mat << ",";
	myfile<<labels.at(i);
	myfile <<"\n";
  }
  myfile.close();
}

void fisherAction(string imagedir,string labelsdir){
  Categories categ=readCategories(labelsdir);
  pair<vector<Mat>,vector<int>> p=readInput(imagedir,categ);
  vector<Mat> images= p.first;
  vector<int> labels= p.second;
  pair<vector<Mat>,vector<int>> train=getTrainingData(images,labels);
 
  cout << "Computing Fisher vectors \n";
  Ptr<cv::FaceRecognizer> model = createFisherFaceRecognizer(); 
  model->train(train.first, train.second);
  cout << "Eigenvectors computed";
  showEigenvectors(model);
  //vector<Mat> proj = projectAll(images,model);
  
  //saveFisher("fisherZY.csv",proj,labels);

}