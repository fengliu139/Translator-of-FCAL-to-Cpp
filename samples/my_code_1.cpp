#include <iostream>
#include "Matrix.h"
#include <math.h>
using namespace std; 
int main() {
Matrix matrix1(Matrix ::readMatrix( "../samples/my_code_1_matrix1.data"));  
Matrix matrix2(Matrix ::readMatrix( "../samples/my_code_1_matrix2.data"));  
int rows1;int cols1;int rows2;int cols2;rows1=numRows( matrix1);
cols1=numCols( matrix1);
rows2=numRows( matrix2);
cols2=numCols( matrix2);
int i;int j;int k;Matrix result(rows1, cols2); 
for (int i = 0; i < rows1; i++) {
for (int j = 0; j < cols2; j++) {
*(result.access(i, j)) = 0;}
}i=0;
j=0;
k=0;
for (i=0; i <= rows1 - 1; i ++) {	for (j=0; j <= cols2 - 1; j ++) {	for (k=0; k <= cols1 - 1; k ++) {	*result.access(i,j) = *result.access (i, j)
 + *matrix1.access (i, k)
 * *matrix2.access (k, j)
;

}

}

}
cout<<result; 

}
