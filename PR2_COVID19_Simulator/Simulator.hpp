#include "BAGraphGenerator.hpp"
#include "Node.hpp"
#include "config.hpp"
#include "Output.hpp"
#include <algorithm> // sort
#include <random>

// 여러가지 상수들
// 확률 - 마스크를 안쓴 경우의 확률과 쓴 경우의 확률 필요.
// S->I 확률:0.02
// I->R 확률:1/15

// vector는 Node가 요소다. Node객체 만들때 graph로부터 차수를 받아온다. 
// 이후에 vector를 정렬한다. - 링크 수에 따라 다른 시도를 해보기 위해!  

// '시간'은 반복문의 한 turn이다. 이 한 턴에서 노드마다 각각의 행위를 하게 된다. 

// 실험에 필요한 조작을 이 Simulator CLASS에서 한다. 
// 실험의 진행
// 최초 감염 Node 설정. - 핵심.  - 무작위? 허브? 일반? 몇개나(이건 큰 의미 없었음.)?


class Simulator { // -> 사실상 그래프
    WeightType adjMat[MAX_NODE_SIZE][MAX_NODE_SIZE]{0,}; // '위치' 정보. 밀집 지역(허브)이냐 희박한 지역이냐
    vector<Node*> peapleNodePList; // node는 사람. 
    vector<Id> nextNodeList[MAX_NODE_SIZE]; // 연결 정보를 가지고 있음.  
    LinkCount linkCount[MAX_NODE_SIZE]; //'차수' 정보 가짐. 이걸로 허브인지 아닌지 알 수 있음. 
 
    TimeCount totalTime = 0;
    int susceptor = MAX_NODE_SIZE; // 취약자.
    int infector = 0; // 감염자수.
    int recoverer = 0; // 면역자수.

    random_device rnd;
	mt19937 rndGen;

// 얘를 생성자에서 만들어서, 가변성을 높이자. 조금만 조작해도 확률 바꿀 수 있게! 
    // 표본 집합 - chanceToInfector 1/5 , chanceToInfectorOfMask 1/20, chanceToRecoverer 1/15
    const State chanceToInfector[INFECT_RATE]{ State::S,State::S,State::S,State::S,State::I};
    const State chanceToInfectorOfMask[INFECT_RATE_MASK]{ State::S,State::S,State::S,State::S,State::S,State::S,State::S,State::S,State::S,State::S,
                                            State::S,State::S,State::S,State::S,State::S,State::I,State::S,State::S,State::S,State::S};
    const State chanceToRecoverer[RECOVER_RATE]{ State::I,State::I,State::I,State::I,State::I,State::I,State::I,State::I,State::I,State::I,State::I,State::I,State::I,State::I,State::R};

    // 감염자 벡터 -> time 한턴 끝날때마다 회복할지 말지 적용하기 위해. 
    // 또한 초기 감염자 설정 위해.
    vector<Node*> infectorVec;

    const int initialInfectorCondition;
    vector<int> hub_boundary;
    int hub_index;

    vector<int> shuffledIndex;
public:
    ~Simulator() { for(int i=0; i<peapleNodePList.size(); i++) delete peapleNodePList[i]; }
    Simulator(int iic=RANDOM_INFECTED,int hi = 2): initialInfectorCondition(iic), hub_index(hi){
        rndGen.seed(rnd());
        shuffledIndex.reserve(MAX_NODE_SIZE);
        BAGraph *genBA = new BAGraph();
        WeightType (*genGraph)[MAX_NODE_SIZE] = (WeightType(*)[MAX_NODE_SIZE])genBA->getGraphPointer();  
        for(int i=0; i<MAX_NODE_SIZE; i++) // 생성된 그래프 가져오기.
            for(int j=0; j<MAX_NODE_SIZE; j++)
                adjMat[i][j] = genGraph[i][j] ;
        for(int id=0; id<MAX_NODE_SIZE; id++){  // 노드 초기화 
            linkCount[id] = genBA->getNodeDegree(id); // 각 위치당 링크의 개수를 가져옴. 
            peapleNodePList.push_back(new Node() ); 
            for(int v =0;v<MAX_NODE_SIZE; v++)
                if(adjMat[id][v] != 0) nextNodeList[id].push_back(v);  
        }
        hub_boundary = genBA->getHubBoundary();
        delete genBA; // 그래프 생성기 메모리 제거.
        for(int i=0;i<MAX_NODE_SIZE; i++) shuffledIndex[i] = i;
    }
 
