//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2006      Ping Rong-Gang @IHEP
//
// Module:  EvtDecayTag.cc
//
// Description:  Class to deal with the decay tag, e.g. the decay mode and multiplicity
//
// Modification history:
//
//    Ping R.-G.       December, 2011-Jan       Module created
//
//------------------------------------------------------------------------
//


#include <iostream>
#include "EvtGenBase/EvtDecayTag.hh"

using std::endl;
using std::fstream;

void EvtDecayTag::makeTag(EvtParticle* par){

  int ndaug = par->getNDaug();
  if(ndaug < 2){
    EvtId id = par->getId();
    int theTag = TagIndex(id);
    _multiplicity[theTag]++;
  } else {
  //--
    if(par == root_par) {
     int theMode = root_par->getChannel();
     _nmode.push_back(theMode);
    }
  //--
    int ND=0;
    EvtParticle *theD;
    for(int i=0;i<ndaug;i++){
      EvtParticle *theDaug = par->getDaug(i);
      if(par == root_par && theDaug->getNDaug()!=0){
        int theMode = theDaug->getChannel();
	_nmode.push_back(theMode);
	ND++;
	theD = theDaug;
      }

      int id = EvtPDL::getStdHep(theDaug->getId());
      if (id == 310) _multiplicity[6]++; //Ks
      if (id ==3122) _multiplicity[7]++; // Lambda or Lambdabar 
      makeTag(theDaug);    
    }
    //-- for root particle decays into only one unstable particle, the flag1 further tag the third decay
      if(ND ==1){
	int NDD= theD->getNDaug();
	for(int i=0;i<NDD;i++){
	  EvtParticle *theDD=theD->getDaug(i);
          if(theDD->getNDaug() !=0 ){
	    int ndd = theDD->getChannel();
	    _nmode.push_back(ndd);
	    break;
	  }
	}
      }
      //-------------------
  }

}

int EvtDecayTag::TagIndex(EvtId pid){
  /**
   * gamma : 0
   * e+/e- : 1
   * mu+/- : 2
   * pi+/- : 3
   * K+/-  : 4
   * p+/-  : 5
   * Ks    : 6
   * Lambda: 7
   * others: 8
   **/

 int id = EvtPDL::getStdHep(pid);
 int absid=fabs(id);
 if( absid == 22 )     {return 0;}  //photon
 else if(absid == 11 ) {return 1;} // electron
 else if(absid ==  13) {return 2;} // muon
 else if(absid == 211) {return 3;} // pion
 else if(absid == 321) {return 4;} // Kaon
 else if(absid ==2212) {return 5;} // pronton / anti-proton
 else if(absid == 310) {return 6;} // Ks
 else if(absid ==3122) {return 7;} // Lambda / Lambdabar
 else                  {return 8;}
}

int EvtDecayTag::getModeTag(){
  //get ConExc 74110 mode selected index
  std::vector<int> intFlag = root_par->getIntFlag();
  if(intFlag.size()>0){
    if(intFlag[0]==74110 || intFlag[0]==-100) return intFlag[1];
  }

  /**
   *  coding root_parent_mode * 10^9 + daug_0*10^6 + daug_1*10^3
   **/
  double seg0,seg1,seg2;

  int three   = 1000;
  int six     = 1000000;
  int themode = 1000000000;
  //---debugging
  // for(int im=0;im<_nmode.size();im++){std::cout<<"_nmode["<<im<<"]"<<_nmode[im]<<endl;;}

  if(_nmode.size()==1) {themode += _nmode[0]*six; return themode;}
  else if(_nmode.size() == 2){
    seg0 = _nmode[0]*six;
    seg1 = _nmode[1]*three;
    themode += seg0 + seg1;
    return themode;
  }
  else if(_nmode.size() >= 3){
    seg0 = _nmode[0]*six;
    seg1 = _nmode[1]*three;
    seg2 = _nmode[2];
    themode += seg0 + seg1 + seg2;
    return themode;
  }
}

int EvtDecayTag::getMultTag(){
  /**
   * 10 digit in the order: N_gamma, N_gamma, N_e,  N_mu,    N_pi,
   *                        N_Kaon,  N_p,     N_Ks, N_Lambda,DecayType
   **/
  int thetag = 0;
  for(int i=1; i<9; i++){
    if (_multiplicity[i] >9) return 0; 
  }
  for(int i=0; i<9; i++){
    int ndx = 8-i;
    int dig = pow(10,ndx);
    thetag += dig*_multiplicity[i];
  }
  return thetag;
}

int EvtDecayTag::getHdrdBase(int index){
  int ten = 10;
  int base;
  if( index ==0 || index>=100 && index <1000) { return index;}
  else {
    base = index*ten;
    getHdrdBase(base);
  }
}
