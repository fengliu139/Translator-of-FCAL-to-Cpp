
#include <string>
#include <iostream> 

#include "AST.h"




using namespace std;

/*!
In this file, we defined the constructor for all concrete classes
*/
string Root::unparse(){
string s;
s=varname -> unparse() + "(){" + stmts-> unparse () + "}";
return s;
}


string EmptyStmts::unparse() {
return "";
}

string NonEmptyStmts :: unparse() {
string s;
s=stmt -> unparse() + stmts -> unparse();
return s;
}


string DeclStmt :: unparse() {
string s;
s=decl -> unparse();
return s;
} 



string StmtsStmt :: unparse() {
string s;
s="{" + stmts-> unparse() + "}";
return s;
} 



string IfStmt:: unparse() {
string s;
s="if (" + expr -> unparse() + ")" + stmt -> unparse();
return s;
} 



string IfElseStmt :: unparse() {
string s;
s="if (" + expr -> unparse()  + ")"+stmt1-> unparse() + "else" + stmt2 -> unparse();
return s;
} 



string NormalAssignStmt :: unparse() {
string s;
s=varname -> unparse() + "=" + expr -> unparse() +";";
return s;
}


string MatrixAssignStmt :: unparse() {
string s;
s=varname-> unparse() + "[" + expr1 -> unparse() + "," + expr2 -> unparse() + "] = " + expr3 -> unparse();
return s;
} 



string PrintStmt :: unparse() {
string s;
s="print (" + expr -> unparse() + ");";
return s;
} 




string ForStmt :: unparse() {
string s;
s="for (" + varname -> unparse() + "=" + expr1 -> unparse() + ":" + expr2 -> unparse() + ")" + stmt -> unparse();
return s;
} 




string WhileStmt :: unparse() {
string s;
s="while (" + expr -> unparse() + ")" + stmt -> unparse();
return s;
} 



string SemiColonStmt :: unparse() {
return ";";
} 



string NormalDecl :: unparse() {
string s;
s=kwd + " " + varname -> unparse() + ";";
return s;
} 



string MatrixDecl1 :: unparse() {
string s;
s="Matrix " + varname1 -> unparse()  + "[ " + expr1 -> unparse() + "," + expr2-> unparse() + "] " + varname2 -> unparse() + ", " + varname3-> unparse() + "= " + expr3 -> unparse() + ";";
return s;
} 


string MatrixDecl2 :: unparse() {
string s;
s="Matrix " + varname -> unparse() + "= " + expr -> unparse() + ";";
return s;
} 


string VarName ::unparse(){
return lexeme;
}



string BinOpExpr ::unparse() {
string s;
s=left -> unparse() + " " + *op + " " + right -> unparse();
return s;
}



string SingleValueExpr::unparse(){
return constString;
}


string MatrixExpr :: unparse() {
string s;
s=varname-> unparse() + "[ " + left -> unparse() + "," + right -> unparse() + "]";
return s;
} 



string NestedFunctionCallExpr :: unparse() {
string s;
s=varname -> unparse() + "( " + expr -> unparse() + ")";
return s;
} 


string ParenExpr :: unparse() {
string s;
s="(" + expr -> unparse() + ")";
return s;
} 


string LetExpr :: unparse() {
string s;
s="let " + stmts -> unparse() + " in " + expr -> unparse() + " end";
return s ;
} 




string IfThenElseExpr :: unparse() {
string s;
s="if " + expr1 -> unparse() + " then " + expr2 -> unparse() + " else " + expr3 -> unparse();
return s;
} 




string NotExpr :: unparse() {
string s;
s="!" + expr -> unparse();
return s;
} 



string LetExpr :: cppCode() {
string s;
s="({" + stmts -> cppCode() + "\n" + expr -> cppCode() + ";})\n\n";
return s ;
} 

string NotExpr :: cppCode() {
string s;
s="!" + expr -> cppCode();
return s;
} 


