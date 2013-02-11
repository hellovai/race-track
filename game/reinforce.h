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
	double e;
	
	//reward list
	double ****reward;
	int ****visits;
	
	//private functions
	Vel Explore();
	Vel Exploit();
	
	public:
		Reinforce( Game* curr_game, double epsilon );
		Vel Move( );
		void Update( );
		void Epsilon( int gameCounter );
		void Log( string filename, int i, double mean, int mini, int maxi, double std, double epsi );		
		void Change_game ( Game* curr_game ) { game = curr_game; };
		double ep() { return e; }
		void ChangeEpsilon( double x ) { e = x; }
};

//=================================
// end guard
#endif
//=================================
