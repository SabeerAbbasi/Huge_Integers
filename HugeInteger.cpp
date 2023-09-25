#include "HugeInteger.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/multiprecision/cpp_int.hpp>
using namespace std;

HugeInteger::HugeInteger(const std::string& val) {
	if(val[0]=='-'){ //determining whether the first index has a negative sign
		negative = '-'; //if it does then assign negative variable to '-'
	}
	else {
		negative = '+'; //otherwise assigns negative variable to '-'
	}

	string valcopy = val[0] == '-' ? val.substr(1) : val; //checks if first index in valcopy has a negative sign
		 //if it does then starts copying after the first index

	if (valcopy.empty()) { //checking if valcopy is an empty string
	    throw std::invalid_argument("Invalid Input, cannot be an empty string"); //throws error message if it is
	}

	for (char c : valcopy) { //for loop to go through each character c in valcopy string
	    if (!std::isdigit(c)) { //check if the character c is not a digit
	        throw std::invalid_argument("Invalid Input. Please enter a valid number"); //throws error message
	    }
	} //if all characters are digits, then nothing happens

	HugeInt = new int[valcopy.size()]; //allocates memory for HugeInt array using new

	for(int i = 0; i < valcopy.size(); i++){
		HugeInt[i] = valcopy[i] - '0'; //subtracting by 0 to get the integer value from the ASCII representation of the digits
	}
	sizeofarray = valcopy.size(); //setting sizeofarray to the array size of valcopy

}

HugeInteger::HugeInteger(int n) {

	if(n < 1){ //first digit can't be less than 1
		throw invalid_argument("Invalid Input. n must be larger or equal to 1"); //throws an error message
	}

	sizeofarray = n; //assign valid value of n to array size variable
	HugeInt = new int[n]; //allocates memory for HugeInt array using new

	//ensures first digit is not a zero and all other numbers are random between 0-9
	HugeInt[0] = rand()%9 + 1; //changes the assigned numbers from 0-8 to 1-9
	for(int i=1;i<n;i++){
		HugeInt[i] = rand()%10;
	}

}

int HugeInteger::compare_array(const HugeInteger& h){

	if (sizeofarray > h.sizeofarray)
		return 1; //returns 1 if the first array is larger
	if (sizeofarray < h.sizeofarray)
		return -1; //returns -1 if the second array is larger

	for (int i = 0; i < sizeofarray; i++){ //size of arrays are the same
	    if (HugeInt[i] > h.HugeInt[i])
	    	return 1; //returns 1 if digits in first array are larger
	    if (HugeInt[i] < h.HugeInt[i])
	    	return -1; //returns -1 if digits in second array are larger
	}

	return 0; //returns 0 if both arrays are equal

}

HugeInteger HugeInteger::add(const HugeInteger& h) {
	int *big; //used to store the contents of the bigger array
	int *sum; //used to store sum of the 2 integers in an array
	int bigSize; //array storing the bigger size array
	int sumArray; //array storing solution to sum of integers

	int carry = 0; //holds the carry value for addition

	//holding size of both arrays, -1 as this is an index and counting starts from 0
	int size_HugeInt = sizeofarray - 1;
	int size_h = h.sizeofarray - 1;

	string num_sign = ""; //determines sign of the number
	string math_sign = ""; //used to determine if the operation is addition or subtraction

	//both arrays are equal or digits in first array are larger
	if(compare_array(h) >= 0){
		//case 1: 1st term is -ve and 2nd is +ve
		if(negative == "-" && h.negative == "+" && math_sign == ""){
			math_sign = "-"; //subtracting first number from second using subtract method
			return subtract(h);
		}
		//case 2: 1st term is +ve and 2nd is -ve
		if (negative == "+" && h.negative == "-" && math_sign == ""){
			math_sign = "-"; //
			return subtract(h);
		}

		sum = (int*)calloc((size_HugeInt + 2),sizeof(int)); //allocating memory for 'size_HugeInt + 2'
		sumArray = size_HugeInt;	//Store size of HugeInt array into sumArray
		big = HugeInt; //points big pointer to the HugeInt array
		bigSize = sizeofarray; //sets bigSize variable to the size of HugeInt array
		if (math_sign == ""){ // if math_sign is empty,set num_sign to current value of negative
			num_sign = negative;
		}
		else{ //otherwise set to math_sign.
			num_sign = math_sign;
		}

	}

	//digits in 2nd array are larger
	else {
		if (negative == "-" && h.negative == "+" && math_sign == ""){
			math_sign = "+";
			return subtract(h);
		} if (negative == "+" && h.negative == "-" && math_sign == ""){
			math_sign = "-";
			return subtract(h);
		}
		sum = new int[size_h + 2];
		for (int i = 0; i < size_h + 2; i++){ //initializing each element in array to zero
		    sum[i] = 0;
		}
		sumArray = size_h;
		big = h.HugeInt;
		bigSize = h.sizeofarray;
		if(math_sign == ""){
			num_sign = h.negative;
		}
		else {
			num_sign = math_sign;
		}
	}

	while(size_HugeInt >= 0 && size_h >= 0){
		//adding both arrays to their corresponding indices with the carry
		sum[sumArray+1] = HugeInt[size_HugeInt] + h.HugeInt[size_h] + carry;
		if(sum[sumArray+1] > 9){ //implementing logical math carry
			carry = 1;
		}
		else {
			carry = 0;
		}
		sum[sumArray+1] = sum[sumArray+1]%10; //puts the ones digit into the index

		//cycling through the indices
		size_HugeInt--;
		size_h--;
		sumArray--;
	}

	//if there is an array that has more digits than the other
	for(sumArray; sumArray>=0;sumArray--){
		sum[sumArray + 1] = big[sumArray] + carry;
		//resetting carryover every loop
		if(sum[sumArray+1] > 9){
			carry = 1;
		}
		else {
			carry = 0;
		}
		sum[sumArray+1] = sum[sumArray+1]%10;

	}


	sum[0] = carry; // first index in sum array is the last carryover
	string solution = ""; //empty string for final solution

	if(num_sign == "-") //checking if the sign was negative
		solution += '-'; //add a negative sign to the solution

	for(int i = 0; i < bigSize + 1; i++){
		if(i!=0 || (i == 0 && sum[0] != 0)){ //make sure i isn't 0, or if i is equal to 0, then carry is not equal to 0.
			solution += ((char)(sum[i] + '0')); //converts integer stored in sum[i] to a character by adding ASCII value of 0
			//appended to solution string
		}
	}
	return HugeInteger(solution); // Return HugeInteger object
}



HugeInteger HugeInteger::subtract(const HugeInteger& h) {
	// TODO
	return HugeInteger("");
}




HugeInteger HugeInteger::multiply(const HugeInteger& h) {
	// TODO
	return HugeInteger("");
}

int HugeInteger::compareTo(const HugeInteger& h) {
	// TODO
	return 0;
}


std::string HugeInteger::toString() {
	bool zero = false;
	string stringsol; //string holding the final solution string

	if(negative == "-"){
		stringsol += "-"; //if originally HugeInteger was a negative number, add a negative sign to the string
	}

		for(int i = 0; i < sizeofarray; i++){
		if(HugeInt[i] != 0 || zero){
			stringsol += to_string(HugeInt[i]);
			zero=true;
		}
	}

	if(stringsol == "-" || stringsol.empty() || stringsol == "0") { //returning 0 if the string is negative, empty or zero
		return "0";
	}
	else {
		return stringsol;
	}
}


