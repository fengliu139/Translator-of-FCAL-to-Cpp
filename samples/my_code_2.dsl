/* 2x3 matrix addtion */
main(){
	Matrix matrix1 = readMatrix ( "../samples/my_code_1_matrix1.data") ;
	Matrix matrix3 = readMatrix ( "../samples/my_code_2_matrix3.data") ;
	
	Int rows1;
	Int cols1;
	Int rows3;
	Int cols3;
	rows1 = numRows(matrix1);
	cols1 = numCols(matrix1);
	rows3 = numRows(matrix3);
	cols3 = numCols(matrix3);
	
	Int i;
	Int j;

	
	Matrix result [rows1, cols3] i, j = 0;
	
	i = 0;
	j = 0;

	
	while( i != rows1) {
		j=0;
		while( j != cols1){
			result[i, j] = matrix1[i, j] + matrix3[i, j];
			j = j+1;
		}
		i = i+1;
	}
	
	print(result);
	
}	
	
