

#include "parser.h"
#include "scanner.h"
#include "extToken.h"
#include "AST.h"
#include <stdio.h>
#include <assert.h>
#include <string>

using namespace std ;

Parser::~Parser() {
    if (s) delete s ;

    ExtToken *extTokenToDelete ;
    currToken = tokens ;
    while (currToken) {
        extTokenToDelete = currToken ;
        currToken = currToken->next ;
        delete extTokenToDelete ;
    }

    Token *tokenToDelete ;
    Token *currScannerToken = stokens ;
    while (currScannerToken) {
        tokenToDelete = currScannerToken ;
        currScannerToken = currScannerToken->next ;
        delete tokenToDelete ;
    }

}



Parser::Parser ( ) { 
    currToken = NULL; prevToken = NULL ; tokens = NULL; 
    s = NULL; stokens = NULL; 
}

ParseResult Parser::parse (const char *text) {
    assert (text != NULL) ;

    ParseResult pr ;
    try {
        s = new Scanner() ;
        stokens = s->scan (text) ;        
        tokens = extendTokenList ( this, stokens ) ;

        assert (tokens != NULL) ;
        currToken = tokens ;
        pr = parseProgram( ) ;
    }
    catch (string errMsg) {
        pr.ok = false ;
        pr.errors = errMsg ;
        pr.ast = NULL ;
    }
    return pr ;
}

/*! 
 * parse methods for non-terminal symbols
 * --------------------------------------
 */


/*! Program*/
ParseResult Parser::parseProgram () {
    ParseResult pr ;
    /*! root
     Program ::= varName '(' ')' '{' Stmts '}' 
     */
    match(variableName) ;
    string name( prevToken->lexeme ) ;
    
    VarName *n = new VarName (name);
    match(leftParen) ;
    match(rightParen) ;
    match(leftCurly);
    ParseResult prStmts = parseStmts() ;
    match(rightCurly);
    match(endOfFile) ;    
    Stmts *s = dynamic_cast<Stmts *> (prStmts.ast);
    pr.ast = new Root(n, s);    
    return pr ;
}


/*! MatrixDecl
 identical purpose of parseDecl, handles special matrix syntax.
 */
ParseResult Parser::parseMatrixDecl () {
    ParseResult pr ;
    match(matrixKwd);
    match(variableName) ;
    
	string name1 (prevToken -> lexeme);
	VarName *n1 = new VarName(name1);
	
    /*!Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';'
    */
    if(attemptMatch(leftSquare)){
        ParseResult r1 = parseExpr(0);
        match(comma);
        ParseResult r2 = parseExpr(0);
        match(rightSquare);
        parseVariableName();        
        string name2 (prevToken -> lexeme);
        VarName *n2 = new VarName(name2);
        match(comma);
        parseVariableName();        
        string name3(prevToken -> lexeme);
        VarName *n3 = new VarName(name3);        
        match(assign);
        ParseResult r3 = parseExpr(0);        
        Expr *e1=dynamic_cast<Expr *>(r1.ast);
        Expr *e2=  dynamic_cast<Expr *>(r2.ast);
        Expr *e3=dynamic_cast<Expr *>(r3.ast);         
      	pr.ast = new MatrixDecl1(n1, e1, e2, n2, n3, e3);
               
    }
    /*! Decl ::= 'Matrix' varName '=' Expr ';'
    */
    else if(attemptMatch(assign)){
        ParseResult r = parseExpr(0);        
        Expr *e = dynamic_cast<Expr *>(r.ast);
       	pr.ast = new MatrixDecl2(n1, e);
    }
    else{
        throw ( (string) "Bad Syntax of Matrix Decl in in parseMatrixDecl" ) ;
    }   

    match(semiColon) ;

    return pr ;
}
/*!standardDecl 
Decl ::= integerKwd varName | floatKwd varName | stringKwd varName
*/
ParseResult Parser::parseStandardDecl(){
    ParseResult pr ;
    string s;
    
    /*!ParseResult prType = parseType() ;
    */

    if ( attemptMatch(intKwd) ) {
        /*! Type ::= intKwd
        */
        s = prevToken -> lexeme;
    } 
    else if ( attemptMatch(floatKwd) ) {
        /*! Type ::= floatKwd
        */
        s = prevToken -> lexeme;
    }
    else if ( attemptMatch(stringKwd) ) {
        /*! Type ::= stringKwd
        */
        s = prevToken -> lexeme;
    }
    else if ( attemptMatch(boolKwd) ) {
        /*! Type ::= boolKwd
        */
        s = prevToken -> lexeme;
    }
    match(variableName) ;
    string name(prevToken -> lexeme);
    VarName *n = new VarName(name);
    
    match(semiColon) ;
    pr.ast = new NormalDecl(s, n);
    return pr ;
}

