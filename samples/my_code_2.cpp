#include <iostream>
#include "Matrix.h"
#include <math.h>
using namespace std; 
int main() {
Matrix matrix1(Matrix ::readMatrix( "../samples/my_code_1_matrix1.data"));  
Matrix matrix3(Matrix ::readMatrix( "../samples/my_code_2_matrix3.data"));  
int rows1;int cols1;int rows3;int cols3;rows1=numRows( matrix1);
cols1=numCols( matrix1);
rows3=numRows( matrix3);
cols3=numCols( matrix3);
int i;int j;Matrix result(rows1, cols3); 
for (int i = 0; i < rows1; i++) {
for (int j = 0; j < cols3; j++) {
*(result.access(i, j)) = 0;}
}i=0;
j=0;
while (i != rows1){	j=0;
while (j != cols1){	*result.access(i,j) = *matrix1.access (i, j)
 + *matrix3.access (i, j)
;
j=j + 1;

}
i=i + 1;

}
cout<<result; 

}
