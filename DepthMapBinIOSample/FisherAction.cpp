#include "StdAfx.h"
#include "FisherAction.h"

void fisherValidation(FisherValidationParams* params,Reduction reduction){
	Categories trainCat=readCategories(params->trainLabels);
	cout <<"abc";
	pair<vector<Mat>,vector<int>> train=readInput(params->trainImages,trainCat);
	Ptr<cv::FaceRecognizer> model;
    if(reduction==fisherReduction){
      cout << "Computing Fisher vectors \n";
      model = createFisherFaceRecognizer(); 
    }else{
      cout << "Computing PCA vectors \n";
      model = createEigenFaceRecognizer();
    }
	model->train(train.first, train.second);

	vector<Mat> projTrain = projectAll(train.first,model);
	saveFisher(params->trainOutput,projTrain,train.second);

	Categories testCat=readCategories(params->testLabels);
	pair<vector<Mat>,vector<int>> test=readInput(params->testImages,testCat);
	vector<Mat> projTest = projectAll(test.first,model);
	saveFisher(params->testOutput,projTest,test.second);
}

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
    Mat dst,dst2;
	if(src.channels()==1){
      cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
      uchar mode=dst.at<uchar>(0,0);
	  clean(dst,mode);
	  threshold( dst, dst2, 120, 255,3 );
	}
    return dst2;
}

void showEigenvectors(  Ptr<cv::FaceRecognizer> model){
  Mat eigen = model->getMat("eigenvectors");
  
  int size=min(25,eigen.cols);
  for (int i = 0; i < size; i++) {          
	
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

void fisherAction(string imagedir,string labelsdir,string output,Reduction reduction,Operation op){
  Categories categ=readCategories(labelsdir);
  pair<vector<Mat>,vector<int>> p=readInput(imagedir,categ);
  vector<Mat> images= p.first;
  vector<int> labels= p.second;
  pair<vector<Mat>,vector<int>> train=getTrainingData(images,labels);
 
  Ptr<cv::FaceRecognizer> model;
  if(reduction==fisherReduction){
     cout << "Computing Fisher vectors \n";
     model = createFisherFaceRecognizer(); 
  }else{
     cout << "Computing PCA vectors \n";
     model = createEigenFaceRecognizer();//createFisherFaceRecognizer(); 
  }
  model->train(train.first, train.second);
  cout << "Eigenvectors computed";
  if(op==showOp){
    showEigenvectors(model);
  }else{
    vector<Mat> proj = projectAll(images,model);
    saveFisher(output,proj,labels);
  }
}

FisherValidationParams::FisherValidationParams(){
  this->trainImages="C:/Users/TP/Desktop/doktoranckie/images/train/xy";
  this->trainLabels="C:/Users/TP/Desktop/doktoranckie/images/train/labelsXY.csv";
  this->trainOutput="C:/Users/TP/Desktop/doktoranckie/trainxy.out";
  this->testImages="C:/Users/TP/Desktop/doktoranckie/images/test/xy";
  this->testLabels="C:/Users/TP/Desktop/doktoranckie/images/test/labelsXY.csv";
  this->testOutput="C:/Users/TP/Desktop/doktoranckie/testxy.out";
}
