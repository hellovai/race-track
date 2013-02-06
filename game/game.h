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

class Game {
	int **track;
	int height;
	int width;
	bool status;
	int reward;
//keep track of possible starting points
	vector<Coor> start;

//Car's Position
	Coor coor;

//Velocity tracking
	int uVel;
	int rVel;

//keep track of all the moves the user makes
	vector<Identity> moveList;

//private functions
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
        void halfmove();
        
        //return functions
		bool Status() { return status; };
		int Reward() { return reward; };
		Coor Position() { return coor; }
		Vel Velocity() {Vel temp; temp.up = uVel; temp.right = rVel; return temp; };
		int Height() { return height; }
		int Width() { return width; }
		vector<Identity> Moves() { return moveList; }
};

//=================================
// end guard
#endif
//=================================
