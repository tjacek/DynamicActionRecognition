#include "StdAfx.h"
#include "pca.h"

void test_pca(){
  vector<vector<double>> points=generateData(100);
  MatrixXd dataPoints = vectorsToMat(points);
  //cout <<dataPoints;
  MatrixXd projection=pca(2,dataPoints);
 // cout <<  applyProjection(points.at(0),projection).size();
}

vector<vector<double>> nonzeroVectors(vector<vector<double>>  vectors){
	//cout <<"Before "<< vectors.at(0).size() <<"\n";
  vector<vector<double>> nonzero;
  int height=vectors.size();
  int width=vectors.at(0).size();
  for(int i=0;i<height;i++){
	vector<double> instance;
	/*for(int j=0;j<width; j++){
		instance.push_back(0);
	}*/
	nonzero.push_back(instance);
  }
  //cout << "Nonzero: " << vectors.size() << "\n";
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
	   //vector<double> oldVector=vectors.at(j);
	   //vector<double> newVector =nonzero.at(j);
	   nonzero.at(j).push_back(vectors.at(j).at(i));
	 }
	}
  }
  //cout <<"After: "<< nonzero.at(0).size() <<"\n";
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
 
  cout <<"\n cov"<< Covariance.cols() <<"\n";

  //MatrixXd eigenVectors = MatrixXd::Zero(dim,dim); 
  MatrixXd eigenVectors = m_solve.eigenvectors().real();

  VectorXd eigenvalues = VectorXd::Zero(dim);
  eigenvalues = m_solve.eigenvalues().real();
  cout <<"\n"<< eigenvalues(0);
  PermutationIndices pi;
  for (int i = 0 ; i < dim; i++){

	  pi.push_back(std::make_pair(eigenvalues(i), i));
	  //cout << eigenvalues(i) <<"\n";
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