#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

#include "main.h"
#include "game.h"
#include "reinforce.h"

using namespace std;

Reinforce::Reinforce( Game* curr_game, double epsilon, double qVal ) {
	game = curr_game;
	e = epsilon;
	debug = false;
	lambda = 1;
	
	int width = curr_game->Width();
	int height = curr_game->Height();
	int vel = VELMAX - VELMIN + 1;
	//initialize rewards to 0
	reward = new double***[width];
	for( int i=0; i<width; i++ ) {
		reward[i] = new double**[height];
		for( int j=0; j<height; j++ ) {
			reward[i][j] = new double*[vel];
			for( int k=0; k<vel; k++ ) {
				reward[i][j][k] = new double[vel];
				for ( int l=0; l<vel; l++ )
					reward[i][j][k][l] = qVal;
			}
		}
	}
	visits = new int***[width];
	for( int i=0; i<width; i++ ) {
		visits[i] = new int**[height];
		for( int j=0; j<height; j++ ) {
			visits[i][j] = new int*[vel];
			for( int k=0; k<vel; k++ ) {
				visits[i][j][k] = new int[vel];
				for ( int l=0; l<vel; l++ )
					visits[i][j][k][l] = 0;
			}
		}
	}
}

//log for arrays
void Reinforce::Log( string filename, int i, double mean, int mini, int maxi, double std, double epsi ) {
	filename.append(".log");
	ofstream myfile;
	myfile.open(filename.c_str(), ios::app);
	myfile<<i<<" "<<mean<<" "<<mini<<" "<<maxi<<" "<<mean-std<<" "<<mean+std<<" "<<epsi<<endl;
	myfile.close();
}

//log for box plot
void Reinforce::Log( string filename, int i, vector<int> rewardList, int mini, int maxi, double mean) {	
	sort(rewardList.begin(), rewardList.end());
	double q1, q2, q3, box_min, box_max;
	q2 = get_median(rewardList);
	
	vector<int> top, bottom;
	for(int in=0; in<(int) rewardList.size(); in++) {
		if(in < (int) rewardList.size()/2)
			bottom.push_back(rewardList[in]);
		else if( in >(int)  rewardList.size()/2)
			top.push_back(rewardList[in]);
		else if( rewardList.size() % 2 == 0) // if i is middle value is implied already
			bottom.push_back(rewardList[in]);
	}
	
	q1 = get_median(bottom);
	q3 = get_median(top);
	
	box_min = min(q1 - 1.5*(q1 - q3), (double) maxi);
	box_max = max(q3 + 1.5*(q1 - q3), (double) mini);
	
	filename.append(".log");
	ofstream myfile;
	myfile.open(filename.c_str(), ios::app);
	myfile<<i<<" "<<mini<<" "<<box_max<<" "<<q1<<" "<<q2<<" "<<q3<<" "<<box_min<<" "<<maxi<<" "<<mean<<endl;
	myfile.close();
}

void Reinforce::Update() {
	vector<Identity> moveList = game->Moves();
	int total_reward = game->Reward();
	
	for(int i=0; i<(int) moveList.size(); i++ ) {
		double sum = 0;
		int count = 1;
		if(debug) cout<<"sum = ( 0";
		for(int j=0; j<(int) moveList.size()-1-i; j++) {
			sum += pow(lambda, count-1)*get_reward(i, count);
			if(debug) cout<<"+ pow("<<lambda<<", "<<count-1<<") * R_["<<i<<"]("<<count<<") ";
			count++;
		}
		sum *= (1-lambda);
		if(debug) cout<<") * ("<<1-lambda<<") ";
		
			Identity item = moveList[i];
			item.vel.up -=  VELMIN;
			item.vel.right -=  VELMIN;
			double* curr_r = &reward[item.loc.x][item.loc.y][item.vel.up][item.vel.right];
			int* curr_v = &visits[item.loc.x][item.loc.y][item.vel.up][item.vel.right];

		sum += pow(lambda, (int) moveList.size()-1-i)*get_reward(i, moveList.size());
		if(debug) cout<<"+ pow("<<lambda<<", "<<moveList.size()-1-i<<") * "<<get_reward(i, moveList.size())<<endl;
		if(debug) {
			cout<<"Updating..."<<endl;
			cout<<"\tPosition:\t"<<item.loc.x<<", "<<item.loc.y<<endl;
			cout<<"\tVelocity:\t"<<item.vel.up<<", "<<item.vel.right<<endl;
			cout<<"\tReward:\t"<<item.reward<<endl;
			cout<<"\tSum:\t\t"<<sum<<endl;
			cout<<"\tFrom:\t\t"<<(*curr_r)<<" Visited: "<<(*curr_v)<<endl;
		}
		visits[item.loc.x][item.loc.y][item.vel.up][item.vel.right] = (*curr_v)+1;
		reward[item.loc.x][item.loc.y][item.vel.up][item.vel.right] = (*curr_r) + 1/((double) (*curr_v))*(sum - (*curr_r));		
		if(debug) {
			cout<<"\tTo:\t\t"<<(*curr_r)<<" Visited: "<<(*curr_v)<<endl;
		}
	}
}

