#ifndef LOCAL_ETATOPIPIPI0_SELECTOR_H
#define LOCAL_ETATOPIPIPI0_SELECTOR_H

#include "BesDChain/CDDecay.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class LocalEtatoPiPiPi0Selector : public DCSelectionFunction<CDDecay>
{
 public :

  LocalEtatoPiPiPi0Selector();

  bool operator() (CDDecay& aEp);


 private :

  LocalEtatoPiPiPi0Selector( const LocalEtatoPiPiPi0Selector& );
  const LocalEtatoPiPiPi0Selector& operator= ( const LocalEtatoPiPiPi0Selector& );

  double m_minMass;
  double m_maxMass;

};

extern LocalEtatoPiPiPi0Selector etatoPiPiPi0Selector;

#endif
