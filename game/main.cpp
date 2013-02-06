#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "game.h"

using namespace std;

int main(int argc, char* argv[]) {	
	srand ( time(NULL) );
	string filestart = "race";
	if(argc == 2)
		filestart = argv[1];
		
	Game game(filestart);
	cout<<"Initialized!"<<endl;
	int up_vel, right_vel;
	while(game.Status()) {
        game.Print();
        sleep(1);
        game.halfmove();
		game.Print();
		cout<<"Up Velocity: ";
		cin>>up_vel;
		cout<<"Right Velocity: ";
		cin>>right_vel;
		game.Move(up_vel, right_vel);
	}
    game.Print();
	cout<<"Congrats! Score: "<<game.Reward()<<endl;
	return 0;
}