/*! Decl
*/
ParseResult Parser::parseDecl () {
    ParseResult pr ;
    /*! Decl :: Matrix variableName ....
    */
    if(nextIs(matrixKwd)){
        pr =parseMatrixDecl();
    } 
    /*! Decl ::= Type variableName semiColon
    */
    else{
        pr=parseStandardDecl();
    }
    return pr ;
}



/*! Stmts
*/
ParseResult Parser::parseStmts () {
    ParseResult pr ;
    if ( ! nextIs(rightCurly) && !nextIs(inKwd)  ) {
        /*!Stmts ::= Stmt Stmts
        */
        ParseResult prStmt = parseStmt() ;
        ParseResult prStmts = parseStmts() ;
        pr.ast = new NonEmptyStmts((Stmt*)prStmt.ast, (Stmts*)prStmts.ast);
    }
    else {
        /*! Stmts ::= 
         nothing to match.
         */
        pr.ast = new EmptyStmts();
    }
    return pr ;
}


/*! Stmt
*/
ParseResult Parser::parseStmt () {
    ParseResult pr ;

    /*!Stmt ::= Decl
    */
    if(nextIs(intKwd)||nextIs(floatKwd)||nextIs(matrixKwd)||nextIs(stringKwd)||nextIs(boolKwd)){
        ParseResult s1 = parseDecl();
        
        Decl* a = dynamic_cast<Decl*>(s1.ast);
    		pr.ast = new DeclStmt(a);
    }
    /*!Stmt ::= '{' Stmts '}'
    */
    else if (attemptMatch(leftCurly)){
        ParseResult s1 = parseStmts() ; 
        
        Stmts* b = NULL;        
		    b = dynamic_cast<Stmts*>(s1.ast);
		    pr.ast = new StmtsStmt(b);        
        match(rightCurly);
    }   
    /*!Stmt ::= 'if' '(' Expr ')' Stmt
    Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    */
    else if (attemptMatch(ifKwd)){
        match(leftParen);
        ParseResult e1 = parseExpr(0);
        match(rightParen);
        ParseResult s2 = parseStmt();
        
        Expr *c = NULL;
        Stmt* d = NULL;
        
        if(attemptMatch(elseKwd)){
            ParseResult s3 = parseStmt();
            Stmt* e = NULL;
				c = dynamic_cast<Expr*>(e1.ast);
				d = dynamic_cast<Stmt*>(s2.ast);
				e = dynamic_cast<Stmt*>(s3.ast);
			pr.ast = new IfElseStmt(c, d, e);
		}
		else {
				c = dynamic_cast<Expr*>(e1.ast);
				d = dynamic_cast<Stmt*>(s2.ast);
			pr.ast = new IfStmt(c, d);
        }

    }
    /*!Stmt ::= varName '=' Expr ';'  | varName '[' Expr ',' Expr ']' '=' Expr ';'
    */
    else if  ( attemptMatch (variableName) ) {
		string name(prevToken -> lexeme);
		VarName *n = new VarName(name);
		
        if (attemptMatch ( leftSquare ) ) {
			ParseResult e2 = parseExpr(0);
            match ( comma ) ;
            ParseResult e3 = parseExpr (0) ;
            match  ( rightSquare ) ; 
            match ( assign ) ;           
            ParseResult e4 = parseExpr(0);
            match(semiColon);
			  
	        Expr *f = NULL, *g = NULL, *h = NULL;
				f = dynamic_cast<Expr*>(e2.ast);
				g = dynamic_cast<Expr*>(e3.ast);
				h = dynamic_cast<Expr*>(e4.ast);
			pr.ast = new MatrixAssignStmt(n, f, g, h); 
		}
		else { 
			match(assign);
			ParseResult e5 = parseExpr(0);
			match(semiColon);
			
			Expr *i = NULL;
				i = dynamic_cast<Expr*>(e5.ast);
			pr.ast = new NormalAssignStmt(n, i); 
		}
	}

    /*!Stmt ::= 'print' '(' Expr ')' ';'
    */
    else if ( attemptMatch (printKwd) ) {
        match (leftParen) ;
        ParseResult e6 = parseExpr(0);
        match (rightParen) ;
        match (semiColon) ;
        
        Expr* j = NULL;
			j = dynamic_cast<Expr*>(e6.ast);
		pr.ast = new PrintStmt(j);
	}
	
    /*!Stmt ::= 'for' '(' varName '=' Expr ':' Expr ')' Stmt
    */
    else if ( attemptMatch (forKwd) ) {
        match (leftParen) ;
        match (variableName) ;
        string name(prevToken-> lexeme);
        VarName *n = new VarName (name);
        
        match (assign) ;
        ParseResult e7 = parseExpr (0) ;
        match (colon) ;
        ParseResult e8 = parseExpr (0) ;
        match (rightParen) ;
        ParseResult s4 = parseStmt () ;
        
        Expr *k = NULL, *l = NULL; 
        Stmt* m = NULL;
        
			k = dynamic_cast<Expr*>(e7.ast);
			l = dynamic_cast<Expr*>(e8.ast);
			m = dynamic_cast<Stmt*>(s4.ast);
		pr.ast = new ForStmt(n, k, l, m); 
    }
    
    /*!Stmt ::= 'while' '(' Expr ')' Stmt
    */
    else if (attemptMatch(whileKwd)) {
        match(leftParen);
        ParseResult e9 = parseExpr(0);
        match(rightParen);
        ParseResult s5 = parseStmt();
        
        Expr *n = NULL;
        Stmt *o = NULL;
        
		n = dynamic_cast<Expr*>(e9.ast);
			o = dynamic_cast<Stmt*>(s5.ast);
		pr.ast = new WhileStmt(n, o); }
    /*!Stmt ::= ';
    */
    else if ( attemptMatch (semiColon) ) {
        /*! parsed a skip
        */
        pr.ast = new SemiColonStmt();
    }
    else{
        throw ( makeErrorMsg ( currToken->terminal ) + " while parsing a statement" ) ;
    }
    /*! Stmt ::= variableName assign Expr semiColon
    */
    return pr ;
}



