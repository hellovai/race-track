//=================================
// include guard
#ifndef __MAIN_H_INCLUDED__
#define __MAIN_H_INCLUDED__
//=================================

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#define VELMAX 5
#define VELMIN -5

typedef struct {
	int x;
	int y;
} Coor;

typedef struct {
	int up;
	int right;
} Vel;

typedef struct {
	Coor loc;
	Vel vel;
	int reward;
} Identity;

//=================================
// end guard
#endif
//=================================
