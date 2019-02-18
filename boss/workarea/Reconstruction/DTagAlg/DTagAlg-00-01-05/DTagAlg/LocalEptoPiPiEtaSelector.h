#ifndef LOCAL_EPTOPIPIETA_SELECTOR_H
#define LOCAL_EPTOPIPIETA_SELECTOR_H

#include "BesDChain/CDDecay.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class LocalEptoPiPiEtaSelector : public DCSelectionFunction<CDDecay>
{
   public :

      LocalEptoPiPiEtaSelector();

      bool operator() (CDDecay& aEp);


   private :

      LocalEptoPiPiEtaSelector( const LocalEptoPiPiEtaSelector& );
      const LocalEptoPiPiEtaSelector& operator= ( const LocalEptoPiPiEtaSelector& );

      double m_minMass;
      double m_maxMass;

};

extern LocalEptoPiPiEtaSelector eptoPiPiEtaSelector;

#endif
