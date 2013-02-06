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
		void Log( string filename );		
		void Change_game ( Game* curr_game ) { game = curr_game; };
};

//=================================
// end guard
#endif
//=================================
