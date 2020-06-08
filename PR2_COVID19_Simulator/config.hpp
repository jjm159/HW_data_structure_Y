// 전체 프로그램에서 사용하는 상수와 데이터 타입의 모음.
#ifndef __CONFIG__
#define __CONFIG__

enum { TIME_LIMIT = 1000 };  // 총 진행 시간 - 충분히 큰 수로. 
enum {  INFECT_RATE = 5, 
        INFECT_RATE_MASK = 20, 
        RECOVER_RATE = 15   
}; // 감염 확률 분모
enum { MAX_NODE_SIZE = 1000 };  
enum class State { S, I, R }; // 감염상태

typedef short WeightType;
typedef unsigned int TimeCount;
typedef int Id;
typedef unsigned int LinkCount;

enum InitialInfectorCondition { RANDOM_INFECTED=0, SPECIAL_INFECTED }; 

enum { NUM_INITIAL_INFECTOR = 1 };// 초기 감염자 수. // 고정!!
// 이게 너~무 많으면 마스크의 효과가 줄어듬. 하지만 거의 상관 없음. 즉, 너무 많지만 않으면 됨.

enum {TIPPING_POINT_HUB = 0}  ;   // 허브 기준 - 마스크 쓰는 횟수와 연관. 
// 낮을 수록 마스크 많이 쓴다. 
// 디폴트 0. 항상 모두 마스크를 쓴다.

enum { TRY_TEST =100 };


#endif
