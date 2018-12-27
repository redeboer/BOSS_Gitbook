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
// Module:  EvtDecayTag.hh
//
// Description:  Class to pick out the final state
//
// Modification history:
//
//    Ping R.-G.       December, 2011-Mar.       Module created
//
//------------------------------------------------------------------------
//

#include "EvtFSPick.hh"


void EvtFSPick::FSPick(EvtParticle* par){

  int ndaug = par->getNDaug();
  EvtId eid=par->getId();
  if(ndaug < 2 && EvtPDL::getStdHep(eid) != -22){
    _EvtFS.push_back(par);
    _fsid.push_back(eid);
    // std::cout<<"I get FS "<<EvtPDL::getStdHep(eid)<<std::endl;
  } else {
  
    int ND=0;
    EvtParticle *theD;
    for(int i=0;i<ndaug;i++){
      EvtParticle *theDaug = par->getDaug(i);
      FSPick(theDaug);    
    }
  }
}


void EvtFSPick::setFinalStatePar(std::vector <std::string> vnames){
  if(vnames.size()!= _EvtFS.size())
    {std::cout<<"EvtFSPick::The number in final state is inconsistent"<<std::endl;abort();}
  for (int i=0;i<vnames.size();i++){

    _vnames.push_back(vnames[i]);
    EvtId eid = EvtPDL::getId(std::string(vnames[i]));
    _vId.push_back(eid);

    //   std::cout<<"FS= "<<EvtPDL::getStdHep(_vId[i])<<std::endl;
  }
 
}



std::vector <EvtVector4R>  EvtFSPick::getP4(){
  EvtVector4R ptep;
  int nfs = _vnames.size();
  std::vector <EvtVector4R> vp4; 
  for(int i=0;i<nfs;i++) {
    int xid = getIndex(_vId[i]);
    ptep= _EvtFS[xid]->getP4();
    vp4.push_back(ptep);
  }
  return vp4;
} 

std::vector <EvtVector4R>  EvtFSPick::getP4Lab(){
  EvtVector4R ptep;
  int nfs = _vnames.size();
  std::vector <EvtVector4R> vp4; 
  for(int i=0;i<nfs;i++) {
    int xid = getIndex(_vId[i]);
    ptep= _EvtFS[xid]->getP4Lab();
    vp4.push_back(ptep);
  }
  return vp4;
} 

int EvtFSPick::getIndex(EvtId eid){
  for(int i=0;i<_vId.size();i++){
    EvtId pid = _fsid[i];
    if( eid == pid ){
      EvtId did = EvtPDL::getId("d");
      _fsid[i] = did;
      return i;
    } 
  }
    std::cout<<"EvtFSPick::Cannot find the particle "
             <<EvtPDL::getStdHep(eid) 
             <<" in the generated final state"<<std::endl;
    abort();
}