Vel Reinforce::Move( ) {
	double numb = rand() % 100;
	if( numb <= e )
		return Explore();
	else
		return Exploit();
}

void Reinforce::Epsilon( int gameCounter ) {
	if(gameCounter % 10 == 9)
		e*= 0.999;
	if(debug)
		cout<<"Epsilon is: "<<e<<endl;
}

void Reinforce::DumpPolicy() {
	int **upPref, **rightPref;
	upPref = new int*[game->Width()];
	rightPref = new int*[game->Width()];
	for( int x = 0; x < game->Width(); x++ ) {
		upPref[x] = new int[game->Height()];
		rightPref[x] = new int[game->Height()];
	for( int y = 0; y < game->Height(); y++ ) 
	{
		if(game->Track(x,y) != 0) {
			upPref[x][y] = -1;
			rightPref[x][y] = -1;
		} else {
			double **curr_reward = reward[x][y];
			double uMax[VELMAX-VELMIN+1] = {0};
			double rMax[VELMAX-VELMIN+1] = {0};
			for (int i = 0; i < VELMAX-VELMIN; ++i) {
				uMax[i] = curr_reward[i][0];
				rMax[i] = curr_reward[0][i];
				for (int j = 1; j < VELMAX-VELMIN; ++j)
				{
					if(uMax[i] <= curr_reward[i][j])
						uMax[i] = curr_reward[i][j];
					if(rMax[i] <= curr_reward[j][i])
						rMax[i] = curr_reward[j][i];
				}
			}
			vector<int> indexr, indexu;
			indexu.push_back(0);
			indexr.push_back(0);
			for (int i = 1; i < VELMAX-VELMIN; ++i)
			{
				if(rMax[indexr[0]] < rMax[i]) {
					indexr.clear();
					indexr.push_back(i);
				} else if (rMax[indexr[0]] == rMax[i]) {
					indexr.push_back(i);
				}
				if(uMax[indexu[0]] < uMax[i]) {
					indexu.clear();
					indexu.push_back(i);
				} else if (uMax[indexu[0]] == uMax[i]) {
					indexu.push_back(i);
				}
			}
			if(indexu.size() == 1)
			upPref[x][y] = indexu[0];
			else
			upPref[x][y] = -indexu.size();
			if(indexr.size() == 1)
			rightPref[x][y] = indexr[0];
			else
			rightPref[x][y] = -indexr.size();
		}
	}
	}
	

	cout<<"Up Policy"<<endl;
	for(int i=0; i < game->Width(); i++) {
		for( int j=0; j < game->Height(); j++) {
			if (upPref[i][j] == -1)
				cout<<"|";
			else if (upPref[i][j] < 0)
				cout<<"\033[1;31m"<<upPref[i][j]*-1<<"\033[0m";
			else 
				cout<<upPref[i][j];
			cout<<" ";
		}
		cout<<endl;
	}
	cout<<"Right Policy"<<endl;
	for(int i=0; i < game->Width(); i++) {
		for( int j=0; j < game->Height(); j++) {
			if (rightPref[i][j] == -1)
				cout<<"|";
			else if (rightPref[i][j] < 0)
				cout<<"\033[1;31m"<<rightPref[i][j]*-1<<"\033[0m";
			else 
				cout<<rightPref[i][j];
			cout<<" ";
		}
		cout<<endl;
	}
}

