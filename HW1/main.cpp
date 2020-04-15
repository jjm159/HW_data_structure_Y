#include "hw01_d_linked_list.h"
#include <iostream>
using namespace std;

// 우선 그냥 과제 하고 ,  예외 처리 적용해보자. 

class Test {
    string a;
    public:
        Test(){};
        Test(string a):a(a) {};
        ~Test() {
            cout << a << " is dead" << endl;
        }
        friend ostream& operator<< (ostream& out, Test t);
};
ostream& operator<< (ostream& out, Test t){
    out << t.a;
    return out; 
}

int main(){
    d_linked_list<string> testList;
    testList.push_front("a");
    testList.push_front("b");
    testList.push_front("c");
    testList.push_front("d");
    for(int i = 0 ; i< 4; i++){
        cout << testList.pop_front() << endl;
    }
cout << "------------------------------"<< endl;
    testList.push_back("a");
    testList.push_back("b");
    testList.push_back("c");
    testList.push_back("d");
    for(int i = 0 ; i< 4; i++){
        cout << testList.pop_back() << endl;
    }
cout << "------------------------------"<< endl;
    testList.push_back("a");
    testList.push_back("c");
    testList.push_back("d");
    testList.push_back("e");
    testList.insert_node_at(1,"b");
    for(int i = 0 ; i< 5; i++){
        cout << testList.pop_front() << endl;
    }
cout << "------------------------------"<< endl;
    testList.push_back("a");
    testList.push_back("c");
    testList.push_back("d");
    testList.push_back("e");
    testList.insert_node_at(1,"b");
    testList.delete_list();


    d_linked_list<Test> a; // 이때 Test 타입의 메모리가 만들어지면서 Test클래스의 디폴트 생성자가 호출되어야 한다.
    a.push_back(Test("a"));
    a.push_back(Test("b"));
    a.push_back(Test("c"));

    cout << a.front() << endl;
    cout << a.back() << endl;

    cout << a.pop_node_at(1) << endl;
    cout << a.pop_back() << endl;
    cout << a.pop_back() << endl;
    //cout << a.pop_node_at(1) << endl;
    cout << "------------------------------"<< endl;

    dll_deque<int> num ;
    num.addFront(1);
    num.addFront(2);
    num.addFront(3);
    cout << num.front() << " " << num.back() << endl;
    cout << num.deleteRear() << endl;
    cout << num.deleteRear() << endl;
    cout << num.deleteRear() << endl;

    cout << "------------------------------"<< endl;

    num.addRear(1);
    num.addRear(2);
    num.addRear(3);
    cout << num.front() << " " << num.back() << endl;
    cout << num.getFront() << " " << num.getRear() << endl;
    cout << num.deleteFront() << endl;
    cout << num.deleteFront() << endl;
    cout << num.deleteFront() << endl;
    num.addRear(3);
    cout << num.getFront() << " " << num.getRear() << endl;


    //testList.front();
    //testList.back();




    return 0;
}

// 클래스를 리스트의 요소로 줘서 
// 메모리가 잘 해제되는지 소멸자로 확인해보기.
// 그니까 벡터가 소멸될 때 노드를 모두 잘 소멸시키는지 확인하기 위해
// 요소를 내가 만든 클래스의 객체로줘서 
// 이 요소가 메모리에서 사라질 때 호출되는 
// 소멸자를 만들어서 cout으로 찍어보기!