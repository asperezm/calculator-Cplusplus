#include "parser.h"
#include "calcex.h"
#include <string>
#include <sstream>

Parser::Parser(istream* in , bool compilated) : compilated(compilated){
   scan = new Scanner(in);
}

Parser::~Parser() {
   try {
      delete scan;
   } catch (...) {}
}

AST* Parser::parse(Calculator* calc) {
   return prog(calc);
}

AST* Parser::prog(Calculator* calc) {
  return ListExpr(expr());
}

AST* Parser::ListExpr(AST* e) {
   AST* result = nullptr;
   Token* t = scan->getToken();
   
   if (t->getType() != eof) {
     cout << " parser error" << endl;
     throw ParseError;
   }

   return e;
}

AST* Parser::expr() {
   return restExpr(term());
}

AST* Parser::restExpr(AST* e) {
   Token* t = scan->getToken();

   if (t->getType() == add) {
      return restExpr(new AddNode(e,term()));
   }

   if (t->getType() == sub) {
      return restExpr(new SubNode(e,term()));
   }
   scan->putBackToken();

   return e;
}

AST* Parser::term() {
  return restTerm(storable());
}

AST* Parser::restTerm(AST* e) {
  
   Token* t = scan->getToken();

   AST* result = nullptr;

   switch(t->getType()){
   case times :
     result = restTerm(new TimesNode(e,storable()));
     break;
     
   case divide:
     result = restTerm(new DivideNode(e,storable()));
     break;

   case module:
     result = restTerm(new ModNode(e,storable()));
     break;

   default:
     scan->putBackToken();
     result = e;
     break;
   }
   return result;
}

AST* Parser::storable() {
  return MemOperation(factor());
}

AST* Parser:: MemOperation(AST* e){
  Token *t = scan->getToken();
  if(t->getType() == identifier){
    return new IdNode(t->getLex());
  }
  
  if (t->getType() == keyword) {
    if (t->getLex() == "S") {
      return new StoreNode(e);
    }
    else if (t->getLex() == "P") {
      return new PlusNode(e);
    }
    else if (t->getLex() == "M") {
      return new MinusNode(e);
    }
    else {
      //cerr << "Expected S, P, M found: "
      //   << t->getLex() << endl;
      cerr << "Parser error" << endl;
      throw ParseError;
    }
  }
  else {
    scan->putBackToken();
  }
  return e;
 
}

AST* Parser::factor() {
Token* t = scan->getToken();

 if(t->getType() == identifier){
    return new IdNode(t->getLex());
  }

 
   if (t->getType() == number) {
      istringstream in(t->getLex());
      int val;
      in >> val;
      return new NumNode(val);
   }
    if (t->getType() == keyword) {
     if (t->getLex() == "R") {
      return  new RecallNode();
    }
     if (t->getLex() == "C") {
      return  new Clear();
    }
    else {
      cerr << "Expected R, C found: "
	   << t->getLex() << endl;
      throw ParseError;
    }
  }
    if (t->getType() == lparen) {
      AST *result = expr();
      t = scan->getToken();
      if (t->getType() != rparen) {
	cerr << "Expected )"
	     << endl;
	throw ParseError;
      }
      return result;
    }
    cerr << "Expected number, R, ("
         << endl;
    throw ParseError;
}
   