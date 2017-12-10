#include "matrix.h"
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

// no-arg "default" constructor
// makes a 1x1 matrix
matrix::matrix():rows(1),cols(1){  
  the_matrix = new double* [1];
  the_matrix[0] = new double [1];
  this->clear();
}


// Parameterized constructor
matrix::matrix(unsigned int rows, unsigned int cols):rows(rows),cols(cols){
  the_matrix = new double* [cols];
  for(int i=0;i<cols; i++){
    the_matrix[i] = new double [rows];
  }
  this->clear();
}

// Copy constructor
matrix::matrix(const matrix& from):rows(from.rows),cols(from.cols){

  //Create identically sized matrix
  the_matrix = new double* [cols];
  for(int i=0;i<cols; i++){
    the_matrix[i] = new double [rows];
  }
  
  //Copy values into new matrix
  for(int i=0; i<cols; i++){
    for(int j=0; j<rows; j++){
      the_matrix[i][j] = from.the_matrix[i][j];
    }
  }

}

// Destructor
matrix::~matrix(){
  for(int i=0; i<cols; i++){
    delete [] the_matrix[i];
  }
  delete [] the_matrix;
}

// Assignment operator
matrix& matrix::operator=(const matrix& rhs){
  if(this != &rhs){
    this->~matrix();

    //Recreate matrix
    this->the_matrix = new double* [rhs.cols];
    this->rows = rhs.rows;
    this->cols = rhs.cols;
    for(int i=0;i<cols; i++){
      this->the_matrix[i] = new double [rhs.rows];
    }
    
    //Copy values into new matrix
    for(int i=0; i<cols; i++){
      for(int j=0; j<rows; j++){
	this->the_matrix[i][j] = rhs.the_matrix[i][j];
      }
    }  
  }
  return *this;
}

// Binary operations
matrix matrix::operator+(const matrix& rhs) const{
  if(this -> rows == rhs.rows && this -> cols == rhs.cols){
    
    //Create identically sized matrix
    matrix retVal;
    retVal.the_matrix = new double* [rhs.cols];
    retVal.rows = rhs.rows;
    retVal.cols = rhs.cols;
    for(int i=0;i<cols; i++){
      retVal.the_matrix[i] = new double [rhs.rows];
    }
    
    //Assign sums to new matrix
    for(int i=0; i<cols; i++){
      for(int j=0; j<rows; j++){
	retVal.the_matrix[i][j] = rhs.the_matrix[i][j]+this->the_matrix[i][j];
      }
    }   
    return retVal;
  }else{
    throw matrixException("Matrices must be of identical size for '+' operation");
  }
}


matrix matrix::operator*(const matrix& rhs) const{
  if(this->cols == rhs.rows){
    //Create correctly sized matrix
    matrix retVal;
    retVal.the_matrix = new double* [rhs.cols];
    retVal.rows = this->rows;
    retVal.cols = rhs.cols;
    for(int i=0;i<cols; i++){
      retVal.the_matrix[i] = new double [this->rows];
    }
    
    //Assign products to new matrix
    for(int i=0; i<this->rows; i++){
      for(int j=0; j<rhs.cols; j++){
	retVal.the_matrix[i][j] = 0;
	for(int k=0; k<rhs.rows; k++){
	  retVal.the_matrix[i][j] += (rhs.the_matrix[i][k]*this->the_matrix[k][j]);
	}
      }
    }
    return retVal;
  }else{
    throw matrixException("Incorrect matrix dimensions for '*' operation");
  }
}

matrix matrix::operator*(const double scale) const{
  //Create identically sized matrix
    matrix retVal;
    retVal.the_matrix = new double* [this->cols];
    retVal.rows = this->rows;
    retVal.cols = this->cols;
    for(int i=0;i<cols; i++){
      retVal.the_matrix[i] = new double [this->rows];
    }
  
  //Assign products to new matrix
  for(int i=0; i<cols; i++){
    for(int j=0; j<rows; j++){
      retVal.the_matrix[i][j] = this->the_matrix[i][j]*scale;
    }
  }
  return retVal;
}


