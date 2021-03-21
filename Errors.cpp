#include "Errors.h"

using namespace std;

string Errors::message(int error_code)
{
	switch (error_code)
	{
	case 0:
		return "No errors";
	case 10:
		return "There is no such cell in the matrix, you gave wrong parameters. Returned NULL.";
	case 100:
		return "To perform this method, you need to pass only square matrices. These are not.";
	case 110:
		return "These is not a vector. Returned NULL";
	case 111:
		return "out of range";
	case 1010:
		return "Cannot open the file";
	default:
		return "Unrecognised error";
	}
};