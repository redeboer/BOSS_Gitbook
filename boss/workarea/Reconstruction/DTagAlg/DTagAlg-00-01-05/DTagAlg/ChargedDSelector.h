#ifndef CHARGED_D_SELECTOR_H
#define CHARGED_D_SELECTOR_H

#include "BesDChain/CDDecay.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class ChargedDSelector : public DCSelectionFunction<CDDecay>
{
  public :
    
    ChargedDSelector();
  
    inline void setebeam(double ebeam) {m_ebeam=ebeam;}
    inline void setbeta(Hep3Vector beta) {m_beta=beta;}
    bool operator() (CDDecay& theD);

  private :
    
    ChargedDSelector( const ChargedDSelector& );
    const ChargedDSelector& operator= ( const ChargedDSelector& );
    
    double m_ebeam;
    Hep3Vector m_beta;

    bool m_useMbcCuts;
    bool m_useDeltaECuts;
    bool m_useDeltaMassCuts;

    double m_minMbc;
    double m_maxMbc;
    double m_minDeltaE;
    double m_maxDeltaE;
    double m_minDeltaMass;
    double m_maxDeltaMass;
    
};

extern ChargedDSelector chargedDSelector;

#endif
