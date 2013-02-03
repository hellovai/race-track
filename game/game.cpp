#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "game.h"

using namespace std;

Game::Game( string filename ) {
	string name = filename;
	name.append(".dat");
	ifstream fin (name.c_str());
	if(fin.is_open()) {
		fin>>height>>width;
		string temp;
		getline(fin, temp);
		track = new int*[width];
		for(int i=0; i<width; i++) {
			track[i] = new int[height];
			getline(fin, temp);
			for( int j=0; j<height; j++) {
				track[i][j] = fromGUI(temp[j]);
				if(track[i][j] == 1) {
					Coor temp;
					temp.x = i;
					temp.y = j;
					start.push_back(temp);
				}
			}
		}
		Reset();
	} else {
		cout<<"Unable to open "<<name<<endl;
		exit(0);
	}
}

void Game::Reset() {
	coor = start[rand() % start.size()];
	reward = 0;
	uVel = 0;
	rVel = 0;
	status = true;
}

void Game::Print() {
	for(int i=0; i<width; i++) {
		for( int j=0; j<height; j++) {
			cout<<((coor.x == i && coor.y == j) ? 'C' : gui(track[i][j]));
		}
		cout<<endl;
	}
	cout<<"Up: "<<uVel<<" ::: Right: "<<rVel<<endl;
}

int Game::Move(int up, int right) {
	uVel += up;
	rVel += right;
	int res = processMove();
	//always lose 1 for making a move
	reward -= 1;
	//lose additional 5 for crashing
	if(res == -5)
		reward += res;
	
	if(res == 2)
		status = false;
	cout<<"Move Result: "<<res<<endl;
	return res;
}

int Game::processMove() {
	coor.y += rVel;
	coor.x -= uVel;
	
	bool crash = false;
	if(coor.x < 0) {
		coor.x = 0;
		crash = true;
	} else if(coor.x >= width) {
		coor.x = width-1;
		crash = true;
	}
	if(coor.y < 0) {
		coor.y = 0;
		crash = true;
	} else if (coor.y >= height) {
		coor.y = height - 1;
		crash = true;
	}
	
	//bring it back on the road if it drove off
	if(track[coor.x][coor.y] == -5) {
		int tempx = coor.x, tempy = coor.y;
		if(uVel > 0)
			while(track[tempx][tempy] == -5)
				tempx++;
		if(uVel < 0)
			while(track[tempx][tempy] == -5)
				tempx--;
		if(rVel > 0)
			while(track[tempx][tempy] == -5)
				tempy--;
		if(rVel < 0)
			while(track[tempx][tempy] == -5)
				tempy++;
		coor.x = tempx;
		coor.y = tempy;
		crash = true;
	}
	
	//This means we got the end
	if(track[coor.x][coor.y] == 2)
		crash = false;
	
	if(crash)
		return -5;
	
	return track[coor.x][coor.y];
}

bool Game::Status() {
	return status;
}

int Game::Reward() {
	return reward;
}

char Game::gui(int x) {
	switch(x) {
		case 0:
			return '.';
		case 1:
			return 'S';
		case 2:
			return 'F';
		case -5:
		default:
			return '|';
	}
}

int Game::fromGUI(char x) {
	switch (x) {
		case '.':
			return 0;
		case 'S':
			return 1;
		case 'F':
			return 2;
		case '|':
		default:
			return -5;
	}
}
