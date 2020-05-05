#include <iostream>
using namespace std;

// 숫자 1234넣으면 4321	을 출력해줌. 

int reverseDigits(int num1, int num2 = 0) {

	int quotient = num1 / 10;
	int remainder = num1 % 10;
	int result = num2 * 10 + remainder;
	if ( quotient == 0) {
		return result;
	}
	return reverseDigits( quotient , result ) ;
}


int main () {


	cout << reverseDigits(123) << endl;
	cout << reverseDigits(220) << endl;



	return 0;
}
