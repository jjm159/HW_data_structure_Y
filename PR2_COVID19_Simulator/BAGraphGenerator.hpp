/*
	c++11 이상 요구.
	Barabasi-Albert 모델을 베이스로 그래프를 생성합니다. 
	객체 생성시 파일이름을 주면, 그래프를 생성하고 해당 이름으로 파일을 생성합니다.
	만약 해당 이름과 같은 파일이 존재하면 그 파일의 그래프를 로드해서 그대로 사용합니다.

*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <numeric>
#include <utility>

#include "config.hpp"

using namespace std;


class BAGraph{
	WeightType ba_graph[MAX_NODE_SIZE][MAX_NODE_SIZE]{0,}; // 된다. 
	random_device rnd;
	mt19937 rndGen;
	vector<int> targetID;
// 0 <-> 1 간선 추가. 0과 1을 이 집합에 넣어야 한다. 
	vector<short> accumulatedSet {0,1};
	string fileName;
public:
// 파일이름을 생성자에 주지 않으면 디폴트 이름으로 그래프를 파일로 생성하고 그걸 로드해버림. ( 최초 실행 )
	BAGraph(const string& fn = "graph.txt"):fileName(fn) {
		if(loadFile()){ // 읽기 성공하면
			return;
		}// 읽기 실패하면, 그 파일 이름을 가진 파일이 없다는 것. 새로 생성. 	
		ba_graph[0][1] = ba_graph[1][0] = 1; // 0과 1을 연결. 		
		targetID.reserve(2);
		rndGen.seed(rnd());
		generate();
		printFile();
	}
	void generate() {
		for(int add_node_index = 2; add_node_index < MAX_NODE_SIZE; add_node_index++){
			uniform_int_distribution<int> dis(0,accumulatedSet.size()-1);
			while(targetID[0] == targetID[1]) {		// 연결될 노드 두 개를 뽑기.
				targetID[0] = accumulatedSet[dis(rndGen)];
				targetID[1] = accumulatedSet[dis(rndGen)];
			}	
	
			accumulatedSet.push_back(targetID[0]); accumulatedSet.push_back(add_node_index);	// targetID[0]와 add_node_index 간선의 양 끝점 추가. 
			accumulatedSet.push_back(targetID[1]); accumulatedSet.push_back(add_node_index); 	// targetID[1]와 add_node_index 간선의 양 끝점 추가. 

			ba_graph[add_node_index][targetID[0]] = 1;	ba_graph[targetID[0]][add_node_index] = 1;	// 인접여부 기록. 
			ba_graph[add_node_index][targetID[1]] = 1;	ba_graph[targetID[1]][add_node_index] = 1;

			targetID[0] = targetID[1] = 0; // 그 다음 for 턴에서 while문이 돌게 하기 위해 둘이 같도록 초기화
		}
	}
	void printFile() { 
		ofstream out(fileName);		
		if(!out.is_open()) cout << "파일 생성 실패" << endl;
		else{
			for(int i=0;i<MAX_NODE_SIZE; i++)
				for(int j=0;j<MAX_NODE_SIZE; j++)
					out << ba_graph[i][j];
			out.close();
		}	
	}
	bool loadFile(){ // 생성자에 파일 이름을 줄 때만 호출. 생성자에서. 
		ifstream in(fileName, ios::binary);
		if(!in.is_open()) {
			cout << "파일 읽기 실패, 없어서 새로 만듭니다!" << endl;
			return false;
		}
		else {
			char temp[1];
			for(int i=0;i<MAX_NODE_SIZE; i++)
				for(int j=0;j<MAX_NODE_SIZE; j++){
					in.read(temp, sizeof(char));
					ba_graph[i][j] = (WeightType)atoi(temp);
				}
			in.close();
		}
		return true;
	}

// 연결이 원하는대로 잘되었는지 테스트하기 위한.
	void printTest() {
		vector<short> count;
		count.resize(MAX_NODE_SIZE,0);
		for(int i = 0; i< MAX_NODE_SIZE ;i ++){
			for(int j = 0; j< MAX_NODE_SIZE ; j++){
				count[i] += ba_graph[i][j];
			}
		}
		sort(count.begin(), count.end());

		int tippingPoint = 0;
		int c = 0;
		for(int i = 0; i<MAX_NODE_SIZE; i++){
			if(tippingPoint != count[i]) { // 달라져?
				cout << '(' << i << ',' << count[i] << ')' <<' ';
				tippingPoint = count[i];
				c++;
			}
		}
		cout << '\n' << c << endl;
	}

	void* getGraphPointer() { return ba_graph;} // 그래프의 주소반환. node 클래스에서 줍줍할 거임. 

	LinkCount getNodeDegree(int id) { // 원하는 node의 차수를 반환.
		int degree = 0;
		for(int i = 0; i< MAX_NODE_SIZE ;i ++){
			degree += ba_graph[id][i];
		}
		return degree;
	}

// 만들어진 그래프의 링크수가 변할 때 그 링크수를 벡터에 저장하였다가 반환
	vector<int> getHubBoundary () { 
		vector<short> count;
		count.resize(MAX_NODE_SIZE,0);
		vector<int> hub_boundary;
		
		for(int i = 0; i< MAX_NODE_SIZE ;i ++)
			count[i] += getNodeDegree(i);
		sort(count.begin(), count.end());

		int tippingPoint = 0, c = 0;
		for(int i = 0; i<MAX_NODE_SIZE; i++){
			if(tippingPoint != count[i]) { // 달라져?
				hub_boundary.push_back(count[i]);
				tippingPoint = count[i];
				c++;
			}
		}
		return hub_boundary;
	}
// 만들어진 그래프의 링크수가 변할 때 그 때까지의 노드의 개수를 벡터에 저장, 그리고 반환.
	vector<int> getUnderTippingPoint () {
		vector<short> count;
		count.resize(MAX_NODE_SIZE,0);
		vector<int> hub_boundary;
		
		for(int i = 0; i< MAX_NODE_SIZE ;i ++)
			count[i] += getNodeDegree(i);
		sort(count.begin(), count.end());

		int tippingPoint = 0, c = 0;
		for(int i = 0; i<MAX_NODE_SIZE; i++){
			if(tippingPoint != count[i]) { // 달라져?
				hub_boundary.push_back(i);
				tippingPoint = count[i];
				c++;
			}
		}
		return hub_boundary;
	}
// 링크수가 변할때까지의 노드 개수 반환
	vector<int> getHubCount () {
		vector<short> count;
		count.resize(MAX_NODE_SIZE,0);
		vector<int> hub_boundary;
		
		for(int i = 0; i< MAX_NODE_SIZE ;i ++)
			count[i] += getNodeDegree(i);
		sort(count.begin(), count.end());

		int tippingPoint = 0, c = 0;
		for(int i = 0; i<MAX_NODE_SIZE; i++){
			if(tippingPoint != count[i]) { // 달라져?
				hub_boundary.push_back(i);
				tippingPoint = count[i];
				c++;
			}
		}
		hub_boundary.push_back(1000);
		for(int i=0;i<hub_boundary.size()-1; i++){
			hub_boundary[i] = hub_boundary[i+1] - hub_boundary[i];
		}
		hub_boundary.pop_back();
		return hub_boundary;
	}

};


	
