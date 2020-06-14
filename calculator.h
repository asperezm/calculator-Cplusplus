#pragma once

#define calculator_h
#include <map>
#include <string>
 
using namespace std;


class Calculator {
 public:
   Calculator();

  int eval(string expr, bool compilated);
   void store(int val);
   void plus(int val);
   void minus(int val);
   int clear();
   int recall();
   void Setidentifier(string str, int a);
   int Getidentifier(string str);
   string getKeys();
   string compile(string expr);
   void setID();
   void set_file_Ewe();
private:
  int memory;
  bool id, file_ewe, end_of_line;
  map<string, int> identificador;
};

extern Calculator* calc;