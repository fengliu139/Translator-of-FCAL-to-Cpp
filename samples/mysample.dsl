/* Sample test using FCAL language
 */
main(){   
   Int Sum; 
   Int Num; 
   Bool Flag; 
   Sum = 0;
   Num = 1;
   Flag = True;

   while (Flag) {
	 Sum = Sum + Num;
	 Num = Num + 1;
    if (Num > 10) Flag = False ;
	}
}
