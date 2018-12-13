#ifndef GLOBALSET_HH
#define GLOBALSET_HH
#include<string>
#include<iostream>
#include <vector>
 
using namespace std;
 

class EvtGlobalSet {
  
public:
  EvtGlobalSet(){}
  //deconstructor
  virtual ~EvtGlobalSet(){}
  
  static std::vector<double> DV;
  static std::vector<int> IV;
  static std::vector<std::string> SV;
  static bool ConExcPythia;
  static std::vector<std::vector<double> > dVV;
  static std::vector<std::vector<int> > iVV;
private:
 
};
 
#endif
