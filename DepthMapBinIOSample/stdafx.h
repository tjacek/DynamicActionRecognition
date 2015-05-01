// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <math.h>
#define _USE_MATH_DEFINES
#pragma once

#include <cmath>
#include <tchar.h>
#include <cstdlib>
#include <vector>
#include <map>

#include <iostream>
#include <fstream>

#include <opencv\cv.h>
#include <opencv\ml.h>
#include <opencv\highgui.h>

using namespace std;

#define emptyPoint 0

extern string intToString(int i);
extern void saveToFile(string filename,string data);
extern string doubleToString(double raw);