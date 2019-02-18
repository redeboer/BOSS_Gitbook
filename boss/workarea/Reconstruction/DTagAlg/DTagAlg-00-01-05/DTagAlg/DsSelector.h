#ifndef DS_SELECTOR_H
#define DS_SELECTOR_H

#include "BesDChain/CDDecay.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class DsSelector : public DCSelectionFunction<CDDecay>
{
  public :
    
    DsSelector();
  
    inline void setebeam(double ebeam) {m_ebeam=ebeam;}
    inline void setbeta(Hep3Vector beta) {m_beta=beta;}
    bool operator() (CDDecay& theD);

  private :
    
    DsSelector( const DsSelector& );
    const DsSelector& operator= ( const DsSelector& );
    
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

extern DsSelector dsSelector;

#endif
