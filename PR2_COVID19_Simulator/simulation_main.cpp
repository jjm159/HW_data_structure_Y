#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "Simulator.hpp"
#include "config.hpp"

void random_test(int);

int main(int argc, char* argv[]){

	// BA그래프 생성. 생성된 그래프는 파일로 출력. 
	// 만약 그래프 생성시 파일이 이미 존재한다면 그 파일을 load해옴.
	BAGraph *g = new BAGraph(); 
	vector<int> hub_boundary(g->getHubBoundary()); // 만들어진 그래프가 
	vector<int> hub_node_count(g->getHubCount()); 
	vector<int> under_tipping_point_count(g->getUnderTippingPoint());  // 
	delete g;

	cout << "밀집지역에서는 마스크를 쓰고, 희박지역에서는 마스크를 쓰지 않습니다." << endl;
	cout << "-------------------------------------------------------------------" << endl;
	for(int i=0;i<hub_boundary.size(); i++){
		// tipping Point는 허브가 가진 링크의 수. 
		cout << "Tipping Point: " << hub_boundary[i] << ", " << "Node Size: " << hub_node_count[i] << endl;
		cout << "마스크를 쓴 사람(=밀집지역이라고 지정된 장소)의 수: " << 1000 - under_tipping_point_count[i] << endl;
		cout << "마스크를 쓰지 않은 사람(=희박지역이라고 지정된 장소)의 수: " << under_tipping_point_count[i] << endl;
		random_test(hub_boundary[i]); // 초기 감염자의 발생지가 랜덤.
		cout << "-------------------------------------------------------------------" << endl;
	}
	return 0;
}

void random_test(int pointOfWearingMask){
	int p = TIPPING_POINT_HUB + pointOfWearingMask;
	Output sumResult;
	Output avrgResult;
	for(int i=0;i<TRY_TEST;i++){
		Simulator s(RANDOM_INFECTED, 0); 
		s.simulate(p);
		sumResult += Output(s.outResult());
	}
	avrgResult = sumResult / 100;
	avrgResult.print(); // 평균 수치들 출력. 
}

