#pragma once
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

class AST {
 public:
   AST();
   virtual ~AST() = 0;
   virtual int evaluate() = 0;
   virtual string compile() = 0;
};

class BinaryNode : public AST {
 public:
   BinaryNode(AST* left, AST* right);
   ~BinaryNode();

   AST* getLeftSubTree() const;
   AST* getRightSubTree() const;

 private:
   AST* leftTree;
   AST* rightTree;
};

class UnaryNode : public AST {
 public:
   UnaryNode(AST* sub);
   ~UnaryNode();

   AST* getSubTree() const;

 private:
   AST* subTree;
};

class AddNode : public BinaryNode {
 public:
   AddNode(AST* left, AST* right);
   
   int evaluate();
   string compile();
};

class SubNode : public BinaryNode {
 public:
   SubNode(AST* left, AST* right);

   int evaluate();
   string compile();
};

class NumNode : public AST {
 public:
   NumNode(int n);

   int evaluate();
   string compile();
private:
   int val;
};

class TimesNode : public BinaryNode {
public:
  TimesNode(AST* left, AST* right);

  int evaluate();
  string compile();
};

class DivideNode : public BinaryNode {
public:
  DivideNode(AST* left, AST* right);

  int evaluate();
  string compile();
};

class RecallNode : public AST {
 public:
   RecallNode();

   int evaluate();
   string compile();
};

class StoreNode : public UnaryNode {
public:
  StoreNode(AST* sub);
  int evaluate();
  string compile();
};

class ModNode : public BinaryNode {
public:
  ModNode(AST* left, AST* right);

  int evaluate();
  string compile();
};

class PlusNode : public UnaryNode {
public:
  PlusNode(AST* sub);
  int evaluate();
  string compile();
};

class MinusNode : public UnaryNode {
public:
  MinusNode(AST* sub);
  int evaluate();
  string compile();

};

class Clear : public AST {
 public:
   Clear();

   int evaluate();
   string compile();
};

class IdNode : public AST{
 public:
  IdNode(string str);
  int evaluate();
  string compile();
 private:
  string str;
};

class AssignNode : public UnaryNode{
 public:
  AssignNode(string str ,AST* sub);
  ~AssignNode();

  int evaluate();
  string compile();

private:
  string str;

};