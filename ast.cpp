#include "ast.h"
#include <iostream>
#include "calculator.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
using namespace std;
// for debug information uncomment
//#define debug

AST::AST() {}

AST::~AST() {}

BinaryNode::BinaryNode(AST* left, AST* right):
   AST(),
   leftTree(left),
   rightTree(right)
{}

BinaryNode::~BinaryNode() {
#ifdef debug
   cout << "In BinaryNode destructor" << endl;
#endif

   try {
      delete leftTree;
   } catch (...) {}

   try {
      delete rightTree;
   } catch(...) {}
}
   
AST* BinaryNode::getLeftSubTree() const {
   return leftTree;
}

AST* BinaryNode::getRightSubTree() const {
   return rightTree;
}

UnaryNode::UnaryNode(AST* sub):
   AST(),
   subTree(sub)
{}

UnaryNode::~UnaryNode() {
#ifdef debug
   cout << "In UnaryNode destructor" << endl;
#endif

   try {
      delete subTree;
   } catch (...) {}
}
   
AST* UnaryNode::getSubTree() const {
   return subTree;
}

AddNode::AddNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int AddNode::evaluate() {
  return getLeftSubTree()->evaluate() + getRightSubTree()->evaluate();
}

string AddNode::compile(){
  ostringstream os;
  os <<  getLeftSubTree()->compile();
  os <<  getRightSubTree()->compile();
  os << "# Add" << endl;
  os << "op2 := M[sp+0]" << endl;
  os << "op1 := M[sp+1]" << endl;
  os << "op1 := op1 + op2" << endl;
  os << "sp := sp + one" << endl;
  os << "M[sp+0] := op1" << endl;
  return os.str();
}

SubNode::SubNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int SubNode::evaluate() {
   return getLeftSubTree()->evaluate() - getRightSubTree()->evaluate();
}

string SubNode::compile() {
  ostringstream os;
  os << getLeftSubTree()->compile();
  os << getRightSubTree()->compile();
  os << "# Sub\n"
      << "op2 := M[sp+0]\n"
      << "op1 := M[sp+1]\n"
      << "op1 := op1 - op2\n"
      << "sp := sp + one\n"
      << "M[sp+0] := op1\n";
  return os.str();
}

NumNode::NumNode(int n) :
   AST(),
   val(n)
{}

int NumNode::evaluate() {
   return val;
}

string NumNode::compile(){
  ostringstream os;
  os << "# push(" << val << ")\n"
      << "sp := sp - one\n"
      << "op1 := " << val <<"\n"
      << "M[sp+0] := op1\n";
  return os.str();
}

DivideNode::DivideNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int DivideNode::evaluate() {
   return getLeftSubTree()->evaluate() / getRightSubTree()->evaluate();
}

string DivideNode::compile(){
  ostringstream os;
  os << getLeftSubTree()->compile();
  os << getRightSubTree()->compile();
  os << "# Divide\n"
      << "op2 := M[sp+0]\n"
      << "op1 := M[sp+1]\n"
      << "op1 := op1 / op2\n"
      << "sp := sp + one\n"
      << "M[sp+0] := op1\n";
  return os.str();
}

TimesNode::TimesNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int TimesNode::evaluate() {
   return getLeftSubTree()->evaluate() * getRightSubTree()->evaluate();
}

string TimesNode::compile(){
  ostringstream os;
  os << getLeftSubTree()->compile();
  os << getRightSubTree()->compile();
  os  << "# Times\n"
      << "op2 := M[sp+0]\n"
      << "op1 := M[sp+1]\n"
      << "op1 := op1 * op2\n"
      << "sp := sp + one\n"
      << "M[sp+0] := op1\n";
  return os.str();
}


RecallNode::RecallNode() :
   AST()
{}

int RecallNode::evaluate() {
  return calc->recall();
}

string RecallNode::compile(){
  ostringstream os;
  os << "# Recall\n"
      << "sp := sp - one\n"
      << "M[sp+0] := memory\n";
  return os.str();
}

StoreNode::StoreNode(AST *sub) : UnaryNode(sub) { }

int StoreNode::evaluate(){
  calc->store(getSubTree()->evaluate());
    return calc->recall();
}

string StoreNode::compile(){
  ostringstream os;
  calc->store(getSubTree()->evaluate());
  os << getSubTree()->compile();
  os << "# Store\n"
      << "memory := M[sp+0]\n";
  return os.str();
}


ModNode::ModNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int ModNode::evaluate() {
   return getLeftSubTree()->evaluate() % getRightSubTree()->evaluate();
}

string ModNode::compile(){
  ostringstream os;
  os << getLeftSubTree()->compile();
  os << getRightSubTree()->compile();
  os << "# mod\n"
      << "op2 := M[sp+0]\n"
      << "op1 := M[sp+1]\n"
      << "op1 := op1 % op2\n"
      << "sp := sp + one\n"
      << "M[sp+0] := op1\n";
  return os.str();
}


PlusNode::PlusNode(AST *sub) : UnaryNode(sub) { }

int PlusNode::evaluate(){
  calc->plus(getSubTree()->evaluate());
    return calc->recall();
}

string PlusNode::compile(){
  ostringstream os;
  calc->plus(getSubTree()->evaluate());
  os << getSubTree()->compile();
  os << "# Memory Plus\n"
      << "op2 := M[sp+0]\n"
      << "memory := memory + op2\n"
      << "M[sp+0] := memory\n";
  return os.str();
}

MinusNode::MinusNode(AST *sub) : UnaryNode(sub) { }

int MinusNode::evaluate(){
  calc->minus(getSubTree()->evaluate());
    return calc->recall();
}

string MinusNode::compile(){
  ostringstream os;
    int result = getSubTree()->evaluate();
    calc->minus(result);
    os << getSubTree()->compile();
    os << "# Memory Minus\n"
      << "op2 := M[sp+0]\n"
      << "memory := memory - op2\n"
      << "M[sp+0] := memory\n";
  return os.str();
}

Clear::Clear() :
   AST()
{}

int Clear::evaluate() {
  return calc->clear();
}

string Clear::compile(){
  ostringstream os;
  calc->clear();
  os << "# Clear\n"
      << "memory := zero\n"
      << "sp := sp - one\n"
      << "M[sp+0] := memory\n";
  return os.str();
}


IdNode::IdNode(string str) :
  AST(),
  str(str)
{}

int IdNode::evaluate(){
  return calc->Getidentifier(str);
}

string IdNode::compile(){
  ostringstream os;
  os << "sp := sp - one\n"
      << "op1 := " << str <<"\n"
      << "M[sp+0] := op1\n";
  return os.str();
}

AssignNode::AssignNode(string str, AST* sub) : UnaryNode(sub), str(str) {}

AssignNode::~AssignNode() {}

int AssignNode::evaluate(){
  cout << str << "<-";
  calc->setID();
  calc->Setidentifier(str,getSubTree()->evaluate());
  return getSubTree()->evaluate();
}

string AssignNode::compile(){
  ostringstream os;
  os << getSubTree()->compile();
  os << "# Assign\n"
      << str << " := M[sp+0]\n";
  calc->Setidentifier(str,getSubTree()->evaluate());
  return os.str();
}