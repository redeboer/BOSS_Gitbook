#ifndef LOCAL_EPTORHOGAM_SELECTOR_H
#define LOCAL_EPTORHOGAM_SELECTOR_H

#include "BesDChain/CDDecay.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class LocalEptoRhoGamSelector : public DCSelectionFunction<CDDecay>
{
   public :

      LocalEptoRhoGamSelector();

      bool operator() (CDDecay& aEp);


   private :

      LocalEptoRhoGamSelector( const LocalEptoRhoGamSelector& );
      const LocalEptoRhoGamSelector& operator= ( const LocalEptoRhoGamSelector& );

      double m_minMass;
      double m_maxMass;
};

extern LocalEptoRhoGamSelector eptoRhoGamSelector;
#endif