/*! Expr
*/
ParseResult Parser::parseExpr (int rbp) {
    /*! Examine current token, without consuming it, to call its
       associated parse methods.  The ExtToken objects have 'nud' and
       'led' methods that are dispatchers that call the appropriate
       parse methods.
    */
    ParseResult left = currToken->nud() ;
   
    while (rbp < currToken->lbp() ) {
        left = currToken->led(left) ;
    }

    return left ;
}


/*!
  parse methods for Expr productions
 ----------------------------------
 */

 /*! Expr ::= trueKwd
 */
 ParseResult Parser::parseTrueKwd ( ) {
     ParseResult pr ;
     match ( trueKwd ) ;
     pr.ast =  new SingleValueExpr(prevToken-> lexeme);
     return pr ;
 }

 /*! Expr ::= trueKwd
 */
 ParseResult Parser::parseFalseKwd ( ) {
     ParseResult pr ;
     match ( falseKwd ) ;
     pr.ast =  new SingleValueExpr(prevToken-> lexeme);
     return pr ;
 }

/*! Expr ::= intConst
*/
ParseResult Parser::parseIntConst ( ) {
    ParseResult pr ;
    match ( intConst ) ;
    pr.ast =  new SingleValueExpr(prevToken-> lexeme);
    return pr ;
}

/*! Expr ::= floatConst
*/
ParseResult Parser::parseFloatConst ( ) {
    ParseResult pr ;
    match ( floatConst ) ;
    pr.ast =  new SingleValueExpr(prevToken-> lexeme);
    return pr ;
}

