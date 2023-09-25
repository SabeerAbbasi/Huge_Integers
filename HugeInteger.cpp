#include "HugeInteger.h"
using namespace std;

HugeInteger::HugeInteger(const std::string& val) {

    if (val.empty()) {
        throw std::invalid_argument("The input string is empty");
    }

    size_t index = 0;

    if (val[0] == '-') { // Check if the number is negative
        if (val.size() == 1) {
            throw std::invalid_argument("The input string only contains a negative sign"); // Throw an exception if the string only contains a negative sign
        }
        negative = true; // Set the negative flag and increment the index
        ++index;
    }

    for (; index < val.size(); ++index) {
        if (val[index] >= '0' && val[index] <= '9') { // Check if the character is a digit
            numvalue.push_back(val[index] - '0'); // Convert the digit to an integer and add it to the numvalue vector
        }
        else if (val[index] == '0') { // Check if the character is a zero

            if (numvalue.empty()) {
                if (index == val.size() - 1) { // If the string only contains a single zero, set the zero flag and add the digit to the vector
                    zero = true;
                    negative = false;
                    numvalue.push_back(0);
                }
            } else {
                numvalue.push_back(0); // Otherwise, add the digit to the vector
            }
        }
        else {
            throw std::invalid_argument("The input string contains a non-numeric character");
        }
    }
}

HugeInteger::HugeInteger(int n) {
    // Throw an exception if the input is zero or negative
    if (n <= 0)
        throw std::invalid_argument("Input cannot be negative or zero");

    // Generate n random numvalue and add them to the numvalue vector
    for (int i = 0; i < n; i++)
        numvalue.push_back(randDigit(10));
}


HugeInteger::HugeInteger() {

    negative = false; // Sets the negative flag to false, indicating that the HugeInteger is positive
    zero = true; // Sets the zero flag to true, indicating that the HugeInteger is zero
    numvalue.push_back(0); // Adds a single digit zero to the numvalue vector
}

bool HugeInteger::tenscomp(HugeInteger &h, int n) {

    if (h.numvalue.size() == 1 && h.numvalue[0] == 0) { // If the HugeInteger has only one digit and it is zero, insert n zeros and return false
        insertZero(h, n); // Inserts n zeros at the beginning of the HugeInteger
        return false;
    }

    insertZero(h, n); // Inserts n zeros at the beginning of the HugeInteger
    bool overflow = true; // Flag to indicate if there was a carry during the computation

    for (int i = h.numvalue.size()-1; i >=0; i--) { // Loops through the numvalue of the HugeInteger from right to left
        h.numvalue[i] = (9 - h.numvalue[i] + overflow) % 10; // Computes the 10's complement of the current digit and updates the digit
        overflow = (9 - h.numvalue[i] + overflow) / 10;// Updates the overflow flag for the next digit
    }

    return true;
}

// Inserts zeros at the beginning of the HugeInteger to make it have at least n numvalue
void HugeInteger::insertZero(HugeInteger& h, int &n) {

    for (int i = h.numvalue.size(); i < n; i++) {
        h.numvalue.insert(h.numvalue.begin(), 0);
    }
}

// Removes any leading zeros from the numvalue vector of the HugeInteger
bool HugeInteger::removeZero() {

    while ((!numvalue.empty()) && (*numvalue.begin() == 0))
        numvalue.erase(numvalue.begin());
    bool zero = numvalue.empty();
    if (zero) numvalue.push_back(0);
    return zero;
}


// Returns a new HugeInteger that is the original HugeInteger shifted n numvalue to the left
HugeInteger HugeInteger::shift(const int& n) {

    if (n <= 0) return *this; // If n is less than or equal to zero, returns the original HugeInteger
    HugeInteger ans = *this;
    for (int i = 0; i < n; i++)
        ans.numvalue.push_back(0); // Adds n zeros to the end of the numvalue vector of the new HugeInteger
    return ans;
}


