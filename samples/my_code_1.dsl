/*  matrix multiplication */
main(){
	Matrix matrix1 = readMatrix ( "../samples/my_code_1_matrix1.data") ;
	Matrix matrix2 = readMatrix ( "../samples/my_code_1_matrix2.data") ;
	
	Int rows1;
	Int cols1;
	Int rows2;
	Int cols2;
	rows1 = numRows(matrix1);
	cols1 = numCols(matrix1);
	rows2 = numRows(matrix2);
	cols2 = numCols(matrix2);
	
	Int i;
	Int j;
	Int k;
	
	Matrix result [rows1, cols2] i, j = 0;
	
	i = 0;
	j = 0;
	k = 0;
	
	for (i =0 : rows1-1) {
		for(j = 0 :  cols2 -1 ) {
			for ( k =0 : cols1-1) {
				result[i, j] = result[i, j] + matrix1[i, k]* matrix2[k, j];
				}
			}
		}	
			
	print ( result);
	}
