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
	double lambda;
	
	//reward list
	double ****reward;
	int ****visits;
	
	//private functions
	Vel Explore();
	Vel Exploit();
	
	double get_median(vector<int> array);
	bool debug;
	double get_reward(int , int);
	
	public:
		Reinforce( Game* curr_game, double epsilon, double qVal );
		Vel Move( );
		void Update( );
		void Epsilon( int gameCounter );
		
		//log min, avg, std, max
		void Log( string filename, int i, double mean, int mini, int maxi, double std, double epsi );	
		//log for box plot
		void Log( string filename, int i, vector<int> rewardList, int mini, int maxi, double mean );	
		
		void Change_game ( Game* curr_game ) { game = curr_game; };
		double ep() { return e; }
		void ChangeEpsilon( double x ) { e = x; }
		void setDebug(bool debg ) { debug = debg; }
		void setLambda(double number ) { lambda = number; }
		void DumpPolicy();
};

//=================================
// end guard
#endif
//=================================
