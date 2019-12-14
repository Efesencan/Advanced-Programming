#ifndef _matrixclass
#define _matrixclass
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
// Efe Sencan 25083
// This is a templated class of the 2D dynamic array by the use of methods which consists of operative overloading
using namespace std;

template <class data_type>
class Matrix2D {
	private: 
		int rows, cols;
		data_type ** data;
	friend istream &  operator >> <data_type>(istream & inputFile,Matrix2D<data_type> & matrix); // overloading the >> operator which the left hand side operand will be the file object and right hand side object would ne the matrix object
	friend ostream & operator << <data_type>(ostream & inputFile,Matrix2D<data_type> & matrix);  // overloading the << operator by reading the each element of our 2D array matrix, and printing the values of them
	friend Matrix2D<data_type>  & operator + <data_type>(const Matrix2D<data_type> & first_matrix,const Matrix2D<data_type> & second_matrix); 
    public:
		Matrix2D(int r, int c); // constructor
		Matrix2D(const Matrix2D<data_type> & matrix); // deep copy constructor
		data_type GetIndex(int i, int j); // getting the index of the matrix
		void SetIndex(int i, int j, data_type val); // setting the index of the matrix
		Matrix2D(); // default constructor
		~Matrix2D(); // destructor
		const Matrix2D<data_type> & operator = (const Matrix2D<data_type> & rhs); // '=' operator
		const Matrix2D<data_type> & operator += (const Matrix2D<data_type> & rhs); // '+=' operator

};

template <class data_type>
Matrix2D<data_type>::Matrix2D(int r, int c) // creating the constructor of the class
{
    rows=r;
	cols=c;
	data = new data_type* [r]; 

	for (int i = 0; i<rows; i++) 
		data[i] = new data_type[cols];	
}

template <class data_type>
data_type Matrix2D<data_type>::GetIndex(int i, int j) // getting the value of the matrix
{
    return data[i][j];
}


template <class data_type>
void Matrix2D<data_type>::SetIndex(int i, int j, data_type value) // setting the value of the index
{
    data[i][j] = value;
}

template <class data_type> 
Matrix2D<data_type>::Matrix2D() // for default constructor
{
	rows = 0; // initializing the private data's
	cols = 0;
	data = NULL;
}

template <class data_type>
Matrix2D<data_type>::~Matrix2D() // destructor for the dynamic matrix
{
	for(int i = 0; i<rows; i++)
	{
		delete [] data[i]; // since we have 2D dynamic array we have to destruct both rows containing pointers and the final pointer to pointer
	}
	delete [] data; 
}

template <class data_type>
istream & operator >> (istream & inputFile,Matrix2D<data_type> & matrix) // forming the matrix through file
{
	int column = 0, row = 0,temp_column;
	string line;
	data_type another;
	while(getline(inputFile,line)) // first we count the number of rows and columns in the input file
	{
		column = 0;
		istringstream sentence(line);
		while(sentence >> another)
		{
			column++;
		}
		temp_column = column;
		row++;
	}
	matrix.rows = row; // after counting the rows and columns through the file we assign that value to the private data of our matrix
	matrix.cols = temp_column;
	matrix.data = new data_type *[row]; // we form a new 2D array by the use of pointer to the pointer
	for(int i = 0; i<row; i++)
	{
		matrix.data[i] = new data_type [temp_column];
	}
	inputFile.clear();
	inputFile.seekg(0);
	int new_row = 0, new_col = 0;
	while(getline(inputFile,line)) // after creating our matrix we add valeus by reading through the file
	{
		istringstream sentence(line);
		while(sentence >> another)
		{
			matrix.data[new_row][new_col] = another;
			new_col++;
		}
		new_col = 0;
		new_row++;
	}
	return inputFile;
}

template <class data_type>
ostream & operator << (ostream & inputFile,Matrix2D<data_type> & matrix) // printing the values of the each element of the matrix 
{
	for(int i = 0; i<matrix.rows; i++)
	{
		for(int j = 0; j<matrix.cols; j++)
		{
			inputFile<<setw(5)<<matrix.data[i][j];
		}
		inputFile<<endl;
	}
	return inputFile;

}

template <class data_type>
Matrix2D<data_type>::Matrix2D(const Matrix2D<data_type> & matrix) // deep copy constructor
{
	if(matrix.data == NULL) // if the data points to null than we dont have to equalize the elements of the one matriz to another since the matriz is empty
	{
		data = matrix.data;
	}
	else
	{
		rows = matrix.rows; // otherwise we equalize the number of rows and columns of the matrixes 
		cols = matrix.cols;
		data = new data_type * [matrix.rows];
		for(int i = 0; i<matrix.rows; i++)
		{
			data[i] = new data_type [matrix.cols]; // we form a new 2D array with the same value of the matrix parameter 
		}
		for(int i = 0; i<rows; i++)
		{
			for(int j = 0; j<cols; j++)
			{
				data[i][j] = matrix.data[i][j];
			}
		}
	}
}

template <class data_type>
 const Matrix2D<data_type> & Matrix2D<data_type>::operator  =  (const Matrix2D<data_type> & rhs)
{
	if(this != &rhs) // we first check if the equalization operands are exactly at the same address thus have the same value
	{
		for(int i = 0; i<rhs.rows; i++) // then we assign the value of the each index of the right hand side to the left hand side
		{
			for(int j = 0; j<rhs.cols; j++)
			{
				data[i][j] = rhs.data[i][j];
			}
		}
	}
	return *this;
}

 template <class data_type>
 const Matrix2D<data_type> & Matrix2D<data_type>::operator  +=  (const Matrix2D<data_type> & rhs) // since we can assume that the number of the rows and columns of the first and second matrix (assigned matrixes) are equal, we can directly add the value of the rhs operand to lhs opreand
 {
	 for(int i = 0; i<rows; i++)
	 {
		 for(int j = 0; j<cols; j++)
		 {
			 data[i][j] += rhs.data[i][j]; // we add the value of the right hand side to the left hand side
		 }
	 }
	 return *this; // after that we return the this pointer
 }

  template <class data_type>
  Matrix2D<data_type> &  operator + (const Matrix2D<data_type> & lhs, const Matrix2D<data_type> & rhs)
  {
	  Matrix2D<data_type> * ptr = new Matrix2D<data_type> (lhs); // we are using pointer which points to a newly created dynamic array of the deep copy of the lhs operand
	  for(int i = 0; i<lhs.rows; i++) // after that we add the value of the rhs to newly created dynamic array
	  {
		  for(int j = 0; j<lhs.cols; j++)
		  {
			  ptr->data[i][j] += rhs.data[i][j];
		  }
	  }
	  return *ptr; // at the end of the function we return the array of the pointers
  }

#endif