    void simulate(int tippintPoint){
        initInfector(initialInfectorCondition); // 최초 감염자 수를 설정. 
        while(infectorVec.size() != 0){
            if(totalTime >= TIME_LIMIT) { return; } // 시뮬 종료 조건2. //  printResult();
            synchronizeTime(); // 상태변화가 가능하도록 time 동기화. 
            cheackContactCondition(tippintPoint); // 노드를 돌면서 접촉 노드면 감염 시도.  
            tryEscapeFromCovid19(); // 회복 시도.             
            humanRandomMovement(); // 사람들의 이동. 
            totalTime++;
        }
        //printResult();
    }

    Output outResult(){
        return Output(susceptor, infector, recoverer, totalTime);
    }
    
// 여기서부터는 main에서 호출 못하도록. 클래스 내부에서만 쓰는 함수들임. 
private: 
    void initInfector(int iic){
        auto rng = default_random_engine {};
        shuffle(shuffledIndex.begin(), shuffledIndex.end(),rng);
        int infector_count = 0;
        if(iic==RANDOM_INFECTED)
        {   // random infected
            for(int i=0; i<MAX_NODE_SIZE; i++){
                int index = shuffledIndex[i];
                addInfector(index);
                if(NUM_INITIAL_INFECTOR <= ++infector_count) return;
            }
        } 
        else { // 특정 링크 개수를 가진 허브에 감염자 발생시키기.
            cout << hub_boundary[hub_index] << endl; // test
            for(int i=0; i<MAX_NODE_SIZE; i++){
                int index = shuffledIndex[i];
                //cout << index << endl;
                if( hub_boundary[hub_index] == linkCount[index]){
                    addInfector(index);
                    if(NUM_INITIAL_INFECTOR <= ++infector_count) return;
                }
            }
        }
    }
    void addInfector(int i){
        infectorVec.push_back(peapleNodePList[i]);
        peapleNodePList[i]->setState(State::I);
        susceptor--; infector++;
    }
    void synchronizeTime() {
        for(int id=0; id<MAX_NODE_SIZE; id++){ peapleNodePList[id]->setTime() = totalTime; }
    }
    void humanRandomMovement(){
        auto rng = default_random_engine {};
        shuffle(peapleNodePList.begin(), peapleNodePList.end(), rng);
    }
    void cheackContactCondition(int tippingPoint=0) {
        for(int id=0; id<MAX_NODE_SIZE; id++){ // id는 현재 peapleNodePList[id]가 서 있는 장소. 
            State state = peapleNodePList[id]->getState();
            if ( state == State::I && (peapleNodePList[id]->getTime() == totalTime)){ // node가 I일때만 옆 노드에 영향력 행사. 
                if(nextNodeList[id].size() >= tippingPoint) { 
                    tryInfectWithCovid19(id,chanceToInfectorOfMask,INFECT_RATE_MASK); // 밀집에서는 마스크를.
                } else {
                    tryInfectWithCovid19(id,chanceToInfector,INFECT_RATE);
                }
                
            }
        }
    }
    void tryInfectWithCovid19(Id rPosition, const State attachment[], int attachmentSize) {
        //nextNodeList[id_R] - id_R위치에 있는 노드에 연결된 노드들의 id를 가지고 있는 벡터.
        vector<Id> rNodeNeighborList = nextNodeList[rPosition]; // 지금 id_R에 위치한 노드는 감염노드임.
        for(int i=0; i< rNodeNeighborList.size(); i++){
            int index = rNodeNeighborList[i];
            State state = peapleNodePList[index]->getState();
            uniform_int_distribution<int> dis(0,attachmentSize-1);
            if(state == State::I || state == State::R){ return; } // 변화 시도 안함. 
            else { // 이웃 노드가 S상태면, 감염을 시도함.
                State toStatus = attachment[dis(rndGen)];
                peapleNodePList[index]->setState() = toStatus; // S면 그대로인거, I면 변한거.감염된거.
                if( toStatus == State::I){ // 변할 확률에 걸렸을때. 변함.
                    susceptor--; infector++;
                    infectorVec.push_back(peapleNodePList[index]);
                    peapleNodePList[index]->setTime()++; // 이제 막 변화된 노드는 회복을 시도하면 안됨. sync를 틀어준다.
                } // 감염된 노드의 주소를 감염노드포인터 벡터에 줌.
            }
        }
    }
    void tryEscapeFromCovid19() {
        // 회복 시도. 감염자 노드만 돌면서 회복을 시도함. time을 체크해서 이제막 변한 노드라면 변화시도 안함.
        for(int i=0; i<infectorVec.size(); i++){
            if(infectorVec[i]->getTime() != totalTime) continue;
            uniform_int_distribution<int> dis(0,RECOVER_RATE-1); // 0~14(15개)는 회복일 수. 
            State toStatus = chanceToRecoverer[dis(rndGen)];
            if(toStatus == State::R){
                infector--; recoverer++;
                infectorVec[i]->setState() = toStatus; 
                infectorVec.erase(infectorVec.begin()+i);
            }
        }
    }
    void printResult() {
        cout << "susceptor: " << susceptor << '\n';
        cout << "infector: " << infector << '\n';
        cout << "recoverer: " << recoverer << '\n';
        cout << "totalTime: " << totalTime << '\n';
    }
   
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*

