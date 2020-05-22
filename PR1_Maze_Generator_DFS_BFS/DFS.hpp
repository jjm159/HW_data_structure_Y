#include <iostream>
#include <random>
#include <deque>
#include <vector>
#include "constant.hpp"
using namespace std;

class DFS {
    int mazeMap[HEIGHT][WIDTH]; 
	deque<tuple<int,int> > skein;
public:
    DFS(int map[][WIDTH]) {
        for(int y = 0 ; y< HEIGHT; y++ ){
            for(int x = 0; x<WIDTH; x++){
                mazeMap[y][x] = map[y][x];
            }
        }
    }

    int isRoute(const tuple<int,int>& dest) const { return mazeMap[get<1>(dest)][get<0>(dest)]; }
// 검사 - 갈수 있는곳 식별 - 랜덤으로 다음 목적지 선택.
	bool indexCheck(const tuple<int,int> &dest) const {
		auto [destX, destY] = dest;
		return destX < WIDTH && destY < HEIGHT 
			&& destX >=0 && destY >= 0;
	}
// DFS 탐색
	void searchDFS() {
		skein.push_back(make_tuple(0,0)); // 시작점에 실풀기.
		
        random_device rd;
        //int loopCount = 0 ;
		while(!skein.empty()) {
            //loopCount++;
			auto [x,y] = skein.back();
            skein.pop_back();

            //if( (x == WIDTH/2) && (y == 0)) {
			if( (x == WIDTH-1) && (y == HEIGHT -1)) { 
                mazeMap[y][x] = EXIT;
                //cout << loopCount << "," << endl;
                return; // 목적지 도착!
			}

            if(mazeMap[y][x] == THREAD) continue;
			mazeMap[y][x] = THREAD; // 2는 들린곳.

			vector<tuple<int,int>> direction = { make_tuple(x+1, y), make_tuple(x, y-1), make_tuple(x, y+1), make_tuple(x-1, y)};
			shuffle(direction.begin(), direction.end(), default_random_engine((rd())));

			for(int i = 0; i< 4; i++) {
				if(indexCheck(direction[i]) && (isRoute(direction[i]) == ROUTE)) {
					skein.push_back(direction[i]);
				}
			}
		}
	}	
    void printMap(int option = 0){
		if( option == 0 ){
			for(int y = 0 ; y< HEIGHT; y++){
				for(int x = 0; x< WIDTH; x++) {
					cout << getImage(mazeMap[y][x]);
				}
				cout << endl;
			}
		}
		else {
			for(int y = 0 ; y< HEIGHT; y++){
				for(int x = 0; x< WIDTH; x++) {
					cout << mazeMap[y][x];
				}
				cout << endl;
			}	
		}
	}
	char getImage(int target){
		if(target == WALL) return ' ';
		else if (target == ROUTE ) return '+';
        else if (target == THREAD ) return '0';
        else if (target == EXIT) return 'E'; 
        else return '?';
	}
    void* spitMap() { return mazeMap; }

	int countWalking() {
		int count = 0;
		for(int y = 0 ; y< HEIGHT; y++ ){
            for(int x = 0; x<WIDTH; x++){
                if(mazeMap[y][x] == THREAD) count++;
            }
        }
		return count;
	}
};