/*! Expr ::= stringConst
*/
ParseResult Parser::parseStringConst ( ) {
    ParseResult pr ;
    match ( stringConst ) ;
    pr.ast =  new SingleValueExpr(prevToken-> lexeme);
    return pr ;
}

/*! Expr ::= variableName .....
*/
ParseResult Parser::parseVariableName ( ) {
    ParseResult pr ;
    match ( variableName ) ;
    string name(prevToken-> lexeme);
    VarName *n = new VarName(name);
    
    if(attemptMatch(leftSquare)){
        ParseResult r1 = parseExpr(0);
        match(comma);
        ParseResult r2 = parseExpr(0);
        match(rightSquare);
        
        Expr *e1 = dynamic_cast<Expr*>(r1.ast);
        Expr *e2 = dynamic_cast<Expr*>(r2.ast);

		pr.ast = new MatrixExpr(n, e1, e2); }
    /*!Expr ::= varableName '(' Expr ')'     (NestedOrFunctionCall)
    */
    else if(attemptMatch(leftParen)){
        ParseResult e3 = parseExpr(0);
        match(rightParen);
        
        Expr* c = dynamic_cast<Expr*>(e3.ast);        
	pr.ast = new NestedFunctionCallExpr(n, c);
        
    }
    /*!Expr := variableName
    */
    else{
        /*! variable 
        */
        pr.ast = new VarName(name);
    }
    return pr ;
}


/*! Expr ::= leftParen Expr rightParen
*/
ParseResult Parser::parseNestedExpr ( ) {
    ParseResult pr ;
    match ( leftParen ) ;
    ParseResult r1 = parseExpr(0) ; 
    match(rightParen) ;    
    Expr* e =dynamic_cast<Expr*>(r1.ast);
	  pr.ast = new ParenExpr(e);
    return pr ;
}

/*!Expr ::= 'if' Expr 'then' Expr 'else' Expr  
*/
ParseResult Parser::parseIfExpr(){  
     ParseResult pr ; 
        match(ifKwd);
    ParseResult r1 = parseExpr(0);
    match(thenKwd);
    ParseResult r2 = parseExpr(0);
    match(elseKwd);
    ParseResult r3 = parseExpr(0);
    
    Expr *e1 = dynamic_cast<Expr*>(r1.ast);
    Expr *e2 = dynamic_cast<Expr*>(r2.ast);
    Expr *e3 =dynamic_cast<Expr*>(r3.ast);
  	pr.ast = new IfThenElseExpr(e1, e2, e3);

    return pr;
}


/*! Expr ::= 'let' Stmts 'in' Expr 'end' 
*/
ParseResult Parser::parseLetExpr(){
   ParseResult pr ;
   match(letKwd);
   ParseResult s1 = parseStmts();
   match(inKwd);
   ParseResult e1 = parseExpr(0);
   match(endKwd);
	Stmts* ss = dynamic_cast<Stmts*>(s1.ast);
	Expr* e = dynamic_cast<Expr*>(e1.ast);
	pr.ast = new LetExpr(ss, e);

   return pr;
}

/*! Expr ::= '!' Expr 
*/
ParseResult Parser::parseNotExpr () {
    ParseResult pr ;
    match ( notOp ) ;
    ParseResult r = parseExpr( 0 );     
    Expr* e = dynamic_cast<Expr*>(r.ast);
   	pr.ast = new NotExpr(e);
    
    return pr ;

}
/*! Expr ::= Expr plusSign Expr
*/
ParseResult Parser::parseAddition ( ParseResult prLeft ) {
    /*! parser has already matched left expression.
    */
    ParseResult pr ;     
	Expr *left = dynamic_cast<Expr *> (prLeft.ast);	
	match(plusSign);
	string *op = new string(prevToken -> lexeme);	
	ParseResult prRight = parseExpr (prevToken -> lbp());
	Expr *right = dynamic_cast<Expr *> (prRight.ast);
	pr.ast = new BinOpExpr(left, op,right);
    return pr ;
}

