#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EventModel/EventModel.h"
#include "EventModel/EventHeader.h"
#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "ParticleID/ParticleID.h"
#include "DTagAlg/LocalEtatoPiPiPi0Selector.h"

LocalEtatoPiPiPi0Selector::LocalEtatoPiPiPi0Selector()
{
  IJobOptionsSvc* jobSvc;
  Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);

  PropertyMgr m_propMgr;

  //Declare the properties
  m_propMgr.declareProperty("minMassEtatoPiPiPi0",               m_minMass=0.40);
  m_propMgr.declareProperty("maxMassEtatoPiPiPi0",               m_maxMass=0.70);
   
   
  jobSvc->setMyProperties("LocalEtatoPiPiPi0Selector", &m_propMgr);
}

bool LocalEtatoPiPiPi0Selector::operator() (CDDecay& aEta) {

  aEta.setUserTag(1);
  double mass = aEta.mass();
  if(mass >= m_minMass && mass<= m_maxMass)
    return true;
  else 
    return false;
}


LocalEtatoPiPiPi0Selector etatoPiPiPi0Selector;
