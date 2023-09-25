
#ifndef HUGEINTEGER_H_
#define HUGEINTEGER_H_
#include <iostream>

class HugeInteger
{
private:
	int* HugeInt;  //integer for the huge integer class

public:
	// Variables
		std::string negative;
		int sizeofarray;
		std::string math_sign;

		bool removeZero();							// removes all leading zeros and returns true if the resultant number is zero
		void insertZero(HugeInteger& h, int& n);
		bool tenComplement(HugeInteger& h, int n);	//added

	// Required methods
	HugeInteger(const std::string& val);
	HugeInteger(int n);
	int compare_array(const HugeInteger& h);
	HugeInteger add(const HugeInteger& h);
	HugeInteger subtract(const HugeInteger& h);
	HugeInteger multiply(const HugeInteger& h);
	int compareTo(const HugeInteger& h);
	std::string toString();
};

#endif /* HUGEINTEGER_H_ */



