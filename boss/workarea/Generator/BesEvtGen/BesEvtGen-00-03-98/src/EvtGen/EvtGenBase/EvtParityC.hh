#ifndef PWAPARITYC_HH
#define PWAPARITYC_HH

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

class parityC{

public:
  static void readParityC();
  static double getC(string parname);


  virtual ~parityC(){}
private:
  static std::map<string, double> _parityCMap_;

};


#endif 
