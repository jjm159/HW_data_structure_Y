#include "config.hpp"
using namespace std;

class Node {
    TimeCount time = 0; // I가 되면 그때 시간이 증가. // 상태변화 sync 맞추기 위해 필요. 
    State state = State::S; // S I R 
public:
    State getState() const { return state;}
    State& setState()  { return state;}
    void setState(State s)  { state=s;}
    TimeCount getTime() const { return time; }
    TimeCount& setTime() {return time;}
};
