#include "StdAfx.h"
#include "Difference.h"


/*
void differenceOfGaussian3D(Action * action,bool filter){
  ActionArray background(action);
  //Kernel * exp=new ExpKernel(1.0,1.0,1.0);
  ActionArray * orginal=toActionArray(action);
  background.convol(NULL,orginal);
  for(int i=0;i<action->size();i++){
	CDepthMap* dmap=action->at(i);
	for(int j=0;j<dmap->GetNRows();j++){
	  for(int k=0;k<dmap->GetNCols();k++){
        float orginal=dmap->GetItem(j,k);
		float val= orginal - background.data[i][j][k];
		if(orginal!=0){
		 // cout << val <<" " << dmap->GetItem(j,k) << " " << background.data[i][j][k]  <<"\n";
		}
		if(val > 30.0){
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



float getValue(int x,int y,int t,Action * action){
  CDepthMap *map =action->at(t);
  return map->GetItem(x,y);
}

ActionSummarry:: ActionSummarry(Action * action){
  CDepthMap *map =action->at(0);
  int rows=map->GetNRows();
  int cols=map->GetNCols();
  variance.SetSize(rows,cols);
  mean.SetSize(rows,cols);
  int n=action->size();
  float n_f=n;
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      float sum=0.0;
      for(int k=0;k<n;k++){
	    sum+=getValue(i,j,k,action);
      }
	  sum/=n_f;
	  //if(sum>10000){
	//	 cout << sum <<"\n";
	 // }
	  if(sum>0){
	    mean.SetItem(i,j,sum);
	  }else{
		mean.SetItem(i,j,0);
	  }
	}
  }

  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      float sd=0;
      for(int k=0;k<n;k++){
		  if(getValue(i,j,k,action)>0){
			//  cout << mean.GetItem(i,j);
		 // }
		   double diff =mean.GetItem(i,j) - getValue(i,j,k,action); 
		   sd+=diff*diff;
		  }
      }
	  sd/=(n_f-1.0);
	  variance.SetItem(i,j,sqrt(sd));
	}
  }
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
*/