#include "mazeGen.hpp"
#include "BFS.hpp"
//#include "BFS_noshuffle.hpp"
#include "DFS.hpp"
#include <chrono>
#include "constant.hpp"
using namespace std;

int main(int argc, char* argv[]){
    cout << "맵 크기 : 높이-"<<HEIGHT<<" / 너비-"<<WIDTH<<endl;
    
    chrono::system_clock::time_point start;
    chrono::duration<double> sec;

    MazeComeOn m;   
    //start = chrono::system_clock::now();
	m.makeMaze();
    //sec = chrono::system_clock::now() - start;
    //m.printMap();
    //cout << "맵 만드는데 걸린시간: " << sec.count() << " seconds" << endl;
    //cout << "맵에서 길의 개수: " << m.countWalking() << endl;

    
    start = chrono::system_clock::now();
    long long total = 0;
    const int TRY = 100;
    for(int i = 0 ; i< TRY; i++){
        DFS dfs((int (*)[WIDTH])m.spitMap());
	    dfs.searchDFS();
        total += dfs.countWalking();
    }
    sec = chrono::system_clock::now() - start;
    //DFS 평균 탐색 시간
    cout << "DFS: " << sec.count() / TRY << " seconds, ";
    //DFS 탐색 길이`
    cout << total / TRY << "개 방문 "<< endl;
    //dfs.printMap();

    
    total = 0;
    start = chrono::system_clock::now();
    for(int i = 0 ; i< TRY; i++) {
        BFS bfs((int(*)[WIDTH])m.spitMap());
        bfs.searchBFS();
        total += bfs.countWalking();
    }
    sec = chrono::system_clock::now() - start;

    //BFS 평균 탐색 시간
    cout << "BFS: " << sec.count() / TRY << " seconds, ";
    //BFS 탐색 길이
    cout <<  total / TRY << "개 방문"<< endl;
    //bfs.printMap();

    cout << "--------------------------------------------" << endl;

	return 0;
}

