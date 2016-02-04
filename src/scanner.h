/* A small meaningless comment */
/* A small meaningless comment */
#ifndef SCANNER_H
#define SCANNER_H

#include <regex.h>
#include "regex.h"
#include <string>
using namespace std;


class Token ;

/* This enumerated type is used to keep track of what kind of
   construct was matched. 
 */

enum tokenEnumType { 

	intKwd, floatKwd,   boolKwd,
    trueKwd, falseKwd, stringKwd, matrixKwd,
	letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd,
	forKwd, whileKwd, printKwd,

	// Constants
	intConst, floatConst, stringConst, 

	// Names
	variableName ,

	// Punctuation
	leftParen, rightParen, 
	leftCurly, rightCurly, 
	leftSquare, rightSquare,

	comma, semiColon, colon,

	//Operators
	assign, 
	plusSign, star, dash, forwardSlash,
	lessThan, lessThanEqual, greaterThan, greaterThanEqual,
	equalsEquals, notEquals,
	andOp, orOp, notOp,

	// Special terminal types
	endOfFile ,
	lexicalError
} ;
typedef enum tokenEnumType tokenType ;

// Below you need to write your class definitions for Token and Scanner.
// the class token difinitions
class Token{
	public:
	tokenType terminal;
	std::string lexeme;
	Token *next;
	Token ();
	Token (std::string , tokenType , Token *);
	};


//the class scanner difinitions
class Scanner{
	public:
        regex_t *tt[43];
        regex_t *whiteSpace;
        regex_t *blockComment;
        regex_t *lineComment;
	    Token *scan (const char *);
        Scanner();
};


#endif /* SCANNER_H */
