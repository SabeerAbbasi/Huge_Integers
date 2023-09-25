#ifndef HUGEINTEGER_H_
#define HUGEINTEGER_H_
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

class HugeInteger
{
private:
	std::vector <int> numvalue;
	bool negative = false;
	bool zero = false;
	int randDigit(int base){
		srand(time(0));
		return  rand()%base;
	};

	bool removeZero();
	void insertZero(HugeInteger& h, int& n);
	bool tenscomp(HugeInteger& h, int n);

public:
	HugeInteger();
	HugeInteger(const std::string& val);
	HugeInteger(int n);
	HugeInteger add(const HugeInteger& h);
	HugeInteger subtract(const HugeInteger& h);
	HugeInteger multiply(const HugeInteger& h);
	int compareTo(const HugeInteger& h);
	HugeInteger shift(const int& n);
	std::string toString();
};

#endif /* HUGEINTEGER_H_ */




