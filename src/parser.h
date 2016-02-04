

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "parseResult.h"

#include <string>

class ExtToken ;

class Parser {

public:
    Parser() ;
    ~Parser() ;

    ParseResult parse (const char *text) ;
    // Parser methods for the nonterminals:

    ParseResult parseProgram () ;
    ParseResult parseDecl () ;
    ParseResult parseStandardDecl () ;
    ParseResult parseMatrixDecl () ;
    ParseResult parseStmts () ;
    ParseResult parseStmt () ;
    ParseResult parseExpr (int rbp) ;
    // methods for parsing productions for Expr
    ParseResult parseTrueKwd () ;
    ParseResult parseFalseKwd () ;
    ParseResult parseIntConst () ;
    ParseResult parseFloatConst () ;
    ParseResult parseStringConst () ;
    ParseResult parseCharConst () ;
    ParseResult parseVariableName () ;
    ParseResult parseNestedExpr () ;
    ParseResult parseNotExpr();
    ParseResult parseLetExpr() ;
    ParseResult parseIfExpr() ;
    ParseResult parseAddition ( ParseResult left ) ;
    ParseResult parseMultiplication ( ParseResult left ) ;
    ParseResult parseSubtraction ( ParseResult left ) ;
    ParseResult parseDivision ( ParseResult left ) ;

    ParseResult parseRelationalExpr ( ParseResult left ) ;

    // Helper function used by the parser.
    void match (tokenType tt) ;
    bool attemptMatch (tokenType tt) ;
    bool nextIs (tokenType tt) ;
    void nextToken () ;

    std::string terminalDescription ( tokenType terminal ) ;
    std::string makeErrorMsg ( tokenType terminal ) ;
    std::string makeErrorMsgExpected ( tokenType terminal ) ;
    std::string makeErrorMsg ( const char *msg ) ;

    ExtToken *tokens ;
    ExtToken *currToken ;
    ExtToken *prevToken ;

    Token *stokens ;
    Scanner *s ;
} ;

#endif /* PARSER_H */
