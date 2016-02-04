#ifndef Node_H
#define Node_H

#include <string>
#include <iostream> 

#include "scanner.h"

class Node;
class Root;
class Stmts;
class VarName;
class Stmt;
class Decl;
class Expr;



/*!Node Class
*/
class Node {
	public:
		virtual std::string unparse ( ) { return " This should be pure virtual" ; } ;
		virtual std::string cppCode ( ) { return " This should be pure virtual" ; } ;
		virtual ~Node() { };
} ;
/*!
Root Class
This class matches Program ::= varName '(' ')' '{' Stmts '}'
a subclass of Node class
*/
class Root : public Node {
	public:
		Root(VarName *v, Stmts *s) : varname (v), stmts(s) {};
		std::string unparse();
		std::string cppCode();
		virtual ~Root() { };
	private:
		VarName *varname;
		Stmts *stmts;
		Root() : varname(NULL), stmts (NULL) {};
		Root(const Root&) {};
};

/*!These four classes are subclasses of Node class
*/
class Expr : public Node{};
class Stmts: public Node{};
class Stmt : public Node{};
class Decl : public Node{};


/*!This class matches Stmts ::= <<empty>>
*/
class EmptyStmts: public Stmts {
	public:
		EmptyStmts(){};
		std::string unparse();
		std::string cppCode();

};

/*!This class matches Stmts ::= Stmt Stmts
*/
class NonEmptyStmts: public Stmts {
	public:
		NonEmptyStmts(Stmt* s, Stmts* ss) : stmt(s), stmts(ss) {};
		std::string unparse();
		std::string cppCode();
	private:
		Stmt* stmt;
		Stmts* stmts;

};

/*!This class matches Stmt ::= Decl
*/

class DeclStmt: public Stmt {
	public:
		DeclStmt(Decl* d) : decl(d) {};
		std::string unparse();
		std::string cppCode();
	private:
		Decl* decl;

};
/*!This class matches Stmt ::= '{' Stmts '}'
*/
class StmtsStmt : public Stmt {
	public: 
		StmtsStmt(Stmts* ss) : stmts(ss) {};
		std::string unparse();
		std::string cppCode();
	private:
		Stmts* stmts;

};
/*!
This class matches Stmt ::= 'if' '(' Expr ')' Stmt 
*/	
class IfStmt : public Stmt {
	public:
		IfStmt(Expr* e, Stmt* s) : expr(e), stmt(s) {};
		std::string unparse();
		std::string cppCode();
	private:
		Expr* expr;
		Stmt* stmt;

};
/*!This class matches Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
*/	
class IfElseStmt : public Stmt {
	public:
		IfElseStmt(Expr* e, Stmt *s1, Stmt * s2) : expr (e), stmt1(s1), stmt2(s2) {}; 
		std::string unparse();
		std::string cppCode();
	private:
		Expr* expr;
		Stmt* stmt1;
		Stmt* stmt2;

};

/*!This class matches Stmt ::= varName '=' Expr ';' 
*/	
class NormalAssignStmt : public Stmt {
	public:
		NormalAssignStmt(VarName* v, Expr* e): varname(v), expr(e) {};
		std::string unparse();
		std::string cppCode();
	private:
		VarName *varname;
		Expr* expr;
};


/*!This class matches Stmt ::=  varName '[' Expr ',' Expr ']' '=' Expr ';'
*/	
class MatrixAssignStmt : public Stmt {
	public: 
		MatrixAssignStmt(VarName* v, Expr* e1, Expr* e2, Expr* e3):
			varname(v), expr1(e1), expr2(e2), expr3(e3) {};
		std::string unparse();
		std::string cppCode();
	private:
		VarName *varname;
		Expr* expr1;
		Expr* expr2;
		Expr* expr3;
};

/*!This class matches 'print' '(' Expr ')' ';'
*/	
class PrintStmt : public Stmt {
	public:
		PrintStmt(Expr* e) : expr(e) {};
		std::string unparse();
		std::string cppCode();
	private:
		Expr* expr;
};


/*!This class matches Stmt ::= 'for' '(' varName '=' Expr ':' Expr ')' Stmt
*/		
class ForStmt : public Stmt {
	public:
		ForStmt(VarName* v, Expr* e1, Expr* e2, Stmt* s) : varname(v), expr1(e1), expr2 (e2), stmt (s){};
		std::string unparse();
		std::string cppCode();
	private:
		VarName* varname;
		Expr* expr1;
		Expr* expr2;
		Stmt* stmt;
};


/*!This class matches Stmt ::= 'while' '(' Expr ')' Stmt
*/		
class WhileStmt : public Stmt {
	public:
		WhileStmt(Expr* e, Stmt* s) : expr(e), stmt(s) {};
		std::string unparse();
		std::string cppCode();
	private:
		Expr* expr;
		Stmt* stmt;

};


