#include <iostream>
using namespace std;

string reverseString(string str) {
	if( str.length() == 1) return str;
	string lastToFirstChar ;
   	lastToFirstChar.append(1,str.back());
   	str.pop_back();	
	return  lastToFirstChar.append( reverseString(str) );
}

int main(int argc, char* argv[]) {

	string s;

	if(argc < 2) 
	{
		s.append("abcde");
	} 
	else 
	{
		s.append(argv[1]);
	}


	cout << reverseString(s) << endl;


	return 0;
}
