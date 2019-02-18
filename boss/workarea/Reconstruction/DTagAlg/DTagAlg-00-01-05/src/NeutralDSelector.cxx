#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "CLHEP/Vector/LorentzVector.h"
#include "DTagAlg/NeutralDSelector.h"


NeutralDSelector::NeutralDSelector( ){
  
  IJobOptionsSvc* jobSvc;
  Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);
  
  PropertyMgr m_propMgr;
  
  m_propMgr.declareProperty("UseMbcCuts",        m_useMbcCuts       = true );
  m_propMgr.declareProperty("UseDeltaECuts",     m_useDeltaECuts    = true );
  m_propMgr.declareProperty("UseDeltaMassCuts",  m_useDeltaMassCuts = false );

  m_propMgr.declareProperty("DMbcMinCut",        m_minMbc  = 1.83 );
  m_propMgr.declareProperty("DMbcMaxCut",        m_maxMbc  = 10.0 );
  m_propMgr.declareProperty("DDeltaEMinCut",     m_minDeltaE = -0.1 );
  m_propMgr.declareProperty("DDeltaEMaxCut",     m_maxDeltaE = +0.1 );
  m_propMgr.declareProperty("DDeltaMassMinCut",  m_minDeltaMass = -0.085 );
  m_propMgr.declareProperty("DDeltaMassMaxCut",  m_maxDeltaMass = +0.085 );
 
  jobSvc->setMyProperties("NeutralDSelector", &m_propMgr);
}

bool NeutralDSelector::operator() (CDDecay& theD) {
  
  /// Calculating mbc and deltaE
  HepLorentzVector p4( theD.momentum(), theD.energy() );
  p4.boost(-m_beta);
  double mbc2 = m_ebeam*m_ebeam - p4.v().mag2();
  double mbc = mbc2 > 0 ? sqrt( mbc2 ) : -10;
  double deltaE = p4.t() - m_ebeam;
 
  if(m_useMbcCuts&&((mbc <= m_minMbc)||(mbc >= m_maxMbc))) return false;
  if(m_useDeltaECuts&&((deltaE <= m_minDeltaE)||(deltaE >= m_maxDeltaE))) return false;

  /// Calculating invariant mass difference
  double neutralDMass = 1.8648;
  double deltaMass = theD.mass() - neutralDMass;

  if(m_useDeltaMassCuts &&
     ((deltaMass <= m_minDeltaMass)||(deltaMass >= m_maxDeltaMass))) return false;


  return true;
}


NeutralDSelector neutralDSelector;
