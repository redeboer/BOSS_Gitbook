#ifndef LOCAL_RHOTOPIPI_SELECTOR_H
#define LOCAL_RHOTOPIPI_SELECTOR_H

#include "BesDChain/CDDecay.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class LocalRhotoPiPiSelector : public DCSelectionFunction<CDDecay>
{
   public :

      LocalRhotoPiPiSelector();

      bool operator() (CDDecay& aRho);


   private :

      LocalRhotoPiPiSelector( const LocalRhotoPiPiSelector& );
      const LocalRhotoPiPiSelector& operator= ( const LocalRhotoPiPiSelector& );

      double m_minMass;
      double m_maxMass; 

};

extern LocalRhotoPiPiSelector rhotoPiPiSelector;

#endif
