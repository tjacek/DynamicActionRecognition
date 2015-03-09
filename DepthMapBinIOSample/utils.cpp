#include "StdAfx.h"
#include "utils.h"

double mean(vector<double> data){
  double avg=0.0;
  vector<double>::iterator it ;
  double n= (data.size()-1);
  for (it = data.begin(); it != data.end(); it++){
      float x= *it;
	  avg+=x;
  }
  avg /=n;
  return avg;
}

double standardDeviation(vector<double> data){
  vector<double>::iterator it ;
  double avg=mean(data);
  double sd=0.0;
  double n= (data.size()-1);
  for(it = data.begin(); it != data.end(); it++){
     double error=avg - *it;
	 error=(error*error)/n;
	 sd+=error;
  }
  return sqrt( sd);
}