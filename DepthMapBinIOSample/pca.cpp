#include "StdAfx.h"
#include "pca.h"

void test_pca(){
  vector<vector<double>> points=generateData(100);
  MatrixXd dataPoints = vectorsToMat(points);
  //cout <<dataPoints;
  MatrixXd projection=pca(2,dataPoints);
  cout <<  applyProjection(points.at(0),projection).size();
}

vector<vector<double>> nonzeroVectors(vector<vector<double>>  vectors){
	cout <<"OKk "<< vectors.at(0).size() <<"\n";
  vector<vector<double>> nonzero;
  int height=vectors.size();
  int width=vectors.at(0).size();
  for(int i=0;i<height;i++){
	  vector<double> instance;
	  nonzero.push_back(instance);
  }
  for(int i=0;i<width;i++){
    bool isZero=true;
    for(int j=0;j<height;j++){
	  vector<double> vector=vectors.at(j);
	  if(vector.at(i)!=0.0){
        isZero=false;
		  nonzero.push_back(vector);
	  }
     }
	if(!isZero){
	 for(int j=0;j<height;j++){
	   vector<double> vector=vectors.at(j);
	   nonzero.at(j).push_back(vector.at(i));
	 }
	}
  }
  cout <<"OK "<< nonzero.at(0).size() <<"\n";
  return nonzero;
}

MatrixXd vectorsToMat(vector<vector<double>>  vectors){
  vector<vector<double>> nonzero=nonzeroVectors(vectors);
  //cout <<"OK "<< nonzero.size() <<"\n";
  int height=nonzero.size();
  int width=nonzero.at(0).size();
  MatrixXd matrix = MatrixXd::Zero(height,width);
  for(int i=0;i<height;i++){
	 vector<double> vector= nonzero.at(i);
     for(int j=0;j<width;j++){
		 matrix(i,j) =vector.at(j);
    } 
  }
 // cout << matrix;
  return matrix.transpose();
}

EigenVectors pca(int newDim,MatrixXd dataPoints){
  //cout << dataPoints;
  int dim = dataPoints.rows(); 
  int size = dataPoints.cols();
  
  double mean; VectorXd meanVector;
 
  for (int i = 0; i < dim; i++){
	   mean = (dataPoints.row(i).sum())/  ((double)size);		 
	   meanVector  = VectorXd::Constant(size,mean); 
	   dataPoints.row(i) -= meanVector;
  }
  //  cout << dataPoints;
  MatrixXd Covariance = MatrixXd::Zero(dim, dim);
  Covariance = (1 / (double) size)  * dataPoints* dataPoints.transpose();
  Eigen::EigenSolver<MatrixXd> m_solve(Covariance);
 
  //cout << Covariance;

  MatrixXd eigenVectors = MatrixXd::Zero(dim,dim); 
  eigenVectors = m_solve.eigenvectors().real();

  VectorXd eigenvalues = VectorXd::Zero(dim);
  eigenvalues = m_solve.eigenvalues().real();

  PermutationIndices pi;
  for (int i = 0 ; i < dim; i++){

	  pi.push_back(std::make_pair(eigenvalues(i), i));
	  //cout << eigenvalues(i) <<"\n";
  }
  sort(pi.begin(), pi.end());

  return getProjectionMatrix(newDim, eigenVectors, pi);
}

MatrixXd getProjectionMatrix(int k,EigenVectors eigenVectors,PermutationIndices pi){
  int size=pi.size();
  MatrixXd projection=MatrixXd::Zero(k,pi.size());
  for(int i=0;i<k;i++){
	  int index=pi.at(size-i-1).second;
	  eigenVectors.col(i).normalize();
	  projection.row(i) =eigenVectors.col(i).transpose();
  }
  return projection;
}

vector<double> applyProjection(vector<double> point, MatrixXd projection){
  vector<double> newPoint;
  VectorXd v=VectorXd::Zero(point.size());
  for(int i=0;i<point.size();i++){
	v[i]=point.at(i);
  }
  VectorXd v2=projection * v;
  for(int i=0;i<v2.size();i++){
	newPoint.push_back(v2[i]);
  }
  return newPoint;
}

vector<vector<double>> generateData(int n){
  vector<vector<double>> vectors;
  /*   std::default_random_engine re;
  std::uniform_real_distribution<double> unif(0,100.0);
  std::uniform_real_distribution<double> noise(0,0.1);
  for(int i=0;i<n;i++){
	 vector<double> vector;
	 double x=unif(re);
	 double y=unif(re);
	 double z=noise(re);
	 double t=noise(re);
	 vector.push_back(x);
     vector.push_back(y);
     vector.push_back(z);
     vector.push_back(t);
	 
     vectors.push_back(vector);
  }*/
  return vectors;
}