    그래프는 맵. 링크수가 많은 건 밀집(실내)장소, 적은 건 분포(야외)장소. 
    그래프의 인덱스는 장소의 식별자. 0~999가 각 장소의 id.
    peapleNodePList는 매 시간마다 섞이는데, 이건 위치 이동을 의미. 
    위치 이동 후, 그 위치의 id마다 돌면서 노드의 상태를 파악하고 I인 경우 뭔가 조치를 취함. S,R이면 걍 암것도 안함. 
    옆 노드 확인은, 위치 id로 함. vector를 돌 때 사용한 id가 현재 그 위치를 의미, 그 위치의 주변 위치는 nextNodeList에 나와 있음.
    id=0~999를 돌면서, peapleNodePList[i]->getState()가 S면 통과, R이면 통과, I라면 {
        1. 매 턴마다 다시 nextNodeList를 돌면서, nextNodeList가 반환하는 위치의 노드( *(peapleNodePList[nextNodeList[0~nextNodeList.size()-1]])  )의 SIR 상태를 파악함.
        2. SIR파악 후, S라면 감염 시도{
            이 때, 장소가 '허브'인지 아닌지 판단해서 다른 확률 적용. 
        }. 그 노드가 감염이 되었다? 그럼 그 노드의 상태를 바꿔줌.  
        3. 해당 time에서 상태변화가 일어나면 s,i,r 개수 변경. - 기록 위해 필요한 변수. 
        4. 끝나기 전에 I의 상태를 R로 바꾸는 시도 ㄱ. 
    }
    

    time이 정해진 만큼 돌았거나, i가 0이 되면 종료. 
    i가 0일 때의 s와 r의 값을 보는 것 의미. 

*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 몇가지 가정

// 링크 수가 많으면(경계 지정) 마스크를 꼭 쓰게한다. - 접촉해도 ㄱㅊ.
// 링크 수가 적으면 (그 경계 이하) 마스크를 뺀다. - 접촉시 위험.

// 링크 수가 많은 노드를 '실내(indoor)'라 가정한다. 그리고 밀집지역이라 가정한다.
// 링크 수가 적은 노드를 '야외(outdoor)'라 가정한다. 그리고 분산지역이라 가정한다. 
// 실험의 단순화를 위해, 실내와 야외라고만 설정하도록 하겠다. 

// 실험 결과 -> 1m 이상 거리두기는 약10%, 마스크 쓰기는 약 14% 감염율을 떨어트렸다.
//마스크는 코로나19 감염률 14%, 2m 거리두기는 9.8% 떨어트린다 
// http://dongascience.donga.com/news.php?idx=37117
// 이 실험 결과를 바탕으로 밀집지역에서는 StoI의 확률을 

// 감염 확률은 정확히는 야외에서 감염될 확률

// 이외에 다른 자료와의 큰 격차가 없기에 위 자료를 근거로 하고, 조금 더 확률을 단순화 하여 시뮬레이션을 진행한다. 

// 실험의 목적은 '실내에서는 마스크를 쓰고 실외에서는 벗어도 되지 않을까'를 밝히는 것이다. 
// 따라서 마스크와 거리두기 자체의 효과에 대해서는 '있다'고 가정하고 실험을 진행한다.
// 오로지 '밀집도'에 따른 감염자 추이만을 보기 위함이다. 

// 초기 감염자는 1(또는 2 또는 3)로 한다. 유툽에서 초기 감염자의 수는 최종 감염자 비율에 큰 영향을 주지 않기 때문에. 해보고 이상하면 늘려보자. 
// nodeVec를 링크 수를 기준으로 정렬하여 오름차순==야외감염자출연//내림차순==실내감염자출연//랜덤==랜덤



