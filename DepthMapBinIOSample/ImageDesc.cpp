#include "StdAfx.h"
#include "ImageDesc.h"

void getBowDesc(string input,string output){
  BOWExtractor ext;
  ext.createDictionary(input);
  Desc desc=ext.getDescriptor(input);
  saveDesc(input,output,desc);
}

void  BOWExtractor::createDictionary(string dirName){
  Mat img;
 // cout << dirName;
  ImageList imageList=getImageList(dirName);
 // cout << imageList->size();
  for(int i=0;i<imageList->size();i++){
	string name=imageList->at(i);
    cout << "\n "<< name <<"   \n";
	  //cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );

	img = imread(name.c_str(), CV_LOAD_IMAGE_GRAYSCALE );
	//cv::imshow("Display window",img);
	// cv::waitKey(0);
	vector<KeyPoint> keypoint;
	detector->detect(img, keypoint);
	Mat features;
	extractor->compute(img, keypoint, features);
	bowTrainer->add(features);
	//delete img;*/
  }
  cout << "clustering \n";
  Mat dictionary = bowTrainer->cluster();
  bowDE->setVocabulary(dictionary);
}

Desc BOWExtractor::getDescriptor(string dirName){
  Mat img;
  Desc  desc=new map<string,Mat>();
  ImageList imageList=getImageList(dirName);
  for(int i=0;i<imageList->size();i++){
	string name=imageList->at(i);
	cout << name <<"\n";
   img = imread(name.c_str(), CV_LOAD_IMAGE_GRAYSCALE );
   vector<KeyPoint> keypoint1;
	Mat bowDescriptor;
	detector->detect(img, keypoint1);
	bowDE->compute(img, keypoint1, bowDescriptor);
	std::pair<string ,Mat> ptmp(name,bowDescriptor) ;
	desc->insert(ptmp);
	//delete img;
  }
  return desc;
}

void saveDesc(string dirName,string output,Desc desc){
  ofstream myfile;
  myfile.open (output);
  ImageList imageList=getImageList(dirName);
  for(int i=0;i<imageList->size();i++){
    string name=imageList->at(i);
	Mat mat=desc->find(name)->second;
	myfile << name<< ",";
	for(int j=0;j<mat.size().width;j++){
		myfile << mat.at<float>(0,j) << ",";
	}
	myfile <<"\n";
  }
  myfile.close();
}

BOWExtractor::BOWExtractor(){
  this->matcher = DescriptorMatcher::create("FlannBased");
  this->extractor = new SurfDescriptorExtractor();
  this->detector=new SurfFeatureDetector(100);
  dictionarySize = 300;
  TermCriteria tc(CV_TERMCRIT_ITER, 10, 0.001);
  int retries = 1;
  int flags = KMEANS_PP_CENTERS;
  bowTrainer=new BOWKMeansTrainer(dictionarySize, tc, retries, flags);
  bowDE=new BOWImgDescriptorExtractor(extractor, matcher);
}
