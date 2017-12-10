/**
 * matrix.h - declaration of matrix class.  This class will be
 * capable of represting a Matrix as a first-class type.
 * 
 * Do not change any public methods in this file unless otherwise instructed.
 *
 * For CS321.  (c) 2014 Dr. Darrin Rothe
 */
 
 
// compile guard 
#ifndef MATRIX_H
#define MATRIX_H
 
#include <iostream>     // for std::ostream
#include <stdexcept>	// for std::runtime_error
#include <string>	// used in exception
 
// a helper class to bundle a message with any thrown exceptions.  To use, 
// simply 'throw matrixException("A descriptive message about the problem")'
// This will throw the exception object by value.  Recommendation is to 
// catch by reference (to prevent slicing).
class matrixException:public std::runtime_error
{
	public:
		matrixException(std::string message):
		      std::runtime_error((std::string("Matrix Exception: ") + 
		               message).c_str()) {}
};
 
 
class matrix
{
	public:
		// No-Arg Constructor - create Matrix and clear cells
		// Make a 1x1 "default" Matrix
		matrix();
 
		// Constructor - create Matrix and clear cells.  If rows or cols
		// is < 1, throw a matrixException.  Note, we will not use
		// "exception specifications" as multiple sources report that the
		// mechanism is not properly supported by most compilers.
		//
		// throw (matrixException)
		//
		matrix(unsigned int rows, unsigned int cols);
 
		// Copy constructor - make a new Matrix just like rhs
		matrix(const matrix& from);
 
		// Destructor.  Free allocated memory
		~matrix();
 
		// Assignment operator - make this just like rhs.  Must function
		// correctly even if rhs is a different size than this.
		matrix& operator=(const matrix& rhs);
 
		// Matrix addition - lhs and rhs must be same size otherwise
		// an exception shall be thrown
		//
		// throw (matrixException)
		//
		matrix operator+(const matrix& rhs) const;
 
		// Matrix multiplication - lhs and rhs must be compatible otherwise
		// an exception shall be thrown
		//
		// throw (matrixException)
		//
		matrix operator*(const matrix& rhs) const;
 
		// Scalar multiplication.  Note, this function will support
		// someMatrixObject * 5.0, but not 5.0 * someMatrixObject.
		matrix operator*(const double scale) const;

		// Inverse of a square Matrix - must be invertible, otherwise an
		// exception shall be thrown.  An invertible matrix is square and
		// has a non-zero determinate.
		//
		// Implementing Gauss-Jordan elimination is recommended as detailed
		// here: http://en.wikipedia.org/wiki/Gaussian_elimination
		// #Finding_the_inverse_of_a_matrix
		//
		// You may wish to build "helper" methods for creating the augmented
		// matrix, operating on the rows, etc, but they are not explicitly
		// required.  The implementation should work for any size square
		// matrix.
		//
		// As at least a 3rd-year CE student, you are expected to be able to
		// perform independent research and implement various algorithms
		// as needed.
		//
		// throw (matrixException)
		//
		matrix operator!() const;
 
		// Clear Matrix to all members 0.0
		void clear();
  
		// Access Operators - throw an exception if index out of range
		//
		// Note how these operators are to work.  Consider a Matrix object
		// being addressed with two sets of brackets - m1[1][2], for example.
		// The compiler will execute this: (m1[1])[2].  The first set of
		// brackets will call this function, and this function should return a
		// pointer to the first element of the requested row.  The second set
		// of brackets is applied to the double*, which results in it being
		// treated as an array, thus the requested column is indexed.  The
		// const version is necessary if you would like to use the operator
		// within other const methods.  Both of these operators are extremely
		// dangerous as prototyped.  The nature of the danger and a fix are
		// left up to you to discover and fix.  A proper fix will
		// require a change to these function signatures and the use of an
		// internal "helper class."
		//
		// throw (matrixException)
		//
		double* operator[](unsigned int row);
 
		// const version of above - throws an exception if indices are out of
		// range
		//
		// throw (matrixException)
		//
		double* operator[](unsigned int row) const;
 
		// I/O - for convenience - this is intended to be called by the global
		// << operator declared below.
		std::ostream& out(std::ostream& os) const;
 
	private:
		// The data - note, per discussion on arrays, you can store these data
		// as a 1-D dynamic array, thus the double* below.  Alternatively, and
		// perhaps preferred, you could store the data as an array of arrays
		// which would require the_Matrix to be changed to a double**.
		double** the_matrix;
		unsigned int rows;
		unsigned int cols;

		/** routines **/

		//Finds the transpose of the matrix
		matrix transpose(const matrix rhs) const;

		//Finds the Matrix of Minors for the matrix
		//Each element in the Matrix of Minors is the determinant of the
		//remaining elements if you were to take out the row and column
		//of the element in question
		matrix createMoM(const matrix rhs) const;

		//Finds the determinant of a given matrix
		//Finds the sum of the product of each downward diagonal, then subtracts
		//the sum of the product of each upward diagonal
		//
		//For use in matrix inversion
		double getDeterminant(const matrix rhs) const;

};

/** Some Related Global Functions **/
 
// Overloaded global << with std::ostream as lhs, Matrix as rhs.  This method
// should generate output compatible with an ostream which is commonly used
// with console (cout) and files.  Something like:
// [[ r0c0, r0c1, r0c2 ]
//  [ r1c0, r1c1, r1c2 ]
//  [ r0c0, r0c1, r0c2 ]]
// would be appropriate.
//
// Since this is a global function, it does not have access to the private
// data of a Matrix object.  So, it will need to use the public interface of
// Matrix to do its job.  The method Matrix::out was added to Matrix
// specifically for this purpose.  The other option would have been to make
// it a "friend"
 
std::ostream& operator<<(std::ostream& os, const matrix& rhs);
 
// We would normally have a corresponding >> operator, but
// will defer that exercise until a later assignment.


// Scalar multiplication with a global function.  Note, this function will
// support 5.0 * someMatrixObject, but not someMatrixObject * 5.0
matrix operator*(const double scale, const matrix& rhs);

 
#endif
