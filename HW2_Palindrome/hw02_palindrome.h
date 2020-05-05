// 20132927 정재명
#pragma once
#include <string>


int reverseDigits(int number, int number2 = 0){
	int quotient = number / 10;
	int remainder = number % 10;
	int result = number2 * 10 + remainder;
	if( quotient == 0) {
		return result;
	}
	return reverseDigits(quotient, result);
}

bool isPalindromeInt(int number) {
	if( number == reverseDigits(number)) return true;
	return false;
}

std::string reverseString(std::string str) {
	if(str.length() == 0) return str;
	std::string last;
	last.push_back(str.back());
	str.pop_back();
	return last + reverseString(str);
}

bool isPalindromeString(std::string str){
	if(str.compare(reverseString(str))) return false;
	return true;
}
