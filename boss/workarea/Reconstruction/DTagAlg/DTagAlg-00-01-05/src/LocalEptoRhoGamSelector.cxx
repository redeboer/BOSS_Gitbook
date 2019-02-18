#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EventModel/EventModel.h"
#include "EventModel/EventHeader.h"
#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "ParticleID/ParticleID.h"
#include "DTagAlg/LocalEptoRhoGamSelector.h"

LocalEptoRhoGamSelector::LocalEptoRhoGamSelector()
{
  IJobOptionsSvc* jobSvc;
  Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);

  PropertyMgr m_propMgr;

  //Declare the properties
  m_propMgr.declareProperty("minMassEptoRhoGam",               m_minMass = 0.938);
  m_propMgr.declareProperty("maxMassEptoRhoGam",               m_maxMass = 0.978);
   
   
  jobSvc->setMyProperties("LocalEptoRhoGamSelector", &m_propMgr);
}

bool LocalEptoRhoGamSelector::operator() (CDDecay& aEp) {

  aEp.setUserTag(1);
  double mass = aEp.mass();
  if(mass >= m_minMass && mass<= m_maxMass)
    return true;
  else
    return false;
}

LocalEptoRhoGamSelector eptoRhoGamSelector;
