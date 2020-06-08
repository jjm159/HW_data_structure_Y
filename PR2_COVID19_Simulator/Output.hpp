
// Test 결과 출력을 돕습니다. 

#include "config.hpp"
#include <iostream>
using namespace std;

class Output {
    int susceptor=0;
    int infector=0;
    int recoverer=0;
    TimeCount totalTime=0;
public:
    Output(int s=0, int i=0, int r=0, TimeCount t=0):susceptor(s), infector(i), recoverer(r), totalTime(t) {}
    Output(const Output& source){ 
        susceptor=source.susceptor;
        infector=source.infector;
        recoverer=source.recoverer;
        totalTime=source.totalTime;
    }
    void operator+=(const Output& source) {
        susceptor+=source.susceptor;
        infector+=source.infector;
        recoverer+=source.recoverer;
        totalTime+=source.totalTime;
    }
    Output operator/(int divisor){
        return Output(susceptor/divisor,
                        infector/divisor,
                        recoverer/divisor,
                        totalTime/divisor);
    }
    void print(){
        cout << "susceptor: " << susceptor << '\n';
        cout << "infector: " << infector << '\n';
        cout << "recoverer: " << recoverer << '\n';
        cout << "totalTime: " << totalTime << '\n';
    }
};