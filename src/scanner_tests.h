#include <cxxtest/TestSuite.h>


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>  
#include <string.h>
#include "scanner.h"
#include <regex.h>
#include "regex.h"
#include "readInput.h"

using namespace std ;

class ScannerTestSuite : public CxxTest::TestSuite 
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    Scanner *s ;
    void test_setup_code ( ) {
        s = new Scanner() ;
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You may need to write several tests to adequately test the
       functions that are called from "scan".  Once you are confident
       that the components used by "scan" work properly, then you can
       run tests on the "scan" method itself.
    */

    // Here you should place tests for these other functions.


    /* Below is one of the tests for these components in the project
       solution created by your instructor..  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly ( ) {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{" ) ;
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */





    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the 
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (Token *tks) {
        Token *currentToken = tks ;
        while (currentToken != NULL) {
            if (currentToken->terminal == lexicalError) {
                printf("priblem: %s\n",currentToken->lexeme.c_str());
                return false ;
            }
            else {
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned. 
    */
    void scanFileNoLexicalErrors ( const char* filename ) {
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT (noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
    bool sameTerminals (Token *tks, int numTerms, tokenType *ts) {
        Token *currentToken = tks ;
        int termIndex = 0 ;
        while (currentToken != NULL && termIndex < numTerms ) {
            if (currentToken->terminal != ts[termIndex]) {
            printf("%i: %i should be %i\n",termIndex,currentToken->terminal , ts[termIndex]);
            fflush(stdout);
                return false ;
            }
            else {
                ++ termIndex ;
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty ( ) {
        Token *tks = s->scan ("  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    void test_scan_empty_comment ( ) {
        Token *tks = s->scan (" /* a comment */ ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    // When a lexical error occurs, the scanner creates a token with a 
    // single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors ( ) {
        Token *tks = s->scan ("$&1  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "$");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "&");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, intConst) ;
        TS_ASSERT_EQUALS (tks->lexeme, "1");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    //test cases:
    void test_terminal_all(const char * expression, Token* tokpoint){
        Token *tks = s->scan (expression);
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->lexeme, tokpoint->lexeme);
        TS_ASSERT_EQUALS (tks->terminal, tokpoint->terminal) ;
        TS_ASSERT (tks->next != NULL) ;
        TS_ASSERT_EQUALS (tks->next->terminal, endOfFile) ;

}

    //keyword
        void test_terminal_intKwd ( ) {
        Token* tokpoint = new Token("Int" , intKwd , NULL);
        test_terminal_all(" Int ", tokpoint);
    } //test cases for keyword  Int

     void test_terminal_floatKwd ( ) {
        Token* tokpoint = new Token("Float" , floatKwd , NULL);
        test_terminal_all(" Float ", tokpoint);
    } //test cases for keyword Float

     void test_terminal_boolKwd ( ) {
        Token* tokpoint = new Token("Bool" , boolKwd , NULL);
        test_terminal_all(" Bool ", tokpoint);
    } //test cases for keyword Float

     void test_terminal_trueKwd ( ) {
        Token* tokpoint = new Token("True" , trueKwd , NULL);
        test_terminal_all(" True ", tokpoint);
    } //test cases for keyword Float

     void test_terminal_falseKwd ( ) {
        Token* tokpoint = new Token("False" , falseKwd , NULL);
        test_terminal_all(" False ", tokpoint);
    } //test cases for keyword Float

     void test_terminal_stringKwd ( ) {
        Token* tokpoint = new Token("Str" , stringKwd , NULL);
        test_terminal_all(" Str ", tokpoint);
    } //test cases for keyword String

     void test_terminal_matrixKwd ( ) {
        Token* tokpoint = new Token("Matrix" , matrixKwd , NULL);
        test_terminal_all(" Matrix ", tokpoint);
    } //test cases for keyword Matrix

    void test_terminal_letKwd ( ) {
        Token* tokpoint = new Token("let" , letKwd , NULL);
        test_terminal_all(" let ", tokpoint);
    }  //test cases for keyword let

     void test_terminal_inKwd ( ) {
        Token* tokpoint = new Token("in" , inKwd , NULL);
        test_terminal_all(" in ", tokpoint);
    }  //test cases for keyword in

     void test_terminal_endKwd ( ) {
        Token* tokpoint = new Token("end" , endKwd , NULL);
        test_terminal_all(" end ", tokpoint);
    } //test cases for keyword end

    void test_terminal_ifKwd ( ) {
        Token* tokpoint = new Token("if" , ifKwd , NULL);
        test_terminal_all(" if ", tokpoint);
    } //test cases for keyword if

    void test_terminal_thenKwd ( ) {
        Token* tokpoint = new Token("then" , thenKwd , NULL);
        test_terminal_all(" then ", tokpoint);
    } //test cases for keyword then

    void test_terminal_elseKwd ( ) {
        Token* tokpoint = new Token("else" , elseKwd , NULL);
        test_terminal_all(" else ", tokpoint);
    } //test cases for keyword else

    void test_terminal_forKwd ( ) {
        Token* tokpoint = new Token("for" , forKwd , NULL);
        test_terminal_all(" for ", tokpoint);
    } //test cases for keyword for

    void test_terminal_whileKwd ( ) {
        Token* tokpoint = new Token("while" , whileKwd , NULL);
        test_terminal_all(" while ", tokpoint);
    } //test cases for keyword while

    void test_terminal_printKwd ( ) {
        Token* tokpoint = new Token("print" , printKwd , NULL);
        test_terminal_all(" print ", tokpoint);
    } //test cases for keyword print
    
    //constant
    void test_terminal_intConst ( ) {
        Token* tokpoint = new Token("2" , intConst , NULL);
        test_terminal_all(" 2 ", tokpoint);
    } //test cases for intConst

    void test_terminal_floatConst ( ) {
        Token* tokpoint = new Token("12.3" , floatConst , NULL);
        test_terminal_all(" 12.3 ", tokpoint);
    } //test cases for floatConst
    
    void test_terminal_stringConst ( ) {
        Token* tokpoint = new Token("\"yo\"" , stringConst , NULL);
        test_terminal_all(" \"yo\" ", tokpoint);
    }  //test cases for stringConst
    
    //Name
    void test_terminal_variableName ( ) {
        Token* tokpoint = new Token("foo" , variableName , NULL);
        test_terminal_all(" foo ", tokpoint);
    } //test cases for variable name
    
    //punctuation
    void test_terminal_leftParen ( ) {
        Token* tokpoint = new Token("(" , leftParen , NULL);
        test_terminal_all(" ( ", tokpoint);
    } //test cases for leftParen

    void test_terminal_rightParen ( ) {
        Token* tokpoint = new Token(")" , rightParen , NULL);
        test_terminal_all(" ) ", tokpoint);
    } //test cases for rightParen

    void test_terminal_leftCurly ( ) {
        Token* tokpoint = new Token("{" , leftCurly , NULL);
        test_terminal_all(" { ", tokpoint);
    } //test cases for leftCurly

    void test_terminal_rightCurly ( ) {
        Token* tokpoint = new Token("}" , rightCurly , NULL);
        test_terminal_all(" } ", tokpoint);
    } //test cases for rightCurly

    void test_terminal_leftSquare ( ) {
        Token* tokpoint = new Token("[" , leftSquare , NULL);
        test_terminal_all(" [ ", tokpoint);
    } //test cases for leftSquare

    void test_terminal_rightSquare ( ) {
        Token* tokpoint = new Token("]" , rightSquare , NULL);
        test_terminal_all(" ] ", tokpoint);
    } //test cases for rightSquare

    void test_terminal_comma ( ) {
        Token* tokpoint = new Token("," , comma , NULL);
        test_terminal_all(" , ", tokpoint);
    } //test cases for comma

    void test_terminal_semiColon ( ) {
        Token* tokpoint = new Token(";" , semiColon , NULL);
        test_terminal_all(" ; ", tokpoint);
    } //test cases for semiColon

    void test_terminal_colon ( ) {
        Token* tokpoint = new Token(":" , colon , NULL);
        test_terminal_all(" : ", tokpoint);
    } //test cases for colon
    
    //Operators
    void test_terminal_assign ( ) {
        Token* tokpoint = new Token("=" , assign , NULL);
        test_terminal_all(" = ", tokpoint);
    } //test cases for assign

    void test_terminal_plusSign ( ) {
        Token* tokpoint = new Token("+" , plusSign , NULL);
        test_terminal_all(" + ", tokpoint);
    } //test cases for plusSign

    void test_terminal_star ( ) {
        Token* tokpoint = new Token("*" , star , NULL);
        test_terminal_all(" * ", tokpoint);
    } //test cases for star

    void test_terminal_dash ( ) {
        Token* tokpoint = new Token("-" , dash , NULL);
        test_terminal_all(" - ", tokpoint);
    } //test cases for dash

    void test_terminal_forwardSlash ( ) {
        Token* tokpoint = new Token("/" , forwardSlash , NULL);
        test_terminal_all(" / ", tokpoint);
    } //test cases for forwardSlash

    void test_terminal_lessThan ( ) {
        Token* tokpoint = new Token("<" , lessThan , NULL);
        test_terminal_all(" < ", tokpoint);
    } //test cases for lessThan

    void test_terminal_lessThanEqual ( ) {
        Token* tokpoint = new Token("<=" , lessThanEqual , NULL);
        test_terminal_all(" <= ", tokpoint);
    } //test cases for lessThanEquals

    void test_terminal_greaterThan ( ) {
        Token* tokpoint = new Token(">" , greaterThan , NULL);
        test_terminal_all(" > ", tokpoint);
    } //test cases for greaterThan

    void test_terminal_greaterThanEqual ( ) {
        Token* tokpoint = new Token(">=" , greaterThanEqual , NULL);
        test_terminal_all(" >= ", tokpoint);
    } //test cases for greaterThanEquals

    void test_terminal_equalsEquals ( ) {
        Token* tokpoint = new Token("==" , equalsEquals , NULL);
        test_terminal_all(" == ", tokpoint);
    } //test cases for equalsEquals

    void test_terminal_notEquals ( ) {
        Token* tokpoint = new Token("!=" , notEquals , NULL);
        test_terminal_all(" != ", tokpoint);
    } //test cases for notEquals

    void test_terminal_andOp ( ) {
        Token* tokpoint = new Token("&&" , andOp , NULL);
        test_terminal_all(" && ", tokpoint);
    } //test cases for andOP

    void test_terminal_orOp ( ) {
        Token* tokpoint = new Token("||" , orOp , NULL);
        test_terminal_all(" || ", tokpoint);
    } //test cases for orOp

    void test_terminal_notOp ( ) {
        Token* tokpoint = new Token("!" , notOp , NULL);
        test_terminal_all(" ! ", tokpoint);
    } //test cases for notOp
    


    // A test for scanning numbers and a variable.
    void test_scan_nums_vars ( ) {
        Token *tks = s->scan (" 123 x 12.34 ") ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 4, ts ) ) ;
    }

   // more multi scanner
   void test_scan_nums_string ( ) {
        Token *tks = s->scan ("123 x 12.34 \"yo\" " ) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, stringConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 5, ts ) ) ;
    }
    void test_scan_nums_star ( ) {
        Token *tks = s->scan ("123 x 12.34 * " ) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, star, endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 5, ts ) ) ;
    }
    void test_scan_nums_leftParen ( ) {
        Token *tks = s->scan ("123 x 12.34 ( " ) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, leftParen, endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 5, ts ) ) ;
    }
    void test_scan_nums_leftParen_string ( ) {
        Token *tks = s->scan ("123 x 12.34 ( \"yo\" " ) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, leftParen, stringConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 5, ts ) ) ;
    }
    void test_scan_nums_leftParen_string_notEquals ( ) {
        Token *tks = s->scan ("123 x 12.34 ( \"yo\" !=" ) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, leftParen, stringConst, notEquals,endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 6, ts ) ) ;
    }




    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexeme
       are correct.
     */
    
    void test_scan_bad_syntax_good_tokens ( ) {
        const char *filename = "../samples/bad_syntax_good_tokens.dsl" ;
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { 
            intConst, intConst, intConst, intConst, 

            stringConst, stringConst, stringConst,

            floatConst, floatConst, floatConst,

                   matrixKwd,



            semiColon, comma, colon,
            leftCurly, leftParen, rightCurly, rightParen,

            plusSign, star, dash, forwardSlash, 

            equalsEquals, lessThanEqual, 
            greaterThanEqual, notEquals, 
            assign,

            variableName, variableName, variableName, variableName, 
            variableName, variableName, variableName,


            intKwd, floatKwd,  stringKwd,  whileKwd,

            endOfFile
       } ;
        int count = 39; 
        TS_ASSERT ( sameTerminals ( tks, count, ts ) ) ;
    }

    void test_scan_sample_forestLoss ( ) {
        scanFileNoLexicalErrors ("../samples/forest_loss_v2.dsl") ;
    }

      

} ;