/*!This class matches Stmt ::= ';'
*/		
class SemiColonStmt : public Stmt {
	public:
		SemiColonStmt(){};
		std::string unparse();
		std::string cppCode();

};	

/*!This class matches the following:
        Decl ::= integerKwd varName ';'
        Decl ::= integerKwd varName ';'
        Decl ::= floatKwd varName ';'
        Decl ::= stringKwd varName ';'
        Decl ::= boolKwd varName ';'
*/		

class NormalDecl : public Decl {
	public:
		NormalDecl(std::string k, VarName* v) : kwd(k), varname(v) {};
		std::string unparse();
		std::string cppCode();
	private:
		std::string kwd;
		VarName* varname;
};


/*!This class matches Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';'
*/
class MatrixDecl1: public Decl {
	public:
		MatrixDecl1(VarName* v1, Expr* e1, Expr* e2, VarName * v2, VarName* v3, Expr* e3) :varname1(v1), expr1(e1), expr2(e2), varname2(v2), varname3 (v3), expr3(e3) {};
			
		std::string unparse();
		std::string cppCode();
	private:
		VarName *varname1;
    Expr* expr1;
    Expr* expr2;
		VarName *varname2;
		VarName * varname3;
   		Expr* expr3;   
};

/*!This class matches Decl ::= 'Matrix' varName '=' Expr ';'
*/
class MatrixDecl2 : public Decl {
	public:
		MatrixDecl2(VarName* v, Expr * e) : varname(v), expr(e) {};
		std::string unparse();
		std::string cppCode();
	private:
		VarName* varname;
		Expr* expr;
};


/*!This class matches Expr ::= varName
*/
class VarName : public Expr {
	public:
		VarName(std::string s) : lexeme(s) {};
		std::string unparse();
		std::string cppCode();
	private:
		std::string lexeme;

};

/*!This class matches the following:
        Expr ::= integerConst | floatConst |  stringConst
        Expr ::= trueKwd | falseKwd
*/
class SingleValueExpr : public Expr {
	public:
		SingleValueExpr(std::string s) : constString(s) {};
		std::string unparse();
		std::string cppCode();
	private:
		std::string constString;
};


/*!This class matches the following:
        Expr ::= Expr '*' Expr 
        Expr ::= Expr '/' Expr 
        Expr ::= Expr '+' Expr
        Expr ::= Expr '-' Expr
        Expr ::= Expr '>' Expr
        Expr ::= Expr '>=' Expr 
        Expr ::= Expr '<' Expr
        Expr ::= Expr '<=' Expr 
        Expr ::= Expr '==' Expr 
        Expr ::= Expr '!=' Expr 
        Expr ::= Expr '&&' Expr 
        Expr ::= Expr '||' Expr 
*/
class BinOpExpr : public Expr {
	public:
		BinOpExpr (Expr* _left, std::string *_op, Expr *_right) : left(_left), op(_op), right(_right) {};
		std::string unparse();
		std::string cppCode();
	private:
		Expr *left;
		std::string *op;
		Expr *right;
};


/*!This class matches Expr ::= varName '[' Expr ',' Expr ']'
*/
class MatrixExpr : public Expr {
	public:
		MatrixExpr(VarName* v, Expr* l, Expr* r) : varname(v), left(l), right (r) {};
		std::string unparse();
		std::string cppCode();
	private:
		VarName* varname;
		Expr* left;
		Expr* right;
};

/*!This class matches Expr ::= varName '(' Expr ')' 
*/
class NestedFunctionCallExpr : public Expr {
	public:
		NestedFunctionCallExpr(VarName* v, Expr* e) : varname(v), expr(e) {};
		std::string unparse();
		std::string cppCode();
	private:
		VarName* varname;
		Expr* expr;
};

/*!This class matches Expr ::= '(' Expr ')'  
*/
class ParenExpr : public Expr {
	public:
		ParenExpr(Expr *e) : expr(e) {};
		std::string unparse();
		std::string cppCode();
	private:
		Expr* expr;
};

/*!This class matches Expr ::= 'let' Stmts 'in' Expr 'end' 
*/
class LetExpr : public Expr {
	public:
		LetExpr(Stmts* s, Expr* e) : stmts(s), expr(e) {};
		std::string unparse();
		std::string cppCode();
	private:
		Stmts* stmts;
		Expr* expr;
};

/*!This class matches Expr ::= 'if' Expr 'then' Expr 'else' Expr 
*/
class IfThenElseExpr : public Expr {
	public:
		IfThenElseExpr(Expr* c, Expr* t, Expr* _e) : expr1(c), expr2(t), expr3(_e) {};
		std::string unparse();
		std::string cppCode();
	private:
		Expr* expr1;
		Expr* expr2;
		Expr* expr3;
};

/*!This class matches Expr ::= '!' Expr 
*/
class NotExpr : public Expr {
	public:
		NotExpr(Expr* e) : expr(e) {};
		std::string unparse();
		std::string cppCode();
	private:
		Expr* expr;
};

#endif

