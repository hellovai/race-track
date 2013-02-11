#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <sstream>

#include "main.h"
#include "game.h"
#include "reinforce.h"

using namespace std;

void simulate(string filestart, Reinforce* agent, int counter);
void store(string filename,int counter,  int game, int reward);

int simulateGames = 1000;

int main(int argc, char* argv[]) {	
	srand ( time(NULL) );
	string filestart = "race";
	int gameCounter = 1000;
	int epsilon = 10;
	if(argc >= 2)
		filestart = argv[1];
	if(argc >= 3)
		epsilon = atoi(argv[2]);
	if(argc >= 4)
		gameCounter = atoi(argv[3]);
	
	Game game(filestart);
	Reinforce agent(&game, epsilon);
	
	simulate(filestart, &agent, 0);
	agent.Change_game(&game);
	
	for(int i=0; i<gameCounter; i++) {
		game.Reset();
		//cout<<"Game: "<<i<<endl;
		while(game.Status()) {
//		    game.Print();
//		    sleep(1);
		    game.halfmove();
//			game.Print();
//			sleep(1);
			Vel temp = agent.Move();
			game.Move(temp.up, temp.right);
//			cin.ignore();
		}
		//game.Print();
		//cout<<"Congrats! Score: "<<game.Reward()<<endl;
		agent.Update();
		agent.Epsilon(i);
		if(i % 10 == 9) {
			simulate(filestart, &agent, i);
			agent.Change_game(&game);
		}
	}
	return 0;
}

void simulate(string filestart, Reinforce* agent, int counter) {
	Game temp(filestart);
	agent->Change_game(&temp);
	double ep = agent->ep();
//	agent->ChangeEpsilon(0);
	vector<int> rewards;
	for( int i=0; i<simulateGames; i++ ) {
		temp.Reset();
		while(temp.Status()) {
			Vel velo = agent->Move();
			temp.Move(velo.up, velo.right );
		}
		rewards.push_back(temp.Reward());
		store(filestart, counter, i, temp.Reward() );
	}
	double sum = 0;
	double scale = 1;
	for( int i=0; i<(int) rewards.size(); i++ )
		sum+=rewards[i];
	double m = sum / rewards.size();
	double accum = 0.0;
	int mini = rewards[0];
	int maxi = rewards[0];
	for( int i=1; i<(int) rewards.size(); i++ ) {
		accum+=(rewards[i]-m)*(rewards[i]-m);
		if(rewards[i] < mini)
			mini = rewards[i];
		else if (rewards[i] > maxi)	
			maxi = rewards[i];
	}
	double std = sqrt(accum/rewards.size());
	
	cout<<"Counter: "<<counter<<endl;
	cout<<"Scale: "<<scale<<" Mean: "<<m<<" STD: "<<std<<endl;
	agent->Log(filestart, counter, m, mini, maxi, std, ep);
//	agent->ChangeEpsilon(ep);
}

void store(string filename, int counter, int game, int reward) {
	string filestore = "./log/";
	filestore.append(filename);
	filestore.append("_");
	ostringstream convert;
	convert<<counter;
	filestore.append(convert.str());
	filestore.append(".dat");
	ofstream myfile;
	myfile.open(filestore.c_str(), ios::app);
	myfile<<game<<" "<<reward<<endl;
	myfile.close();
}

