#ifndef LOCAL_PI0_SELECTOR_H
#define LOCAL_PI0_SELECTOR_H

#include "BesDChain/CDPi0.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class LocalPi0Selector : public DCSelectionFunction<CDPi0>
{
   public :

      LocalPi0Selector();

   bool operator() (CDPi0& aPi0);


   private :

      LocalPi0Selector( const LocalPi0Selector& );
      const LocalPi0Selector& operator= ( const LocalPi0Selector& );

      double m_minMass;
      double m_maxMass; 
      double m_maxChisq;

};

extern LocalPi0Selector pi0Selector;

#endif
