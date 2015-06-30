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
        //if(argc == 2) {
            imshow(format("fisherface_%d", i), cgrayscale );
       // } else {
       //     imwrite(format("%s/fisherface_%d.png", output_folder.c_str(), i), norm_0_255(cgrayscale));
       // }
    }
}

vector<Mat> getTrainingData(vector<Mat> data){
  vector<Mat> train;
  for(int i=0;i<data.size();i++){
	  if(i % 2 ==0){
		train.push_back(data.at(i));
	  }
  }
  return train;
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
  pair<vector<Mat>,vector<int>> pair=readInput(imagedir,categ);
  vector<Mat> images= pair.first;
  vector<int> labels= pair.second;
  int height = images[0].rows;
  cout << "Computing Fisher vectors \n";
  Ptr<cv::FaceRecognizer> model = createFisherFaceRecognizer(); 
  cout << "Eigenvectors computed";
  model->train(images, labels);
  //Mat eigenvalues = model->getMat("eigenvalues");
  //Mat W = model->getMat("eigenvectors");
  vector<Mat> proj = model->get<vector<Mat> >("projections");
  
  cout <<proj.size() <<" " << proj.at(0).cols << " " << proj.at(0).rows;
  saveFisher("fisher.csv",proj,labels);
  //showEigenvectors( W, eigenvalues, height );
}