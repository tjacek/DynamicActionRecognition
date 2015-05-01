#include "StdAfx.h"
#include "Difference.h"



void differenceOfGaussian3D(Action * action,bool filter){
  ActionArray background(action);
  //Kernel * exp=new ExpKernel(1.0,1.0,1.0);
  ActionArray * orginal=toActionArray(action);
  background.convol(NULL,orginal);
  for(int i=0;i<action->size();i++){
	CDepthMap* dmap=action->at(i);
	for(int j=0;j<dmap->GetNRows();j++){
	  for(int k=0;k<dmap->GetNCols();k++){
		float val=dmap->GetItem(j,k)- background.data[i][j][k];
		if(val > 10.0){
          if(!filter){
		    dmap->SetItem(j,k,val);
		  }
		}else{
          dmap->SetItem(j,k,0);
		}
	  }
	}
   }
  delete orginal;
  zero(action->at(0));
  zero(action->at(1));
  zero(action->at(action->size()-2));
  zero(action->at(action->size()-1));
}

void zero(CDepthMap * dmap){
  for(int j=0;j<dmap->GetNRows();j++){
	for(int k=0;k<dmap->GetNCols();k++){
		dmap->SetItem(j,k,0);
	}
  }
}

ActionArray* toActionArray(Action * action){
  ActionArray * actionArray=new ActionArray(action);
  for(int i=0;i<action->size();i++){
	CDepthMap* dmap=action->at(i);
	for(int j=0;j<dmap->GetNRows();j++){
	  for(int k=0;k<dmap->GetNCols();k++){
		 actionArray->data[i][j][k]=dmap->GetItem(j,k);
		 //cout <<  actionArray->data[i][j][k];
	   }
	}
  }
  return actionArray;
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

void ActionArray::convol(Kernel * kernel,ActionArray * orginal){
  for(int i=0;i<frames;i++){
	for(int j=0;j<rows;j++){
	  for(int k=0;k<cols;k++){
	    //data[i][j][k]=weightedSum(i,j,k, orginal, kernel);
		//data[i][j][k]=timeSum(i,j,k, orginal, kernel);
	  }
    }
  }
}

ExpKernel::ExpKernel( double sigmaX,double sigmaY,double sigmaZ){
  this->sigmaX=sigmaX;
  this->sigmaY=sigmaY;
  this->sigmaZ=sigmaZ;
}

double ExpKernel::eval(int x,int y,int t){
  double d= (x*x)/sigmaX + (y*y)/sigmaY + (t*t)/sigmaZ;
  d/=2.0;
  d*= -1.0;
  double C=1.0/(sqrt( 2*M_PI*M_PI*M_PI * sigmaX*sigmaY*sigmaZ));
  return exp(C*d);
}

double weightedSum(int t0,int x0,int y0,ActionArray * action,Kernel * kernel){
  if(x0==0 || y0==0 || t0==0){
    return 0;
  }
  if((t0>=action->frames-1) || (x0>=action->rows-1) || (y0>=action->cols-1)){
    return 0;
  }
  if(action->data[t0][x0][y0]==0){
    //cout << "\n" << t0 << " " << x0 << " " << y0;
	return 0;
  }
  double sum=0.0;
  double C=1.0/27.0;
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      for(int k=0;k<3;k++){
        int t_i=t0+i-1;
		int x_j=x0+j-1;
		int y_k=y0+k-1;
		//cout << "\n" << action->cols << " " << action->rows << " " <<action->frames;
		//cout << "\n" << t_i << " " << x_j << " " << y_k;
		//cout << "\n" << t0 << " " << x0 << " " << y0;

		sum+=C*action->data[t_i][x_j][y_k];
      }
    }
  }
  //cout << sum <<" ";
  return sum;
}


double timeSum(int t0,int x0,int y0,ActionArray * action,Kernel * kernel){
  if(t0<2){
    return 0;
  }
  if(t0>=action->frames-2){
    return 0;
  }
  if(action->data[t0][x0][y0]==0){
    //cout << "\n" << t0 << " " << x0 << " " << y0;
	return 0;
  }
  double sum=0.0;
  double C=1.0/5.0;
  for(int i=0;i<5;i++){
     int t_i=t0+i-2;
	 sum+=C*action->data[t_i][x0][y0];
  }
  //cout << sum <<" ";
  return sum;
}
