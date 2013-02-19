#include <stdlib.h>
#include <cstdlib>
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

void simulate(Reinforce* agent, int counter);
void store(string filename,int counter,  int game, int reward);

int simulateGames = 1000;
bool halfstep = true;
string filestart;
bool debug = false;

int main(int argc, char* argv[]) {	
	
	srand ( time(NULL) );
	
	int gameCounter = 1000;
	double epsilon = 0.0;
	double qVal = 0.0;
	int GAMEPRINT = 5;
	double lambda = 1;
	
	int argCounter = 1;  //to account for starting filename that it counts
	argc -= 1;
	while(argc > 0) {
		bool valid = true;
		string key = argv[argCounter++];
		argc--;
		if ( key.compare("-off") != 0 && key.compare("-debug") != 0 && argc == 0 )
			improper(key);
		
		if ( key.compare("-f") == 0 )
			filestart=argv[argCounter++];
		else if ( key.compare("-e") == 0 )
			epsilon = atof(argv[argCounter++]);
		else if ( key.compare("-q") == 0 )
			qVal = atof(argv[argCounter++]);
		else if ( key.compare("-p") == 0 )
			GAMEPRINT = atoi(argv[argCounter++]);
		else if ( key.compare("-g") == 0 )
			gameCounter = atoi(argv[argCounter++]);
		else if ( key.compare("-l") == 0 )
			lambda = atof(argv[argCounter++]);
		else if ( key.compare("-off") == 0 ) {
			argc++;
			halfstep = false;
		} else if ( key.compare("-debug") == 0 ) {
			argc++;
			debug = true;
			cout<<"Entering Debug mode..."<<endl;
			cout<<"No files will be created and saved!"<<endl;
		} else 
			improper(key);

		argc--;		
	}
	
	if(filestart.compare("") == 0){
		cout<<"No input file..."<<endl;
		improper("-h");
	}
	
	epsilon = max(epsilon, 100.0);
	epsilon = min(epsilon, 0.0);
	
	if(debug) {
		cout<<"------------------------"<<endl;
		cout<<"Configuration"<<endl;
		cout<<"------------------------"<<endl;
		cout<<"File:\t\t"<<filestart<<".dat"<<endl;
		cout<<"Episodes:\t"<<gameCounter<<endl;
		cout<<"Initial Reward:\t"<<qVal<<endl;
		cout<<"Epsilon:\t"<<epsilon<<endl;
		cout<<"lambda:\t\t"<<lambda<<endl;
		cout<<"Halfstep:\t"<<(halfstep ? "Yes" : "No")<<endl;
		cout<<"Press Enter to continue: ";
		cin.ignore();
	}
	
	Game game(filestart);
	Reinforce agent(&game, epsilon, qVal);
	
	agent.setDebug(debug);
	agent.setLambda(lambda);
	simulate(&agent, 0);
	agent.Change_game(&game);
	
	for(int i=0; i<gameCounter; i++) {
		game.Reset();
		while(game.Status()) {
            bool print = false;
            if(debug == true || (i<GAMEPRINT || i>gameCounter-GAMEPRINT))
            	print = true;
            	
            if(print) {
                game.Print();
                cout<<"Episode: "<<i<<endl;
                sleep(1);
            }
            if(halfstep) {
				game.halfmove();
		        if(print) {
		            game.Print();
		            cout<<"Half-step!"<<endl;
                	cout<<"Episode: "<<i<<endl;
		            if(!debug) sleep(1);
		        }
            }
			Vel temp = agent.Move();
			game.Move(temp.up, temp.right);
			if(debug) {
				cout<<"Press enter to continue: ";
				cin.ignore();
			}
		}
		agent.Update();
		agent.Epsilon(i);
		if(debug) {
			cout<<"Press enter to continue: ";
			cin.ignore();
		}
		if(i % 10 == 9) {
			simulate(&agent, i);
			agent.Change_game(&game);
		}
	}
	return 0;
}

void simulate(Reinforce* agent, int counter) {
	Game temp(filestart);
	agent->Change_game(&temp);
	agent->setDebug(false);
	double ep = agent->ep();
	vector<int> rewards;
	for( int i=0; i<simulateGames; i++ ) {
		temp.Reset();
		while(temp.Status()) {
			if(halfstep)
				temp.halfmove();
			Vel velo = agent->Move();
			temp.Move(velo.up, velo.right );
		}
		rewards.push_back(temp.Reward());
		if(!debug) store(filestart, counter, i, temp.Reward() );
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
	if(!debug) {
	//	agent->Log(filestart, counter, m, mini, maxi, std, ep);
		agent->Log(filestart, counter, rewards, mini, maxi, m);
	}
	agent->setDebug(debug);
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

void improper(string key) {
	if(key.compare("-h") != 0)
		cout<<"Improper usage of "<<key<<endl;
	cout<<"Usage: ./race-track -f filename-without-dat"<<endl;
	cout<<"\t\t[-g episode] [-p games-to-print]"<<endl;
	cout<<"\t\t[-e epsilon] [-q reward]"<<endl;
	cout<<"\t\t[-off] [-debug]"<<endl;
	cout<<"./race-track -h displays usage"<<endl;
	cout<<"View README for more info"<<endl;
	exit(0);
}
