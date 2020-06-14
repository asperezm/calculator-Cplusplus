#pragma once 

#include "ast.h"
#include "scanner.h"
#include "calculator.h"
class Parser {
 public:
  Parser(istream* in, bool compilated);
   ~Parser();

   AST* parse(Calculator* calc);


   AST* prog(Calculator* calc);
   AST* ListExpr(AST* e);
   AST* expr();
   AST* restExpr(AST* e);
   AST* term();
   AST* restTerm(AST* t);
   AST* storable();
   AST* MemOperation(AST* t);
   AST* factor();

private: 
   Scanner* scan;
   bool compilated;
};