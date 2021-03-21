
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 
#include <algorithm>
#include "Constants.h"
#include "Errors.h"

using namespace std;

template <typename T>
class CMatrix
{
private:
	int C_Rows, C_Columns;
	T** C_Matrix;

	void deallocate();
	void allocate();

public:
	CMatrix(); 
	CMatrix(int rows, int columns);
	~CMatrix(); 
	CMatrix(CMatrix<T> const& pc_other); 
	
	void createMatrix(int rows, int columns);
	void setValue(T value, int row, int column, Errors* er); 
	T getValue(int row, int column, Errors* er);  
	void fillInTheMatrix(); 
	void fillTheMatrixWithZeros();
	void setIdentityMatrix(Errors* er); 
	bool printValue(int row, int column); 
	void printMatrix(); 
	CMatrix<T> transposition();  

	bool loadMatrixFromFile(string fileName, Errors* er); 
	void appendFromFile(string fileName, T value, Errors* er);

	CMatrix<T> createVectorByRow(int row, Errors* er); 
	CMatrix<T> createVectorByColumn(int column, Errors* er); 
	T scalarProduct(CMatrix<T>& m1, Errors* er);  

	void multiplyElementsOfMatrix(int number); 
	bool addTwoMatrices(CMatrix<T>& m1, CMatrix<T>& m2);  
	bool subtractTwoMatrices(CMatrix<T>& m1, CMatrix<T>& m2); 
	bool multiplyTwoMatrices(CMatrix<T>& m1, CMatrix<T>& m2);

	CMatrix<T> operator+(CMatrix<T>& m1);  
	CMatrix<T> operator-(CMatrix<T>& m1); 
	CMatrix<T> operator*(CMatrix<T>& m1);   
	void operator=(const CMatrix<T>& pc_other);
	T operator()(int row, int column, Errors* er); 
};

template <typename T>
CMatrix<T>::CMatrix()
{
	C_Columns = DEF_SIZE;
	C_Rows = DEF_SIZE;

	allocate();
};

template <typename T>
CMatrix<T>::CMatrix(int rows, int columns)
{
	createMatrix(rows, columns);
};

template <typename T>
CMatrix<T>::~CMatrix()
{
	if (C_Matrix != nullptr)
		deallocate();
};

template <typename T>
CMatrix<T>::CMatrix(CMatrix<T> const& pc_other)
{
	C_Rows = pc_other.C_Rows;
	C_Columns = pc_other.C_Columns;

	allocate();

	for (int i = 0; i < C_Rows; i++)
		for (int j = 0; j < C_Columns; j++)
			C_Matrix[i][j] = pc_other.C_Matrix[i][j];
};

template <typename T>
void CMatrix<T>::deallocate()
{
	for (int i = 0; i < C_Rows; i++)
		delete[] C_Matrix[i];
	delete[] C_Matrix;
};

template <typename T>
void CMatrix<T>::allocate()
{
	C_Matrix = new T * [C_Rows];

	for (int i = 0; i < C_Rows; i++)
		C_Matrix[i] = new T[C_Columns];
};

template <typename T>
void CMatrix<T>::createMatrix(int rows, int columns)
{
	deallocate();

	if (rows > 0 && columns > 0)
	{
		C_Rows = rows;
		C_Columns = columns;
	}
	else
	{
		C_Rows = DEF_SIZE;
		C_Columns = DEF_SIZE;
	}
	allocate();
};

template <typename T>
void CMatrix<T>::setValue(T value, int row, int column, Errors* er)
{
	if (row >= 0 && column >= 0 && C_Rows >= row && C_Columns >= column)
	{
		C_Matrix[row][column] = value;
	}
	else
		er->setErrorCode(NO_SUCH_CELL_IN_THE_MATRIX);
};

template <typename T>
T CMatrix<T>::getValue(int row, int column, Errors* er)
{
	if (row >= 0 && column >= 0 && row < C_Rows && column < C_Columns)
		return C_Matrix[row][column];
	else
	{
		er->setErrorCode(NO_SUCH_CELL_IN_THE_MATRIX);
		return NULL;
	}
};

template <typename T>
void CMatrix<T>::fillInTheMatrix()
{
	for (int i = 0; i < C_Rows; ++i)
		for (int j = 0; j < C_Columns; ++j)
		{
			cout << "Enter element " << i + 1 << j + 1 << " : ";
			cin >> C_Matrix[i][j];
		}
};

template <typename T>
void CMatrix<T>::fillTheMatrixWithZeros()
{
	for (int i = 0; i < C_Rows; i++)
		for (int j = 0; j < C_Columns; j++)
			C_Matrix[i][j] = ZERO_VALUE;
};

