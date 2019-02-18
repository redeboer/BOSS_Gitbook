#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EventModel/EventModel.h"
#include "EventModel/EventHeader.h"
#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "ParticleID/ParticleID.h"
#include "DTagAlg/LocalRhotoPiPiSelector.h"

LocalRhotoPiPiSelector::LocalRhotoPiPiSelector()
{
  IJobOptionsSvc* jobSvc;
  Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);

  PropertyMgr m_propMgr;

  //Declare the properties
  m_propMgr.declareProperty("minMassRhotoPiPi",               m_minMass = 0.5);
  m_propMgr.declareProperty("maxMassRhotoPiPi",               m_maxMass = 1.0);
   
  jobSvc->setMyProperties("LocalRhotoPiPiSelector", &m_propMgr);
}

bool LocalRhotoPiPiSelector::operator() (CDDecay& aRho) {

  aRho.setUserTag(1);
  double mass = aRho.mass();
  if( mass >= m_minMass && mass <= m_maxMass)
    return true;
  return false;
}

LocalRhotoPiPiSelector rhotoPiPiSelector;
