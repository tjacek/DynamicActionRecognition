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

static Mat norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

void showEigenvectors(Mat W,Mat eigenvalues,int height ){

	for (int i = 0; i < min(16, W.cols); i++) {
        string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
        cout << msg << endl;
        // get eigenvector #i
        Mat ev = W.col(i).clone();
        // Reshape to original size & normalize to [0...255] for imshow.
        Mat grayscale = norm_0_255(ev.reshape(1, height));
        // Show the image & apply a Bone colormap for better sensing.
          Mat cgrayscale;
         applyColorMap(grayscale, cgrayscale, COLORMAP_BONE);
        // Display or save:
            imshow(format("fisherface_%d", i), cgrayscale );
      
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
	//for(int j=0;j<mat.size().width;j++){
	//	cout <<"\n"<< j <<" ";
	//	cout << mat;
	//	myfile << mat << ",";
	//}
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
 
  int height = images[0].rows;
  cout << "Computing Fisher vectors \n";
  Ptr<cv::FaceRecognizer> model = createFisherFaceRecognizer(); 
  cout << "Eigenvectors computed";
  model->train(train.first, train.second);
  
  //project(images.at(0),model );
  vector<Mat> proj = projectAll(images,model);
  //model->get<vector<Mat> >("projections");
  
  //cout <<proj.size() <<" " << proj.at(0).cols << " " << proj.at(0).rows;
  saveFisher("fisherZY.csv",proj,labels);

}