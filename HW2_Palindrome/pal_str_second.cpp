#include <iostream>
#include <string>
using namespace std;

string reverseString(string str) {
	if(str.length() == 1) {
		return str;
	}
	string last; 
   	last.push_back(str.back()) ;
	str.pop_back();
	return	last + reverseString(str); 
}	

int main() {
	cout << reverseString("abcdefg") << endl;

	return 0;
}
