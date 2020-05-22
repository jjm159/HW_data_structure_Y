#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <random>
#include <deque>
#include <algorithm> 
#include <iterator>
#include "constant.hpp"

using namespace std;
	
// s = (0, 0) , d = (WIDTH-1, HEIGHT-1)
class MazeComeOn {
	int mazeMap[HEIGHT][WIDTH] = {0,}; // 0은 벽이고 1은 길이다. 
	deque<tuple<int,int> > skein; // 돌아가기 위한 실타래.
	// DFS 면 stack -> push_back pop_back()
	// BFS 면 queue -> push_front pop_back()
public:
	MazeComeOn() { mazeMap[HEIGHT-1][WIDTH-1] = 1; }
	int isRoute(const tuple<int,int>& dest) const { return mazeMap[get<1>(dest)][get<0>(dest)]; }
// 검사 - 갈수 있는곳 식별 - 랜덤으로 다음 목적지 선택.
	bool indexCheck(const tuple<int,int> &dest) const {
		auto [destX, destY] = dest;
		return destX < WIDTH && destY < HEIGHT 
			&& destX >=0 && destY >= 0;
	}
	bool blockCheck(const tuple<int,int>& dest) const {
		auto [x,y] = dest;
		auto lu = make_tuple(x - 1, y - 1 ),  mu = make_tuple(x , y - 1 ), ru = make_tuple(x + 1, y - 1 );
		auto lm = make_tuple(x - 1, y ), rm = make_tuple(x + 1, y );
		auto ld = make_tuple(x - 1, y + 1 ),  md = make_tuple(x,  y + 1 ), rd = make_tuple(x + 1, y + 1 );
		if(indexCheck(lu) && indexCheck(mu) && indexCheck(lm)){
			if(isRoute(lu) + isRoute(mu) + isRoute(lm) + 1 == 4) return false;
		}
		if(indexCheck(mu) && indexCheck(ru) && indexCheck(rm)){
			if(isRoute(mu) + isRoute(ru) + isRoute(rm) + 1 == 4) return false;
		}
		if(indexCheck(lm) && indexCheck(ld) && indexCheck(md)){
			if(isRoute(lm) + isRoute(ld) + isRoute(md) + 1 == 4) return false;
		}
		if(indexCheck(rm) && indexCheck(md) && indexCheck(rd)){
			if(isRoute(rm) + isRoute(md) + isRoute(rd) + 1 == 4) return false;
		}
		return true;
	}
	bool isSafeRoute(const tuple<int,int>& dest) const {
		return indexCheck(dest) && !isRoute(dest) && blockCheck(dest) ;
	}
	void makeMaze() {  // route inspector
		skein.push_back(make_tuple(0,0));
		mazeMap[0][0] = ROUTE;
		random_device rd;
		while(!skein.empty()) {
			auto [x,y] = skein.back();
			//skein.pop_back();
			mazeMap[y][x] = ROUTE;

			vector<tuple<int,int>> direction = { make_tuple(x, y-1), make_tuple(x, y+1), make_tuple(x-1, y), make_tuple(x+1, y)};
			shuffle(direction.begin(), direction.end(), default_random_engine((rd())));
			int i;
			for( i = 0; i< 4; i++) {
				if(isSafeRoute(direction[i])) {
					//tie(x,y) = direction[i];
					skein.push_back(direction[i]); // 실타래 풀어놓기.
					break;
				} 
			}
			if(i == 4) skein.pop_back();
		}
		// 갈수있는 곳 다 감.	
	}
	void* spitMap() { return mazeMap; }
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
		return '+';
	}
	int countWalking() {
		int count = 0;
		for(int y = 0 ; y< HEIGHT; y++ ){
            for(int x = 0; x<WIDTH; x++){
                if(mazeMap[y][x] == ROUTE) count++;
            }
        }
		return count;
	}
};


