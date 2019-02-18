#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EvtRecEvent/EvtRecPi0.h"
#include "DTagAlg/LocalPi0Selector.h"

LocalPi0Selector::LocalPi0Selector()
{
   IJobOptionsSvc* jobSvc;
   Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);

   PropertyMgr m_propMgr;

   m_propMgr.declareProperty("Pi0MinMassCut",  m_minMass = 0.098 );
   m_propMgr.declareProperty("Pi0MaxMassCut",  m_maxMass = 0.165 );
   m_propMgr.declareProperty("Pi0MaxChisqCut", m_maxChisq = 2500 );
   

   jobSvc->setMyProperties("LocalPi0Selector", &m_propMgr);
}

bool LocalPi0Selector::operator() (CDPi0& aPi0) {

  aPi0.setUserTag(1);
  EvtRecPi0* pi0 = const_cast<EvtRecPi0*>( aPi0.navPi0() );
  
  double mass = pi0->unconMass();
  double chi2 = pi0->chisq();
  if ((mass <= m_minMass) || (mass >= m_maxMass) || (chi2 >= m_maxChisq)) return false;
  
  if(mass < 0.115 || mass > 0.150)
    aPi0.setUserTag(2);
  
  return true;
}

LocalPi0Selector pi0Selector;
