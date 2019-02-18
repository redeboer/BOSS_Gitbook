#ifndef Reconstruction_NeutralDReconstruction_H
#define Reconstruction_NeutralDReconstruction_H 

#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/NTuple.h"

#include "DTagAlg/ReadBeamInfFromDb.h"
#include "EvtRecEvent/EvtRecDTag.h"

#include "BesDChain/CDChargedPionList.h"
#include "BesDChain/CDChargedKaonList.h"
#include "BesDChain/CDPi0List.h"
#include "BesDChain/CDKsList.h"
#include "BesDChain/CDDecayList.h"


#include <vector>
#include <string>

class NeutralDReconstruction : public Algorithm {

 public:
  NeutralDReconstruction(const std::string& name, ISvcLocator* pSvcLocator);
  StatusCode initialize();
  StatusCode execute();
  StatusCode finalize();  
  
  void saveD0Info(CDDecayList::iterator, double, int, int, EvtRecDTag*);
  void savetrack(vector<int>, vector<int>, EvtRecTrackIterator, EvtRecTrackIterator, EvtRecTrackIterator, EvtRecTrackIterator,EvtRecDTag*);
  void pidtag(vector<int>, vector<int>, CDChargedKaonList&, CDChargedPionList&,EvtRecDTag* );
  // HepLorentzVector vfit(string channel, vector<int> kaonid, vector<int> pionid, HepPoint3D vx, EvtRecTrackIterator charged_begin);
  vector<string> getlist(string& filename);
  
 private:
  StatusCode registerEvtRecDTagCol(EvtRecDTagCol* dtagCol, MsgStream& log);

 private:
 
   bool m_debug;
   
   int  m_irun;
   int  m_ievt;
   int  m_nChrg;
   int  m_nNeu;
   int  m_nPion;
   int  m_nKaon;
   int  m_nPi0;
   int  m_nKs;

   string m_decaylist;
   vector<string> chanlist;

   double m_beamE;
   Hep3Vector m_beta;
   bool m_ReadBeamEFromDB;
   bool m_usecalibBeamE;
   bool m_usevertexfit;
   ReadBeamInfFromDb m_readDb;
   
};
#endif
