#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EvtRecEvent/EvtRecEtaToGG.h"
#include "DTagAlg/LocalEtatoGGSelector.h"

LocalEtatoGGSelector::LocalEtatoGGSelector()
{
  IJobOptionsSvc* jobSvc;
  Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);

  PropertyMgr m_propMgr;

  m_propMgr.declareProperty("EtaMinMassCut",  m_minMass = 0.40 );
  m_propMgr.declareProperty("EtaMaxMassCut",  m_maxMass = 0.70 );
  m_propMgr.declareProperty("EtaMaxChisqCut", m_maxChisq = 2500 );
   

  jobSvc->setMyProperties("LocalEtatoGGSelector", &m_propMgr);
}

bool LocalEtatoGGSelector::operator() (CDEta& aEta) {

  aEta.setUserTag(1);
  EvtRecEtaToGG* eta = const_cast<EvtRecEtaToGG*>( aEta.navEta() );
  
  double mass = eta->unconMass();
  double chi2 = eta->chisq();
  if ((mass <= m_minMass) || (mass >= m_maxMass) || (chi2 >= m_maxChisq)) return false;
  
    
  return true;
}

LocalEtatoGGSelector etatoGGSelector;