/*! Expr ::= Expr star Expr
*/
ParseResult Parser::parseMultiplication ( ParseResult prLeft ) {
    /*! parser has already matched left expression 
    */
    ParseResult pr ;
     
	Expr *left = dynamic_cast<Expr *> (prLeft.ast);
	match(star);
	string *op = new string(prevToken -> lexeme);
	
	ParseResult prRight = parseExpr (prevToken -> lbp());
	Expr *right = dynamic_cast<Expr *> (prRight.ast);
	pr.ast = new BinOpExpr(left, op,right);

    return pr ;
}

/*! Expr ::= Expr dash Expr
*/
ParseResult Parser::parseSubtraction ( ParseResult prLeft ) {
    /*! parser has already matched left expression 
    */
    ParseResult pr ;
     
	Expr *left = dynamic_cast<Expr *> (prLeft.ast);
	match(dash);	string *op = new string(prevToken -> lexeme);
	ParseResult prRight = parseExpr (prevToken -> lbp());
	Expr *right = dynamic_cast<Expr *> (prRight.ast);
	pr.ast = new BinOpExpr(left, op,right);

    return pr ;
}

/*! Expr ::= Expr forwardSlash Expr
*/
ParseResult Parser::parseDivision ( ParseResult prLeft ) {
    /*! parser has already matched left expression 
    */
    ParseResult pr ;
     
	Expr *left = dynamic_cast<Expr *> (prLeft.ast);
	match(forwardSlash);
	string *op = new string(prevToken -> lexeme);
	
	ParseResult prRight = parseExpr (prevToken -> lbp());
	Expr *right = dynamic_cast<Expr *> (prRight.ast);
	
	pr.ast = new BinOpExpr(left, op,right);

    return pr ;
}


/*! Expr ::= Expr equalEquals Expr
 Expr ::= Expr lessThanEquals Expr
 Expr ::= Expr greaterThanEquals Expr
 Expr ::= Expr notEquals Expr
 Expr ::= Expr leftAngle Expr
 Expr ::= Expr rightAngle Expr
*/
/*!Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
 */
ParseResult Parser::parseRelationalExpr ( ParseResult prLeft ) {
    /*! parser has already matched left expression 
    */
    ParseResult pr ;
	
	Expr *left = dynamic_cast<Expr *> (prLeft.ast);
    nextToken( ) ;
    string *op = new string(prevToken->lexeme) ;

    ParseResult prRight = parseExpr( prevToken->lbp() ); 
    
    Expr *right = dynamic_cast<Expr *> (prRight.ast);
	
	pr.ast = new BinOpExpr (left, op, right);
	
    return pr ;
}


/*! Helper function used by the parser.
*/
void Parser::match (tokenType tt) {
    if (! attemptMatch(tt)) {
        throw ( makeErrorMsgExpected ( tt ) ) ;
    }
}

bool Parser::attemptMatch (tokenType tt) {
    if (currToken->terminal == tt) { 
        nextToken() ;
        return true ;
    }
    return false ;
}

bool Parser::nextIs (tokenType tt) {
    return currToken->terminal == tt ;
}

void Parser::nextToken () {
    if ( currToken == NULL ) 
        throw ( string("Internal Error: should not call nextToken in unitialized state"));
    else 
    if (currToken->terminal == endOfFile && currToken->next == NULL) {
        prevToken = currToken ;
    } else if (currToken->terminal != endOfFile && currToken->next == NULL) {
        throw ( makeErrorMsg ( "Error: tokens end with endOfFile" ) ) ;
    } else {
        prevToken = currToken ;
        currToken = currToken->next ;
    }
}

string Parser::terminalDescription ( tokenType terminal ) {
    Token *dummyToken = new Token ("",terminal, NULL) ;
    ExtToken *dummyExtToken = extendToken (this, dummyToken) ;
    string s = dummyExtToken->description() ;
    delete dummyToken ;
    delete dummyExtToken ;
    return s ;
}

string Parser::makeErrorMsgExpected ( tokenType terminal ) {
    string s = (string) "Expected " + terminalDescription (terminal) +
        " but found " + currToken->description() ;
    return s ;
}

string Parser::makeErrorMsg ( tokenType terminal ) {
    string s = "Unexpected symbol " + terminalDescription (terminal) ;
    return s ;
}

string Parser::makeErrorMsg ( const char *msg ) {
    return msg ;
}

