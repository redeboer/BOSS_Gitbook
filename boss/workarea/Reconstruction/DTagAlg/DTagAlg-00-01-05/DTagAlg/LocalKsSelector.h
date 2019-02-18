#ifndef LOCAL_KS_SELECTOR_H
#define LOCAL_KS_SELECTOR_H

#include "BesDChain/CDKs.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class LocalKsSelector : public DCSelectionFunction<CDKs>
{
   public :

      LocalKsSelector();

      bool operator() (CDKs& aKs);

   private :

      LocalKsSelector( const LocalKsSelector& );
      const LocalKsSelector& operator= ( const LocalKsSelector& );

      double m_minMass;
      double m_maxMass;
      double m_maxChisq;

      bool m_doSecondaryVFit;
      double m_maxVFitChisq;
      double m_minFlightSig;

};

extern LocalKsSelector ksSelector;

#endif
