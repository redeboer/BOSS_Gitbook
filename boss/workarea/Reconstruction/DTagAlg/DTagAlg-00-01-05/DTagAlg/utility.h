#ifndef Utility_H
#define Utility_H

#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "EvtRecEvent/EvtRecVeeVertex.h"

#include "VertexFit/VertexFit.h"
#include "VertexFit/SecondVertexFit.h"
#include "VertexFit/IVertexDbSvc.h"

class utility {
  
 public:
  HepLorentzVector getp4(RecMdcKalTrack* mdcKalTrack, int pid);
  HepLorentzVector vfit(string channel, vector<int> kaonid, vector<int> pionid, HepPoint3D vx, EvtRecTrackIterator charged_begin);
  vector<double> SecondaryVFit(EvtRecVeeVertex* ks, IVertexDbSvc* vtxsvc);
  
};

#endif