HugeInteger HugeInteger::add(const HugeInteger& h) {

	const int num1 = numvalue.size(), num2 = h.numvalue.size(); // Get the size of the two integers
	if (num1 == 0 || num2 == 0) {
		throw std::invalid_argument("adding NULL is not allowed");
	}

	// Create copies of this HugeInteger object and the argument HugeInteger object
	HugeInteger num1copy = *this;
	HugeInteger num2copy = h;

	// Check if either of the HugeInteger objects is zero
	const bool thisIsZero = this->zero;
	const bool otherIsZero = h.zero;
	if (thisIsZero || otherIsZero) {
		return (thisIsZero) ? num2copy : num1copy;
	}

	// Ensure that both HugeInteger objects have the same number of numvalue
	num1copy.numvalue.resize(num2, 0);
	num2copy.numvalue.resize(num1, 0);

//	insertZero(num2copy, num1);
//	insertZero(num1copy, num2);

	const bool needsComplement = negative != (h.negative); // Check if 10's complement notation needs to be used

	if (needsComplement) { //only used if signs are different
		if (this->negative) {
			tenscomp(num1copy, num2);
		}
		else {
			tenscomp(num2copy, num1);
		}
	}

	bool carry = false;
	int sum = 0;
	const int maxIndex = (num1 >= num2) ? num1 - 1 : num2 - 1;

	// Add the numvalue of the HugeInteger objects starting from the least significant digit
	for (int i = maxIndex; i >= 0; --i) {
		sum = num1copy.numvalue[i] + num2copy.numvalue[i] + carry;
		num1copy.numvalue[i] = sum % 10;
		carry = sum / 10;
	}

	if (needsComplement) {
		if (carry) {
			num1copy.negative = !num1copy.zero;
		}
		else {
			tenscomp(num1copy, num1);
			num1copy.zero = num1copy.removeZero();
			num1copy.negative = !num1copy.zero;
		}

		return num1copy;
	}

	// If there is carry, add it to the beginning of the numvalue vector
	if (carry) {
		num1copy.numvalue.insert(num1copy.numvalue.begin(), carry);
	}

	num1copy.zero = num1copy.removeZero(); // Remove leading zeroes

	return num1copy;
}

HugeInteger HugeInteger::subtract(const HugeInteger& h) {

	negative = !negative; // toggle the sign of this HugeInteger object
	HugeInteger ans = add(h); // add the other HugeInteger object to this HugeInteger object
	negative = !negative; // toggle the sign of this HugeInteger object back to its original sign
	ans.negative = ans.zero ? false : !(ans.negative); // toggle the sign of the answer HugeInteger object if it is not zero
	return ans;

}

HugeInteger HugeInteger::multiply(const HugeInteger& h) {

	if (this->zero || h.zero) { // check if either of the HugeIntegers is zero
		return HugeInteger("0");
	}

	HugeInteger result;
	for (int i = h.numvalue.size() - 1; i >= 0; i--) // Iterate through the numvalue of the second HugeInteger object from right to left
	{
		int currentDigit = h.numvalue.at(i);
		// Add this HugeInteger object to the result the number of times indicated by the current digit
		for (int j = 0; j < currentDigit; j++)
		{
			result = result.add(*this);
		}
		this->shift(1); // Shift this HugeInteger object to the left by one digit
	}

	result.negative = this->negative != h.negative; // Determine the sign of the result HugeInteger object

	return result;
}


int HugeInteger::compareTo(const HugeInteger& h) {

    if (negative != h.negative) { // Check if the two HugeInteger objects have different signs
        return (negative == true) ? -1 : 1; // return -1 if this HugeInteger object is negative
    }

    if (numvalue.size() > h.numvalue.size()) {
        return (negative ? -1 : 1); //return -1 if numvalue is negative

    } else if (numvalue.size() < h.numvalue.size()) {

        return (negative ? 1 : -1); //return -1 if h.numvalue is negative
    }

    for (int i = 0; i < numvalue.size(); i++) { //if they have same length, compare respective numvalues

        if (numvalue.at(i) > h.numvalue.at(i)) {
            return negative ? -1 : 1; //return -1 if numvalue is negative

        } else if (numvalue.at(i) < h.numvalue.at(i)) {

            return negative ? 1 : -1; //return -1 if h.numvalue is negative
        }
    }

    return 0; // If the two HugeInteger objects are equal, return 0
}


std::string HugeInteger::toString() {

	if (numvalue.empty()) {
		return "0";
	}

	std::string bigNum = negative ? "-" : ""; //Initialize an empty string "bigNum" to hold the final output, and add a negative sign if the number is negative
	std::vector<int>::iterator it; //Define an iterator "it" to traverse the "numvalue" vector

	//Iterate through the "numvalue" vector and append each digit to "bigNum"
	for (it = numvalue.begin(); it != numvalue.end(); it++) {
		bigNum += std::to_string(*it);
	}

	return bigNum;
}