// Unary operations
matrix matrix::operator!() const{
  //Create identically sized matrix
  matrix retVal;
  retVal.the_matrix = new double* [this->cols];
  retVal.rows = this->rows;
  retVal.cols = this->cols;
  for(int i=0;i<cols; i++){
    retVal.the_matrix[i] = new double [this->rows];
  }
  
  if(rows == cols){
    if(rows == 1){
      retVal[0][0] = 1/this->the_matrix[0][0];
    }else{
      double dtm = getDeterminant(*this);
      if(dtm != 0){
	matrix MoM = createMoM(*this);
	MoM = transpose(MoM);
	retVal = MoM*(1/dtm);
      }else{
	throw matrixException("Matrix cannot be inverted: Determinant = 0");
      }
    }
  }else{
    throw matrixException("Matrix must be square in order to invert");
  }
  
  return retVal;
}

matrix matrix::transpose(const matrix rhs) const{
  matrix retVal;
  retVal.the_matrix = new double* [rhs.rows];
  retVal.rows = rhs.cols;
  retVal.cols = rhs.rows;
  for(int i=0;i<cols; i++){
    retVal.the_matrix[i] = new double [rhs.cols];
  }

  for(int i=0; i<rhs.rows; i++){
    for(int j=0; j<rhs.cols; j++){
      retVal.the_matrix[j][i] = rhs.the_matrix[i][j];
    }
  }
  return retVal;
}

matrix matrix::createMoM(const matrix rhs) const{
  //Create correctly sized matrix
    matrix MoMMatrix;
    MoMMatrix.the_matrix = new double* [rhs.cols];
    MoMMatrix.rows = rhs.rows;
    MoMMatrix.cols = rhs.cols;
    for(int i=0;i<cols; i++){
      MoMMatrix.the_matrix[i] = new double [rhs.rows];
    }

  //Populate with Matrix of Minors values
  matrix dtmMatrix = matrix(rows-1, cols-1);
  int m,n;
  for(int i=0; i<rhs.rows; i++){
    for(int j=0; j<rhs.cols; j++){
      m = 0;
      n = 0;
      for(int k=0; k<rhs.rows; k++){
	for(int l=0; l<rhs.cols; l++){
	  if(k != i && l != j){
	    dtmMatrix.the_matrix[m][n] = rhs.the_matrix[k][l];
	    if(n<(rows-2)){
	      n++;
	    }else{
	      n=0;
	      m++;
	    }
	  }
	}
      }
      MoMMatrix.the_matrix[i][j] = pow(-1, i+j)*getDeterminant(dtmMatrix);
    }
  }
  return MoMMatrix;
}

double matrix::getDeterminant(const matrix rhs) const{
  double dtm = 0;
  double diag;
  int k;

  if(rhs.rows == 1){
    dtm = rhs[0][0];
  }else if(rhs.rows == 2){
    dtm = rhs[0][0]*rhs[1][1] - rhs[1][0]*rhs[0][1];
  }else{
    
    //Positive Diagonals
    for(int i=0; i<rhs.cols; i++){
      diag = 1;
      for(int j=0; j<rhs.rows; j++){
	k = j;
	diag *= rhs.the_matrix[(i+k)%rhs.cols][j];
      }
      dtm += diag;
    }
    
    //Negative Diagonals
    for(int i=rhs.cols-1; i>-1; i--){
      diag = 1;
      for(int j=0; j<rhs.rows; j++){
	k = j;
	diag *= rhs.the_matrix[(i-k+rhs.cols)%rhs.cols][j];
      }
      dtm -= diag;
    }
  }
  return dtm;
}


void matrix::clear(){
  for(int i=0; i<cols; i++){
    for(int j=0; j<rows; j++){
      the_matrix[i][j] = 0.0;
    }
  }
}

double* matrix::operator[](unsigned int row){
  return the_matrix[row];
}

double* matrix::operator[](unsigned int row) const{
  return the_matrix[row];
}


std::ostream& matrix::out(std::ostream& os) const{
  os << '+';
  for(int i=0; i<cols*7; i++){
    os << '-';
  }
  os << '+' << endl;

  for(int i=0; i<rows; i++){
    os << '|';
    for(int j=0; j<cols; j++){
      os << setw(7) << setprecision(4) << the_matrix[j][i];
    }
    os << '|' << endl;
  }

  os << '+';
  for(int i=0; i<cols*7; i++){
    os << '-';
  }
  os << '+' << endl;

  return os;	
}

// Global insertion and operator
std::ostream& operator<<(std::ostream& os, const matrix& rhs){
  rhs.out(os);
  return os;
}

// Global scalar multiplication
matrix operator*(const double scale, const matrix& rhs){
  //Call mult*scale operator
  return rhs*scale;
}


