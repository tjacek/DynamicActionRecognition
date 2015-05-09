#include "StdAfx.h"
#include "ActionVariance.h"

PointCloud * actionVar(Action * action,Projection projection){
  /*zero(action->at(0));
  zero(action->at(1));
  zero(action->at(action->size()-2));
  zero(action->at(action->size()-1));*/
  PointCloud * cloud=new PointCloud();
  ActionArray * diff= actionDifference(action, projection);
  ActionSummary * summary=new ActionSummary(diff);
  cloud->addDepthMap(&summary->variance);  
  cloud->normalize();
  delete diff;
  delete summary;
  return cloud;
}

ActionArray * actionDifference(Action * action,Projection projection){
  double *kernel=NULL;
  ActionArray * orginal= transformAction(action, projection);
  ActionArray * difference=new ActionArray(action);
  difference->convol(kernel,orginal);
  delete orginal;
  return difference;
}

ActionArray * transformAction(Action * action, Projection projection){
  ActionArray * actionArray=new ActionArray(action);
  for(int t=0;t<action->size();t++){
	CDepthMap * mat=projection(action->at(t));
	for(int i=0;i<mat->GetNRows();i++){
		for(int j=0;j<mat->GetNCols();j++){
           //cout << mat->GetNRows() <<" " << mat->GetNCols()<<"\n";
		  //cout << actionArray->rows <<" " << actionArray->cols<<"\n";

		  double value=mat->GetItem(i,j);
		  actionArray->data[t][i][j]=value;
	  }
	}
	delete mat;
  }
  return actionArray;
}

CDepthMap * projectionXY(CDepthMap * dimage){
  CDepthMap * nimage=new CDepthMap();
  nimage->SetSize(dimage->GetNCols(),dimage->GetNRows());
  for(int i=0;i<dimage->GetNRows();i++){
	for(int j=0;j<dimage->GetNCols();j++){
		nimage->SetItem(i,j, dimage->GetItem(i,j));
    }
  }
  return nimage;
}

CDepthMap *  projectionZX(CDepthMap * dimage){
  CDepthMap *  zx=new CDepthMap();
  zx->SetSize(  dimage->GetNCols(), dimage->GetNRows() );
  PointCloud pointCloud;
  pointCloud.addDepthMap(dimage);
  pair<Point3D, Point3D> extr=pointCloud.computeExtremes();
  double z_min=extr.first.val[2];
  double d_z=extr.second.val[2] - z_min;
  for(int i=0;i<pointCloud.points.size();i++){
	 int z_i=pointCloud.points.at(i).val[2] - z_min;
	 double y_i=pointCloud.points.at(i).val[1];
	 int x_i=pointCloud.points.at(i).val[0];
	 if(0<x_i && x_i<dimage->GetNRows()){
	  // zx.at<uchar>(x_i,z_i)=y_i;
	   if(0<z_i && z_i<dimage->GetNCols()){
		 zx->SetItem(x_i,z_i,y_i);
	   }
	   //zx.at<uchar>(x_i,z_i-1)=y_i;
	 }
  }
  return zx;
}

void zero(CDepthMap * dmap){
  for(int j=0;j<dmap->GetNRows();j++){
	for(int k=0;k<dmap->GetNCols();k++){
		dmap->SetItem(j,k,0);
	}
  }
}

CDepthMap *projectionZY(CDepthMap * dimage){
  CDepthMap *  zx=new CDepthMap();
  zx->SetSize( dimage->GetNCols(), dimage->GetNRows() );
  PointCloud pointCloud;
  pointCloud.addDepthMap(dimage);
  pair<Point3D, Point3D> extr=pointCloud.computeExtremes();
  double z_min=extr.first.val[2];
  double d_z=extr.second.val[2] - z_min;
  for(int i=0;i<pointCloud.points.size();i++){
	 int z_i=pointCloud.points.at(i).val[2] - z_min;
	 int y_i=pointCloud.points.at(i).val[1];
	 double x_i=pointCloud.points.at(i).val[0];
	 if(0<x_i && x_i<dimage->GetNRows()){
	   if(0<z_i && z_i<dimage->GetNCols()){
		 zx->SetItem(z_i,y_i,x_i);
	   }
	 }

  }
  return zx;
}

void showImage(cv::Mat * m,const char * name){
  cv::namedWindow( name, CV_WINDOW_AUTOSIZE );
  cv::imshow( name, *m);
  cv::waitKey(0);
}

