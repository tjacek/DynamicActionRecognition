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