//Private Functions
Vel Reinforce::Exploit( ) {
	if(debug)
		cout<<"EXPLOTING!"<<endl;
	Coor car = game->Position();
	Vel vel = game->Velocity();
	Vel orig = vel;
	
	vel.up -= VELMIN;
	vel.right -= VELMIN;
	double **curr_reward = reward[car.x][car.y];
	int uStart = max(vel.up - 1, 0);
	int uEnd = min(vel.up + 1, VELMAX-VELMIN);
	int rStart = max(vel.right - 1, 0);
	int rEnd = min(vel.right + 1, VELMAX-VELMIN);
	
	vector<int> uMAX, rMAX;
	uMAX.push_back(uStart);
	rMAX.push_back(rStart);
	if(debug) {
		cout<<"Position: "<<car.x<<" "<<car.y<<endl;
		cout<<"Velocity: "<<vel.up<<" "<<vel.right<<endl;
		cout<<"U Min: "<<uStart - abs(VELMIN)<<endl;
		cout<<"U Max: "<<uEnd - abs(VELMIN)<<endl;
		cout<<"R Min: "<<rStart - abs(VELMIN)<<endl;
		cout<<"R Max: "<<rEnd - abs(VELMIN)<<endl;
	}
	double maxReward = curr_reward[uStart][rStart]; 
	for( int i=uStart; i<=uEnd; i++ )
		for( int j=rStart; j<=rEnd; j++ ) {
			if(debug) cout<<"\tReward: "<<i<<" "<<j<<", "<<curr_reward[i][j]<<endl;
			if(curr_reward[i][j] > maxReward) {
				maxReward = curr_reward[i][j];
				uMAX.clear();
				rMAX.clear();
				uMAX.push_back(i);
				rMAX.push_back(j);
			} else if (curr_reward[i][j] == maxReward) {
				uMAX.push_back(i);
				rMAX.push_back(j);
			}
	}
	
	if(debug) {
		cout<<"uMax ";
		for(int i =0; i<(int) uMAX.size(); i++)
			cout<<uMAX[i]<<" ";
			cout<<endl<<"rMax ";
		for(int i=0; i<(int) rMAX.size(); i++)
			cout<<rMAX[i]<<" ";
			cout<<endl;
	}
	
	vel.up = uMAX[rand() % uMAX.size()] + VELMIN - orig.up;
	vel.right = rMAX[rand() % rMAX.size()] + VELMIN - orig.right;
	
	if(vel.up < 0 ) vel.up = -1;
	else if(vel.up > 0 ) vel.up = 1;
	if(vel.right < 0 ) vel.right = -1;
	else if(vel.right > 0 ) vel.right = 1;
	
	if(debug) {
		cout<<"velocity: "<<vel.up<<" "<<vel.right<<endl;
		sleep(1);
	}
	
	return vel;
}

Vel Reinforce::Explore( ) {
	if(debug)
		cout<<"EXPLORING!"<<endl;
	Vel vel;
	vel.up = rand() % 3 - 1;
	vel.right = rand() % 3 - 1;
	if(debug)
	cout<<"Velocity: "<<vel.up<<" "<<vel.right<<endl;
	return vel;
}

double Reinforce::get_median( vector<int> array ) {
	double median_item;
	if(array.size() % 2 == 1)
		median_item = (double) array[array.size()/2 + 1];
	else
		median_item = ((double) array[array.size()/2] + (double) array[array.size()/2 - 1])/2.0;
	return median_item;
}

double Reinforce::get_reward(int time, int n) {
	vector<Identity> moves = game->Moves();
	int i = time;
	int sum = moves[i].reward; 
	for(i=i+1; i<n+time && i<(int) moves.size(); i++ ) {
		sum += pow(lambda, i) * moves[i].reward;
	}
	if(i<(int) moves.size()) {
		sum += pow(lambda, i) * get_reward(i, moves.size());
	}
	return (double) sum;
}
