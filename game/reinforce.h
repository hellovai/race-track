//=================================
// include guard
#ifndef __REINFORCE_H_INCLUDED__
#define __REINFORCE_H_INCLUDED__
//=================================

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Reinforce {
	Game* game;
	int e;
	
	//reward list
	double ****reward;
	int ****visits;
	
	//private functions
	Vel Explore();
	Vel Exploit();
	
	public:
		Reinforce( Game* curr_game, int epsilon );
		Vel Move( );
		void Update( );
		void Epsilon( int gameCounter );
		void Log( string filename, int i, double mean, int mini, int maxi, double std );		
		void Change_game ( Game* curr_game ) { game = curr_game; };
		int ep() { return e; }
		void ChangeEpsilon( int x ) { e = x; }
};

//=================================
// end guard
#endif
//=================================
