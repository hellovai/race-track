#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "main.h"
#include "game.h"
#include "reinforce.h"

using namespace std;

int main(int argc, char* argv[]) {	
	srand ( time(NULL) );
	string filestart = "race";
	int epsilon = 10;
	if(argc == 2)
		filestart = argv[1];
	if(argc == 3)
		epsilon = atoi(argv[2]);
	
	Game game(filestart);
	Reinforce agent(&game, epsilon);
	
	int gameCounter = 1000;
	
	for(int i=0; i<gameCounter; i++) {
		game.Reset();
		//cout<<"Initialized!"<<endl;
		int up_vel, right_vel;
		while(game.Status()) {
		    //game.Print();
		    //sleep(1);
		    game.halfmove();
			//game.Print();
			//sleep(1);
			Vel temp = agent.Move();
			game.Move(temp.up, temp.right);
		}
		//game.Print();
		//cout<<"Congrats! Score: "<<game.Reward()<<endl;
		agent.Update();
		agent.Epsilon(gameCounter);
		agent.Log(filestart);
	}
	return 0;
}
