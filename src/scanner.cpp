// scanner.cpp provides linked lists of tokens
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>  
#include <string.h>
#include "scanner.h"
#include <regex.h>
#include "regex.h"
#include "readInput.h"

using namespace std;
Token::Token(){};

Token::Token(string s, tokenType t, Token *n){
	lexeme=s;
	terminal=t;
	next=n;
	};

Scanner::Scanner(){

//regular expressions for keyword	
tt[intKwd] = makeRegex ("^Int"); //regular expression for keyword Int
tt[floatKwd] = makeRegex ("^Float"); //regular expression for keyword Float
tt[boolKwd] = makeRegex ("^Bool");
tt[trueKwd] = makeRegex ("^True");
tt[falseKwd] = makeRegex ("^False");
tt[stringKwd] = makeRegex ("^Str"); //regular expression for keyword String
tt[matrixKwd] = makeRegex ("^Matrix"); //regular expression for keyword Matrix
tt[letKwd] = makeRegex ("^let"); //regular expression for keyword let
tt[inKwd] = makeRegex ("^in"); //regular expression for keyword in
tt[endKwd] = makeRegex ("^end"); //regular expression for keyword end
tt[ifKwd] = makeRegex ("^if"); //regular expression for keyword if
tt[thenKwd] = makeRegex ("^then"); //regular expression for keyword then
tt[elseKwd] = makeRegex ("^else"); //regular expression for keyword else
tt[forKwd] = makeRegex ("^for"); //regular expression for keyword for
tt[whileKwd] = makeRegex ("^while"); //regular expression for keyword while
tt[printKwd] = makeRegex ("^print"); //regular expression for keyword print

// regular expressions for Constants
tt[intConst] = makeRegex ("^[0-9]+"); //regular expression for intConst
tt[floatConst] = makeRegex ("^[0-9]+\\.[0-9]*"); //regular expression for floatConst
tt[stringConst] = makeRegex ("^\"[^\"]*\""); //regular expression for stringConst

//regular expressions for Names
tt[variableName] = makeRegex ("^[a-zA-Z_]+[a-zA-Z0-9_]*"); //regular expression for variableName

//regular expressions for Punctuation
tt[leftParen] = makeRegex ("^\\("); //regular expression for (
tt[rightParen] = makeRegex ("^\\)"); //regular expression for )
tt[leftCurly]= makeRegex ("^\\{"); //regular expression for {
tt[rightCurly] = makeRegex ("^\\}"); //regular expression for }
tt[leftSquare] = makeRegex ("^\\["); //regular expression for [
tt[rightSquare] = makeRegex ("^\\]"); //regular expression for ]
tt[comma] = makeRegex ("^,"); //regular expression for ,
tt[semiColon] = makeRegex ("^;"); //regular expression for ;
tt[colon] = makeRegex ("^:"); //regular expression for :

//regular expressions for Operators
tt[assign] = makeRegex ("^="); //regular expression for =
tt[plusSign] = makeRegex ("^\\+"); //regular expression for + 
tt[star] = makeRegex ("^\\*"); //regular expression for *
tt[dash] = makeRegex ("^\\-"); //regular expression for -
tt[forwardSlash] = makeRegex ("^\\/"); //regular expression for /
tt[lessThan]= makeRegex ("^<"); //regular expression for <
tt[lessThanEqual] = makeRegex ("^<="); //regular expression for <= 
tt[greaterThan] = makeRegex ("^>"); //regular expression for >
tt[greaterThanEqual] = makeRegex ("^>="); //regular expression for >=
tt[equalsEquals] = makeRegex ("^=="); //regular expression for ==
tt[notEquals] = makeRegex ("^!="); //regular expression for !=
tt[andOp] = makeRegex ("^&&"); //regular expression for &&
tt[orOp] = makeRegex ("^\\|\\|"); //regular expression for || 
tt[notOp] = makeRegex ("^!");
whiteSpace = makeRegex ("^[\n\t\r ]+");

blockComment = makeRegex ("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");

lineComment = makeRegex ("^//[^\n]*\n");

} 



int consumeWhiteSpaceAndComments (regex_t *whiteSpace, 
                                  regex_t *blockComment,
                                  regex_t *lineComment,
                                  const char *text) ;
                                  


Token *Scanner::scan (const char *text) {
	


    
	

	int numMatchedChars;
	int maxNumMatchedChars = 0 ;
	numMatchedChars = consumeWhiteSpaceAndComments (whiteSpace, blockComment, lineComment, 
                                                   text) ;

    text = text + numMatchedChars ;
    Token *N_head, *N_tail;
    Token *N_new;
    N_head=new Token("",endOfFile,NULL);
    N_tail=N_head;
    char *lexeme_string ;
    int i;
    lexeme_string=(char *)malloc (1024);

   	while ( text[0] != '\0' ) {
		maxNumMatchedChars = 0 ;
		
		int ii;
		for(i=0;i<43;i++){
		
			numMatchedChars = matchRegex (tt[i], text) ;
			if (numMatchedChars > maxNumMatchedChars) {
				maxNumMatchedChars = numMatchedChars ;
				ii=i;
				}   	
		}

		if(maxNumMatchedChars == 0) {
				maxNumMatchedChars=1;
				N_new = new Token("",endOfFile,NULL);
				N_tail->next=N_new;
				N_tail->terminal=lexicalError;
								strncpy(lexeme_string,text,maxNumMatchedChars);
				lexeme_string[maxNumMatchedChars]='\0';
				N_tail->lexeme = lexeme_string;
				N_tail=N_new;
			
			}
		else{  
			    N_new = new Token("",endOfFile,NULL);
				N_tail->next=N_new;
				N_tail->terminal=(tokenEnumType)ii;
				strncpy(lexeme_string,text,maxNumMatchedChars);
				lexeme_string[maxNumMatchedChars]='\0';
				N_tail->lexeme = lexeme_string;
				N_tail=N_new;}
		text = text + maxNumMatchedChars;
		numMatchedChars = consumeWhiteSpaceAndComments (whiteSpace, blockComment, lineComment, 
                                                    text) ;
        text = text + numMatchedChars ;
		
		
	}
	free(lexeme_string);
	return N_head;
	
};


int consumeWhiteSpaceAndComments (regex_t *whiteSpace, 
                                  regex_t *blockComment, 
				  regex_t *lineComment,
                                  const char *text) {
    int numMatchedChars = 0 ;
    int totalNumMatchedChars = 0 ;
    int stillConsumingWhiteSpace ;

    do {
        stillConsumingWhiteSpace = 0 ;  // exit loop if not reset by a match

        // Try to match white space
        numMatchedChars = matchRegex (whiteSpace, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match block comments
        numMatchedChars = matchRegex (blockComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match single-line comments
        numMatchedChars = matchRegex (lineComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }
    }
    while ( stillConsumingWhiteSpace ) ;    

    return totalNumMatchedChars ;
}
