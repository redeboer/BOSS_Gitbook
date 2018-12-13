#include "EvtParityC.hh"

std::map<string, double> parityC::_parityCMap_;


void parityC::readParityC(){
  string location = getenv("BESEVTGENROOT");
  location += "/share/_parityC.list_";
  //-- debugging
  cout<<"The _parity.list_ location is "<<location<<endl;
  //--
  ifstream file(location.c_str());
  string parname;
  double pval;
  if(file==0){cout<<" parity::readParity: No particle parity list are available"<<endl;abort();}
  while(!file.eof()){
    file>>parname>>pval;
    pair<string, double> prt(parname,pval);
    //-- for debugging
    //cout<<"parname, pval "<<parname<<" "<<pval<<endl;

    _parityCMap_.insert(prt);
  }
}

double parityC::getC(string parname){
  double thepar;
  thepar = _parityCMap_[parname];
  //if(thepar ==0 ){cout<<"parity::getP::No entries in parity list available for "<<parname<<endl; abort();}
  return thepar;
} 
