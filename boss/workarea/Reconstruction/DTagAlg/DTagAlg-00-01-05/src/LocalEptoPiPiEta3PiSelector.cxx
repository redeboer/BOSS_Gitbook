#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EventModel/EventModel.h"
#include "EventModel/EventHeader.h"
#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "ParticleID/ParticleID.h"
#include "DTagAlg/LocalEptoPiPiEta3PiSelector.h"

LocalEptoPiPiEta3PiSelector::LocalEptoPiPiEta3PiSelector()
{
  IJobOptionsSvc* jobSvc;
  Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);

  PropertyMgr m_propMgr;

  //Declare the properties
  m_propMgr.declareProperty("minMassEptoPiPiEta3Pi", 							m_minMass = 0.938);
  m_propMgr.declareProperty("maxMassEptoPiPiEta3Pi",							 	m_maxMass = 0.978);
   
   
  jobSvc->setMyProperties("LocalEptoPiPiEta3PiSelector", &m_propMgr);
}

bool LocalEptoPiPiEta3PiSelector::operator() (CDDecay& aEp) {

  aEp.setUserTag(1);
  double mass = aEp.mass();
  if(mass >= m_minMass && mass<= m_maxMass)
    return true;
  else
    return false;
}


LocalEptoPiPiEta3PiSelector eptoPiPiEta3PiSelector;