string Root::cppCode() {
string s;
s = (string) "#include <iostream>\n#include \"Matrix.h\"\n#include <math.h>\nusing namespace std; \nint " +
varname -> cppCode() + "() {\n" +
stmts-> cppCode() + "\n}";
return s;
}


string EmptyStmts::cppCode() {
return "";
}


string NonEmptyStmts :: cppCode() {
string s;
s=stmt -> cppCode() + stmts -> cppCode();
return s;
}


string DeclStmt :: cppCode() {
string s;
s=decl -> cppCode();
return s;
} 


string StmtsStmt :: cppCode() {
string s;
s="{\t" + stmts-> cppCode() + "\n}\n";
return s;
} 


string IfStmt:: cppCode() {
string s;
s="if (" + expr -> cppCode() + ")" + stmt -> cppCode();
return s;
} 


string IfElseStmt :: cppCode() {
string s;
s="if (" + expr -> cppCode()  + ") \n" + stmt1-> cppCode() + "else" + stmt2 -> cppCode();
return s;
} 



string ParenExpr :: cppCode() {
string s;
s="(" + expr -> cppCode() + ")";
return s;
} 

string NormalAssignStmt :: cppCode() {
string s;
s=varname -> cppCode() + "=" + expr -> cppCode() +";\n";
return s;
}


string MatrixAssignStmt :: cppCode() {
string s;
s= "*" + varname-> cppCode() + ".access(" + expr1 -> cppCode() + "," + expr2 -> cppCode() + ") = " + expr3 -> cppCode() + ";\n";
return s;
} 



string PrintStmt :: cppCode() {
string s;
s="cout<<" + expr -> cppCode() + "; \n";
return s;
} 



string ForStmt :: cppCode() {
string s;
string var = varname -> cppCode();

s="for (" + var + "=" + expr1 -> cppCode() +"; " + var + " <= " + expr2 -> cppCode() + "; " + var + " ++) "  + stmt -> cppCode();
return s;
} 



string WhileStmt :: cppCode() {
string s;
s="while (" + expr -> cppCode() + ")" + stmt -> cppCode();
return s;
} 


string NormalDecl :: cppCode() {
string s;
if (kwd == "Int") {s = "int ";}
else if (kwd == "Float") {s = "float ";}
else {s = "string ";}
s += varname -> cppCode() + ";";
return s;
} 



string SemiColonStmt :: cppCode() {
return ";\n";
} 



string MatrixDecl1 :: cppCode() {
string s;
s = "Matrix " + varname1 -> cppCode() + "(" + expr1 -> cppCode() + ", " + expr2 -> cppCode() + "); \n";
string i; 
i = varname2 -> cppCode();
string j; 
j = varname3 -> cppCode();

s += "for (int " + i + " = 0; " + i + " < " + expr1 -> cppCode() + "; " + i + "++) {\n";
s+=	"for (int " + j + " = 0; " + j + " < " + expr2 -> cppCode() + "; " + j + "++) {\n";
s+=	"*(" + varname1 -> cppCode() + ".access(" + i + ", " + j + ")) = " + expr3 -> cppCode() + ";}\n}";
return s;
}



string MatrixDecl2 :: cppCode() {	
string s;
s = "Matrix " + varname -> cppCode() + "(Matrix ::" + expr -> cppCode() + ");  \n";
return s;
}


string BinOpExpr ::cppCode() {
string s;
s=left -> cppCode() + " " + *op + " " + right -> cppCode();
return s;
}


string IfThenElseExpr :: cppCode() {
string s;
s= expr1 -> cppCode() + "?" + expr2 -> cppCode() + " : " + expr3 -> cppCode() ;
return s;
} 



string SingleValueExpr::cppCode(){
return constString;
}


string VarName ::cppCode(){
return lexeme;
}


string MatrixExpr :: cppCode() {
string s;
s = "*" + varname -> cppCode() + ".access ("+ left -> cppCode() + ", " + right -> cppCode() + ")\n";
return s;
}


string NestedFunctionCallExpr :: cppCode() {
string s;
s=varname -> cppCode() + "( " + expr -> cppCode() + ")";
return s;
} 
