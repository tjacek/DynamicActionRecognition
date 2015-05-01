#include "StdAfx.h"
#include "pca.h"

/*void test_pca(){
  vector<vector<double>> points=generateData(100);
  MatrixXd dataPoints = vectorsToMat(points);
  MatrixXd projection=pca(2,dataPoints);
}

vector<vector<double>> nonzeroVectors(vector<vector<double>>  vectors){
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
	  vector<double> instance=vectors.at(j);
	  if(instance.at(i)!=0.0){
		isZero=false;
	  }
	  if( instance.at(i)!=instance.at(i) ){
			cout << "NAN";
	  }
     }
	if(!isZero){
	 for(int j=0;j<height;j++){
	  
	   nonzero.at(j).push_back(vectors.at(j).at(i));
	 }
	}
  }
  return nonzero;
}

MatrixXd vectorsToMat(vector<vector<double>>  vectors){
	cout << "\n before: " << vectors.size()<<"\n";
  vector<vector<double>> nonzero=nonzeroVectors(vectors);
  //cout <<"OK "<< nonzero.size() <<"\n";
  int height=nonzero.size();
  int width=nonzero.at(0).size();
  cout << "after: " << height <<" " << width << "\n";
  MatrixXd matrix = MatrixXd::Zero(height,width);
  for(int i=0;i<height;i++){
	 vector<double> vector= nonzero.at(i);
     for(int j=0;j<width;j++){
		 matrix(i,j) =vector.at(j);
    } 
  }
  cout << matrix.size();
  return matrix.transpose();
}

EigenVectors pca(int newDim,MatrixXd dataPoints){
  int dim = dataPoints.rows(); 
  int size = dataPoints.cols();
  
  double mean; VectorXd meanVector;
 
  for (int i = 0; i < dim; i++){
	   mean = (dataPoints.row(i).sum())/  ((double)size);		 
	   meanVector  = VectorXd::Constant(size,mean); 
	   dataPoints.row(i) -= meanVector;
  }
  MatrixXd Covariance = MatrixXd::Zero(dim, dim);
  Covariance = (1 / (double) size)  * dataPoints* dataPoints.transpose();
  Eigen::EigenSolver<MatrixXd> m_solve(Covariance);
 
  cout <<"\n cov"<< Covariance.cols() <<"\n";

  MatrixXd eigenVectors = m_solve.eigenvectors().real();

  VectorXd eigenvalues = VectorXd::Zero(dim);
  eigenvalues = m_solve.eigenvalues().real();
  cout <<"\n"<< eigenvalues(0);
  PermutationIndices pi;
  for (int i = 0 ; i < dim; i++){

	  pi.push_back(std::make_pair(eigenvalues(i), i));
  }
  sort(pi.begin(), pi.end());

  if(newDim < dim){
    return getProjectionMatrix(newDim, eigenVectors, pi);
  }else{
    return getProjectionMatrix(dim, eigenVectors, pi);
  }
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

  return vectors;
}*/