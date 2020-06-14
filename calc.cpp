#include <iostream>
#include <sstream>
#include <string>
#include "calcex.h"
#include "calculator.h"
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
using namespace std;

Calculator* calc;
void noCompiler(ifstream&);
string compiler(ifstream&);

int main(int argc, char* argv[]) {
  string line;
  string calcRef = ".calc";
  bool compilated = false;
  bool hayArchivos = false;
  vector<string> archivos;
  string chain;
  ifstream archivo;
  bool ciclo = true;
  bool especial = true;
  try {

    calc = new Calculator();

    for (int i = 1; i < argc; i++) {
      if (i != argc) {
	chain = argv[i];
        string comando(argv[i]);
        if (comando == "-v") {

          string arg = argv[i + 1];
          int p = arg.find_first_of("=");
          string nombre = arg.substr(0,p);
          string valor = arg.substr(p + 1, arg.size());

          int valAux = 0;
          stringstream convert(valor);
          convert >> valAux;

          calc->Setidentifier(nombre, valAux);
        }else if(comando == "-c") {
          compilated = true;

        }else if(chain.size()>4 and chain.substr(chain.size()-5, chain.size())==".calc" && compilated){
	  string ewe;
	  string cat;
	  string dog;
	  archivo.open(argv[i], ios::in);
	  if(!archivo.fail()){
	    cat = argv[i];
	    int a = cat.length() - 5;
	    ewe= compiler(archivo);
	    dog = cat.substr(0,a);
	  }
	  ofstream archivo;
	  archivo.open(dog+".ewe");
	  string Porfin= ewe;
	  archivo << ewe << endl;
	  archivo << "# Write result \n";
	  archivo << "op1 := M[sp + 0] \n";
	  archivo << "sp := sp - one \n";
	  archivo << "writeInt(op1) \n";
	  archivo << "end : halt \n";
	  archivo << "equ memory M[0] \n";
	  archivo << "equ one M[1] \n";
	  archivo << "equ zero M[2] \n";
	  archivo << "equ op1 M[3] \n";
	  archivo << "equ op2 M[4] \n";
	  archivo << "equ sp M[5] \n";
	  archivo << calc->getKeys();

	  archivo << "equ stack M[1000] \n";

	  archivo.close();
	  ciclo = false;
	  especial = false;
        }else if(chain.size()>4 and chain.substr(chain.size()-5, chain.size())==".calc" && especial){

	  archivo.open(argv[i], ios::in);
	  if(!archivo.fail()){
	    noCompiler(archivo);
	    archivo.close();
	    ciclo = false;
	  }
	}
      }

    }
    while (ciclo) {
      string ewe;
      if (compilated) {
        cout << "> ";
	getline(cin, line);

	if (!cin) break;

	ofstream resp;
	string b;
	resp.open("a.ewe");

	b = calc->compile(line);
	string Porfin= b;
	resp << b << endl;
	resp << "# Write result \n";
	resp << "op1 := M[sp + 0] \n";
	resp << "sp := sp - one \n";
	resp << "writeInt(op1) \n";
	resp << "end : halt \n";
	resp << "equ memory M[0] \n";
	resp << "equ one M[1] \n";
	resp << "equ zero M[2] \n";
	resp << "equ op1 M[3] \n";
	resp << "equ op2 M[4] \n";
	resp << "equ sp M[5] \n";
	resp << calc->getKeys();


	// archivoEwe_ << "equ stack M[1000]";
	resp.close();
	if(!cin) break;


      }else{

	cout << "> ";
	getline(cin, line);
	if(!cin) break;
	int result = calc->eval(line, compilated);

	cout << "= " << result << endl;
      }
    }


    delete calc;
  }catch (Exception ex) {
    cout << "Program Aborted due to exception!" << endl;

  }


}

void noCompiler(ifstream &archivo) {
  string line;
  while(!archivo.eof()){
    try {
      getline(archivo, line);
      int result = calc->eval(line,false);

      cout << "= " << result << endl;
    }catch(Exception ex) {
      //cout << "Program Aborted due to exception!" << endl;
      //cout << "* parser error" << endl;
    }
  }
}

string compiler(ifstream &archivo) {
  string line;
  ostringstream as;
  while(!archivo.eof()){
    getline(archivo, line);
    as << calc->compile(line) << endl;

  }
  return as.str();
}