template <typename T>
void CMatrix<T>::setIdentityMatrix(Errors* er)
{
	if (C_Rows == C_Columns) 
	{
		for (int i = 0; i < C_Rows; i++)
			for (int j = 0; j < C_Columns; j++)
				if (i == j)
					C_Matrix[i][j] = 1;
				else
					C_Matrix[i][j] = 0;
	}
	else
	{
		er->setErrorCode(NOT_SQUARE_MATRICES);
	}
};

template <typename T>
bool CMatrix<T>::printValue(int row, int column)
{
	if (row >= 0 && column >= 0 && C_Rows >= row && C_Columns >= column)
	{
		cout << C_Matrix[row][column];
		return true;
	}
	else
		return false;
};

template<typename T>
void CMatrix<T>::printMatrix()
{
	for (int i = 0; i < C_Rows; i++)
	{
		for (int j = 0; j < C_Columns; j++)
		{
			cout << C_Matrix[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl;
};

template<typename T>
CMatrix<T> CMatrix<T>::transposition()
{
	CMatrix transposited(C_Columns, C_Rows);

	for (int i = 0; i < C_Rows; i++)
		for (int j = 0; j < C_Columns; j++)
			transposited.C_Matrix[j][i] = C_Matrix[i][j];

	return transposited;
};

template<typename T>
bool CMatrix<T>::loadMatrixFromFile(string fileName, Errors* er)
{
	vector<int> numbersOfNumberOfColumns;  
	ifstream file;
	stringstream stringStream;
	string rowOfMatrix;
	T number;

	file.open(fileName.c_str());
	if (!file.good())
	{
		er->setErrorCode(CANNOT_OPEN_THE_FILE);
		return false;
	}
		
	string line;
	while (getline(file, line))
	{
		stringStream << line;

		int numberOfNumbersInRow = 0;
		while (!stringStream.eof())
		{
			stringStream >> rowOfMatrix;
			if (stringstream(rowOfMatrix) >> number)
			{
				numberOfNumbersInRow++;
			}
			rowOfMatrix = "";
		}
		numbersOfNumberOfColumns.push_back(numberOfNumbersInRow);
		stringStream.clear();
	}

	file.close();
	int maxNumber = *max_element(numbersOfNumberOfColumns.begin(), numbersOfNumberOfColumns.end());

	deallocate();

	C_Rows = numbersOfNumberOfColumns.size();
	C_Columns = maxNumber;

	allocate();

	file.open(fileName.c_str());
	if (!file.good())
	{
		er->setErrorCode(CANNOT_OPEN_THE_FILE);
		return false;
	}

	int actualRow = 0;
	while (getline(file, line))
	{
		stringStream << line;

		for (int i = 0; i < C_Columns; i++)
		{
			if (!stringStream.eof())
			{
				stringStream >> rowOfMatrix;
				if (stringstream(rowOfMatrix) >> number)
				{
					C_Matrix[actualRow][i] = number;
				}
				rowOfMatrix = "";
			}
			else
			{
				C_Matrix[actualRow][i] = 0;
			}
		}
		actualRow++;
		stringStream.clear();
	}
	file.close();
	return true;
};

template <typename T>
void CMatrix<T>::appendFromFile(string fileName, T value, Errors* er)
{
	CMatrix<T> fromFile;
	int rows = C_Rows;
	int cols = C_Columns;

	vector<vector<T>> table;

	fromFile.loadMatrixFromFile(fileName, er);

	for (int i = 0; i < rows; i++)
	{
		vector<T> v1;
		for (int j = 0; j < cols; j++)
		{
			v1.push_back(C_Matrix[i][j]);
		}
		table.push_back(v1);
	}
		
	deallocate();
	C_Rows = C_Rows + fromFile.C_Rows;

	if (C_Columns < fromFile.C_Columns)
		C_Columns = fromFile.C_Columns;
	
	allocate();

	for (int i = 0; i < C_Rows; i++)
		for (int j = 0; j < C_Columns; j++)
			if (i < rows)
			{
				if (j >= cols)
					C_Matrix[i][j] = value;
				else
					C_Matrix[i][j] = table[i][j];
			}
			else
			{
				if (j >= fromFile.C_Columns)
					C_Matrix[i][j] = value;
				else
					C_Matrix[i][j] = fromFile.C_Matrix[i-rows][j];
			}
};

template <typename T>
CMatrix<T> CMatrix<T>::createVectorByRow(int row, Errors* er)
{
	CMatrix vector(1, C_Columns);
	vector.allocate();
	
	if (row > 0 && row <= C_Rows)
	{
		for (int i = 0; i < C_Columns; i++)
		{
			vector.C_Matrix[0][i] = C_Matrix[row - 1][i];
		}
		return vector;
	}
	else 
	{
		er->setErrorCode(OUT_OF_RANGE);
		return vector;
	}
};

template <typename T>
CMatrix<T> CMatrix<T>::createVectorByColumn(int column, Errors* er)
{
	CMatrix vector(1, C_Rows);
	vector.allocate();

	if (column > 0 && column <= C_Columns)
	{
		for (int i = 0; i < C_Rows; i++)
		{
			vector.C_Matrix[0][i] = C_Matrix[i][column - 1];
		}
		return vector;
	}
	else
	{
		er->setErrorCode(OUT_OF_RANGE);
		return vector;
	}
};

template<typename T>
T CMatrix<T>::scalarProduct(CMatrix<T>& m1, Errors* er)
{
	if (C_Rows == 1 && m1.C_Rows == 1 && m1.C_Columns == C_Columns)
	{
		T product = 0;

		for (int i = 0; i < m1.C_Columns; i++)
			product += m1.C_Matrix[0][i] * C_Matrix[0][i];

		return product;
	}
	else
	{
		er->setErrorCode(NOT_A_VECTOR);
		return NULL;
	}
};

template<typename T>
void CMatrix<T>::multiplyElementsOfMatrix(int number)
{
	for (int i = 0; i < C_Rows; i++)
		for (int j = 0; j < C_Columns; j++)
			C_Matrix[i][j] = C_Matrix[i][j] * number;
};

template <typename T>
bool CMatrix<T>::addTwoMatrices(CMatrix<T>& m1, CMatrix<T>& m2)
{
	if (m1.C_Rows == m2.C_Rows && m1.C_Rows == C_Rows && m1.C_Columns == m2.C_Columns && m1.C_Columns == C_Columns)
	{
		for (int i = 0; i < C_Rows; i++)
		{
			for (int j = 0; j < C_Columns; j++)
			{
				C_Matrix[i][j] = m1.C_Matrix[i][j] + m2.C_Matrix[i][j];
			}
		}
		return true;
	}
	else
		return false;
};

template<typename T>
bool CMatrix<T>::subtractTwoMatrices(CMatrix<T>& m1, CMatrix<T>& m2)
{
	if (m1.C_Rows == m2.C_Rows && m1.C_Rows == C_Rows && m1.C_Columns == m2.C_Columns && m1.C_Columns == C_Columns)
	{
		for (int i = 0; i < C_Rows; i++)
		{
			for (int j = 0; j < C_Columns; j++)
			{
				C_Matrix[i][j] = m1.C_Matrix[i][j] - m2.C_Matrix[i][j];
			}
		}
		return true;
	}
	else
		return false;
};

template<typename T>
bool CMatrix<T>::multiplyTwoMatrices(CMatrix<T>& m1, CMatrix<T>& m2)
{
	if (m1.C_Columns == m2.C_Rows && C_Rows == m1.C_Rows && C_Columns == m2.C_Columns)
	{
		this->fillTheMatrixWithZeros();

		for (int j = 0; j < C_Rows; j++)
			for (int k = 0; k < C_Columns; k++)
				for (int i = 0; i < m2.C_Rows; i++)
					C_Matrix[j][k] += m1.C_Matrix[j][i] * m2.C_Matrix[i][k];

		return true;
	}
	else
		return false;
};

template<typename T>
CMatrix<T> CMatrix<T>::operator+(CMatrix<T>& m1)
{
	CMatrix<T> result(C_Rows, C_Columns);

	if (result.addTwoMatrices(*this, m1) != true)
		throw invalid_argument(WRONG_PARAMETERS);

	else return result;
};

template<typename T>
CMatrix<T> CMatrix<T>::operator-(CMatrix<T>& m1)
{
	CMatrix<T> result(C_Rows, C_Columns);

	if (result.subtractTwoMatrices(*this, m1) != true)
		throw invalid_argument(WRONG_PARAMETERS);

	else return result;
};

template <typename T>
CMatrix<T> CMatrix<T>::operator*(CMatrix<T>& m1)
{
	CMatrix<T> result(C_Rows, C_Columns);

	if (result.multiplyTwoMatrices(*this, m1) != true)
		throw invalid_argument(WRONG_PARAMETERS);

	else return result;
};

template <typename T>
void CMatrix<T>::operator=(const CMatrix<T>& pc_other)
{
	deallocate();

	C_Rows = pc_other.C_Rows;
	C_Columns = pc_other.C_Columns;

	allocate();

	for (int i = 0; i < C_Rows; i++)
		for (int j = 0; j < C_Columns; j++)
			C_Matrix[i][j] = pc_other.C_Matrix[i][j];
};

template <typename T>
T CMatrix<T>::operator()(int row, int column, Errors* er)
{
	this->getValue(row, column, er);

	if (er->getErrorCode() != NO_ERROR)
		throw invalid_argument(WRONG_PARAMETERS);

	else return this->getValue(row, column, er);
};
