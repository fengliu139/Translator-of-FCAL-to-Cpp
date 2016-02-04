#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "Matrix.h"

Matrix :: Matrix(int i, int j) {
	rows = i;
	cols = j;
	data = new float[i*j];
}

Matrix :: Matrix (const Matrix& m) {
	rows = m.rows;
	cols = m.cols;
	data = m.data;
}

int Matrix :: numRows ( ) { return rows; }
int Matrix :: numCols ( ) { return cols; }

float* Matrix :: access(const int i, const int j) const {
	return &data[i*cols + j];
}
 
std::ostream& operator<<(std::ostream &os, Matrix &m) {
	int rows = m.rows;
	int cols = m.cols;
	
	os<< rows << " " << cols << "\n";
	for (int i = 0; i< rows; i++) {
		for (int j = 0; j< cols; j++) {
			os << m.data[i*cols + j] << "  ";
		}
		os<<"\n";
	}
	return os;
}

Matrix Matrix :: readMatrix ( std::string filename ) {
	std::ifstream m;
	int i = 0, j = 0, r, c;
	
	m.open(filename.c_str());
	
	if(!m.is_open()) exit(1);
	
	m >> r;
	m >> c;

	Matrix *mat = new Matrix(r, c);	
	
	while(i!= r) {
		j = 0;
		while (j != c) {
			m >> *mat->access(i, j);
			j = j +1;
		}
		i = i +1;
	}
			
	m.close();
	return *mat;
}

int numRows (Matrix m) { return m.numRows(); }
int numCols (Matrix m) { return m.numCols(); }


