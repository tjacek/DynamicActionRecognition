// stdafx.cpp : source file that includes just the standard includes
// DepthMapBinIOSample.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

string intToString(int i){
    string tmp; 
  sprintf((char*)tmp.c_str(), "%d", i);
 // string tmp2 = tmp.c_str();
  string * str=new string(tmp.c_str());
  return *str;
}

string doubleToString(double raw){
  string tmp; 
  sprintf((char*)tmp.c_str(), "%f", (float)raw);
  string * str=new string(tmp.c_str());
  return *str;
}

void saveToFile(string filename,string data){
 ofstream myfile;
  myfile.open(filename);
  myfile << data;
  myfile.close();
}