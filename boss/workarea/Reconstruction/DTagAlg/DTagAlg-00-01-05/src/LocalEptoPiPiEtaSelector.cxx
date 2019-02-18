#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EventModel/EventModel.h"
#include "EventModel/EventHeader.h"
#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "ParticleID/ParticleID.h"
#include "DTagAlg/LocalEptoPiPiEtaSelector.h"

LocalEptoPiPiEtaSelector::LocalEptoPiPiEtaSelector()
{
  IJobOptionsSvc* jobSvc;
  Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);

  PropertyMgr m_propMgr;

  //Declare the properties
  m_propMgr.declareProperty("minMassEptoPiPiEta",               m_minMass=0.938);
  m_propMgr.declareProperty("maxMassEptoPiPiEta",               m_maxMass=0.978);
   
   
  jobSvc->setMyProperties("LocalEptoPiPiEtaSelector", &m_propMgr);
}

bool LocalEptoPiPiEtaSelector::operator() (CDDecay& aEp) {

  aEp.setUserTag(1);
  double mass = aEp.mass();
  if(mass >= m_minMass && mass<= m_maxMass)
    return true;
  else
    return false;
}


LocalEptoPiPiEtaSelector eptoPiPiEtaSelector;
