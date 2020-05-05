#include <iostream>
#include <string>
using namespace std;


int reverseDigits( int number1 , int number2 = 0 ) {

	if( number1 / 10 < 1 ) return number1 % 10 + number2 * 10 ;

	int extractNumber = number1 % 10 ;

	return reverseDigits(number1 / 10 , extractNumber + number2 * 10);
}

int main(int argc, char* argv[]) {

	int testNum = atoi(argv[1]);

	cout << reverseDigits(testNum) << endl;


	return 0;
}
