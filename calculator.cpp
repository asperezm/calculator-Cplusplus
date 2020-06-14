#include "calculator.h"
#include "parser.h"
#include "ast.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

Calculator::Calculator():
  memory(0), end_of_line(false), id(false), file_ewe(true)
{}

int Calculator::eval(string expr, bool compilated) {

  Parser* parser = new Parser(new istringstream(expr), compilated);

  AST* tree = parser->parse(this);

  int result = tree->evaluate();

  delete tree;

  delete parser;

  return result;
}

void Calculator::store(int val) {
  memory = val;
}

int Calculator::recall() {
  return memory;
}

void Calculator::plus(int val) {
  memory = memory + val;
}

void Calculator::minus(int val) {
  memory = memory - val;
}

int Calculator::clear() {
  memory = 0;
  return memory;
}

int Calculator::Getidentifier(string str){
  if(identificador.count(str) <= 0){
    identificador[str] = 0;
    return identificador[str];
  
  }
  else{
    return identificador[str];
  }
}


void Calculator::Setidentifier(string str, int a){
  identificador[str] = a;
}

string Calculator::getKeys() {
  string keys;
  int count = 7;
  for(map<string, int>::iterator it = identificador.begin(); it != identificador.end(); ++it){
    keys = keys + "equ " + it->first + " M[" + to_string(count)+ "] \n";
    count++;
  }

  return keys;
}

string Calculator::compile(string expr){
  ostringstream os;
  os << "# Expresion " << expr << ":\n"
      << "# Instrucciones antes del recorrido del arbol abstracto sintactico\n"
      << "sp := 1000\n"
      << "one := 1\n"
      << "zero := 0\n";
  if(file_ewe){
    for (map<string, int>::iterator it = identificador.begin(); it != identificador.end(); ++it) {
      // it->first
      // it->second
      os << it->first << " := "<< it->second << "\n";
    }
    set_file_Ewe();
  }
  
  os << "# Comienza el recorrido del arbol\n";
  string oss = os.str();
  Parser* parser = new Parser(new istringstream(expr), file_ewe);

  AST* tree = parser->parse(this);

  string prueba=tree->compile();
  string total = oss + prueba;
  delete tree;

  delete parser;

  return total;
}

void Calculator::setID(){
  id = !id;
}

void Calculator::set_file_Ewe(){
  file_ewe = false;
}