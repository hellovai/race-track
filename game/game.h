//=================================
// include guard
#ifndef __GAME_H_INCLUDED__
#define __GAME_H_INCLUDED__
//=================================

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef struct {
	int x;
	int y;
} Coor;

class Game {
	int **track;
	int height;
	int width;
	bool status;
//keep track of possible starting points
	vector<Coor> start;
	
	int reward;
//Car's Position
	Coor coor;
	
//Velocity tracking
	int uVel;
	int rVel;
	
	char gui(int x);
	int fromGUI(char x);
	int processMove();
	bool fixCrash(int uMove, int rMove);
	
	public:
		Game(string name);
		int Move(int up, int right);
		void Print();
		void Reset();
		bool checkFinish();
		bool Status() { return status; };
		int Reward() { return reward; };
		Coor Position() { return coor; }
};

//=================================
// end guard
#endif
//=================================
