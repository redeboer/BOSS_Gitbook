#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EventModel/EventModel.h"
#include "EventModel/EventHeader.h"
#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "ParticleID/ParticleID.h"
#include "DTagAlg/LocalKaonSelector.h"

#include "VertexFit/IVertexDbSvc.h"
#include "VertexFit/VertexFit.h"
#include "VertexFit/Helix.h"

#include "SimplePIDSvc/ISimplePIDSvc.h"

LocalKaonSelector::LocalKaonSelector()
{
  m_pidtype=0;
  
  IJobOptionsSvc* jobSvc;
  Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);
  
  PropertyMgr m_propMgr;
  
  //Declare the properties
  m_propMgr.declareProperty("RxyCut",               m_VrCut           = 1.0);
  m_propMgr.declareProperty("Vz0Cut",               m_VzCut           = 10.0);
  m_propMgr.declareProperty("CosThetaCut",          m_CosThetaCut     = 0.93);
  
  m_propMgr.declareProperty("useSimplePID",         m_useSimplePID    = true);

  m_propMgr.declareProperty("PidUseDedx",           m_useDedx         = true);
  m_propMgr.declareProperty("PidUseTof1",           m_useTof1         = true);
  m_propMgr.declareProperty("PidUseTof2",           m_useTof2         = true);
  m_propMgr.declareProperty("PidUseTofE",           m_useTofE         = false);
  m_propMgr.declareProperty("PidUseTofQ",           m_useTofQ         = false);
  m_propMgr.declareProperty("PidUseEmc",            m_useEmc          = false);
  m_propMgr.declareProperty("PidUseMuc",            m_useMuc          = false);
  
  m_propMgr.declareProperty("PidwithProbability",   m_probability     = true);
  m_propMgr.declareProperty("PidProbCut",           m_PidProbCut      = 0.001);
  m_propMgr.declareProperty("RejectPion",           m_rejectPion      = true);
  m_propMgr.declareProperty("RejectProton",         m_rejectProton    = false);
  
  m_propMgr.declareProperty("PidwithLikelihood",    m_likelihood      = false);
  m_propMgr.declareProperty("PidwithNeuronNetwork", m_neuronNetwork   = false);
  m_neuronValCut.clear();
  m_neuronValCut.push_back(1.5);
  m_neuronValCut.push_back(2.5);
  m_propMgr.declareProperty("NeuronValue",      m_neuronValCut);
  
  jobSvc->setMyProperties("LocalKaonSelector", &m_propMgr);
}

bool LocalKaonSelector::operator() (CDChargedKaon& aKaon) {
  
  aKaon.setUserTag(1);
  EvtRecTrack* recTrk = const_cast<EvtRecTrack*>( aKaon.track() );
  
  // MDC track selection

  RecMdcKalTrack::setPidType(RecMdcKalTrack::kaon);
  if ( !recTrk->isMdcKalTrackValid() ) return false;
  RecMdcKalTrack* mdcKalTrk = recTrk->mdcKalTrack();
  if ( mdcKalTrk->charge()==0 ) return false;
  
  Hep3Vector xorigin(0,0,0);
  IVertexDbSvc*  vtxsvc;
  Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
  if(vtxsvc->isVertexValid()){
    double* dbv = vtxsvc->PrimaryVertex();
    double*  vv = vtxsvc->SigmaPrimaryVertex();
    xorigin.setX(dbv[0]);
    xorigin.setY(dbv[1]);
    xorigin.setZ(dbv[2]);
  }
  HepVector a = mdcKalTrk->getZHelixK();
  HepSymMatrix Ea = mdcKalTrk->getZErrorK();
  HepPoint3D point0(0.,0.,0.);
  HepPoint3D IP(xorigin[0],xorigin[1],xorigin[2]);
  VFHelix helixip3(point0,a,Ea);
  helixip3.pivot(IP);
  HepVector  vecipa = helixip3.a();
  
  double dr=fabs(vecipa[0]);
  double dz=fabs(vecipa[3]);
  double costheta=cos(mdcKalTrk->theta());
  if (  dr>= m_VrCut ) return false;
  if (  dz>= m_VzCut ) return false;
  if ( fabs(costheta) >= m_CosThetaCut ) return false;

  // Simple PID
  if(m_useSimplePID){
    ISimplePIDSvc* m_simplePIDSvc;
    Gaudi::svcLocator()->service("SimplePIDSvc", m_simplePIDSvc);
    
    m_simplePIDSvc->preparePID(recTrk);
    if( !m_simplePIDSvc->iskaon() ){
      aKaon.setUserTag(2);
    }  
  }
  else{
    //particleID pacakge
    ParticleID *pid = ParticleID::instance();
    pid->init();
    pid->setMethod(pid->methodProbability());
    pid->setChiMinCut(4);
    pid->setRecTrack(recTrk);
    pid->usePidSys(pid->useDedx() | pid->useTofCorr() );
    pid->identify(pid->onlyPion() | pid->onlyKaon());
    pid->calculate();
    
    if( !(pid->probKaon()>0 && pid->probKaon()>pid->probPion()) )
      aKaon.setUserTag(2);
    
  }
  

  
  if(m_pidtype==0) return true;
  
  if(aKaon.userTag()==1)
    return true;
  else
    return false;
  
}

LocalKaonSelector kaonSelector;
