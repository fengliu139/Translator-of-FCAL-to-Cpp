# Translator-of-FCAL-to-Cpp
 This is a translator that takes FCAL (forest cover analysis language) program file as input, recognizes the tokens 
in the program using regular expression, parses the sentences and translates it to C++;

The translator consists of a Scanner (Regex Matching and token generation), a Parser (Takes the linked list of tokens and creates nodes for the AST) and
an AST (Abstract Syntax Tree that holds all the nodes in the proper hierarchy). 