ActionArray::ActionArray(Action * action){
  this->frames=action->size();
  this->rows=action->at(0)->GetNRows();
  this->cols=action->at(0)->GetNCols();
  data=new double**[frames];
  for(int i=0;i<frames;i++){
	data[i]=new double*[rows];
	for(int j=0;j<rows;j++){
	  data[i][j]=new double[cols];
    }
  }
}

ActionArray::~ActionArray(){
  for(int i=0;i<frames;i++){
	for(int j=0;j<rows;j++){
	  delete data[i][j];
    }
    delete data[i];
  }
}

void ActionArray::convol(double * kernel,ActionArray * orginal){
  for(int i=0;i<frames;i++){
	for(int j=0;j<rows;j++){
	  for(int k=0;k<cols;k++){
	    //data[i][j][k]=weightedSum(i,j,k, orginal, kernel);
		data[i][j][k]=applyKernel2(j,k,i,2, kernel, orginal);
	  }
    }
  }
}

double applyKernel2(int x_0,int y_0,int t_0,int k,double * kernel,ActionArray * orginal){
  if(t_0<1){
    return 0;
  }
  if(t_0>=orginal->frames-2){
    return 0;
  }
  if(x_0==0 || y_0==0){
	  return 0;
  }
  if(x_0==orginal->rows-1 || y_0==orginal->cols-1){
	  return 0;
  }
  if(orginal->data[t_0][x_0][y_0]==0){
	return 0;
  }
  double sum=0.0;
  double C= 1.0/ 8.0;
  sum+=orginal->data[t_0+1][x_0+1][y_0+1];
  sum+=orginal->data[t_0+1][x_0+1][y_0];
  sum+=orginal->data[t_0+1][x_0+1][y_0-1];
  
  sum+=orginal->data[t_0+1][x_0][y_0+1];
  sum+=orginal->data[t_0+1][x_0][y_0];
  sum+=orginal->data[t_0+1][x_0][y_0-1];

  sum+=orginal->data[t_0+1][x_0-1][y_0-1];
  sum+=orginal->data[t_0+1][x_0-1][y_0-1];
  sum+=orginal->data[t_0+1][x_0-1][y_0-1];
  sum*=C;
  sum=orginal->data[t_0][x_0][y_0]-sum;
  if(sum>30){
    return sum;
  }else{
    return 0;
  }
}

double applyKernel(int x_0,int y_0,int t_0,int k,double * kernel,ActionArray * orginal){
  if(t_0<2){
    return 0;
  }
  if(t_0>=orginal->frames-2){
    return 0;
  }
  if(orginal->data[t_0][x_0][y_0]==0){
	return 0;
  }
  double sum=0.0;
  //double C= 1/ (2.0 * ((double) k) + 1.0);
  double C=1/5.0;
  for(int i=1;i<k;i++){
     int t_i=t_0+i;
	 if(t_i<orginal->frames){
	   sum-=C*orginal->data[t_i][x_0][y_0];
	 }
  }
  sum+=(1.0 - C)*orginal->data[t_0][x_0][y_0];
  for(int i=k;i>0;i--){
     int t_i=t_0-i;
	 if(0<=t_i){
	   sum-=C*orginal->data[t_i][x_0][y_0];
	 }
  }
  if(sum>10){
    return sum;
  }else{
    return 0;
  }
}

ActionSummary::~ActionSummary(){
	this->mean.~CDepthMap();
	this->variance.~CDepthMap();
}

ActionSummary:: ActionSummary(ActionArray * action){
  variance.SetSize(action->rows,action->cols);
  mean.SetSize(action->rows,action->cols);
  int n=action->frames;
  float n_f=n;
  for(int i=0;i<action->rows;i++){
	  for(int j=0;j<action->cols;j++){
      float sum=0.0;
      for(int k=0;k<n;k++){
		sum+=action->data[k][i][j];
      }
	  sum/=n_f;
	  if(sum>0){
	    mean.SetItem(i,j,sum);
	  }else{
		mean.SetItem(i,j,0);
	  }
	}
  }

  for(int i=0;i<action->rows;i++){
    for(int j=0;j<action->cols;j++){
      float sd=0;
      for(int k=0;k<n;k++){
		if(action->data[k][i][j]>0){
		   double diff =mean.GetItem(i,j) - action->data[k][i][j]; 
		   sd+=diff*diff;
		  }
      }
	  sd/=(n_f-1.0);
	  variance.SetItem(i,j,sqrt(sd));
	}
  }
}