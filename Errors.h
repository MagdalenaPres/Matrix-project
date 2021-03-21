#include <iostream>
#include "DefinedErrors.h"

using namespace std;

class Errors
{
private:
	int error_code;
public:
	Errors() { error_code = NO_ERROR; };
	int getErrorCode() { return error_code; };
	void setErrorCode(int error) { error_code = error; };
	string message(int error_code);
};
